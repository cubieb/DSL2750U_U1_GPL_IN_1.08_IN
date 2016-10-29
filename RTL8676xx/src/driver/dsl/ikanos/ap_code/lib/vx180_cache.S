/* =============================================================================
 *
 * Copyright (C) 2007 Ikanos Communications. All rights reserved
 *
 * The information and source code contained herein is the exclusive property
 * of Ikanos Communications and may not be disclosed, examined or reproduced in whole
 * or in part without the explicit written authorization from Ikanos
 * Communications.
 *
 * ===========================================================================*/

/* ===========================================================================
 * File name    : $RCSfile: vx180_cache.S,v $ 
 * Description  : Functions to manipulate the cache for vx180 or mips 24ke.
		  In particular to invalidate some, or all, of the instruction 
		  and data caches
 * ========================================================================== */

#define zero	$0	/* wired zero */
#define AT	$at	/* assembler temp */
#define v0	$2	/* return value */
#define v1	$3
#define a0	$4	/* argument registers a0-a3 */
#define a1	$5
#define a2	$6
#define a3	$7
#define t0	$8	/* caller saved  t0-t9 */
#define t1	$9
#define t2	$10
#define t3	$11
#define t4	$12
#define t5	$13
#define t6	$14
#define t7	$15
#define s0	$16	/* callee saved s0-s8 */
#define s1	$17
#define s2	$18
#define s3	$19
#define s4	$20
#define s5	$21
#define s6	$22
#define s7	$23
#define t8	$24
#define t9	$25
#define k0	$26	/* kernel usage */
#define k1	$27	/* kernel usage */
#define gp	$28	/* sdata pointer */
#define sp	$29	/* stack pointer */
#define s8	$30	/* yet another saved reg for the callee */
#define fp	$30	/* frame pointer - this is being phased out by MIPS */
#define ra	$31	/* return address */

#define LEAF(name)\
  		.globl	name;\
  		.ent	name;\
name:


#define SLEAF(name)\
  		.text;\
  		.ent	name;\
name:

#define ENDLEAF(name)\
	.end 	name

/* cache related definations and macros 
 */

#define Hit_Invalidate_D	0x11
#define Hit_Writeback_Inv_D	0x15
#define Hit_Writeback_D		0x19
#define Index_WriteBack_Inv_D	0x1

#define _mincache(size, maxsize) \
   bltu  size,maxsize,19f ; \
   move  size,maxsize ;    \
19:

#define _align(minaddr, maxaddr, linesize) \
   .set noat ; \
   subu  AT,linesize,1 ;   \
   not   AT ;        \
   and   minaddr,AT ;      \
   addu  maxaddr,-1 ;      \
   and   maxaddr,AT ;      \
   .set at

#define addr 	t0
#define maxaddr	t1
#define mask	t2

#define cacheop(kva, n, linesize, op) 	\
	.set noreorder;			\
	/* check for bad size */	\
	blez 	n, 21f ;  		\
	addu 	maxaddr,kva,n ;		\
	/* align to line boundaries */	\
	subu	mask,linesize,1 ;	\
	not	mask ;			\
	and 	addr,kva,mask ;		\
	addu	maxaddr, -1 ;		\
	and 	maxaddr, mask ;		\
20:	cache	op, 0(addr) ;		\
	bne	addr,maxaddr,20b ;	\
	addu	addr,linesize ;		\
21:	.set reorder
			

	/* subu	MASK,linesize,1 ; */
/*
 *  write back and invalidate address range in data caches 
 */

LEAF(mips_flush_inv_dcache)
	
	cacheop(a0,a1,a2,Hit_Writeback_Inv_D)
	
	j ra
ENDLEAF(mips_flush_inv_dcache)

/*
 *  just flush address range in data caches 
 */

LEAF(mips_flush_dcache)
	
	cacheop(a0,a1,a2,Hit_Writeback_D)
	
	j ra
ENDLEAF(mips_flush_dcache)

/*
 *  write back and invalidate address range in data caches 

LEAF(mips_inv_dcache)
	
	cacheop(a0,a1,a2,Hit_Invalidate_D)
	
	j ra
ENDLEAF(mips_inv_dcache)
*/

LEAF(mips_flush_inv_entire_dcache)

	cacheop(a0,a1,a2,Index_WriteBack_Inv_D)

	j ra
ENDLEAF(mips_flush_inv_entire_dcache)

