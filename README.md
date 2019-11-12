monty
=====

Commodore 64 Stereo 6581 SID Synth

Monty is a MIDI synth controller for the ATmega1284p which drives up to 2 6581 SID chips.

A demo of the previous Raspberry Pi powered version (this version is still in the commit history)..

[![Monty](http://img.youtube.com/vi/0jyIRRmpcOg/0.jpg)](http://www.youtube.com/watch?v=0jyIRRmpcOg)

Pin wirings...

~~~~
  These are the 328p pins. todo: update for 1284p..
  
  AVR SID

  PC0 A0
  PC1 A1
  PC2 A2
  PC3 A3
  PC4 A4
  PC5 RW (write low)
  PD0 serial rd (MIDI IN)
  PD1 serial wr
  PD2 D0
  PD3 D1
  PD4 D2
  PD5 D3
  PD6 D4
  PD7 D5
  PB0 RESET (active low)
  PB1 O2 SID 1mhz clock
  PB2 CS SID1 select (active low)
  PB3 CS SID2 select (active low)
  PB4 D6
  PB5 D7
~~~~

![Monty](http://kamome.slipperyseal.net/monty-pcb1.jpg "Monty")


Fuse settings. Only needs to be done once. Sets the AVR to use a 16mhz external crystal, and very importantly disable JTAG as we need those pins..

  `avrdude -p m1284p -c pi -U lfuse:w:0xff:m -U hfuse:w:0xd9:m`

Compiling...

  `avr-gcc -g -Os -mmcu=atmega1284p -c monty.cpp`
  
  `avr-gcc -g -mmcu=atmega1284p -o monty.elf monty.o` 

AVR Programming...

  `avrdude -p m1284p -c YOURPROGAMMER -U flash:w:monty.elf`

