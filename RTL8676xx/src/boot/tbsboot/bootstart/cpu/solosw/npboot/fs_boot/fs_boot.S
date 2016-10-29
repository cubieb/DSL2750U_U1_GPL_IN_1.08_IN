/*
// *****************************************************************************
//
//       Bandit family boot: first-stage boot code
//       =========================================
//
//       This code can be loaded under three conditions:
//
//       Flash Boot - after a reset the on-chip boot ROM will load the code
//               from FLASH into SRAM and execute it
//
//       
// *****************************************************************************
*/

#include "chf_sdram.h"
#include "bf_devconf.h"	     //Defines for pll initialisation
#include "arm1026.h"         //Defines for SDRAM on chip initialisation


#define    CHM_MAP_PERIPH    0xff000000	
#define    MEMSIZE_WORD_A    0xf1e2c3d4      // Test patterns for SDRAM sizing
#define    MEMSIZE_WORD_B    0x12345678
#define    MEMSIZE_WORD_C    0x87654321
#define    CASWIDTH_WORD     0x12340000
#define    MEMSIZE_INCREMENT 1024*1024       // Memory test address increment
#define    MEMSIZE_MAXIMUM   1024*1024*256   // 256MB maximum memory



#define    BF_DEVCONF                                (0x40000000)
#define    BF_DEVCONF_REGS_CONFIG_SDRAM_CLK_DEL       (0x78)         
#define    BF_DEVCONF_SDRAM_DEL1_100                 (0x00000002)

#define    CHM_MAP_SDRAMU                            (0x20000000)      /* Start of SDRAM (uncached)    */
#define    CHM_MAP_FLASH                             (0x38000000)      /* Start of FLASH               */
#define    CHM_SDRAM_BOOT                            (0x20400000)      /* Start of SDRAM for boot      */
#define    CHM_MAP_DTCM                              (0x60000000)
#define    STACK_SIZE                                (0x400)           /* 1K stack */


#define PLL_TEST_ENABLE 0x70
#define FPLL1_POST_DEV  0x4A
#define FPLL1_VCO_RANGE 0x8481
#define FPLL0_POST_DEV  0x48
#define FPLL0_VCO_RANGE 0x5A81
#define PLL_TEST_ENABLE 0x70

#define CHARM_TIMER1_VALUE  (0x108)


/*
 *************************************************************************
 *
 * Startup Code (reset vector)
 * 
 * do important init only if we don't start from memory!
 * setup Memory and board specific bits prior to relocation.
 * relocate armboot to ram
 * setup stack
 *       
 *************************************************************************
 */


.globl reset
reset:
        /*
         * set the cpu to SVC32 mode
         */
        mrs     r0,cpsr
        bic     r0,r0,#0x1f
        orr     r0,r0,#0xd3
        msr     cpsr,r0


cpu_init_crit:
        /*
         * flush v4 I/D caches
         */
        mov     r0, #0
        mcr     p15, 0, r0, c7, c7, 0   /* flush v3/v4 cache */
        mcr     p15, 0, r0, c8, c7, 0   /* flush v4 TLB */

        /*
         * disable MMU stuff and caches
         */
        mrc     p15, 0, r0, c1, c0, 0
        bic     r0, r0, #0x00002300     /* clear bits 13, 9:8 (--V- --RS) */
        bic     r0, r0, #0x00000087     /* clear bits 7, 2:0 (B--- -CAM) */
        orr     r0, r0, #0x00000002     /* set bit 2 (A) Align */
        orr     r0, r0, #0x00001000     /* set bit 12 (I) I-Cache */
        mcr     p15, 0, r0, c1, c0, 0


        // Set up stack for subroutine calls
        // Put stack in D-TCM
        ldr     r0,=CHM_MAP_DTCM
        add     sp, r0, #STACK_SIZE

/*
// *****************************************************************************
//
//      _mem_init:       
//
//      Do SDRAM memory on chip initialisation
//        
//      Entry:
//
//      Exit:
//
// *****************************************************************************
*/

_mem_init:
        // Set up data TCM
        // Read current region register
        mrcal   15, 0, r0, ARM1026_CP15_xTCM_REGION, ARM1026_CP15_xTCM_REGION2, ARM1026_CP15_xTCM_REGION_DTCM
        // Clear address
        ldr     r1, =ARM1026_CP15_xTCM_REGION_BASE
        bic     r0, r0, r1
        // Set base
        orr     r0, r0, #CHM_MAP_DTCM
        // Set enable bit
        orr     r0, r0, #ARM1026_CP15_xTCM_REGION_ENABLED
        // Write back
        mcral   15, 0, r0, ARM1026_CP15_xTCM_REGION, ARM1026_CP15_xTCM_REGION2, ARM1026_CP15_xTCM_REGION_DTCM


/*       
*******************************************************************************
*       
*       Set up PLLs, and enter MISSION mode
*       
*******************************************************************************
*/      
        bl pll_init


