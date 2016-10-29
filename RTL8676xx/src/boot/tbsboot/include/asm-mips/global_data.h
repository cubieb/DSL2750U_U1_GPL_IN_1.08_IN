/*
 * (C) Copyright 2007

 */

#ifndef	__ASM_GBL_DATA_H
#define __ASM_GBL_DATA_H

#include <asm/regdef.h>

/*
 * The following data structure is placed in some memory wich is
 * available very early after boot (like DPRAM on MPC8xx/MPC82xx, or
 * some locked parts of the data cache) to allow for a minimum set of
 * global variables during system initialization (until we have set
 * up the memory controller so that we can use RAM).
 *
 * Keep it *SMALL* and remember to set CFG_GBL_DATA_SIZE > sizeof(gd_t)
 */


typedef	struct	global_data {
	unsigned long	bi_baudrate;    /* serial console baudrate */
	volatile unsigned long	bi_ip_addr;	    /* IP Address */
	volatile unsigned char	bi_enetaddr[6];	/* Ethernet adress */
	unsigned long	bi_arch_number;	/* unique id for this board */
	unsigned long	bi_boot_params;	/* where this board expects params */
	unsigned long	bi_memstart;	/* start of DRAM memory */
	unsigned long	bi_memsize;	    /* size	 of DRAM memory in bytes */
	unsigned long	bi_flashstart;	/* start of FLASH memory */
	unsigned long	bi_flashsize;	/* size  of FLASH memory */
	unsigned long	bi_flashoffset;	/* reserved area for startup monitor */
	unsigned long	have_console;	/* serial_init() was called */
	unsigned long	flags;	
	unsigned long	reloc_off;	    /* Relocation Offset */
	unsigned long	syscfg_addr;	/* Address of system configuration map */
	int             syscfg_ver;	    /* System configuration version */
	void		    **jt;		    /* jump table */
} gd_t;

volatile struct global_data *gd;

/*
 * Global Data Flags
 */
#define	GD_FLG_RELOC	0x00001		/* Code was relocated to RAM     */
#define	GD_FLG_DEVINIT	0x00002		/* Devices have been initialized */
#define	GD_FLG_SILENT	0x00004		/* Silent mode			         */
#define GD_FLG_IMGOK1   0x00008     /* Image 1 OK                    */
#define GD_FLG_IMGOK2   0x00010     /* Image 2 OK                    */


#endif /* __ASM_GBL_DATA_H */
