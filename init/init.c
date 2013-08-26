/**
 * file: init.c
 * created on: 2013-08-14
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

int is_norflash_boot(void)
{
	volatile unsigned int *p = (unsigned int *)0;
	unsigned int value;

	value = *p;
	*p = 0xdeadbeaf;
	if (0xdeadbeaf == *p) {
		*p = value; //restore value
		return 0;   //boot from nandflash
	} else {
		return 1;   //boot from norflash
	}

}

void clear_bss(void)
{
	extern int __bss_start, __bss_end;
	int *p = &__bss_start;

	for(; p < &__bss_end; p++)
		*p = 0;
}

void copy_bootloader_to_sdram(unsigned char *src, unsigned char *dst,
		unsigned int len)
{
	unsigned int i = 0;


	if (is_norflash_boot) {
		while (i < len) {
			*dst++ = *src++;
			i++;
		}
	} else {
		nand_read((unsigned int)src, dst, len);

	}

}
