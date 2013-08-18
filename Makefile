# file: boot.c
# created on: 2013-08-18
# author: pursuitxh
# email:  pursuitxh@gmail.com

CC 		= arm-linux-gcc
LD 		= arm-linux-ld
OBJCOPY 	= arm-linux-objcopy
OBJDUMP		= arm-linux-objdump

CFLAGS		= -O2 -g
ASFLAGS		= -O2 -g
LDFLAGS		= -Tboot.lds
CPPFLAGS	= -nostartfiles -nostdlib

OBJS := start.o init.o boot.o


bootloader.bin: $(OBJS)
	$(LD) $(LDFLAGS) -o bootloader.elf $^
	$(OBJCOPY) -O binary -S bootloader.elf $@
	$(OBJDUMP) -D -m arm bootloader.elf > bootloader.dis

%.o:%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<
%.o:%.S
	$(CC) $(CPPFLAGS) $(ASFLAGS) -c -o $@ $<
	
clean:
	rm -f *.o *.bin *.elf *.dis
