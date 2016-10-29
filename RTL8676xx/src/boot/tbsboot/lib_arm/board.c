/*
 * (C) Copyright 2002-2006
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * (C) Copyright 2002
 * Sysgo Real-Time Solutions, GmbH <www.elinos.com>
 * Marius Groeger <mgroeger@sysgo.de>
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
 * To match the U-Boot user interface on ARM platforms to the U-Boot
 * standard (as on PPC platforms), some messages with debug character
 * are removed from the default U-Boot build.
 *
 * Define DEBUG here if you want additional info as shown below
 * printed upon startup:
 *
 * U-Boot code: 00F00000 -> 00F3C774  BSS: -> 00FC3274
 * IRQ Stack: 00ebff7c
 * FIQ Stack: 00ebef7c
 */

#include <common.h>
#include <command.h>
#include <malloc.h>
#include <version.h>
#include <net.h>

DECLARE_GLOBAL_DATA_PTR;

ulong monitor_flash_len;

#ifndef CONFIG_IDENT_STRING
#define CONFIG_IDENT_STRING ""
#endif

const char version_string[] =
	U_BOOT_VERSION" (" __DATE__ " - " __TIME__ ")"CONFIG_IDENT_STRING;

/*
 * Begin and End of memory area for malloc(), and current "brk"
 */
static ulong mem_malloc_start = 0;
static ulong mem_malloc_end = 0;
static ulong mem_malloc_brk = 0;

static void mem_malloc_init (void)
{
	ulong dest_addr = CONFIG_DRAM_BASE + CFG_SDRAM_SIZE - SDRAM_SIZE + BOOTSTART_OFFSET;

	mem_malloc_end = dest_addr;
	mem_malloc_start = dest_addr - TOTAL_MALLOC_LEN;
	mem_malloc_brk = mem_malloc_start;

	memset ((void *) mem_malloc_start,
		0,
		mem_malloc_end - mem_malloc_start);
	
	alloc_init( CONFIG_DRAM_BASE + CFG_SDRAM_SIZE - SDRAM_SIZE + ALLOC_INIT_OFFSET );
}

void *sbrk (ptrdiff_t increment)
{
	ulong old = mem_malloc_brk;
	ulong new = old + increment;

	if ((new < mem_malloc_start) || (new > mem_malloc_end))
	{
		return (NULL);
	}
	mem_malloc_brk = new;

	return ((void *) old);
}

/************************************************************************
 * Init Utilities							*
 ************************************************************************
 * Some of this code should be moved into the core functions,
 * or dropped completely,
 * but let's get it working (again) first...
 */

static int init_baudrate (void)
{
	return (0);
}

static int display_banner (void)
{
	printf ("\n\n%s\n\n", version_string);
	debug ("U-Boot code: %08lX -> %08lX  BSS: -> %08lX\n",
	       _armboot_start, _bss_start, _bss_end);
#ifdef CONFIG_MODEM_SUPPORT
	debug ("Modem Support enabled\n");
#endif

	return (0);
}

#ifndef CFG_NO_FLASH
static void display_flash_config (ulong size)
{
	puts ("Flash: ");
	print_size (size, "\n");
}
#endif /* CFG_NO_FLASH */

/*
 * Breathe some life into the board...
 *
 * Initialize a serial port as console, and carry out some hardware
 * tests.
 *
 * The first part of initialization is running from Flash memory;
 * its main purpose is to initialize the RAM so that we
 * can relocate the monitor code to RAM.
 */

/*
 * All attempts to come up with a "common" initialization sequence
 * that works for all boards and architectures failed: some of the
 * requirements are just _too_ different. To get rid of the resulting
 * mess of board dependent #ifdef'ed code we now make the whole
 * initialization sequence configurable to the user.
 *
 * The requirements for any new initalization function is simple: it
 * receives a pointer to the "global data" structure as it's only
 * argument, and returns an integer return code, where 0 means
 * "continue" and != 0 means "fatal error, hang the system".
 */
typedef int (init_fnc_t) (void);

int print_cpuinfo (void); /* test-only */

init_fnc_t *init_sequence[] = 
{
	cpu_init,		/* basic cpu dependent setup */
	init_baudrate,		/* initialze baudrate settings */
	serial_init,		/* serial communications setup */
	console_init_f,		/* stage 1 init of console */
	display_banner,		/* say that we are here */
	init_led,
	NULL,
};

