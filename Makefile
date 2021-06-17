#==== Main Options =============================================================
MCU = atmega1284p
PROGRAM_MCU = m1284p
F_CPU = 16000000
TARGET = monty

#==== Compile Options ==========================================================

CFLAGS = -mmcu=$(MCU) -I.
CFLAGS += -DF_CPU=$(F_CPU)UL
CFLAGS += -Os
#CFLAGS += -mint8
#CFLAGS += -mshort-calls
#CFLAGS += -funsigned-char
CFLAGS += -funsigned-bitfields
CFLAGS += -fpack-struct
CFLAGS += -fshort-enums
#CFLAGS += -fno-unit-at-a-time
CFLAGS += -Wall
#CFLAGS += -Wstrict-prototypes
CFLAGS += -Wundef
#CFLAGS += -Wunreachable-code
#CFLAGS += -Wsign-compare
#CFLAGS += -std=gnu99

#==== Programming Options (avrdude) ============================================

AVRDUDE_PROGRAMMER = pi2
#AVRDUDE_PORT = /dev/ttyUSB0
#AVRDUDE_BAUD = 19200

#AVRDUDE_FLAGS = -p $(PROGRAM_MCU) -P $(AVRDUDE_PORT) -b $(AVRDUDE_BAUD) -c $(AVRDUDE_PROGRAMMER)
AVRDUDE_FLAGS = -p $(PROGRAM_MCU) -c $(AVRDUDE_PROGRAMMER)

#===============================================================================
#==== Targets ==================================================================

CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
REMOVE = rm -f

all: build

build:
	$(CC) -c $(CFLAGS) $(TARGET).cpp -o $(TARGET).o
	$(CC) -c $(CFLAGS) max.cpp -o max.o
	$(CC) -c $(CFLAGS) led.cpp -o led.o
	$(CC) -c $(CFLAGS) tables.cpp -o tables.o
	$(CC) -c $(CFLAGS) devices.cpp -o devices.o
	$(CC) $(CFLAGS) $(TARGET).o led.o tables.o max.o devices.o --output $(TARGET).elf
	$(OBJCOPY) -O ihex -j .text -j .data $(TARGET).elf $(TARGET).hex

program:
	$(AVRDUDE) $(AVRDUDE_FLAGS) $(AVRDUDE_WRITE_FLASH) -U flash:w:$(TARGET).hex

clean:
	$(REMOVE) "$(TARGET).hex"
	$(REMOVE) "$(TARGET).elf"
	$(REMOVE) "$(TARGET).o"