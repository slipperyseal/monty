# Monty

### Commodore 64 Stereo 6581 SID Synth

Monty is a MIDI synth controller for the ATmega1284p which drives up to 2 6581 SID chips.

The software receives MIDI messages via UART (serial), tracking "note on", "note off", "pitch bend" and other such
MIDI commands. Up to 2 SID chips can be controlled which totals 6 voices across 2 channels. Instrument definitions
decide which of the SIDs voice types to use, which include pulse wave (with variable duty cycle), triangle wave,
sawtooth wave, pseudorandom noise (white noise). The SID chips also contain a filter and ring modulator.
As was common the Commodore 64, software can then apply arpeggios and other register manipulations to create more
textured sounds.

By rapidly changing the volume level of the SID chip, sampled sound can be crudely played. On boot, Monty will
play a sample of Max Headroom asking "what kind of show is this anyway?"

Two Monty's each responding to alternate MIDI channels... 

[![Monty](https://img.youtube.com/vi/tqf2bS5UaQM/0.jpg)](https://www.youtube.com/watch?v=tqf2bS5UaQM)

https://www.youtube.com/watch?v=tqf2bS5UaQM

![Monty](http://kamome.slipperyseal.net/monty-pcb1.jpg "Monty")

Monty now has a chip tune built in!

SID chip tunes are 6502 processor code and binary data which actually ran on the Commodore 64.
But rather than try to squeeze a 6502 emulator on the AVR, I cross assembled the play routine and data.
I mapped 65 of the 6502s 151 instructions to AVR assembler, intercepted write the SID chip at $d4xx and handled one
piece of self modifiying code.  Hold the center button on boot to start playing the tune.

Jetpack 6502 to AVR cross assembler:

https://github.com/slipperyseal/jetpack

Monty playing `Monty On The Run` by Rob Hubbard.

[![Monty](https://img.youtube.com/vi/i0d1r9NZg9I/0.jpg)](https://www.youtube.com/watch?v=i0d1r9NZg9I)

https://www.youtube.com/watch?v=i0d1r9NZg9I

### Building

You'll need `AVR-GCC` and `avrdude` (or your preferred AVR programmer)  

#### Fuse settings

These only needs to be done once per AVR. This sets the AVR to use a 16mhz external crystal,
and very importantly disables JTAG as we need those pins..

  `avrdude -p m1284p -c YOURPROGAMMER -U lfuse:w:0xff:m -U hfuse:w:0xd9:m`

#### Compiling

  `make`

#### AVR Programming

  `avrdude -p m1284p -c YOURPROGAMMER -U flash:w:monty.hex`

## A brief history

About a million years ago there was a personal computer called the Commodore 64. One of its revolutionary features for
the time was a sound chip that was capable of more than simple beeps common in computers of the time.
The 6581 SID chip was a three voice analog synth with a range of selectable waveforms, multi mode filter and attack,
decay, sustain and release envelopes normally only found in professional synthesisers. The unique sound of this chip
has prompted the creation of highly accurate emulators, but if you are like me, you’ll agree nothing beats the real thing.
So at the turn of the century I was able to collect a bunch of SID chips from discarded Commodore 64s.
Over the years I prototyped various versions of a SID based MIDI synthesizer. A device which receives MIDI commands
and translates those the required register writes to a SID chip. 

My first prototype was C code compiled on a PC. Of course a PC doesn’t have a SID chip, so I relayed data to an
actual Commodore 64 using the PCs parallel port and an old joystick cable. A few lines of assembler code on the 64
reassembled two nibbles sent on the up, down, left and right lines while the fire button line was used as a timing clock.
This let me run a Commodore 64 emulator and a chip tune player that could drive a real SID.

I breadboarded a few microcontroller based solutions over the years, killing several SID chips in the process. More
recently a good friend of mine helped me gain EDA skills to fulfill this dream using a proper printed circuit board.
So while we were at it, we used two SID chips, for hard core 6 voice stereo. Now this hasn’t been an especially uncommon
thing to do. There are various projects around ranging from simple hacks to commercial products which do the same thing.
But this one is mine. I named it Monty, after the best game (with the best game music) ever created, Monty On The Run.

A demo of the previous Raspberry Pi powered version. This is receiving SID register data from a C64 emulator via TCP/IP.
This version is still in the commit history...

[![Monty](https://img.youtube.com/vi/0jyIRRmpcOg/0.jpg)](https://www.youtube.com/watch?v=0jyIRRmpcOg)

https://www.youtube.com/watch?v=0jyIRRmpcOg

## How it works

An optocoupler receives and buffers the MIDI serial signal.  The MCU running the whole show is an Atmel AVR.
I’ve had this working on 28 pin variants but I decided to go for the full 40 pin experience of the ATMega1284P.
This helped simplify the IO and allowed me to add a 7 segment LED display and some key switches.
So how do we control the SID chip itself? It’s pretty simple, luckily. The AVR and the SID use a standard 5 volt VCC.
The original 6581 also requires a 12 volt rail while the newer 8580 chips wants 9 volts. The synth is powered by a 12
volt PSU and I’ve added a 5 and 9 volt regulator. The SID runs on a 1mhz clock. The audio oscillators in the chip are
derived from this clock, so it’s important it’s exactly 1mhz. I run the AVR at 16mhz and program one of the GPIOs
to emit a 1mhz square wave. Simples. The chip has an 8 bit data bus, a 5 bit address bus, a read/write mode pin and a
chip select. Writes to the chip don’t need to be synced exactly to the 1mhz clock. As long as you write to the bus at
least as long as one SID clock cycle, it’s happy. The code on the AVR sets up the address and data bus values on its GPIO,
sets the chip select, waits with a few AVR No Op instructions, then clears the chip select. Of course both SIDs can
sit on the same bus and will respond to their respective chip select pins.

## The software

The main program loop listens for incoming MIDI commands from a UART. Voice objects represent the state of each of
the three voices in each SID.  When a “Note On” MIDI command is received, the Voice objects are searched for one which is
free. Should the tune have more polyphony (simultaneous notes playing) than voices, the oldest note playing is taken over.
Values from the instrument object are set up on the SID voice via the appropriate registers and a note trigger bit is set.
“Note Off” commands in turn mark the voice as available and trigger the voice to stop. Note that the SID chip has a proper
attack, decay, sustain and release envelope which is all managed by the chip. No need to do anything fancy to make that work.
Voice allocation is done round-robin which allows the voice’s envelope to ring out as long as possible.
A common trick with game music on the Commodore 64 was to change certain values in the SID as notes were playing.
Arpeggios were common (changing the notes of a single voice rapidly), pitch bends, even changing the waveform type itself.
These effects are handled by an interrupt which fires 50 times per second.

## What do the buttons do?

There is a simple memu system for editing voice properties (values in the range 0 to F).

```
Knob knobs[] = {
    {'A', &monty.synth.instrument.attackDecay, true },
    {'D', &monty.synth.instrument.attackDecay, false },
    {'S', &monty.synth.instrument.sustainRelease, true },
    {'R', &monty.synth.instrument.sustainRelease, false },
    {'H', &monty.synth.instrument.sineAmplitude, false },
    {'W', &monty.synth.instrument.sineWidth, false },
};
```

- Press center button: Letter flashes (The Knob).
- Move left and right through letters.
- Press center again to edit the value.
- Edit with left and right: 0 to F
- Center to return to letters.
- After a timeout Knob mode clears. 

Also, hold the centre button on boot to start playing the Monty On The Run chip tune.

## Project files for the PCB

![Monty](http://kamome.slipperyseal.net/monty-pcb.png "Monty PCB")

https://easyeda.com/slipperyseal/monty

## Notes about components and construction

- The 7809 (and its capacitor) are only required if you are going to use at least one 8580.
- You can mix 6581 and 8580 on the same board. Ensure the jumpers are set correctly for each.
Above the SIDs are 3 pin headers. Place a two pin jumper as per your SID or SIDs. Smoke may come out if they are wrong.
- Warning: left and right SIDs are "RIGHT and LEFT" on the board. This was to align with the rear view of the RCA sockets
(although they also should have probably been the other way around also).
- If you only have one SID put it in the socket marked LEFT and change TOTAL_SIDS to 1 in the source.
- The 7 Segment LED display needs to be common cathode (common ground).
- 7 seg LEDRx and LED_RES resistors should be chosen as appropriate your components. For Blue LEDs perhaps 470 ohm. For red, maybe 220 ohm.
- CapA and CapB for each SID are the filter capacitors and should also be chosen as per the SID type you are using.
- MIDI IN is a 5 pin din socket.
- The header in the bottom right is for the second UART. This can be used for chaining Montys or for any other reason you can think of having another UART.
- Depending on your programmer, it may cause some LED segments to go dark while connected. This is normal.
- Key switches are not mandatory to get it running.
- Use IC sockets everywhere!
- I would recommend, start by assembling the 7805 power section, the bottom section with the MCU, and the 7 seg. Test
this before putting a SID anywhere near it. 😄
- I have had reports that the drill holes for the power socket are not quite right. I have never open sourced a PCB before,
please bear with me.
- If there is no switch on your power socket I would suggest unplugging the PSU from the mains before inserting or removing
the plug, to reduce scratchy power spikes. Obviously double check the polarity of the power going to the socket.
Use a PSU which supplies a stable 12 Volts. Anything 500 milliamps or above should be fine.
- Support for both common anode and cathode 7 segment LEDs (add a jumper wire as appropriate under the 7 seg).
- I have  made the drill holes for the power socket a little bigger in the EasyEDA project.
I have had reports that the Gerber files of the current version have issues regarding the power socket:
The slots in the KO layer differ from the drill layer. I'm not sure how to explain this as in EasyEDA, you define
the hole and these layers should be based on that? ¯\\\_(ツ)\_/¯
