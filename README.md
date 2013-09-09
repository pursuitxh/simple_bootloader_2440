simple_bootloader_2440
======================

a simple bootloader for jz2440, just used for demo how a bootloader works.

this bootloader can boot from nandflash or norflash,
Now we did not support kernel and filesystem downloading.

Install:
1. execute make, bootloader.bin will generate in out dir.
2. using j-link or open-jtag burn the bootloader.bin


clean:
make distclean
