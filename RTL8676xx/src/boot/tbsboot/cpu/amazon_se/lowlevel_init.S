/*
 *  Memory sub-system initialization code for atheros7130.
 *
 *  Copyright (c) 20036  javen <zhangyu@twsz.com>
 *  
 *  lowlevel_init:
 *  
 *  This routine should initialize SDRAM.
 */

#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include "amazon_se_reg.h"

/*TODO: liupeng check !!! */
#define EBU_MODUL_BASE		0xB4102000
#define EBU_CLC(value)		0x0000(value)
#define EBU_CON(value)		0x0010(value)
#define EBU_ADDSEL0(value)	0x0020(value)
#define EBU_ADDSEL1(value)	0x0024(value)
#define EBU_ADDSEL2(value)	0x0028(value)
#define EBU_ADDSEL3(value)	0x002C(value)
#define EBU_BUSCON0(value)	0x0060(value)
#define EBU_BUSCON1(value)	0x0064(value)
#define EBU_BUSCON2(value)	0x0068(value)
#define EBU_BUSCON3(value)	0x006C(value)

#define MC_MODUL_BASE		0xBF800000
#define MC_ERRCAUSE(value)	0x0010(value)
#define MC_ERRADDR(value)	0x0020(value)
#define MC_CON(value)		0x0060(value)

#define MC_SRAM_ENABLE		0x00000004
#define MC_SDRAM_ENABLE		0x00000002
#define MC_DDRRAM_ENABLE	0x00000001

#define MC_SDR_MODUL_BASE	0xBF800200
#define MC_IOGP(value)		0x0000(value)
#define MC_CTRLENA(value)	0x0010(value)
#define MC_MRSCODE(value)	0x0020(value)
#define MC_CFGDW(value)		0x0030(value)
#define MC_CFGPB0(value)	0x0040(value)
#define MC_LATENCY(value)	0x0080(value)
#define MC_TREFRESH(value)	0x0090(value)
#define MC_SELFRFSH(value)	0x00A0(value)

#define MC_DDR_MODUL_BASE	0xBF801000
#define MC_DC00(value)		0x0000(value)
#define MC_DC01(value)		0x0010(value)
#define MC_DC02(value)		0x0020(value)
#define MC_DC03(value)		0x0030(value)
#define MC_DC04(value)		0x0040(value)
#define MC_DC05(value)		0x0050(value)
#define MC_DC06(value)		0x0060(value)
#define MC_DC07(value)		0x0070(value)
#define MC_DC08(value)		0x0080(value)
#define MC_DC09(value)		0x0090(value)
#define MC_DC10(value)		0x00A0(value)
#define MC_DC11(value)		0x00B0(value)
#define MC_DC12(value)		0x00C0(value)
#define MC_DC13(value)		0x00D0(value)
#define MC_DC14(value)		0x00E0(value)
#define MC_DC15(value)		0x00F0(value)
#define MC_DC16(value)		0x0100(value)
#define MC_DC17(value)		0x0110(value)
#define MC_DC18(value)		0x0120(value)
#define MC_DC19(value)		0x0130(value)
#define MC_DC20(value)		0x0140(value)
#define MC_DC21(value)		0x0150(value)
#define MC_DC22(value)		0x0160(value)
#define MC_DC23(value)		0x0170(value)
#define MC_DC24(value)		0x0180(value)
#define MC_DC25(value)		0x0190(value)
#define MC_DC26(value)		0x01A0(value)
#define MC_DC27(value)		0x01B0(value)
#define MC_DC28(value)		0x01C0(value)
#define MC_DC29(value)		0x01D0(value)
#define MC_DC30(value)		0x01E0(value)
#define MC_DC31(value)		0x01F0(value)
#define MC_DC32(value)		0x0200(value)
#define MC_DC33(value)		0x0210(value)
#define MC_DC34(value)		0x0220(value)
#define MC_DC35(value)		0x0230(value)
#define MC_DC36(value)		0x0240(value)
#define MC_DC37(value)		0x0250(value)
#define MC_DC38(value)		0x0260(value)
#define MC_DC39(value)		0x0270(value)
#define MC_DC40(value)		0x0280(value)
#define MC_DC41(value)		0x0290(value)
#define MC_DC42(value)		0x02A0(value)
#define MC_DC43(value)		0x02B0(value)
#define MC_DC44(value)		0x02C0(value)
#define MC_DC45(value)		0x02D0(value)
#define MC_DC46(value)		0x02E0(value)

#define RCU_OFFSET  0xBF203000
#define RCU_RST_REQ      (RCU_OFFSET + 0x0010)
#define RCU_STS          (RCU_OFFSET + 0x0014)

