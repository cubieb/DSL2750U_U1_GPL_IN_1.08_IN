/*
 *  Cache-handling routined for FUSIV Lexra Core
 * 	Ikanos Communications
 */


#include <config.h>
#include <version.h>
#include "asmlexra.h"
#include "addrspace.h"

		.global invalidateCaches	
        .ent invalidateCaches
invalidateCaches:
	mtc0	zero, C0_CCTL		/* Zero the Cache Control register.  */
	nop
        li 		t0, CCTL_IIvl | CCTL_DIvl
        mtc0    t0, C0_CCTL
        nop
	mtc0	zero, C0_CCTL		/* Zero the Cache Control register.  */
	nop

        li      t1, 10			/* enough for CPU Stall */
1:      subu    t1, 1
        bnez    t1, 1b

        jr      ra
        .end invalidateCaches


		.global invalidateDCaches	
        .ent invalidateDCaches
invalidateDCaches:
        mfc0    t1, C0_CCTL
        nop
        ori     t0, t1, CCTL_DIvl
        mtc0    t0, C0_CCTL
        nop
        mtc0 t1, C0_CCTL

        li      t1, 10			/* enough for CPU Stall */
1:      subu    t1, 1
        bnez    t1, 1b

        jr      ra
        .end invalidateDCaches
        
/*******************************************************************************
*
* lexra_cache_lock - lock instruction cache set 1 
*/
	.globl	lexra_cache_lock
	.ent	lexra_cache_lock
lexra_cache_lock:
        mfc0    t0, C0_CCTL
        ori     t1, t0, CCTL_ILk1
        mtc0    t1, C0_CCTL

        li      t1, 10			/* enough for CPU Stall */
1:      subu    t1, 1
        bnez    t1, 1b

        ori     t1, t0, CCTL_ILk2
        mtc0    t1, C0_CCTL

		j	ra
	.end	lexra_cache_lock

	.global flush_cache
	.ent	flush_cache
flush_cache:
	lui	t0, (KSEG1 >> 16)		/* load uncached address	*/
	lw	t0, 0(t0)		/* read in order to flush 	*/
	j	ra			/* return to caller		*/
	.end flush_cache
	
	.global dcache_status
	.ent dcache_status
dcache_status:
	j	ra
	.end dcache_status

	.global dcache_disable
	.ent dcache_disable
dcache_disable:
	j	ra
	.end dcache_disable
		
