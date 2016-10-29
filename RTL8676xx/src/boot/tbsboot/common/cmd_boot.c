/*
 * (C) Copyright 2000-2003
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * Misc boot support
 */
#include <common.h>
#include <command.h>
#include <zlib.h>
#include <asm/byteorder.h>
#include <flash_layout_private.h>
#include <zlib.h>

int do_bootm_linux(unsigned int src, char *arg)
{
	unsigned int entry;
	void (*theKernel)(void);
	int ret = -1;

	if(0 == src)
		{
		goto err;
		}
	entry = (unsigned int)load_addr;
	prepare_kernel_commandline(arg);/* ºÏ³Ékernel command*/
	if(src == entry)
		{
		ret = 0;
		}
	else
		{
		printf ("Uncompress linux from %#x to %#x... ", src, entry);
		ret = uncompressLZMA((Bytef *)entry, (Bytef *)src);		   
		if(0 != ret)
			{
			printf("Failed!\n");
			}
		else
			{
			printf("OK!\n");
			}
		}
	if(0 == ret)
		{	/* we assume that the kernel is in place */
		printf("Booting the kernel ...\n");	
		theKernel = (void (*)(void))entry;
		theKernel();
		}
	
err:	
	return ret;
}

int do_go (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	ulong	addr, rc;
	int     rcode = 0;

	if (argc < 2) {
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
	}

	addr = simple_strtoul(argv[1], NULL, 16);

	printf ("## Starting application at 0x%08lX ...\n", addr);

	/*
	 * pass address parameter as argv[0] (aka command name),
	 * and all remaining args
	 */
#if defined(CONFIG_I386)
	/*
	 * x86 does not use a dedicated register to pass the pointer
	 * to the global_data
	 */
	argv[0] = (char *)gd;
#endif
#if !defined(CONFIG_NIOS)
	rc = ((ulong (*)(int, char *[]))addr) (--argc, &argv[1]);
#else
	/*
	 * Nios function pointers are address >> 1
	 */
	rc = ((ulong (*)(int, char *[]))(addr>>1)) (--argc, &argv[1]);
#endif
	if (rc != 0) rcode = 1;

	printf ("## Application terminated, rc = 0x%lX\n", rc);
	return rcode;
}

int do_unlzma (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
        int ret;
        unsigned int src;
        unsigned int entry;

        if (argc < 3) {
                printf ("Usage:\n%s\n", cmdtp->usage);
                return 1;
        }

        src   = simple_strtoul(argv[1], NULL, 16);
        entry = simple_strtoul(argv[2], NULL, 16);
        printf("Decompress code from 0x%lx to 0x%x... ",src,entry);

        ret = uncompressLZMA((Bytef *)entry, (Bytef *)src);

        if(!ret)
        {
            printf("ok!\n\r");
            return 0;
        }
        else
        {
            printf("failed!\n\r");
            return 1;
        }
}

int do_bootm(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int src;
	
	if(argc >= 2)
		{
		src = simple_strtoul(argv[1], NULL, 16);
		}
	else
		{
		src = prepare_kernel();
		}
	if(0 != src)
		{
		do_bootm_linux(src, NULL);
		}
	return 0;
}

int do_reboot(cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	void (*reboot)(void) = (void *)CONFIG_TEXT_BASE_BOOTSTART;
	reboot();

	return 0;
}

/* -------------------------------------------------------------------- */

U_BOOT_CMD(
	go, CFG_MAXARGS, 1,	do_go,
	"go      - start application at address 'addr'\n",
	"addr [arg ...]\n    - start application at address 'addr'\n"
	"      passing 'arg' as arguments\n"
);


U_BOOT_CMD(
	reset, CFG_MAXARGS, 1,	do_reset,
	"reset   - Perform RESET of the CPU\n",
	NULL
);


U_BOOT_CMD(
	reboot, CFG_MAXARGS, 1,	do_reboot,
	"reboot  - Perform system reboot\n",
	NULL
);


U_BOOT_CMD(
        unlzma, 3, 2,     do_unlzma,
        "unlzma  - decompress code with LZMADecoder \n",
        "source target\n    - unlzma code from source to target address\n"
);


U_BOOT_CMD(
 	bootm,	CFG_MAXARGS,	1,	do_bootm,
 	"bootm   - boot application image from memory\n",
 	"[addr [arg ...]]\n    - boot application image stored in memory\n"
 	"\tpassing arguments 'arg ...'; when booting a Linux kernel,\n"
 	"\t'arg' can be the address of an initrd image\n"
);

