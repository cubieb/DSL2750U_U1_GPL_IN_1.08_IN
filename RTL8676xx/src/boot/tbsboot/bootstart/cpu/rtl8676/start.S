
#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include <asm/mipsregs.h>

#define MEM_LIMIT				0x80800000
#define	FLASH_START				0xBFC00000
#define	MEM_START				0x80000000
#define	C0_SR					t4
#define	C0_CAUSE				t5
#define	C0_EPC					t6
#define SR_BEV                  0x00400000      /* Bootstrap Exception Vector */
#define SR_CU0					0x10000000	/* Coprocessor 0 usable */
#define SR_CU1                  0x20000000      /* Coprocessor 1 usable */

#define RVECENT(f,n) 	b f; nop

#if defined(CONFIG_RAM_AUTO)
#define LX_210_DDR166		1
#elif defined(CONFIG_RAM_166)
#define LX_210_DDR166		1
#elif defined(CONFIG_RAM_133)
#define LX_210_DDR133		1
#else
//#error "RAM Clock undefined"
#endif


	################################################################################
	#  Noop_Delay  <iteration> <register,default T1>, For All production
	################################################################################
	.macro Noop_Delay iter reg=t1
		li	\reg,\iter
		1:
		subu		\reg,1
		nop	
		bnez		\reg,1b
		nop			
	.endm

	################################################################################
	#  Detect_Clock_Source - Detect clock source and store result to v0, For RTL8676
	#    v0=0 : 35.328Mhz
	#    v0=1 : 40Mhz
	################################################################################
	.macro Detect_Clock_Source
		li		t1,0xb8000308
		lw		t0,0(t1)
		nop
		srl		t0,t0,23
		and		v0,t0,1		
	.endm
	
	################################################################################
	#  Mem_Set_Word <StartAddr> <Size> <Pattern>  - Fill memory with pattern, For All production
	################################################################################
	.macro Mem_Set_Word addr size patt
		#define	CLR_TEMP1	t0
		#define	CLR_TEMP2	t2
		#define	CLR_TEMP3	t3
		li		CLR_TEMP1,\addr				//start address
		addiu	CLR_TEMP2,CLR_TEMP1,\size		//2k bytes, end address
		li		CLR_TEMP3,\patt
		1:	
		sw		CLR_TEMP3,0(CLR_TEMP1)
		addi	CLR_TEMP1,CLR_TEMP1,4
		bne		CLR_TEMP1,CLR_TEMP2,1b
		nop		
		#undef	CLR_TEMP1	
		#undef	CLR_TEMP2	
		#undef	CLR_TEMP3	
	.endm
	
	
	################################################################################
	#  Detect_RAM_Size
	################################################################################
	#	 here we auto detect the sdram size. according Designer's info's on memory controller behavior
	#	 use address range = 0x0000_0000 ~ 0x03FF_FFFF
	#	input address 0xA3F0_1234 => physical address = 0x03F0_1234
	#	predefine 16bits (bytes) DRAM => DRAM address = 0x03F0_1234 / 2 = 0x1F8_091A
	#	column address range a0~a9 (0x400) => 0x1F8_091A mod 0x400 = 11A (COL address)
	#	                                                0x1F8_091A / 0x400 = 0x7E02 ( for ROW / BANK)
	#	row address range a0~a12 (0x2000)  => 0x7E02 mod 0x2000 = 1E02 (ROW address)
	#                                                                    0x7E02 / 0x2000 = 3 (BANK address)
	# we have conclusion on MCR with 64MB setting:
	#	2MB *(0xa3f01234) == *(0xa1f01234)
	#	8MB *(0xa3f01234) == *(0xA3F01034)
	#	16MB *(0xa3f01234) == *(0xA3701634)
	#	32MB *(0xa3f01234) == *(0xA3F01634)
	#	64MB *(0xa3f01234) unique
	
	.macro Detect_RAM_Size
		li	t5,0xb8001000		//detect DDR or SDR on board.
		lw	t5,0(t5)
		nop
		and	t6,t5,0x80000000
		nop
		beqz	t6,is_SDRAM_MCR
		nop
		//li 	t6, 0x00030000
		li	t0,0xb8001004
		lw	t1,0(t0)
		nop
		and 	t6,t1,0x30000
		b	is_DDR_MCR
		nop
is_SDRAM_MCR:
		li	t6, 0x00000000		
is_DDR_MCR:		
		li  	t0, 0xb8001004
		or	t1, t6, 0x54880000 
		
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
		li  k0, 0x200000
		b	setting_mcr
		nop
SDRAM_8MB:
		li	t2, 0x52080000
		li  k0, 0x800000
		b	setting_mcr
		nop		
SDRAM_16MB:
		or	t2, t6, 0x52480000
		li  k0, 0x1000000
		b	setting_mcr
		nop		
SDRAM_32MB:
		or	t2, t6, 0x54480000
		li  k0, 0x2000000
		b	setting_mcr
		nop		
SDRAM_64MB:
		or	t2, t6, 0x54880000
		li  k0, 0x4000000
		b	setting_mcr
		nop		
setting_mcr:
		li	t3, 0xb8001004
		nop
		sw	t2, 0(t3)
		nop

	.endm // Detect_RAM_Size

	################################################################################
	#  SRAM_Map	 <Bus Address> <Size> <Segment> <Base>
	#     Size      (0-128/1-256/2-512/3-1k/4-2k/5-4k/6-8k/7-16k/8-32k) 
	#     Segment   (0x00/0x10/0x20/0x30) which set of register to use
	################################################################################	
	.macro SRAM_Map addr size segment base
		/////////  SRAM unmap & map  /////////
		#define	SRAM_START_ADDR	t0
		#define	SRAM_SIZE			t1
		#define	SRAM_BASE			t2
		#define	REG_TEMP			t3
		////// Segment 0 ,32k bytes	//////
		li	SRAM_START_ADDR,(\addr | 0x00000001)		//last bit it define enable
		li	SRAM_SIZE,\size					//32k bytes
		li	REG_TEMP,0xb8001300				//unmap addr reg.
		addi	REG_TEMP,REG_TEMP,\segment
		sw	SRAM_START_ADDR,0(REG_TEMP)
		nop
		li	REG_TEMP,0xb8001304				//unmap size reg.
		addi	REG_TEMP,REG_TEMP,\segment
		sw	SRAM_SIZE,0(REG_TEMP)
		nop	
		li	REG_TEMP,0xb8004000				//map addr reg.
		addi	REG_TEMP,REG_TEMP,\segment
		sw	SRAM_START_ADDR,0(REG_TEMP)
		nop
		li	REG_TEMP,0xb8004004				//map size reg.
		addi	REG_TEMP,REG_TEMP,\segment
		sw	SRAM_SIZE,0(REG_TEMP)	
		nop
		li	SRAM_BASE,\base					//
		li	REG_TEMP,0xb8004008				//Segment 0 Base reg.
		addi	REG_TEMP,REG_TEMP,\segment
		sw	SRAM_BASE,0(REG_TEMP)	
		nop
		#undef	REG_TEMP
		#undef	SRAM_BASE
		#undef	SRAM_SIZE
		#undef	SRAM_START_ADDR
	.endm

	################################################################################
	#  UN_SRAM_Map	<Segment> <Base=0>
	#     
	#     Segment   (0/1/2/3) -> (0x00/0x10/0x20/0x30)	
	################################################################################	
	.macro UN_SRAM_Map segment
		/////////  SRAM unmap & map  /////////
		#define	SRAM_START_ADDR	t0
		#define	SRAM_SIZE			t1
		#define	SRAM_BASE			t2
		#define	REG_TEMP			t3
		////// Segment 0 ,32k bytes	//////
		li	SRAM_START_ADDR,(0x00000000)		//last bit it define enable
		li	SRAM_SIZE,(0x00000000)					//set size to zero
		li	REG_TEMP,0xb8001300				      //unmap addr reg.
		addi	REG_TEMP,REG_TEMP,\segment
		sw	SRAM_START_ADDR,0(REG_TEMP)
		nop
		li	REG_TEMP,0xb8001304				//unmap size reg.
		addi	REG_TEMP,REG_TEMP,\segment
		sw	SRAM_SIZE,0(REG_TEMP)
		nop	
		li	REG_TEMP,0xb8004000				//map addr reg.
		addi	REG_TEMP,REG_TEMP,\segment
		sw	SRAM_START_ADDR,0(REG_TEMP)
		nop
		li	REG_TEMP,0xb8004004				//map size reg.
		addi	REG_TEMP,REG_TEMP,\segment
		sw	SRAM_SIZE,0(REG_TEMP)	
		nop
		li	SRAM_BASE,(0x00000000)					//
		li	REG_TEMP,0xb8004008				//Segment 0 Base reg.
		addi	REG_TEMP,REG_TEMP,\segment
		sw	SRAM_BASE,0(REG_TEMP)
		nop
		#undef	REG_TEMP
		#undef	SRAM_BASE
		#undef	SRAM_SIZE
		#undef	SRAM_START_ADDR
	.endm
	
	################################################################################
	#  Mem_Init_RTL8676
	################################################################################
	.macro Mem_Init_RTL8676

		// SWR parameter change
		li		t1,0xb8000218
		li		t0,0x845555e7
		sw		t0,0(t1)
		nop
		li		t1,0xb800021c
		li		t0,0x0003a11e
		sw		t0,0(t1)
		nop		
		Noop_Delay 1500
		//
		li		t1,0xb8000200			//load System Clock Frequency Control Register	
		li		t0,0x77001200			//clear OCP and DRAM CLK fields
		li		t2,0xb8000308			//PCM is bonded if bit24=1 or bit25=1 in 0xb8000308
		lw 		t3,0(t2)
		nop
		srl 		t3,t3,24
		andi		t3,t3,0x3
		bne		t3,0x0,PCM_OCP_CLK
		nop
NORMAL_OCP_CLK:
		ori		t0,t0,0x15			//OCP=560MHz (0x1A)  460MHz(0x15)
		b		DRAM_CLK
		nop
PCM_OCP_CLK:
		ori		t0,t0,0x1D			//OCP=620MHz (0x1D)
DRAM_CLK:
#ifdef LX_210_DDR166
		li		t2,0x080000			//Set DRAM=166MHz
		or		t0,t0,t2
#else
		li		t2,0x060000			//Set DRAM=133MHz
		or		t0,t0,t2
#endif
		sw		t0,0(t1)
		nop
		Noop_Delay 500
		nop

		##############################################
		# reg b800_1200: bit31-29,0:RAM clk/2, 1: RAM clk/4, 2: RAM clk/6, 3: RAM clk/8
		# Set default spi clock as RAM clk/6, e.g.200Mhz/6 = 33.333Mhz
		# bit26-22: deselect time
		# CS# deselect time min 100n for MX25L3205D, so set 19 to guarantee all device access normally.
		and		t2,t0,0x0F0000
		blt		t2,0x70000,set_div4		//DRAM freq < 150MHz, div 4, deselect value set to 0x10
		nop
		li		t0,0x5cc00000				//DRAM freq >= 150MHz, div 6, deselect value set to 0x13
		b		set_div
		nop
set_div4:
		li		t0,0x3c000000			//
set_div:		
		li		t1,0xb8001200
		sw		t0,0(t1)
		nop	
		Noop_Delay 100
		###############################################
		
		nop
		
		li		t1,0xb8000300
		li		t0,0x4
		sw		t0,0(t1)
		nop
		
		###############################################
		li		t0,0xb8000308		//Pin Status Register
		lw		t1,0(t0)
		nop
		and		t1,t1,~0x20000000	//bit29:Switch setting (0:master, 1:slave)
		sw		t1,0(t0)
		nop	
		
		//0xb800030c, Enable IP / IP selection Register,
		
	.endm
	
	
	#define NAND_CHECK_READY()      \
