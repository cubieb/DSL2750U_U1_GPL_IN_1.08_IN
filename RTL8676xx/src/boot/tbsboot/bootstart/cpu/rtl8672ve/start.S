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

//#if 1 //Diable Init
##############################################
# reg b800_1200: bit31-29,0:RAM clk/2, 1: RAM clk/4, 2: RAM clk/6, 3: RAM clk/8
# Set default spi clock as RAM clk/6, e.g.200Mhz/6 = 33.333Mhz
	li		t0,0x5ff80000
	li		t1,0xb8001200
	sw		t0,0(t1)
	nop
	

###############################################	
//	Async mode setting.
        li      t1,0xb800332c   
        lw      t0, 0(t1)
        nop
        and     t0,t0,~0x18000000    //make sure [n28..n27] =0
	sw		t0,0(t1)
	nop
##############################################

#---cathy, modem power on issue in MAC OS
#---set reg(0xf2)=0x10; to remove pull-up resistor
	li		t0,0x6110e100
	li		t1,0xb8003314
	sw		t0,0(t1)
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
	


	#--- config lexra bus and sdram clock freque
	li		t1, SCCR
	lw		t0, 0(t1)
	nop

# check SCCR bit 21,20 and 16
	srl 		t0, t0, 0x10
	andi		t0, t0, 0x00000031
	beq 		t0, 17, AAA 
	nop
        beq             t0, 1, BBB  
        nop
        beq             t0, 48, CCC 
        nop
        beq             t0, 49, CCC 
        nop
	li		t0, 0x0000140b
	li		t1, 0xb8003304
	sw		t0, 0(t1)
	nop
	li		t0, 0x00028000
	li		t1, 0xb800332C
	sw		t0, 0(t1)
	nop		
	li		t0,0x90060000
	li		t1,0xb8003300
	sw		t0,0(t1)
	nop
	#with packet processor
	#li		t0,0x800008f0
	#without packet processor
	li		t0,0x000000f0
	li		t1,0xb800330c
	sw		t0,0(t1)
	nop
# SDRAM/DDR Memory timing register, MUST set before MCR(config. reg)

	li		t0,0x6d0b0ac0
	li		t1,0xb8001008
	sw		t0,0(t1)
	nop

        b               MEM_CONF_DONE
        nop
	
AAA:
	li		t0,0x00000010		
	li		t1,0xb8003338
	sw		t0,0(t1)
	nop		
	li		t0,0x4CC3106D		
	li		t1,0xb8003204
	sw		t0,0(t1)
	nop
	nop
	nop
	li	t1,100
1:
	subu		t1,1
	nop
	nop
	bnez		t1,1b
	nop	
	nop		
###############################################		
    li      t1,0xb8003318       
	lw		t0, 0(t1)
	nop	
    and     t0,t0,~0x00000200    //make sure bit9 =0
    sw      t0, 0(t1)
    nop
## 0xb8003200 n0~3=0
	//li		t0,0xFFDF0017		//340,170,133
	//li		t0,0xFFDF0007		//340,180,133
	//li		t0,0xFFDF0d07		//400,180,133
	//li		t0,0xFFDF0d00		//400,180,166
	li		t0,0xFFDF0d07		//400,180,133*
	//li		t0,0xFFDF1b37		//260,150,133
	//li		t0,0xFFDF1b07		//260,180,133
	//li		t0,0xFFDF1607		//470,180,133
	//li		t0,0xFFDF1600		//470,180,166
	
	//li		t0,0xFFDF1c67			// for ext. 25MHz clock input, 400,212.5,133
	//li		t0,0xFFDF1c60			// for ext. 25MHz clock input, 400,212.5,166
	li		t1,0xb8003200
	sw		t0,0(t1)
	nop
        // following nops are necessary for clk settle after above clk configuration
	nop
	nop
	nop
	nop
	li		t0,0x300d		//SDRAM TX/RX delay parameter setup
	//li		t0,0x00c0040f		//SDRAM TX/RX delay parameter setup, for 216pin QA
	li		t1,0xb8003304
	sw		t0,0(t1)
	nop	
	nop	
	#for gmac
	li		t0,0x90002000		//GPA6/25Mout select, default 25Mout [n12=0] 
	//li		t0,0x90003000		//GPA6/25Mout select, disable 25Mout, as normal gpio pin [n12=1] 
	li		t1,0xb8003300
	sw		t0,0(t1)
	nop
	
	li		t0,0xfb001fff			//enable all
	li		t1,0xb800330c
	sw		t0,0(t1)
	nop			
#######PCIe MDIO and MAC reset  ###################
	li		t0,0x1d400000
	li		t1,0xb8003400
	sw		t0,0(t1)
	nop		
	li		t0,0x1d500000
	li		t1,0xb8003400
	sw		t0,0(t1)
	nop		
	li		t0,0xfb0015ff
	li		t1,0xb800330c
	sw		t0,0(t1)
	nop		
	li		t0,0xfb0017ff
	li		t1,0xb800330c
	sw		t0,0(t1)
	nop		
