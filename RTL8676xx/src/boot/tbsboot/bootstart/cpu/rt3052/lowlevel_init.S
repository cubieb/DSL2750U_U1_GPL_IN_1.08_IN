/*
 *  Memory sub-system initialization code for rt3052.
 *
 *  Copyright (c) 20036  xuanguanglei <xuanguanglei@twsz.com>
 *  
 *  lowlevel_init:
 *  
 *  This routine should initialize SDRAM.
 */

#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include "autoconf.h"
#include "rt_mmap.h"

#define SDRAM_CFG0_REG RALINK_SYSCTL_BASE + 0x0300
#define SDRAM_CFG1_REG RALINK_SYSCTL_BASE + 0x0304
#define SDRAM_CFG0_ALWAYS_ONE ( 1 << 31)
#define SDRAM_CFG1_SDRAM_INIT_START ( 1 << 31)
#define SDRAM_CFG1_SDRAM_INIT_DONE ( 1 << 30)


	.globl  lowlevel_init
lowlevel_init:
#if 1
/* delay cycle */	
	li t0,0xFFFF
	li t1,0x1
1:
	sub t0, t0, t1
	bnez t0, 1b
/* end of delay cycle */	

	li t5,SDRAM_CFG0_REG
	lw t6,0(t5)
	nop
	and t6,0xF0000000
	
#if defined (RT2880_FPGA_BOARD) || defined (RT2883_FPGA_BOARD) || defined (RT3052_FPGA_BOARD)
#ifdef RT2880_FPGA_BOARD
#ifdef RT2880_MP
	nop
	or t6,0x01825282
	//or t6,0x01815282
	nop	
#else /* RT2880_SHUTTLE */
	or t6,0x91825282
	//or t6,0x91815282
#endif
#else //2883, 3052 fpga
	nop
	or t6,0xD1825282
	//or t6,0x01815282
	nop	
#endif
#else //RT2880_ASIC_BOARD, RT2883_ASIC_BOARD, RT3052_ASIC_BOARD
	or t6,0xD1825272
#endif
	nop
	sw t6,0(t5)
	nop


    	li t5,SDRAM_CFG1_REG
	nop
#if defined (RT2880_ASIC_BOARD) || defined (RT2883_ASIC_BOARD) || defined (RT3052_ASIC_BOARD)
/*
 *
 *	Turn on SDRAM RBC (BIT 29 in SDRAM_CFG1, offset 0x4) in RT3052.
 *	  RT2880 RBC bit is Reserved bit, and change the same value for RT2880 and RT3052
 *	  Original 0x81xx0600 -> 0xa1xx0600
 *
 *		by bobtseng, 2008.7.7.
 */
#ifdef ON_BOARD_64M_DRAM_COMPONENT
	//64Mbits sdram component
	li t6,0xa1010600
#elif  ON_BOARD_128M_DRAM_COMPONENT 
	//128Mbits sdram component
	li t6,0xa1110600
#elif  ON_BOARD_256M_DRAM_COMPONENT 
	//256Mbits sdram component
	li t6,0xa1120600
#else
	DRAM Component not defined
#endif
#ifdef ON_BOARD_32BIT_DRAM_BUS
	and t6,0xFEFFFFFF
	or t6,(1<<24)
#elif  ON_BOARD_16BIT_DRAM_BUS
	and t6,0xFEFFFFFF
#elif
	DRAM bus not defined
#endif
#else
#ifdef ON_BOARD_64M_DRAM_COMPONENT
	//64Mbits sdram component
	li t6,0x81010096
#elif  ON_BOARD_128M_DRAM_COMPONENT 
	//128Mbits sdram component
	li t6,0x81110096
#elif  ON_BOARD_256M_DRAM_COMPONENT 
	//256Mbits sdram component
	li t6,0x81120096
#elif
	DRAM Component not defined
#endif
#ifdef ON_BOARD_32BIT_DRAM_BUS
	and t6,0xFEFFFFFF
	or t6,(1<<24)
#elif  ON_BOARD_16BIT_DRAM_BUS
	and t6,0xFEFFFFFF
#elif
	DRAM bus not defined
#endif
#endif
	nop
	sw t6,0(t5)
	nop

	
WAIT_SDRAM_INIT_DOWN:	

	lw t6,0(t5)
	nop
	and  	t6, t6, SDRAM_CFG1_SDRAM_INIT_DONE
	beqz 	t6, WAIT_SDRAM_INIT_DOWN
	nop
#endif
	j ra
	nop



		.globl  delay_time
delay_time:
		li t0 , 256

1:
		nop
		nop
		nop
		nop
		sub t0 , 1
		bnez t0 , 1b
		nop
		j ra
		nop

	
