/* asmlexra.h - assembler definitions header file for ADI Lexra core
	Don't use for other chipset
	Leo @ Analog Devices in Mountain View 2004.
*/

#ifndef _ASMLEXRA_H_
#define _ASMLEXRA_H_

#ifdef __cplusplus
extern "C" {
#endif

/*
*  MIPS register definitions
*/

#define zero	$0	/* wired zero */
#define AT	$at	/* assembler temp */
#define v0	$2	/* return reg 0 */
#define v1	$3	/* return reg 1 */
#define a0	$4	/* arg reg 0 */
#define a1	$5	/* arg reg 1 */
#define a2	$6	/* arg reg 2 */
#define a3	$7	/* arg reg 3 */
#define t0	$8	/* caller saved 0 */
#define t1	$9	/* caller saved 1 */
#define t2	$10	/* caller saved 2 */
#define t3	$11	/* caller saved 3 */
#define t4	$12	/* caller saved 4 */
#define t5	$13	/* caller saved 5 */
#define t6	$14	/* caller saved 6 */
#define t7	$15	/* caller saved 7 */
#define s0	$16	/* callee saved 0 */
#define s1	$17	/* callee saved 1 */
#define s2	$18	/* callee saved 2 */
#define s3	$19	/* callee saved 3 */
#define s4	$20	/* callee saved 4 */
#define s5	$21	/* callee saved 5 */
#define s6	$22	/* callee saved 6 */
#define s7	$23	/* callee saved 7 */
#define t8	$24	/* caller saved 8 */
#define t9	$25	/* caller saved 9 */
#define k0	$26	/* kernel temp 0 */
#define k1	$27	/* kernel temp 1 */
#define gp	$28	/* global pointer */
#define sp	$29	/* stack pointer */
#define s8	$30	/* callee saved 8 */
#define ra	$31	/* return address */

/*
* MIPS Coprocessor 0 regs, ADI core special
*/
#define	C0_INX		$0		/* tlb index */
#define	C0_RAND		$1		/* tlb random */
#define	C0_TLBLO	$2		/* tlb entry low */
#define	C0_CTXT		$4		/* tlb context */
#define	C0_TLBWIRED	$6		/* tlb context */
#define	C0_BADVADDR	$8		/* bad virtual address */
#define	C0_TLBHI	$10		/* tlb entry hi */


#define	C0_SR		$12		/* status register */
#define	C0_CAUSE	$13		/* exception cause */
#define	C0_EPC		$14		/* exception pc */
#define C0_PRID		$15		/* chip ID */
#define C0_DBG		$16		/* debug register */
#define C0_DEPC	$17		/* debug epc */
#define C0_DSAV	$18		/* debug save */
#define C0_CCTL		$20		/* cache controller */
/* Cache control bits */
#define CCTL_DIvl       0x00000001      /* Invalidate Dcache */
#define CCTL_IIvl       0x00000002      /* Invalidate Icache/IRAM */
#define CCTL_ILk1       0x00000008
#define CCTL_ILk2       0x0000000C		/* lock set 1 */


#define CP0_INDEX $0
#define CP0_RANDOM $1
#define CP0_ENTRYLO0 $2
#define CP0_ENTRYLO1 $3
#define CP0_CONF $3
#define CP0_CONTEXT $4
#define CP0_PAGEMASK $5
#define CP0_WIRED $6
#define CP0_INFO $7
#define CP0_BADVADDR $8
#define CP0_COUNT $9
#define CP0_ENTRYHI $10
#define CP0_COMPARE $11
#define CP0_STATUS $12
#define CP0_CAUSE $13
#define CP0_EPC $14
#define CP0_PRID $15
#define CP0_CONFIG $16
#define CP0_LLADDR $17
#define CP0_WATCHLO $18
#define CP0_WATCHHI $19
#define CP0_XCONTEXT $20
#define CP0_FRAMEMASK $21
#define CP0_DIAGNOSTIC $22
#define CP0_PERFORMANCE $25
#define CP0_ECC $26
#define CP0_CACHEERR $27
#define CP0_TAGLO $28
#define CP0_TAGHI $29
#define CP0_ERROREPC $30

/* Lexra CP0 registers */
#define LXC0_STAT	$0
#define LXC0_CAUSE	$1
#define LXC0_IVEC	$2

/*
*  MIPS floating point coprocessor register definitions , We don't have it , undef
*/
#if 0
#define fp0	$f0	/* return reg 0 */
#define fp1	$f1	/* return reg 1 */
#define fp2	$f2	/* return reg 2 */
#define fp3	$f3	/* return reg 3 */
#define fp4	$f4	/* caller saved 0 */
#define fp5	$f5	/* caller saved 1 */
#define fp6	$f6	/* caller saved 2 */
#define fp7	$f7	/* caller saved 3 */
#define fp8	$f8	/* caller saved 4 */
#define fp9	$f9	/* caller saved 5 */
#define fp10	$f10	/* caller saved 6 */
#define fp11	$f11	/* caller saved 7 */
#define fp12	$f12	/* arg reg 0 */
#define fp13	$f13	/* arg reg 1 */
#define fp14	$f14	/* arg reg 2 */
#define fp15	$f15	/* arg reg 3 */
#define fp16	$f16	/* caller saved 8 */
#define fp17	$f17	/* caller saved 9 */
#define fp18	$f18	/* caller saved 10 */
#define fp19	$f19	/* caller saved 11 */
#define fp20	$f20	/* callee saved 0 */
#define fp21	$f21	/* callee saved 1 */
#define fp22	$f22	/* callee saved 2 */
#define fp23	$f23	/* callee saved 3 */
#define fp24	$f24	/* callee saved 4 */
#define fp25	$f25	/* callee saved 5 */
#define fp26	$f26	/* callee saved 6 */
#define fp27	$f27	/* callee saved 7 */
#define fp28	$f28	/* callee saved 8 */
#define fp29	$f29	/* callee saved 9 */
#define fp30	$f30	/* callee saved 10 */
#define fp31	$f31	/* callee saved 11 */

#define C1_IR $0	/* implementation/revision reg */
#define C1_SR $31	/* control/status reg */
#endif 	/* float point CO */

/*
* define aliases for operations that are different in 64bit mode
*/
#define SW	sw
#define LW	lw
#define MFC0	mfc0
#define MTC0	mtc0

/*
 * The following macros are especially useful for __asm__
 * inline assembler.
 */
#ifndef __STR
#define __STR(x) #x
#endif
#ifndef STR
#define STR(x) __STR(x)
#endif

/*
 * Macros to access the system control coprocessor
 */
#define read_32bit_cp0_register(source)                         \
({ int __res;                                                   \
	__asm__ __volatile__(                                   \
	".set\tpush\n\t"					\
	".set\treorder\n\t"					\
	"mfc0\t%0,"STR(source)"\n\t"                            \
	".set\tpop"						\
	: "=r" (__res));                                        \
	__res;})

#define write_32bit_cp0_register(register,value)                \
	__asm__ __volatile__(                                   \
	"mtc0\t%0,"STR(register)"\n\t"				\
	"nop"							\
	: : "r" (value));

#define tlb_write_indexed()                                     \
	__asm__ __volatile__(                                   \
		".set noreorder\n\t"                            \
		"tlbwi\n\t"                                     \
".set reorder")

#ifndef _LANGUAGE_ASSEMBLY
/*
 * Manipulate the status register.
 * Mostly used to access the interrupt bits.
 */
#define __BUILD_SET_CP0(name,register)                          \
extern __inline__ unsigned int                                  \
set_cp0_##name(unsigned int set)				\
{                                                               \
	unsigned int res;                                       \
								\
	res = read_32bit_cp0_register(register);                \
	res |= set;						\
	write_32bit_cp0_register(register, res);        	\
								\
	return res;                                             \
}								\
								\
extern __inline__ unsigned int                                  \
clear_cp0_##name(unsigned int clear)				\
{                                                               \
	unsigned int res;                                       \
								\
	res = read_32bit_cp0_register(register);                \
	res &= ~clear;						\
	write_32bit_cp0_register(register, res);		\
								\
	return res;                                             \
}								\
								\
extern __inline__ unsigned int                                  \
change_cp0_##name(unsigned int change, unsigned int new)	\
{                                                               \
	unsigned int res;                                       \
								\
	res = read_32bit_cp0_register(register);                \
	res &= ~change;                                         \
	res |= (new & change);                                  \
	if(change)                                              \
		write_32bit_cp0_register(register, res);        \
								\
	return res;                                             \
}

__BUILD_SET_CP0(status,CP0_STATUS)
__BUILD_SET_CP0(cause,CP0_CAUSE)
__BUILD_SET_CP0(config,CP0_CONFIG)

#endif /* defined (_LANGUAGE_ASSEMBLY) */

/*
 * Bitfields in the  cp0 status register.
 */
#define ST0_IEC                 0x00000001
#define ST0_KUC			0x00000002
#define ST0_IEP			0x00000004
#define ST0_KUP			0x00000008
#define ST0_IEO			0x00000010
#define ST0_KUO			0x00000020
#define ST0_ISC			0x00010000	
#define ST0_SWC			0x00020000
#define ST0_CM			0x00080000


/*
 * Status register bits available in all MIPS CPUs.
 */
#define ST0_IM			0x0000ff00
#define  STATUSB_IP0		8
#define  STATUSF_IP0		(1   <<  8)
#define  STATUSB_IP1		9
#define  STATUSF_IP1		(1   <<  9)
#define  STATUSB_IP2		10
#define  STATUSF_IP2		(1   << 10)
#define  STATUSB_IP3		11
#define  STATUSF_IP3		(1   << 11)
#define  STATUSB_IP4		12
#define  STATUSF_IP4		(1   << 12)
#define  STATUSB_IP5		13
#define  STATUSF_IP5		(1   << 13)
#define  STATUSB_IP6		14
#define  STATUSF_IP6		(1   << 14)
#define  STATUSB_IP7		15
#define  STATUSF_IP7		(1   << 15)
#define  STATUSB_IP8		0
#define  STATUSF_IP8		(1   << 0)
#define  STATUSB_IP9		1
#define  STATUSF_IP9		(1   << 1)
#define  STATUSB_IP10		2
#define  STATUSF_IP10		(1   << 2)
#define  STATUSB_IP11		3
#define  STATUSF_IP11		(1   << 3)
#define  STATUSB_IP12		4
#define  STATUSF_IP12		(1   << 4)
#define  STATUSB_IP13		5
#define  STATUSF_IP13		(1   << 5)
#define  STATUSB_IP14		6
#define  STATUSF_IP14		(1   << 6)
#define  STATUSB_IP15		7
#define  STATUSF_IP15		(1   << 7)
#define ST0_CH			0x00040000
#define ST0_SR			0x00100000
#define ST0_BEV			0x00400000
#define ST0_RE			0x02000000
#define ST0_FR			0x04000000
#define ST0_CU			0xf0000000
#define ST0_CU0			0x10000000
#define ST0_CU1			0x20000000
#define ST0_CU2			0x40000000
#define ST0_CU3			0x80000000
#define ST0_XX			0x80000000	/* MIPS IV naming */

/*
 * Bitfields and bit numbers in the coprocessor 0 cause register.
 *
 * Refer to your MIPS R4xx0 manual, chapter 5 for explanation.
 */
#define  CAUSEB_EXCCODE		2
#define  CAUSEF_EXCCODE		(31  <<  2)
#define  CAUSEB_IP		8
#define  CAUSEF_IP		(255 <<  8)
#define  CAUSEB_IP0		8
#define  CAUSEF_IP0		(1   <<  8)
#define  CAUSEB_IP1		9
#define  CAUSEF_IP1		(1   <<  9)
#define  CAUSEB_IP2		10
#define  CAUSEF_IP2		(1   << 10)
#define  CAUSEB_IP3		11
#define  CAUSEF_IP3		(1   << 11)
#define  CAUSEB_IP4		12
#define  CAUSEF_IP4		(1   << 12)
#define  CAUSEB_IP5		13
#define  CAUSEF_IP5		(1   << 13)
#define  CAUSEB_IP6		14
#define  CAUSEF_IP6		(1   << 14)
#define  CAUSEB_IP7		15
#define  CAUSEF_IP7		(1   << 15)
#define  CAUSEB_IV		23
#define  CAUSEF_IV		(1   << 23)
#define  CAUSEB_CE		28
#define  CAUSEF_CE		(3   << 28)
#define  CAUSEB_BD		31
#define  CAUSEF_BD		(1   << 31)

/*
 * Stack frame allocation.
 * These macros are used in assembly language routines to automate the
 * allocation and use of stack frames.
 */
#define _RTypeSize	4
#define Fsize 4

/* Return the size of the frame for the named routine */
#define FRAMESZ(routine)	_##routine##Fsize
/*
* Calculate the frame size for the named routine
* 16 bytes is allocated for subroutines to store the 32bit argument registers
* 1 register location is allocated for saving RA
* nregs register locations are reserved for storing locally used registers
*/
#define SETFRAME(routine,nregs) \
	FRAMESZ(routine) = (4*4)+_RTypeSize*(1+(nregs))
/* The location at which to store the return address */
#define FRAMERA(routine) \
	FRAMESZ(routine)-_RTypeSize
/* Locations at which to store locally used registers */
#define FRAMER(routine,regn) \
	(4*4)+(_RTypeSize*(regn))
#define FRAMER0(routine) FRAMER(routine,0)
#define FRAMER1(routine) FRAMER(routine,1)
#define FRAMER2(routine) FRAMER(routine,2)
#define FRAMER3(routine) FRAMER(routine,3)
/* Locations at which to store 32bit argument registers */
#define FRAMEA(routine, regn) \
	FRAMESZ(routine)+4*(regn)
#define FRAMEA0(routine) FRAMEA(routine,0)
#define FRAMEA1(routine) FRAMEA(routine,1)
#define FRAMEA2(routine) FRAMEA(routine,2)
#define FRAMEA3(routine) FRAMEA(routine,3)

#ifdef __cplusplus
}
#endif

#endif 
