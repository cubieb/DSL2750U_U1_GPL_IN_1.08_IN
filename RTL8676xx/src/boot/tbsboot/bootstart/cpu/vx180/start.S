#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>

#define RVECENT(f,n) \
   b f; nop
#define XVECENT(f,bev) \
   b f     ;           \
   li k0,bev


#define VX180_DEVID_ADDR  0xb900003C
#define VX180_DEVID        0x6850

//#define FUSIV_SYSDDR_128MB 1
//#define FUSIV_SYSDDR_32MB 1
#define FUSIV_SYSDDR_64MB 1

/* Address of register which contains deviceId information */
#if   FUSIV_SYSDDR_16MB
#define XTN_MEM_BASE                  0x0080007f
#elif FUSIV_SYSDDR_32MB
#define	XTN_MEM_BASE		      0x0180007f
#elif FUSIV_SYSDDR_64MB
#define	XTN_MEM_BASE		      0x0380007f
#elif FUSIV_SYSDDR_128MB
#define	XTN_MEM_BASE		      0x0780007f
#endif
#define PSB_CTL                     0xb90000EC

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

	la	k0,INITIAL_CONFIG0_UNCACHE	# Set up config0 register
	mtc0	k0,CP0_CONFIG			# to disable cache
	nop
	nop
	nop

	la      t9, memsetup
	jalr    t9
	nop
#if 1
	/* Enabling Write thru cache Mode */
	li	t0, 0
	mtc0	t0, CP0_CONFIG

	/* Initialize caches...
	 */
	bal	mips_cache_reset
	nop

	/* Initialize soc basic setting. */
	bal	basicInit
	nop
#endif

#endif

/*
 * t0 = addr_sp
 * t2 = destination address
 */
	li	t0, CONFIG_DRAM_BASE + SP_OFFSET
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


	.ent    memsetup
memsetup:
#DLL Adjustment values

#Config reset

# Based on the SDRAM/DDR Controller we will have to change the
# SDRAM controller Parameters here.
# Currently Configured for 32-bit DDR with 10-bit column address
# Initially Change the configuration
# Store 0xb925_0000 0010_0011_1001_1001_1000_1010_0000_0000 (0x23998a00)
# Store 0xb925_0004 0001_0000_0000_1010_0111_0011_0010_0011 (0x100a7323)
# Store 0xb925_0008 0000_0000_0000_0000_0000_0000_0110_0010 (0x00000062) (cas of 2.5 and burst 4)

    la t0, 0xb9000014
    li t2, 0x0
    sw t2, 0(t0)
    nop


    la t0, 0xb9000018
    li t2, 0xff
    sw t2, 0(t0)
    nop

# Following are for BME
    la t0, PSB_CTL
    la t1, XTN_MEM_BASE
    sw t1, 0(t0)
    nop
    nop


# Following code is added for PCI register setting before system reset
    la t0, 0xb90000b0
    li t2, 0x00080c00
    sw t2, 0(t0)
    nop

# Following code is added for DSP PLL setting to 115 MHz
    la t0, 0xb90000bc
    li t2, 0x00001c00
    sw t2, 0(t0)
    nop
#

    la t0, 0xb90000b8
    li t2, 0x00010000
    sw t2, 0(t0)
    nop

#DLL disable
    la t0, 0xb90000e0
    li t2, 0x10
    sw t2, 0(t0)
    nop

    /* Enable the second Chip Select CS1 for FLASH Access */
    la t0, 0xb9148000
    li t2, 0x000000F6
    sw t2, 0(t0)

    la t0, 0xb9250000
    /*li t2, 0x24216514 ---by Zhang Yu*/
    li t2, 0xB4196514
    sw t2, 0(t0)
    nop


#if FUSIV_SYSDDR_16MB
    li t2, 0x300a1323
#elif FUSIV_SYSDDR_32MB
    li t2, 0x300e1323
#elif FUSIV_SYSDDR_64MB
li t2, 0x20121323
#elif FUSIV_SYSDDR_128MB
    li t2, 0x10127323
#endif
    sw t2, 4(t0)
    nop

    /* Set CAS Latency to 3 ,by Zhang Yu*/
    li t2, 0x00000032
    sw t2, 8(t0)
    nop
	jr	ra
	nop
	.end    memsetup

	.ent    basicInit