/*
*******************************************************************************
*
*       Do an sdram on board initialisation
*
*******************************************************************************
*/
        bl sdram_init
       
        


/*
*******************************************************************************
*       
*    Copy codes from flash to sdram
*
// *****************************************************************************
//
//       flash_boot
//
//       This reads the flash boot program from flash. This program needs
//       to comprehend the format that mkflash has built the flash in.
//       This is
//
//       Word 0: NP boot length in words (b)
//       Word 1: First word of NP boot
//       :
//       Word b:Last word of NP boot
//       Word b + 1: NP boot checksum
//       Word b + 2: Number of unused words (u)
//       :
//       Word b + u + 3: PP boot length (p)
//       Word b + u + 4: First word of PP boot
//       :
//       Word b + u + p + 3:Last word of PP boot
//       Word b + u + p + 4: Number of unused words (v)
//       :
//       Word b + u + p + v + 4: Configuration Information 
//       :
//
//       Entry:
//
*******************************************************************************
*/


flash_boot:



        //// R11:  non-zero for 8-bit, zero for 16-bit
        mov   r11, #1


        //// Find the address of the next stage
        //// First, get count of NP words

        ldr     r0,=CHM_MAP_FLASH      // Start of flash in memory space
        mov     r6, r0

        ldr     r0, [r6]                // Read first word from flash
        cmn     r0, #1                  // 0xffffffff?
        cmpne   r0, #0                  // ...or 0x00000000?
        beq     err_exit                // Display the address, and error message

        //// Get unused count
        mov     r4, r0                  // Save for later
        add     r0, r0, #2                  // address b + 2 is number of unused words
        mov     r0, r0, lsl #2          // convert from word to a byte address
        ldr     r0, [r6, r0]            // Read unused count
        add     r4, r0, r4              // b + u
        add     r4, r4, #3                  // b + u + 3 is length of PP boot
        mov     r4, r4, lsl #2          // convert to byte address

        //// Get count of PP words
        ldr     r0, [r6, r4]
        cmn     r0, #1                  // 0xffffffff?
        cmpne   r0, #0                  // ...or 0x00000000?
        beq     err_exit
        mov     r5, r0                  // Save word count

        //// r0:  Word count
        //// r4:  Holds address of PP word count
        //// r5:  Word count
read_words:
        ldr     r0, =CHM_SDRAM_BOOT        // Start of SDRAM
        mov     r7, r0
        //// Loop, with
        //// r4:  Address to read from next
        //// r5:  Count of words still to read
        //// r7:  Destination to write to next        
0:
        add     r4, r4, #4              // Increment address to read from
        ldr     r0, [r6, r4]            // Read next word
        str     r0, [r7], #4            // Store word, and increment address
        subs    r5, r5, #1              // Decrement word count
        bne     0b                     // Loop until all is done

        //// Set up entry conditions to nextstage_loaded
        mov     r7, #0
        mov     r11, #1


        // Jump to runn

        ldr    r0,=CHM_SDRAM_BOOT
	mov    pc,r0


// *****************************************************************************
//
//       Error messages
//
// *****************************************************************************

err_exit:
        b err_exit


     







/******************************************************************************
//
//     pll_init
//
//     Set up PLLs and associated configuration
//     Results in switching to MISSION mode
//
//     Prints out a warning and does nothing else if already in mission mode        
//     Entry:
//     sp: Stack
//
//     Exit:
//
//
******************************************************************************/

pll_init:

        //// Save registers on stack
        stmfd   sp!, {r10, lr}

        //// Throughout this routine, assume that r10 holds the base of the devconf registers
        mov     r10, #BF_DEVCONF
        



