/*
 *  Startup Code for FUSIV Vox CPU-core
 *	Ikanos Communications
 *
 *  Copyright (c) 2003	Wolfgang Denk <wd@denx.de>
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


#include <config.h>
#include <version.h>
#include "asmlexra.h"


/* Address of register which contains deviceId information */
#define DEVID_ADDR	0xb900003C
#define AD6834_ID	0x6834
#define AD6833_ID	0x6833
#define AD6501_ID	0x6501
#define AD6502_ID	0x6502
#define AD6503_ID	0x6503
#define AD6836_ID	0x6836

#define CLOCK_GATE		0xb90000CC
#define ENABLE_CLOCK_GATE	0x3FFF


#define RVECENT(f,n) \
   b f; nop
#define XVECENT(f,bev) \
   b f     ;           \
   li k0,bev

#define RELOC(toreg,address) \
        bal     9f; \
9:; \
        la      toreg,address; \
        addu    toreg,ra; \
        la      ra,9b; \
        subu    toreg,ra

	.set noreorder

	.globl _start
	.text
_start:
	RVECENT(reset,0)	/* U-boot entry point */	/*00, utlb vector */
	RVECENT(reset,1)	/* software reboot */

	.align 4
reset:

	/* STATUS register */
	li	k0, ST0_BEV
	mtc0	k0, CP0_STATUS
	nop
	nop
	nop

	/* CAUSE register */
	mtc0	zero, CP0_CAUSE
	nop
	nop
	nop

	/* Initialize caches...
	 */
	bal romCacheClear
	nop

	/* Initialize DDR */
	li	a0, 0xb9250010
	li	t0, 0x00000002
	sw	t0, 0(a0)
	nop
	nop
	nop

	li	a0, 0xb9250010
	li	t0, 0x00000003
	sw	t0, 0(a0)
	nop
	nop
	nop

	li	a0, 0xb9250000
	li	t0, 0x194a030c
	sw	t0, 0(a0)
	nop
	nop
	nop

	li	a0, 0xb9250004
	li	t0, 0x100e6212
	sw	t0, 0(a0)
	nop
	nop
	nop

	li	a0, 0xb9250008
	li	t0, 0x00000029
	sw	t0, 0(a0)
	nop
	nop
	nop

	bal     1f
	nop     
	.word   _GLOBAL_OFFSET_TABLE_
1:      
	lw t1 ,0(ra)
	nop
	move    gp, t1
	
	/* Initialize soc basic setting. */
	bal	basicInit
	nop

/*
 * t0 = addr_sp
 * t2 = destination address
 */
	li	t0, CONFIG_DRAM_BASE + CFG_SDRAM_SIZE - SDRAM_SIZE + SP_OFFSET
	move	sp, t0		/* Set new stack pointer		*/

	li	t0, TEXT_BASE_BOOTSTART
	la	t3, in_ram
	nop
	lw	t2, -12(t3)	/* t2 <-- uboot_end_data	*/
	nop
	
	li	a2, CONFIG_DRAM_BASE + CFG_SDRAM_SIZE - SDRAM_SIZE + BOOTSTART_OFFSET
	move	t1, a2

	/*
	 * Fix GOT pointer:
	 *
	 * New GOT-PTR = (old GOT-PTR - CFG_MONITOR_BASE) + Destination Address
	 */
	move	t6, gp
	sub	gp, TEXT_BASE_BOOTSTART
	add	gp, a2			/* gp now adjusted		*/
	sub	t6, gp, t6		/* t6 <-- relocation offset	*/
	nop

	/*
	 * t0 = source address
	 * t1 = target address
	 * t2 = source end address
	 */
	/* On the purple board we copy the code earlier in a special way
	 * in order to solve flash problems
	 */

1:
	lw	t3, 0(t0)
	nop
	sw	t3, 0(t1)
	nop
	addu	t0, 4
	addu	t1, 4	
	ble	t0, t2, 1b
	nop			/* delay slot			*/


	/* Jump to where we've relocated ourselves.
	 */
	addi	t0, a2, in_ram - _start
	j	t0
	nop

	.word	uboot_end_data
	.word	uboot_end
	.word	num_got_entries

in_ram:
	/* Now we want to update GOT.
	 */
	lw	t3, -4(t0)	/* t3 <-- num_got_entries	*/
	nop
	addi	t4, gp, 8	/* Skipping first two entries.	*/
	li	t2, 2

