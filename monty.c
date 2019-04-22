
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

struct Synth synth;

uint8_t readSerial() {
    while ((UCSR0A & (1 << RXC0)) == 0);
    return UDR0;
}

void writeSid(uint8_t reg, uint8_t val, uint8_t sidSelect) {
    PORTC = reg;   // A0-A4 + (write low)
    PORTD = val << 2;       // D0-D5
    uint8_t valueb = (val >> 2) & ((1<<4)|(1<<5)) | SID_RESET;
    PORTB = valueb | sidSelect;
    NO_OP16()
    PORTB = valueb | SID_CS_CLEAR;
}

void setVolume(uint8_t volume) {
    writeSid(REGISTER_VOLUME, volume, SID_CS_BOTH);
}

void sidReset() {
    DDRB = 0xFF;
    DDRC = 0xFF;
    DDRD = 0xFF;

    PORTB = SID_CS_CLEAR | SID_RESET;
    _delay_ms(100);
    PORTB = SID_CS_CLEAR;
    _delay_ms(100);
    PORTB = SID_CS_CLEAR | SID_RESET;
    _delay_ms(100);

    setVolume(0xf);
}

struct Voice * findVoice() {
    uint8_t x;
    if (++synth.nextVoice >= TOTAL_VOICES) {
        synth.nextVoice = 0;
    }

    for (x=0;x<TOTAL_VOICES;x++) {
        uint8_t v = synth.nextVoice + x;
        if (v >= TOTAL_VOICES) {
            v -= TOTAL_VOICES;
        }
        struct Voice * voice = &synth.voiceTable[v];
        if (voice->velocity == 0) {
            return voice;
        }
    }
    return &synth.voiceTable[ synth.nextVoice ];
}

uint16_t getSidFrequency(float note) {
    float freq = 8.1758 * powf(2,((note < 0 ? 0 : note)/12.0));
    float result = (16777216.0 / 985248.0) * freq; // 1022727 for 6567R8 VIC 6567R56A
    return result > 65535.0 ? 0xffff : (uint16_t)result;
}

void updateVoice(struct Voice * voice) {
    float note = voice->key;
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
    writeSid(voice->offset + REGISTER_FREQ_LO, freq & 0xff, voice->sidSelect);
    writeSid(voice->offset + REGISTER_FREQ_HI, (freq >> 8) & 0xff, voice->sidSelect);

    if (synth.instrument.control & VOICE_PULSE) {
        uint16_t pw = (synth.instrument.pulseWidth+1) << 5; // 7 bit > 12 bit number
        writeSid(voice->offset + REGISTER_PW_LO, pw & 0xff, voice->sidSelect);
        writeSid(voice->offset + REGISTER_PW_HI, (pw >> 8) & 0xff, voice->sidSelect);
    }
}

void setNoteOn(struct Voice * voice, uint8_t key, uint8_t velocity) {
    voice->key = key;
    voice->sustain = synth.sustain; // remember sustain state when key was pressed
    if (velocity < 50) {
        velocity = 50;
    }
    voice->velocity = velocity;

    writeSid(voice->offset + REGISTER_AD,
            (synth.instrument.velocityFunction & VELOCITY_ATTACK) ?
                    (0xf - ((velocity>>3)<<4)) | (synth.instrument.attackDecay & 0x0f) :
                    synth.instrument.attackDecay, voice->sidSelect );
    writeSid(voice->offset + REGISTER_SR,
            synth.instrument.sustainRelease, voice->sidSelect);

    updateVoice(voice);

    writeSid(voice->offset + REGISTER_CONTROL, synth.instrument.control | VOICE_GATE, voice->sidSelect);
}

void setVoiceOff(struct Voice * voice) {
    voice->velocity = 0;
    voice->sustain = 0;
    writeSid(voice->offset + REGISTER_CONTROL, synth.instrument.control & VOICE_CLOSEGATE, voice->sidSelect);
}

void setNoteOff(uint8_t key) {
    uint8_t x;
    for (x=0;x<TOTAL_VOICES;x++) {
        struct Voice * voice = &synth.voiceTable[x];
        if (voice->key == key) {
            setVoiceOff(voice);
        }
    }
}

void setSustain() {
    uint8_t x;
    for (x=0;x<TOTAL_VOICES;x++) {
        struct Voice * voice = &synth.voiceTable[x];
        if (voice->velocity != 0) {
            voice->sustain = synth.sustain;
        }
    }
}

void releaseSustain() {
    uint8_t x;
    for (x=0;x<TOTAL_VOICES;x++) {
        struct Voice * voice = &synth.voiceTable[x];
        if (voice->sustain != 0 && voice->velocity != 0) {
            setVoiceOff(voice);
        }
    }
}

