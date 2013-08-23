/**
 * file: s3c2440.h
 * created on: 2013-08-20
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

#ifndef _S3C2440_H_
#define _S3C2440_H_

/* NANDFLASH Controller Registers */
#define NFCONF		(*(volatile unsigned long *)0x4E000000)
#define NFCONT		(*(volatile unsigned long *)0x4E000004)
#define NFCMMD		(*(volatile unsigned char *)0x4E000008)
#define NFADDR		(*(volatile unsigned char *)0x4E00000C)
#define NFDATA		(*(volatile unsigned char *)0x4E000010)
#define NFSTAT		(*(volatile unsigned char *)0x4E000020)

/* UART Registers */
#define ULCON0		(*(volatile unsigned long *)0x50000000)
#define UCON0		(*(volatile unsigned long *)0x50000004)
#define UFCON0		(*(volatile unsigned long *)0x50000008)
#define UMCON0		(*(volatile unsigned long *)0x5000000c)
#define UTRSTAT0	(*(volatile unsigned long *)0x50000010)
#define UTXH0		(*(volatile unsigned char *)0x50000020)
#define URXH0		(*(volatile unsigned char *)0x50000024)
#define UBRDIV0		(*(volatile unsigned long *)0x50000028)

/* GPIO Registers */
#define GPHCON		(*(volatile unsigned long *)0x56000070)
#define GPHUP		(*(volatile unsigned long *)0x56000078)

#endif /* _S3C2440_H_ */
