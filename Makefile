TARGET         = test4
CSRC           = test4.c
ASRC           = xitoa.S
MCU_TARGET     = at90s2313
OPTIMIZE       = -Os -mcall-prologues -mtiny-stack
DEFS           =
LIBS           =
DEBUG          = dwarf-2

CC             = avr-gcc
ASFLAGS        = -Wa,-adhlns=$(<:.S=.lst),-gstabs 
ALL_ASFLAGS    = -mmcu=$(MCU_TARGET) -I. -x assembler-with-cpp $(ASFLAGS)
CFLAGS         = -Wall -Wextra -g$(DEBUG) $(OPTIMIZE) -mmcu=$(MCU_TARGET) $(DEFS)
LDFLAGS        = -Wl,-Map,$(TARGET).map
OBJ            = $(CSRC:.c=.o) $(ASRC:.S=.o)

OBJCOPY        = avr-objcopy
OBJDUMP        = avr-objdump
SIZE           = avr-size



all: $(TARGET).elf lst text size

$(TARGET).elf: $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)


clean:
	rm -rf *.o $(TARGET).elf *.eps *.bak *.a
	rm -rf *.lst *.map $(EXTRA_CLEAN_FILES)
	rm -rf $(TARGET).hex

size: $(TARGET).elf
	$(SIZE) -C --mcu=$(MCU_TARGET) $(TARGET).elf

lst:  $(TARGET).lst
%.lst: %.elf
	$(OBJDUMP) -h -S $< > $@

%.o : %.S
	$(CC) -c $(ALL_ASFLAGS) $< -o $@



text: hex
hex:  $(TARGET).hex

%.hex: %.elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@


