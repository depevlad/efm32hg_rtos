all: temp.bin

CPU = -mcpu=cortex-m0 -mthumb
CFLAGS = -O -g -Wall #-fpie -mno-pic-data-is-text-relative -msingle-pic-base -mpic-register=r9
CC = ../../gcc-arm-none-eabi/bin/arm-none-eabi-gcc
OBJCPY = ../../gcc-arm-none-eabi/arm-none-eabi/bin/objcopy
SIZE = ../../gcc-arm-none-eabi/bin/arm-none-eabi-size

%.elf: %.o startup.o
	$(CC) $(CPU) $(CFLAGS) -T ../symbols.txt -nostdlib \
	    $^ -o $@ -Wl,-Map,$*.map
	$(SIZE) $@
	objdump -d -h $@
	ls -lah $@
	rm $*.map

%.o: %.c
	$(CC) --specs=nosys.specs $(CPU) $(CFLAGS) -c $< -o $@

%.bin: %.elf
	$(OBJCPY) -O binary $< $@

clean:
	rm -f *.bin *.elf *.map *.o