###################################
## Int. LDO parameters setup ######
/*
<1>Intend to set page1 register 23 RG7X<8:6> as constant 100
ew 0xb801805c=0x841f0001      // change to int-phy page 1
ew 0xb801805c=0x84170b23      // set reg23=0b23(default 0ba3)
 
<2>Intend to pull high ethernet 100Mhz output voltage to around 1.0(v)
ew 0xb801805c=0x841f0001    // change to int-phy page 1
ew 0xb801805c=0x84135c00    // set reg19=5c00(default 6400)
*/	
#if 1
	li		t0,0x841f0001		//int-phy page select : change to page 1
	li		t1,0xb801805c
	sw		t0,0(t1)
	nop
	li		t2,100
1:
	subu		t2,1
	nop
	nop
	bnez		t2,1b
	nop

	li		t0,0x84170b23
	li		t1,0xb801805c
	sw		t0,0(t1)
	nop
	li		t2,100
1:
	subu		t2,1
	nop
	nop
	bnez		t2,1b
	nop
	// For ethernet MAC: set the value of band gap, finetune LDO driving
	li		t0,0x84134400
	li		t1,0xb801805c
	sw		t0,0(t1)
	nop
	li		t2,100
1:
	subu		t2,1
	nop
	nop
	bnez		t2,1b
	nop
	
	li		t0,0x841f0000			//int-phy page select : change to page 0
	li		t1,0xb801805c
	sw		t0,0(t1)
	nop		
	li		t2,100
1:
	subu		t2,1
	nop
	nop
	bnez		t2,1b
	nop	
#endif	
	li		t0,0xb8001000		//detect DDR or SDR on board.
	lw		t0,0(t0)
	and		t1,t0,0x80000000
	nop
	beqz	       t1,is_SDRAM
	nop
	nop
###############################################
# SDRAM/DDR Memory timing register, MUST set before MCR(config. reg)

    	li      t0,0x6CEB84C0       //DDR 133MHz, (3,3,7,11,8,2,3)*
	//li        t0,0x910DA4C0       //DDR 166MHz, (4,4,8,13,10,2,3)*
    	//li        t0,0x912FB500       //DDR 200MHz, (4,4,9,15,11,2,4)*
	li		t1,0xb8001008
	sw		t0,0(t1)
	nop	
	nop
###############################################	
//	SDR & DDR I/O PAD current select for test only. 
	li		t0,0x0c00
	li		t1,0xb8003310
	sw		t0,0(t1)
	nop		
	nop
	
###############################################
	//li		t0,0x00000000		//For Calibration use, clk_m delay set to zero [n8..n11]
	li		t1,0xb800332c
       li              t2, 0xfffff0ff
       lw              t0,0(t1)
       nop
       and             t0, t0, t2
       nop
       sw              t0, 0(t1)
       nop
###############################################		

	//li		t0,0x00b4A104		//default enable spreading spectrum n[23]=0, for DDR have to disable.
	li		t1,0xb8003334
	li		t2,0x800000			//default enable spreading spectrum n[23]=0, for DDR have to disable.
	lw		t0,0(t1)
	nop
	or		t0,t0,t2
	nop
	sw		t0,0(t1)	
	nop
# FIXME! Need real calibration!!
	li		t0,0x54a00000		//166MHz, PLL analog calibration value for DDR only.*
	nop
	li		t1,0xb8001050
	sw		t0,0(t1)
	nop		
 	nop
 	nop
 	nop
 	nop

###############################################
	//li		t0,0x52480000		//16M, for SDRAM & DDR
	li		t0,0x544b0000		//32M, for SDRAM & DDR
	//li		t0,0x9a480000		//32M, only for DDR dual 
	//li		t0,0x94480000		//32M, only for DDR
	//li		t0,MCR0_VAL
	li		t1,0xB8001004							//initial DDR signal re-send
	sw		t0,0(t1)
	nop
###############################################
	li	t1,1000
1:	
	subu		t1,1
	nop
	nop
	bnez		t1,1b
	nop	
	nop	
#################################################
#if 0
#define SW_DRAM_ADR t0
#define SW_DLL0_ADR t2
#define CORNER2_flag t6
#define CORNER1_L s0
#define CORNER1_R s1
#define L0 s3
#define R0 s4
#define DONE_BIT	s5
#if	1
////////////enable GDMA module 
//enable GDMA module 0xb800330c[n10]=1
	li	t1,0xb800330c
	nop
	lw	t7,0(t1)			//load 0xb800330c
	nop
	ori	t7,t7,0x400		//set 0xb800330c[n10]=1
 	nop
 	sw	t7,0(t1)			//enable GDMA module
#endif

CALIBRATE_DLL0_BEGIN:

///////////////////////////////////////////////
 	//li	t8, 0x00000000 			//use	buffer delay cell
 	li	t8, 0x00000001 			//use	inverter delay cell
 	li	t5, 0						// save calibration window
 	li 	CORNER2_flag,0			// flag reset.
 	li	s2, 31					//Calibartion first corner for 32TAP test 
 	move	t4, zero 	
 	nop
 	nop
