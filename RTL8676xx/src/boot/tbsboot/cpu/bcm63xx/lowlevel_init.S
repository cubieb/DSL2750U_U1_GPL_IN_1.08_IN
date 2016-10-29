/*
 *  Memory sub-system initialization code for Broadcom 6338.
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
#include "63xx_common.h"

#if CONFIG_BCM6338
	.globl  lowlevel_init
lowlevel_init:


	li      t1,SDRAM_BASE

	
	/* Enable prioritization */ 
        lw      t2,SDR_PRIOR(t1)
        or      t2,SDR_EN_PRIOR
        or      t2,SDR_ADSL_HPE
        sw      t2,SDR_PRIOR(t1)
        


        /* Default SDRAM settings for BCM6338 board (8MB, 1 chip) */
        li      t3,SDR_PEND|SDR_2_BANKS|SDR_8_COLS|SDR_12_ROWS
        li      v0,8

1:
        or      t3,SDR_MASTER_EN|SDR_MRS_CMD|SDR_PEND
        li      t2,SDR_SLOW_MEM|SDR_FULL_PG
        sw      t2,SDR_CFG_REG(t1)    # Full page burst, fast memory
        li      t2,SDR_MASTER_EN|SDR_PRE_CMD
        sw      t2,SDR_INIT_CTL(t1)   # Pre-charge powerup sequence
        li      t2,SDR_MASTER_EN|SDR_CBR_CMD
        sw      t2,SDR_INIT_CTL(t1)   # 8 or more auto-refresh cmds
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t3,SDR_INIT_CTL(t1)   # Mode Register set,
        li      t2,SDR_REF_EN|0x40    # Refresh period 0x40, Enable
        sw      t2,SDR_REF_CTL(t1)

        j       ra
        nop
#endif

#if CONFIG_BCM6348
	.globl  lowlevel_init
lowlevel_init:



        li      t1,SDRAM_BASE          

        /* Enable prioritization */
        lw      t2,SDR_PRIOR(t1)
        or      t2,SDR_EN_PRIOR
        or      t2,SDR_ADSL_HPE
        sw      t2,SDR_PRIOR(t1)
        
        /* check memory config type 32MB and default to 8MB */
        li      t0,BOARD_SDRAM_TYPE_ADDRESS
        FIXUP(t0)
        lw      t0,0(t0)
		
        /* setting for 32MB, 2 chip */
        li      t3,SDR_2_BANKS|SDR_32_BIT|SDR_9_COLS|SDR_12_ROWS
        li      v0,32
        li      t4,BP_MEMORY_32MB_2_CHIP
        beq     t0,t4,1f
        nop
 
        /* setting for 16MB, 2 chip */
        li      t3,SDR_2_BANKS|SDR_32_BIT|SDR_8_COLS|SDR_12_ROWS
        li      v0,16
        li      t4,BP_MEMORY_16MB_2_CHIP
        beq     t0,t4,1f
        nop
      
        /* Default SDRAM settings for BCM6348 board (8MB, 1 chip) */
        li      t3,SDR_2_BANKS|SDR_32_BIT|SDR_8_COLS|SDR_11_ROWS
        li      v0,8
			
1:
        or      t3,SDR_MASTER_EN|SDR_MRS_CMD|SDR_PEND
        li      t2,SDR_SLOW_MEM|SDR_FULL_PG
        sw      t2,SDR_CFG_REG(t1)    # Full page burst, fast memory
        li      t2,SDR_MASTER_EN|SDR_PRE_CMD
        sw      t2,SDR_INIT_CTL(t1)   # Pre-charge powerup sequence
        li      t2,SDR_MASTER_EN|SDR_CBR_CMD
        sw      t2,SDR_INIT_CTL(t1)   # 8 or more auto-refresh cmds
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t2,SDR_INIT_CTL(t1)
        sw      t3,SDR_INIT_CTL(t1)   # Mode Register set, 
        li      t2,SDR_REF_EN|0x40    # Refresh period 0x40, Enable 
        sw      t2,SDR_REF_CTL(t1)    

        j       ra
        nop