123:;							\
	la		s0, NAND_CTRL_BASE+0x4; \
	lw    	t0, 0(s0);       \
	and   	t0, t0, 0x80000000;      \
	bne   	t0, 0x80000000, 123b;    \
	nop
	
	
	################################################################################
	#  NAND Controller Registers	
	################################################################################
	#define NandRegCtrl			(NAND_CTRL_BASE+0x04)
	#define NandRegRW			(NAND_CTRL_BASE+0x10)
	#define NandRegFlashAddr	(NAND_CTRL_BASE+0x18)
	#define NandRegRAMAddr		(NAND_CTRL_BASE+0x1C)
	#define NandRegTagAddr		(NAND_CTRL_BASE+0x54)
		
	################################################################################
	#  NAND_Ready  
	#     Loop until NAND controller is ready
	################################################################################
	.macro NAND_Reay
	11:
		la		t7, NandRegCtrl
		lw		t7, 0(t7)
		nop
		srl		t7, 31
		beq		zero, t7, 11b
		nop
	.endm
	
	################################################################################
	#  NAND_Copy_2048 <page_from> <page_to> <Ram> <Tag> 
	#     Copy n x 2k page from NAND Flash address to RAM/Tag
	################################################################################			
	.macro NAND_Copy_2048 PageFrom PageTo RamAddr TagAddr
		la		t5, 0xc00fffff
		la		t4, NandRegCtrl
		sw		t5, 0(t4)
		
		li		t1, \PageFrom * 0x1000
		li		t2, \RamAddr
		li		t3, \TagAddr
		li		t4, \PageTo * 0x1000
	
21:		
		li		t0, 4
20:		
		la		t5, NandRegFlashAddr	# initialize variables
		sw		t1, 0(t5)
		la		t6, NandRegRAMAddr
		sw		t2, 0(t6)
		la		t5, NandRegTagAddr
		sw		t3, 0(t5)
		la		t6, NandRegRW
		li		t5, 0x5b				# start write
		sw		t5, 0(t6)
		NAND_Reay
		
		addi	t0, t0, -1
		addi	t1, t1, 0x210
		addi	t2, t2, 0x200
		addi	t3, t3, 0x10
		bne		zero, t0, 20b
		nop
		
		addi	t1, t1, (0x1000 - 0x840)				
		bne		t1, t4, 21b
		nop		
	.endm
	
	################################################################################
	#  NAND_Copy_512 <Page> <Ram> <Tag> <Size>
	#     Size      (0-128/1-256/2-512/3-1k/4-2k/5-4k/6-8k/7-16k/8-32k) 
	#     Segment   (0x00/0x10/0x20/0x30) which set of register to use
	################################################################################
	.macro NAND_Copy_512 PageFrom PageTo RamAddr TagAddr
		la		t5, 0xc00fffff
		la		t4, NandRegCtrl
		sw		t5, 0(t4)
		
		
		li		t2, \RamAddr
		li		t3, \TagAddr
		li		t1, \PageFrom * 0x200
		li		t0, \PageTo * 0x200
		
30:
		la		t4, NandRegFlashAddr	# initialize variables
		sw		t1, 0(t4)
		la		t5, NandRegRAMAddr
		sw		t2, 0(t5)
		la		t4, NandRegTagAddr
		sw		t3, 0(t4)
		la		t5, NandRegRW
		li		t4, 0x5b				# start write
		sw		t4, 0(t5)
		NAND_Reay
		
		addi	t1, t1, 0x200
		addi	t2, t2, 0x200
		addi	t3, t3, 0x10
		
		bne		t0, t1, 30b
		nop		
	.endm
	
////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////

	
	/* For hard or soft reset, jump forward to handler. otherwise, jump to hard reset entry point, unless exception handler is called. */
	.set noreorder
	.globl _start
	.text
_start:
	RVECENT(reset,0)	/* U-boot entry point */
	RVECENT(reset,1)	/* software reboot */
	RVECENT(reset,2)
    RVECENT(reset,3)
        RVECENT(reset,4)
        RVECENT(reset,5)
        RVECENT(reset,6)
        RVECENT(reset,7)
        RVECENT(reset,8)
        RVECENT(reset,9)
        RVECENT(reset,10)
        RVECENT(reset,11)
        RVECENT(reset,12)
        RVECENT(reset,13)
        RVECENT(reset,14)
        RVECENT(reset,15)
        RVECENT(reset,16)
        RVECENT(reset,17)
        RVECENT(reset,18)
        RVECENT(reset,19)
        RVECENT(reset,20)
        RVECENT(reset,21)
        RVECENT(reset,22)
        RVECENT(reset,23)
        RVECENT(reset,24)
        RVECENT(reset,25)
        RVECENT(reset,26)
        RVECENT(reset,27)
        RVECENT(reset,28)
        RVECENT(reset,29)
        RVECENT(reset,30)
        RVECENT(reset,31)
        RVECENT(reset,33)
        RVECENT(reset,34)
        RVECENT(reset,35)
        RVECENT(reset,36)
        RVECENT(reset,37)
        RVECENT(reset,38)
        RVECENT(reset,39)
        RVECENT(reset,40)
        RVECENT(reset,41)
        RVECENT(reset,42)
        RVECENT(reset,43)
        RVECENT(reset,44)
        RVECENT(reset,45)
        RVECENT(reset,46)
        RVECENT(reset,47)
        RVECENT(reset,48)
	.align 4
reset:
	bal		beginning
	nop
	.word   _GLOBAL_OFFSET_TABLE_
beginning:
	lw		t1,	0(ra)
	nop
	move	gp,	t1
	mfc0    k0,t6               // save for nvram
	move    k1,ra                   // save for nvram
//	mfc0    gp,t4
	mfc0    t7,C0_CAUSE	
	li      v0,SR_BEV
	mtc0    v0,C0_SR                // state unknown on reset
	mtc0    zero,C0_CAUSE           // clear software interrupts
	nop                             // paranoia

	/* second entry point -- sw reboot inherits a0 start type            */
	li	t0, SR_CU1 | SR_BEV
	nop
	mtc0	t0, C0_SR		/* disable all interrupts fpa on,    */
	nop
	nop							/* prom exception handlers           */
	mtc0	zero, C0_CAUSE		/* clear all interrupts              */
	nop
	nop
	/* Light On led_power_red */
	la t0, 0xB8003500
	lw t1, 0x1C(t0)
	and t1, t1, 0xFFFEFFFF
	sw t1, 0x1C(t0)
	lw t1, 0x24(t0)
	or t1, t1, 0x10000
	sw t1, 0x24(t0)
	lw t1, 0x28(t0)
	and t1, t1, 0xFFFEFFFF
	sw t1, 0x28(t0)
	/* Light On led_power_red */
#if 0	// by xiachaoren
//--- bypass copying if in RAM


	bal		1f
	nop
1:
	la		t0, 0x10000000
	nop
	and		t0, ra, t0
	beq		t0, zero, in_ram				//decide if ram based on BIT28
	nop
#endif
///////////////////////////////////////////////////////////
#ifdef SUPPORT_8676_PLATFORM  //----------------------- start SUPPORT_8676_PLATFORM -------------------------------------//
#ifdef CONFIG_RTL8676_NAND_FLASH
//--- the supported flash type 
	li		t1, Boot_Select
	lw		t0, 0(t1)
	nop
	and		t0, t0, 0x40
	nop
	beqz	t0, SPIandNOR
	nop

NAND:
// map 0xbfc01000 (4K) to SRAM segment 1

		/////////  SRAM unmap & map  /////////////// Segment 0 ,32k bytes	//////
		li	t0,(0x1fc01000 | 0x00000001)		//last bit it define enable
		li	t1,	0x5					//32k bytes
		li	t3,0xb8001300				//unmap addr reg.
		addi	t3,t3, 0x10
		sw	t0,0(t3)
		nop
		li	t3,0xb8001304				//unmap size reg.
		addi	t3,t3, 0x10
		sw	t1,0(t3)
		nop	
		li	t3,0xb8004000				//map addr reg.
		addi	t3,t3, 0x10
		sw	t0,0(t3)
		nop
		li	t3,0xb8004004				//map size reg.
		addi	t3,t3, 0x10
		sw	t1,0(t3)	
		nop
		li	t2,0x1000					//
		li	t3,0xb8004008				//Segment 0 Base reg.
		addi	t3,t3, 0x10
		sw	t2,0(t3)	
		nop

//	--- copy next 4K bytes to 0x1fc01000 ---
	la		t1, 0x2000
	la		t2, 0x4000
	la		t3, 0x1fc01000
	la		t4, NAND_CTRL_BASE+0x4
	la		t5, 0xc000ffff
	sw		t5, 0(t4)
selnand4:
	la		t6, 4
selnand5:
//	--- NAND DMA Flash data address ---
	la		t4, NAND_CTRL_BASE+0x18
	sw		t1, 0(t4)
	nop
//	--- NAND DMA SDRAM oob address ---
	la		t4, NAND_CTRL_BASE+0x54
	la		s1, 0x200000	
	sw		s1, 0(t4)
	nop
//	--- NAND DMA SDRAM data address ---
	la		t4, NAND_CTRL_BASE+0x1c
	sw		t3, 0(t4)
	nop
//	--- NAND DMA Read operation ---
	la		t4, NAND_CTRL_BASE+0x10
	la		t5, 0x0000005b
	sw		t5, 0(t4)
	nop

nand_rdy1:
	la		s0, NAND_CTRL_BASE+0x4
	lw    	t0, 0(s0)
	and   	t0, t0, 0x80000000
	bne   	t0, 0x80000000, nand_rdy1
	nop
	addi	t1, t1, 528
	addi	t3, t3, 512
	addi	t6, t6, -1
	bne		t6, 0, selnand5
	nop
	addi	t1, t1, 1984
	bne		t1, t2, selnand4
	nop
#endif //CONFIG_RTL8676_NAND_FLASH

SPIandNOR:
	Mem_Init_RTL8676
	SRAM_Map 0x00600000, 0x7, 0x30, 0x2000	// map 0x80600000 (16K) to SRAM segment 2
	Mem_Set_Word 0xa0600000,0x4000,0x0  // fill 0
//#############################################################
	li		t0,0xb8000308		//detect DDR or SDR on board.
	lw		t9,0(t0)
	nop
	srl 	t9,8
	and		t8,t9,0x2
	beqz	t8,is_SDRAM
	nop
////////////// //Disable clock SSD  //////////////////////////
	li		t0,0x80000000		//Disable clock SSD for DDR, DDR SDRAM
	li		t1,0xb8000220
	sw		t0,0(t1)
	nop	
	Noop_Delay	100
//	SDR & DDR I/O PAD current select for test only. 	
	//li		t0,0x00000407		//all DDR bus, driving upgrade	
	//li		t0,0x00000404		//DDR bus,data, clock driving upgrade	
	li		t0,0x0000400			//data bus, driving upgrade
	//li		t0,0x0				//driving normally
	li		t1,0xb8000314
	sw		t0,0(t1)
	nop	
// SDRAM/DDR Memory timing register, MUST set before MCR(config. reg)
#ifdef LX_210_DDR166
	li		t0,0x6CEC8480
	li		t2,0x54A00000		//RX DQS calibration default value, PLL analog calibration value for DDR only.*	.	
#else										//DRAM_133MHz:
	li		t0,0x6CEBE280		//DTR parameter
	li		t2,0x5EF00000		//RX DQS calibration default value, PLL analog calibration value for DDR only.*	.
#endif
	nop
	li		t1,0xb8001008		//set_DTR:
	sw		t0,0(t1)
	nop	
	li		t1,0xb8001050
	sw		t2,0(t1)
	nop	
	
#if 1		//DRAM PLL reset
		li 		t0,0x10000131
		li		t1,0xb800100c
	sw		t0,0(t1)
	nop		
		li		t0,0x524a0000
		li		t1,0xb8001004		
		sw		t0,0(t1)		
		Noop_Delay	1200	