CALIBRATE_DLL0_START:
 	move 	DONE_BIT,zero
 	li	SW_DLL0_ADR, 0xB8003330    
 	
 	move	L0, zero
 	move	R0, zero
 	nop
 	nop   
CALIBRATE_DLL0_LOOP1:
	bltu	s2, t4, CALIBRATE_DLL0_LOOP1_OUT		//if s2 < t4 and then jump to label
	nop
	nop
	sll	t7, t4, 1															//sll r, s, c  :  r <- shift s left c bits,
	nop
	nop
	//bnez		CORNER2_flag,CORNER2_Proc
	nop
	or	t7, t7, t8														//or r, s, t   :  r <-  s | t
	//.set noreorder
	sw	t7, 0(SW_DLL0_ADR)										//store data to DLL0
	nop
 	nop
 	nop
 	nop
	nop
	nop
	
	//.set reorder
	//and	t7, t7, t5														//and
	nop
CALIBRATE_DLL0_L0_SET:
	bnez		DONE_BIT,CALIBRATE_DLL0_R0_SET						//got L0 jump to CALIBRATE_DLL0_R0_SET
	//bne	L0, zero, CALIBRATE_DLL0_R0_SET						//bne r, s, l   r != s,jump to label
    	nop
	nop
////////////Clean data from 0xa0005000 to 0xa0005100
	li		t7,0xa0005100
	li		t1,0xa0005000
	li		t3,0xa5a55a5a				//destination area default pattern
1:	
	nop
	nop
	//sw		t1,0(t1)
	sw		t3,0(t1)
	nop
	nop
	addi		t1,t1,4
	nop
	nop
	bne		t1,t7,1b
	nop		
////////////GEN data from 0xa0001000 to 0xa0001100
	li		t7,0xa0001110
	li		t1,0xa0001000
	li		t3,0x5a5aa5a5				//change pattern
#if	1
1:	
	nop
	nop
	//sw		t1,0(t1)
	sw		t3,0(t1)
	nop
	nop
	addi		t1,t1,4
	nop
	nop
	bne		t1,t7,1b
	nop
#else
1:	
	nop
	li		t3,0x5a5aa5a5	
	nop
	sw		t3,0(t1)
	nop
	addi		t1,t1,4
	nop
	li		t3,0x0f0ff0f0
	nop
	sw		t3,0(t1)
	nop
	addi		t1,t1,4
	nop
	li		t3,0x0000ffff
	nop
	sw		t3,0(t1)
	nop
	addi		t1,t1,4
	nop
	li		t3,0x12345678
	nop
	sw		t3,0(t1)
	nop
	addi		t1,t1,4
	nop
	//bne		t1,t7,1b
	bgt		t7,t1,1b
	nop
	nop
#endif
////////////GDMA access 
/////////////////////////////////////////////
 	li	t1,0xb800a000
 	li	t7,0x0
 	sw	t7,0(t1)			//Software reset GDMA, 0xb800a000[n31]=0	-> 1
 	nop
 	nop
 	li	t7,0x80000000
 	sw	t7,0(t1)			//Enable GDMA, 0xb800a000[n31]=1, and func set to memory copy [n27..n24]=0
 	nop
 	nop
 	li	t7,0x0
 	li	t1,0xb800a004
 	sw	t7,0(t1)			//Interrupt Mask Register clean
 	nop
 	nop
 	li	t7,0x90000000
 	li	t1,0xb800a008
 	sw	t7,0(t1)			//Interrupt status Register, [n31] and [n28] write 1 to clean
 	nop
 	nop
 ///////////memory copy (setup source)/////////////////////////////////
 	li	t7,0xa0001000	//source data address
 	li	t1,0xb800a020
 	sw	t7,0(t1)			//setting source data address
 	nop
 	nop
  	li	t7,0x80000100			//[n31]=1, last data block, and source data length[n12..n0]
 	li	t1,0xb800a024
 	sw	t7,0(t1)					//setting source data length
 	nop
 	nop
/////////////memory copy (setup destination)/////////////////////////////////
 	li	t7,0xa0005000	//destination data address
 	li	t1,0xb800a060
 	sw	t7,0(t1)			//setting destination data address
 	nop
 	nop
 	li	t7,0x80000100			//[n31]=1, last data block, and destination data length[n12..n0]
 	li	t1,0xb800a064
 	sw	t7,0(t1)					//setting destination data length
 	nop
 	nop  
//////////////startting  GDMA process 	/////////////////////////////////
  	li	t7,0xC0000000			//GDMA_ENABLE | GDMA_POLL | GDMA_MEMCPY	//startting  GDMA process
 	li	t1,0xb800a000
 	sw	t7,0(t1)					//startting  GDMA 
 	nop
 	nop  	
//////////////polling GDMA copy done flag	/////////////////////////////////
  	li			t7,0x0					// clean data buffer
  	li			t5,0x80000000
 	li			t1,0xb800a008			// GDMA	Interrupt Status Register
1:
	nop
	nop
 	lw			t7,0(t1)					// load 
 	nop
 	nop
 	and 		t7,t7,t5	
 	nop
 	beqz		t7,1b				//GDMA_not_complete
 	nop
 	nop  	
