/*
 * Copyright (C) 2006 Mindspeed Technologies Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <common.h>
#include <config.h>
#include <asm/mach-types.h>
#include "bsp_1000.h"
#include "comcerto_1000.h"


DECLARE_GLOBAL_DATA_PTR;


static void config_board1(void)
{
  // Set Ipsec clock to 266MHz (AHB_Pll/3)
  *(volatile u32 *)CLKCORE_DDR_PCIE_CLK_CNTRL =
    (*(volatile u32 *)CLKCORE_DDR_PCIE_CLK_CNTRL & ~0xf000UL) | 0x3000;
  // Configure DDR
	*(volatile u64*)(DENALI_CTL_00_DATA) = (u64)DENALI_CTL_00_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_01_DATA) = (u64)DENALI_CTL_01_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_02_DATA) = (u64)DENALI_CTL_02_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_03_DATA) = (u64)DENALI_CTL_03_VAL_CFG1;
//	*(volatile u64*)(DENALI_CTL_04_DATA) = (u64)DENALI_CTL_04_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_05_DATA) = (u64)DENALI_CTL_05_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_06_DATA) = (u64)DENALI_CTL_06_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_07_DATA) = (u64)DENALI_CTL_07_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_08_DATA) = (u64)DENALI_CTL_08_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_09_DATA) = (u64)DENALI_CTL_09_VAL_CFG1;
//	*(volatile u64*)(DENALI_CTL_10_DATA) = (u64)DENALI_CTL_10_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_11_DATA) = (u64)DENALI_CTL_11_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_12_DATA) = (u64)DENALI_CTL_12_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_13_DATA) = (u64)DENALI_CTL_13_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_14_DATA) = (u64)DENALI_CTL_14_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_15_DATA) = (u64)DENALI_CTL_15_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_16_DATA) = (u64)DENALI_CTL_16_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_17_DATA) = (u64)DENALI_CTL_17_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_18_DATA) = (u64)DENALI_CTL_18_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_19_DATA) = (u64)DENALI_CTL_19_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_20_DATA) = (u64)DENALI_CTL_20_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_21_DATA) = (u64)DENALI_CTL_21_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_22_DATA) = (u64)DENALI_CTL_22_VAL_CFG1;
//	*(volatile u64*)(DENALI_CTL_23_DATA) = (u64)DENALI_CTL_23_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_24_DATA) = (u64)DENALI_CTL_24_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_25_DATA) = (u64)DENALI_CTL_25_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_26_DATA) = (u64)DENALI_CTL_26_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_27_DATA) = (u64)DENALI_CTL_27_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_28_DATA) = (u64)DENALI_CTL_28_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_29_DATA) = (u64)DENALI_CTL_29_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_30_DATA) = (u64)DENALI_CTL_30_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_31_DATA) = (u64)DENALI_CTL_31_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_32_DATA) = (u64)DENALI_CTL_32_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_33_DATA) = (u64)DENALI_CTL_33_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_34_DATA) = (u64)DENALI_CTL_34_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_35_DATA) = (u64)DENALI_CTL_35_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_36_DATA) = (u64)DENALI_CTL_36_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_37_DATA) = (u64)DENALI_CTL_37_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_38_DATA) = (u64)DENALI_CTL_38_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_39_DATA) = (u64)DENALI_CTL_39_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_40_DATA) = (u64)DENALI_CTL_40_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_41_DATA) = (u64)DENALI_CTL_41_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_42_DATA) = (u64)DENALI_CTL_42_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_43_DATA) = (u64)DENALI_CTL_43_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_44_DATA) = (u64)DENALI_CTL_44_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_45_DATA) = (u64)DENALI_CTL_45_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_46_DATA) = (u64)DENALI_CTL_46_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_47_DATA) = (u64)DENALI_CTL_47_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_48_DATA) = (u64)DENALI_CTL_48_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_49_DATA) = (u64)DENALI_CTL_49_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_50_DATA) = (u64)DENALI_CTL_50_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_51_DATA) = (u64)DENALI_CTL_51_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_52_DATA) = (u64)DENALI_CTL_52_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_53_DATA) = (u64)DENALI_CTL_53_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_54_DATA) = (u64)DENALI_CTL_54_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_55_DATA) = (u64)DENALI_CTL_55_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_56_DATA) = (u64)DENALI_CTL_56_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_57_DATA) = (u64)DENALI_CTL_57_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_58_DATA) = (u64)DENALI_CTL_58_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_59_DATA) = (u64)DENALI_CTL_59_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_60_DATA) = (u64)DENALI_CTL_60_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_61_DATA) = (u64)DENALI_CTL_61_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_62_DATA) = (u64)DENALI_CTL_62_VAL_CFG1;
//	*(volatile u64*)(DENALI_CTL_63_DATA) = (u64)DENALI_CTL_63_VAL_CFG1;
//	*(volatile u64*)(DENALI_CTL_64_DATA) = (u64)DENALI_CTL_64_VAL_CFG1;
//	*(volatile u64*)(DENALI_CTL_65_DATA) = (u64)DENALI_CTL_65_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_66_DATA) = (u64)DENALI_CTL_66_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_67_DATA) = (u64)DENALI_CTL_67_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_68_DATA) = (u64)DENALI_CTL_68_VAL_CFG1;
	*(volatile u64*)(DENALI_CTL_69_DATA) = (u64)DENALI_CTL_69_VAL_CFG1;

	// start DDRC
	*(volatile u64*)(DENALI_CTL_02_DATA) = (u64)DENALI_CTL_02_VAL_CFG1 | (1LL << 32);
	//wait int_status[2] (DRAM init complete)
	while(( *(volatile u32*)(DENALI_CTL_36_DATA+8) & 0x40000) == 0);
//	*(volatile u64*)(DENALI_CTL_11_DATA) = ((u64)DENALI_CTL_11_VAL_CFG1 & ~(0x00007F0000000000LL)) | (wr_dqs_shift << 40);

}

static void config_board2(void)
{
  // Set Ipsec clock to 266MHz (AHB_Pll/3)
  *(volatile u32 *)CLKCORE_DDR_PCIE_CLK_CNTRL =
    (*(volatile u32 *)CLKCORE_DDR_PCIE_CLK_CNTRL & ~0xf000UL) | 0x3000;
  // Configure DDR
	*(volatile u64*)(DENALI_CTL_00_DATA) = (u64)DENALI_CTL_00_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_01_DATA) = (u64)DENALI_CTL_01_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_02_DATA) = (u64)DENALI_CTL_02_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_03_DATA) = (u64)DENALI_CTL_03_VAL_CFG2;
//	*(volatile u64*)(DENALI_CTL_04_DATA) = (u64)DENALI_CTL_04_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_05_DATA) = (u64)DENALI_CTL_05_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_06_DATA) = (u64)DENALI_CTL_06_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_07_DATA) = (u64)DENALI_CTL_07_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_08_DATA) = (u64)DENALI_CTL_08_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_09_DATA) = (u64)DENALI_CTL_09_VAL_CFG2;
//	*(volatile u64*)(DENALI_CTL_10_DATA) = (u64)DENALI_CTL_10_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_11_DATA) = (u64)DENALI_CTL_11_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_12_DATA) = (u64)DENALI_CTL_12_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_13_DATA) = (u64)DENALI_CTL_13_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_14_DATA) = (u64)DENALI_CTL_14_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_15_DATA) = (u64)DENALI_CTL_15_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_16_DATA) = (u64)DENALI_CTL_16_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_17_DATA) = (u64)DENALI_CTL_17_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_18_DATA) = (u64)DENALI_CTL_18_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_19_DATA) = (u64)DENALI_CTL_19_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_20_DATA) = (u64)DENALI_CTL_20_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_21_DATA) = (u64)DENALI_CTL_21_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_22_DATA) = (u64)DENALI_CTL_22_VAL_CFG2;
//	*(volatile u64*)(DENALI_CTL_23_DATA) = (u64)DENALI_CTL_23_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_24_DATA) = (u64)DENALI_CTL_24_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_25_DATA) = (u64)DENALI_CTL_25_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_26_DATA) = (u64)DENALI_CTL_26_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_27_DATA) = (u64)DENALI_CTL_27_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_28_DATA) = (u64)DENALI_CTL_28_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_29_DATA) = (u64)DENALI_CTL_29_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_30_DATA) = (u64)DENALI_CTL_30_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_31_DATA) = (u64)DENALI_CTL_31_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_32_DATA) = (u64)DENALI_CTL_32_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_33_DATA) = (u64)DENALI_CTL_33_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_34_DATA) = (u64)DENALI_CTL_34_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_35_DATA) = (u64)DENALI_CTL_35_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_36_DATA) = (u64)DENALI_CTL_36_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_37_DATA) = (u64)DENALI_CTL_37_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_38_DATA) = (u64)DENALI_CTL_38_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_39_DATA) = (u64)DENALI_CTL_39_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_40_DATA) = (u64)DENALI_CTL_40_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_41_DATA) = (u64)DENALI_CTL_41_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_42_DATA) = (u64)DENALI_CTL_42_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_43_DATA) = (u64)DENALI_CTL_43_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_44_DATA) = (u64)DENALI_CTL_44_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_45_DATA) = (u64)DENALI_CTL_45_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_46_DATA) = (u64)DENALI_CTL_46_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_47_DATA) = (u64)DENALI_CTL_47_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_48_DATA) = (u64)DENALI_CTL_48_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_49_DATA) = (u64)DENALI_CTL_49_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_50_DATA) = (u64)DENALI_CTL_50_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_51_DATA) = (u64)DENALI_CTL_51_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_52_DATA) = (u64)DENALI_CTL_52_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_53_DATA) = (u64)DENALI_CTL_53_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_54_DATA) = (u64)DENALI_CTL_54_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_55_DATA) = (u64)DENALI_CTL_55_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_56_DATA) = (u64)DENALI_CTL_56_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_57_DATA) = (u64)DENALI_CTL_57_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_58_DATA) = (u64)DENALI_CTL_58_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_59_DATA) = (u64)DENALI_CTL_59_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_60_DATA) = (u64)DENALI_CTL_60_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_61_DATA) = (u64)DENALI_CTL_61_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_62_DATA) = (u64)DENALI_CTL_62_VAL_CFG2;
//	*(volatile u64*)(DENALI_CTL_63_DATA) = (u64)DENALI_CTL_63_VAL_CFG2;
//	*(volatile u64*)(DENALI_CTL_64_DATA) = (u64)DENALI_CTL_64_VAL_CFG2;
//	*(volatile u64*)(DENALI_CTL_65_DATA) = (u64)DENALI_CTL_65_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_66_DATA) = (u64)DENALI_CTL_66_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_67_DATA) = (u64)DENALI_CTL_67_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_68_DATA) = (u64)DENALI_CTL_68_VAL_CFG2;
	*(volatile u64*)(DENALI_CTL_69_DATA) = (u64)DENALI_CTL_69_VAL_CFG2;

	// start DDRC
	*(volatile u64*)(DENALI_CTL_02_DATA) = (u64)DENALI_CTL_02_VAL_CFG2 | (1LL << 32);
	//wait int_status[2] (DRAM init complete)
	while(( *(volatile u32*)(DENALI_CTL_36_DATA+8) & 0x40000) == 0);
//	*(volatile u64*)(DENALI_CTL_11_DATA) = ((u64)DENALI_CTL_11_VAL_CFG2 & ~(0x00007F0000000000LL)) | (wr_dqs_shift << 40);

}



void apply_ddr_setting(void)
{
#if 0
	u32 boot_option = *(volatile u32*)(GPIO_BOOTSTRAP_STATUS_REG) & 0xC00;
	switch (boot_option) {
		case 0x400:
			// rate case 1 ARM pll 650MHz, AHB pll 800, PHY pll 250  => FCLK=650 AHB=200 DDR=400
			config_board1();	
		break;
		case 0x800:
			// rate case 2 ARM pll 650MHz, AHB pll 800, PHY pll 250  => FCLK=650 AHB=200 DDR=200
			config_board2();
		break;
		default:
			config_board1();
			break;
	}
#else
			// rate case 2 ARM pll 650MHz, AHB pll 800, PHY pll 250  => FCLK=650 AHB=200 DDR=200
			
#if (CFG_DDR_CLOCK == 400000000) 
	config_board1();
#elif (CFG_DDR_CLOCK == 200000000)
	config_board2();
#endif
	
	/* training is required for 400MHz only,
         * but nice to have for 200MHz also */
