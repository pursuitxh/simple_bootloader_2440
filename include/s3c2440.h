/**
 * file: s3c2440.h
 * created on: 2013-08-20
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

#ifndef _S3C2440_H_
#define _S3C2440_H_

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


#endif /* _S3C2440_H_ */
