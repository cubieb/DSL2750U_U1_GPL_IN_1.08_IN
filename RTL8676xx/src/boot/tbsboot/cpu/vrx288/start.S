#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include "../../bootstart/cpu/vrx288/yamon.h"
#include <asm/addrspace.h>

#define RVECENT(f,n) \
   b f; \
   nop
#define XVECENT(f,bev) \
   b f     ;           \
   li k0,bev

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
	lw 		t1 ,0(ra)
	nop
	move    gp, t1
	li	t0, CONFIG_DRAM_BASE + CFG_SDRAM_SIZE - SDRAM_SIZE + SP_OFFSET
	la	sp, 0(t0)

	la  t0, clear_bss
	j	t0
	nop

	.word	uboot_end_data
	.word	uboot_end
	.word	num_got_entries

clear_bss:

	lw	t1, -12(t0)	/* t1 <-- uboot_end_data	*/
	lw	t2, -8(t0)	/* t2 <-- uboot_end		*/

	sub	t1, 4
1:	addi	t1, 4
	bltl	t1, t2, 1b
	sw	zero, 0(t1)	/* delay slot	*/
	la	t9, board_init_r
	j	t9
	nop



	/* Exception handlers.	 */
romReserved:
	b romReserved
romExcHandle:
	b romExcHandle


dcache_linesize_mips32:
.globl   dcache_linesize_mips32
.ent     dcache_linesize_mips32
	/* Read CONFIG1 register, which holds implementation data */
	MFC0_SEL_OPCODE( R_t9, R_C0_Config1, R_C0_SelConfig1 )

	/* D-cache line size */
	li	t8, M_Config1DL
	and	t8, t9
	//beq	t8, zero, dcache_linesize_zero
	li	t7, S_Config1DL
	srl	t8, t7
	li	t7, 0x2
	sll	v0, t7, t8

	jr	ra
	nop
.end    dcache_linesize_mips32
/* Subroutine : Determine dcache line */

dcache_writeback_invalidate:
//    a0 : size
//    a1 : dcache_line_size
//    a2 : start addr
.globl   dcache_writeback_invalidate
.ent     dcache_writeback_invalidate

	addu	t8, a2, a0
	subu  t8, a1
	/* Loop through all address */
1:
	cache	DCACHE_ADDR_HIT_WRITEBACK_INVALIDATE, 0(a2)	/* clear tag */
//	bne		a2, t8, 1b
	sub	t9, a2, t8
	blez	t9, 1b
	addu	a2, a1

9:
	jr	ra
	nop
        .end    dcache_writeback_invalidate

dcache_hit_invalidate:
//    a0 : size
//    a1 : dcache_line_size
//    a2 : start addr
.globl   dcache_hit_invalidate
.ent     dcache_hit_invalidate

	addu	t8, a2, a0
	subu  t8, a1
	/* Loop through all address */
1:
	cache	DCACHE_ADDR_HIT_INVALIDATE, 0(a2)	/* clear tag */
	bne		a2, t8, 1b
	addu	a2, a1

9:
	jr	ra
	nop
        .end dcache_hit_invalidate