#define CGU_OFFSET  0xBF103000
#define  PLL0_CFG     (CGU_OFFSET + 0x0004)
#define  PLL1_CFG     (CGU_OFFSET + 0x0008)
#define  PLL2_CFG     (CGU_OFFSET + 0x000C)
#define  CGU_SYS      (CGU_OFFSET + 0x0010)
#define  CGU_UPDATE   (CGU_OFFSET + 0x0014)
#define  IF_CLK       (CGU_OFFSET + 0x0018)
#define  CGU_SMD      (CGU_OFFSET + 0x0020)
#define  CGU_CT1SR    (CGU_OFFSET + 0x0028)
#define  CGU_CT2SR    (CGU_OFFSET + 0x002C)
#define  CGU_PCMCR    (CGU_OFFSET + 0x0030)
#define  PCI_CR_PCI   (CGU_OFFSET + 0x0034)
#define  CGU_OSC_CTRL (CGU_OFFSET + 0x001C)
#define  CGU_MIPS_PWR_DWN (CGU_OFFSET + 0x0038)
#define  CLK_MEASURE  (CGU_OFFSET + 0x003C)

//05252006
#define  pll0_35MHz_CONFIG 0x9D861059
#define  pll1_35MHz_CONFIG 0x1A260CD9
#define  pll2_35MHz_CONFIG 0x8000f1e5
#define  pll0_36MHz_CONFIG 0x1000125D 
#define  pll1_36MHz_CONFIG 0x1B1E0C99
#define  pll2_36MHz_CONFIG 0x8002f2a1 
//05252006

//06063001-joelin disable the PCI CFRAME mask -start
/*CFRAME is an I/O signal, in the chip, the output CFRAME is selected via GPIO altsel pins, so if you select MII1 RXD1, the CFRAME will not come out.
But the CFRAME input still take the signal from the pad and not disabled when altsel choose other function. So when MII1_RXD1 is low from other device, the EBU interface will be disabled.

The chip function in such a way that disable the CFRAME mask mean EBU not longer check CFRAME to be the device using the bus.
The side effect is the entire PCI block will see CFRAME low all the time meaning PCI cannot use the bus at all so no more PCI function.
*/
#define PCI_CR_PR_OFFSET  0xBE105400
#define PCI_CR_PCI_MOD_REG          (PCI_CR_PR_OFFSET + 0x0030)
#define PCI_CONFIG_SPACE  0xB7000000
#define CS_CFM		(PCI_CONFIG_SPACE + 0x6C)

#define IFX_CONFIG_MEMORY_SIZE 16       //add by xgl

	.globl	lowlevel_init
lowlevel_init:
	/* EBU, CGU and SDRAM/DDR-RAM Initialization.
	 */
        /*cgu initialization*/
        li      t1, 0xbf103010
        li      t2, 0x20
        sw      t2, 0(t1)


	li	t1, MC_MODUL_BASE

	/* Clear Error log registers */
	sw	zero, MC_ERRCAUSE(t1)
	sw	zero, MC_ERRADDR(t1)

	li      t2, 0x2
	sw      t2, 0x200(t1)

	li      t2, 0x0
	sw      t2, 0x210(t1)

	li      t2, 0x2
	sw      t2, 0x230(t1)

	li      t2, 0x20
	sw      t2, 0x220(t1)

	li      t2, 0x14c8
	sw      t2, 0x240(t1)
#if 0

#if (IFX_CONFIG_MEMORY_SIZE == 8)
	li      t2, 0x14c8
#endif

#if (IFX_CONFIG_MEMORY_SIZE == 16)
	li      t2, 0x14c9
#endif
	sw      t2, 0x240(t1)
  
#if (IFX_CONFIG_MEMORY_SIZE == 8)
        li      t2, 0x14c8
#endif

#if (IFX_CONFIG_MEMORY_SIZE == 16)
        li      t2, 0x14c9
#endif
#endif
 	 li      t2, 0x14c8
        sw      t2, 0x250(t1)

	li      t2, 0x36325
	sw      t2, 0x280(t1)

	li      t2, 0x81d
	sw      t2, 0x290(t1)

	li      t2, 0x0
	sw      t2, 0x2A0(t1)

        li      t2, 0x1
	sw      t2, 0x210(t1)

     //  li      t1, 0xbe105360 
	//li      t2, 0x1d7ff
	//sw      t2, 0x0(t1)

        li      t1, 0xbe190400
	li      t2, 0
	sw      t2, 0x0(t1)


	j	ra
	nop


