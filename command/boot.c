/**
 * file: boot.c
 * created on: 2013-08-27
 * author: pursuitxh
 * email:  pursuitxh@gmail.com
 */

#include "tag.h"
#include "boot.h"
#include "uart.h"

static struct tag *params;
static char *commandline = "noinitrd root=/dev/mtdblock3 init=/linuxrc console=ttySAC0";
void (*theKernel)(int zero, int arch, unsigned int params);

static void setup_start_tag (void *tag_addr)
{
	params = (struct tag *)tag_addr;
	
	params->hdr.tag = ATAG_CORE;
	params->hdr.size = tag_size (tag_core);

	params->u.core.flags = 0;
	params->u.core.pagesize = 0;
	params->u.core.rootdev = 0;

	params = tag_next (params);
}

static void setup_memory_tags(void)
{
	params->hdr.tag = ATAG_MEM;
	params->hdr.size = tag_size (tag_mem32);

	params->u.mem.start = 0x30000000;
	params->u.mem.size = 0x4000000;

	params = tag_next (params);
}

int strlen(char *str)
{
	int i = 0;
	while (str[i]) {
		i++;
	}
	return i;
}

void strcpy(char *dest, char *src)
{
	while ((*dest++ = *src++) != '\0');
}

static void setup_commandline_tag(char *commandline)
{
	char *p;

	if (!commandline)
		return;

	/* eat leading white space */
	for (p = commandline; *p == ' '; p++);

	/* skip non-existent command lines so the kernel will still
	 * use its default command line.
	 */
	if (*p == '\0')
		return;

	params->hdr.tag = ATAG_CMDLINE;
	params->hdr.size =
		(sizeof (struct tag_header) + strlen (p) + 1 + 4) >> 2;

	strcpy (params->u.cmdline.cmdline, p);

	params = tag_next (params);
}

static void setup_end_tag(void)
{
	params->hdr.tag = ATAG_NONE;
	params->hdr.size = 0;
}

void boot_kernel()
{
	/* 1. copy kernel from nandflash into sdram */
	printf("read kernel from nandflash...\n");
	nand_read(0x60000+64, (unsigned char *)0x30008000, 0x200000);

	/* 2. setup u-boot parameters */
	setup_start_tag((void *)0x30000100);
	setup_memory_tags();
	setup_commandline_tag(commandline);
	setup_end_tag();

	/* 3. jump to kernel entry */
	theKernel = (void (*)(int, int, unsigned int))0x30008000;
	theKernel(0, 362, 0x30000100);
}
