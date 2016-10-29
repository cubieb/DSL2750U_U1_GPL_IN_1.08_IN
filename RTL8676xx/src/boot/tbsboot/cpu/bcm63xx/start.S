/*
 */


#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>

#define RVECENT(f,n) \
   b f; nop
#define XVECENT(f,bev) \
   b f     ;           \
   li k0,bev

	.set noreorder
	.globl _start
	.text
_start:		
	bal 1f
	nop
1:	li t8,TEXT_BASE
	sub t7,ra,8
	sub t8,t8,t7
	
	RVECENT(reset,0)	/* U-boot entry point */
	RVECENT(reset,1)	/* software reboot */

	.align 4
reset:
	/* Clear watch registers.
	 */
	mtc0	zero, CP0_WATCHLO
	mtc0	zero, CP0_WATCHHI

	/* STATUS register */
	mfc0	k0, CP0_STATUS		
	li	k1, ~ST0_IE
	and	k0, k1
	mtc0	k0, CP0_STATUS

	/* CAUSE register */
	mtc0	zero, CP0_CAUSE

	/* Init Timer */
	mtc0	zero, CP0_COUNT
	mtc0	zero, CP0_COMPARE

	/* CONFIG0 register */
	li	t0, CONF_CM_UNCACHED
	mtc0	t0, CP0_CONFIG
	
	/*Initialize all the TLB entries to some invalid value 	
	*/	
  	mtc0    zero,CP0_ENTRYLO0       /* tlblo0 = invalid */
  	nop
  	mtc0    zero,CP0_ENTRYLO1       /* tlblo1 = invalid */
  	nop
  	mtc0    zero,CP0_PAGEMASK       /* 4K pages */
  	nop
  	li      t0,0xa0000000                   /* tlbhi  = impossible vpn */
  	li      t1,31      /* index */
  	nop
1:  
	mtc0    t0,CP0_ENTRYHI
  	nop
   	mtc0    t1,CP0_INDEX
  	nop
  	addu    t0,0x2000               /* inc vpn */
  	tlbwi
  	bnez    t1,1b
  	subu    t1,1                  
	bal     1f
	nop
	
	.word   _GLOBAL_OFFSET_TABLE_
1:
	move    gp, ra
	lw      t1, 0(ra)
	sub 	t1,t8
	move	gp, t1

	/* Initialize any external memory.
	 */
	la      t9, lowlevel_init
	sub 	t9,t8
	jalr    t9
	nop

	/*
	 * a2 = destination address
	 */		
	li 	a2,CFG_LOAD_ADDR
	move	sp, a0		/* Set new stack pointer		*/		
	la	t3, in_ram
	sub 	t3,t8
	lw	t2, -12(t3)	/* t2 <-- uboot_end_data	*/
	sub 	t2,t8
	move	t1, a2

	/*
	 * Fix GOT pointer:
	 *
	 * New GOT-PTR = (old GOT-PTR - CFG_MONITOR_BASE) + Destination Address
	 */
	move	t6, gp	
	sub gp ,t7
	add	gp, a2			/* gp now adjusted		*/
	sub	t6, gp, t6		/* t6 <-- relocation offset	*/	
	move t0,t7

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
	sw	t3, 0(t1)
	addu	t0, 4
	ble	t0, t2, 1b
	addu	t1, 4			/* delay slot			*/	

	/* If caches were enabled, we would have to flush them here.
	 */

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
	addi	t4, gp, 8	/* Skipping first two entries.	*/
	li	t2, 2
1:
	lw	t1, 0(t4)
	beqz	t1, 2f
	add	t1,t6
	sub 	t1,t8
	sw	t1, 0(t4)
2:
	addi	t2, 1
	blt	t2, t3, 1b
	addi	t4, 4		/* delay slot			*/

	/* Clear BSS.
	 */
	lw	t1, -12(t0)	/* t1 <-- uboot_end_data	*/
	lw	t2, -8(t0)	/* t2 <-- uboot_end		*/
	add	t1, t6		/* adjust pointers		*/
	add	t2, t6

	sub	t1, 4
1:	addi	t1, 4
	bltl	t1, t2, 1b
	sw	zero, 0(t1)	/* delay slot			*/
	
	li 	t2,7
	la 	t4,init_sequence		

1:	lw 	t1,0(t4)
	beqz 	t1,5f
	nop
	add 	t1,t6
	sub 	t1,t8
	sw 	t1,0(t4)
	beq 	t2,7,1b
	add 	t4,4
5:		
	li	t0, CONFIG_DRAM_BASE + CFG_INIT_SP_OFFSET
	la	sp, 0(t0)	
	move 	a1,t8
	la	t9, board_init_f
	j	t9
	nop

	/* Exception handlers.
	 */
romReserved:
	b romReserved
romExcHandle:
	b romExcHandle
