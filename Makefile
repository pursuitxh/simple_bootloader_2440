# file: Makefile
# created on: 2013-08-18
# author: pursuitxh
# email:  pursuitxh@gmail.com

CROSS_COMPILE	?= arm-linux-

TOPDIR := $(shell pwd)
INCLUDEDIR := $(TOPDIR)/include

CC 		= $(CROSS_COMPILE)gcc
LD 		= $(CROSS_COMPILE)ld
OBJCOPY 	= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

CFLAGS		= -O2 -g
ASFLAGS		= -O2 -g
LDFLAGS		= -T$(TOPDIR)/init/boot.lds
CFLAGS	    += -nostartfiles -nostdlib -fno-builtin -I$(TOPDIR)/include

export CC LD OBJCOPY OBJDUMP CFLAGS ASFLAGS LDFLAGS
export TOPDIR

#obj-y += example.o
obj-y += init/
obj-y += driver/
obj-y += command/
#obj-y += shell/

TARGET := bootloader

all:
	make -f $(TOPDIR)/makefile.mk -C $(TOPDIR)
	$(LD) $(LDFLAGS) -o $(TARGET).elf built-in.o
	$(OBJCOPY) -O binary -S $(TARGET).elf $(TARGET) 
	$(OBJDUMP) -D -m arm $(TARGET).elf > $(TARGET).dis
	mkdir -p $(TOPDIR)/out
	mv $(TARGET) $(TARGET).elf $(TARGET).dis $(TOPDIR)/out/

clean:
	rm -f $(shell find -name "*.o") $(TARGET)
	rm -f $(shell find -name ".*.o.d")

distclean:
	rm -f $(shell find -name "*.o") $(TARGET)
	rm -f $(shell find -name ".*.o.d")
	rm -f $(shell find -name "*.elf")
	rm -f $(shell find -name "*.dis")
	rm -rf $(TOPDIR)/out
