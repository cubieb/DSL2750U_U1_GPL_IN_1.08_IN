#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include "ar7100_soc.h"

#define RVECENT(f,n) \
   b f; nop
#define XVECENT(f,bev) \
   b f     ;           \
   li k0,bev

#define INITIAL_SR_ARCH 	0x1000ff00
#define INITIAL_SR_VAR 0x00000000
#define INITIAL_SR_PLF 0x00000000
#define INITIAL_SR 	(INITIAL_SR_ARCH|INITIAL_SR_VAR|INITIAL_SR_PLF)
#define	INITIAL_CONFIG0_UNCACHE	0x00000002
#define	INITIAL_CONFIG0_CACHE	0x00000003

	.set noreorder
	.globl _start
	.text
_start:
	RVECENT(reset,0)	/* U-boot entry point */
	RVECENT(reset,1)	/* software reboot */

	.align 4
reset:

	bal     1f
	nop
	.word   _GLOBAL_OFFSET_TABLE_
1:
	lw t1 ,0(ra)
	nop
	move    gp, t1
#if 1
	/* Initialize any external memory.
	 */
	la     t9, set_pll
	jalr    t9
	nop

	la	k0,INITIAL_CONFIG0_UNCACHE	# Set up config0 register
	mtc0	k0,CP0_CONFIG		# to disable cache
	nop
	nop
	nop

	# Initialize/clear watchpoint registers
	mtc0	zero, CP0_WATCHLO
	nop
	nop
	nop
	mtc0	zero, CP0_WATCHHI 
	nop
	nop
	nop

	mtc0	zero,CP0_CAUSE		# zero cause reg
	nop

	la	v0,INITIAL_SR 		# initialize status register
	mtc0	v0,CP0_STATUS
	nop
	nop
	nop
	la	v0,INITIAL_CONFIG0_UNCACHE
	mtc0	v0,CP0_CONFIG
	nop
	nop
	nop

	la      t9, lowlevel_init	
	jalr    t9
	nop
#endif

/*
 * t0 = addr_sp
 * t2 = destination address
 */
	li	t0, CONFIG_DRAM_BASE + CFG_SDRAM_SIZE - SDRAM_SIZE + SP_OFFSET
	move	sp, t0		/* Set new stack pointer		*/

	li	t0, TEXT_BASE_BOOTSTART
	la	t3, in_ram
	lw	t2, -12(t3)	/* t2 <-- uboot_end_data	*/
	
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
	add	t1, t6
	sw	t1, 0(t4)
2:
	addi	t2, 1
	blt	t2, t3, 1b
	addi	t4, 4		/* delay slot			*/

	la      t9, mips_cache_reset	
	jalr    t9
	nop

	/* Clear BSS.
	 */
	lw	t1, -12(t0)	/* t1 <-- uboot_end_data	*/
	lw	t2, -8(t0)	/* t2 <-- uboot_end		*/
	move a0 , t1
	add	t1, t6		/* adjust pointers		*/
	add	t2, t6

	sub	t1, 4
1:	addi	t1, 4
	bltl	t1, t2, 1b
	sw	zero, 0(t1)	/* delay slot			*/

	la	t9, uncompress_bootloader_second 
	j	t9
	nop

	/* Exception handlers.
	 */
romReserved:
	b romReserved
romExcHandle:
	b romExcHandle
