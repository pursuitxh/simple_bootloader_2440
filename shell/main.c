/**
 * file: main.c
 * created on: 2013-08-23
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

#include "uart.h"
#include "stdio.h"
#include "s3c2440.h"

#define	GPF4_OUT	(1<<(4*2))
#define	GPF5_OUT	(1<<(5*2))
#define	GPF6_OUT	(1<<(6*2))

void  wait(volatile unsigned long dly)
{
	for(; dly > 0; dly--);
}

int main(int argc, char *argv[])
{

	int num;
	unsigned int i = 0;
	uart_init();
	putc('A');

	//printf("\r\njust for test our bootloader can print or not\n");

#if 0
	GPFCON = GPF4_OUT|GPF5_OUT|GPF6_OUT;
	while(1){
		wait(1000000);
		GPFDAT = (~(i<<4));
		if(++i == 8)
			i = 0;
	}
#endif

	while(1) {

		printf("\n ************************************** \n\r");
		printf("\n **** Simple BootLoader for JZ2440 **** \n\r");
		printf("\n ************************************** \n\r");
		printf("1: Boot Linux from Nand Flash.\n\r");
		printf("\nSelect Number: ");

		scanf("%d", &num);

		switch (num) {
		case 1:
			printf(" boot linux from nandflash...\n\r");
			boot_kernel();
			break;
		default:
			printf(" Not supported...\n\r");
			break;
		}

	}

	return 0;
}
