/**
 * file: main.c
 * created on: 2013-08-23
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

#include "uart.h"
#include "stdio.h"
#include "s3c2440.h"
#include "boot.h"

int main(int argc, char *argv[])
{

	int num;
	uart_init();

	while(1) {
		printf("\n ************************************** \n\r");
		printf("\n **** Simple BootLoader for JZ2440 **** \n\r");
		printf("\n ************************************** \n\r");
		printf("\n\r");
		if (is_norflash_boot())
			printf("\nboot from norflash\n\r");
		else
			printf("\nboot from nandflash\n\r");
			
		printf("1: Boot Linux from SDRAM.\n\r");
		printf("\nSelect Number: \n\r");

		scanf("%d", &num);

		switch (num) {
		case 1:
			printf(" boot linux from sdram...\n\r");
			boot_kernel();
			break;
		default:
			printf(" Not supported...\n\r");
			break;
		}

	}

	return 0;
}
