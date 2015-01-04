#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

#include "monty.h"

// Monty SID synth for Raspberry Pi
//
// GPIO code is based on Jamie Nuttall's excelent SidPI project https://github.com/papawattu/SidPi
//
// compile with:     gcc monty.cpp -o monty -lm -O3

static unsigned char gpioToShift[] = { // shift for the 3 bits per pin in each GPFSEL port
    0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
    0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 0, 3, 6, 9, 12, 15, 18, 21, 24, 27
};

static const int DATA[] = { 11, 9, 10, 22, 27, 17, 3, 2 };
static const int ADDR[] = { 23, 24, 25, 8, 7 };

unsigned long dataPins[256];
unsigned long addrPins[32];
struct bcm2835_peripheral gpioPlease = {GPIO_BASE};
struct bcm2835_peripheral gpioClock = {GPIO_CLOCK};
struct bcm2835_peripheral gpioTimer = {GPIO_TIMER};
Synth synth;

void iowrite32(unsigned long value, unsigned long * addr) {
    ((volatile unsigned long *)addr)[0] = value;
}

unsigned long ioread32(unsigned long * addr) {
    return ((volatile unsigned long *)addr)[0];
}

void sidDelay() {
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = 1000; // 1000 nanoseconds; 1 microsecond; cycle time of 1 Mhz
    nanosleep(&tim, &tim2);
}

void writeSid(int reg, int val) {
    val &= 0xff;
    iowrite32((unsigned long) addrPins[reg % 32], (unsigned long *) gpioPlease.addr + 7);
    iowrite32((unsigned long) ~addrPins[reg % 32] & addrPins[31], (unsigned long *) gpioPlease.addr + 10);
    iowrite32((unsigned long) 1 << CS, (unsigned long *) gpioPlease.addr + 10);
    iowrite32((unsigned long) dataPins[val % 256], (unsigned long *) gpioPlease.addr + 7);
    iowrite32((unsigned long) ~dataPins[val % 256] & dataPins[255], (unsigned long *) gpioPlease.addr + 10);
    sidDelay();
    iowrite32((unsigned long) 1 << CS, (unsigned long *) gpioPlease.addr + 7);
    sidDelay();
}

void startSidClock() {
    int freq = 1000000; // The Commodore 64 ran at a blistering 1 Mhz
    int divi = 19200000 / freq;
    int divr = 19200000 % freq;
    int divf = (int) (divr * 4096 / 19200000);

    if (divi > 4095) {
        divi = 4095;
    }
    iowrite32(BCM_PASSWORD | GPIO_CLOCK_SOURCE, (unsigned long *) gpioClock.addr + 28);
    while ((ioread32((unsigned long *)gpioClock.addr + 28) & 0x80) != 0);

    iowrite32(BCM_PASSWORD | (divi << 12) | divf, (unsigned long *) gpioClock.addr + 29);
    iowrite32(BCM_PASSWORD | 0x10 | GPIO_CLOCK_SOURCE, (unsigned long *) gpioClock.addr + 28);
    iowrite32(0x0000280, (unsigned long *) gpioTimer.addr + TIMER_CONTROL);
    iowrite32(0x00000F9, (unsigned long *) gpioTimer.addr + TIMER_PRE_DIV);
}

void setPinOutput(int pin, int mode) {
    int fSel = pin/10;
    int shift = gpioToShift[pin];
    iowrite32(ioread32((unsigned long *) gpioPlease.addr + fSel) & ~(7 << shift) | (mode << shift), (unsigned long *) gpioPlease.addr + fSel);
}

void setPinsToOutput(void) {
    for (int i = 0; i < 8; i++) {
        setPinOutput(DATA[i], 1);
    }
    for (int i = 0; i < 5; i++) {
        setPinOutput(ADDR[i], 1);
    }
    setPinOutput(CS, 1);
    setPinOutput(RW, 1);
    setPinOutput(RES, 1);
    setPinOutput(CLK, 4);
}

void generatePinTables(void) {
    int i;

    for (i = 0; i < 256; i++) {
        dataPins[i] = (unsigned long) (i & 1) << DATA[0];
        dataPins[i] |= (unsigned long) ((i & 2) >> 1) << DATA[1];
        dataPins[i] |= (unsigned long) ((i & 4) >> 2) << DATA[2];
        dataPins[i] |= (unsigned long) ((i & 8) >> 3) << DATA[3];
        dataPins[i] |= (unsigned long) ((i & 16) >> 4) << DATA[4];
        dataPins[i] |= (unsigned long) ((i & 32) >> 5) << DATA[5];
        dataPins[i] |= (unsigned long) ((i & 64) >> 6) << DATA[6];
        dataPins[i] |= (unsigned long) ((i & 128) >> 7) << DATA[7];
    }

    for (i = 0; i < 32; i++) {
        addrPins[i] = (unsigned long) (i & 1) << ADDR[0];
        addrPins[i] |= (unsigned long) ((i & 2) >> 1) << ADDR[1];
        addrPins[i] |= (unsigned long) ((i & 4) >> 2) << ADDR[2];
        addrPins[i] |= (unsigned long) ((i & 8) >> 3) << ADDR[3];
        addrPins[i] |= (unsigned long) ((i & 16) >> 4) << ADDR[4];
    }
}

