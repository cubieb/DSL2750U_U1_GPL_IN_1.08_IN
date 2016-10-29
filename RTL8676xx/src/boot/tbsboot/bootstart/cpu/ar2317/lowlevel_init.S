/*
 *  Memory sub-system initialization code for atheros7130.
 *
 *  Copyright (c) 20036  javen <zhangyu@twsz.com>
 *  
 *  lowlevel_init:
 *  
 *  This routine should initialize SDRAM.
 */

#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include "ar2316reg.h"

	.globl  lowlevel_init
lowlevel_init:

	# Initialize SDRAM configuration.	
	
	li	a2, K1BASE | AR2316_MEM_STMGOR
	li	t0, 0x2265655
	sw	t0, 0(a2)
	sync

	li	a2, K1BASE | AR2316_MEM_CFG

	lw	t0, 0(a2)		# MEM_CFG
	and	t0, ~(SDRAM_DATA_WIDTH_M | SDRAM_COL_WIDTH_M | \
	         SDRAM_ROW_WIDTH_M|SDRAM_BANKADDR_BITS_M)
	
#if AR2316_SDRAM_DDR_SDRAM
	li	t1, (((AR2316_SDRAM_DATA_WIDTH / 8) - 1) << SDRAM_DATA_WIDTH_S)
#else
	li	t1, (((AR2316_SDRAM_DATA_WIDTH / 8) - 2) << SDRAM_DATA_WIDTH_S)
#endif
    
	/* Column Width */
	or  t1, ((AR2316_SDRAM_COL_WIDTH - 1) << SDRAM_COL_WIDTH_S)

	/* Row Width */
	or  t1, ((AR2316_SDRAM_ROW_WIDTH - 1) << SDRAM_ROW_WIDTH_S)
    
	/* Bank Address Bits are 2 */
	or  t1, (1 << SDRAM_BANKADDR_BITS_S)
	
	or  t0, t1
	sw	t0, 0(a2)		# MEM_CFG
	sync
	
	/* Setup Memory Refresh value */
	li	a2, K1BASE | AR2316_MEM_REF
	li	t0, AR2316_SDRAM_MEMORY_REFRESH_VALUE

	sw	t0, 0(a2)		# MEM_REF
	sync

	/* Place SDRAM into Auto Initialize state */
	li	a2, K1BASE | AR2316_MEM_CTRL
	li	t0, 0x3089

	sw	t0, 0(a2)		# MEM_CTRL
	sync
	
1:	
	lw	t0, 0(a2)		# MEM_CTRL
	andi	t1,t0,0x1
	bnez	t1, 1b
	nop
	
	j ra

	




