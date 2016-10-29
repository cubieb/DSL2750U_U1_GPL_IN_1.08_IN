#include <config.h>
#include <version.h>
#include <asm/regdef.h>
#include "ar7100_soc.h"
#include "autoconf.h"

#define PLL_CONFIG_SW_UPDATE_VAL (1 << 31)
#define CLOCK_CTRL_SWITCH_VAL    (1 << 1)
    
/*
 * Helper macros.
 * These Clobber t7, t8 and t9
 */
#define clear_mask(_reg, _mask)                     \
    li  t7, _reg|K1BASE;                            \
    lw  t8, 0(t7);                                  \
    li  t9, ~_mask;                                 \
    and t8, t8, t9;                                 \
    sw  t8, 0(t7)            

#define set_val(_reg, _mask, _val)                  \
    li  t7, _reg|K1BASE;                            \
    lw  t8, 0(t7);                                  \
    li  t9, ~_mask;                                 \
    and t8, t8, t9;                                 \
    li  t9, _val;                                   \
    or  t8, t8, t9;                                 \
    sw  t8, 0(t7)            

#define set_val_reg(_reg, _mask, _valreg)           \
    li  t7, _reg;                                   \
    lw  t8, 0(t7);                                  \
    li  t9, ~_mask;                                 \
    and t8, t8, t9;                                 \
    or  t8, t8, _valreg;                            \
    sw  t8, 0(t7)


#define get_val(_reg, _mask, _shift, _res_reg)      \
    li  t7, _reg|K1BASE;                                   \
    lw  t8, 0(t7);                                  \
    li  t9, _mask;                                  \
    and t8, t8, t9;                                 \
    srl _res_reg, t8, _shift                        \

#define pll_clr(_mask)                              \
    clear_mask(AR7100_CPU_PLL_CONFIG, _mask)

#define pll_set(_mask, _val)                        \
    set_val(AR7100_CPU_PLL_CONFIG,  _mask, _val)

#define pll_set_reg(_mask, _reg)                    \
    set_val_reg(AR7100_CPU_PLL_CONFIG,  _mask, _reg)

#define pll_get(_mask, _shift, _res_reg)            \
    get_val(AR7100_CPU_PLL_CONFIG, _mask, _shift, _res_reg)

#define clk_clr(_mask)                               \
    clear_mask(AR7100_CPU_CLOCK_CONTROL, _mask)

#define clk_set(_mask, _val)                         \
    set_val(AR7100_CPU_CLOCK_CONTROL,  _mask, _val)

#define clk_get(_mask, _shift, _res_reg)              \
    get_val(AR7100_CPU_CLOCK_CONTROL, _mask, _shift, _res_reg)

 #define MEMDELAY(count, reg)    \
			li      reg, count;     \
	9: 	addi    reg, -1;        \
			bgtz    reg, 9b;        \
			nop


	.globl  set_pll
set_pll: 

#ifdef CONFIG_PLL_800_400_400

	li 	t0, (0x0  << PLL_CONFIG_CPU_DIV_SHIFT)
    	li 	t1, (0x1  << PLL_CONFIG_DDR_DIV_SHIFT)
    	li 	t2, (0x0  << PLL_CONFIG_AHB_DIV_SHIFT)	//ahb = cpu / 2 
    	li 	t3, (0x13 << PLL_CONFIG_PLL_FB_SHIFT)
    	li 	t4, (0x0  << PLL_CONFIG_PLL_LOOP_BW_SHIFT)

#elif CONFIG_PLL_600_400_300
	li 	t0, (0x1  << PLL_CONFIG_CPU_DIV_SHIFT)
    	li 	t1, (0x2  << PLL_CONFIG_DDR_DIV_SHIFT)
    	li 	t2, (0x0  << PLL_CONFIG_AHB_DIV_SHIFT)	//ahb = cpu / 2 
    	li 	t3, (0x1d << PLL_CONFIG_PLL_FB_SHIFT)
    	li 	t4, (0x0  << PLL_CONFIG_PLL_LOOP_BW_SHIFT)

#elif CONFIG_PLL_600_400_150
	li 	t0, (0x1  << PLL_CONFIG_CPU_DIV_SHIFT)
    	li 	t1, (0x2  << PLL_CONFIG_DDR_DIV_SHIFT)
    	li 	t2, (0x1  << PLL_CONFIG_AHB_DIV_SHIFT)	//ahb = cpu / 4
    	li 	t3, (0x1d << PLL_CONFIG_PLL_FB_SHIFT)
    	li 	t4, (0x0  << PLL_CONFIG_PLL_LOOP_BW_SHIFT)

#elif CONFIG_PLL_400_400_200
    	li 	t0, (0x1  << PLL_CONFIG_CPU_DIV_SHIFT)
    	li 	t1, (0x1  << PLL_CONFIG_DDR_DIV_SHIFT)
    	li 	t2, (0x0  << PLL_CONFIG_AHB_DIV_SHIFT)
    	li 	t3, (0x13 << PLL_CONFIG_PLL_FB_SHIFT)
    	li 	t4, (0x3  << PLL_CONFIG_PLL_LOOP_BW_SHIFT)