#endif	
	li		t0,0x52480000							//16M, for SDRAM & DDR
	li		t1,0xB8001004							//initial DDR signal re-send
	sw		t0,0(t1)
	nop			
 	and		t8,t9,0x1
 	beqz	t8,is_DDR1
 	nop
is_DDR2:
//##############################################
// MRS configuration for DDR setting.
#if 1
// MRS configuration for DDR setting.
#define	MCR_PARA			t0
#define	MRS_VALUE			t1
#define	EMRS_VALUE			t2
#define	EDTCR_ADDR			t3
#define	EDTCR_ORG			t4
#define	MCR_ADDR			t5
#define	REG_TEMP			t6
#define	REG_TEMP2			t7
#define	MCR_ORG			t8
#define	REG_TEMP1			t9
	li		MCR_ADDR,0xb8001004
	li		EDTCR_ADDR,0xb800100c
	//li		MRS_VALUE,0x31					//DLL reset=0, TM=0, CAS# Latency=3, BurstType=0, BurstLength=1(2), n15..n14=00(Mode Register)
	//li		EMRS_VALUE,0x4406				//I/O Strength reduce (half), n15..n14=01(EMR), ODT 75ohm,DQS# disable
	lw		MCR_ORG,0(MCR_ADDR)			//Load MCR
	nop	
	or		MCR_PARA,MCR_ORG,0x20000		//MR MODE EN.	
#if defined(CONFIG_DDR2_8banks)
	li		REG_TEMP,0x50004406						//set EMRS value, supported 8 banks for DDR2, and modified WTR from 111b -> 010b
#else
	li		REG_TEMP,0x10004406						//set EMRS value, supported 4 banks for DDR2, and modified WTR from 111b -> 010b
#endif	
	nop
#if 1
	sw		REG_TEMP,0(EDTCR_ADDR)				//set EMRS for DDR1 initial 
1:																		
	lw		REG_TEMP2,0(MCR_ADDR)
	nop
	and		REG_TEMP2,REG_TEMP2,0x1				//check DCR_WR_BUSY
	nop
	bnez	REG_TEMP2,1b
	nop
#endif
	sw		MCR_PARA,0(MCR_ADDR)					//set MR mode enable 
1:
	lw		REG_TEMP2,0(MCR_ADDR)
	nop
	and		REG_TEMP2,REG_TEMP2,0x1				//check DCR_WR_BUSY
	nop
	bnez	REG_TEMP2,1b
	nop
#if 1	
	sw		MCR_ORG,0(MCR_ADDR)						//MR MODE Disable.
	nop
1:
	lw		REG_TEMP2,0(MCR_ADDR)
	nop
	and		REG_TEMP2,REG_TEMP2,0x1				//check DCR_WR_BUSY
	nop
	bnez	REG_TEMP2,1b
	nop
#endif
#undef	MCR_PARA
#undef	MRS_VALUE
#undef	EMRS_VALUE
#undef	EDTCR_ADDR
#undef	MCR_ADDR
#undef	REG_TEMP
#undef	MCR_ORG
#undef	REG_TEMP2
#undef	EDTCR_ORG
	b			DDR_Pre_Setup_finish
	nop
#endif	
//##############################################	
is_DDR1:
#if 1
	li		t5,0xb8001004
	li		t1,0x52480000
	sw		t1,0(t5)
1:
	lw		t2,0(t5)
	nop
	and		t2,t2,0x1				//check DCR_WR_BUSY
	nop
	bnez		t2,1b
	nop	
	li		t0,0xb800100c
	li		t1,0x10004002								//modified WTR from 111b -> 010b
	sw		t1,0(t0)
	nop
	nop
	li		t1,0x524a0000
	sw		t1,0(t5)
	nop
	li	t1,		100
	Noop_Delay 100
1:
	lw		t2,0(t5)
	nop
	and		t2,t2,0x1				//check DCR_WR_BUSY
	nop
	bnez		t2,1b
	nop		
	li		t1,0x52480000
	sw		t1,0(t5)
	nop
#endif

DDR_Pre_Setup_finish:
	Noop_Delay 1200
	////////////enable GDMA module 
	//enable GDMA module 0xb800030c[n10]=1
	li	t1,0xb800030c	
	lw	t7,0(t1)			//load 0xb800030c
	nop
	ori	t7,t7,0x400		//set 0xb800030c[n10]=1
 	nop
 	sw	t7,0(t1)			//enable GDMA module
 	nop
//###############################################
//////// Global paramter setup //////
#if defined(CONFIG_RAM_AUTO)
#define CLK_133MHZ_FLAG	s2
#ifdef	DRAM166
 	move	CLK_133MHZ_FLAG,zero		//default run 166MHz
#else	 	
	li		CLK_133MHZ_FLAG,1		//run in 133MHz
#endif	
#endif	
///////copy GENERATE PATTERN code to SRAM 0xA0606000 ////////
#define		CP_BLK_START_ADDR		t0
#define		CP_BLK_END_ADDR			t1
#define		CP_BLK_LENGTH			t2
#define		CP_BLK_TEMP1			t3
#define		CP_BLK_TEMP2			t4
	la		CP_BLK_START_ADDR, DQS_CALIBRATION_BEGIN
	la		CP_BLK_END_ADDR, DQS_CALIBRATION_FINISH
	sub		CP_BLK_LENGTH,CP_BLK_END_ADDR,CP_BLK_START_ADDR
	li		CP_BLK_TEMP1,0xa0600050											//save copy block length
	sw		CP_BLK_LENGTH,0(CP_BLK_TEMP1)

	li		CP_BLK_TEMP1,0xbfc00000											//boot start address.
	or		CP_BLK_START_ADDR,CP_BLK_START_ADDR,CP_BLK_TEMP1				//transfer DRAM(0x80000000) to FLASH(0xbfc00000) address
	or		CP_BLK_END_ADDR,CP_BLK_END_ADDR,CP_BLK_TEMP1					//transfer DRAM(0x80000000) to FLASH(0xbfc00000) address
	
	li		CP_BLK_TEMP1, 0xA0600100										//start address of SRAM reserve area  
	
1:	
	lw		CP_BLK_TEMP2, 0(CP_BLK_START_ADDR)								//load code from flash
	nop
	sw		CP_BLK_TEMP2, 0(CP_BLK_TEMP1)									//restore code to SRAM
	addi		CP_BLK_START_ADDR, CP_BLK_START_ADDR, 4
	addi		CP_BLK_TEMP1, CP_BLK_TEMP1, 4
	bne		CP_BLK_START_ADDR, CP_BLK_END_ADDR, 1b
	nop
	li		CP_BLK_TEMP1, 0xA0600100										//
	jalr		CP_BLK_TEMP1													//jump to SRAM
	nop
	//nop	

#undef		CP_BLK_START_ADDR
#undef		CP_BLK_END_ADDR
#undef		CP_BLK_LENGTH
#undef		CP_BLK_TEMP1
#undef		CP_BLK_TEMP2
///////////////////////////////////////////////////////// 	
DQS_CALIBRATION_BEGIN:
	nop
	move	t6,ra																//save ra address
	li		t7,0xa0600054
	sw		t6,0(t7)
////GENERATE PATTERN /////////////////////////////
GENERATE_PATTERN:
#define	PATTERN_TEMP1		t1
#define	PATTERN_TEMP2		t2
#define	PATTERN_LENGTH		t3
#define	PATTERN				t4		
#define	PATTERN1			t5
PATTERN_START:
	li		PATTERN,0x5a5aa5a5							//default pattern
	li		PATTERN1,0x0FFFF								//default pattern
////////////GEN data from 0xa0602000 to 0xa0602FFF, 4k bytes
	li		PATTERN_TEMP1,0xa0602000				//start address
	li		PATTERN_LENGTH,0x1000						//4k bytes, end address
1:	
	sw		PATTERN1,0(PATTERN_TEMP1)				//pattern1(0x0FFFF)
	addi	PATTERN_TEMP1,PATTERN_TEMP1,4
	not		PATTERN_TEMP2,PATTERN1					//pattern1 reverse (0xFFFF0000)
	sw		PATTERN_TEMP2,0(PATTERN_TEMP1)
	addi	PATTERN_TEMP1,PATTERN_TEMP1,4
	sw		PATTERN,0(PATTERN_TEMP1)				//pattern(0x5a5aa5a5)
	addi	PATTERN_TEMP1,PATTERN_TEMP1,4
	not		PATTERN_TEMP2,PATTERN						//pattern reverse(0xa5a55a5a)
	sw		PATTERN_TEMP2,0(PATTERN_TEMP1)
	addi	PATTERN_TEMP1,PATTERN_TEMP1,4
	subu	PATTERN_LENGTH,0x10
	bnez	PATTERN_LENGTH,1b
	nop
#undef	PATTERN_TEMP1
#undef	PATTERN_TEMP2
#undef	PATTERN_LENGTH	
#undef	PATTERN					
#undef	PATTERN1
GENERATE_PATTERN_FINISH:
///////// generate data finish /////////

//#======Start  DDR calibration (GDMA)===========
#if 1
/////////  DQS calibration start  /////////
#define 	SW_SRAM_ADDR		t0
#define 	DQS_TEMP5		 	t1
#define 	RUN_CYCLE	 		t2
#define 	SW_DDCR_VAL 		t3
#define 	DQS_TEMP1	 		t4
#define 	DQS_TEMP2	 		t5
#define 	DQS_TEMP3	 		t6
#define 	DQS_TEMP4	 		t7
#define 	POINT_CURSOR 		t8
#define 	UP_LIMIT		 		t9
#define 	DQS_CALI_TEMP		s0
#define 	DATA_MASK			s1
//s2 reg. is for global parameter.
#define 	L0 					s3
#define 	R0 					s4
#define 	L1 					s5
#define 	R1 					s6
#define 	L_DONE_BIT			s7
	//.set reorder
//// insert DDR calibration version, record in 0xA0600018 //start///	
	li		DQS_TEMP1,0xA0600018
	li		DQS_TEMP2,0x1D02					//version: 1.02
	sw		DQS_TEMP2,0(DQS_TEMP1)
//// insert DDR calibration version, record in 0xA0600018 //end///	
DQS_CALIBRATION_START:	
///////Initial REG. default value ///////////
	li		SW_SRAM_ADDR,0xa0603000
	li		DQS_TEMP1, 0xb8001050
 	li		SW_DDCR_VAL, 0x40000000           			//  [n31]=1:sw cal, 0:hw cal
	sw		SW_DDCR_VAL, 0(DQS_TEMP1)
	li		DQS_CALI_TEMP,0
 	li		UP_LIMIT, 31
 	li		RUN_CYCLE,0
 	move	L0, zero
 	move	L1, zero
 	move	L_DONE_BIT, zero
 	li		R0, 0
 	li		R1, 0

///////Initial REG. default value finish///////////	
////////////GEN data to DDR, the address from 0xa0602000 to 0xa0602FFF, 4k bytes  ////////
	nop
	li		DQS_TEMP1,0xA0612000				//start address	(DDR start address)
	li		DQS_TEMP2,0xA0613000				//end address	(DDR end address)
	li		DQS_TEMP4,0xA0602000				//start address	(SRAM start address)
1:	
	lw		DQS_TEMP3,0(DQS_TEMP4)				//read data form SRAM
	sw		DQS_TEMP3,0(DQS_TEMP1)				//write data to DDR
	addiu	DQS_TEMP1,DQS_TEMP1,4
	addiu	DQS_TEMP4,DQS_TEMP4,4	
	bne		DQS_TEMP1,DQS_TEMP2,1b	
	nop
//////////// GEN data to DDR finish ///////////////
CALIBRATE_DQS0:
#if defined(CONFIG_RAM_AUTO)
	bgt		RUN_CYCLE,2,RUN133MHz					//if RUN_CYCLE > 2, jump RUN133MHz
#else
	bgt		RUN_CYCLE,2,FIXED_DQS					//if RUN_CYCLE > 2, jump FIXED_DQS
#endif	
	nop
 	li		POINT_CURSOR, 0
