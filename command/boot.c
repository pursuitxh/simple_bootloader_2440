/**
 * file: boot.c
 * created on: 2013-08-17
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

int main(int argc, char *argv[])
{
#if 0
	void (*theKernel)(int zero, int arch, unsigned int params);

	/* 0. setup serial port */
	uart_init();

	/* 1. read kernel from nandflash into sdram */
	nand_read(0x60000+64, 0x30008000, 0x200000);

	/* 2. set command paramters */
	setup_start_tag();
	setup_memory_tag();
	setup_commandline_tag();
	setup_end_tag();

	/* 3. start boot kernel */
	theKernel = (void (*)(int, int, unsigned int))0x30008000;
	theKernel(0, 362, 0x30000100);
#endif
	
	return 0;
}
