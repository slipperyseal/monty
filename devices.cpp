
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "monty.h"

extern const uint8_t font[128] PROGMEM;
extern Monty monty;

Uart0::Uart0() {
    UCSR0B |= 1<<RXEN0;     // read only
    UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01);
    UBRR0H  = (BAUD_PRESCALE_0 >> 8);
    UBRR0L  = BAUD_PRESCALE_0;
}

uint8_t Uart0::read() {
    while ((UCSR0A & (1 << RXC0)) == 0);

    uint8_t value = UDR0;
    if (monty.dom) {
        monty.gimpMidi.write(value);
    }
    return value;
}

Uart1::Uart1() {
    UCSR1B |= (1<<RXEN0) | (1<<TXEN0);  // read and write
    UCSR1C |= (1<<UCSZ10) | (1<<UCSZ11);
    UBRR1H  = (BAUD_PRESCALE_1 >> 8);
    UBRR1L  = BAUD_PRESCALE_1;
}

uint8_t Uart1::read() {
    while ((UCSR1A & (1<<RXC1)) == 0);
    return UDR1;
}

void Uart1::write(uint8_t data) {
    while (((UCSR1A & (1<<UDRE1)) == 0));
    UDR1=data;
}

SidClock::SidClock() {
    // SID 1mhz clock OSC 2 on PD7
    TCNT2 = 0;
    OCR2A = 7;
    TCCR2A = (1<<COM2A0) + (1<<WGM21);
    TCCR2B = (1<<CS20);
    DDRD |= (1<<7);
}

SevenSeg::SevenSeg() {
    DDRB = 0xFF;
}

uint8_t SevenSeg::getFontValue(uint8_t value) {
    return pgm_read_byte_near(&font[value]);
}

void SevenSeg::updateNumeric(uint8_t value) {
    value &= 0xf;
    updateFont((value < 0xa ? '0' : 'A'-0xa) + value);
}

void SevenSeg::updateFont(uint8_t value) {
    update(getFontValue(value));
}

uint8_t SevenSeg::scrollDown2(uint8_t value) {
    return (value&SEG_A)<<('D'-'A');
}

uint8_t SevenSeg::scrollDown1(uint8_t value) {
    return
    ((value&SEG_A)<<('G'-'A')) |
    ((value&SEG_G)>>('G'-'D')) |
    ((value&SEG_F)>>('F'-'E')) |
    ((value&SEG_B)<<('C'-'B'));
}

uint8_t SevenSeg::scrollUp1(uint8_t value) {
    return
    ((value&SEG_G)>>('G'-'A')) |
    ((value&SEG_D)<<('G'-'D')) |
    ((value&SEG_E)<<('F'-'E')) |
    ((value&SEG_C)>>('C'-'B'));
}

uint8_t SevenSeg::scrollUp2(uint8_t value) {
    return (value&SEG_D)>>('D'-'A');
}

void SevenSeg::update(uint8_t value) {
#ifdef LED_ANODE
    PORTB = ~value;
#else
    PORTB = value;
#endif
}

Button::Button(uint8_t bitmask) {
    this->bit = bitmask;
    DDRD &= ~bitmask;
    PORTD |= bitmask;
}

void Button::poll() {
    bool down = (PIND & this->bit) == 0;
    this->changed = down != this->down;
    this->down = down;
}

bool Button::pressed() {
    return this->changed && this->down;
}

bool Button::released() {
    return this->changed && !this->down;
}

uint8_t Knob::get() {
    return this->upperNibble ? *this->location >> 4 : *this->location & 0x0f;
}

void Knob::set(uint8_t value) {
    *this->location = this->upperNibble ?
        (*this->location & 0x0f) | value << 4 :
        (*this->location & 0xf0) | (value & 0x0f);
}

Menu::Menu() :
    buttonA(BUTTON_1), buttonB(BUTTON_2), buttonC(BUTTON_3) {
}

void Menu::update() {
    this->buttonA.poll();
    this->buttonB.poll();
    this->buttonC.poll();

    if (this->buttonB.down) {
        if (this->buttonA.pressed()) {
            monty.enableDom();
            monty.synth.setAllVoicesOff();
            return;
        } else if (this->buttonC.pressed()) {
            // todo: broken. regular mode will be blocking on the wrong UART
            monty.enableGimp();
            monty.synth.setAllVoicesOff();
            return;
        }
    }

    if (this->timeout == 0) {
        if (this->buttonA.pressed() || this->buttonB.pressed() || this->buttonC.pressed()) {
            // reset the timeout. ignore this first press.
            this->timeout = FRAMES_PER_SECOND*10;
        } else {
            // if menus are not being used (no buttons have been pressed in a while), mirror voice status to the 7 seg
            this->sevenSeg.update(monty.synth.getVoiceOnBits());
        }
        return;
    } else {
        this->timeout--;
    }

    Knob * knob = &knobs[this->selectedKnob];
    if (this->buttonB.pressed()) {
        this->edit = !this->edit;
    }
    if (this->edit) {
        uint8_t value = knob->get();
        if (this->buttonA.pressed() && value > 0) {
            knob->set(--value);
        }
        if (this->buttonC.pressed() && value < 0x0f) {
            knob->set(++value);
        }
        this->sevenSeg.updateNumeric(value);
    } else {
        if (this->buttonA.pressed() && this->selectedKnob > 0) {
            this->selectedKnob--;
        }
        if (this->buttonC.pressed() && this->selectedKnob < this->knobCount-1) {
            this->selectedKnob++;
        }
        this->sevenSeg.updateFont(this->flash++ & 0b00001000 ? 0 : knob->label);
    }
}
