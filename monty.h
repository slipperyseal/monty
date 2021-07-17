
#ifndef _MONTY_H
#define _MONTY_H

#define SID_VOICES                 3
#define TOTAL_SIDS                 2 // total SIDs in your Monty. valid values are 1 and 2
#define TOTAL_VOICES               (TOTAL_SIDS*SID_VOICES)
//#define DOM_MODE    // compile for Dom mode which will echo midi to UART1 and ignore half the channels
//#define GIMP_MODE   // compile for Gimp mode which read from UART1 and ignore half the channels
//todo: make this a runtime option, perhaps even self detect the gimp

#define SID_HZ 1000000

#define USART_BAUDRATE_0 31250 // MIDI baud rate 
#define BAUD_PRESCALE_0 (((F_CPU/(USART_BAUDRATE_0*16UL)))-1)

#define USART_BAUDRATE_1 31250
#define BAUD_PRESCALE_1 (((F_CPU/(USART_BAUDRATE_1*16UL)))-1)

#define FRAMES_PER_SECOND  32UL
#define ISR_COUNTER        (65536-(F_CPU/1024UL/FRAMES_PER_SECOND))

#define NO_OP16() asm volatile("nop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n\tnop\n"::);

// cli and sei replacements with questional taste in music
#define CLI()   const uint8_t dontStopBelievinHoldOnToThatFeelin = SREG; cli();
#define SEI()   SREG = dontStopBelievinHoldOnToThatFeelin;

#define VOICE_NOISE  128
#define VOICE_PULSE  64
#define VOICE_SAWTOOTH 32
#define VOICE_TRIANGLE 16
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

// PORT A
#define SID_CS_LEFT    (1<<7) // right high 
#define SID_CS_RIGHT   (1<<6) // left high 
#define SID_CS_BOTH    0      // both low
#define SID_CS_CLEAR   (SID_CS_LEFT | SID_CS_RIGHT) // both high
#define SID_RW_READ    (1<<5) // read/write line is connected to GPIO even though we only ever write

// PORTB
//#define LED_INVERT   // common cathode 7 seg displays will need the bits inverted
 
// PORT D
#define SID_RESET      (1<<1)
#define STATUS_PIN_0   (1<<2) // status pins when UART1 disabled
#define STATUS_PIN_1   (1<<3) 
#define BUTTON_1       (1<<4)
#define BUTTON_2       (1<<5)
#define BUTTON_3       (1<<6)
#define BUTTON_ALL     (BUTTON_1 | BUTTON_2 | BUTTON_3)

#define SEG_A  (1<<0)
#define SEG_B  (1<<1)
#define SEG_C  (1<<2)
#define SEG_D  (1<<3)
#define SEG_E  (1<<4)
#define SEG_F  (1<<5)
#define SEG_G  (1<<6)
#define SEG_DP (1<<7)

#define SYNTH_ALL_CHANNEL          0xff

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

class Instrument {
public:
    uint8_t control;          // waveform control register
    uint8_t filterFlags;      // filter flags
    uint16_t pulseWidth;      // initial pulse width
    uint8_t attackDecay;      // A/D
    uint8_t sustainRelease;   // S/R
    uint8_t sineAmplitude;
    uint8_t sineWidth;
    uint16_t freqCutoff;      // filter cutoff
    uint16_t resData;
    uint16_t resMode;
    uint16_t defaultModulation; // 0 = disable, [midi = chorus]
    uint8_t velocityFunction;
    
    void setDefaults();
};

class Voice {
public:
    uint8_t offset;         // offset to the 3 identical sets of voice registers (0, 7 or 14)
    uint8_t sidSelect;
    uint8_t key;
    uint8_t velocity;
    uint8_t sustain;        // sustain flag (voice wont be stopped until sustain cleared)

    void updateVoice();
    void setNoteOn(uint8_t key, uint8_t velocity);
    void setVoiceOff();
};

class Synth {
public:
    Instrument instrument;
    Voice voices[TOTAL_VOICES];
    uint16_t frame;
    uint16_t frequencyScan;
    uint16_t frequencyWidth;
    uint16_t reverb;
    uint8_t channel;     // MIDI channel to respond to or SYNTH_ALL_CHANNEL todo: make this a bitset
    uint8_t pitch;       // pitch wheel position
    uint8_t modulation;  // 64 = center
    uint8_t sustain;
    uint8_t volume;
    uint8_t nextVoice;

    Synth();
    void initSids();
    void writeSid(uint8_t reg, uint8_t val, uint8_t sidSelect);
    void setVolume(uint8_t volume);
    Voice * findVoice();
    void setNoteOff(uint8_t key);
    void setSustain();
    void releaseSustain();
    void setupVoices();
    uint8_t getVoiceOnBits();
    void updateVoices();
    void injectMidi();
    void playSample();

private:
    void sampleDelay();
};

class Uart0 {
public:
    Uart0();
    uint8_t read();
};

class Uart1 {
public:
    Uart1();
    uint8_t read();
    void write(uint8_t data);
};

class SidClock {
public:
    SidClock();
};

class SevenSeg {
public:
    SevenSeg();
    uint8_t getFontValue(uint8_t value);
    uint8_t scrollDown2(uint8_t value);
    uint8_t scrollDown1(uint8_t value);
    uint8_t scrollUp1(uint8_t value);
    uint8_t scrollUp2(uint8_t value);
    void update(uint8_t value);
    void updateFont(uint8_t value);
    void updateNumeric(uint8_t value);
};

class Button {
public:
    Button(uint8_t bitmask);

    uint8_t bit;
    bool down;
    bool changed;
    
    void poll();
    bool pressed();
    bool released();
};

class Knob {
public:
    uint8_t label;
    uint8_t* location;
    bool upperNibble;

    Knob(uint8_t la, uint8_t* lo, bool un) {
        label = la;
        location = lo;
        upperNibble = un;
    }
    uint8_t get();
    void set(uint8_t value);
};

class Menu {
public:
    SevenSeg sevenSeg;
    Button buttonA;
    Button buttonB;
    Button buttonC;
    Knob* knobs;
    uint8_t knobCount;
    uint8_t selectedKnob;
    bool edit;
    uint8_t flash;    
    uint16_t timeout;

    Menu();
    void update();
};

class Monty {
public:
    SidClock sidClock;
#ifdef GIMP_MODE
    Uart1 uartMidi;
#else
    Uart0 uartMidi;
#endif
#ifdef DOM_MODE
    Uart1 gimpMidi;
#endif
    Menu menu;
    Synth synth;

    Monty();
    void initIsr();
    void run();
};

#endif