//////////software compare data 
	li	t5,0
	li	t7,0xa0001000
	li	t1,0xa0005000
#if	1
1:	
	addi	t1,t1,4
	nop
	addi	t7,t7,4
	nop
	beq	t1,0xa0005100,data_pass_L0
	nop
	lw	t5,0(t1)
	nop
	nop
	nop
	nop
	
	//beq	t7,t5,1b
	beq	t3,t5,1b
	nop
	nop
	b	CALIBRATE_DLL0_LOOP1_REENTRY		//data compress fail
	nop
	nop	
#else
1:	
	li	t3,0x5a5aa5a5
	nop
	bgt	t1,0xa0005100,data_pass_L0
	nop
	nop
	lw	t5,0(t1)
	nop	
	nop
	bne	t3,t5,CALIBRATE_DLL0_LOOP1_REENTRY
	nop
	nop
	addi	t1,t1,4
	nop
	li	t3,0x0f0ff0f0
	nop
	lw	t5,0(t1)
	nop	
	nop
	bne	t3,t5,CALIBRATE_DLL0_LOOP1_REENTRY
	nop
	nop
	addi	t1,t1,4
	nop
	li	t3,0x0000ffff
	nop
	lw	t5,0(t1)
	nop	
	nop
	bne	t3,t5,CALIBRATE_DLL0_LOOP1_REENTRY	
	nop
	nop
	addi	t1,t1,4
	nop
	li	t3,0x12345678
	nop
	lw	t5,0(t1)
	nop	
	nop
	bne	t3,t5,CALIBRATE_DLL0_LOOP1_REENTRY	
	nop
	nop
	addi	t1,t1,4
	nop
	nop
	b	1b
	nop
	nop

#endif

data_pass_L0:	
	addiu	 L0, t4, 0			//unsigned add t4 + 0 ,write to L0, modified by kevinchung
	addiu	DONE_BIT,DONE_BIT,1
	//b CALIBRATE_DLL0_LOOP1_REENTRY
    	nop                                             
CALIBRATE_DLL0_R0_SET:
////////////Clean data from 0xa0005000 to 0xa0005100
	li		t7,0xa0005100
	li		t1,0xa0005000
	li		t3,0xa5a55a5a				//destination area default pattern
1:	
	nop
	nop
	//sw		t1,0(t1)
	sw		t3,0(t1)
	nop
	nop
	addi		t1,t1,4
	nop
	nop
	bne		t1,t7,1b
	nop	
////////////GEN data from 0xa0001000 to 0xa0002000
	li		t7,0xa0001110
	li		t1,0xa0001000
	li		t3,0x5a5aa5a5				//change pattern	
#if	1
1:	
	nop
	nop
	//sw		t1,0(t1)
	sw		t3,0(t1)
	nop
	nop
	addi		t1,t1,4
	nop
	nop
	bne		t1,t7,1b
	nop
#else
1:	
	nop
	li		t3,0x5a5aa5a5	
	nop
	sw		t3,0(t1)
	nop
	addi		t1,t1,4
	nop
	li		t3,0x0f0ff0f0
	nop
	sw		t3,0(t1)
	nop
	addi		t1,t1,4
	nop
	li		t3,0x0000ffff
	nop
	sw		t3,0(t1)
	nop
	addi		t1,t1,4
	nop
	li		t3,0x12345678
	nop
	sw		t3,0(t1)
	nop
	addi		t1,t1,4
	nop
	//bne		t1,t7,1b
	bgt		t7,t1,1b
	nop
	nop
#endif
////////////GDMA accress 
 /////////////////////////////////////////////
 	li	t1,0xb800a000
 	li	t7,0
 	sw	t7,0(t1)			//Software reset GDMA, 0xb800a000[n31]=0	-> 1
 	nop
 	nop
 	li	t7,0x80000000
 	sw	t7,0(t1)			//Enable GDMA, 0xb800a000[n31]=1, and func set to memory copy [n27..n24]=0
 	nop
 	nop
 	li	t7,0x0
 	li	t1,0xb800a004
 	sw	t7,0(t1)			//Interrupt Mask Register clean
 	nop
 	nop
 	li	t7,0x90000000
 	li	t1,0xb800a008
 	sw	t7,0(t1)			//Interrupt status Register, [n31] and [n28] write 1 to clean
 	nop
 	nop 	
 ///////////memory copy (setup source)/////////////////////////////////
 	li	t7,0xa0001000	//source data address
 	li	t1,0xb800a020
 	sw	t7,0(t1)			//setting source data address
 	nop
 	nop
  	li	t7,0x80000100			//[n31]=1, last data block, and source data length[n12..n0]
 	li	t1,0xb800a024
 	sw	t7,0(t1)					//setting source data length
 	nop
 	nop
/////////////memory copy (setup destination)/////////////////////////////////
 	li	t7,0xa0005000	//destination data address
 	li	t1,0xb800a060
 	sw	t7,0(t1)			//setting destination data address
 	nop
 	nop
  	li	t7,0x80000100			//[n31]=1, last data block, and destination data length[n12..n0]
 	li	t1,0xb800a064
 	sw	t7,0(t1)					//setting destination data length
 	nop
 	nop  
