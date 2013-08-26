/**
 * file: 	uart.c
 * Created on: 	2013-08-22
 * Author: 	pursuitxh
 * Email: 	pursuitxh@gmail.com
 */

#include "s3c2440.h"
#include "uart.h"

void putc(unsigned char c)
{
	while (!(UTRSTAT0 & TXD0READY));
	    UTXH0 = c;
}

void uart_queue(char *p,int len){
	while(*p&&len--){
		putc(*p++);
	};
}

void uart_init(void)
{
	GPHCON  |= 0xa0;
	GPHUP   = 0x0c;

	ULCON0  = 0x03;
	UCON0   = 0x05;
	UFCON0  = 0x00;
	UMCON0  = 0x00;
	UBRDIV0 = UART_BRD;

}