#elif CONFIG_PLL_333_333_166
    	li 	t0, (0x2  << PLL_CONFIG_CPU_DIV_SHIFT)
    	li 	t1, (0x2  << PLL_CONFIG_DDR_DIV_SHIFT)
    	li 	t2, (0x0  << PLL_CONFIG_AHB_DIV_SHIFT)
    	li 	t3, (0x18 << PLL_CONFIG_PLL_FB_SHIFT)
    	li 	t4, (0x3  << PLL_CONFIG_PLL_LOOP_BW_SHIFT)

#elif CONFIG_PLL_266_266_133
    	li 	t0, (0x2  << PLL_CONFIG_CPU_DIV_SHIFT)
    	li 	t1, (0x2  << PLL_CONFIG_DDR_DIV_SHIFT)
    	li 	t2, (0x0  << PLL_CONFIG_AHB_DIV_SHIFT)
    	li 	t3, (0x13 << PLL_CONFIG_PLL_FB_SHIFT)
    	li 	t4, (0x3  << PLL_CONFIG_PLL_LOOP_BW_SHIFT)

#elif CONFIG_PLL_266_266_66
    	li 	t0, (0x2  << PLL_CONFIG_CPU_DIV_SHIFT)
    	li 	t1, (0x2  << PLL_CONFIG_DDR_DIV_SHIFT)
    	li 	t2, (0x1  << PLL_CONFIG_AHB_DIV_SHIFT)
    	li 	t3, (0x13 << PLL_CONFIG_PLL_FB_SHIFT)
    	li 	t4, (0x3  << PLL_CONFIG_PLL_LOOP_BW_SHIFT)

#elif CONFIG_PLL_200_200_100
    	li 	t0, (0x3  << PLL_CONFIG_CPU_DIV_SHIFT)
    	li 	t1, (0x3  << PLL_CONFIG_DDR_DIV_SHIFT)
    	li 	t2, (0x0  << PLL_CONFIG_AHB_DIV_SHIFT)
    	li 	t3, (0x13 << PLL_CONFIG_PLL_FB_SHIFT)
    	li 	t4, (0x3  << PLL_CONFIG_PLL_LOOP_BW_SHIFT)

#else  /* CONFIG_PLL_300_300_150 */
    	li 	t0, (0x3  << PLL_CONFIG_CPU_DIV_SHIFT)
    	li 	t1, (0x3  << PLL_CONFIG_DDR_DIV_SHIFT)
    	li 	t2, (0x0  << PLL_CONFIG_AHB_DIV_SHIFT)
    	li 	t3, (0x1d << PLL_CONFIG_PLL_FB_SHIFT)
    	li 	t4, (0x0  << PLL_CONFIG_PLL_LOOP_BW_SHIFT)
    	
#endif

    	b 	got_setting
    	nop

got_setting:
	clk_get(CLOCK_CONTROL_RST_SWITCH_MASK, CLOCK_CONTROL_RST_SWITCH_SHIFT, t6)
	bne zero, t6, initialize_pll
	nop 

	pll_set_reg(PLL_CONFIG_CPU_DIV_MASK, t0)
	pll_set_reg(PLL_CONFIG_AHB_DIV_MASK, t2)
	pll_set_reg(PLL_CONFIG_DDR_DIV_MASK, t1)

	pll_set(PLL_CONFIG_SW_UPDATE_MASK, PLL_CONFIG_SW_UPDATE_VAL)

	/*
	 * Will cause a reset
	 */
	clk_set(CLOCK_CONTROL_RST_SWITCH_MASK, CLOCK_CTRL_SWITCH_VAL)
	clk_set(CLOCK_CONTROL_CLOCK_SWITCH_MASK, 1) 
	nop
    
initialize_pll:
	pll_set(PLL_CONFIG_SW_UPDATE_MASK, PLL_CONFIG_SW_UPDATE_VAL)
	clk_clr(CLOCK_CONTROL_RST_SWITCH_MASK)
	pll_set_reg(PLL_CONFIG_PLL_FB_MASK, t3)
	pll_set_reg(PLL_CONFIG_PLL_LOOP_BW_MASK, t4)

	pll_clr(PLL_CONFIG_PLL_POWER_DOWN_MASK);
	pll_clr(PLL_CONFIG_PLL_BYPASS_MASK);

	/* set pll for ethernet and pci*/
	li      a0, 0xb8050004 
	li	t0, 0x50C0	//gen 1Ghz
	sw	t0, 0(a0)

	li      a0, 0xb8050018 
	li	t0, 0x1313	//ethernet 25Mhz (base)
	sw	t0, 0(a0)

	li      a0, 0xb805001c
	//li	t0, 0x77	//PCI clk 66 MHz
	li	t0, 0xee	//PCI clk 33 MHz
	sw	t0, 0(a0)


wait_for_pll_lock:
	pll_get(PLL_CONFIG_LOCKED_MASK, PLL_CONFIG_LOCKED_SHIFT, t6)
	beq zero, t6, wait_for_pll_lock
	nop 

pll_locked:
	clk_set(CLOCK_CONTROL_CLOCK_SWITCH_MASK, 1)
	nop

	j ra
	nop
	
