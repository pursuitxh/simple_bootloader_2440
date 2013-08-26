/**
 * file: nandflash.C
 * created on: 2013-08-20
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

#include "nandflash.h"
#include "s3c2440.h"

void nand_init(void)
{
	/* set timing sequence */
	NFCONF = (TACLS<<12)|(TWRPH0<<8)|(TWRPH1<<4);
	/* enable nandflash controller, ECC, disable chip select */
	NFCONT = (1<<4)|(1<<1)|(1<<0);


}

static void delay(void)
{
	volatile int i;
	for(i = 0; i < 10; i++);
}

static void nand_send_cmd(unsigned char cmd)
{
	NFCMMD = cmd;
	delay();
}

static void nand_send_addr(unsigned int addr)
{
	unsigned int col = addr%2048;
	unsigned int page = addr/2048;

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

static void nand_wait_ready(void)
{
	while(!(NFSTAT & 0x1));
}

static unsigned char nand_read_data(void)
{
	return NFDATA;
}

static void nand_select(void)
{
	NFCONT &= ~(1<<1);
}

static void nand_deselect(void)
{
	NFCONT |= (1<<1);
}

void nand_read(unsigned int addr, unsigned char *buf, unsigned int len)
{
	int i = 0;
	int col = addr%2048;

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
