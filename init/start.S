/**
 * file: start.S
 * created on: 2013-08-14
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

/* memory size: 64M */
.equ MEM_SIZE, 0x4000000
.equ MEM_BASE, 0x30000000

/* 4k stack for each mode, user and svc share the sp */
.equ _SVC_STACK, (MEM_BASE+MEM_SIZE-4)
.equ _IRQ_STACK, (_SVC_STACK-0x1000)
.equ _FIQ_STACK, (_IRQ_STACK-0x1000)
.equ _ABT_STACK, (_FIQ_STACK-0x1000)
.equ _UND_STACK, (_ABT_STACK-0x1000)
.equ _SYS_STACK, (_UND_STACK-0x1000)

.equ DISABLE_IRQ, 0x80
.equ DISABLE_FIQ, 0x40
.equ SYS_MODE,	  0x1f
.equ IRQ_MODE,    0x12
.equ FIQ_MODE,    0x11
.equ SVC_MODE,    0x13
.equ ABT_MODE,    0x17
.equ UND_MODE,    0x1b 

 .global _start
 _start: b start_code
	ldr pc, _vector_undefined_instruction
	ldr pc, _vector_software_interupt
	ldr pc, _vector_prefetch_abort
	ldr pc, _vector_data_abort
	ldr pc, _vector_reserved
	ldr pc, _vector_irq
	ldr pc, _vector_fiq

_vector_undefined_instruction: .word vector_undefined_instruction
_vector_software_interupt: .word vector_software_interupt
_vector_prefetch_abort: .word vector_prefetch_abort
_vector_data_abort: .word vector_data_abort
_vector_reserved: .word vector_reserved
_vector_irq: .word vector_irq
_vector_fiq: .word vector_fiq

start_code:
/* 1.set the cpu to SVC32 mode */
	mrs	r0, cpsr
	bic	r0, r0, #0x1f
	orr	r0, r0, #0xd3
	msr	cpsr, r0

/* 2. close watchdog */
#  define pWTCON	0x53000000
	ldr	r0, =pWTCON
	mov	r1, #0x0
	str r1, [r0]

/* 3. set clock */
#  define CLKDIVN	0x4C000014
	/* FCLK:HCLK:PCLK = 1:4:8 */
	ldr r0, =CLKDIVN
	mov r1, #0x5
	str r1, [r0]

	/* Change fast bus mode to asynchronous bus mode */
	mrc	p15, 0, r1, c1, c0, 0
	orr	r1, r1, #0xc0000000
	mcr	p15, 0, r1, c1, c0, 0

#   define S3C2440_MPLL_400MHZ     ((92<<12)|(1<<4)|(1))
#   define MPLLCON	0x4c000004
	ldr r0, =MPLLCON
	ldr r1, =S3C2440_MPLL_400MHZ
	str r1, [r0]

/* 4. set SDRAM */
#	define MEM_CTL_BASE		0x48000000
	ldr r0, =MEM_CTL_BASE
	adr r1, sdram_cfg_value
	add r3, r0, #(13*4)
1:
	ldr r2, [r1], #4
	str r2, [r0], #4
	cmp r0, r3
	bne 1b

/* 5. enable ICACHE */
	mrc p15, 0, r0, c1, c0, 0	@ read control reg
	orr r0, r0, #(1<<12)
	mcr	p15, 0, r0, c1, c0, 0   @ write it back

/* 6. set stack */
	msr cpsr, #(DISABLE_IRQ|DISABLE_FIQ|IRQ_MODE)
	ldr sp, = _IRQ_STACK

	msr cpsr, #(DISABLE_IRQ|DISABLE_FIQ|FIQ_MODE)
	ldr sp, = _FIQ_STACK

	msr cpsr, #(DISABLE_IRQ|DISABLE_FIQ|ABT_MODE)
	ldr sp, = _ABT_STACK

	msr cpsr, #(DISABLE_IRQ|DISABLE_FIQ|UND_MODE)
	ldr sp, = _UND_STACK

	msr cpsr, #(DISABLE_IRQ|DISABLE_FIQ|SYS_MODE)
	ldr sp, = _SYS_STACK

	msr cpsr, #(DISABLE_IRQ|DISABLE_FIQ|SVC_MODE)
	ldr sp, = _SVC_STACK

/* 7. relocate: copy bootloader code into linked address */
	bl nand_init

	mov r0, #0
	ldr r1, =_start
	ldr r2, =__bss_start
	sub r2, r2, r1
	bl copy_bootloader_to_sdram
	bl clear_bss

/* 8. execute main */
	ldr lr, =halt
	ldr pc, =main
halt:
	b halt


sdram_cfg_value:
        .long   0x2201D110      @ BWSCON
        .long   0x00000700      @ BANKCON0
        .long   0x00000700      @ BANKCON1
        .long   0x00000700      @ BANKCON2
        .long   0x00001F7C      @ BANKCON3
        .long   0x00000700      @ BANKCON4
        .long   0x00000700      @ BANKCON5
        .long   0x00018005      @ BANKCON6
        .long   0x00018005      @ BANKCON7
        .long   0x008C04F4      @ REFRESH
        .long   0x000000B1      @ BANKSIZE
        .long   0x00000030      @ MRSRB6
        .long   0x00000030      @ MRSRB7

#Fix me
.align 4
vector_undefined_instruction:
	nop
.align 4
vector_software_interupt:
	nop
.align 4
vector_prefetch_abort:
	nop
.align 4
vector_data_abort:
	nop
.align 4
vector_reserved:
	nop
.align 4
vector_irq:
	nop
.align 4
vector_fiq:
	nop

