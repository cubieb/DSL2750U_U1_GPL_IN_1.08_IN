#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>
//#include "ar7100_soc.h"

#define RVECENT(f,n) \
   b f; nop
#define XVECENT(f,bev) \
   b f     ;           \
   li k0,bev

#define	INITIAL_CONFIG0_UNCACHE	0x00000002
#define	INITIAL_CONFIG0_CACHE	0x00000003


#if defined( R4K ) || defined( r4k )
#define SR_CUMASK		0xf0000000	/* Coprocessor usable bits */
#define	SR_CU3			0x80000000	/* Coprocessor 3 usable */
#define SR_CU2			0x40000000	/* coprocessor 2 usable */
#define SR_CU1			0x20000000	/* Coprocessor 1 usable */
#define SR_CU0			0x10000000	/* Coprocessor 0 usable */
#define SR_FR			0x04000000	/* Enable Additional Floating-point Reg.*/
#define SR_RE			0x02000000	/* Reverse Endian Bit	*/
#define SR_DL			0x01000000	/* Data Cache Lock: refill into A disable*/
#define SR_IL			0x00800000	/* Instr. Cahe Lock: Refill to A disabled*/
#define SR_BEV			0x00400000	/* Bootstrap Exception Vector */
#define SR_SR			0x00100000	/* Soft Reset or NMI occurring bit	*/
#define SR_CH			0x00040000	/* Cache Hit(: 1)/Miss(: 0) */
#define SR_CE			0x00020000	/* ECC register set(: 1) */
#define SR_DE			0x00010000	/* Cache parity exception:dis/enabled(1/0)*/

#define SR_IMASK		0x0000ff00	/* Interrupt Mask */
#define SR_IMASK8		0x00000000	/* Interrupt Mask level=8 */
#define SR_IMASK7		0x00008000	/* Interrupt Mask level=7 */
#define SR_IMASK6		0x0000c000	/* Interrupt Mask level=6 */
#define SR_IMASK5		0x0000e000	/* Interrupt Mask level=5 */
#define SR_IMASK4		0x0000f000	/* Interrupt Mask level=4 */
#define SR_IMASK3		0x0000f800	/* Interrupt Mask level=3 */
#define SR_IMASK2		0x0000fc00	/* Interrupt Mask level=2 */
#define SR_IMASK1		0x0000fe00	/* Interrupt Mask level=1 */
#define SR_IMASK0		0x0000ff00	/* Interrupt Mask level=0 */

#define SR_IBIT8		0x00008000	/*  (Intr5) */
#define SR_IBIT7		0x00004000	/*  (Intr4) */
#define SR_IBIT6		0x00002000	/*  (Intr3) */
#define SR_IBIT5		0x00001000	/*  (Intr2) */
#define SR_IBIT4		0x00000800	/*  (Intr1) */
#define SR_IBIT3		0x00000400	/*  (Intr0) */
#define SR_IBIT2		0x00000200	/*  (Software Interrupt 1) */
#define SR_IBIT1		0x00000100	/*  (Software Interrupt 0) */

#define	SR_KX			0x00000080
#define SR_SX			0x00000040
#define SR_UX			0x00000020	/* 1=64 bit enabled; 0=32 bit enabled	*/
#define SR_UM			0x00000010	/* 0=Kernal Mode / 1=User Mode	*/
#define SR_KSU			0x00000018	/* Supervisor Mode.  4600/4700	*/
#define SR_ERL			0x00000004	/* Error Level: 0-normal/1-error	*/
#define SR_EXL			0x00000002	/* Exception Level: 0-normal/1-exception */
#define SR_IE			0x00000001	/* Interrupt Enable, current */
#else	/* end of R4K, start of R3K:	*/
#define SR_CUMASK		0xf0000000	/* Coprocessor usable bits */
#define	SR_CU3			0x80000000	/* Coprocessor 3 usable */
#define SR_CU2			0x40000000	/* coprocessor 2 usable */
#define SR_CU1			0x20000000	/* Coprocessor 1 usable */
#define SR_CU0			0x10000000	/* Coprocessor 0 usable */
#define SR_BEV			0x00400000	/* Bootstrap Exception Vector */
#define SR_TS			0x00200000	/* TLB shutdown */
#define SR_PE			0x00100000	/* Parity Error */
#define SR_CM			0x00080000	/* Cache Miss */
#define SR_PZ			0x00040000	/* Parity Zero */
#define SR_SWC			0x00020000	/* Swap Caches */
#define SR_ISC			0x00010000	/* Isolate Cache */

