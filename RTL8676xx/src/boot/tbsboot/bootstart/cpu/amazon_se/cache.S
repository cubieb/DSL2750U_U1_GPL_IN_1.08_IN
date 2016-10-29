

#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/cacheops.h>

	/* 16KB is the maximum size of instruction and data caches on
	 * MIPS 4K.
	 */
#define MIPS_MAX_CACHE_SIZE	0x4000
# define	CACHE_LOCK_SIZE	(CFG_DCACHE_SIZE)

/*
 * cacheop macro to automate cache operations
 * first some helpers...
 */
#define _mincache(size, maxsize) \
   bltu  size,maxsize,9f ; \
   move  size,maxsize ;    \
9:

#define _align(minaddr, maxaddr, linesize) \
   .set noat ; \
   subu  AT,linesize,1 ;   \
   not   AT ;        \
   and   minaddr,AT ;      \
   addu  maxaddr,-1 ;      \
   and   maxaddr,AT ;      \
   .set at

/* general operations */
#define doop1(op1) \
   cache op1,0(a0)
#define doop2(op1, op2) \
   cache op1,0(a0) ;    \
   nop ;          \
   cache op2,0(a0)

/* specials for cache initialisation */
#define doop1lw(op1) \
   lw zero,0(a0)
#define doop1lw1(op1) \
   cache op1,0(a0) ;    \
   lw zero,0(a0) ;      \
   cache op1,0(a0)
#define doop121(op1,op2) \
   cache op1,0(a0) ;    \
   nop;           \
   cache op2,0(a0) ;    \
   nop;           \
   cache op1,0(a0)

#define _oploopn(minaddr, maxaddr, linesize, tag, ops) \
   .set  noreorder ;    \
10:   doop##tag##ops ;  \
   bne     minaddr,maxaddr,10b ; \
   add      minaddr,linesize ;   \
   .set  reorder

/* finally the cache operation macros */
#define vcacheopn(kva, n, cacheSize, cacheLineSize, tag, ops) \
   blez  n,11f ;        \
   addu  n,kva ;        \
   _align(kva, n, cacheLineSize) ; \
   _oploopn(kva, n, cacheLineSize, tag, ops) ; \
11:

#define icacheopn(kva, n, cacheSize, cacheLineSize, tag, ops) \
   _mincache(n, cacheSize);   \
   blez  n,11f ;        \
   addu  n,kva ;        \
   _align(kva, n, cacheLineSize) ; \
   _oploopn(kva, n, cacheLineSize, tag, ops) ; \
11:

#define vcacheop(kva, n, cacheSize, cacheLineSize, op) \
   vcacheopn(kva, n, cacheSize, cacheLineSize, 1, (op))

#define icacheop(kva, n, cacheSize, cacheLineSize, op) \
   icacheopn(kva, n, cacheSize, cacheLineSize, 1, (op))


#define IFX_CACHE_EXTRA_OPERATION						\
	/* set WST bit */							\
	mfc0	a0, CP0_ECC;							\
	li	a1, ECCF_WST;							\
	or	a0, a1;								\
	mtc0	a0, CP0_ECC;							\
										\
	li	a0, K0BASE;							\
	move	a2, t2;		/* icacheSize */				\
	move	a3, t4;		/* icacheLineSize */				\
	move	a1, a2;								\
	icacheop(a0,a1,a2,a3,(Index_Store_Tag_I));				\
										\
	/* clear WST bit */							\
	mfc0	a0, CP0_ECC;							\
	li	a1, ~ECCF_WST;							\
	and	a0, a1;								\
	mtc0	a0, CP0_ECC;							\
										\
	/* 1: initialise dcache tags. */					\
										\
	/* cache line size */							\
	li	a2, CFG_CACHELINE_SIZE;						\
	/* kseg0 mem address */							\
	li	a1, 0;								\
	li	a3, CFG_CACHE_SETS * CFG_CACHE_WAYS;				\
1:										\
	/* store tag (invalid, not locked) */					\
	cache 0x8, 0(a1);							\
	cache 0x9, 0(a1);							\
										\
	add	a3, -1;								\
	bne	a3, zero, 1b;							\
	add	a1, a2;								\
										\
	/* set WST bit */							\
	mfc0	a0, CP0_ECC;							\
	li	a1, ECCF_WST;							\
	or	a0, a1;								\
	mtc0	a0, CP0_ECC;							\
										\
	li	a0, K0BASE;							\
	move	a2, t3;		/* dcacheSize */				\
	move	a3, t5;		/* dcacheLineSize */				\
	move	a1, a2;								\
	icacheop(a0,a1,a2,a3,(Index_Store_Tag_D));				\
										\
	/* clear WST bit */							\
	mfc0	a0, CP0_ECC;							\
	li	a1, ~ECCF_WST;							\
	and	a0, a1;								\
	mtc0	a0, CP0_ECC;


	.globl	mips_cache_reset	
mips_cache_reset:

	li	t2, CFG_ICACHE_SIZE
	li	t3, CFG_DCACHE_SIZE
	li	t4, CFG_CACHELINE_SIZE
	move	t5, t4

	li	v0, MIPS_MAX_CACHE_SIZE

	/* Now clear that much memory starting from zero.
	 */

	li	a0, KSEG1
	addu	a1, a0, v0

2:	sw	zero, 0(a0)
	sw	zero, 4(a0)
	sw	zero, 8(a0)
	sw	zero, 12(a0)
	sw	zero, 16(a0)
	sw	zero, 20(a0)
	sw	zero, 24(a0)
	sw	zero, 28(a0)
	addu	a0, 32
	bltu	a0, a1, 2b

	/* Set invalid tag.
	 */

	mtc0	zero, CP0_TAGLO
	mtc0	zero, CP0_TAGLO, 1
	mtc0	zero, CP0_TAGLO, 2
	mtc0	zero, CP0_TAGLO, 3
	mtc0	zero, CP0_TAGLO, 4
   /*
    * The caches are probably in an indeterminate state,
    * so we force good parity into them by doing an
    * invalidate, load/fill, invalidate for each line.
    */

										
	li	a0, K0BASE;							
	move	a2, t2;		/* icacheSize */				
	move	a3, t4;		/* icacheLineSize */				
	move	a1, a2;								
	icacheopn(a0,a1,a2,a3,121,(Index_Store_Tag_I,Fill))
										
	IFX_CACHE_EXTRA_OPERATION

	j  ra
	nop


	.globl	mips_cache_lock
mips_cache_lock:

	li	a1, K0BASE - CACHE_LOCK_SIZE
	addu	a0, a1
	li	a2, CACHE_LOCK_SIZE
	li	a3, CFG_CACHELINE_SIZE
	move	a1, a2
	icacheop(a0,a1,a2,a3,0x1d)

	j	ra
	nop
	
