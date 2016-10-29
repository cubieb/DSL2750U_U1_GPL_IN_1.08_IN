/*
 * Copyright (C) 2007-2008 Mindspeed Technologies Inc.
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#include <common.h>
#include "hardware.h"
#include "bsp_1000.h"


void SoC_Check_Device(void)
{
	// This should run on a M83xxx SoC, let's check it
	u32 devicetype = (*(volatile u32*)(GPIO_DEVID_REG)) & 0xFF;

	if (devicetype != 0x170)
	{
		//error wrong device
		// TODO: what should we do?
	}
}

void SoC_APB_setup(void)
{
	// Setting APB Bus Wait states to 0, set post write
	*(volatile u32*)(APB_ACCESS_WS_REG) = 0x40;
}

void SoC_ARAM_setup(void)
{
	// enable ARAM collision detection
	// read mod write to set bit 2 to enable.  Keep default of 32 byte mask
	*(volatile u32*)(MEMCORE_BASEADDR+0x4) |= 4;
	// disable ARAM pipeline mode
	*(volatile u32*)(MEMCORE_BASEADDR+0x38) = 0;
}


void SoC_AHB_setup(void)
{
	// DDR size 512MB, starting from 80000000
	*(volatile u32*) AHB_HIGHMEM_ADDR_TRANSLATION = 0x9FFF; 	//(AHB_BASEADDR + 0x04)
	// Disable REMAP (Low Mem region is mapped to DDR base)
	//*(volatile u32*) AHB_LOWMEM_REMAP_CTRL = 0x0;		//(AHB_BASEADDR + 0x00)
	// 2nd stage level muxs are configured to round-robin
	*(volatile u32*) AHB_STAGE2_ARBITER_CFG	= 0x0;	//(AHB_BASEADDR + 0x08)
}

void SoC_PLL_init(void)
{
	u32 armpll = CLKCORE_ARM_PLL_DEFAULT;
	u32 ahbpll = CLKCORE_AHB_PLL_DEFAULT;
	u32 phypll = CLKCORE_PHY_PLL_DEFAULT;
	u32 armclk = CFG_ARM_CLOCK;
	u32 ahbclk = CFG_HZ_CLOCK;
	u32 ddrclk = CFG_DDR_CLOCK; 
	u32 boot_option;

	*(volatile u32*)(GPIO_ARM_MEMORY_SENSE_REGS) = 0x1B6DB6DB;
	// Take DDR controller out of reset
	*(volatile u32*)(CLKCORE_BLK_RESET) |= BLK_RESET_DDR_REF_RESET_N | BLK_RESET_DDR_AHB_RESET_N;

#if defined(CONFIG_BOARD_C1KASIC)
	// read bootstrap
	boot_option = *(volatile u32*)(GPIO_BOOTSTRAP_STATUS_REG) & 0xC00;
	
	switch (boot_option) {
	case 0x400:
		// rate case 1 ARM pll 650MHz, AHB pll 800, PHY pll 250  => FCLK=650 AHB=200 DDR=400
		break;
	case 0x800:
		// rate case 2 ARM pll 650MHz, AHB pll 800, PHY pll 250  => FCLK=650 AHB=200 DDR=200
		ddrclk = ahbclk;
		break;
	case 0xC00:
		// rate case 3 ARM pll 610MHz, AHB pll 744, PHY pll 250  => FCLK=610 AHB=186 DDR=372
		armpll = 610000000;
		ahbpll = 744000000;
		phypll = CLKCORE_PHY_PLL_DEFAULT;
		armclk = 610000000;
		ahbclk = 186000000;
		ddrclk = 372000000;
		break;
	default:
		break;
	}
#endif

	HAL_all_pll_bypass();
	HAL_set_arm_pll(armpll);
	HAL_set_ahb_pll(ahbpll);
	HAL_set_phy_pll(phypll);
	*(volatile u32 *)CLKCORE_DDR_PCIE_CLK_CNTRL &= ~DDR_PCIE_CLK_CNTRL_IPsec_DIV0_BYPASS;
	HAL_set_arm_clk(armclk, armclk);
	HAL_set_ahb_ddr_clk(ahbclk, ddrclk);
	HAL_set_phy_gem_clk(CFG_GEM0_CLOCK, CFG_GEM1_CLOCK, CFG_PHY_CLOCK);
	HAL_all_pll_remove_bypass();
}

void SoC_nand_init(void)
{
	// TODO: verify timings

	// Configure CS4 - 8 bits - NAND
	*(volatile u32*)(EX_CS4_CFG_REG) = EX_MEM_BUS_8 | EX_NAND_MODE;
	// timings (CSw = 9, RE WE 9), 
	*(volatile u32*)(EX_CS4_TMG1_REG) = 9 | 9<<16 | 9<<24 ;
	*(volatile u32*)(EX_CSEN_REG)  |= EX_CS4_EN;	
}

/*
* SoC_gpio_cfg - configure GPIO pins as input or output pins
*
* gpio - gpio pin
*
* mode - gpio pin mode
*	GPIO_TYPE_OUTPUT = output
*	GPIO_TYPE_INPUT = input
*
*/
int SoC_gpio_cfg(int gpio, int mode)
{
	if ((gpio < 0) || (gpio > 31))
		goto err;

	switch (mode) {
	case GPIO_TYPE_INPUT:
	default:
		*(volatile u32 *) GPIO_OE_REG &= ~(1 << gpio);
		break;

	case GPIO_TYPE_OUTPUT:
		*(volatile u32 *) GPIO_OE_REG |= 1 << gpio;
		break;
	}

	return 0;

err:
	return -1;
}