#define SR_IMASK		0x0000ff00	/* Interrupt Mask */
#define SR_IMASK8		0x00000000	/* Interrupt Mask level=8 */
#define SR_IMASK7		0x00008000	/* Interrupt Mask level=7 */
#define SR_IMASK6		0x0000c000	/* Interrupt Mask level=6 */
#define SR_IMASK5		0x0000e000	/* Interrupt Mask level=5 */
#define SR_IMASK4		0x0000f000	/* Interrupt Mask level=4 */
#define SR_IMASK3		0x0000f800	/* Interrupt Mask level=3 */
#define SR_IMASK2		0x0000fc00	/* Interrupt Mask level=2 */
#define SR_IMASK1		0x0000fe00	/* Interrupt Mask level=1 */
#define SR_IMASK0		0x0000ff00	/* Interrupt Mask level=0 */

#define SR_IBIT8		0x00008000	/*  (Intr5) */
#define SR_IBIT7		0x00004000	/*  (Intr4) */
#define SR_IBIT6		0x00002000	/*  (Intr3) */
#define SR_IBIT5		0x00001000	/*  (Intr2) */
#define SR_IBIT4		0x00000800	/*  (Intr1) */
#define SR_IBIT3		0x00000400	/*  (Intr0) */
#define SR_IBIT2		0x00000200	/*  (Software Interrupt 1) */
#define SR_IBIT1		0x00000100	/*  (Software Interrupt 0) */

#define SR_KUO			0x00000020	/* Kernel/User mode, old */
#define SR_IEO			0x00000010	/* Interrupt Enable, old */
#define SR_KUP			0x00000008	/* Kernel/User mode, previous */
#define SR_IEP			0x00000004	/* Interrupt Enable, previous */
#define SR_KUC			0x00000002	/* Kernel/User mode, current */
#define SR_IEC			0x00000001	/* Interrupt Enable, current */
#endif	/*	R3K	*/


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



#---cathy, modem power on issue in MAC OS
#---set reg(0xf2)=0x10; to remove pull-up resistor
	li		t0,0x6110e100
	li		t1,0xb8003314
	sw		t0,0(t1)
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	li		t0,0x10024002
	li		t1,0xb8030034
	sw		t0,0(t1)
	nop
	li		t0,0x10024000
	li		t1,0xb8030034
	sw		t0,0(t1)
	nop
	li		t0,0x10024002
	li		t1,0xb8030034
	sw		t0,0(t1)
	nop
	li		t0,0x100F4002
	li		t1,0xb8030034
	sw		t0,0(t1)
	nop
	li		t0,0x100F4000
	li		t1,0xb8030034
	sw		t0,0(t1)
	nop
	li		t0,0x100F4002
	li		t1,0xb8030034
	sw		t0,0(t1)
	nop
		

#---set reg(0xf0)=0xfc
        li              t0,0x61fce100
        li              t1,0xb8003314
        sw              t0,0(t1)
        nop
        li              t0,0x10004002
        li              t1,0xb8030034
        sw              t0,0(t1)
        nop
        li              t0,0x10004000
        li              t1,0xb8030034
        sw              t0,0(t1)
        nop
        li              t0,0x10004002
        li              t1,0xb8030034
        sw              t0,0(t1)
        nop
        li              t0,0x100F4002
        li              t1,0xb8030034
        sw              t0,0(t1)
        nop
        li              t0,0x100F4000
        li              t1,0xb8030034
        sw              t0,0(t1)
        nop
        li              t0,0x100F4002
        li              t1,0xb8030034
        sw              t0,0(t1)
        nop


       // mfc0    k0,C0_EPC               # save for nvram
        //move    k1,ra                   # save for nvram
       // mfc0    gp,C0_SR
        //mfc0    t7,C0_CAUSE
        li      v0,SR_BEV
        mtc0    v0,CP0_STATUS                # state unknown on reset
        mtc0    zero,CP0_CAUSE           # clear software interrupts
        nop                             # paranoia


//__romReboot:
	/* second entry point -- sw reboot inherits a0 start type            */
	//.set	noreorder
	li	t0, SR_CU1 | SR_BEV
	nop
	mtc0	t0, CP0_STATUS		/* disable all interrupts fpa on,    */
	nop
	nop
				   	/* prom exception handlers           */
	mtc0	zero, CP0_CAUSE		/* clear all interrupts              */
	nop
	nop

	#---Disable WatchDog
	li		t0,0xA5800000
	li		t1,0xB9C0103C
	#tylo, for 8672 fpga
	#sw		t0,0(t1)
	#---Disable GIMR/GISR
	li		t0,0x0000FFFF
	li		t1,0xB9C03010
	#tylo, for 8672 fpga
	#sw		t0,0(t1)
	#---Reset GPIO
	#tylo, set 8672 memory configuration related parameters
	#li		t0,0x52480000
	#li		t1,0xb8001004
	#sw		t0,0(t1)
	li		t0,0x6d0b0ac0
	li		t1,0xb8001008
	sw		t0,0(t1)
	nop
	
	li		t0,0x00C00406
	li		t1,0xb8003304
	sw		t0,0(t1)
	nop


	#--- config lexra bus and sdram clock freque
	li		t1, SCCR
	lw		t0, 0(t1)
	nop

