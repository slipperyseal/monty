
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "monty.h"

extern const uint16_t sid_frequency[] PROGMEM;
extern const uint8_t sin_table[] PROGMEM;
extern const uint8_t max[7416] PROGMEM;

Monty monty;

Knob knobs[] = {
    {'A', &monty.synth.instrument.attackDecay, true },
    {'D', &monty.synth.instrument.attackDecay, false },
    {'S', &monty.synth.instrument.sustainRelease, true },
    {'R', &monty.synth.instrument.sustainRelease, false },
    {'H', &monty.synth.instrument.sineAmplitude, false },
    {'W', &monty.synth.instrument.sineWidth, false },
};

Synth::Synth() {
    DDRA = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;//|= SID_RESET; todo: fix why this didnt work

    this->initSids();

    this->frequencyScan = 200;
    this->channel = SYNTH_ALL_CHANNEL;
    this->volume = 0xf;
    setVolume(this->volume);

    this->setupVoices();
    this->instrument.setDefaults();

    this->playSample();
}

void Synth::writeSid(uint8_t reg, uint8_t val, uint8_t sidSelect) {
    reg &= 0b11111;

    PORTC = val;
    PORTA = reg | sidSelect;
    NO_OP16()
    PORTA = reg | SID_CS_CLEAR;
    NO_OP16()
}

void Synth::setVolume(uint8_t volume) {
    writeSid(REGISTER_VOLUME, volume, SID_CS_BOTH);
}

void Synth::initSids() {
    PORTA = SID_CS_CLEAR; // address and control bits
    PORTC = 0; // data bits

    PORTD |= SID_RESET;
    _delay_ms(300);
    PORTD &=~SID_RESET;
    _delay_ms(300);
    PORTD |= SID_RESET;
    _delay_ms(300);
}

Voice * Synth::findVoice() {
    if (++this->nextVoice >= TOTAL_VOICES) {
        this->nextVoice = 0;
    }

    // find an inactive voice
    for (uint8_t x=0;x<TOTAL_VOICES;x++) {
        uint8_t v = this->nextVoice + x;
        if (v >= TOTAL_VOICES) {
            v -= TOTAL_VOICES;
        }
        Voice * voice = &this->voices[v];
        if (voice->velocity == 0) {
            return voice;
        }
    }
    // no inactive voices, just override the oldest voice
    return &this->voices[ this->nextVoice ];
}

void Voice::updateVoice() {
    uint16_t note = this->key << 4;
    
    if (monty.synth.instrument.sineAmplitude) {
        uint8_t frame = monty.synth.frame;

        uint8_t w = frame << monty.synth.instrument.sineWidth;
        int8_t s = pgm_read_byte_near(&sin_table[w*4]);
        note += s >> monty.synth.instrument.sineAmplitude;
    }
    
    uint16_t freq = pgm_read_word_near(&sid_frequency[note]);

    monty.synth.writeSid(this->offset + REGISTER_FREQ_LO, freq & 0xff, this->sidSelect);
    monty.synth.writeSid(this->offset + REGISTER_FREQ_HI, (freq >> 8) & 0xff, this->sidSelect);

    if (monty.synth.instrument.control & VOICE_PULSE) {
        uint16_t pw = (monty.synth.instrument.pulseWidth+1) << 5; // 7 bit > 12 bit number
        monty.synth.writeSid(this->offset + REGISTER_PW_LO, pw & 0xff, this->sidSelect);
        monty.synth.writeSid(this->offset + REGISTER_PW_HI, (pw >> 8) & 0xff, this->sidSelect);
    }
}

void Voice::setNoteOn(uint8_t key, uint8_t velocity) {
    this->key = key;
    this->sustain = monty.synth.sustain; // remember sustain state when key was pressed
    if (velocity < 50) {
        velocity = 50;
    }
    this->velocity = velocity;

    cli();
    monty.synth.writeSid(this->offset + REGISTER_AD,
            (monty.synth.instrument.velocityFunction & VELOCITY_ATTACK) ?
                    (0xf - ((velocity>>3)<<4)) | (monty.synth.instrument.attackDecay & 0x0f) :
                    monty.synth.instrument.attackDecay, this->sidSelect );
    monty.synth.writeSid(this->offset + REGISTER_SR,
            monty.synth.instrument.sustainRelease, this->sidSelect);

    this->updateVoice();

    monty.synth.writeSid(this->offset + REGISTER_CONTROL, monty.synth.instrument.control | VOICE_GATE, this->sidSelect);
    sei();
}

void Voice::setVoiceOff() {
    this->velocity = 0;
    this->sustain = 0;
    monty.synth.writeSid(this->offset + REGISTER_CONTROL, monty.synth.instrument.control & VOICE_CLOSEGATE, this->sidSelect);
}

void Synth::setNoteOff(uint8_t key) {
    cli();
    for (uint8_t x=0;x<TOTAL_VOICES;x++) {
        Voice * voice = &this->voices[x];
        if (voice->key == key) {
            voice->setVoiceOff();
        }
    }
    sei();
}

void Synth::setSustain() {
    for (uint8_t x=0;x<TOTAL_VOICES;x++) {
        Voice * voice = &this->voices[x];
        if (voice->velocity != 0) {
            voice->sustain = monty.synth.sustain;
        }
    }
}