#if defined(DDR_TRAINING)
		/* Do training */
		start_training();
#endif

#endif
}

u32 get_ddr_size(void)
{
	return CFG_SDRAM_SIZE;
}


static void nor_hw_init(void)
{
	*(volatile u32 *)EX_CS0_SEG_REG = 0xFFF;
// AHB runs at 200MHz , and EXP at 40MHz
	*(volatile u32 *)EX_CLOCK_DIV_REG = 0x5;
}

void bsp_init(void)
{
	SoC_PLL_init();
	SoC_APB_setup();
	SoC_AHB_setup();
	SoC_ARAM_setup();

	//while(1);
	icache_enable();

	//Apply DENALI DDR configuration
	apply_ddr_setting();
//	SoC_nand_init();
}

void nand_hw_init(void)
{
        SoC_gpio_cfg(29, GPIO_TYPE_OUTPUT);
        SoC_gpio_cfg(30, GPIO_TYPE_OUTPUT);
        SoC_gpio_cfg(31, GPIO_TYPE_OUTPUT);
        *(volatile u32 *) GPIO_PIN_SELECT_REG &= ~((1 << 31)|(1 << 30)|(1 << 29)|(1 << 21)|(1 << 20)|(1 << 5));
        *(volatile u32 *) GPIO_PIN_SELECT_REG |= (1 << 6);
}

