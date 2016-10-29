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
#include "ar7100_soc.h"

#define CYGNUM_DDR_CONFIG_VAL 0xefbc8cd0
#define CYGNUM_DDR_CONFIG2_VAL 0x8e7156a2
#define CYGNUM_DDR_MODE_VAL 0x61
#define CYGNUM_DDR_REFRESH_VAL 0x461b
//#define CYGNUM_DDR_REFRESH_VAL 0x4050
#define  CYGNUM_DDR_RD_DATA_THIS_CYCLE_VAL 0xffff

#define CAS_LATENCY_SHIFT	 	27
#define CAS_LATENCY_MASK 		 7
#define DELAY_TIME 50
#define MEMDELAY(count, reg)    \
			li      reg, count;     \
	9: 	addi    reg, -1;        \
			bgtz    reg, 9b;        \
			nop
	.globl  lowlevel_init
lowlevel_init:

	# Initialize SDRAM configuration.	
	
	li      a0, CYGARC_UNCACHED_ADDRESS(AR7100_DDR_CONFIG)
	/* Use whatever is in CDL file, presumably CAS 2.5 */
	li	t0, CYGNUM_DDR_CONFIG_VAL
	sw	t0, 0(a0)

	

2:	li      a1, K1BASE | AR7100_DDR_CONFIG2
	li	t3, CYGNUM_DDR_CONFIG2_VAL
	sw	t3, 0(a1)
        MEMDELAY(DELAY_TIME, t2)

        li      a0, K1BASE | AR7100_DDR_CONTROL
        li      a1, K1BASE | AR7100_DDR_EXT_MODE

        li      t5, 8
        sw      t5, 0(a0)
        MEMDELAY(DELAY_TIME, t2)

        li      t5, 1
        sw      t5, 0(a0)
        MEMDELAY(DELAY_TIME, t2)

        li      t5, 0
        sw      t5, 0(a1)
        MEMDELAY(DELAY_TIME, t2)

        li      t5, 2
        sw      t5, 0(a0)
        MEMDELAY(DELAY_TIME, t2)

        li      t5, 8
        sw      t5, 0(a0)
        MEMDELAY(DELAY_TIME, t2)

	# DDR_MODE
        li      a2, K1BASE | AR7100_DDR_MODE

        /* t1 still has the mode in it */

	li	t5, CYGNUM_DDR_MODE_VAL
	
	sw	t5, 0(a2)
	MEMDELAY(DELAY_TIME, t2)

        li      t5, 1
        sw      t5, 0(a0)
        MEMDELAY(DELAY_TIME, t2)

        li      a0, K1BASE | AR7100_DDR_REFRESH
        li      t5, CYGNUM_DDR_REFRESH_VAL
        sw      t5, 0(a0)
        MEMDELAY(DELAY_TIME, t2)

        li      a2, K1BASE | AR7100_DDR_RD_DATA_THIS_CYCLE
	li	t5, CYGNUM_DDR_RD_DATA_THIS_CYCLE_VAL
	sw	t5, 0(a2)
	
        li      a1, 0x07
        li	a0, K1BASE|AR7100_DDR_TAP_CONTROL0
        sw      a1, 0(a0)
        li	a0, K1BASE|AR7100_DDR_TAP_CONTROL1
        sw      a1, 0(a0)
        li	a0, K1BASE|AR7100_DDR_TAP_CONTROL2
        sw      a1, 0(a0)
        li	a0, K1BASE|AR7100_DDR_TAP_CONTROL3
        sw      a1, 0(a0)
	j ra

	




