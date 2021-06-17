monty
=====

Commodore 64 Stereo 6581 SID Synth

Monty is a MIDI synth controller for the ATmega1284p which drives up to 2 6581 SID chips.

The software receives MIDI messages via UART (serial), tracking "note on", "note off", "pitch bend" and other such
MIDI commands. Up to 2 SID chips can be controlled which totals 6 voices across 2 channels. Instrument definitions
decide which of the SIDs voice types to use, which include pulse wave (with variable duty cycle), triangle wave,
sawtooth wave, pseudorandom noise (white noise). The SID chips also contain a filter and ring modulator.
As was common the Commodore 64, software can than apply arpeggios and other register manipulations to create more
textured sounds.

My rapidly changing the volume level of the SID chip, sampled sound can be crudely played. On boot, Monty will
play a sample of Max Headroom asking "what kind of show is this anyway?"

A demo of the previous Raspberry Pi powered version (this version is still in the commit history)..

[![Monty](http://img.youtube.com/vi/0jyIRRmpcOg/0.jpg)](http://www.youtube.com/watch?v=0jyIRRmpcOg)

![Monty](http://kamome.slipperyseal.net/monty-pcb1.jpg "Monty")

Fuse settings. These only needs to be done once. Sets the AVR to use a 16mhz external crystal, and very importantly disable JTAG as we need those pins..

  `avrdude -p m1284p -c YOURPROGAMMER -U lfuse:w:0xff:m -U hfuse:w:0xd9:m`

Compiling...

  `make`

AVR Programming...

  `avrdude -p m1284p -c YOURPROGAMMER -U flash:w:monty.hex`

