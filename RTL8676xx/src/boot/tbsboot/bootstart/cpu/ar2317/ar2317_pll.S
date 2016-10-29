#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include "ar2316reg.h"

	.globl  pll_cpu184_amba_92
pll_cpu184_amba_92:

/* Configure PLLC for 184 MHz CPU and 92 MHz AMBA */

	/* Set PLLC_CTL */
	li	a0, K1BASE|AR2316_PLLC_CTL
	lw	t0, 0(a0)
         
	/* Choose Ref Div to 5 hence val to be 3 */
	and	t0, ~PLLC_REF_DIV_M
	or	t0, 0x3 << PLLC_REF_DIV_S
         
	/* Choose Div value to 23 */
	and	t0, ~PLLC_FDBACK_DIV_M
	or	t0, 0x17 << PLLC_FDBACK_DIV_S
         
	/* Choose Divby2 value to be 0 */
	and	t0, ~PLLC_ADD_FDBACK_DIV_M
	or	t0, 0 << PLLC_ADD_FDBACK_DIV_S
         
	/* Choose clkc value to be 368 % 2 = 184 Mhz */
	and	t0, ~PLLC_CLKC_DIV_M
	or	t0, 0 << PLLC_CLKC_DIV_S
         
	/* Choose clkm value to be 368 % 2 = 184 Mhz */
	and	t0, ~PLLC_CLKM_DIV_M
	or	t0, 0 << PLLC_CLKM_DIV_S
         
	/* Store the PLLc Control to be 40/5 * 2 * (0 + 1) * 23= 368 Mhz */
	sw	t0, 0(a0)
	sync 
	
	nop
	nop
	nop
	nop

		
	/* Set CPUCLK_CTL to use clkm / 1 = 184 */
	li	a0, K1BASE|AR2316_CPUCLK
	lw	t0, 0(a0)
	
	# Choose CLKm                                   
	and	t0, ~CPUCLK_CLK_SEL_M
	or	t0, 0  << CPUCLK_CLK_SEL_S  
         
	and	t0, ~CPUCLK_CLK_DIV_M
	or	t0, 0 << CPUCLK_CLK_DIV_S   # Choose div % 1 
         
	sw	t0, 0(a0)
	sync 
	
	nop
	nop
	nop
	nop
	
	/* Set AMBACLK_CTL to use clkc / 2 = 92MHz */
	li	a0, K1BASE|AR2316_AMBACLK
	lw	t0, 0(a0)
	
	and	t0, ~AMBACLK_CLK_SEL_M
	or	t0, 0 << AMBACLK_CLK_SEL_S
         
	and	t0, ~AMBACLK_CLK_DIV_M
	or	t0, 1 << AMBACLK_CLK_DIV_S
         
	sw	t0, 0(a0)
	sync 
	
	/* Set PLL to By pass Mode */
	li	a0, K1BASE|AR2316_MISCCLK
	li	t0, 0x0	
	sw	t0, 0(a0)
	sync 
	
	nop
	nop
	nop
	nop

	/* disable PCI & Local Bus Interface on 2316 by default */
	li	a0, K1BASE|AR2316_IF_CTL
	li	t0, IF_DISABLED
	sw	t0, 0(a0)
	sync

	nop
	nop
	nop
	nop

	j ra