CALIBRATE_DQS0_LOOP1:
////////////Clearn data from 0xa0603000 to 0xa0603FFF, 4k bytes
	move	DQS_TEMP1,SW_SRAM_ADDR			//start address
	li		DQS_TEMP2,0x1000					//Clean length 4k bytes
	li		DQS_TEMP3,0
1:	
	sw		DQS_TEMP3,0(DQS_TEMP1)
	addiu	DQS_TEMP1,DQS_TEMP1,4
	subu		DQS_TEMP2,4
	nop
	bnez		DQS_TEMP2,1b
	nop
///////// Clearn data  finish /////////
DQS_PARAMETER_CH:
	li		SW_DDCR_VAL,0x40000000
	bltu	UP_LIMIT, POINT_CURSOR, CALIBRATE_DQS0_LOOP1_OUT
	nop
	sll		DQS_TEMP1, POINT_CURSOR, 25
	or		SW_DDCR_VAL, SW_DDCR_VAL, DQS_TEMP1				// SW_DDCR_VAL default equ 0x40000000, write DQS0 value.
	li		DQS_TEMP1,0xb8001050
	sw		SW_DDCR_VAL, 0(DQS_TEMP1)						//setting DQS0 value to 0xb8001050.
////////GDMA move data from DDR to SRAM  ///////////////////////////
//// GDMA use TX_TEMP1,TX_TEMP2,TX_TEMP3 REG.	/////////////////
 	li		DQS_TEMP1,0xb800a000
 	li		DQS_TEMP2,0x0
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//Software reset GDMA, 0xb800a000[n31]=0	-> 1
 	li		DQS_TEMP2,0x80000000
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//Enable GDMA, 0xb800a000[n31]=1, and func set to memory copy [n27..n24]=0
 	li		DQS_TEMP2,0x0
 	li		DQS_TEMP1,0xb800a004
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//Interrupt Mask Register clean
 	li		DQS_TEMP2,0x90000000
 	li		DQS_TEMP1,0xb800a008
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//Interrupt status Register, [n31] and [n28] write 1 to clean
 ///////////GDMA memory copy (setup source)/////////////////////////////////
 	li		DQS_TEMP2,0x00612000			//source data address
 	li		DQS_TEMP1,0xb800a020
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//setting source data address
  li		DQS_TEMP2,0x80001000			//[n31]=1, last data block, and source data length[n12..n0]
 	li		DQS_TEMP1,0xb800a024
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//setting source data length
/////////////GDMA memory copy (setup destination)/////////////////////////////////
	and		DQS_TEMP2,SW_SRAM_ADDR,0x0FFFFFFF	//destination data address
 	li		DQS_TEMP1,0xb800a060
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//setting destination data address
 	li		DQS_TEMP2,0x80001000			//[n31]=1, last data block, and destination data length[n12..n0]
 	li		DQS_TEMP1,0xb800a064
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//setting destination data length
//////////////startting  GDMA process 	/////////////////////////////////
  li		DQS_TEMP2,0xC0000000			//GDMA_ENABLE | GDMA_POLL | GDMA_MEMCPY	//startting  GDMA process
 	li		DQS_TEMP1,0xb800a000
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//startting  GDMA 
//////////////polling GDMA copy done flag	/////////////////////////////////
  li		DQS_TEMP2,0x0					// clean data buffer
  li		DQS_TEMP3,0x80000000
 	li		DQS_TEMP1,0xb800a008			// GDMA	Interrupt Status Register
1:
 	lw		DQS_TEMP2,0(DQS_TEMP1)			// load 
 	nop
 	and 		DQS_TEMP2,DQS_TEMP2,DQS_TEMP3	
 	beqz		DQS_TEMP2,1b					//GDMA_not_complete
 	nop	
////////GDMA move data from DDR to SRAM  finish ///////////////////////////	
	bnez		L_DONE_BIT, CALIBRATE_DQS0_R0_SET
	nop
//////////software compare data /////////////////
	move	DQS_TEMP1,SW_SRAM_ADDR							//start compare address
	li		DQS_TEMP3,0xA0612000
	li		DQS_TEMP5,0x1000									//length, end compare address
 	li		DATA_MASK,0x00FF00FF	
 	subu	DQS_TEMP5,4	
	//bnez		L_DONE_BIT, CALIBRATE_DQS0_R0_SET
	//nop 	

1:	
	lw		DQS_TEMP2,0(DQS_TEMP1)								//read data from SRAM ( just use GDMA copy data from DDR to SRAM)
	nop
	lw		DQS_TEMP4,0(DQS_TEMP3)								//read data from DRAM ( just use GDMA copy data from DDR to SRAM)
	nop
	and		DQS_TEMP2,DQS_TEMP2,DATA_MASK					//data and mask.
	and		DQS_TEMP4,DQS_TEMP4,DATA_MASK
	addiu	DQS_TEMP1,DQS_TEMP1,4								//address plus 4
	addiu	DQS_TEMP3,DQS_TEMP3,4								//address plus 4
	bne		DQS_TEMP2,DQS_TEMP4,CALIBRATE_DQS0_LOOP1_REENTRY	//compare pattern
	nop
	subu	DQS_TEMP5,4
	bnez	DQS_TEMP5,1b
	nop
	move 	L0, POINT_CURSOR
	addiu	L_DONE_BIT,L_DONE_BIT,1	
CALIBRATE_DQS0_R0_SET:	
	move	DQS_TEMP1,SW_SRAM_ADDR							//start compare address						
	li		DQS_TEMP3,0xA0612000
	li		DQS_TEMP5,0x1000									//length, end compare address
 	li		DATA_MASK,0x00FF00FF	
	subu	DQS_TEMP5,4 	
1:	
	lw		DQS_TEMP2,0(DQS_TEMP1)								//read data from SRAM ( just use GDMA copy data from DDR to SRAM)
	nop
	lw		DQS_TEMP4,0(DQS_TEMP3)								//read data from DRAM ( just use GDMA copy data from DDR to SRAM)
	nop
	and		DQS_TEMP2,DQS_TEMP2,DATA_MASK						//data and mask.
	and		DQS_TEMP4,DQS_TEMP4,DATA_MASK
	addiu	DQS_TEMP1,DQS_TEMP1,4								//address plus 4
	addiu	DQS_TEMP3,DQS_TEMP3,4								//address plus 4	
	bne		DQS_TEMP2,DQS_TEMP4,Check_R0_value					//compare pattern
	nop
	subu		DQS_TEMP5,4
	bnez		DQS_TEMP5,1b
	nop	
	move 	R0, POINT_CURSOR
	//b		CALIBRATE_DQS0_LOOP1_REENTRY	
	nop		
CALIBRATE_DQS0_LOOP1_REENTRY:	
	addiu 	POINT_CURSOR, POINT_CURSOR, 1
	b 		CALIBRATE_DQS0_LOOP1
  nop
Check_R0_value:  	
  	beqz		R0,L0_plus1_CLR_L_DONE					//if R0=0, L0 plus 1
  	nop
  	beq		R0,L0,L0_plus1_CLR_L_DONE				//if R0=L0, L0 plus 1
  	nop
  	sub		DQS_TEMP1,R0,L0
	blt		DQS_TEMP1,2,L0_plus1_CLR_L_DONE  		//if (R0-L0) < 2 ,L0 plus 1
	nop
  	b		CALIBRATE_DQS0_LOOP1_R0_OUT
  	nop
L0_plus1_CLR_L_DONE:
	li		L_DONE_BIT,0
	li		R0,0
	b		CALIBRATE_DQS0_LOOP1_REENTRY
	nop
//##################
CALIBRATE_DQS0_LOOP1_R0_OUT:
	addiu	RUN_CYCLE,RUN_CYCLE,1
	beqz		R0,CALIBRATE_DQS0
	nop
	add		DQS_TEMP1,R0,L0
	beqz		DQS_TEMP1,CALIBRATE_DQS0				//if (R0+L0) = 0, re-try agaim.
	nop
	sub		DQS_TEMP1,R0,L0
	blt		DQS_TEMP1,2,CALIBRATE_DQS0				//if (R0-L0) < 2 , the vale is very close, so re-try again.
	nop
	beq		R0,L0,CALIBRATE_DQS0					//if R0=L0, re-try again.
	nop
CALIBRATE_DQS0_LOOP1_OUT:	
//###################
	beqz		R0,CALIBRATE_DQS0_LOOP1_R0_OUT
	nop
	move	DQS_TEMP1, L0										//move L0 to DQS_TEMP1
	add		DQS_TEMP1, DQS_TEMP1, R0				//DQS_TEMP1 equ sum( L0 + R0 )
	srl 		DQS_TEMP1, DQS_TEMP1, 1					//calibration value div 2		

//#######################	
	sll		DQS_TEMP1, DQS_TEMP1, 25
	li		SW_DDCR_VAL, 0x40000000 				//initial analog calibration.		
	or		DQS_CALI_TEMP, SW_DDCR_VAL, DQS_TEMP1	//finish DQS0 calibration, save in DQS_CALI_TEMP
	li		DQS_TEMP1,0xb8001050
	sw		DQS_CALI_TEMP, 0(DQS_TEMP1)
//////////Starting DQS1 calibration //////////////
	move	L_DONE_BIT, zero
	li		RUN_CYCLE,0
CALIBRATE_DQS1:
#if defined(CONFIG_RAM_AUTO)
	bgt		RUN_CYCLE,2,RUN133MHz					//if RUN_CYCLE > 2, jump RUN133MHz
#else	
	bgt		RUN_CYCLE,2,FIXED_DQS					//if RUN_CYCLE > 2, jump FIXED_DQS	
#endif	
	nop
 	move	POINT_CURSOR, zero	
CALIBRATE_DQS1_LOOP1: 	
////////////Clearn data from 0xa0603000 to 0xa0603FFF, 4k bytes
	move	DQS_TEMP1,SW_SRAM_ADDR				//start address
	li		DQS_TEMP2,0x1000						//Clean length 4k bytes
	li		DQS_TEMP3,0
1:	
	sw		DQS_TEMP3,0(DQS_TEMP1)
	addiu	DQS_TEMP1,DQS_TEMP1,4
	subu		DQS_TEMP2,4
	bnez		DQS_TEMP2,1b
	nop
///////// Clearn data  finish /////////
DQS1_PARAMETER_CH:
	move	SW_DDCR_VAL,DQS_CALI_TEMP							//load calibration method and DQS0 value.
	bltu	UP_LIMIT, POINT_CURSOR, CALIBRATE_DQS1_LOOP1_OUT	//out of parameter range
	nop
	sll		DQS_TEMP1, POINT_CURSOR, 20							//DQS1 set bit[n24..n20]
	or		SW_DDCR_VAL, SW_DDCR_VAL, DQS_TEMP1				// SW_DDCR_VAL default equ 0x40000000, write DQS0 value.
	li		DQS_TEMP1,0xb8001050
	sw		SW_DDCR_VAL, 0(DQS_TEMP1)						//setting DQS1 value to 0xb8001050.
