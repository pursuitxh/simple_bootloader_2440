/**
 * file: start.S
 * created on: 2013-08-14
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

 .gobal _start
 _start:

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
	add r3, #(13*4)
1:
	ldr r2, [r1], #4
	str r2, [r0], #4
	cmp r0, r3
	bne 1b

/* 5. relocate: copy bootloader code into linked address */
	@TODO:
	ldr sp, =0x34000000
	bl copy_code_to_sdram

/* 6. execute main */
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