#if 0
	# check if chip version is RL8671B/BH
	srl 		t0, t0, 0x10
	andi		t0, t0, 0x10
	bnez 		t0, RL8672
	nop
	li		t0, 0x0000140b
	li		t1, 0xb8003304
	sw		t0, 0(t1)
	nop
	li		t0, 0x00028000
	li		t1, 0xb800332C
	sw		t0, 0(t1)
	nop		
	li		t1, SCCR
	lw		t0, 0(t1)
	nop	
#endif

	/* setup bootloader stack */
	li		t0,0x90060000
	li		t1,0xb8003300
	sw		t0,0(t1)
	nop
#if 0 // move forward for patch RL8671B/BH	
	li		t0,0x00000406
	li		t1,0xb8003304
	sw		t0,0(t1)
	nop
#endif	
	li		t0,0x3ff80000
	li		t1,0xb8001200
	sw		t0,0(t1)
	nop
	#with packet processor
	#li		t0,0x800008f0
	#without packet processor
	li		t0,0x000000f0
	li		t1,0xb800330c
	sw		t0,0(t1)
	nop


	#--- invalidate the icache and dcache with a 0->1 transition
	mtc0	zero, $20
	nop
	nop

	li	t0, 0x202
	mtc0	t0, $20
	nop
	nop
	mtc0	zero, $20
	nop
	nop
	#--- initialize and start COP3
	mfc0	t0,$12
	nop
	or		t0,0x80000000
	mtc0	t0,$12
	nop
	nop
	
	# disable IRAM
	li		t0, 0x84000000	#address without SDRAM
	and		t0, 0x0ffffc00    # translate to physical address
	mtc3	t0, $0      # $0: i-ram base
	nop
	nop
	li		t0, 4096 - 1
	mtc3	t0, $1      # $1: i-ram top
	nop
	nop

	# disable DRAM
	li	t0, 0x85000000	#address without SDRAM
	and	t0, 0x0ffffc00    # translate to physical address
	mtc3	t0, $4      # $4: d-ram base
	nop
	nop
	li	t0, 4096 - 1
	mtc3	t0, $5      # $5: d-ram top
	nop
	nop
	#--- enable icache and dcache
	mtc0	$0, $20	# CCTL
	nop
	nop


		#--- configure memory controller
	#---this is for ASIC

#--- determine memory configuraton 
//#ifdef SDRAM_AUTO_DETECT
	# here we auto detect the sdram size. according Designer's info's on memory controller behavior
		# use address range = 0x0000_0000 ~ 0x03FF_FFFF
		#input address 0xA3F0_1234 => physical address = 0x03F0_1234
		#predefine 16bits (bytes) DRAM => DRAM address = 0x03F0_1234 / 2 = 0x1F8_091A
		#column address range a0~a9 (0x400) => 0x1F8_091A mod 0x400 = 11A (COL address)
		#                                                0x1F8_091A / 0x400 = 0x7E02 ( for ROW / BANK)
		#row address range a0~a12 (0x2000)  => 0x7E02 mod 0x2000 = 1E02 (ROW address)
		#                                                                    0x7E02 / 0x2000 = 3 (BANK address)
	# we have conclusion on MCR with 64MB setting:
		#2MB *(0xa3f01234) == *(0xa1f01234)
		#8MB *(0xa3f01234) == *(0xA3F01034)
		#16MB *(0xa3f01234) == *(0xA3701634)
		#32MB *(0xa3f01234) == *(0xA3F01634)
		#64MB *(0xa3f01234) unique
		li  	t0, 0xb8001004
		li	t1, 0x54880000
		#set MCR to 64MB setting temprory
		sw	t1, 0(t0)
		nop
		li	t2, 0xAAAA5555
		li	t3, 0xA3f01234
		sw	$0, 0(t3)
		li	t3, 0xa1f01234
		sw	$0, 0(t3)
		li	t3, 0xA3F01034
		sw	$0, 0(t3)
		li	t3, 0xA3701634
		sw	$0, 0(t3)
		li	t3, 0xA3F01634
		sw	$0, 0(t3)		
		nop
		li	t3, 0xA3F01234
		sw	t2, 0(t3)
		nop
		li	t3, 0xA1f01234
		lw	t4, 0(t3)
		nop
		beq  t4, t2,  SDRAM_2MB
		nop
		li	t3, 0xA3F01034
		lw 	t4, 0(t3)
		nop
		beq  t4, t2, SDRAM_8MB
		nop
		li	t3, 0xA3701634
		lw 	t4, 0(t3)
		nop
		beq  t4, t2, SDRAM_16MB
		nop
		li	t3, 0xA3F01634
		lw 	t4, 0(t3)
		nop
		beq  t4, t2, SDRAM_32MB
		nop
                # Otherwise, 64MB is the only possible setting
		b    SDRAM_64MB
		nop		