////////GDMA move data from DDR to SRAM  ///////////////////////////
//// GDMA use TX_TEMP1,TX_TEMP2,TX_TEMP3 REG.	/////////////////
 	li		DQS_TEMP1,0xb800a000
 	li		DQS_TEMP2,0x0
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//Software reset GDMA, 0xb800a000[n31]=0	-> 1
 	li		DQS_TEMP2,0x80000000
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//Enable GDMA, 0xb800a000[n31]=1, and func set to memory copy [n27..n24]=0
 	li		DQS_TEMP2,0x0
 	li		DQS_TEMP1,0xb800a004
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//Interrupt Mask Register clean
 	li		DQS_TEMP2,0x90000000
 	li		DQS_TEMP1,0xb800a008
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//Interrupt status Register, [n31] and [n28] write 1 to clean
 ///////////GDMA memory copy (setup source)/////////////////////////////////
 	li		DQS_TEMP2,0x00612000			//source data address
 	li		DQS_TEMP1,0xb800a020
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//setting source data address
  	li		DQS_TEMP2,0x80001000			//[n31]=1, last data block, and source data length[n12..n0]
 	li		DQS_TEMP1,0xb800a024
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//setting source data length
/////////////GDMA memory copy (setup destination)/////////////////////////////////
	and		DQS_TEMP2,SW_SRAM_ADDR,0x0FFFFFFF	//destination data address
 	li		DQS_TEMP1,0xb800a060
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//setting destination data address
 	li		DQS_TEMP2,0x80001000			//[n31]=1, last data block, and destination data length[n12..n0]
 	li		DQS_TEMP1,0xb800a064
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//setting destination data length
//////////////startting  GDMA process 	/////////////////////////////////
  li		DQS_TEMP2,0xC0000000			//GDMA_ENABLE | GDMA_POLL | GDMA_MEMCPY	//startting  GDMA process
 	li		DQS_TEMP1,0xb800a000
 	sw		DQS_TEMP2,0(DQS_TEMP1)			//startting  GDMA 	
//////////////polling GDMA copy done flag	/////////////////////////////////
  li		DQS_TEMP2,0x0					// clean data buffer
  li		DQS_TEMP3,0x80000000
 	li		DQS_TEMP1,0xb800a008			// GDMA	Interrupt Status Register
1:
 	lw		DQS_TEMP2,0(DQS_TEMP1)					// load 
 	nop
 	and 		DQS_TEMP2,DQS_TEMP2,DQS_TEMP3	
 	beqz		DQS_TEMP2,1b					//GDMA_not_complete
 	nop	
////////GDMA move data from DDR to SRAM  finish ///////////////////////////	
	bnez		L_DONE_BIT, CALIBRATE_DQS1_R1_SET
	nop
//////////software compare data /////////////////
	move	DQS_TEMP1,SW_SRAM_ADDR							//start compare address
	li		DQS_TEMP3,0xA0612000
	li		DQS_TEMP5,0x1000									//length, end compare address
 	li		DATA_MASK,0xFF00FF00	
	subu		DQS_TEMP5,4 	
	//bnez		L_DONE_BIT, CALIBRATE_DQS1_R1_SET
	//nop
1:	
	lw		DQS_TEMP2,0(DQS_TEMP1)								//read data from SRAM ( just use GDMA copy data from DDR to SRAM)
	nop
	lw		DQS_TEMP4,0(DQS_TEMP3)								//read data from DRAM ( just use GDMA copy data from DDR to SRAM)
	nop
	and		DQS_TEMP2,DQS_TEMP2,DATA_MASK						//data and mask.
	and		DQS_TEMP4,DQS_TEMP4,DATA_MASK
	addiu	DQS_TEMP1,DQS_TEMP1,4								//address plus 4
	addiu	DQS_TEMP3,DQS_TEMP3,4								//address plus 4	
	bne		DQS_TEMP2,DQS_TEMP4,CALIBRATE_DQS1_LOOP1_REENTRY	//compare pattern
	nop
	subu		DQS_TEMP5,4
	bnez		DQS_TEMP5,1b
	nop
	move 	L1, POINT_CURSOR
	addiu	L_DONE_BIT,L_DONE_BIT,1		
CALIBRATE_DQS1_R1_SET:	
	move	DQS_TEMP1,SW_SRAM_ADDR							//start compare address
	li		DQS_TEMP3,0xA0612000
	li		DQS_TEMP5,0x1000									//length, end compare address
 	li		DATA_MASK,0xFF00FF00	
	subu		DQS_TEMP5,4 	
1:	
	lw		DQS_TEMP2,0(DQS_TEMP1)								//read data from SRAM
	nop
	lw		DQS_TEMP4,0(DQS_TEMP3)								//read data from DRAM
	nop
	and		DQS_TEMP2,DQS_TEMP2,DATA_MASK						//data and mask.
	and		DQS_TEMP4,DQS_TEMP4,DATA_MASK
	addiu	DQS_TEMP1,DQS_TEMP1,4								//address plus 4
	addiu	DQS_TEMP3,DQS_TEMP3,4								//address plus 4	
	bne		DQS_TEMP2,DQS_TEMP4,Check_R1_value					//compare pattern
	nop
	subu		DQS_TEMP5,4
	bnez		DQS_TEMP5,1b
	nop	
	move 	R1, POINT_CURSOR
	nop		
CALIBRATE_DQS1_LOOP1_REENTRY:	
	addiu 	POINT_CURSOR, POINT_CURSOR, 1
	b 		CALIBRATE_DQS1_LOOP1
  nop
Check_R1_value:  	
  	beqz		R1,L1_plus1_CLR_L_DONE					//if R1=0, L1 plus 1
  	nop
  	beq		R1,L1,L1_plus1_CLR_L_DONE				//if R1=L1, L1 plus 1
  	nop
  	sub		DQS_TEMP1,R1,L1
	blt		DQS_TEMP1,2,L1_plus1_CLR_L_DONE  		//if (R1-L1) < 2 ,L1 plus 1
	nop
  	b		CALIBRATE_DQS1_LOOP1_R1_OUT
	nop
L1_plus1_CLR_L_DONE:
	li		L_DONE_BIT,0
	li		R1,0
	b		CALIBRATE_DQS1_LOOP1_REENTRY
	nop
CALIBRATE_DQS1_LOOP1_R1_OUT:
	addiu	RUN_CYCLE,RUN_CYCLE,1
	beqz		R1,CALIBRATE_DQS1						//if R1=0, re-try again.
	nop
	add		DQS_TEMP1,R1,L1
	beqz	DQS_TEMP1,CALIBRATE_DQS1				//if (R1+L1) = 0, re-try agaim.
	nop
	sub		DQS_TEMP1,R1,L1
	blt		DQS_TEMP1,2,CALIBRATE_DQS1				//if (R1-L1) < 2 , the vale is very close, so re-try again.
	nop
	beq		R1,L1,CALIBRATE_DQS1					//if R1=L1, re-try again.
	nop
	beqz	R1,CALIBRATE_DQS1_LOOP1_REENTRY
	nop	
CALIBRATE_DQS1_LOOP1_OUT:	
//####################
	beqz		R1,CALIBRATE_DQS1_LOOP1_R1_OUT
	nop
	add		DQS_TEMP1, L1, R1							//DQS_TEMP1 equ sum( L1 + R1 )
	srl 	DQS_TEMP1, DQS_TEMP1, 1						//calibration value div 2		

//#######################	
	sll		DQS_TEMP1, DQS_TEMP1, 20					// left shift 20 bit for DQS1 parameter.
	or		DQS_CALI_TEMP, DQS_CALI_TEMP, DQS_TEMP1	//finish DQS1 calibration, save in DQS_CALI_TEMP
	li		DQS_TEMP1,0xb8001050
	sw		DQS_CALI_TEMP, 0(DQS_TEMP1)
////////////////////////////////////////////////////
RECORD_DATA:
	li		DQS_TEMP3,0x0
#if defined(CONFIG_RAM_AUTO)	
	beqz		CLK_133MHZ_FLAG,CLK_166MHZ_RECORD_DATA
	nop
	li		DQS_TEMP3,0x10
#endif	
CLK_166MHZ_RECORD_DATA:	
	move	DQS_TEMP1, L0
	li		DQS_TEMP2, 0xa0600020
	add		DQS_TEMP2,DQS_TEMP2,DQS_TEMP3
	nop
	sw		DQS_TEMP1, 0(DQS_TEMP2)
	move	DQS_TEMP1, R0
	li		DQS_TEMP2, 0xa0600024
	add		DQS_TEMP2,DQS_TEMP2,DQS_TEMP3	
	sw		DQS_TEMP1, 0(DQS_TEMP2)
	move	DQS_TEMP1, L1
	li		DQS_TEMP2, 0xa0600028	
	add		DQS_TEMP2,DQS_TEMP2,DQS_TEMP3
	sw		DQS_TEMP1, 0(DQS_TEMP2)
	move	DQS_TEMP1, R1
	li		DQS_TEMP2, 0xa060002c		
	add		DQS_TEMP2,DQS_TEMP2,DQS_TEMP3	
	sw		DQS_TEMP1, 0(DQS_TEMP2)
//////if 166MHz not stable, change to 133MHz/////////////////////////////////////////////
#if defined(CONFIG_RAM_AUTO)
	li		DQS_TEMP2,0x8								//if (R0-L0)<8, the DRAM clock will down gread to 133MHz
	sub		DQS_TEMP1,R0,L0
	bnez		CLK_133MHZ_FLAG,DQS_133MHz_ONGOING
	nop	
	bge		DQS_TEMP2,DQS_TEMP1,RUN133MHz  
	nop
	sub		DQS_TEMP1,R1,L1	
	bge		DQS_TEMP2,DQS_TEMP1,RUN133MHz  
	nop
DQS_133MHz_ONGOING:
	//nop
	b		CALIBRATE_OUT
	nop
RUN133MHz:
///////CLK_166MHZ_RECORD_DATA: start/////////
	move	DQS_TEMP1, L0
	li		DQS_TEMP2, 0xa0600020
	sw		DQS_TEMP1, 0(DQS_TEMP2)
	move	DQS_TEMP1, R0
	li		DQS_TEMP2, 0xa0600024
	sw		DQS_TEMP1, 0(DQS_TEMP2)
	move	DQS_TEMP1, L1
	li		DQS_TEMP2, 0xa0600028	
	sw		DQS_TEMP1, 0(DQS_TEMP2)
	move	DQS_TEMP1, R1
	li		DQS_TEMP2, 0xa060002c		
	sw		DQS_TEMP1, 0(DQS_TEMP2)
///////CLK_166MHZ_RECORD_DATA: end/////////	
	bnez		CLK_133MHZ_FLAG,FIXED_DQS
	nop
	li		DQS_TEMP1,0xb8000200
	lw		DQS_TEMP2,0(DQS_TEMP1)
	li		DQS_TEMP3,~0x0F0000
	and		DQS_TEMP2,DQS_TEMP2,DQS_TEMP3
	li		DQS_TEMP3,0x6								//0xb8000200[n19..n16] to setup DRAM clock rate. RTL8676(8:166MHz, 6:133MHz)
	sll		DQS_TEMP3,16
	or		DQS_TEMP2,DQS_TEMP2,DQS_TEMP3
	sw		DQS_TEMP2,0(DQS_TEMP1)
	li		DQS_TEMP3,300						//delay count.
1:
	subu		DQS_TEMP3,1
	bnez		DQS_TEMP3,1b
	nop		
	li		DQS_TEMP1,0xb8001008
	li		DQS_TEMP2,0x6CEB84C0
	sw		DQS_TEMP2,0(DQS_TEMP1)
	li		DQS_TEMP1,0xb8001050
	li		DQS_TEMP2,0x5ef00000
	sw		DQS_TEMP2,0(DQS_TEMP1)
############################################## 	 	
	//DRAM PLL reset
	li 		t0,0x10000131
	li		t1,0xb800100c
	sw		t0,0(t1)
	nop
	li		t0,0x524a0000
	li		t1,0xb8001004		
	sw		t0,0(t1)		
	Noop_Delay 1200
############################################## 		
	li		DQS_TEMP1,0xb8001004
	li		DQS_TEMP2,0x52480000
	sw		DQS_TEMP2,0(DQS_TEMP1)
	li		CLK_133MHZ_FLAG,0x1
	b		DQS_CALIBRATION_START
	nop	
#endif	
	
CALIBRATE_OUT:
	beqz		R0,FIXED_DQS
	nop
	beqz		R1,FIXED_DQS
	nop
	b		FINISH_CALI_DQS
	nop
	//nop
FIXED_DQS:
#ifdef DRAM166
 	li		DQS_TEMP2,0x54a00000					//RUN in 166MHz
#else
 	li		DQS_TEMP2,0x5ef00000					//RUN in 133MHz
