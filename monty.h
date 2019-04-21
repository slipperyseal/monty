
#ifndef _MONTY_H
#define _MONTY_H

#define TOTAL_SIDS                 2 
#define TOTAL_VOICES               (TOTAL_SIDS*3)

#define NO_OP16() asm volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n"::);

#define USART_BAUDRATE 31250 // MIDI baud rate 
#define BAUD_PRESCALE (((F_CPU/(USART_BAUDRATE*16UL)))-1)

#define VOICE_NOISE  128
#define VOICE_PULSE  64
#define VOICE_SAWTOOTH 32
#define VOICE_TRIANGE 16
#define VOICE_TEST 8 // not to be included in Instrument definitions
#define VOICE_RINGMOD 4
#define VOICE_SYNC 2
#define VOICE_GATE 1 // not to be included in Instrument definitions
#define VOICE_CLOSEGATE 254

#define REGISTER_GROUP_OFFSET 7
#define REGISTER_FREQ_LO 0
#define REGISTER_FREQ_HI 1
#define REGISTER_PW_LO   2
#define REGISTER_PW_HI   3
#define REGISTER_CONTROL 4
#define REGISTER_AD      5
#define REGISTER_SR      6

#define REGISTER_VOLUME 24
#define VELOCITY_PULSEWIDTH 1
#define VELOCITY_ATTACK 2
#define PULSEWIDTH_VELOCITY 1
#define PULSEWIDTH_SWEEP 2

#define SID_HZ 1000000

#define SID_CS_LEFT    (1<<2) // right high 
#define SID_CS_RIGHT   (1<<3) // left high 
#define SID_CS_BOTH    0      // both low
#define SID_CS_CLEAR   SID_CS_LEFT | SID_CS_RIGHT // both high
#define SID_RW    31
#define SID_RESET 1

#define SYNTH_KEY_CHANNEL          0
#define SYNTH_PAD_CHANNEL          1

#define MIDI_COMMANDBIT            128
#define MIDI_NOTEOFF               8
#define MIDI_NOTEON                9
#define MIDI_POLYPHONICAFTERTOUCH  10
#define MIDI_CONTROL               11
#define MIDI_PROGRAM               12
#define MIDI_CHANNELAFTERTOUCH     13
#define MIDI_PITCHWHEEL            14
#define MIDI_SYSEX                 15

// standard MIDI
#define MIDI_CONTROL_BANKSELECT            0
#define MIDI_CONTROL_MODULATION            1
#define MIDI_CONTROL_BREATH                2
#define MIDI_CONTROL_FOOT                  4
#define MIDI_CONTROL_PORTAMENTOTIME        5
#define MIDI_CONTROL_VOLUME                7
#define MIDI_CONTROL_BALANCE               8
#define MIDI_CONTROL_PAN                   10
#define MIDI_CONTROL_SUSTAIN               64
#define MIDI_CONTROL_REVERB                91
// monty effects
#define MIDI_CONTROL_PULSEWIDTH            2
#define MIDI_CONTROL_FREQUENCY_SCAN        3
#define MIDI_CONTROL_FREQUENCY_WIDTH       4

struct Instrument {
    uint8_t control;          // waveform control register
    uint8_t filterFlags;	  // filter flags
    uint16_t pulseWidth;		  // inital pulse width
    uint8_t attackDecay;      // A/D
    uint8_t sustainRelease;   // S/R
    uint16_t freqCutoff;		  // filter cutoff
    uint16_t resData;
    uint16_t resMode;
    uint16_t defaultModulation; // 0 = disable, [midi = chorus]
    uint8_t velocityFunction;
};

struct Voice {
    uint8_t offset;         // offset to the 3 identicle sets of voice registers (0, 7 or 14)
    uint8_t sidSelect;
    uint8_t key;
    uint8_t velocity;
    uint8_t sustain;        // sustain flag (voice wont be stopped until sustain cleared)
};

struct Synth {
    struct Instrument instrument;
    struct Voice voiceTable[TOTAL_VOICES];
    uint16_t frame;
    uint16_t frequencyScan;
    uint16_t frequencyWidth;
    uint16_t reverb;
    uint8_t channel;     // the channel we are listening for
    uint8_t pitch;       // pitch wheel position
    uint8_t modulation;  // 64 = center
    uint8_t sustain;
    uint8_t volume;
    uint8_t nextVoice;
};

#endif
