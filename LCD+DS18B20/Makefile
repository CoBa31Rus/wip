.PHONY: all clean

TARG=inkubator
SRCS= main.c lcd.c onewire.c pid.c eeprom.c button.c


CC = avr-gcc
OBJCOPY = avr-objcopy

OBJS = $(SRCS:.c=.o)
MCU=atmega8
CFLAGS = -mmcu=$(MCU) -std=gnu99 -DF_CPU=8000000UL -Wall -g -Os -Werror -lm  -mcall-prologues
LDFLAGS = -mmcu=$(MCU)  -Wall -g -Os  -Werror 

all: $(TARG) size mrproper

$(TARG): $(OBJS)
	$(CC) $(LDFLAGS) -o $@.elf  $(OBJS) -lm
	$(OBJCOPY) -O binary -R .eeprom -R .nwram  $@.elf $@.bin
	$(OBJCOPY) -O ihex -R .eeprom -R .nwram  $@.elf $@.hex

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

size: $(TARG).elf
	@echo
	@avr-size -C --mcu=$(MCU) $(TARG).elf
mrproper:
	rm -f *.o

clean:
	rm -f *.elf *.bin *.hex  $(OBJS) *.map