//////////////startting  GDMA process 	/////////////////////////////////
  	li	t7,0xC0000000			//GDMA_ENABLE | GDMA_POLL | GDMA_MEMCPY	//startting  GDMA process
 	li	t1,0xb800a000
 	sw	t7,0(t1)					//startting  GDMA 
 	nop
 	nop  	
//////////////polling GDMA copy done flag	/////////////////////////////////
  	li	t7,0x0					// clean data buffer
  	li	t5,0x80000000
 	li	t1,0xb800a008			// GDMA	Interrupt Status Register
1:
	nop
	nop
 	lw			t7,0(t1)					// load 
 	nop
 	nop
 	and 		t7,t7,t5	
 	nop
 	beqz		t7,1b				//GDMA_not_complete
 	nop
 	nop  	
//////////software compare data 
	li	t5,0
	li	t7,0xa0001000
	li	t1,0xa0005000
#if	1
1:	
	addi	t1,t1,4
	nop
	addi	t7,t7,4
	nop
	beq	t1,0xa0005100,data_pass_R0
	nop
	lw	t5,0(t1)
	nop
	nop
	nop
	nop
	
	//beq	t7,t5,1b
	beq	t3,t5,1b
	nop
	nop
	b	CALIBRATE_DLL0_LOOP1_OUT		//data compress fail
	nop
	nop	
#else
1:	
	li	t3,0x5a5aa5a5
	nop
	bgt	t1,0xa0005100,data_pass_R0
	nop
	nop
	lw	t5,0(t1)
	nop	
	nop
	bne	t3,t5,CALIBRATE_DLL0_LOOP1_OUT
	nop
	nop
	addi	t1,t1,4
	nop
	li	t3,0x0f0ff0f0
	nop
	lw	t5,0(t1)
	nop	
	nop
	bne	t3,t5,CALIBRATE_DLL0_LOOP1_OUT
	nop
	nop
	addi	t1,t1,4
	nop
	li	t3,0x0000ffff
	nop
	lw	t5,0(t1)
	nop	
	nop
	bne	t3,t5,CALIBRATE_DLL0_LOOP1_OUT	
	nop
	nop
	addi	t1,t1,4
	nop
	li	t3,0x12345678
	nop
	lw	t5,0(t1)
	nop	
	nop
	bne	t3,t5,CALIBRATE_DLL0_LOOP1_OUT	
	nop
	nop
	addi	t1,t1,4
	nop
	nop
	b	1b
	nop
	nop

#endif

data_pass_R0:	                                         

	move R0, t4
	nop
CALIBRATE_DLL0_LOOP1_REENTRY:
	addiu t4, t4, 1
	nop
	nop
	b CALIBRATE_DLL0_LOOP1
  nop         
  nop
CALIBRATE_DLL0_LOOP1_OUT:
	nop
	nop
	nop
	nop
	nop
	nop
	bnez		CORNER2_flag,CORNER2_PASS
	nop
	nop
	subu		t3,R0,L0
	nop
	nop
	bnez		t3,CORNER1_PASS	
	nop
	nop
//////////////////////////////////////	
	nop
	nop
	move	CORNER1_L,L0
	move	CORNER1_R,R0		
	nop
	nop
	li		CORNER2_flag,0x1
	li		s2,22
	li		t4,5					//initial start of scan range, if it under 3 that will cover coner1.
	//li		t8,0x7001			//
	li		t8,0x7000			//
	nop
	nop
	b		CALIBRATE_DLL0_START
	nop
	nop

CORNER1_PASS:	
CORNER2_PASS:	
	beqz		R0,CALIBRATE_DLL0_BEGIN
	nop
	nop
	li		t7, 0												//clean t7    
	li		t1, 0
	nop
	nop
	nop
	add		t7, t7, L0											//
	nop                                                                                    
	add		t7, t7, R0
 	nop 
 	srl 		t7, t7, 1			//div 2
	nop
	nop 	
	nop
	//bnez		CORNER2_flag,ONGOING
	nop
	
	move	t1,t7
	nop
	add		t7,t7,L0			
	nop
 	srl 		t7, t7, 1			//div 2, value equ 1/4 window 	
	nop
	beqz		CORNER2_flag,ONGOING
	nop
	nop
	
	add		t7,t7,t1
	nop
	srl		t7,t7,1			//value equ 3/8 window 
 	nop
/*	
	add		t7,t7,L0			
	nop
 	srl 		t7, t7, 1			//div 2, value equ 1/8 window 	
 	nop 	
*/

ONGOING:
	nop
 	sll 		t7, t7, 1	
 	nop
	or		t7, t7, t8
	nop
	nop
#if 0
TEST_FIXED:	
	bnez		CORNER2_flag,C2
	nop
	nop
	li		t7,0x0f
	nop
	b		SET_FINAL_VALUE
	nop
	nop
C2:	li		t7,0x701c
	nop
	nop