#endif 


#if CONFIG_BCM6358
	.globl  lowlevel_init ;
	.ent	name;		
lowlevel_init:

#        li      t2, 0x00000000
#       sw      t2, MEMC_BASE
#        li      t2, 0x00000000
#      sw      t2, MEMC_BASE+4       

        li      sp, 0xfff84000                  # for now use the buffer manager memory for stack
        sub     sp, 4
        sw      ra, 0(sp)

        .set    noreorder

        li      t0, MEMC_BASE
        li      t1, DDR_BASE

        /* setting for 32MB, 1 chip */
        li      t2, (MEMC_13BIT_ROW | MEMC_9BIT_COL)
 
        or      t2, (MEMC_SELPRIORITY | MEMC_HIPRRTYQEN | MEMC_MIPS1HIPREN)
        or      t2, (MEMC_DQSGATEEN | MEMC_16BIT_BUS | MEMC_MEMTYPE_DDR)
        sw      t2, MEMC_CONFIG(t0)             # Enable DDR Mem & SEQ EN, 16MB

        li      t2, 0x7                         # Reduce drive strength for command pins (per John Lorek)
        sw      t2, DDR_CMD_PAD_CNTL(t1)

        li      t2, 0x000754d9
        sw      t2, MEMC_DRAM_TIM(t0)           # DDR Timing Set Latency  2.5 Latency
        li      t2, 0x00000003
        sw      t2, MEMC_CONTROL(t0)            # Turn on CKE 
        li      t2, 0x0000000B
        sw      t2, MEMC_CONTROL(t0)            # PreCharge 
        li      t2, 0x00004002
        sw      t2, MEMC_M_EM_BUF(t0)           # Value for Extended Mode Register
        li      t2, 0x00000013
        sw      t2, MEMC_CONTROL(t0)            # MRS command
        li      t2, 0x00000163
        sw      t2, MEMC_M_EM_BUF(t0)           # Reset DLL, Burst Length = 8, Burst Type Sequential  2.5 Latency
        li      t2, 0x00000013
        sw      t2, MEMC_CONTROL(t0)            # MRS command
        nop                                     # Delay 200 DDR clock cycles (~1.5 uS)
        nop
        nop
        li      t2, 0x0000000B
        sw      t2, MEMC_CONTROL(t0)            # Precharge 
        li      t2, 0x0000840f
        sw      t2, MEMC_REF_PD_CONTROL(t0)     # Enable auto refresh
        li      t2, 0x00000007
        sw      t2, MEMC_CONTROL(t0)            # Set Auto Refresh Mode  
        li      t2, 0x00000007
        sw      t2, MEMC_CONTROL(t0)            # Set Auto Refresh Mode  
        li      t2, 0x00000063
        sw      t2, MEMC_M_EM_BUF(t0)           # Reset DLL, Burst Length = 8, Burst Type Sequential 2.5 Latency 
        li      t2, 0x00000013
        sw      t2, MEMC_CONTROL(t0)            # MRS
	j ra
	nop
#endif 

#if USE_EXT
        # It's very important to turn off the counter bypass bit BEFORE writing
        # new values to the phase control registers.
        li      t2, (7 << 28) | (7 << 24) | (1<<20) | (7 << 16) # BUI Clock | Bus Clock | Enable PLL cnt updt | Other Clocks
        sw      t2, DDR_MIPS_PHASE_CNTL(t1)

        # Calculate a value for a 90 degree phase shift.
        lw      t2, DDR_MIPSDDR_PLL_CONFIG(t1)
        srl     t2, 8                               # Shift and mask off DDR_MDIV
        and     t2, 0xff
        sll     t2, 3                               # PI_steps = (90deg * 32 * MBUS(t2) + 2)/360 ~= MBUS * 8
        or      t2, (1 << 13)                       # set the count direction

        lw      t3, DDR_DDR3_4_PHASE_CNTL0(t1)      # Get current DDR3/4 value.
        ori     t3, 0x7fff                          # Clear low 15 bits (DDR3 value).
        xori    t3, 0x7fff
        or      t3, t2                              # Set new DDR3 value, preserving existing DDR4 value.
        sw      t3, DDR_DDR3_4_PHASE_CNTL0(t1)

        .set    reorder
        
       /** jal     sdramDqsPhaseSet **/
        
        /**----- switch to sync -----------------------------------------**/
        li      t0, 0xff410000
        li      t1, DDR_BASE
        li      t2, 2048
        li      t3, 1

