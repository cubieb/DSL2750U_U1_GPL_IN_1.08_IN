

#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
#include <asm/addrspace.h>
#include <asm/cacheops.h>


#define CFG_MT          0x00000380              /* MMU type */
#define CFG_MT_TLB      0x00000080              /* standard TLB (4Kc) */
#define CFG_KU          0x0e000000              /* k/useg cacheability */
#define CFG_K23         0x70000000              /* kseg2/3 cacheability */
#define CFG_KU_S        25
#define CFG_K0MASK      0x00000007              /* KSEG0 coherency algorithm */
#define CFG_K23_S       28	
#define CFG_C_UNCACHED   2
#define C0_TAGLO        $28             /* Low-order portion of cache tag interface */
#define MFC0_T0_CONFIG1 .word 0x40088001        /* Not built into gas */
#define CFG1_DA_S       7
#define CFG1_DA         0x00000380              /* Num of d$ ways - 1 */
#define CFG1_DL         0x00001c00              /* d$ line size */
#define CFG1_DL_S       10
#define CFG1_DS         0x0000e000              /* Num of d$ sets per way */
#define CFG1_DS_S       13
#define CFG1_IA         0x00070000              /* Num of i$ ways - 1 */
#define CFG1_IA_S       16
#define CFG1_IL         0x00380000              /* i$ line size */
#define CFG1_IL_S       19
#define CFG1_IS         0x01c00000              /* Num of i$ sets per way */
#define CFG1_IS_S       22

	
#define DCACHE_SIZE(ways, linesize, depth)	\
	MFC0_T0_CONFIG1;			\
	and	ways, t0, CFG1_DA;		\
	srl	ways, CFG1_DA_S;		\
	add	ways, 1;			\
	and	linesize, t0, CFG1_DL;		\
	srl	linesize, CFG1_DL_S;		\
	addi	linesize, 1;			\
	and	depth, t0, CFG1_DS;		\
	srl	depth, CFG1_DS_S;		\
	li	t0, 64;				\
	sll	depth, t0, depth;		\
	li	t0, 1;				\
	sll	linesize, t0, linesize

#define ICACHE_SIZE(ways, linesize, depth)	\
	MFC0_T0_CONFIG1;			\
	and	ways, t0, CFG1_IA;		\
	srl	ways, CFG1_IA_S;		\
	add	ways, 1;			\
	and	linesize, t0, CFG1_IL;		\
	srl	linesize, CFG1_IL_S;		\
	and	depth, t0, CFG1_IS;		\
	srl	depth, CFG1_IS_S;		\
	addi	linesize, 1;			\
	li	t0, 64;				\
	sll	depth, t0, depth;		\
	li	t0, 1;				\
	sll	linesize, t0, linesize

	.globl	mips_cache_reset	
mips_cache_reset:

	mfc0	t0, CP0_CONFIG
	and	t1, t0, CFG_MT		# check for TLB or FM
	sub	t1, CFG_MT_TLB
	beqz	t1, 1f
	/* 4Kp init all fixed mapping modes -- leave U and 23 uncached */
	and	t0, ~(CFG_KU | CFG_K23)
	or	t0, (CFG_C_UNCACHED << CFG_KU_S) | \
		    (CFG_C_UNCACHED << CFG_K23_S)
1:
	and	t0, ~CFG_K0MASK
	/* for 4Kec  cache mode CFG_C_UNCACHED is write back but 0 is write through */
	mtc0	t0, CP0_CONFIG
	nop
	nop
	/* Inval data cache tags: t1=ways, t2=linesize, t3=depth */
  	mfc0	t1, C0_TAGLO
 	li	t2, 0xfffffC00
  	and	t1, t1, t2
  	mtc0	t1, C0_TAGLO

	DCACHE_SIZE(t1, t2, t3)
	beqz	t3, 2f			# any d$?
	mul	a1, t1, t3		# number of lines (ways * depth)
	li	a0, K0BASE
1:
	cache	9, 0(a0)		# invalidate data line
	add	a0, t2			# p += linesize
	add	a1, -1			# number of lines - 1
	bgtz	a1, 1b			# any left?
2:

	/* Inval instruction cache tags: t1=ways, t2=linesize, t3=depth */
	ICACHE_SIZE(t1, t2, t3)
	beqz	t3, 2f			# any i$?
	mul	a1, t1, t3		# number of lines
	li	a0, K0BASE
1:
	cache	0, 0(a0)		# invalidate instr line
	add	a0, t2			# p += linesize
	add	a1, -1			# number of lines - 1
	bgtz	a1, 1b			# any left?
2:
	nop
	nop
	sync
	j	ra
	