#endif

SET_FINAL_VALUE:
	nop
	nop
	sw	t7, 0(SW_DLL0_ADR)
	nop
	nop
	nop
	nop
	nop
 	nop
CORNER1_SET_END: 	
CORNER2_SET_END: 	
	nop
	move	t0,CORNER1_L
	li		t1,0xa0800000
	sw		t0,0(t1)				//save L0 value at 0xa0800000
	nop
	nop
	move	t0,CORNER1_R
	li		t1,0xa0800004
	sw		t0,0(t1)				//save R0 value at 0xa0800004
	nop
	nop	
	move	t0,L0
	li		t1,0xa0800010
	sw		t0,0(t1)				//save L0 value at 0xa0800010
	nop
	nop
	move	t0,R0
	li		t1,0xa0800014
	sw		t0,0(t1)				//save R0 value at 0xa0800014	
	nop
	nop	
	nop
	nop
#undef SW_DRAM_ADR
#undef SW_DLL0_ADR
#undef CORNER1_L
#undef CORNER1_R
#undef L0
#undef R0
#undef DONE_BIT
#if	1
////////////disable GDMA module 
//enable GDMA module 0xb800330c[n10]=1
	li		t1,0xb800330c
	li		t2,0xFFFFFBFF
	nop
	lw		t7,0(t1)			//load 0xb800330c
	nop
	and		t7,t7,t2			//set 0xb800330c[n10]=0, disable
 	nop
 	sw		t7,0(t1)			//enable GDMA module
#endif	
#endif
//=========================================
###############################################
	li	t1,100
1:	
	subu		t1,1
	nop
	nop
	bnez		t1,1b
	nop	
	nop	
#################################################
#======Start  DDR calibration ===========
#if 1
#define SW_DRAM_ADR	t0
#define SW_DRAM_ADDR_END t1
#define SW_DDCR_ADR t2
#define SW_DDCR_VAL t3
#define L_WEIGHT s0
#define R_WEIGHT s1
#define L0 s3
#define R0 s4
#define L1 s5
#define R1 s6
#define DONE_BIT	s7
	//.set reorder
	li	t1, 0xb8001050
	li	t2, 0x40000000
	sw	t2, 0(t1)
	nop
	//li	SW_DRAM_ADR, 0xA0001000
 	li	SW_DDCR_ADR, 0xB8001050
 	//li	SW_DDCR_VAL, 0x00000000           // sw cal:0x80000000, hw cal: 0x00000000
 	li	L_WEIGHT, 4
 	li	R_WEIGHT, 4
 	li	s2, 31
 	move	L0, zero
 	move	L1, zero
 	move	DONE_BIT, zero
 	li	R0, 33
 	li	R1, 33
#if	0
 	li	t1, 0x5A5AA5A5
 	sw	t1, 0(SW_DRAM_ADR)
#else	
////////////GEN data from 0xa0001000 to 0xa0001100
	li		t7,0xa0001100
	li		t1,0xa0001000
	li		t3,0x5a5aa5a5				//change pattern
1:	
	nop
	nop
	sw		t3,0(t1)
	nop
	nop
	addi		t1,t1,4
	nop
	nop
	bne		t1,t7,1b
	nop
#endif 	
CALIBRATE_DQS0:
 	move	t4, zero
 	li	t5, 0x00FF00FF
 	li	t6, 0x005A00A5
 	li	t8, 0x40000000                            //  sw cal: 0xC0000000, hw: 0x40000000
CALIBRATE_DQS0_LOOP1:
	li		SW_DRAM_ADR,0xa0001000
	li		SW_DRAM_ADDR_END,0xa0001100
	nop
	bltu	s2, t4, CALIBRATE_DQS0_LOOP1_OUT
  nop
  nop
	sll		t7, t4, 25
	or		t7, t7, t8
	nop
	//.set noreorder
	sw		t7, 0(SW_DDCR_ADR)
	nop
	nop
1:
	nop	
	lw		t7, 0(SW_DRAM_ADR)
	nop
	nop
	//.set reorder
	and		t7, t7, t5
	nop
CALIBRATE_DQS0_L0_SET:
	nop
	//bne	L0, zero, CALIBRATE_DQS0_R0_SET
	bnez		DONE_BIT, CALIBRATE_DQS0_R0_SET
  nop
  nop
	bne 		t7, t6, CALIBRATE_DQS0_LOOP1_REENTRY
  nop
  nop
  addi		SW_DRAM_ADR,SW_DRAM_ADR,4
  nop
  bne			SW_DRAM_ADR,SW_DRAM_ADDR_END,1b
  nop
  nop
	//addiu 	L0, t4, 1
	addiu 	L0, t4, 0
	nop
	nop
	addiu	DONE_BIT,DONE_BIT,1	
	nop
	nop
	//b CALIBRATE_DQS0_LOOP1_REENTRY
  nop
  nop  
CALIBRATE_DQS0_R0_SET:
	li		SW_DRAM_ADR,0xa0001000									//initial 	SW_DRAM_ADR
	li		SW_DRAM_ADDR_END,0xa0001100							//initial		SW_DRAM_ADDR_END
 	nop
	nop
