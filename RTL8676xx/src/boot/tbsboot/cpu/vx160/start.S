#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include "asmlexra.h"

#define RVECENT(f,n) \
   b f; nop
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
	lw t1 ,0(ra)
	nop
	move    gp, t1

	li	t0, CONFIG_DRAM_BASE + CFG_SDRAM_SIZE - SDRAM_SIZE + SP_OFFSET
	la	sp, 0(t0)
	nop

	la  t0, clear_bss
	nop
	j	t0
	nop

	.word	uboot_end_data
	.word	uboot_end
	.word	num_got_entries

clear_bss:

	lw	t1, -12(t0)	/* t1 <-- uboot_end_data	*/
	nop
	lw	t2, -8(t0)	/* t2 <-- uboot_end		*/
	nop

	sub	t1, 4
1:	addi	t1, 4
	blt	t1, t2, 1b
	sw	zero, 0(t1)	/* delay slot			*/
	nop

	la	t9, board_init_r
	nop
	j	t9
	nop

	/* Exception handlers.
	 */
romReserved:
	b romReserved
romExcHandle:
	b romExcHandle