1:
	lw	t1, 0(t4)
	nop
	beqz 	t1, 2f
	add	t1, t6
	sw	t1, 0(t4)
	nop
2:
	addi	t2, 1
	blt	t2, t3, 1b
	addi	t4, 4		/* delay slot			*/

	/* Clear BSS.
	 */
	lw	t1, -12(t0)	/* t1 <-- uboot_end_data	*/
	nop
	lw	t2, -8(t0)	/* t2 <-- uboot_end		*/
	nop
	move a0 , t1
	add	t1, t6		/* adjust pointers		*/
	add	t2, t6

	sub	t1, 4
1:	addi	t1, 4
	blt	t1, t2, 1b
	sw	zero, 0(t1)	/* delay slot			*/
	nop

	la	t9, uncompress_bootloader_second 
	nop
	j	t9
	nop

/*******************************************************************************
*
* romCacheClear - invalidate all LX-4080 instr. and data cache locations
*
* This routine set all cache instruction and data cache entries to an invalid 
* state.
*
*/

	.ent	romCacheClear
romCacheClear:
	.set	noreorder
	mfc0	t3, C0_SR		/* save SR                           */
	nop
	nop
	mtc0	zero, C0_SR		/* disable interrupts                */


	/* invalidate data cache */

	mtc0	zero, C0_CCTL		/* Zero the Cache Control register.  */
	nop
	li	t1, CCTL_DIvl
	mtc0	t1, C0_CCTL		/* invalidate data cache             */
	nop
	mtc0	zero, C0_CCTL		/* Zero the Cache Control register.  */
	nop

	/*
	 * Give the stall a few cycles to assert.
	 */

	li	t1, 10
1:	subu	t1, 1
	bnez	t1, 1b

	
	/* invalidate instruction cache */

invalidateICache:

	/* invalidate instruction cache */

	mtc0	zero, C0_CCTL		/* Zero the Cache Control register.  */
	nop
	li	t1, CCTL_IIvl
	mtc0	t1, C0_CCTL		/* invalidate instruction cache      */
	nop
	mtc0	zero, C0_CCTL		/* Zero the Cache Control register.  */
	nop

	/*
	 * Give the stall a few cycles to assert.
	 */

	li	t1, 10
1:	subu	t1, 1
	bnez	t1, 1b

	
	mtc0	t3, C0_SR		/* return to previous state          */
	j	ra
	nop
	.set	reorder
	.end	romCacheClear

	.ent	basicInit
basicInit:
	move	t1, ra
	
	li 	t2, DEVID_ADDR
	lw 	t3, 0(t2)			/* t3 has id */
	li 	t2, AD6834_ID
	bne	t3, t2 ,not6834
	la	t0, ad6834pll
	b	pllreset
not6834:
	li 	t2, AD6833_ID
	bne	t3, t2 ,not6833
	la	t0, ad6833pll
	b	pllreset
not6833:
	li	t2, AD6501_ID
	bne	t3,t2 , not6501
	la	t0, ad6501pll
	b	pllreset
not6501:
	li	t2,	AD6502_ID
	bne	t3,t2,not6502
	la	t0, ad6502pll
	b 	pllreset
	nop
not6502:
        li      t2,     AD6503_ID
        bne     t3,t2,not6503
        la      t0, ad6503pll
        b       pllreset
        nop
not6503:
	li	t2,	AD6836_ID
	bne	t3,t2,default_pll
	la	t0, ad6836pll
	b 	pllreset
	nop

default_pll:
	la	t0, ad6834pll
pllreset:	
	/* v0 gets pointer to register */
loop1:
	lw	v0,0(t0)
	lw	v1,4(t0)
	beqz	v0,donepll
	nop
	sw	v1,0(v0)
	addu	t0,8
	b	loop1
donepll:
	nop

	//RELOC(t0,reginittab)
	la	t0, reginittab
	
	/* v0 gets pointer to register */
1:	lw	v0,0(t0)
	lw	v1,4(t0)
	beqz	v0,2f
	nop
	sw	v1,0(v0)
	addu	t0,8
	b	1b
2:	nop
	/* Enable clock gate */
	li      t2, CLOCK_GATE
	li	t3, ENABLE_CLOCK_GATE
	sw	t3,0(t2)

	j	t1
	nop
	.end	basicInit



	/* Exception handlers.
	 */
romReserved:
	li k1, 0xdead
	b romReserved

romExcHandle:
	li k1, 0xbeef
	mfc0 k1, C0_EPC
	jr k1
	rfe
	b romExcHandle
	
