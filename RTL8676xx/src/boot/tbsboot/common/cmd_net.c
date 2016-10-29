/*
 * (C) Copyright 2000
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
 * Boot support
 */
#include <common.h>
#include <command.h>
#include <net.h>
#include <flash_layout_private.h>

extern void flush_cache (ulong start_addr, ulong size);

#if (CONFIG_COMMANDS & CFG_CMD_NET)

ulong load_addr = CONFIG_LOADADDR;


static int netboot_common(proto_t proto, cmd_tbl_t *cmdtp, int argc, char *argv[],unsigned int *file_size)
{
	int size = 0;
	
	switch(argc)
		{
		case 3:
			NetTftpServerIP = string_to_ip(argv[1]);
			copy_filename (BootFile, argv[2], sizeof(BootFile));
			break;
			
		case 4:
			load_addr = simple_strtoul(argv[1], NULL, 16);
			NetTftpServerIP = string_to_ip(argv[2]);
			copy_filename (BootFile, argv[3], sizeof(BootFile));
			break;

		default: 
			printf("Usage:\n%s\n", cmdtp->usage);
			return -1;
		}

	size = NetLoop(proto);
	if(size <= 0)
		{
		eth_init();
		*file_size = 0;
		}
	else
		{
		*file_size = (unsigned int)size;
		}
	/* flush cache */
	flush_cache(load_addr, *file_size);
	if(*file_size < 4)
		{
		return -1;
		}
	else
		return 0;


}


int do_tftpb (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int file_size;

	return netboot_common (TFTP, cmdtp, argc, argv, &file_size);
}


int do_tftp_save (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int file_size = 0;
	int ret = 0;
    int clearcfg = 1;
	struct update_parameters uppav;

    if( ( argc > 4 ) )
	{
		printf ("Usage:\n%s\n", cmdtp->usage);

		return 1;
	}

	if( ( argc == 4 ) && (strcmp(argv[3],"-k") == 0 ) )
	{
		clearcfg = 0;
		argc = argc -1;
	}

	ret = netboot_common(TFTP, cmdtp, argc, argv, &file_size);
	if((ret != 0) || (file_size < 4))
		{
		goto out;
		}
	
	ret = flash_update((unsigned char *)load_addr, file_size, clearcfg, &uppav);
	if( ret == ERROR_OK )
		{
		do_reset(NULL, 0, 0, NULL);
		}
	
out:
	return ret;
}



U_BOOT_CMD(
	tftp,	4,	3,	do_tftpb,
	"tftp	- download image via network using TFTP protocol\n",
	"[loadAddress] tftpServerIP bootfilename\n"
);



U_BOOT_CMD(
	savet,	4,	3,	do_tftp_save,
	"savet	- download IMG image via network using TFTP protocol and save it to flash\n",
	" tftpServerIP IMGFilename\n"
);

#ifdef CMD_SAVEB
int do_tftp_saveb (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int file_size = 0;
	int ret = 0;
	char force_flag = 0;
	struct update_parameters uppav;

	if((argc > 4))
		{
		printf ("Usage:\n%s\n", cmdtp->usage);
		return 1;
		}

	if(argc == 4)
		{
		if(0 == strcmp(argv[3], "-f"))
			{
			force_flag = 1;
			argc = argc -1;
			}
		else if(0 == strcmp(argv[3], "-b"))
			{
			argc = argc -1;
			force_flag = -1;
			}			
		}
		
	ret = netboot_common(TFTP, cmdtp, argc, argv, &file_size);
	if((ret != 0) || (file_size < 4))
		{
		goto out;
		}
	
	ret = flash_update((unsigned char *)load_addr, file_size, force_flag, &uppav);
	if( ret == ERROR_OK )
		{
		do_reset(NULL, 0, 0, NULL);
		}
	
out:
	return ret;
}

U_BOOT_CMD(
	saveb,	4,	3,	do_tftp_saveb,
	"saveb	- download BIN image via network using TFTP protocol and save it to flash\n",
	" tftpServerIP BINFilename [-f]\n"
);
#endif

#ifdef CMD_BOOTH
int do_booth (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	unsigned int file_size = 0;
	int ret = 0;
	char arg[256] = {'\0'};
	char *p = NULL;
	int argcs = argc;
	int i, j = 0;
	
	if((argc >= 4) && (0 == strcmp(argv[3], "-r")))
		{
		for(i = 4; i < argcs; i++)
			{
			strcpy((char *)(arg + j), argv[i]);
			j += strlen(argv[i]);
			memcpy(arg + j, " ", 1);
			j++;
			}
		argcs = 3;
		p = &arg[0];
		}
	ret = netboot_common(TFTP, cmdtp, argcs, argv, &file_size);
	if((0 == ret) && (file_size > 4))  /*if return 0,it is successful. */
		{
		do_bootm_linux((unsigned int)load_addr, p);
		}
	
	return ret;
}

U_BOOT_CMD(
	booth,	CFG_MAXARGS,	3,	do_booth,
	"booth	- boot kernel from host\n",
	" [tftpServerIP] [filename] [-r][bootargs]\n"
);
#endif


int do_ping (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[])
{
	if(argc < 2)
		{
		printf ("Usage:\n%s\n", cmdtp->usage);
		return -1;
		}
	NetPingIP = string_to_ip(argv[1]);
	if(NetPingIP == 0)
		{
		printf ("Usage:\n%s\n", cmdtp->usage);
		return -1;
		}

	if(NetLoop(PING) < 0)
		{
		printf("ping failed; host %s is not alive\n", argv[1]);
		return 1;
		}
	else
		{
		printf("host %s is alive\n", argv[1]);
		return 0;
		}
}

U_BOOT_CMD(
	ping,	2,	1,	do_ping,
	"ping\t- send ICMP ECHO_REQUEST to network host\n",
	"pingAddress\n"
);


#endif	/* CFG_CMD_NET */