#endif
 	li		DQS_TEMP1,0xb8001050
 	sw		DQS_TEMP2,0(DQS_TEMP1)					//fixed DQS
FINISH_CALI_DQS:
	
#if	0		//fixd DQS
  li		DQS_TEMP2,0x54A00000					//RUN in 166MHz
 	li		DQS_TEMP1,0xb8001050
 	sw		DQS_TEMP2,0(DQS_TEMP1)					//fixed DQS
#endif 	
	nop
    .set noreorder
#undef 	SW_SRAM_ADDR		
#undef 	CLK_133MHZ_FLAG
#undef 	RUN_CYCLE 		
#undef 	SW_DDCR_VAL 			
#undef 	DQS_TEMP1	 		
#undef 	DQS_TEMP2	 		
#undef 	DQS_TEMP3	 		
#undef 	DQS_TEMP4	 		
#undef 	POINT_CURSOR 		
#undef 	UP_LIMIT		 		
#undef 	DQS_CALI_TEMP		
#undef 	DQS_TEMP5			
#undef 	L0 					
#undef 	R0 					
#undef 	L1 					
#undef 	R1 					
#undef 	L_DONE_BIT
#else
#ifdef LX_210_DDR166
 	li		t0,0x54a00000			//RUN in 166MHz
#else
 	li		t0,0x5ef00000				//RUN in 133MHz
#endif
 	li		t1,0xb8001050
 	sw		t0,0(t1)					//fixed DQS	
#endif	
////////////////////////////////////////////////////////////////////
#undef CLK_133MHZ_FLAG
	li		t7,0xa0600054			//load ra into t6
	lw		t6,0(t7)
	nop
	li		t7,0xa0600050
	lw		t8,0(t7)
	nop
	add		ra,t6,t8					//ra addr plus copy length offset
	j		ra
	nop

DQS_CALIBRATION_FINISH:
	nop
//#################################################
	nop
  b               MEM_CONF_DONE
	nop
//#===== end DDR calibration ================

is_SDRAM:
//##############################################
//	SDR & DDR I/O PAD current select for test only. 
	//li		t0,0x3407				//for SoC module QAboard
	//li		t0,0x3000
	li		t0,0x0
	li		t1,0xb8000314
	sw		t0,0(t1)
	nop	
////////////// //Set Tx/Rx delay for SDRAM  //////////////////////////
	li		t0,0x06000E00		//SDRAM TX hold time(1.95ns) and RX delay parameter setup for RTL8676,
	li		t1,0xb8000220
	sw		t0,0(t1)
	nop	

//###############################################
//# SDRAM/DDR Memory timing register, MUST set before MCR(config. reg)
	li		t0,0x6d0b0ac0		//SDR
	li		t1,0xb8001008
	sw		t0,0(t1)
	nop
	nop
	nop
//###############################################
	li		t0,0x52080000		//8M, for SDRAM & DDR
	//li		t0,0x52480000		//16M, for SDRAM & DDR
	//li		t0,0x54480000		//32M, for SDRAM & DDR
	//li		t0,MCR0_VAL
	li		t1,0xB8001004
	sw		t0,0(t1)
	nop
#if	1	//SDRAM RX calibration......
#if	1	//copy code to DMEM
///////copy SDRAM RX delay calibraton code to SRAM 0xA0606000 ////////
#define		CP_BLK_START_ADDR	t0
#define		CP_BLK_END_ADDR		t1
#define		CP_BLK_LENGTH			t2
#define		CP_BLK_TEMP1			t3
#define		CP_BLK_TEMP2			t4
	la		CP_BLK_START_ADDR, SDRAM_RX_CALI_BEGIN
	la		CP_BLK_END_ADDR, SDRAM_RX_CALI_FINISH
	sub		CP_BLK_LENGTH,CP_BLK_END_ADDR,CP_BLK_START_ADDR
	li		CP_BLK_TEMP1,0xa0600050											//save copy block length
	sw		CP_BLK_LENGTH,0(CP_BLK_TEMP1)

	li		CP_BLK_TEMP1,0xbfc00000											//boot start address.
	or		CP_BLK_START_ADDR,CP_BLK_START_ADDR,CP_BLK_TEMP1				//transfer DRAM(0x80000000) to FLASH(0xbfc00000) address
	or		CP_BLK_END_ADDR,CP_BLK_END_ADDR,CP_BLK_TEMP1					//transfer DRAM(0x80000000) to FLASH(0xbfc00000) address
	
	li		CP_BLK_TEMP1, 0xA0600100															//start address of SRAM reserve area  
	
1:	
	lw		CP_BLK_TEMP2, 0(CP_BLK_START_ADDR)								//load code from flash
	nop
	sw		CP_BLK_TEMP2, 0(CP_BLK_TEMP1)									//restore code to SRAM
	addi		CP_BLK_START_ADDR, CP_BLK_START_ADDR, 4
	addi		CP_BLK_TEMP1, CP_BLK_TEMP1, 4
	bne		CP_BLK_START_ADDR, CP_BLK_END_ADDR, 1b
	nop
	li		CP_BLK_TEMP1, 0xA0600100										//
	jalr		CP_BLK_TEMP1													//jump to SRAM
	nop

#undef		CP_BLK_START_ADDR
#undef		CP_BLK_END_ADDR
#undef		CP_BLK_LENGTH
#undef		CP_BLK_TEMP1
#undef		CP_BLK_TEMP2
#endif		//copy code to DMEM
/////////////////////////////////////////////////////////
SDRAM_RX_CALI_BEGIN:
	nop
#if	1		//copy code to DMEM	
	move	t6,ra																//save ra address
	li		t7,0xa0600054
	sw		t6,0(t7)
#endif		//copy code to DMEM	
//#======SDRAM RX Calibration start	===========
#define	RX_TEMP1		t1
#define	RX_TEMP2		t2
#define	RX_TEMP6		t3
#define	RX_REG_ADDR	t4
#define	RX_REG_VAL		t5
#define	RX_TEMP5		t6
#define	RX_PARA			t7
#define	RX_TEMP3		t8
#define	RX_TEMP4		t9
#define	RX_L0			s0
#define	RX_MSB__flag		s5
//// insert SDR calibration version, record in 0xA0600018 //start///	
	li		RX_TEMP1,0xA0600018
	li		RX_TEMP2,0x1D02					//version: 1.02
	sw		RX_TEMP2,0(RX_TEMP1)
//// insert SDR calibration version, record in 0xA0600018 //end///

	li		RX_REG_ADDR,0xb8000220			//Delayline Control Register
	lw		RX_REG_VAL,0(RX_REG_ADDR)		//reload orginal value
	nop
	li		RX_PARA,0
	move	RX_L0,zero
RX_PRAR_CH:
/////////	set RX value	/////////
	and		RX_REG_VAL,RX_REG_VAL,0xFFFFE0FF	//[n12..n8]
	nop
	sll		RX_TEMP1,RX_PARA,8
	nop
	or		RX_REG_VAL,RX_REG_VAL,RX_TEMP1
	nop
	sw		RX_REG_VAL,0(RX_REG_ADDR)
	nop
////////////save start GDMA address  ////////////////////////////////////////// 	
	li		RX_TEMP1,0xA0600020				//save  start GDMA address to 0xA0600020
 	li		RX_TEMP2,0xA0400000				//start GDMA address		//change note 20120106. from 0xA0800000 -> 0xA0400000
 	sw		RX_TEMP2,0(RX_TEMP1)
 	nop
	li		RX_TEMP1,0xA0600028				//save  current GDMA address to 0xA0600028
	li		RX_TEMP3,0x1FFFFFFF
 	and		RX_TEMP2,RX_TEMP2,RX_TEMP3		//start GDMA address
 	sw		RX_TEMP2,0(RX_TEMP1)
 	nop 	 	
	li		RX_TEMP1,0xA0600024				//save  end GDMA address to 0xA0600024 
 	li		RX_TEMP2,0xA0420000				//end GDMA address		//change note 20120106. from 0xA0820000 -> 0xA0420000
 	sw		RX_TEMP2,0(RX_TEMP1)
 	nop 	
////////////Clearn data from 0xa0612000 to 0xa0612FFF,  this will write area. /////
////////////Clearn data from 0xa0800000 to 0xa08FFFFF,  this will write area. /////
	li		RX_TEMP1,0xA0600020				//load start GDMA address from 0xA0600020
	lw		RX_TEMP2,0(RX_TEMP1)
	nop
	li		RX_TEMP1,0xA0600024				//load end GDMA address from 0xA0600024
	lw		RX_TEMP3,0(RX_TEMP1)
	nop	
	li		RX_TEMP4,0
1:	
	sw		RX_TEMP4,0(RX_TEMP2)
	nop
	addi		RX_TEMP2,RX_TEMP2,4
	nop
	bgt		RX_TEMP3,RX_TEMP2,1b
	nop
///////// Clearn data  finish /////////
#if	1		//GENERATE PATTERN
////GENERATE PATTERN /////////////////////////////
	li		RX_TEMP4,0x5a5aa5a5		//default pattern
	li		RX_TEMP5,0x0FFFF			//default pattern
////////////GEN data from 0xa0610000 to 0xa061FFFF, 64k bytes
	li		RX_TEMP2,0xA0600020					//load start GDMA address from 0xA0600020
	lw		RX_TEMP1,0(RX_TEMP2)
	nop
	li		RX_TEMP3,0x8000						//32k bytes, end address
1:	
	sw		RX_TEMP5,0(RX_TEMP1)				//pattern1(0x0FFFF)
	addi		RX_TEMP1,RX_TEMP1,4
	not		RX_TEMP2,RX_TEMP5					//pattern1 reverse (0xFFFF0000)
	sw		RX_TEMP2,0(RX_TEMP1)
	addi		RX_TEMP1,RX_TEMP1,4
	sw		RX_TEMP4,0(RX_TEMP1)				//pattern(0x5a5aa5a5)
	addi		RX_TEMP1,RX_TEMP1,4
	not		RX_TEMP2,RX_TEMP4					//pattern reverse(0xa5a55a5a)
	sw		RX_TEMP2,0(RX_TEMP1)
	addi		RX_TEMP1,RX_TEMP1,4
	subu		RX_TEMP3,0x10	
	bnez		RX_TEMP3,1b
	nop	
///////// generate data finish /////////	
#endif		//generate data finish
	
RX_GDMA_START:
/////////	GDMA setup	,GDMA cp DRAM to DMEM /////////
 	li		RX_TEMP1,0xb800a000
 	li		RX_TEMP2,0x0
 	sw		RX_TEMP2,0(RX_TEMP1)			//Software reset GDMA, 0xb800a000[n31]=0	-> 1
	nop
 	li		RX_TEMP2,0x80000000
 	sw		RX_TEMP2,0(RX_TEMP1)			//Enable GDMA, 0xb800a000[n31]=1, and func set to memory copy [n27..n24]=0
 	nop
 	li		RX_TEMP2,0x0
 	li		RX_TEMP1,0xb800a004
 	sw		RX_TEMP2,0(RX_TEMP1)			//Interrupt Mask Register clean
 	nop
 	li		RX_TEMP2,0x90000000
 	li		RX_TEMP1,0xb800a008
 	sw		RX_TEMP2,0(RX_TEMP1)			//Interrupt status Register, [n31] and [n28] write 1 to clean
 	nop
 	li		RX_TEMP5,0x8000					//set cp block size
 ///////////GDMA memory copy (setup source, BLK0)/////////////////////////////////
 	li		RX_TEMP4,0xA0600028
 	lw		RX_TEMP3,0(RX_TEMP4)			//load current GDMA address.
 	nop
 	li		RX_TEMP1,0xb800a020
 	sw		RX_TEMP3,0(RX_TEMP1)			//setting source data address
 	nop
  li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and source data length[n12..n0]
 	li		RX_TEMP1,0xb800a024
 	sw		RX_TEMP2,0(RX_TEMP1)					//setting source data length
 	nop
