/**
 * file: main.c
 * created on: 2013-08-23
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

#include "uart.h"
#include "stdio.h"


#define	GPFCON		(*(volatile unsigned long *)0x56000050)
#define	GPFDAT		(*(volatile unsigned long *)0x56000054)

#define	GPF4_out	(1<<(4*2))
#define	GPF5_out	(1<<(5*2))
#define	GPF6_out	(1<<(6*2))

void  wait(volatile unsigned long dly)
{
	for(; dly > 0; dly--);
}

int main(int argc, char *argv[])
{
	uart_init();

	printf("\r\njust for test our bootloader can print or not\n");
	printf("\r\nhaha\n");
	printf("\r\nit get out\n");

	GPFCON = GPF4_out|GPF5_out|GPF6_out;

	unsigned int i = 0;
	while(1){
		wait(1000000);
		GPFDAT = (~(i<<4));
		if(++i == 8)
			i = 0;
	}

	return 0;
}
