/**
 * file: nandflash.h
 * created on: 2013-08-20
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

#ifndef _NANDFLASH_H_
#define _NANDFLASH_H_

/* NandFlash Registers */
#define TACLS   0
#define TWRPH0  3
#define TWRPH1  0

extern void nand_init(void);
extern void nand_read(unsigned int addr, unsigned char *buf, unsigned int len);

#endif /* _NANDFLASH_H_ */