#ifndef GALLUS_WLC_VALIDATION_BOARD
		// change the regular voltage from 1.26 to 1.22
		ldr	r0, [r10,#BF_DEVCONF_REGS_CONFIG_DIAGNOSTIC]
		orr	r0, r0, #0xE0000000
		str	r0, [r10,#BF_DEVCONF_REGS_CONFIG_DIAGNOSTIC]
#endif



        //// PLL/DLL configuaration - refer to DO-412760-PS  

        //// Ensure DLL/PLL is reset        
        ldr     r0, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]
        bic     r0, r0, #BF_DEVCONF_PLL_DLL_NRST
        str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]                
        
        //// Disable the DLL and AFE clocks. 
        //// Write 00 to bits 5-4 of PLLDLL Control Register.
        bic     r0, r0, #(BF_DEVCONF_DLL_CLK_ENABLE | BF_DEVCONF_AFE_CLK_ENABLE)
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]
	
	//// configuring the PLLDLL macro (Fout = 141.312 Mhz, using values from DO-419028)
	ldr	r0, =BF_DEVCONF_AFE_PLL_P_VAL 
	ldr     r1, =BF_DEVCONF_AFE_PLL_D_VAL
	ldr	r2, =BF_DEVCONF_AFE_PLL_M_VAL 
	ldr     r3, =0x7000
	ldr     r4, =0x700
	ldr     r5, =0x7000
	ldr     r11, =0x8500
	ldr     r8, =0xf0
	ldr     r9, =0xf1		
	bl	pll_setup	
	                       
	
        //// FPLL1 configuration - refer to DO-412760-PS       
        
        //// Reset the FPLL1.         
        ldr     r0, [r10,#BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL]
#ifndef FIB_PART        
        orr     r0, r0, #PLL_TEST_ENABLE
        str     r0, [r10,#BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL]               
#endif        
        
        bic     r0, r0, #BF_DEVCONF_FPLL_SOFT_NRST
        str     r0, [r10,#BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL]
            
	//// Disable Fout of FPLL1.	
        bic     r0, r0, #BF_DEVCONF_FPLL_FOUT_EN
	str     r0, [r10,#BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL]     

        //// Write POSTDIV value to the FPLLx Divider Control Register.
        ldr     r1, =FPLL1_POST_DEV
        str     r1, [r10,#BF_DEVCONF_REGS_F_PLL1_DIVIDER_CONTROL]      
        
        //// Set VCO_RANGE in FPLLx Delta Sigma Control 1 Register..
        ldr     r1, =FPLL1_VCO_RANGE
        str     r1, [r10,#BF_DEVCONF_REGS_F_PLL1_DELTA_CONTROL1]
	

        //// FPLL0 configuration - refer to DO-412760-PS         
        
        //// Reset the FPLL0
        ldr     r0, [r10,#BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL]
#ifndef FIB_PART
        orr     r0, r0, #PLL_TEST_ENABLE
        str     r0, [r10,#BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL]                       
#endif        
        
        bic     r0, r0, #BF_DEVCONF_FPLL_SOFT_NRST
        str     r0, [r10,#BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL]        
	
	//// Disable Fout of FPLL0.	
        bic     r0, r0, #BF_DEVCONF_FPLL_FOUT_EN
	str     r0, [r10,#BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL]    

        //// Write POSTDIV value to the FPLLx Divider Control Register.
        ldr     r1, =FPLL0_POST_DEV
        str     r1, [r10,#BF_DEVCONF_REGS_F_PLL0_DIVIDER_CONTROL]      
        
        //// Set VCO_RANGE in FPLLx Delta Sigma Control 1 Register..
        ldr     r1, =FPLL0_VCO_RANGE        
        str     r1, [r10,#BF_DEVCONF_REGS_F_PLL0_DELTA_CONTROL1]


        //// Take PLLs out of reset , and wait for them to lock
        //// Retry this up to three times - sometimes the PLLs don't lock fist time
        
        mov     r4, #3             

pll_retry_enable:        

	//// Bring the PLL/DLL out of reset.
        ldr     r0, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]
        orr     r0, r0, #BF_DEVCONF_PLL_DLL_NRST
        str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]    
        
        //// Bring the FPLL1 out of reset.
        ldr     r0, [r10,#BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL]
        orr     r0, r0, #BF_DEVCONF_FPLL_SOFT_NRST
	str     r0, [r10,#BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL]  
	
        //// Bring the FPLL0 out of reset.
        ldr     r0, [r10,#BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL]
        orr     r0, r0, #BF_DEVCONF_FPLL_SOFT_NRST
	str     r0, [r10,#BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL]
	
	
	
        ////  Wait for PLLs to lock.
        mov     r3, #0x400000                           // Number of times to loop, waiting
        mov     r1, #(BF_DEVCONF_PLL_DLL_LOCK_STATUS | BF_DEVCONF_FPLL1_LOCK_STATUS | BF_DEVCONF_FPLL0_LOCK_STATUS)
        
0:
        subs    r3, r3, #1                                           // Decrement counter
        beq     1f                                                   // Failed to lock in timeout
	ldr	r2, [r10,#BF_DEVCONF_REGS_LOCK_STATUS]   		     // Get lock status
	and	r2, r2, r1                                               // Mask to get the bits we're interested in
	cmp	r2, r1                                               // Are the bits all set?
	bne	0b                                                   // No - loop


        //// PLLs have locked - skip over retry code
        b       plls_locked_ok

1:
        //// PLLs didn't lock - retry
        subs    r4, r4, #1                              // Decrement retry counter
        beq     pll_lock_fail                           // We've retried enough times - give up

        //// Put PLLs back into reset
        
        //// Ensure DLL/PLL is reset        
        ldr     r0, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]
        bic     r0, r0, #BF_DEVCONF_PLL_DLL_NRST
        str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]
        
        //// Reset the FPLL0. 
        ldr     r0, [r10,#BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL]
        bic     r0, r0, #BF_DEVCONF_FPLL_SOFT_NRST
        str     r0, [r10,#BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL]  
        
        //// Reset the FPLL1.         
        ldr     r0, [r10,#BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL]
        bic     r0, r0, #BF_DEVCONF_FPLL_SOFT_NRST
        str     r0, [r10,#BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL]                  
        

        //// Wait for a bit
        bl      delay

        
        //// Retry
        b       pll_retry_enable


plls_locked_ok:

	
        //// Indicate that PLLs have locked and clocks have been enabled.

        //// PLLs have successfully locked        
        //// Now enable all the clocks
        ldr     r0, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]
	orr	r0, r0, #(BF_DEVCONF_DLL_CLK_ENABLE | BF_DEVCONF_AFE_CLK_ENABLE)    
        str	r0, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]
        

	// Disable bypass mode of FPLL1.
	ldr     r0, [r10,#BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL]
	bic     r0, r0, #BF_DEVCONF_FPLL_BYPASS_PLL
	str     r0, [r10,#BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL]	
	
	// Enable Fout of FPLL1.
        orr     r0, r0, #BF_DEVCONF_FPLL_FOUT_EN
	str     r0, [r10,#BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL] 
		
	// Disable bypass mode of FPLL0.
	ldr     r0, [r10,#BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL]
	bic     r0, r0, #BF_DEVCONF_FPLL_BYPASS_PLL
	str     r0, [r10,#BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL]
		
	// Enable Fout of FPLL0.
        orr     r0, r0, #BF_DEVCONF_FPLL_FOUT_EN
	str     r0, [r10,#BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL]
	

	//// Now that PLL/DLLs are locked, do
        //// Domain clock configuration        
        //// ARM Clock (ARM Configuration Register):
        

	//// Configure Input Select to desired clock.
        ldr     r0, [r10,#BF_DEVCONF_REGS_CONFIG_ARMCK]	
        orr     r0, r0, #BF_DEVCONF_INPUT_FPLL1_FOUT_DIV2
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_ARMCK]      


        //// Dnova Clock (DNOVACK Configuration Register):
        
        //// Configure Input Select to Crystal clock.
        ldr     r0, [r10,#BF_DEVCONF_REGS_CONFIG_DNOVACK]
        bic     r0, r0, #0xf
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_DNOVACK]
	
	//// Set the Divider Bypass bit.
        orr     r0, r0, #BF_DEVCONF_DNOVACK_DIVIDER_BYPASS
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_DNOVACK]
	
	//// Configure Divider select to desired division value.
	orr     r0, r0, #BF_DEVCONF_DNOVACK_DIV_1
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_DNOVACK]
	
	//// Reset the Divider Bypass bit.
        bic     r0, r0, #BF_DEVCONF_DNOVACK_DIVIDER_BYPASS
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_DNOVACK]	

	//// Configure Input Select to desired clock.
        orr     r0, r0, #BF_DEVCONF_INPUT_FPLL0_FOUT_DIV2
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_DNOVACK]
	

        //// USB Clock (USB Configuration Register):
        
        //// Configure Input Select to Crystal clock.
        ldr     r0, [r10,#BF_DEVCONF_REGS_CONFIG_USBCK]
        bic     r0, r0, #0xf
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_USBCK]
	
	//// Set the Divider Bypass bit.
        orr     r0, r0, #BF_DEVCONF_USBCK_DIVIDER_BYPASS
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_USBCK]
	
	//// Configure Divider select to desired division value.
	orr     r0, r0, #BF_DEVCONF_USBCK_DIV_3
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_USBCK]
	
	//// Reset the Divider Bypass bit.
        bic     r0, r0, #BF_DEVCONF_USBCK_DIVIDER_BYPASS
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_USBCK]	

	//// Configure Input Select to desired clock.
        orr     r0, r0, #BF_DEVCONF_INPUT_FPLL0_FVCO_DIV5
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_USBCK]	

        
        //// BBP Clock (BBPCK Configuration Register):
        
        //// Configure Input Select to Crystal clock.
        ldr     r0, [r10,#BF_DEVCONF_REGS_CONFIG_BBPCK]
        bic     r0, r0, #0xf
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_BBPCK]
	
	//// Set the Divider Bypass bit.
        orr     r0, r0, #BF_DEVCONF_BBPCK_DIVIDER_BYPASS
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_BBPCK]
	
	//// Configure Divider select to desired division value.
	orr     r0, r0, #BF_DEVCONF_BBPCK_DIV_6
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_BBPCK]
	
	//// Reset the Divider Bypass bit.
        bic     r0, r0, #BF_DEVCONF_BBPCK_DIVIDER_BYPASS
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_BBPCK]	

	//// Configure Input Select to desired clock.
        orr     r0, r0, #BF_DEVCONF_INPUT_FPLL1_FOUT_DIV2
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_BBPCK]
	

        //// MAC Clock (MACCK Configuration Register):
        
        //// Configure Input Select to Crystal clock.
        ldr     r0, [r10,#BF_DEVCONF_REGS_CONFIG_MACCK]
        bic     r0, r0, #0xf
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_MACCK]
	
	//// Set the Divider Bypass bit.
        orr     r0, r0, #BF_DEVCONF_MACCK_DIVIDER_BYPASS
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_MACCK]
	
	//// Configure Divider select to desired division value.
	orr     r0, r0, #BF_DEVCONF_MACCK_DIV_3
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_MACCK]
	
	//// Reset the Divider Bypass bit.
        bic	r0, r0, #BF_DEVCONF_MACCK_DIVIDER_BYPASS
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_MACCK]	

	//// Configure Input Select to desired clock.
        orr     r0, r0, #BF_DEVCONF_INPUT_FPLL0_FOUT_DIV2
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_MACCK]
	
        //// HSSD Clock (HSSD Configuration Register):
        
        //// Configure Input Select to Crystal clock.
        ldr     r0, [r10,#BF_DEVCONF_REGS_CONFIG_HSSDCK]
        bic     r0, r0, #0xf
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_HSSDCK]
        
	//// Set the Divider Bypass bit.
        orr     r0, r0, #BF_DEVCONF_HSSDCK_DIVIDER_BYPASS
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_HSSDCK]
	
	//// Configure Divider select to desired division value.
	orr     r0, r0, #BF_DEVCONF_HSSDCK_DIV_11
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_HSSDCK]
	
	//// Reset the Divider Bypass bit.
        bic     r0, r0, #BF_DEVCONF_HSSDCK_DIVIDER_BYPASS
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_HSSDCK]	

	//// Configure Input Select to desired clock.
        orr     r0, r0, #BF_DEVCONF_INPUT_FPLL1_FOUT_DIV2
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_HSSDCK] 
	
       //// Reference Clock (REFCK Configuration Register):
	
	//// Configure Divider select to desired division value.
	ldr     r0, =(BF_DEVCONF_REFCK_DIVIDER_BYPASS | BF_DEVCONF_REFCK_DIV_3)
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_REFCK]
	ldr     r0, =BF_DEVCONF_REFCK_DIV_3
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_REFCK]
				
        //// Stun Low Power Clock Clock (STLPCK Configuration Register):
	
	//// Configure Divider select to desired division value.
	ldr     r0, [r10,#BF_DEVCONF_REGS_CONFIG_STLPCK]
	orr     r0, r0, #BF_DEVCONF_STLPCK_DIV_2
	str     r0, [r10,#BF_DEVCONF_REGS_CONFIG_STLPCK]			
        



        //// Switch to Normal mode
        //// Bringing blocks out of reset:
	ldr	r1, [r10,#BF_DEVCONF_REGS_CONFIG_BOOT_CONTROL]
        orr	r1, r1, #BF_DEVCONF_RELEASE_IF_RESET
        str	r1, [r10,#BF_DEVCONF_REGS_CONFIG_BOOT_CONTROL]
        
        //// Wait for reset to complete
0:	ldr	r1, [r10,#BF_DEVCONF_REGS_CONFIG_BOOT_CONTROL]
	tst	r1, #BF_DEVCONF_OUT_OF_RESET
	beq	0b
        

        
        //// Restore registers and return
        //drop    r10
pll_init_return:      
        ldmfd   sp!, {r10, pc}


pll_lock_fail:
        b       pll_lock_fail
                


// *****************************************************************************
//
//      pll_setup
//
//      Sets registers in a specified PLL
//      
//                
//      Entry:
//      r0: Value for PLL control register 3
//      r1: Value for PLL control register 2
//      r2: Value for PLL control registers 4
//      r3: Value for PLL control registers 5
//      r4: Value for PLL control registers 6
//      r5: Value for PLL control registers 7
//      r11: Value for PLL control registers 8
//      r10: Base of DEVCONF
//      sp: Stack
//
//      Exit:
//       r3 corrupted
//
// *****************************************************************************
       
        
pll_setup:

        stmfd   sp!, {lr}
        
        //// Base of devconf registers is passed in in R10
        mov     r10, #BF_DEVCONF

        //// 
        //// Setting PLL Control Register 3
	str	r0, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_WRITE_DATA]	//// CONFIG_PLL_WRITE_DATA
        mov     r7, #BF_DEVCONF_SEL_REG_3                      		 //// CONFIG_PLL_REG_SEL
        bl      pll_write_reg

        //// 
        //// Setting PLL Control Register 2
        str     r1, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_WRITE_DATA]	//// CONFIG_PLL_WRITE_DATA
        mov     r7, #BF_DEVCONF_SEL_REG_2                       	//// CONFIG_PLL_REG_SEL
        bl      pll_write_reg        

        //// 
        //// Setting PLL Control Register 4
	str	r2, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_WRITE_DATA]	//// CONFIG_PLL_WRITE_DATA
      	mov	r7, #BF_DEVCONF_SEL_REG_4				//// CONFIG_PLL_REG_SEL
        bl      pll_write_reg        
        
        //// 
        //// Setting PLL Control Register 5
	str	r3, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_WRITE_DATA]	//// CONFIG_PLL_WRITE_DATA
        mov     r7, #BF_DEVCONF_SEL_REG_5                    		 //// CONFIG_PLL_REG_SEL
        bl      pll_write_reg

        //// 
        //// Setting PLL Control Register 6
        str     r4, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_WRITE_DATA]	//// CONFIG_PLL_WRITE_DATA
        mov     r7, #BF_DEVCONF_SEL_REG_6                       	//// CONFIG_PLL_REG_SEL
        bl      pll_write_reg        

        //// 
        //// Setting PLL Control Register 7
	str	r5, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_WRITE_DATA]	//// CONFIG_PLL_WRITE_DATA
      	mov	r7, #BF_DEVCONF_SEL_REG_7				//// CONFIG_PLL_REG_SEL
        bl      pll_write_reg 
        
        //// Setting PLL Control Register 8
	str	r11, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_WRITE_DATA]	//// CONFIG_PLL_WRITE_DATA
      	mov	r7, #BF_DEVCONF_SEL_REG_8				//// CONFIG_PLL_REG_SEL
        bl      pll_write_reg     
        
        
        //// Setting PLL Control Register 0
	str	r8, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_WRITE_DATA]	//// CONFIG_PLL_WRITE_DATA
      	mov	r7, #BF_DEVCONF_SEL_REG_0				//// CONFIG_PLL_REG_SEL
        bl      pll_write_reg             

        //// Setting PLL Control Register 0
	str	r9, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_WRITE_DATA]	//// CONFIG_PLL_WRITE_DATA
      	mov	r7, #BF_DEVCONF_SEL_REG_0				//// CONFIG_PLL_REG_SEL
        bl      pll_write_reg                            
               
       
        //drop    r10
        
        //// Return
	ldmfd   sp!, {pc}



