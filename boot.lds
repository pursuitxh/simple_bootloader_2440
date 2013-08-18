/**
 * file: boot.lds
 * created on: 2013-08-14
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

ENTRY(_start)
SECTIONS {
    . = 0x33f80000;
    .text : { *(.text) }
    
    . = ALIGN(4);
    .rodata : {*(.rodata*)} 
    
    . = ALIGN(4);
    .data : { *(.data) }
    
    . = ALIGN(4);
    __bss_start = .;
    .bss : { *(.bss)  *(COMMON) }
    __bss_end = .;
}
