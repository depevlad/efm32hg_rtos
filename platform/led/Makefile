all: led.bin

CPU = -mcpu=cortex-m0 -mthumb
CFLAGS = -O -g -Wall
CC = ../gcc-arm-none-eabi/bin/arm-none-eabi-gcc
OBJCPY = ../gcc-arm-none-eabi/arm-none-eabi/bin/objcopy
SIZE = ../gcc-arm-none-eabi/bin/arm-none-eabi-size

%.elf: %.o startup.o
	$(CC) $(CPU) $(CFLAGS) -T ../symbols.txt -nostdlib -lgcc \
	    $^ -o $@ -Wl,-Map,$*.map
	$(SIZE) $@
	objdump -d -syms $@
	rm $*.map

%.o: %.c
	$(CC) --specs=nosys.specs $(CPU) $(CFLAGS) -c $< -o $@

%.bin: %.elf
	$(OBJCPY) -O binary $< $@

clean:
	rm -f *.bin *.elf *.map *.o