// *****************************************************************************
//
//      pll_write_reg
//
//      Writes a specific PLL configuration register
//      Assumes that the relevant PLL has already been selected, and that
//      the data has been written into the CONFIG_PLL_WRITE_DATA register already
//      
//                
//      Entry:
//      r7:  Register to write to
//      r10: Base of DEVCONF
//
//      Exit:
//       r7 is corrupted.
//       Does NOT corrupt r0-r5
//
// *****************************************************************************

pll_write_reg:

        stmfd   sp!, {lr}
        
         //// Base of devconf registers is passed in in R10
        mov     r10, #BF_DEVCONF
        
        //// Select specifed register
	str	r7, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_REG_SEL]		

        bl      delay
        
        //// Actually do the write

        mov	r7, #BF_DEVCONF_PLL_CONTROL_HIW_RN              // Take HIW_RN high
	str	r7, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]

        bl      delay
        
        orr     r7, r7, #BF_DEVCONF_PLL_CONTROL_HIREQ           // Take HIREQ high too
	str	r7, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]

        bl      delay
        
        bic     r7, r7, #BF_DEVCONF_PLL_CONTROL_HIREQ           // Take HIREQ low
	str	r7, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL]

        bl      delay
        
      	bic	r7, r7, #BF_DEVCONF_PLL_CONTROL_HIW_RN          // Take HIW_RN back low
	str	r7, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_CONTROL] 

        bl      delay

        mov     r7, #0
        str     r7, [r10,#BF_DEVCONF_REGS_CONFIG_PLL_REG_SEL]          // Clea the register select
                
        //drop    r10
        
        //// Return
        ldmfd   sp!, {pc}




delay:
        //// Wait for 1ms
        // Save r0-r3 because the routines that called us may assume they aren't corrupted.
        stmfd   sp!, {r0-r3, lr}                                
        mov     r0, #1000                       // delay in us
        bl      timer_us_wait                  
        ldmfd   sp!, {r0-r3, pc}                // Restore registers and return
        
       


/******************************************************************************
;
;      timer_us_wait:
;
;      Entry:
;      r0: the number of microseconds to delay
;
;      Exit:
;
; ****************************************************************************/

timer_us_wait:
	stmfd   sp!, {r4-r6, lr}                        // In order to be helpful to assembly 
                                                        // calls don't trash r0-r3
	mov     r0, r0, lsl #3
        mov     r4, #CHM_MAP_PERIPH
        ldr     r5, [r4, #CHARM_TIMER1_VALUE] 
1:      ldr     r6, [r4, #CHARM_TIMER1_VALUE] 
        sub     r6, r6, r5                              // calculate time delta
        cmp     r6, r0                                  // has timer expired?
        blt     1b                                      // no - loop back 
        ldmfd   sp!, {r4-r6, pc}                        // yes - return              


/*
// *****************************************************************************
//       
//      _sdram_init:
//       
//       Configures and sizes SDR connected to Charm SDR controller
//
//       Configuration options included in config.s
//               SDR_INTERLEAVE_ENABLE - if defined enables interleaving
//               SDR_CAS_LATENCY       - allows overriding of cas latency
//                                       may be set to one of:
//                                               CHARM_SDRAM_MODE_CAS1_5   
//                                               CHARM_SDRAM_MODE_CAS2     
//                                               CHARM_SDRAM_MODE_CAS2_5   
//                                               CHARM_SDRAM_MODE_CAS3     
//
//      Entry:
//
//      Exit:
//      r0: contains SDR size if SDRAM is working, 0 otherwise 
//
// *****************************************************************************
*/


sdram_init:

        stmfd   sp!, {r1-r12, lr}
        // Initial SDRAM configuration
        mov     r12, #CHM_MAP_PERIPH

        // Set the read cycle latency    
        ldr     r1, [r12, #CHARM_SDRAM_LIMIT2]
        bic     r1, r1, #(CHARM_SDRAM_LIMIT2_RD_CAS_MASK | CHARM_SDRAM_LIMIT2_RD_TO_WR_MASK)
        orr     r1, r1, #0x54
        str     r1, [r12, #CHARM_SDRAM_LIMIT2]


        // Set the sdram clock delay
	mov     r10,#BF_DEVCONF
        ldr     r1, [r10, #BF_DEVCONF_REGS_CONFIG_SDRAM_CLK_DEL]
        bic     r1, r1, #0x1f
        orr     r1, r1, #BF_DEVCONF_SDRAM_DEL1_100
        str     r1, [r10, #BF_DEVCONF_REGS_CONFIG_SDRAM_CLK_DEL]


        ldr     r1, [r12, #CHARM_SDRAM_CTL]             // read control reg
#ifdef  SDR_INTERLEAVE_ENABLE
        orr     r1, r1, #CHARM_SDRAM_CTL_BANKILV        // enable interleaving
#endif  /* SDR_INTERLEAVE_ENABLE */
        bic     r1, r1, #CHARM_SDRAM_CTL_PAD_DIS        // enable the pads
        str     r1, [r12, #CHARM_SDRAM_CTL]             // write back
        // Before the controller can be enabled, wait 200us for the clock to settle
        mov     r0, #200
        bl      timer_us_wait
        orr     r1, r1, #CHARM_SDRAM_CTL_DEVICE_SDR     // enable the controller
        str     r1, [r12, #CHARM_SDRAM_CTL]

#ifdef  SDR_CAS_LATENCY
        ldr     r1, [r12, #CHARM_SDRAM_MODE]            // read mode reg
        bic     r1, r1, #CHARM_SDRAM_MODE_CASLATE
        orr     r1, r1, #SDR_CAS_LATENCY                // set the cas latency
        str     r1, [r12, #CHARM_SDRAM_MODE]            // write back
#endif  /* SDR_CAS_LATENCY */

        mov     r2, #CHM_MAP_SDRAMU     // Start of SDRAM
        ldr     r3, =MEMSIZE_WORD_A     // Test pattern

        // Instead of forcing the controller into 32/16-bit mode 
        // use a trial-and-error approach to check the SDRAM width. 
        // First try the default (32-bit) width
        str     r3, [r2]                // Write word to SDRAM
        str     r2, [r2, #4]            // Clear floating bus
        ldr     r4, [r2]                // Read it back
        cmp     r3, r4                  // Is it the same?
        bne     check_16bit             // Jump if not
        mvn     r3, r3                  // Repeat with inverted pattern
        str     r3, [r2]                // Write word to SDRAM
        ldr     r4, [r2]                // Read it back
        cmp     r3, r4                  // Is it the same?
        beq     cas_width               // Yes, 32-bit, check cas width

check_16bit:
        ldr     r1, [r12, #CHARM_SDRAM_LIMIT2]          // read limit2 reg
        bic     r1, r1, #CHARM_SDRAM_LIMIT2_WIDTH_MASK  // mask off the width
        orr     r1, r1, #CHARM_SDRAM_LIMIT2_WIDTH_16     // try 16-bit width
        str     r1, [r12, #CHARM_SDRAM_LIMIT2]          // write back
        str     r3, [r2]                // Write word to SDRAM
        str     r2, [r2, #4]            // Clear floating bus
        ldr     r4, [r2]                // Read it back
        cmp     r3, r4                  // Is it the same?
        bne     err_ret                 // Jump if not
        mvn     r3, r3                  // Repeat with inverted pattern
        str     r3, [r2]                // Write word to SDRAM
        ldr     r4, [r2]                // Read it back
        cmp     r3, r4                  // Is it the same?
        bne     err_ret                 // Yes, 16-bit, check cas width



cas_width:
        ldr     r6, [r12, #CHARM_SDRAM_CTL]             // read control reg
        bic     r6, r6, #CHARM_SDRAM_CTL_CAS_MASK       // Clear configuration bits
        // Try CAS-width A0-11
        orr     r3, r6, #CHARM_SDRAM_CTL_CAS_A0A11
        str     r3, [r12, #CHARM_SDRAM_CTL]             // write back
        bl      trycaswidth
        beq     size_dram
        // Try CAS-width A0-9
        orr     r3, r6, #CHARM_SDRAM_CTL_CAS_A0A9
        str     r3, [r12, #CHARM_SDRAM_CTL]             // write back
        bl      trycaswidth
        beq     size_dram
        // Try CAS-width A0-8
        orr     r3, r6, #CHARM_SDRAM_CTL_CAS_A0A8
        str     r3, [r12, #CHARM_SDRAM_CTL]             // write back
        bl      trycaswidth
        beq     size_dram
        // Try CAS-width A0-7
        orr     r3, r6, #CHARM_SDRAM_CTL_CAS_A0A7
        str     r3, [r12, #CHARM_SDRAM_CTL]             // write back
        bl      trycaswidth
        bne     err_ret

size_dram:
        ldr     r9, =MEMSIZE_WORD_A             // magic pattern
        ldr     r7, =MEMSIZE_WORD_B             // magic pattern
        ldr     r8, =MEMSIZE_WORD_C             // magic pattern
        str     r9, [r2]                        // write to first location
        mov     r0, #MEMSIZE_INCREMENT          // initial offset
1:      add     r1, r2, r0                      // test address
        stmia   r1, {r7, r8}                    // write to test area
        ldmia   r1, {r10, r11}                  // read back
        cmp     r7, r10                         // check result
        cmpeq   r8, r11
        ldreq   r1, [r2]                        // check overwrite of first location
        cmpeq   r1, r9
        bne     ok_ret                          // exit with SDRAM size in R0
        add     r0, r0, #MEMSIZE_INCREMENT      // try next block
        cmp     r0, #MEMSIZE_MAXIMUM            // reached maximum possible size?
        blt     1b                              // loop if not

err_ret:
        mov     r0, #0                          // Flag the error
ok_ret:
        ldmfd   sp!, {r1-r12, pc}               // Return to caller



/*
// =============================================================================
//
//       trycaswidth
//
//       Try out a configuration of the SDRAM and determine whether the selected
//       CAS width is correct
//
//       This works by writing a pattern to the first location of SDRAM, and
//       to wrap points for the various CAS widths (512 for A0-6, 1024 for
//       A0-7, 2048 for A0-8). If writing to these overwrites the first location
//       then the selected CAS width is invalid.
//
//       entry:  R2  = SDRAM base address
//               R3  = Configuration value to write to CF_PERIPH_CS_CONTROL
//               R12 = Pointer CF_NP_PERIPH_IPB
//
//       exit:   EQ  set if CAS width is valid
//               R0-R6 preserved
//               R7-R9 corrupted
//               R12  preserved
//
// =============================================================================
*/


trycaswidth:
        ldr     r7, =CASWIDTH_WORD              // magic pattern
        str     r7, [r2]                        // write to first location
        add     r8, r7, #512                    // new pattern
        str     r8, [r2, #512]                  // write to wrap location
        add     r8, r7, #1024                   // new pattern
        str     r8, [r2, #1024]                 // write to wrap location
        add     r8, r7, #2048                   // new pattern
        str     r8, [r2, #2048]                 // write to wrap location
        ldr     r9, [r2, #2048]                 // forcing read, this ensures all the 
                                                // ...writes have completed before...
        ldr     r9, [r2]                        // check overwrite of first location
        cmp     r7, r9
        mov     pc, lr                          // exit with EQ bit set if valid

/*
// *****************************************************************************
//
//      _sdram_term:
//
//       Disable the pads and controller
//
//       It is important that if the clock speed is dropped below legal SDR
//       rates that the pads are disabled to stop driving the RAMs at illegal
//       speeds. 
//       However disabling the controller and pads must be preceeded by all
//       possible sources to the SDR controller being stopped. These include
//       all the DMAs and cache or instruction executions
//
//      Entry:
//
//      Exit:
//
// *****************************************************************************
*/


sdram_term:
        stmfd   sp!, {r0-r1, lr}
        mov     r0, #CHM_MAP_PERIPH
        ldr     r1, [r0, #CHARM_SDRAM_CTL]              // read control reg
        bic     r1, r1, #CHARM_SDRAM_CTL_DEVICE_MASK    // disable controller
        orr     r1, r1, #CHARM_SDRAM_CTL_PAD_DIS        // disable the pads
        str     r1, [r0, #CHARM_SDRAM_CTL]              // write back
        ldmfd   sp!, {r0-r1, pc}                        // Return to caller

     
