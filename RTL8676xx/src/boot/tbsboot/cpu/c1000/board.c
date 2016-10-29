/*
 * Copyright (C) 2006 Mindspeed Technologies Inc.
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <common.h>
#include <config.h>

DECLARE_GLOBAL_DATA_PTR;

int board_init(void)
{
	/* arch number of Mindspeed Comcerto */
	gd->bd->bi_arch_number = MACH_TYPE_COMCERTO; //MACH_TYPE_M825XX;

	/* adress of boot parameters */
	gd->bd->bi_boot_params = BOOTARGS_OFFSET;


	gd->bd->bi_dram[0].start = CONFIG_DRAM_BASE;
	gd->bd->bi_dram[0].size = CFG_SDRAM_SIZE;

	printf("Reserve MSP memory\n");
	gd->bd->bi_dram[0].start += MSP_BOTTOM_MEMORY_RESERVED_SIZE;
	gd->bd->bi_dram[0].size -= MSP_BOTTOM_MEMORY_RESERVED_SIZE + MSP_TOP_MEMORY_RESERVED_SIZE;


	return 0;
}

