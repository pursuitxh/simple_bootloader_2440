/**
 * file: 	uart.c
 * Created on: 	2013-08-22
 * Author: 	pursuitxh
 * Email: 	pursuitxh@gmail.com *
 */

//TODO:
void put_uart(char *p,int num){
#if 0
	while(*p&&num--){
		*(volatile unsigned int *)0xd0000020=*p++;
	};
#endif
}
