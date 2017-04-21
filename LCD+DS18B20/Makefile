.PHONY: all clean upload

TARG=inkubator
SRCS= main.c lcd.c onewire.c pid.c eeprom.c button.c


CC = avr-gcc
OBJCOPY = avr-objcopy

OBJS = $(SRCS:.c=.o)
MCU=atmega8
CFLAGS += -mmcu=$(MCU) -std=gnu99 -gdwarf-2 -DF_CPU=8000000UL -Os
LDFLAGS += -mmcu=$(MCU)

upload: $(TARG).hex
	avrdude -p$(MCU) -cbutterfly -b19200 -PCOM4 -Uflash:w:$(TARG).bin:a

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