void Synth::releaseSustain() {
    for (uint8_t x=0;x<TOTAL_VOICES;x++) {
        Voice * voice = &this->voices[x];
        if (voice->sustain != 0 && voice->velocity != 0) {
            voice->setVoiceOff();
        }
    }
}

void Synth::setupVoices() {
    uint8_t i=0;
    for (uint8_t sidvoice=0;sidvoice<SID_VOICES;sidvoice++) {
        for (uint8_t sidchip=0;sidchip<TOTAL_SIDS;sidchip++) {
            Voice * voice = &this->voices[i++];
            voice->offset = sidvoice * REGISTER_GROUP_OFFSET;
            voice->sidSelect = sidchip == 0 ? SID_CS_LEFT : SID_CS_RIGHT;
        }
    }
}

uint8_t Synth::getVoiceOnBits() {
    uint8_t bits=0;
    for (uint8_t i=0;i<TOTAL_VOICES;i++) {
        if (this->voices[i].velocity != 0) {
            bits |= (1<<i);
        }
    }
    return bits;
}

void Instrument::setDefaults() {
    this->control = VOICE_TRIANGE;
    this->attackDecay = 0x2c;
    this->sustainRelease = 0x33;
    this->pulseWidth = 0xff;
}

void Synth::updateVoices() {
    for (uint8_t x=0;x<TOTAL_VOICES;x++) {
        voices[x].updateVoice();
    }
}

void Synth::injectMidi() {
    uint8_t command = monty.uartMidi.read();
    if (!(command & MIDI_COMMANDBIT)) {
        // out of sync with command. come back soon y'all
        return;
    }
    uint8_t data1 = monty.uartMidi.read();
    uint8_t data2;    
    uint8_t channel = command & 0x0f;
    uint8_t func = command >> 4;

    if (this->channel != SYNTH_ALL_CHANNEL && this->channel != channel) {
        return;
    }

    if (this->channel == SYNTH_ALL_CHANNEL) {
        // select a voice type based on the channel
        switch (channel & 0x3) {
            case 0:
                this->instrument.control = VOICE_SAWTOOTH;
                break;
            case 1:
                this->instrument.control = VOICE_TRIANGE;
                break;
            case 2:
                this->instrument.control = VOICE_SAWTOOTH;
                break;
            case 3:
                this->instrument.control = VOICE_TRIANGE;
                break;
        }
    }

    switch (func) {
        case MIDI_NOTEOFF:
            data2 = monty.uartMidi.read();
            setNoteOff(data1);
            break;
        case MIDI_NOTEON:
            data2 = monty.uartMidi.read();
            if (data2 == 0) {
                setNoteOff(data1);
            } else {
                Voice * voice = findVoice();
                voice->setNoteOn(data1, data2);
            }
            break;
        case MIDI_CONTROL:
            data2 = monty.uartMidi.read();
            switch (data1) {
                case MIDI_CONTROL_MODULATION:
                    this->modulation = data2;
                    break;
                case MIDI_CONTROL_FREQUENCY_SCAN:
                    this->frequencyScan = data2;
                    break;
                case MIDI_CONTROL_FREQUENCY_WIDTH:
                    this->frequencyWidth = data2;
                    break;
                case MIDI_CONTROL_PULSEWIDTH:
                    this->instrument.pulseWidth = data2;
                    break;
                case MIDI_CONTROL_VOLUME:
                    setVolume(this->volume = data2 >> 3);  // 7 to 4 bit volume
                    break;
                case MIDI_CONTROL_SUSTAIN:
                    if ((this->sustain = data2) == 0) {
                        setSustain();
                    } else {
                        releaseSustain();
                    }
                    break;
                case MIDI_CONTROL_REVERB:
                    this->reverb = data2;
                    break;
            }
            break;
        case MIDI_PITCHWHEEL:
            data2 = monty.uartMidi.read();
            this->pitch = ((data1 | (data2 << 8)) - 16384);
            updateVoices();
            break;
    }
}

void Synth::sampleDelay() {
    for (uint16_t x=0;x<94;x++) { 
        NO_OP16()
    }
}

void Synth::playSample() {
    for (uint16_t x=0;x<sizeof(max);x++) {
        uint8_t m = pgm_read_byte_near(max + x);
        writeSid(REGISTER_VOLUME, m & 0x0f, SID_CS_BOTH);
        sampleDelay();
        writeSid(REGISTER_VOLUME, (m >> 4) & 0x0f, SID_CS_BOTH);
        sampleDelay();
    }
    setVolume(0xf);
}

ISR (TIMER1_OVF_vect) {
     TCNT1 = ISR_COUNTER;

    PORTD |= STATUS_PIN_0;

    monty.synth.frame++;
    monty.synth.updateVoices();
    monty.menu.update();

    PORTD &=~STATUS_PIN_0;
}

Monty::Monty() {
    this->menu.knobs = knobs;
    this->menu.knobCount = (sizeof(knobs)/sizeof(Knob));

    this->initIsr();
    sei();
}

void Monty::initIsr() {
    TCNT1 = ISR_COUNTER;
    TCCR1A = 0x00;
    TCCR1B = (1<<CS10) | (1<<CS12);  // Timer mode with 1024 prescaler
    TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
}

void Monty::run() {
    for (;;) {
        this->synth.injectMidi();
    }
}

int main(int argc, char *argv[]) {
    monty.run();
}
