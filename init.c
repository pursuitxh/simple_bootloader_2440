/**
 * file: init.c
 * created on: 2013-08-14
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

/* NandFlash Registers */
#define TACLS   0
#define TWRPH0  3
#define TWRPH1  0

#define NFCONF (*((volatile unsigned long *)0x4E000000))
#define NFCONT (*((volatile unsigned long *)0x4E000004))
#define NFCMMD (*((volatile unsigned char *)0x4E000008))
#define NFADDR (*((volatile unsigned char *)0x4E00000C))
#define NFDATA (*((volatile unsigned char *)0x4E000010))
#define NFSTAT (*((volatile unsigned char *)0x4E000020))


int norflash_boot(void)
{
	volatile unsigned int *p = (unsigned int *)0;
	unsigned int value;

	value = *p;
	*p = 0xdeadbeaf;
	if (0xdeadbeaf == *p) {
		*p = value; //restore value
		return 0;   //boot from nandflash
	} else {
		return 1;   //boot from norflash
	}

}

void bss_clear(void)
{
	extern int __bss_start, __bss_end;
	int *p = &__bss_start;

	for(; p < &__bss_end; p++)
		*p = 0;
}

void nand_init(void)
{
	/* set timing sequence */
	NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
	/* enable nandflash controller, ECC, disable chip select */
	NFCONT = (1<<4)|(1<<1)|(1<<0);
}

void delay(void)
{
	volatile int i;
	for(i = 0; i < 10; i++);
}

void nand_send_cmd(unsigned char cmd)
{
	NFCMMD = cmd;
	delay();
}

void nand_send_addr(unsigned int addr)
{
	unsigned int col = addr/2048;
	unsigned int page = addr%2048;

	NFADDR = col & 0xff;
	delay();
	NFADDR = (col >> 8) & 0xff;
	delay();
	NFADDR = page & 0xff;
	delay();
	NFADDR = (page >> 8) & 0xff;
	delay();
	NFADDR = (page >> 16) & 0xff;
	delay();
}

void nand_wait_ready(void)
{
	while(!(NFSTAT & 0x1));
}

unsigned char nand_read_data(void)
{
	return NFDATA;
}

void nand_select(void)
{
	NFCONT &= ~(1<<1);
}

void nand_deselect(void)
{
	NFCONT |= (1<<1);
}

void nand_read(unsigned int addr, unsigned char *buf, unsigned int len)
{
	int i = 0;
	int col = addr/2048;

	nand_select();
	while(i < len) {
		/* 1.send read command 0x00h */
		nand_send_cmd(0x00);

		/* 2.send address */
		nand_send_addr(addr);
		/* 3.send command 0x30h */
		nand_send_cmd(0x30);
		/* 4. wait status */
		nand_wait_ready();
		/* 5.read data */
		for(; (col < 2048) && (i < len); col++, i++) {
			buf[i] = nand_read_data();
			addr++;
		}
		col = 0;
	}

	nand_deselect();
}

void copy_bootloader_to_sdram(unsigned char *src, unsigned char *dst,
		unsigned int len)
{
	unsigned int i = 0;

	if (norflash_boot) {
		while (i < len) {
			*src++ = *dst++;
			i++;
		}
	} else {
		nand_read((unsigned int)src, dst, len);

	}
}
