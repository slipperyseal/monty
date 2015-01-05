#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>

#include "monty.h"

// Monty SID synth for Raspberry Pi
//
// GPIO code is based on Jamie Nuttall's excelent SidPI project https://github.com/papawattu/SidPi
//
// compile with:     gcc monty.c -o monty -lm -lpthread -O3

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
struct sockaddr_in clientAddress;
struct sockaddr_in serverAddress;
pthread_t thread;
pthread_mutex_t lock;
struct Synth synth;

void iowrite32(unsigned long value, unsigned long * addr) {
    ((volatile unsigned long *)addr)[0] = value;
}

unsigned long ioread32(unsigned long * addr) {
    return ((volatile unsigned long *)addr)[0];
}

void delayNanos(long nanos) {
    struct timespec tim, tim2;
    tim.tv_sec = 0;
    tim.tv_nsec = nanos; // 1000 nanoseconds; 1 microsecond; cycle time of 1 Mhz
    nanosleep(&tim, &tim2);
}

void writeSid(int reg, int val) {
    val &= 0xff;
    iowrite32((unsigned long) addrPins[reg % 32], (unsigned long *) gpioPlease.addr + 7);
    iowrite32((unsigned long) ~addrPins[reg % 32] & addrPins[31], (unsigned long *) gpioPlease.addr + 10);
    iowrite32((unsigned long) 1 << CS, (unsigned long *) gpioPlease.addr + 10);
    iowrite32((unsigned long) dataPins[val % 256], (unsigned long *) gpioPlease.addr + 7);
    iowrite32((unsigned long) ~dataPins[val % 256] & dataPins[255], (unsigned long *) gpioPlease.addr + 10);
    delayNanos(1000);
    iowrite32((unsigned long) 1 << CS, (unsigned long *) gpioPlease.addr + 7);
    delayNanos(1000);
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
    int i;
    for (i = 0; i < 8; i++) {
        setPinOutput(DATA[i], 1);
    }
    for (i = 0; i < 5; i++) {
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

void mapPeripheral(struct bcm2835_peripheral * p, int blockSize) {
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

void unmapPeripheral(struct bcm2835_peripheral * p, int blockSize) {
    munmap(p->map, blockSize);
    close(p->mem_fd);
}

void sidReset() {
    int i;
    for (i = 0; i < 23; i++) {
        writeSid(i, 0);
    }
    writeSid(24, 15);
}

struct Voice * findVoice() {
    int x;
    if (++synth.nextVoice >= TOTAL_VOICES) {
        synth.nextVoice = 0;
    }

    for (x=0;x<TOTAL_VOICES;x++) {
        int v = synth.nextVoice + x;
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

int getSidFrequency(float key) { // MIDI note with floating point sub-note accuracy
    float freq = 8.1758 * powf(2,((key < 0 ? 0 : key)/12.0));
    int sid = (int)(16777216.0 / 985248.0) * freq; // 1022727 for 6567R8 VIC 6567R56A
    return sid > 0xffff ? 0xffff : sid;
}

void updateVoice(struct Voice * voice) {
    voice->frame++;

    float modKey = voice->key;
    if (synth.pitch) {
        modKey += (1.0/2048.0)*synth.pitch;
    }

    if (synth.frequencyScan) {
        modKey += (1.0 * (256.0 / (voice->frame & 0xff))) * (1.0 * (128.0/synth.frequencyScan));
    }

    int freq = getSidFrequency(modKey);
    writeSid(voice->offset + REGISTER_FREQ_LO, freq & 0xff);
    writeSid(voice->offset + REGISTER_FREQ_HI, (freq >> 8) & 0xff);
    if (synth.instrument.control & VOICE_PULSE) {
        int pw = ((synth.instrument.velocityFunction & VELOCITY_PULSEWIDTH) ? voice->velocity << 4 : voice->frame << 4);
        writeSid(voice->offset + REGISTER_PW_LO, pw & 0xff);
        writeSid(voice->offset + REGISTER_PW_HI, (pw >> 8) & 0xff);
    }
}

void setNoteOn(struct Voice * voice, int key, int velocity) {
    voice->key = key;
    voice->sustain = synth.sustain; // remember sustain state when key was pressed
    if (velocity < 50) {
        velocity = 50;
    }
    voice->velocity = velocity;
    voice->frame = 0;

    writeSid(voice->offset + REGISTER_AD,
            (synth.instrument.velocityFunction & VELOCITY_ATTACK) ?
                    (0xf - ((velocity>>3)<<4)) | (synth.instrument.attackDecay & 0x0f) :
                    synth.instrument.attackDecay );
    writeSid(voice->offset + REGISTER_SR,
            synth.instrument.sustainRelease);

    updateVoice(voice);

    writeSid(voice->offset + REGISTER_CONTROL, synth.instrument.control | VOICE_GATE);
}

void setVoiceOff(struct Voice * voice) {
    voice->velocity = 0;
    voice->sustain = 0;
    writeSid(voice->offset + REGISTER_CONTROL, synth.instrument.control & VOICE_CLOSEGATE);
}

void setNoteOff(int key) {
    int x;
    for (x=0;x<TOTAL_VOICES;x++) {
        struct Voice * voice = &synth.voiceTable[x];
        if (voice->key == key) {
            setVoiceOff(voice);
        }
    }
}

void setSustain() {
    int x;
    for (x=0;x<TOTAL_VOICES;x++) {
        struct Voice * voice = &synth.voiceTable[x];
        if (voice->velocity != 0) {
            voice->sustain = synth.sustain;
        }
    }
}

void releaseSustain() {
    int x;
    for (x=0;x<TOTAL_VOICES;x++) {
        struct Voice * voice = &synth.voiceTable[x];
        if (voice->sustain != 0 && voice->velocity != 0) {
            setVoiceOff(voice);
        }
    }
}

void setupSnyth() {
    int x;

    memset(&synth, 0, sizeof(synth));
    for (x=0;x<TOTAL_VOICES;x++) {
        struct Voice * voice = &synth.voiceTable[x];
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
    int x;

    // for development and debugging. remove this line for production
    printf("channel: %d func: %d note: %d velocity: %d\n", channel, func, data1, data2);

    if (channel != synth.channel) {
        return;
    }

    pthread_mutex_lock(&lock);
    switch (func) {
        case MIDI_NOTEOFF:
            setNoteOff(data1);
            break;
        case MIDI_NOTEON:
            if (data2 == 0) {
                setNoteOff(data1);
            } else if (data1 < 95) {
                setNoteOn(findVoice(), data1, data2);
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
                case MIDI_CONTROL_2:
                    synth.frequencyScan = data2;
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
            for (x=0;x<TOTAL_VOICES;x++) {
                updateVoice(&synth.voiceTable[x]);
            }
            break;
        case MIDI_PROGRAM:
            setupSnyth();
            sidReset();
            break;
    }
    pthread_mutex_unlock(&lock);
}

void * timerLoop() {
    for (;;) {
        delayNanos(100000); // 100th of a second
        pthread_mutex_lock(&lock);
        int x;
        for (x=0;x<TOTAL_VOICES;x++) {
            updateVoice(&synth.voiceTable[x]);
        }
        pthread_mutex_unlock(&lock);
    }
}

void readFromMidi(const char * device) {
    pthread_create(&thread, NULL, timerLoop, NULL);
    if (pthread_mutex_init(&lock, NULL) != 0) {
        exit(EXIT_FAILURE);
    }

    FILE * fp = fopen(device,"r");
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
    if (argc > 2) {
        mapPeripheral(&gpioPlease, GPIO_BLOCK_SIZE);
        mapPeripheral(&gpioClock, GPIO_CLOCK_BLOCK_SIZE);
        mapPeripheral(&gpioTimer, GPIO_TIMER_BLOCK_SIZE);

        generatePinTables();
        setPinsToOutput();
        startSidClock();
        sidReset();
        setupSnyth();

        if (strcmp(argv[1], "midi") == 0) {
            readFromMidi(argv[2]);
        }
        if (strcmp(argv[1], "socket") == 0) {
            readFromSocket(atoi(argv[2]));
        }
    }
    printf("usage:  sudo ./monty midi <midi device>     eg. /dev/snd/midiC1D0\n"
           "   or   sudo ./monty socket <port>          eg. 1111\n");
    exit(EXIT_FAILURE);
}