1:
	nop	
	lw		t7, 0(SW_DRAM_ADR)	
	nop
	nop
	and		t7, t7, t5
	nop
	nop
	bne 	t7, t6, CALIBRATE_DQS0_LOOP1_OUT
  nop 
  nop
  addi		SW_DRAM_ADR,SW_DRAM_ADR,4
  nop
  bne			SW_DRAM_ADR,SW_DRAM_ADDR_END,1b
  nop
  nop
	move 	R0, t4
CALIBRATE_DQS0_LOOP1_REENTRY:	
	nop
	addiu 	t4, t4, 1
	nop
	nop
	b 		CALIBRATE_DQS0_LOOP1
  nop
  nop
  nop
CALIBRATE_DQS0_LOOP1_OUT:
	li	t5, 0
	li	t6, 0
	nop
	nop

	add	t6, t6, L0
	nop
	nop
	add	t6, t6, R0
 	nop
 	nop

	srl 	t6, t6, 1
	nop
	sll		t6, t6, 25
	nop
	li		t7, 0x40000000 									//initial analog calibration.		
	nop
	or		t6, t6, t7
	nop
	nop
	move	SW_DDCR_VAL, t6
	nop
	sw	SW_DDCR_VAL, 0(SW_DDCR_ADR)
	nop
	nop

CALIBRATE_DQS1:
 	move	t4, zero
 	move	DONE_BIT, zero 	
 	li	t5, 0xFF00FF00
 	li	t6, 0x5A00A500
 	li	t8, 0xFE000000
CALIBRATE_DQS1_LOOP1:
	li		SW_DRAM_ADR,0xa0001000									//initial 	SW_DRAM_ADR
	li		SW_DRAM_ADDR_END,0xa0001100							//initial		SW_DRAM_ADDR_END
	nop
	nop
	bltu	s2, t4, CALIBRATE_DQS1_LOOP1_OUT
  nop
  nop
	and		t8, t8, SW_DDCR_VAL
	nop
	sll	t7, t4, 20
	nop
	or	t7, t7, t8
	nop
	nop
	//.set noreorder
	sw	t7, 0(SW_DDCR_ADR)
	nop
	nop
1:
	nop	
	lw	t7, 0(SW_DRAM_ADR)
	nop
	nop
	nop
	//.set reorder
	and	t7, t7, t5
	nop
	nop
CALIBRATE_DQS1_L1_SET:
	bnez		DONE_BIT, CALIBRATE_DQS1_R1_SET
  nop
  nop
	bne t7, t6, CALIBRATE_DQS1_LOOP1_REENTRY
  nop
  nop
  addi	SW_DRAM_ADR,SW_DRAM_ADR,4
  nop
  bne		SW_DRAM_ADR,SW_DRAM_ADDR_END,1b
  nop
  nop
	addiu L1, t4, 0
	nop
	nop
	addiu	DONE_BIT,DONE_BIT,1		
	//b CALIBRATE_DQS1_LOOP1_REENTRY
  nop
  nop
CALIBRATE_DQS1_R1_SET:
	li		SW_DRAM_ADR,0xa0001000									//initial 	SW_DRAM_ADR
	li		SW_DRAM_ADDR_END,0xa0001100							//initial		SW_DRAM_ADDR_END
	nop
1:
	nop	
	lw		t7, 0(SW_DRAM_ADR)	
	nop
	nop
	and		t7, t7, t5
	nop
	nop	
	bne 		t7, t6, CALIBRATE_DQS1_LOOP1_OUT
  nop
  nop
  addi	SW_DRAM_ADR,SW_DRAM_ADR,4
  nop
  bne		SW_DRAM_ADR,SW_DRAM_ADDR_END,1b
  nop
  nop
	move R1, t4
	nop
	nop
CALIBRATE_DQS1_LOOP1_REENTRY:	
	addiu t4, t4, 1
	nop
	b CALIBRATE_DQS1_LOOP1
  nop
  nop
CALIBRATE_DQS1_LOOP1_OUT:
	li	t6, 0
	nop
	nop
	add	t6, t6, L1
	nop
	nop
	add	t6, t6, R1
 	nop
 	nop

CALIBRATE_DQS1_R1_WEIGHT_OUT:
	srl t6, t6, 1
	nop
	nop
	sll	t6, t6, 20
	nop
	nop
	li	t7, 0xFE000000
	nop
	and	t7, t7, SW_DDCR_VAL
	nop
	nop
	or	t6, t6, t7
	nop
	nop
	sw	t6, 0(SW_DDCR_ADR)
	nop
	nop
CALIBRATE_OUT:
	move	t9, L0
	li	t1, 0xa0800100
	sw	t9, 0(t1)
	nop
	nop
	move	t9, R0
	li	t1, 0xa0800104
	sw	t9, 0(t1)
	nop
	nop	
	move	t9, L1
	li	t1, 0xa0800108
	sw	t9, 0(t1)
	nop
	nop	
	move	t9, R1
	li	t1, 0xa080010c
	sw	t9, 0(t1)
	nop
	nop	
    .set noreorder