SDRAM_2MB:
		li	t2, 0x50000000
		b	setting_mcr
		nop
SDRAM_8MB:
		li	t2, 0x52080000
		b	setting_mcr
		nop		
SDRAM_16MB:
		li	t2, 0x52480000
		b	setting_mcr
		nop		
SDRAM_32MB:
		li	t2, 0x54480000
		b	setting_mcr
		nop		
SDRAM_64MB:
		li	t2, 0x54880000
		b	setting_mcr
		nop		
setting_mcr:
		li	t3, 0xb8001004
		nop
		sw	t2, 0(t3)
		nop

	
//#endif //SDRAM_AUTO_DETECT


#ifdef SUPPORT_8672_PLATFORM
	#--- config lexra bus and sdram clock freque
	li		t1, SCCR
	lw		t0, 0(t1)
	nop
#if defined(SYS_CLOCK_175MHZ) || defined(SDRAM_CLOCK_166MHZ) || defined(CPU_CLOCK_340MHZ)
    srl     t0, t0, 12
    sll     t0, t0, 12
#ifdef CONFIG_SDRAM_TYPE_5
#if defined(SYS_CLOCK_175MHZ)
    addi        t0, t0, 0x00
#endif
#if defined(SDRAM_CLOCK_166MHZ)
    addi        t0, t0, 0x0f
#endif
#else
#if defined(SYS_CLOCK_175MHZ)
    addi        t0, t0, 0x40
#endif
#if defined(SDRAM_CLOCK_166MHZ)
    addi        t0, t0, 0x03
#endif

#endif
#if defined(CPU_CLOCK_340MHZ)
    addi        t0, t0, 0x600
#endif
    sw      t0, 0(t1)
    nop
#endif

#endif

	
/*
 * t0 = addr_sp
 * t2 = destination address
 */
	li	t0, CONFIG_DRAM_BASE + CFG_SDRAM_SIZE - SDRAM_SIZE + SP_OFFSET
	move	sp, t0		/* Set new stack pointer		*/

	li	t0, TEXT_BASE_BOOTSTART
	la	t3, in_ram
	nop
	nop
	nop
	lw	t2, -12(t3)	/* t2 <-- uboot_end_data	*/
	nop
	nop
	nop
	
	
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
	nop
	nop
	nop
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
	nop
	nop
	nop
	addi	t4, gp, 8	/* Skipping first two entries.	*/
	li	t2, 2
1:
	lw	t1, 0(t4)
	nop
	nop
	nop
	beqz	t1, 2f
	add	t1, t6
	sw	t1, 0(t4)
2:
	addi	t2, 1
	blt	t2, t3, 1b
	addi	t4, 4		/* delay slot			*/


	mtc0	zero, $20
	nop
	nop
	li	t9, 0x202

	mtc0	t9, $20
	nop
	nop
	mtc0	zero, $20
	nop
	nop	

	li	t9, SR_CU1|SR_CU0
	nop
	mtc0	t9, CP0_STATUS		/* put on processor	*/
	nop
	nop

	mtc0	zero, CP0_CAUSE
	nop
	nop
	nop

	/* Clear BSS.
	 */
	lw	t1, -12(t0)	/* t1 <-- uboot_end_data	*/
	nop
	nop
	nop
	lw	t2, -8(t0)	/* t2 <-- uboot_end		*/
	nop
	nop
	nop
	move a0 , t1
	add	t1, t6		/* adjust pointers		*/
	add	t2, t6

	sub	t1, 4
1:	addi	t1, 4
	blt	t1, t2, 1b
	sw	zero, 0(t1)	/* delay slot			*/

	la	t9, uncompress_bootloader_second 
	nop
	nop
	nop
	j	t9
	nop

	/* Exception handlers.
	 */
romReserved:
	b romReserved
romExcHandle:
	b romExcHandle
