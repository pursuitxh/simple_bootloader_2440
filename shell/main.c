/**
 * file: main.c
 * created on: 2013-08-23
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

#include "uart.h"
#include "stdio.h"

int main(int argc, char *argv[])
{
	uart_init();

	printf("just for test our bootloader can print or not\n");

	return 0;
}