#undef SW_DRAM_ADR
#undef SW_DRAM_VAL
#undef SW_DDCR_ADR
#undef SW_DDCR_VAL
#undef L_WEIGHT
#undef R_WEIGHT    
#endif
	nop
	nop
	nop
        b               MEM_CONF_DONE
        nop
        nop
        nop
#===== end DDR calibration ================
is_SDRAM:
###############################################
# SDRAM/DDR Memory timing register, MUST set before MCR(config. reg)

	li		t0,0x6d0b0ac0		//SDR
	li		t1,0xb8001008
	sw		t0,0(t1)
	nop
	nop
	nop
###############################################
	//li		t0,0x52480000		//16M, for SDRAM & DDR
	li		t0,0x54480000		//32M, for SDRAM & DDR
	//li		t0,MCR0_VAL
	li		t1,0xB8001004
	sw		t0,0(t1)
	nop
        b               MEM_CONF_DONE
        nop
        nop
        nop
CCC:
BBB: /* to be define later */

# SDRAM/DDR Memory timing register, MUST set before MCR(config. reg)
	li		t0,0x6d0b0ac0
	li		t1,0xb8001008
	sw		t0,0(t1)
	nop

	li		t0,0x00000406
	li		t1,0xb8003304
	sw		t0,0(t1)
	nop
	#with packet processor
	#li		t0,0x800008f0
	#without packet processor
	li		t0,0x000000f0
	li		t1,0xb800330c
	sw		t0,0(t1)
	nop
	li		t0,0x90060000
	li		t1,0xb8003300
	sw		t0,0(t1)
	nop

	li		t1, SCCR
	lw		t0, 0(t1)
	nop	
#if defined(SYS_CLOCK_175MHZ) || defined(SDRAM_CLOCK_166MHZ) || defined(CPU_CLOCK_340MHZ) || defined(SDRAM_CLOCK_133MHZ)
	srl		t0, t0, 12
	sll		t0, t0, 12
#if defined(SYS_CLOCK_175MHZ)
	addi		t0, t0, 0x40
#endif
#if defined(SDRAM_CLOCK_166MHZ) 
	addi		t0, t0, 0x03
#endif
#if defined(SDRAM_CLOCK_133MHZ) 
    nop	
#endif
#if defined(CPU_CLOCK_340MHZ)
	addi		t0, t0, 0x600
#endif
	sw		t0, 0(t1)
	nop
#endif


MEM_CONF_DONE:
	/* setup bootloader stack */
	
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
		li	t2, 0x520b0000
		b	setting_mcr
		nop		
SDRAM_16MB:
		li	t2, 0x524b0000
		b	setting_mcr
		nop		
SDRAM_32MB:
		li	t2, 0x544b0000
		b	setting_mcr
		nop		
SDRAM_64MB:
		li	t2, 0x548b0000
		b	setting_mcr
		nop		
setting_mcr:
		li	t3, 0xb8001004
		nop
		sw	t2, 0(t3)
		nop

	
//#endif //SDRAM_AUTO_DETECT

#if 0
	#--- config lexra bus and sdram clock freque
	li		t1, SCCR
	lw		t0, 0(t1)
	nop
#if defined(SYS_CLOCK_175MHZ) || defined(SDRAM_CLOCK_166MHZ) || defined(CPU_CLOCK_340MHZ)
    srl     t0, t0, 12
    sll     t0, t0, 12
#if defined(SYS_CLOCK_175MHZ)
    addi        t0, t0, 0x40
#endif
#if defined(SDRAM_CLOCK_166MHZ)
    addi        t0, t0, 0x03
#endif
#if defined(CPU_CLOCK_340MHZ)
    addi        t0, t0, 0x600
#endif
    sw      t0, 0(t1)
    nop
#endif
#endif

	
//#endif //Diable Init
/*
 * t0 = addr_sp
 * t2 = destination address
 */
	li	t0, CONFIG_DRAM_BASE + CFG_SDRAM_SIZE - SDRAM_SIZE + SP_OFFSET
	move	sp, t0			/* Set new stack pointer		*/


	li	t0, CONFIG_FLASH_BASE	/* source address of bootloaderstart */
	 
	li	a2, CONFIG_DRAM_BASE + CFG_SDRAM_SIZE - SDRAM_SIZE + BOOTSTART_OFFSET
	move	t1, a2			/* relocation target address for bootloaderstart  */


	la	t3, in_ram
	nop

	sub	t6,t1,t0	/* t6 save the offset from RAM to ROM,t6=t1-t0,t6 = 0x81f40000 - 0xbd000000 */

	sub	t3,t3,t6

	lw	t2, -12(t3)	/* t2 <-- uboot_end_data	*/
	nop
	sub	t2,t2,t6
	nop

	/*
	 * t0 = source address
	 * t1 = target address
	 * t2 = source end address
	 * t6 = offset from RAM to ROM
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

	sub	a0 , t1,t6	/* the addr of bootloader_second(in flash),pass argument to uncompress_bootloader_second */

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