/////////////GDMA memory copy (setup destination, BLK0)/////////////////////////////////
	add		RX_TEMP4,RX_TEMP3,RX_TEMP5
 	li		RX_TEMP1,0xb800a060
 	sw		RX_TEMP4,0(RX_TEMP1)			//setting destination data address
 	nop
 	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and destination data length[n12..n0]
 	li		RX_TEMP1,0xb800a064
 	sw		RX_TEMP2,0(RX_TEMP1)			//setting destination data length
 	nop
 ///////////GDMA memory copy (setup source, BLK1)/////////////////////////////////
 	addiu	RX_TEMP3,RX_TEMP3,0x1000		//source data address
 	li		RX_TEMP1,0xb800a028
 	sw		RX_TEMP3,0(RX_TEMP1)			//setting source data address
 	nop
  	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and source data length[n12..n0]
 	li		RX_TEMP1,0xb800a02C
 	sw		RX_TEMP2,0(RX_TEMP1)					//setting source data length
 	nop
/////////////GDMA memory copy (setup destination, BLK1)/////////////////////////////////
	add		RX_TEMP4,RX_TEMP3,RX_TEMP5
 	li		RX_TEMP1,0xb800a068
 	sw		RX_TEMP4,0(RX_TEMP1)			//setting destination data address
 	nop
 	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and destination data length[n12..n0]
 	li		RX_TEMP1,0xb800a06C
 	sw		RX_TEMP2,0(RX_TEMP1)			//setting destination data length
 	nop 	
 ///////////GDMA memory copy (setup source, BLK2)/////////////////////////////////
 	addiu	RX_TEMP3,RX_TEMP3,0x1000		//source data address
 	li		RX_TEMP1,0xb800a030
 	sw		RX_TEMP3,0(RX_TEMP1)			//setting source data address
 	nop
  	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and source data length[n12..n0]
 	li		RX_TEMP1,0xb800a034
 	sw		RX_TEMP2,0(RX_TEMP1)					//setting source data length
 	nop
/////////////GDMA memory copy (setup destination, BLK2)/////////////////////////////////
	add		RX_TEMP4,RX_TEMP3,RX_TEMP5
 	li		RX_TEMP1,0xb800a070
 	sw		RX_TEMP4,0(RX_TEMP1)			//setting destination data address
 	nop
 	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and destination data length[n12..n0]
 	li		RX_TEMP1,0xb800a074
 	sw		RX_TEMP2,0(RX_TEMP1)			//setting destination data length
 	nop 	 	
 ///////////GDMA memory copy (setup source, BLK3)/////////////////////////////////
 	addiu	RX_TEMP3,RX_TEMP3,0x1000		//source data address
 	li		RX_TEMP1,0xb800a038
 	sw		RX_TEMP3,0(RX_TEMP1)			//setting source data address
 	nop
  	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and source data length[n12..n0]
 	li		RX_TEMP1,0xb800a03C
 	sw		RX_TEMP2,0(RX_TEMP1)					//setting source data length
 	nop
/////////////GDMA memory copy (setup destination, BLK3)/////////////////////////////////
	add		RX_TEMP4,RX_TEMP3,RX_TEMP5
 	li		RX_TEMP1,0xb800a078
 	sw		RX_TEMP4,0(RX_TEMP1)			//setting destination data address
 	nop
 	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and destination data length[n12..n0]
 	li		RX_TEMP1,0xb800a07C
 	sw		RX_TEMP2,0(RX_TEMP1)			//setting destination data length
 	nop 	 	 	
 ///////////GDMA memory copy (setup source, BLK4)/////////////////////////////////
 	addiu	RX_TEMP3,RX_TEMP3,0x1000		//source data address
 	li		RX_TEMP1,0xb800a040
 	sw		RX_TEMP3,0(RX_TEMP1)			//setting source data address
 	nop
  	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and source data length[n12..n0]
 	li		RX_TEMP1,0xb800a044
 	sw		RX_TEMP2,0(RX_TEMP1)					//setting source data length
 	nop
/////////////GDMA memory copy (setup destination, BLK4)/////////////////////////////////
	add		RX_TEMP4,RX_TEMP3,RX_TEMP5
 	li		RX_TEMP1,0xb800a080
 	sw		RX_TEMP4,0(RX_TEMP1)			//setting destination data address
 	nop
 	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and destination data length[n12..n0]
 	li		RX_TEMP1,0xb800a084
 	sw		RX_TEMP2,0(RX_TEMP1)			//setting destination data length
 	nop 	 	 	 	
  ///////////GDMA memory copy (setup source, BLK5)/////////////////////////////////
  	addiu	RX_TEMP3,RX_TEMP3,0x1000		//source data address
 	li		RX_TEMP1,0xb800a048
 	sw		RX_TEMP3,0(RX_TEMP1)			//setting source data address
 	nop
  	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and source data length[n12..n0]
 	li		RX_TEMP1,0xb800a04C
 	sw		RX_TEMP2,0(RX_TEMP1)					//setting source data length
 	nop
/////////////GDMA memory copy (setup destination, BLK5)/////////////////////////////////
	add		RX_TEMP4,RX_TEMP3,RX_TEMP5
 	li		RX_TEMP1,0xb800a088
 	sw		RX_TEMP4,0(RX_TEMP1)			//setting destination data address
 	nop
 	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and destination data length[n12..n0]
 	li		RX_TEMP1,0xb800a08C
 	sw		RX_TEMP2,0(RX_TEMP1)			//setting destination data length
 	nop 	
  ///////////GDMA memory copy (setup source, BLK6)/////////////////////////////////
  	addiu	RX_TEMP3,RX_TEMP3,0x1000		//source data address
 	li		RX_TEMP1,0xb800a050
 	sw		RX_TEMP3,0(RX_TEMP1)			//setting source data address
 	nop
  	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and source data length[n12..n0]
 	li		RX_TEMP1,0xb800a054
 	sw		RX_TEMP2,0(RX_TEMP1)					//setting source data length
 	nop
/////////////GDMA memory copy (setup destination, BLK6)/////////////////////////////////
	add		RX_TEMP4,RX_TEMP3,RX_TEMP5
 	li		RX_TEMP1,0xb800a090
 	sw		RX_TEMP4,0(RX_TEMP1)			//setting destination data address
 	nop
 	li		RX_TEMP2,0x00001000				//[n31]=1, last data block, and destination data length[n12..n0]
 	li		RX_TEMP1,0xb800a094
 	sw		RX_TEMP2,0(RX_TEMP1)			//setting destination data length
 	nop  	
  ///////////GDMA memory copy (setup source, BLK7)/////////////////////////////////
 	addiu	RX_TEMP3,RX_TEMP3,0x1000		//source data address
 	li		RX_TEMP1,0xb800a058
 	sw		RX_TEMP3,0(RX_TEMP1)			//setting source data address
 	nop
  	li		RX_TEMP2,0x80001000				//[n31]=1, last data block, and source data length[n12..n0]
 	li		RX_TEMP1,0xb800a05C
 	sw		RX_TEMP2,0(RX_TEMP1)					//setting source data length
 	nop
/////////////GDMA memory copy (setup destination, BLK7)/////////////////////////////////
	add		RX_TEMP4,RX_TEMP3,RX_TEMP5
 	li		RX_TEMP1,0xb800a098
 	sw		RX_TEMP4,0(RX_TEMP1)			//setting destination data address
 	nop
 	li		RX_TEMP2,0x80001000				//[n31]=1, last data block, and destination data length[n12..n0]
 	li		RX_TEMP1,0xb800a09C
 	sw		RX_TEMP2,0(RX_TEMP1)			//setting destination data length
 	nop
/////////////Save current GDMA address.  /////////////////////////////////////////////////////////
	addiu	RX_TEMP3,RX_TEMP3,0x1000		//source data address
	li		RX_TEMP1,0xA0600028				
	sw		RX_TEMP3,0(RX_TEMP1)			//Save current GDMA address.
	nop
//////////////startting  GDMA process 	/////////////////////////////////
  	li		RX_TEMP2,0xC0000000				//GDMA_ENABLE | GDMA_POLL | GDMA_MEMCPY	//startting  GDMA process
 	li		RX_TEMP1,0xb800a000
 	sw		RX_TEMP2,0(RX_TEMP1)					//startting  GDMA 
 	nop
//////////////polling GDMA copy done flag	/////////////////////////////////
  	li		RX_TEMP2,0x0					// clean data buffer
  	li		RX_TEMP3,0x80000000
 	li		RX_TEMP1,0xb800a008				// GDMA	Interrupt Status Register
 	li		RX_TEMP4,0xb800030c				//RX_TEMP4 IP module status
 	lw		RX_TEMP5,0(RX_TEMP4)			//RX_TEMP5 0xb800030c value.
 	nop
 	
1:
 	lw		RX_TEMP2,0(RX_TEMP1)			// load 
 	nop
 	/////////////add IP module power on/off start/////////////////////////// 	
	//li		RX_TEMP4,0xb800030c
	//lw		RX_TEMP5,0(RX_TEMP4)
	//nop
 	ori		RX_TEMP6,RX_TEMP5,0x1FFF	//enable all IP module
 	sw		RX_TEMP6,0(RX_TEMP4)
 	nop
 	li		RX_TEMP6,0x50
 3:	
 	subu		RX_TEMP6,1
 	nop
 	bnez		RX_TEMP6,3b
 	nop
 	and		RX_TEMP6,RX_TEMP5,~0x1BFF		//enable GDMA only
 	sw		RX_TEMP6,0(RX_TEMP4)
 	nop
 	li		RX_TEMP6,0x50
 3:	
 	subu		RX_TEMP6,1
 	nop
 	bnez		RX_TEMP6,3b
 	nop 	
 	/////////////add IP module power on/off end/////////////////////////// 	
 	and 		RX_TEMP2,RX_TEMP2,RX_TEMP3	
 	nop
 	beqz		RX_TEMP2,1b						//GDMA_not_complete
 	nop 	 	
 	/////////////add IP module power on/off start /////////////////////////// 	
 	sw		RX_TEMP5,0(RX_TEMP4)			//restore IP module status.
 	nop 	
 	/////////////add IP module power on/off end/////////////////////////// 	
//////////load current GDMA adddress, if not equ finish GDMA address, it will jump to
	li		RX_TEMP1,0xA0600028
	lw		RX_TEMP2,0(RX_TEMP1)			//load current GDMA adddress
	nop
	li		RX_TEMP4,0xA0000000
	or		RX_TEMP2,RX_TEMP2,RX_TEMP4
	/////////////////////////////
	li		RX_TEMP1,0xA0600024
	lw		RX_TEMP3,0(RX_TEMP1)			//load finish GDMA adddress
	nop	
 	bgt		RX_TEMP3,RX_TEMP2,RX_GDMA_START	//if finish address > current adddress, jump to RX_GDMA_START
 	nop

//////////software compare data /////////////////
	li		RX_TEMP3,0x8000					//compare size 32k
	li		RX_TEMP4,0xA0600024
	lw		RX_TEMP6,0(RX_TEMP4)	//load finish GDMA address
	nop	
	sub		RX_TEMP3,RX_TEMP6,RX_TEMP3	//compare last block
	nop
	li		RX_TEMP1,0x0FFFF			//Pattern 
	li		RX_TEMP2,0x5a5aa5a5		//Pattern1
	li		RX_TEMP5,0				//compare length
