/*
 *	Leo @ Analog Devices
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
#if 0
#define DEBUG
#endif

#include <common.h>
#include "inittab.h"
//#include <pci.h>

/* ------------------------------------------------------------------------- */
//#define SDRAM_MAX_SIZE	 128*1024*1024			/* 128M board first */

//static long int dram_size (long int, long int *, long int);

/* ------------------------------------------------------------------------- */

#if 0 //by zhangyu

/* ------------------------------------------------------------------------- */
long int initddr(int chipid)
{
	long int size0=0,size1=0;
	int mode=0,i;
	int settting[4]={0x10026212,
		0x100e6212,
		0x100a6212,
		0x10066212
	};

	*((int *)0xb9250000) = 0x194a030c;
	*((int *)0xb9250008) = 0x00000029;
	for (i=0; i<4 ;i++) {
		*((int *)0xb9250004) = settting[i];
		udelay(500);
		size1 = get_ram_size((ulong *)CONFIG_DRAM_BASE,SDRAM_MAX_SIZE);
		if(size1>size0) {
			size0 = size1;
			mode =i;
		}
	}
	*((int *)0xb9250004) = settting[mode];
	udelay(500);
	return size0;
}

long int initdram (int board_type)
{
	long int size8, size9;
	long int size = 0;
	int id;
	
	id = *((int *)0xb900003c);
//	if(id == 0x6836)
//		return initddr(id);
	
	/*
	 * Check Bank 0 Memory Size
	 *
	 * try 8 column mode
	 */
	size8 = dram_size (CFG_MAMR_8COL,
			   (ulong *)CONFIG_DRAM_BASE,
			   SDRAM_MAX_SIZE);

	/*
	 * try 9 column mode
	 */
	size9 = dram_size (CFG_MAMR_9COL,
			   (ulong *) CONFIG_DRAM_BASE,
			   SDRAM_MAX_SIZE);
	debug ("size8 %ld , size9 %ld\n",size8,size9);
	if (size8 < size9) {		/* leave configuration at 9 columns */
		size = size9;
		debug ("SDRAM Bank 0 in 9 column mode: %ld KB\n", size >> 10);
	} else {			/* back to 8 columns            */
		size = size8;
		*((int *)0xb9140000) = 0x00d91148;
		*((int *)0xb9140004) = CFG_MAMR_8COL;
		*((int *)0xb9140000) = 0x00d91149;
		udelay (500);
		debug ("SDRAM Bank 0 in 8 column mode: %ld KB\n", size >> 10);	
	}

	return (size);
}

/* ------------------------------------------------------------------------- */

/*
 * Check memory range for valid RAM. A simple memory test determines
 * the actually available RAM size between addresses `base' and
 * `base + maxsize'. Some (not all) hardware errors are detected:
 * - short between address lines
 * - short between data lines
 */

static long int dram_size (long int mamr_value, long int *base,
						   long int maxsize)
{
	*((int *)0xb9140000) = 0x00d91148;
	*((int *)0xb9140004) = mamr_value;
	*((int *)0xb9140000) = 0x00d91149;
	udelay (500);
	return (get_ram_size((base), maxsize));
}


#endif