void i2c_hw_init(void)
{
        *(volatile u32 *) GPIO_PIN_SELECT_REG &= ~((1 << 19)|(1 << 18));
}

int board_init(void)
{
	nor_hw_init();
#if (CONFIG_COMMANDS & CFG_CMD_NAND)
	nand_hw_init();
#endif
	i2c_hw_init();

	/* Setup External reset*/
	SoC_gpio_cfg(17, GPIO_TYPE_OUTPUT);
	udelay(10);
	SoC_gpio_set_0(SoC_gpio_mask(17));
	udelay(10);
	SoC_gpio_set_1(SoC_gpio_mask(17));

	/* arch number of Mindspeed Comcerto */
	//gd->bd->bi_arch_number = MACH_TYPE_M825XX;

	/* adress of boot parameters */
	//gd->bd->bi_boot_params = BOOTARGS_OFFSET;

	return 0;
}

int dram_init(void)
{
#if 0
	gd->bd->bi_dram[0].start = CONFIG_DRAM_BASE;
	gd->bd->bi_dram[0].size = CFG_SDRAM_SIZE;
#endif
	return 0;
}

int misc_init_r(void)
{
	return 0;
}

#ifdef BOARD_LATE_INIT
int board_late_init(void)
{
#ifdef DDR_TRAINING_DBG
	u8 wr_dqs = 0; 
	u8 dqs_out = 0;
	u8 dqs_delay0 = 0;
	u8 dqs_delay1 = 0;
	u8 dqs_delay2 = 0;
	u8 dqs_delay3 = 0;
#endif

#if 0
	printf("Reserve MSP memory\n");
	gd->bd->bi_dram[0].start += MSP_BOTTOM_MEMORY_RESERVED_SIZE;
	gd->bd->bi_dram[0].size -= MSP_BOTTOM_MEMORY_RESERVED_SIZE + MSP_TOP_MEMORY_RESERVED_SIZE;
#endif

#ifdef DDR_TRAINING_DBG
	wr_dqs = DENALI_WR_DQS; 
	dqs_out = DENALI_DQS_OUT;
	dqs_delay0 = DENALI_DQS_DELAY0;
	dqs_delay1 = DENALI_DQS_DELAY1;
	dqs_delay2 = DENALI_DQS_DELAY2;
	dqs_delay3 = DENALI_DQS_DELAY3;
	printf("DDR Training : ");
	printf("wr_dqs 0x%x dqs_out 0x%x delay0 0x%x delay1 0x%x delay2 0x%x delay3 0x%x\n", wr_dqs,dqs_out,dqs_delay0,dqs_delay1,dqs_delay2,dqs_delay3);
#endif

	return 0;
}

#endif
