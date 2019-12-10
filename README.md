monty
=====

Commodore 64 Stereo 6581 SID Synth

Monty is a MIDI synth controller for the ATmega1284p which drives up to 2 6581 SID chips.

A demo of the previous Raspberry Pi powered version (this version is still in the commit history)..

[![Monty](http://img.youtube.com/vi/0jyIRRmpcOg/0.jpg)](http://www.youtube.com/watch?v=0jyIRRmpcOg)

![Monty](http://kamome.slipperyseal.net/monty-pcb1.jpg "Monty")

Fuse settings. These only needs to be done once. Sets the AVR to use a 16mhz external crystal, and very importantly disable JTAG as we need those pins..

  `avrdude -p m1284p -c pi -U lfuse:w:0xff:m -U hfuse:w:0xd9:m`

Compiling...

  `avr-gcc -g -Os -mmcu=atmega1284p -c monty.cpp`
  
  `avr-gcc -g -mmcu=atmega1284p -o monty.elf monty.o` 

AVR Programming...

  `avrdude -p m1284p -c YOURPROGAMMER -U flash:w:monty.elf`

