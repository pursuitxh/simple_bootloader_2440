This is a simple bootloader for jz2440 development board.

How to write a simple bootloader? Here is the steps:
1. copy kernel from NandFlash into SDRAM
a. initialize hardware: close watchdog, set clock, set sdram, initializelize nandflash
b. if the size of bootloader is too big, we need to relocate it to sdram
c. read kernel from nandflash into sdram

2. boot the kernel
a. set the parameters which will pass to kernel
b. jumb to the entry address of kernel, and execute the kernel

references:
1. Weidongshan
2. S-Boot
3. Liwuyan  