void setupSnyth() {
    uint8_t x,y,i=0;

    memset(&synth, 0, sizeof(synth));
    for (x=0;x<3;x++) {
        for (y=0;y<TOTAL_SIDS;y++) {
            struct Voice * voice = &synth.voiceTable[i++];
            voice->offset = x * REGISTER_GROUP_OFFSET;
            voice->sidSelect = y == 0 ? SID_CS_LEFT : SID_CS_RIGHT;
        }
    }

    synth.instrument.control = VOICE_TRIANGE;
    synth.instrument.attackDecay = 0x2c;
    synth.instrument.sustainRelease = 0x33;
    synth.instrument.pulseWidth = 0xff;

    synth.frequencyScan = 200;
    synth.channel = SYNTH_KEY_CHANNEL;
    synth.volume = 0xf;
}

void * updateVoices() {
    uint8_t x;
    for (x=0;x<TOTAL_VOICES;x++) {
        updateVoice(&synth.voiceTable[x]);
    }
}

void injectMidi() {
    uint8_t command = readSerial();
    if (!(command & MIDI_COMMANDBIT)) {
        return;
    }
    uint8_t data1 = readSerial();
    uint8_t data2;    
    uint8_t channel = command & 0x0f;
    uint8_t func = command >> 4;

// respond to all channels for now
//    if (channel != synth.channel) {
//        return;
//    }

    // select a voice type based on the channel
    switch (channel & 0x3) {
        case 0:
            synth.instrument.control = VOICE_SAWTOOTH;
            break;
        case 1:
            synth.instrument.control = VOICE_TRIANGE;
            break;
        case 2:
            synth.instrument.control = VOICE_SAWTOOTH;
            break;
        case 3:
            synth.instrument.control = VOICE_TRIANGE;
            break;
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
                setNoteOn(findVoice(), data1, data2);
            }
            break;
        case MIDI_CONTROL:
            data2 = readSerial();
            switch (data1) {
                case MIDI_CONTROL_MODULATION:
                    synth.modulation = data2;
                    break;
                case MIDI_CONTROL_FREQUENCY_SCAN:
                    synth.frequencyScan = data2;
                    break;
                case MIDI_CONTROL_FREQUENCY_WIDTH:
                    synth.frequencyWidth = data2;
                    break;
                case MIDI_CONTROL_PULSEWIDTH:
                    synth.instrument.pulseWidth = data2;
                    break;
                case MIDI_CONTROL_VOLUME:
                    setVolume(synth.volume = data2 >> 3);  // 0xxxxxxx -> 0000xxxx
                    break;
                case MIDI_CONTROL_SUSTAIN:
                    if ((synth.sustain = data2) == 0) {
                        setSustain();
                    } else {
                        releaseSustain();
                    }
                    break;
                case MIDI_CONTROL_REVERB:
                    synth.reverb = data2;
                    break;
            }
            break;
        case MIDI_PITCHWHEEL:
            data2 = readSerial();
            synth.pitch = ((data1 | (data2 << 8)) - 16384);
            updateVoices();
            break;
    }
}

// too slow at the moment, not implemented
ISR (TIMER0_COMPA_vect) {
    synth.frame++;
    updateVoices();
}

void playSample() {
    uint16_t x,y;
    for (x=0;x<sizeof(max);x++) {
        uint8_t m = pgm_read_byte_near(max + x);
        writeSid(REGISTER_VOLUME, m & 0x0f, SID_CS_BOTH);
        for (y=0;y<94;y++) {
            NO_OP16()
        }
        writeSid(REGISTER_VOLUME, (m >> 4) & 0x0f, SID_CS_BOTH);
        for (y=0;y<94;y++) {
            NO_OP16()
        }
    }
    setVolume(0xf);
}

int main(int argc, char *argv[]) {
    // UART
    UCSR0B |= (1<<RXEN0)  | (1<<TXEN0);
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
    UBRR0H  = (BAUD_PRESCALE >> 8);
    UBRR0L  = BAUD_PRESCALE;

    // 1mhz clock
    ICR1 = 8;
    TCNT1 = 0;
    OCR1A = 4;
    TCCR1A = (1<<COM1A1) + (1<<WGM11);
    TCCR1B = (1<<WGM13) + (1<<CS10);

    sidReset();
    setupSnyth();

    // voice update handler
    // too slow at the moment, not implemented
//    TCCR0A |= (1<<WGM01);
//    OCR0A = 0xF9;
//    TIMSK0 |= (1 << OCIE0A);
//    sei();
//    TCCR0B |= (1<<CS02);

    playSample();
    for (;;) {
        injectMidi();
    }
}