basicInit:
	move	t1, ra
	li	t2, VX180_DEVID_ADDR
	lw	t3, 0(t2)
	li	t2, VX180_DEVID
	la	t0, vx180pll
	b	pllreset

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
	la	t0, reginittab
	/* v0 gets pointer to register */
1:	lw	v0,0(t0)
	lw	v1,4(t0)
	beqz	v0,2f
	nop
	sw	v1,0(v0)
	addu	t0,8
	b	1b
2:
	nop
	j	t1
	nop
	.end	basicInit


	.ent    mips_cache_reset
mips_cache_reset:
	/*  Initialise I cache (not done by testbench if CLEARCACHE == 0).
         *  Determine cache size using CONFIG1 register.
         */
        /* Determine size of I$ */

        /*
         *  In case assembler does not recognize mf with select
        mfc0 t2, 16, 1
         */

        mfc0 t2, $16, 1

        /* Isolate I$ Line Size */
        sll     t3, t2, 10
        srl     t3, 29

        /* Skip ahead if No I$ */
        beq     t3, zero, nocache
        nop

        li      t6, 2
        sllv    t3, t6, t3      /* Now have true I$ line size in bytes */

        sll     t4, t2, 7
        srl     t4, 29
        li      t6, 64
        sllv    t4, t6, t4      /* I$ Sets per way */

        sll     t5, t2, 13
        srl     t5, 29          /* I$ Assoc (-1) */
        add     t5, 1
        mul     t4, t4, t5      /* Total number of sets */

        lui     t6, 0x8000      /* Get a KSeg0 address for cacheops */

        /* Clear TagLo/TagHi registers */
        mtc0    zero, $28, 0 # TagLo0
        mtc0    zero, $28, 2 # TagLo1
        mtc0    zero, $29 #TagHi

        move    t7, t4

        /*  Index Store Tag Cache Op
         *  Will invalidate the tag entry, clear the lock bit,
         *  and clear the LRF bit
         */
1:      cache   0x8, 0(t6)
        add     t7, -1          /* Decrement set counter */
        bne     t7, zero, 1b
        add     t6, t3  /* Get next line address */

        nop

        /*  Now invalidate the D$ */
        /*
         *  In case assembler does not recognize mf with select
        mfc0 t2, 16, 1
         */
        mfc0 t2, $16, 1


        /* Isolate D$ Line Size */
        sll     t3, t2, 19
        srl     t3, 29

        /* Skip ahead if No D$ */
        beq     t3, zero, nocache
        nop

        li      t6, 2
        sllv    t3, t6, t3      /* Now have true D$ line size in bytes */

        sll     t4, t2, 16
        srl     t4, 29
        li      t6, 64
        sllv    t4, t6, t4      /* D$ Sets per way */

        sll     t5, t2, 22
        srl     t5, 29          /* D$ Assoc (-1) */
        add     t5, 1
	mul     t4, t4, t5      /* Get total number of sets */

        lui     t6, 0x8000      /* Get a KSeg0 address for cacheops */

        /* Clear TagLo/TagHi registers */
        mtc0    zero, $28, 0 # TagLo0
        mtc0    zero, $28, 2 # TagLo1
        mtc0    zero, $29 #TagHi

        move    t7, t4

        /*  Index Store Tag Cache Op
         *  Will invalidate the tag entry, clear the lock bit,
         *  and clear the LRF bit
         */
1:      cache   0x9, 0(t6)
        add     t7, -1          /* Decrement set counter */

        bne     t7, zero, 1b
        add     t6, t3  /* Get next line address */
nocache:
        nop
        jr ra
	nop
	.end  mips_cache_reset

	.ent    mips_cache_lock
mips_cache_lock:
	nop
	jr	ra
	nop
	.end  mips_cache_lock
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
/*******************************************************************************
*
* dcache_status - get cache status
*
* RETURNS: 0 - cache disabled; 1 - cache enabled
*
*/
	.global	dcache_status
	.ent	dcache_status
dcache_status:

	mfc0	v0, CP0_CONFIG
	andi	v0, v0, 1
	j	ra

	.end  dcache_status

/*******************************************************************************
*
* dcache_disable - disable cache
*
* RETURNS: N/A
*
*/
	.global	dcache_disable
	.ent	dcache_disable
dcache_disable:

	mfc0	t0, CP0_CONFIG
	li	t1, -8
	and	t0, t0, t1
	ori	t0, t0, CONF_CM_UNCACHED
	mtc0    t0, CP0_CONFIG
	j	ra

	.end  dcache_disable

