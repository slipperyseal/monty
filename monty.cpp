
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "monty.h"
#include "max.c"

Synth synth;

uint8_t readSerial() {
    while ((UCSR0A & (1 << RXC0)) == 0);
    return UDR0;
}

uint8_t readSerial2() {
    while ((UCSR1A & (1 << RXC1)) == 0);
    return UDR1;
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

void Synth::resetSid() {
    PORTA = SID_CS_CLEAR; // address and control bits
    PORTB = 0; // 7 segment display
    PORTC = 0; // data bits

    PORTD = SID_RESET | BUTTON_ALL;
    _delay_ms(1000);
    PORTD = BUTTON_ALL;
    _delay_ms(500);
    PORTD = SID_RESET | BUTTON_ALL;
    _delay_ms(500);

    setVolume(0xf);
}

Voice * Synth::findVoice() {
    if (++this->nextVoice >= TOTAL_VOICES) {
        this->nextVoice = 0;
    }

    // find an inactive voice
    uint8_t x;
    for (x=0;x<TOTAL_VOICES;x++) {
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

uint16_t Voice::getSidFrequency(float note) {
    float freq = 8.1758 * powf(2,((note < 0 ? 0 : note)/12.0));
    float result = (16777216.0 / 985248.0) * freq; // 1022727 for 6567R8 VIC 6567R56A
    return result > 65535.0 ? 0xffff : (uint16_t)result;
}

void Voice::updateVoice() {
    float note = this->key;
    if (synth.pitch) {
        note += (1.0/2048.0)*synth.pitch;
    }

    if (synth.frequencyScan) {
        float scan = (1.0/128.0) * synth.frequencyScan;
        float frame = (1.0/0xfff) * (synth.frame & 0xfff);
        float sine = sin( frame * 360.0 );
        note += (sine * (synth.frequencyWidth) * scan);
    }

    uint16_t freq = getSidFrequency(note);
    synth.writeSid(this->offset + REGISTER_FREQ_LO, freq & 0xff, this->sidSelect);
    synth.writeSid(this->offset + REGISTER_FREQ_HI, (freq >> 8) & 0xff, this->sidSelect);

    if (synth.instrument.control & VOICE_PULSE) {
        uint16_t pw = (synth.instrument.pulseWidth+1) << 5; // 7 bit > 12 bit number
        synth.writeSid(this->offset + REGISTER_PW_LO, pw & 0xff, this->sidSelect);
        synth.writeSid(this->offset + REGISTER_PW_HI, (pw >> 8) & 0xff, this->sidSelect);
    }
}

void Voice::setNoteOn(uint8_t key, uint8_t velocity) {
    this->key = key;
    this->sustain = synth.sustain; // remember sustain state when key was pressed
    if (velocity < 50) {
        velocity = 50;
    }
    this->velocity = velocity;

    synth.writeSid(this->offset + REGISTER_AD,
            (synth.instrument.velocityFunction & VELOCITY_ATTACK) ?
                    (0xf - ((velocity>>3)<<4)) | (synth.instrument.attackDecay & 0x0f) :
                    synth.instrument.attackDecay, this->sidSelect );
    synth.writeSid(this->offset + REGISTER_SR,
            synth.instrument.sustainRelease, this->sidSelect);

    this->updateVoice();

    synth.writeSid(this->offset + REGISTER_CONTROL, synth.instrument.control | VOICE_GATE, this->sidSelect);
}

void Voice::setVoiceOff() {
    this->velocity = 0;
    this->sustain = 0;
    synth.writeSid(this->offset + REGISTER_CONTROL, synth.instrument.control & VOICE_CLOSEGATE, this->sidSelect);
}

void Synth::setNoteOff(uint8_t key) {
    uint8_t x;
    for (x=0;x<TOTAL_VOICES;x++) {
        Voice * voice = &this->voices[x];
        if (voice->key == key) {
            voice->setVoiceOff();
        }
    }
}

void Synth::setSustain() {
    uint8_t x;
    for (x=0;x<TOTAL_VOICES;x++) {
        Voice * voice = &this->voices[x];
        if (voice->velocity != 0) {
            voice->sustain = synth.sustain;
        }
    }
}

void Synth::releaseSustain() {
    uint8_t x;
    for (x=0;x<TOTAL_VOICES;x++) {
        Voice * voice = &this->voices[x];
        if (voice->sustain != 0 && voice->velocity != 0) {
            voice->setVoiceOff();
        }
    }
}

void Synth::setupVoices() {
    uint8_t sidvoice,sidchip,i=0;
    for (sidvoice=0;sidvoice<SID_VOICES;sidvoice++) {
        for (sidchip=0;sidchip<TOTAL_SIDS;sidchip++) {
            Voice * voice = &this->voices[i++];
            voice->offset = sidvoice * REGISTER_GROUP_OFFSET;
            voice->sidSelect = sidchip == 0 ? SID_CS_LEFT : SID_CS_RIGHT;
        }
    }
}

void Instrument::setDefaults() {
    this->control = VOICE_TRIANGE;
    this->attackDecay = 0x2c;
    this->sustainRelease = 0x33;
    this->pulseWidth = 0xff;
}    

void Synth::setupSynth() {
    this->frequencyScan = 200;
    this->channel = SYNTH_ALL_CHANNEL;
    this->volume = 0xf;
    
    this->setupVoices();
    this->instrument.setDefaults();
}

void Synth::updateVoices() {
    uint8_t x;
    for (x=0;x<TOTAL_VOICES;x++) {
        voices[x].updateVoice();
    }
}

void Synth::injectMidi() {
    uint8_t command = readSerial();
    PORTB = command;
    if (!(command & MIDI_COMMANDBIT)) {
        return;
    }
    uint8_t data1 = readSerial();
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
            data2 = readSerial();
            setNoteOff(data1);
            break;
        case MIDI_NOTEON:
            data2 = readSerial();
            if (data2 == 0) {
                setNoteOff(data1);
            } else if (data1 < 95) {
                Voice * voice = findVoice();
                voice->setNoteOn(data1, data2);
            }
            break;
        case MIDI_CONTROL:
            data2 = readSerial();
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
                    setVolume(this->volume = data2 >> 3);  // 0xxxxxxx -> 0000xxxx
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
            data2 = readSerial();
            this->pitch = ((data1 | (data2 << 8)) - 16384);
            updateVoices();
            break;
    }
}

void sleepybobos() {
    // adjust loop counter for sample playback speed
    uint16_t x;
    for (x=0;x<94;x++) { 
        NO_OP16()
    }
}

void Synth::playSample() {
    uint16_t x;
    for (x=0;x<sizeof(max);x++) {
        uint8_t m = pgm_read_byte_near(max + x);
        writeSid(REGISTER_VOLUME, m & 0x0f, SID_CS_BOTH);
        sleepybobos();
        writeSid(REGISTER_VOLUME, (m >> 4) & 0x0f, SID_CS_BOTH);
        sleepybobos();
    }
    setVolume(0xf);
}

int main(int argc, char *argv[]) {
    DDRA = 0xFF;
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = ~BUTTON_ALL;
    PORTD = BUTTON_ALL;

    // Midi IN UART
    UCSR0B |= 1<<RXEN0;
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
    UBRR0H  = (BAUD_PRESCALE >> 8);
    UBRR0L  = BAUD_PRESCALE;

    // control port UART
    UCSR1B |= 1<<RXEN0;
    UCSR1C |= (1<<UCSZ10) | (1<<UCSZ11);
    UBRR1H  = (BAUD_PRESCALE2 >> 8);
    UBRR1L  = BAUD_PRESCALE2;

    // SID 1mhz clock OSC 2
    TCNT2 = 0;
    OCR2A = 7;
    TCCR2A = (1<<COM2A0) + (1<<WGM21);
    TCCR2B = (1<<CS20);

    synth.resetSid();
    synth.setupSynth();

    PORTB = 0xff;
    synth.playSample();
    PORTB = 0;

    for (;;) {
        synth.injectMidi();
    }
}