1:
        lw      t4, 0x40(t0)                # Read a sample value.
        srl     t4, 16                      # The sample is in the upper 16 bits.
 
        andi    t4, t4, 0x41                # Look at the 2 outermost bits; if the LSB is 0 and the MSB is 1, 
        beq     t4, 0x40, 2f                # then there is an edge somewhere in the sample.
 
        and     t5, t3, 0x7ff               # Adjust the sample window.
        or      t5, (7 << 28) | (7 << 24) | (1<<20) | (7 << 16) # BUI Clock | Bus Clock | Enable PLL cnt updt | Other Clocks
        sw      t5, DDR_MIPS_PHASE_CNTL(t1)
 
        lw      a1, 0x40(t0)                # Delay before reading another sample.
        add     t3, 1
        bne     t2, t3, 1b
        b       3f

2:
        # Success
        lw      t2, DDR_MIPS_PHASE_CNTL(t1) # Turn on auto-PI mode.
        and     t2, 0xffff
        or      t2, (7 << 28) | (1 << 24) | (1<< 21) | (1<<20) | (7 << 16) # BUI Clock | Bus Clock | Enable PI | Enable PLL cnt updt | Other Clocks
        sw      t2, DDR_MIPS_PHASE_CNTL(t1)

        li      t2, 0x0010                  # Set PI mask to 0000110, and check new value every 16 MIPS cycles.
        sw      t2, 0x40(t0)                # set PI update to 16 ddr cycles
        li      t2, 0x80000090              # Enable MIPS auto-PI | Enable update period | Set 16 clock update
        sw      t2, 0x40(t0)
        li      t2, 0x80000c90              # Enable MIPS auto-PI | Enable comparator | Enable update period | Set 16 clock update
        sw      t2, 0x40(t0)
        
        lw      t2, 0x40(t0)                # Do a few reads to wait till the edge is stable...
        lw      t2, 0x40(t0)
        lw      t2, 0x40(t0)
        lw      t2, 0x40(t0)
        lw      t2, 0x40(t0)
 
        .word   0x4009b005                  # mfc t1, C0_BCM_CONFIG, 5
        and     t1, ~(0x1 << 28)
        .word   0x4089b005                  # mtc t1, C0_BCM_CONFIG, 5

3:
        /**----- Enable RAC and LMB -------------------------------------**/
        li      t1, MIPS_BASE
        lw      t2, MIPS_LMB_CR(t1)
        or      t2, LMB_EN | DISABLE_UCS_FIX  # Enable LMB
        sw      t2, MIPS_LMB_CR(t1)

        li      t2, 0x1000 << RAC_UPB_SHFT    # Enable prefetch for any address
        sw      t2, MIPS_RAC_ARR(t1)

        lw      t2, MIPS_RAC_CR0(t1)
        or      t2, (RAC_C_INV | RAC_I | RAC_PF_I)
        sw      t2, MIPS_RAC_CR0(t1)

        lw      t2, MIPS_RAC_CR1(t1)
        or      t2, (RAC_C_INV | RAC_I | RAC_PF_I)
        sw      t2, MIPS_RAC_CR1(t1)

        /**----- Enable branch prediction and non-blocking data cache ---**/
        mfc0    t1, C0_BCM_CONFIG
        and     t1, ~CP0_BCM_CFG_BTHD
        or      t1, CP0_BCM_CFG_NBK
        mtc0    t1, C0_BCM_CONFIG

        lw      ra, 0(sp)
        add     sp, 4
        j       ra        
        nop
#endif 
