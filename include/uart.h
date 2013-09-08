/**
 * file: 	uart.h
 * Created on: 	2013-08-22
 * Author: 	pursuitxh
 * Email: 	pursuitxh@gmail.com *
 */

#ifndef __UART_H_
#define __UART_H_

#define PCLK            50000000
#define UART_CLK        PCLK
#define UART_BAUD_RATE  115200
#define UART_BRD        ((UART_CLK  / (UART_BAUD_RATE * 16)) - 1)
#define TXD0READY   	(1<<2)
#define	RXD0READY		(1)

void uart_init(void);
void putc(unsigned char c);
void puts(char *str);
unsigned char getc(void);
void uart_queue(char *p,int len);

#endif /* __UART_H_ */
