
#ifndef _MONTY_H
#define _MONTY_H

#define BCM2708_PERI_BASE       0x20000000
#define GPIO_BASE               (BCM2708_PERI_BASE + 0x200000)
#define GPIO_TIMER              (BCM2708_PERI_BASE + 0x003000)
#define GPIO_CLOCK              (BCM2708_PERI_BASE + 0x00101000)
#define TIMER_OFFSET            (4)
#define GPIO_BLOCK_SIZE         (4*1024)
#define GPIO_CLOCK_BLOCK_SIZE   32
#define GPIO_TIMER_BLOCK_SIZE   64
#define BCM_PASSWORD            0x5A000000
#define GPIO_CLOCK_SOURCE       1
#define TIMER_CONTROL           (0x408 >> 2)
#define TIMER_IRQ_RAW           (0x410 >> 2)
#define TIMER_PRE_DIV           (0x41C >> 2)

#define VOICE_NOISE  128
#define VOICE_PULSE  64
#define VOICE_SAWTOOTH 32
#define VOICE_TRIANGE 16
#define VOICE_TEST 8 // not to be included in Instrument definitions
#define VOICE_RINGMOD 4
#define VOICE_SYNC 2
#define VOICE_GATE 1 // not to be included in Instrument definitions

#define VOICE_CLOSEGATE 254

// register groups offset at SID addresses 0,7 and 14
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

#define CS  18
#define RW  0
#define RES 0
#define CLK 4

#define SYNTH_KEY_CHANNEL          0
#define SYNTH_PAD_CHANNEL          1

#define TOTAL_VOICES               3

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

struct bcm2835_peripheral {
    unsigned long addr_p;
    int mem_fd;
    void * map;
    volatile unsigned int * addr;
};

struct Instrument {
    int control;          // waveform control register
    int filterFlags;	  // filter flags
    int pulseWidth;		  // inital pulse width
    int attackDecay;      // A/D
    int sustainRelease;   // S/R
    int freqCutoff;		  // filter cutoff
    int resData;
    int resMode;
    int defaultModulation; // 0 = disable, [midi = chorus]
    int velocityFunction;
};

struct Voice {
    int offset;         // offset to the 3 identicle sets of voice registers (0, 7 or 14)
    int key;
    int velocity;
    int sustain;        // sustain flag (voice wont be stopped until sustain cleared)
    int frame;		    // frame counter for progressive effects
};

struct Synth {
    int frame;
    int channel;     // the channel we are listening for
    int pitch;       // pitch wheel position
    int modulation;  // 64 = center
    int frequencyScan;
    int frequencyWidth;
    int reverb;
    int sustain;
    int volume;
    int nextVoice;
    struct Instrument instrument;
    struct Voice voiceTable[3];
};

#endif