1:
	lw		RX_TEMP4,0(RX_TEMP3)		//load data source data from SRAM
	nop
	bne		RX_TEMP1,RX_TEMP4,SDRAM_RX_COMPARE_FAIL
	nop
	addiu	RX_TEMP3,RX_TEMP3,4			//load address plus 4
	lw		RX_TEMP4,0(RX_TEMP3)		//load data source data from SRAM
	nop
	not		RX_TEMP5,RX_TEMP1
	bne		RX_TEMP5,RX_TEMP4,SDRAM_RX_COMPARE_FAIL
	nop	
	addiu	RX_TEMP3,RX_TEMP3,4			//load address plus 4
	lw		RX_TEMP4,0(RX_TEMP3)		//load data source data from SRAM
	nop
	bne		RX_TEMP2,RX_TEMP4,SDRAM_RX_COMPARE_FAIL
	nop	
	addiu	RX_TEMP3,RX_TEMP3,4			//load address plus 4
	lw		RX_TEMP4,0(RX_TEMP3)		//load data source data from SRAM
	nop
	not		RX_TEMP5,RX_TEMP2
	bne		RX_TEMP5,RX_TEMP4,SDRAM_RX_COMPARE_FAIL
	nop	
	addiu	RX_TEMP3,RX_TEMP3,4			//load address plus 4
	bgt		RX_TEMP6,RX_TEMP3,1b
	nop
	li		RX_TEMP3,1

SAVE_RX_VALUE:
	li		RX_TEMP1,0xA060001C
	lw		RX_TEMP2,0(RX_TEMP1)				//load rx pass parameter
	nop
	sll		RX_TEMP3,RX_TEMP3,RX_PARA
	nop
	or		RX_TEMP2,RX_TEMP2,RX_TEMP3
	sw		RX_TEMP2,0(RX_TEMP1)				//save rx pass parameter
	nop
	/////////////////////////////////////////////
	addiu	RX_PARA,RX_PARA,1					//Rx parameter plus 
	nop
	bne		RX_PARA,0x1F,RX_PRAR_CH			//if Rx parameter more then 0x10, set R0 value
	nop
	b			RX_FINISH										//add 20110601
	nop

SDRAM_RX_COMPARE_FAIL:
	addiu	RX_PARA,RX_PARA,1				//Tx parameter plus 
	nop
	beq		RX_PARA,0x1F,RX_FINISH			//TX_PRAR max 0x10
	nop
	b		RX_PRAR_CH
	nop	
	
RX_FINISH:
	////////////////////////////////////////////
	li		RX_TEMP1,0xA060001C
	lw		RX_TEMP2,0(RX_TEMP1)				//load rx pass parameter
	nop
	li		RX_TEMP3,0
	li		RX_TEMP4,1
1:	
	srl		RX_TEMP6,RX_TEMP2,RX_TEMP3
	and		RX_TEMP5,RX_TEMP4,RX_TEMP6
	addiu	RX_TEMP3,RX_TEMP3,1
	beq		RX_TEMP3,0x20,SDRAM_FIXED_VALUE
	nop
	beqz		RX_TEMP5,1b
	nop
	subu		RX_TEMP3,1
	move	RX_L0,RX_TEMP3
1:	
	addiu	RX_TEMP3,RX_TEMP3,1
	srl		RX_TEMP6,RX_TEMP2,RX_TEMP3
	and		RX_TEMP5,RX_TEMP4,RX_TEMP6	
	beq		RX_TEMP3,0x20,SDRAM_FIXED_VALUE
	nop	
	bnez		RX_TEMP5,1b
	nop
	sub		RX_TEMP6,RX_TEMP3,RX_L0
	add		RX_TEMP3,RX_TEMP3,RX_L0
	srl		RX_TEMP3,RX_TEMP3,1						//
	nop
	li		RX_TEMP2,0x8
	bge		RX_TEMP2,RX_TEMP6,SDRAM_FIXED_VALUE			//if R0-L0 <= 8, jump to Fixed value
	nop
	sll		RX_TEMP3,RX_TEMP3,8
	////////////////////////////////////
	li		RX_TEMP1,0xb8000220
	li		RX_TEMP4,0xFFFFE0FF
	lw		RX_TEMP2,0(RX_TEMP1)
	nop
	and		RX_TEMP2,RX_TEMP2,RX_TEMP4
	or		RX_TEMP3,RX_TEMP3,RX_TEMP2
	sw		RX_TEMP3,0(RX_TEMP1)				//load rx pass parameter
	nop	
	b		SDRAM_CALI_FINISH
	nop
	
SDRAM_FIXED_VALUE:	
 	li		RX_TEMP2,0x06000A00
 	li		RX_TEMP1,0xb8000220
 	sw		RX_TEMP2,0(RX_TEMP1)					//fixed RX delay
 	nop
 	nop	

SDRAM_CALI_FINISH:

#undef	RX_TEMP1
#undef	RX_TEMP2
#undef	RX_TEMP6
#undef	RX_REG_ADDR
#undef	RX_REG_VAL
#undef	RX_TEMP5
#undef	RX_PARA
#undef	RX_TEMP3
#undef	RX_TEMP4
#undef	RX_L0
#undef	RX_MSB__flag
#======SDRAM RX Calibration end===========
###############################################	
#if	1		//copy code to DMEM
	li		t7,0xa0600054			//load ra into t6
	lw		t6,0(t7)
	nop
	li		t7,0xa0600050
	lw		t8,0(t7)
	nop
	add		ra,t6,t8					//ra addr plus copy length offset
	j		ra
	nop
#endif		//copy code to DMEM	
SDRAM_RX_CALI_FINISH:
				nop
#endif		//RX calibration......				
				nop	
###############################################	
        b               MEM_CONF_DONE
        nop
        nop
        nop
#endif
///////////////////////////////////////////////////////////
MEM_CONF_DONE:
	nop
	
//	#--- invalidate the icache and dcache with a 0->1 transition
	mtc0	zero, $20
	nop
	nop
#ifdef CPU_LX4181
	li	t0, 0x202
#else
	li	t0, 0x3
#endif
	mtc0	t0, $20
	nop
	nop
	mtc0	zero, $20
	nop
	nop
//	#--- initialize and start COP3
	mfc0	t0,$12
	nop
	or		t0,0x80000000
	mtc0	t0,$12
	nop
	nop
	
//	# disable IRAM
	li		t0, 0x84000000	//#address without SDRAM
	and		t0, 0x0ffffc00    //# translate to physical address
	mtc3	t0, $0      //# $0: i-ram base
	nop
	nop
	li		t0, 4096 - 1
	mtc3	t0, $1     // # $1: i-ram top
	nop
	nop
	
//	# disable DRAM
	li	t0, 0x85000000	//#address without SDRAM
	and	t0, 0x0ffffc00   // # translate to physical address
	mtc3	t0, $4      //# $4: d-ram base
	nop
	nop
	li	t0, 4096 - 1
	mtc3	t0, $5     // # $5: d-ram top
	nop
	nop
//	#--- enable icache and dcache
	mtc0	$0, $20	//# CCTL
	nop
	nop

//--- configure memory controller
//---this is for ASIC
//--- determine memory configuraton 
#ifdef SDRAM_AUTO_DETECT
	Detect_RAM_Size
#endif //SDRAM_AUTO_DETECT


#if 0 // by xiachaoren  /* use default value 4MB flash size for bootloader */
	li		t0, 0xbfc0ff00			// parameter start address
keep_search:	
	lw		t1, 0(t0)			
	nop
	beq		t1, 0x4265726c, found_param	// is "Berl" ?
	nop
	add		t0, 0x40
	beq		t0, 0xbfc10000, default_config	// not found parameter, use default
	nop
	b		keep_search
found_param:
	li		a0, 0x02800000			// init mcr0 
	lw		t1, 48(t0)			// load flash size
	nop
	li		v0, 0xffffffff			// check if parameters stored in flash is valid?
	beq		t1, v0, default_config		// default setting in board.h MCR0
	nop
	li		t2, 0xb8001100			// NOR flash configuration register
	sw		t1, 0(t2)			// set NFCR
	nop
#ifndef SDRAM_AUTO_DETECT	
	lw		t1, 52(t0)			// load mem size
	nop
	li		t2, 0xB8001004
	sw		t1, 0(t2)			// set memory configuration
#endif //#ifndef SDRAM_AUTO_DETECT
	b		go_ahead
	nop
default_config:
#ifndef SDRAM_AUTO_DETECT
	li		t0,MCR0_VAL
	li		t1,0xB8001004
	sw		t0,0(t1)
#endif //#ifndef SDRAM_AUTO_DETECT
#endif
//    li		t0,	BOARD_PARAM_FLASHSIZE
	li		t0, 0xFFF88000
	li		t1, 0xB8001100
	sw		t0, 0(t1)
	nop
#if 0 // by xiachaoren
go_ahead:	
// Turns on MA22,MA21 if flash size > 4MB
    li              t0, 0xB8001100                  // load flash size parameter
    lw              t1, 0(t0)
	nop
	and		t1, 0x00070000			// retrieve flash size 
	srl		t1, 16				// shift right 15 bits
	addiu		t1, -4				
	blez		t1, copy_to_ram			// skip if flash size <= 4MB
	nop
	li		t2, 0xb8003304			// turns on NICLED1_MA22, NICLED0_MA21
	lw		t1, 0(t2)	
	nop
	or		t1, t1, 0x00c00000
	sw		t1, 0(t2)
	nop

#endif

copy_to_ram:
	la		t0,	in_ram
	nop
	lw		t1,	-4(t0)			/* t1 <--- num_got_entries */
	nop
	nop
	nop
	lw		t0,	-12(t0)			/* t0 <--- uboot_end_data */
	nop
	nop
	nop
	subu	t1,	0x02			/* skip first 2 entries */
	li		t2,	CONFIG_TEXT_BASE_BOOTSTART
	li		t3,	CONFIG_DRAM_BASE
	addu	t4,	t0,	0x04		/* t4 <--- point to stage 2 header */
	lw		t5,	0(t4)			/* t5 <--- image_len */
	nop
	nop
	nop
	subu	t4,	t0,	t2			/* t4 <--- length of stage 1 */
	addu	t4,	t4,	12			/* t4 <--- include the length of stage 2 header */
	addu	t5,	t5,	t4			/* t5 <--- total length of u-boot */
	
	addu	t4,	t3,	t5			/* t4 <--- destnation ending */
	move	t5,	t2				/* t5 src */
	move	t6,	t3				/* t6 dst */
copyloop:
	lw		t7,	0(t5)
	nop
	nop
	nop
	sw		t7,	0(t6)
	nop
	nop
	nop
	
	addu	t6,	0x04
	addu	t5,	0x04
	ble		t6,	t4,	copyloop
	nop
	
FINISH_COPY:
	nop

	/*  update global offset table */
	
	subu	t4,	t2,	t3		/* t4 <--- diff */
	move	t5,	gp
	subu	t5,	t5,	t4
	addi	t5,	t5,	8		/* skip first 2 entries */
gploop:
	lw		t6,	0(t5)
	nop
	nop
	nop
	subu	t7,	t6,	t4
	sw		t7,	0(t5)
	nop
	nop
	nop
	addi	t5,	t5,	0x04
	subu	t1,	0x01
	bgtz	t1,	gploop
	nop
	
	subu	gp,	t4

	/* flush cache */
	mtc0	zero, s4
	nop
	nop
	li	t0, 0x202
	mtc0	t0, s4
	nop
	nop
	mtc0	zero, s4
	nop
	nop

	/*  Set new stack pointer (in ram) */
	la		sp,	CONFIG_TEXT_BASE + BOOT_RUNTIME_SIZE - STACK_OFFSET
	nop
	nop
	nop
	/*  Jump to RAM */	
	addi	t9,	t3,	in_ram - _start
	jr		t9
	nop
	

	.word uboot_end_data
	.word uboot_end
	.word num_got_entries
in_ram:	
	/* load the addr of bootloader_second(in ram) to a0, argument of uncompress_stage2 */

	li      t0, SR_CU1|SR_CU0
	nop
	mtc0    t0, C0_SR               /* put on processor     */
	mtc0    zero, C0_CAUSE
	nop
	nop
	mfc0    t0, C0_SR
	nop

	la		t7,	in_ram
	lw		a0,	-12(t7)
	nop
	nop
	nop
	/* Clear BSS */
	
	la		t9,	uncompress_stage2
	jr		t9
	nop

/* Exception handlers. */
romReserved:
        b romReserved
romExcHandle:
        b romExcHandle