void mapPeripheral(struct bcm2835_peripheral *p, int blockSize) {
    if ((p->mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) {
        printf("Failed to open /dev/mem, try checking permissions.\n");
        exit(EXIT_FAILURE);
    }

    p->map = mmap(
        NULL,
        blockSize,
        PROT_READ|PROT_WRITE,
        MAP_SHARED,
        p->mem_fd,      // File descriptor to physical memory virtual file '/dev/mem'
        p->addr_p       // Address in physical map that we want this memory block to expose
    );

    if (p->map == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }

    p->addr = (volatile unsigned int *)p->map;
}

void unmapPeripheral(struct bcm2835_peripheral *p, int blockSize) {
    munmap(p->map, blockSize);
    close(p->mem_fd);
}

void sidReset() {
    for (int i = 0; i < 23; i++) {
        writeSid(i, 0);
    }
    writeSid(24, 15);
}

void setNoteOff(int key) {
    for (int x=0;x<TOTAL_VOICES;x++) {
        Voice * voice = &synth.voiceTable[x];
        if (voice->key == key) {
            voice->setVoiceOff();
        }
    }
}

void setSustain() {
    for (int x=0;x<TOTAL_VOICES;x++) {
        Voice * voice = &synth.voiceTable[x];
        if (voice->velocity != 0) {
            voice->sustain = synth.sustain;
        }
    }
}

void releaseSustain() {
    for (int x=0;x<TOTAL_VOICES;x++) {
        Voice * voice = &synth.voiceTable[x];
        if (voice->sustain != 0 && voice->velocity != 0) {
            voice->setVoiceOff();
        }
    }
}

Voice * findVoice() {
    if (++synth.nextVoice >= TOTAL_VOICES) {
        synth.nextVoice = 0;
    }

    for (int x=0;x<TOTAL_VOICES;x++) {
        int v = synth.nextVoice + x;
        if (v >= TOTAL_VOICES) {
            v -= TOTAL_VOICES;
        }
        Voice * voice = &synth.voiceTable[v];
        if (voice->velocity == 0) {
            return voice;
        }
    }
    return (Voice*)&synth.voiceTable[ synth.nextVoice ];
}

void Voice::setNoteOn(int key, int velocity) {
    // set up voice
    this->key = key;
    this->sustain = synth.sustain; // remember sustain state when key was pressed
    if (velocity < 50) {
        velocity = 50;
    }
    this->velocity = velocity;
    this->frame = 0;

    writeSid(this->offset + REGISTER_AD,
            (synth.instrument.velocityFunction & VELOCITY_ATTACK) ?
                    (0xf - ((velocity>>3)<<4)) | (synth.instrument.attackDecay & 0x0f) :
                    synth.instrument.attackDecay );
    writeSid(this->offset + REGISTER_SR,
            synth.instrument.sustainRelease);
    writeSid(this->offset + REGISTER_CONTROL, synth.instrument.control | VOICE_GATE);

    updateVoice();
}

int getSidFrequency(float key) { // MIDI note with floating point sub-note accuracy
    float freq = 8.1758 * powf(2,((key < 0 ? 0 : key)/12.0));
    int sid = (int)(16777216.0 / 985248.0) * freq; // 1022727 for 6567R8 VIC 6567R56A
    return sid > 0xffff ? 0 : sid;
}

void Voice::updateVoice() {
    this->frame++;

    float modKey = this->key;
    if (synth.pitch) {
        modKey += (1.0/2048.0)*synth.pitch;
    }

    int freq = getSidFrequency(modKey);
    int off = this->offset;

    writeSid(off + REGISTER_FREQ_LO, freq & 0xff);
    writeSid(off + REGISTER_FREQ_HI, (freq >> 8) & 0xff);
    if (synth.instrument.control & VOICE_PULSE) {
        int pw = ((synth.instrument.velocityFunction & VELOCITY_PULSEWIDTH) ? this->velocity << 4 : this->frame << 4);
        writeSid(off + REGISTER_PW_LO, pw & 0xff);
        writeSid(off + REGISTER_PW_HI, (pw >> 8) & 0xff);
    }
}

void Voice::setVoiceOff() {
    this->velocity = 0;
    this->sustain = 0;
    writeSid(this->offset+REGISTER_CONTROL, synth.instrument.control & VOICE_CLOSEGATE);
}

void setupSnyth() {
    memset(&synth, 0, sizeof(synth));

    for (int x=0;x<TOTAL_VOICES;x++) {
        Voice * voice = (Voice*)&synth.voiceTable[x];
        voice->offset = x * 7;
    }

    synth.instrument.control = VOICE_TRIANGE;
    synth.instrument.attackDecay = 0x7c;
    synth.instrument.sustainRelease = 0x33;

    synth.channel = 0;
    synth.volume = 15;
    synth.modulation = 64;
}

void injectMidi(int command, int data1, int data2) {
    int channel = command & 0x0f;
    int func = command >> 4;

    // for development and debugging. remove this line for production
    printf("channel: %d func: %d note: %d velocity: %d\n", channel, func, data1, data2);

    if (channel != synth.channel) {
        return;
    }
    switch (func) {
        case MIDI_NOTEOFF:
            setNoteOff(data1);
            break;
        case MIDI_NOTEON:
            if (data2 == 0) {
                setNoteOff(data1);
            } else if (data1 < 95) {
                findVoice()->setNoteOn(data1, data2);
            }
            break;
        case MIDI_CONTROL:
            switch (data1) {
                case 0:
                    sidReset();
                    return;
                case MIDI_CONTROL_MODULATION:
                    synth.modulation = data2;
                    break;
                case MIDI_CONTROL_VOLUME:
                    writeSid(REGISTER_VOLUME, (synth.volume = data2 >> 3) );  // 0xxxxxxx -> 0000xxxx
                    break;
                case MIDI_CONTROL_SUSTAIN:
                    if ((synth.sustain = data2) != 0) {
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
            synth.pitch = ((data1 | (data2 << 8)) - 16384);
            for (int x=0;x<TOTAL_VOICES;x++) {
                synth.voiceTable[x].updateVoice();
            }
            break;
        case MIDI_PROGRAM:
            setupSnyth();
            sidReset();
            break;
    }
}

void readFromMidi(const char * device) {
    FILE * fp = fopen("/dev/snd/midiC1D0","r");
    if( fp == NULL ) {
        perror("error opening MIDI device\n");
        exit(EXIT_FAILURE);
    }

    for (;;) {
        int command = fgetc(fp);
        if (command & MIDI_COMMANDBIT) {
            injectMidi(command, fgetc(fp), fgetc(fp));
        }
    }
}

struct sockaddr_in clientAddress;
struct sockaddr_in serverAddress;

void readFromSocket(int port) {
    int socketfd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketfd < 0) {
        printf("fail sockfd\n");
        return;
    }
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(socketfd, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printf("socket listener failed to bind\n");
        exit(EXIT_FAILURE);
    }

    listen(socketfd,5);
    socklen_t clilen = sizeof(clientAddress);
    socketfd = accept(socketfd, (struct sockaddr *) &clientAddress, &clilen);

    if (socketfd < 0) {
        printf("no FD\n");
        exit(EXIT_FAILURE);
    }

    printf("socket accepted\n");

    unsigned char buffer[2];
    for (; ;) {
        int n = read(socketfd, buffer, 2);
        if (n < 0) {
            perror("error reading from socket");
            exit(EXIT_FAILURE);
        }
        if (n == 1) {
            read(socketfd, &buffer[1], 1);
        }
        writeSid(buffer[0], buffer[1]);
    }
}

int main(int argc, char *argv[]) {
    mapPeripheral(&gpioPlease, GPIO_BLOCK_SIZE);
    mapPeripheral(&gpioClock, GPIO_CLOCK_BLOCK_SIZE);
    mapPeripheral(&gpioTimer, GPIO_TIMER_BLOCK_SIZE);

    generatePinTables();
    setPinsToOutput();
    startSidClock();
    sidReset();
    setupSnyth();

    if (argc > 2) {
        if (strcmp(argv[1], "midi") == 0) {
            readFromMidi(argv[2]);
        }
        if (strcmp(argv[1], "socket") == 0) {
            readFromSocket(atoi(argv[2]));
        }
    }
    printf("usage:  sudo ./monty midi <midi device>     eg. /dev/snd/midiC1D0\n");
    printf("   or   sudo ./monty socket <port>          eg. 1111\n");
    exit(EXIT_FAILURE);
}