void start_armboot(void)
{
	bd_t *bd;
	init_fnc_t **init_fnc_ptr;
	ulong size , addr;
	unsigned short	item_len;
	int syscfg_version = -1;
	struct sys_cfg *syscfg = (struct sys_cfg *)SYSCFGADDR;
	struct sys_config *oldcfg = (struct sys_config *)syscfg;
	const unsigned char default_mac[] ={0x00, 0x02, 0x03, 0x04, 0x05, 0x06};	
	/* compiler optimization barrier needed for GCC >= 3.4 */
	__asm__ __volatile__("": : :"memory");


	 /* Reserve memory for globe arena.*/
	addr = CONFIG_DRAM_BASE + CFG_SDRAM_SIZE - SDRAM_SIZE + SP_START;
	addr -= sizeof(gd_t);
	gd= (gd_t*) addr;
	memset ((void *)gd, 0, sizeof (gd_t));

	/*
	 * (permanently) allocate a Board Info struct
	 * and a permanent copy of the "global" data
	 */
	addr -= sizeof(bd_t);
	bd = (bd_t *)addr;
	gd->bd = bd;
	memset ((void *)bd, 0, sizeof (bd_t));

	monitor_flash_len = _bss_start - _armboot_start;

	for (init_fnc_ptr = init_sequence; *init_fnc_ptr; ++init_fnc_ptr)
	{
		if ((*init_fnc_ptr)() != 0) 
		{
			hang ();
		}
	}

	board_init();
	interrupt_init();

	mem_malloc_init ();
	/* configure available FLASH banks */
	size = flash_init ();
	display_flash_config (size);
	/* armboot_start is defined in the board-specific linker script */

		{
		syscfg_version = is_sysdata((void *)syscfg);
		}
	
	if(syscfg_version == 0)
		{
		printf("\nfirst_kernel_offset=0x%x,first_rootfs_offset=0x%x\n", oldcfg->layout.zone_offset[ZONE_KERNEL_FIRST], oldcfg->layout.zone_offset[ZONE_ROOTFS_FIRST]);
		if(0 != oldcfg->second_image_len) 
			{
			printf("second_kernel_offset=0x%x,second_rootfs_offset=0x%x\n", oldcfg->layout.zone_offset[ZONE_KERNEL_SECOND], oldcfg->layout.zone_offset[ZONE_ROOTFS_SECOND]);
			}
		printf("System configuration is old version %s\n", oldcfg->version);
		/* 合成kernel command*/
		prepare_tags((void *)oldcfg, syscfg_version);
		bd->bi_ip_addr = oldcfg->ip ;		
		memcpy (bd->bi_enetaddr, oldcfg->mac, sizeof(oldcfg->mac));
		}
	else if(syscfg_version == 1)
		{
		printf("\nfirst_kernel_offset=0x%x,first_rootfs_offset=0x%x\n", syscfg->first_kernel_offset, syscfg->first_rootfs_offset);
		if((0 != syscfg->second_kernel_len) && (0 != syscfg->second_rootfs_len))
			printf("second_kernel_offset=0x%x,second_rootfs_offset=0x%x\n", syscfg->second_kernel_offset, syscfg->second_rootfs_offset);
		printf("System configuration is new version %s\n", syscfg->version);
		/* 合成kernel command*/
		prepare_tags((void *)syscfg, syscfg_version);
		bd->bi_ip_addr = syscfg->ip ;		
		memcpy (bd->bi_enetaddr, syscfg->mac, sizeof(syscfg->mac));
		}
	else
		{		
		memset(syscfg, 0xFF, sizeof(struct sys_cfg));
		printf("Can't get system configuration.Use default vlaue.\n");
		#ifdef __BIG_ENDIAN
		bd->bi_ip_addr = 0xc0a80101;
		#else	
		bd->bi_ip_addr = 0x101a8c0;
		#endif
		memcpy(bd->bi_enetaddr, default_mac, sizeof(default_mac));
		}
	
	console_init_r ();	/* fully init console as a device */

#if (CONFIG_COMMANDS & CFG_CMD_NET)
	eth_initialize(gd->bd);
#endif 

	/* main loop can return to retry autoboot, if so just run it again. */
	for (;;) 
	{
		main_loop ((void *)syscfg, syscfg_version);

	}

	/* NOTREACHED - no way out of command loop except booting */
}

void hang (void)
{
	puts ("### ERROR ### Please RESET the board ###\n");
	for (;;);
}

