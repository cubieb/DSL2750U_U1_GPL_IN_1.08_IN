/*
 *
 * BRIEF MODULE DESCRIPTION
 *   OMAP730 hardware map
 *
 * Copyright (C) 2004 MPC-Data Limited. (http://www.mpc-data.co.uk)
 * Author: MPC-Data Limited
 *	   Dave Peverley
 *
 *  This program is free software; you can redistribute	 it and/or modify it
 *  under  the terms of	 the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the	License, or (at your
 *  option) any later version.
 *
 *  THIS  SOFTWARE  IS PROVIDED	  ``AS	IS'' AND   ANY	EXPRESS OR IMPLIED
 *  WARRANTIES,	  INCLUDING, BUT NOT  LIMITED  TO, THE IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 *  NO	EVENT  SHALL   THE AUTHOR  BE	 LIABLE FOR ANY	  DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 *  NOT LIMITED	  TO, PROCUREMENT OF  SUBSTITUTE GOODS	OR SERVICES; LOSS OF
 *  USE, DATA,	OR PROFITS; OR	BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 *  ANY THEORY OF LIABILITY, WHETHER IN	 CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 *  THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  You should have received a copy of the  GNU General Public License along
 *  with this program; if not, write  to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __INCLUDED_SOLOS_W_H
#define __INCLUDED_SOLOS_W_H

#include "sizes.h"

/***************************************************************************
 * OMAP730 Configuration Registers
 **************************************************************************/

#define PERSEUS2_MPU_DEV_ID               ((unsigned int)(0xFFFE1000))
#define PERSEUS2_GSM_DEV_ID0              ((unsigned int)(0xFFFE1000))
#define PERSEUS2_GDM_DEV_ID1              ((unsigned int)(0xFFFE1002))
#define DSP_CONF                          ((unsigned int)(0xFFFE1004))
#define PERSEUS2_MPU_DIE_ID0              ((unsigned int)(0xFFFE1008))
#define GSM_ASIC_CONF                     ((unsigned int)(0xFFFE1008))
#define PERSEUS2_MPU_DIE_ID1              ((unsigned int)(0xFFFE100C))
#define PERSEUS2_MODE1                    ((unsigned int)(0xFFFE1010))
#define PERSEUS2_GSM_DIE_ID0              ((unsigned int)(0xFFFE1010))
#define PERSEUS2_GSM_DIE_ID1              ((unsigned int)(0xFFFE1012))
#define PERSEUS2_MODE2                    ((unsigned int)(0xFFFE1014))
#define PERSEUS2_GSM_DIE_ID2              ((unsigned int)(0xFFFE1014))
#define PERSEUS2_GSM_DIE_ID3              ((unsigned int)(0xFFFE1016))
#define PERSEUS2_ANALOG_CELLS_CONF        ((unsigned int)(0xFFFE1018))
#define SPECCTL                           ((unsigned int)(0xFFFE101C))
#define SPARE1                            ((unsigned int)(0xFFFE1020))
#define SPARE2                            ((unsigned int)(0xFFFE1024))
#define GSM_PBG_IRQ                       ((unsigned int)(0xFFFE1028))
#define DMA_REQ_CONF                      ((unsigned int)(0xFFFE1030))
#define PE_CONF_NO_DUAL                   ((unsigned int)(0xFFFE1060))
#define PERSEUS2_IO_CONF0                 ((unsigned int)(0xFFFE1070))
#define PERSEUS2_IO_CONF1                 ((unsigned int)(0xFFFE1074))
#define PERSEUS2_IO_CONF2                 ((unsigned int)(0xFFFE1078))
#define PERSEUS2_IO_CONF3                 ((unsigned int)(0xFFFE107C))
#define PERSEUS2_IO_CONF4                 ((unsigned int)(0xFFFE1080))
#define PERSEUS2_IO_CONF5                 ((unsigned int)(0xFFFE1084))
#define PERSEUS2_IO_CONF6                 ((unsigned int)(0xFFFE1088))
#define PERSEUS2_IO_CONF7                 ((unsigned int)(0xFFFE108C))
#define PERSEUS2_IO_CONF8                 ((unsigned int)(0xFFFE1090))
#define PERSEUS2_IO_CONF9                 ((unsigned int)(0xFFFE1094))
#define PERSEUS2_IO_CONF10                ((unsigned int)(0xFFFE1098))
#define PERSEUS2_IO_CONF11                ((unsigned int)(0xFFFE109C))
#define PERSEUS2_IO_CONF12                ((unsigned int)(0xFFFE10A0))
#define PERSEUS2_IO_CONF13                ((unsigned int)(0xFFFE10A4))
#define PERSEUS_PCC_CONF_REG              ((unsigned int)(0xFFFE10B4))
#define BIST_STATUS_INTERNAL              ((unsigned int)(0xFFFE10B8))
#define BIST_CONTROL                      ((unsigned int)(0xFFFE10C0))
#define BOOT_ROM_REG                      ((unsigned int)(0xFFFE10C4))
#define PRODUCTION_ID_REG                 ((unsigned int)(0xFFFE10C8))
#define BIST_SECROM_SIGNATURE1_INTERNAL   ((unsigned int)(0xFFFE10D0))
#define BIST_SECROM_SIGNATURE2_INTERNAL   ((unsigned int)(0xFFFE10D4))
#define BIST_CONTROL_2                    ((unsigned int)(0xFFFE10D8))
#define DEBUG1                            ((unsigned int)(0xFFFE10E0))
#define DEBUG2                            ((unsigned int)(0xFFFE10E4))
#define DEBUG_DMA_IRQ                     ((unsigned int)(0xFFFE10E8))

/***************************************************************************
 * OMAP730 EMIFS Registers                                       (TRM 2.5.7)
 **************************************************************************/

#define TCMIF_BASE                 0xFFFECC00

#define EMIFS_LRUREG               (TCMIF_BASE + 0x04)
#define EMIFS_CONFIG               (TCMIF_BASE + 0x0C)
#define FLASH_CFG_0                (TCMIF_BASE + 0x10)
#define FLASH_CFG_1                (TCMIF_BASE + 0x14)
#define FLASH_CFG_2                (TCMIF_BASE + 0x18)
#define FLASH_CFG_3                (TCMIF_BASE + 0x1C)
#define FL_CFG_DYN_WAIT            (TCMIF_BASE + 0x40)
#define EMIFS_TIMEOUT1_REG         (TCMIF_BASE + 0x28)
#define EMIFS_TIMEOUT2_REG         (TCMIF_BASE + 0x2C)
#define EMIFS_TIMEOUT3_REG         (TCMIF_BASE + 0x30)
#define EMIFS_ABORT_ADDR           (TCMIF_BASE + 0x44)
#define EMIFS_ABORT_TYPE           (TCMIF_BASE + 0x48)
#define EMIFS_ABORT_TOUT           (TCMIF_BASE + 0x4C)
#define FLASH_ACFG_0_1             (TCMIF_BASE + 0x50)
#define FLASH_ACFG_1_1             (TCMIF_BASE + 0x54)
#define FLASH_ACFG_2_1             (TCMIF_BASE + 0x58)
#define FLASH_ACFG_3_1             (TCMIF_BASE + 0x5C)

/***************************************************************************
 * OMAP730 Interrupt handlers
 **************************************************************************/

#define OMAP_IH1_BASE		0xFFFECB00     /* MPU Level 1 IRQ handler */
#define OMAP_IH2_BASE           0xfffe0000

/***************************************************************************
 * OMAP730 Timers
 *
 * There are three general purpose OS timers in the 730 that can be
 * configured in autoreload or one-shot modes.
 **************************************************************************/

#define OMAP730_32kHz_TIMER_BASE  0xFFFB9000

/* 32k Timer Registers */
#define TIMER32k_CR               0x08
#define TIMER32k_TVR              0x00
#define TIMER32k_TCR              0x04

/* 32k Timer Control Register definition */
#define TIMER32k_TSS              (1<<0)
#define TIMER32k_TRB              (1<<1)
#define TIMER32k_INT              (1<<2)
#define TIMER32k_ARL              (1<<3)

/* MPU Timer base addresses  */
#define OMAP730_MPUTIMER_BASE	0xfffec500
#define OMAP730_MPUTIMER_OFF	0x00000100

#define OMAP730_TIMER1_BASE	0xFFFEC500
#define OMAP730_TIMER2_BASE	0xFFFEC600
#define OMAP730_TIMER3_BASE	0xFFFEC700

/* MPU Timer Register offsets */
#define CNTL_TIMER	           0x00   /* MPU_CNTL_TIMER */
#define LOAD_TIM	           0x04   /* MPU_LOAD_TIMER */
#define READ_TIM	           0x08   /* MPU_READ_TIMER */

/* MPU_CNTL_TIMER register bits */
#define MPUTIM_FREE               (1<<6)
#define MPUTIM_CLOCK_ENABLE       (1<<5)
#define MPUTIM_PTV_MASK           (0x7<<PTV_BIT)
#define MPUTIM_PTV_BIT            2
#define MPUTIM_AR                 (1<<1)
#define MPUTIM_ST                 (1<<0)

/***************************************************************************
 * OMAP730 GPIO
 *
 * The GPIO control is split over 6 register bases in the OMAP730 to allow
 * access to all the (6 x 32) GPIO pins!
 **************************************************************************/

#define OMAP730_GPIO_BASE_1        0xFFFBC000
#define OMAP730_GPIO_BASE_2        0xFFFBC800
#define OMAP730_GPIO_BASE_3        0xFFFBD000
#define OMAP730_GPIO_BASE_4        0xFFFBD800
#define OMAP730_GPIO_BASE_5        0xFFFBE000
#define OMAP730_GPIO_BASE_6        0xFFFBE800

#define GPIO_DATA_INPUT            0x00
#define GPIO_DATA_OUTPUT           0x04
#define GPIO_DIRECTION_CONTROL     0x08
#define GPIO_INTERRUPT_CONTROL     0x0C
#define GPIO_INTERRUPT_MASK        0x10
#define GPIO_INTERRUPT_STATUS      0x14

#define GPIO_DATA_INPUT_1            ((unsigned int)(OMAP730_GPIO_BASE_1 + GPIO_DATA_INPUT))
#define GPIO_DATA_OUTPUT_1           ((unsigned int)(OMAP730_GPIO_BASE_1 + GPIO_DATA_OUTPUT))
#define GPIO_DIRECTION_CONTROL_1     ((unsigned int)(OMAP730_GPIO_BASE_1 + GPIO_DIRECTION_CONTROL))
#define GPIO_INTERRUPT_CONTROL_1     ((unsigned int)(OMAP730_GPIO_BASE_1 + GPIO_INTERRUPT_CONTROL))
#define GPIO_INTERRUPT_MASK_1        ((unsigned int)(OMAP730_GPIO_BASE_1 + GPIO_INTERRUPT_MASK))
#define GPIO_INTERRUPT_STATUS_1      ((unsigned int)(OMAP730_GPIO_BASE_1 + GPIO_INTERRUPT_STATUS))

#define GPIO_DATA_INPUT_2            ((unsigned int)(OMAP730_GPIO_BASE_2 + GPIO_DATA_INPUT))
#define GPIO_DATA_OUTPUT_2           ((unsigned int)(OMAP730_GPIO_BASE_2 + GPIO_DATA_OUTPUT))
#define GPIO_DIRECTION_CONTROL_2     ((unsigned int)(OMAP730_GPIO_BASE_2 + GPIO_DIRECTION_CONTROL))
#define GPIO_INTERRUPT_CONTROL_2     ((unsigned int)(OMAP730_GPIO_BASE_2 + GPIO_INTERRUPT_CONTROL))
#define GPIO_INTERRUPT_MASK_2        ((unsigned int)(OMAP730_GPIO_BASE_2 + GPIO_INTERRUPT_MASK))
#define GPIO_INTERRUPT_STATUS_2      ((unsigned int)(OMAP730_GPIO_BASE_2 + GPIO_INTERRUPT_STATUS))

#define GPIO_DATA_INPUT_3            ((unsigned int)(OMAP730_GPIO_BASE_3 + GPIO_DATA_INPUT))
#define GPIO_DATA_OUTPUT_3           ((unsigned int)(OMAP730_GPIO_BASE_3 + GPIO_DATA_OUTPUT))
#define GPIO_DIRECTION_CONTROL_3     ((unsigned int)(OMAP730_GPIO_BASE_3 + GPIO_DIRECTION_CONTROL))
#define GPIO_INTERRUPT_CONTROL_3     ((unsigned int)(OMAP730_GPIO_BASE_3 + GPIO_INTERRUPT_CONTROL))
#define GPIO_INTERRUPT_MASK_3        ((unsigned int)(OMAP730_GPIO_BASE_3 + GPIO_INTERRUPT_MASK))
#define GPIO_INTERRUPT_STATUS_3      ((unsigned int)(OMAP730_GPIO_BASE_3 + GPIO_INTERRUPT_STATUS))

#define GPIO_DATA_INPUT_4            ((unsigned int)(OMAP730_GPIO_BASE_4 + GPIO_DATA_INPUT))
#define GPIO_DATA_OUTPUT_4           ((unsigned int)(OMAP730_GPIO_BASE_4 + GPIO_DATA_OUTPUT))
#define GPIO_DIRECTION_CONTROL_4     ((unsigned int)(OMAP730_GPIO_BASE_4 + GPIO_DIRECTION_CONTROL))
#define GPIO_INTERRUPT_CONTROL_4     ((unsigned int)(OMAP730_GPIO_BASE_4 + GPIO_INTERRUPT_CONTROL))
#define GPIO_INTERRUPT_MASK_4        ((unsigned int)(OMAP730_GPIO_BASE_4 + GPIO_INTERRUPT_MASK))
#define GPIO_INTERRUPT_STATUS_4      ((unsigned int)(OMAP730_GPIO_BASE_4 + GPIO_INTERRUPT_STATUS))

#define GPIO_DATA_INPUT_5            ((unsigned int)(OMAP730_GPIO_BASE_5 + GPIO_DATA_INPUT))
#define GPIO_DATA_OUTPUT_5           ((unsigned int)(OMAP730_GPIO_BASE_5 + GPIO_DATA_OUTPUT))
#define GPIO_DIRECTION_CONTROL_5     ((unsigned int)(OMAP730_GPIO_BASE_5 + GPIO_DIRECTION_CONTROL))
#define GPIO_INTERRUPT_CONTROL_5     ((unsigned int)(OMAP730_GPIO_BASE_5 + GPIO_INTERRUPT_CONTROL))
#define GPIO_INTERRUPT_MASK_5        ((unsigned int)(OMAP730_GPIO_BASE_5 + GPIO_INTERRUPT_MASK))
#define GPIO_INTERRUPT_STATUS_5      ((unsigned int)(OMAP730_GPIO_BASE_5 + GPIO_INTERRUPT_STATUS))

#define GPIO_DATA_INPUT_6            ((unsigned int)(OMAP730_GPIO_BASE_6 + GPIO_DATA_INPUT))
#define GPIO_DATA_OUTPUT_6           ((unsigned int)(OMAP730_GPIO_BASE_6 + GPIO_DATA_OUTPUT))
#define GPIO_DIRECTION_CONTROL_6     ((unsigned int)(OMAP730_GPIO_BASE_6 + GPIO_DIRECTION_CONTROL))
#define GPIO_INTERRUPT_CONTROL_6     ((unsigned int)(OMAP730_GPIO_BASE_6 + GPIO_INTERRUPT_CONTROL))
#define GPIO_INTERRUPT_MASK_6        ((unsigned int)(OMAP730_GPIO_BASE_6 + GPIO_INTERRUPT_MASK))
#define GPIO_INTERRUPT_STATUS_6      ((unsigned int)(OMAP730_GPIO_BASE_6 + GPIO_INTERRUPT_STATUS))

/***************************************************************************
 * OMAP730 Watchdog timers
 **************************************************************************/

#define WDTIM_BASE                 0xFFFEC800
#define WDTIM_CONTROL              (WDTIM_BASE + 0x00)    /* MPU_CNTL_TIMER */
#define WDTIM_LOAD                 (WDTIM_BASE + 0x04)    /* MPU_LOAD_TIMER */
#define WDTIM_READ                 (WDTIM_BASE + 0x04)    /* MPU_READ_TIMER */
#define WDTIM_MODE                 (WDTIM_BASE + 0x08)    /* MPU_TIMER_MODE */

/***************************************************************************
 * OMAP730 Interrupt Registers
 **************************************************************************/

/* Interrupt Register offsets */

#define IRQ_ITR                               0x00
#define IRQ_MIR                               0x04
#define IRQ_SIR_IRQ                           0x10
#define IRQ_SIR_FIQ                           0x14
#define IRQ_CONTROL_REG                       0x18
#define IRQ_ILR0                              0x1C  /* ILRx == ILR0 + (0x4 * x) */
#define IRQ_SIR                               0x9C  /* a.k.a.IRQ_ISR */
#define IRQ_GMIR                              0xA0

#define REG_IHL1_MIR  (OMAP_IH1_BASE + IRQ_MIR)
#define REG_IHL2_MIR  (OMAP_IH2_BASE + IRQ_MIR)

/***************************************************************************
 * OMAP730 Intersystem Communication Register                      (TRM 4.5)
 **************************************************************************/

#define ICR_BASE                   0xFFFBB800

#define M_ICR                      (ICR_BASE + 0x00)
#define G_ICR                      (ICR_BASE + 0x02)
#define M_CTL                      (ICR_BASE + 0x04)
#define G_CTL                      (ICR_BASE + 0x06)
#define PM_BA                      (ICR_BASE + 0x0A)
#define DM_BA                      (ICR_BASE + 0x0C)
#define RM_BA                      (ICR_BASE + 0x0E)
#define SSPI_TAS                   (ICR_BASE + 0x12)

/* Star for solos_w */

#define CHMCONF_BASE		(0xFF000000)

/***************************************************************************
 * Vectored Interrupt Controller                                           *
 **************************************************************************/

#ifndef CHF_VIC_H
#define CHF_VIC_H

/* +-----------------------------------------------------------------------+ */
/* |                                                                       | */
/* | CHARM VECTORED INTERRUPT CONTROLLER BLOCK HARDWARE DEFINITIONS        | */
/* |                                                                       | */
/* +-----------------------------------------------------------------------+ */

#define CHARM_VIC                  (0x400) /* Vectored Interrupt Controller  */

/*** Registers in the Console UART Block (offsets from CHM_MAP_PERIPH) *******/

#define CHARM_VIC                  (0x400) /* Vectored Interrupt Controller  */

#define CHARM_VIC_IRQSTAT          (0x400) /* Asserted IRQ status            */
#define CHARM_VIC_FIQSTAT          (0x404) /* Asserted FIQ status            */
#define CHARM_VIC_RAWSTAT          (0x408) /* Raw interrupt status           */
#define CHARM_VIC_INTTYPE          (0x40C) /* IRQ/FIQ interrupt type         */
#define CHARM_VIC_INTENB           (0x410) /* Interrupt source enable        */
#define CHARM_VIC_INTDIS           (0x414) /* Interrupt source disable       */
#define CHARM_VIC_PRISEL           (0x418) /* Internal/External priority sel */
#define CHARM_VIC_PRIUPR           (0x41C) /* Upper priority level select    */
#define CHARM_VIC_ACTSRC           (0x420) /* Active Source                  */
#define CHARM_VIC_ACTIVE           (0x424) /* Active IRQ Vector - Writing to */
                                           /* reg will advance Round Robins  */
#define CHARM_VIC_VECSEL           (0x428) /* Interrupt source vector enb    */
#define CHARM_VIC_VASSIGN0         (0x42C) /* Source to VecReg assignment 0  */
#define CHARM_VIC_VASSIGN1         (0x430) /* Source to VecReg assignment 1  */
#define CHARM_VIC_VASSIGN2         (0x434) /* Source to VecReg assignment 2  */
#define CHARM_VIC_VASSIGN3         (0x438) /* Source to VecReg assignment 3  */
#define CHARM_VIC_SOFTINT          (0x43C) /* Soft int source assert         */
#define CHARM_VIC_RR               (0x440) /* Round robin info               */
#define CHARM_VIC_ACTSRCADV        (0x444) /* Active Source - Reading from   */
                                           /* reg will advance Round Robins  */

/*** Vector Registers 0-7 (all VECREG configuartions) ************************/

#define CHARM_VIC_VECREG0          (0x480) /* Vector address register 0      */
#define CHARM_VIC_VECREG1          (0x484) /* Vector address register 1      */
#define CHARM_VIC_VECREG2          (0x488) /* Vector address register 2      */
#define CHARM_VIC_VECREG3          (0x48C) /* Vector address register 3      */

/*** Vector Registers 4-7 (8/16 VECREG configs only) *************************/

#define CHARM_VIC_VECREG4          (0x490) /* Vector address register 4      */
#define CHARM_VIC_VECREG5          (0x494) /* Vector address register 5      */
#define CHARM_VIC_VECREG6          (0x498) /* Vector address register 6      */
#define CHARM_VIC_VECREG7          (0x49C) /* Vector address register 7      */

/*** Vector Registers 8-15 (16 VECREG config only) ***************************/

#define CHARM_VIC_VECREG8          (0x4A0) /* Vector address register 8      */
#define CHARM_VIC_VECREG9          (0x4A4) /* Vector address register 9      */
#define CHARM_VIC_VECREG10         (0x4A8) /* Vector address register 10     */
#define CHARM_VIC_VECREG11         (0x4AC) /* Vector address register 11     */
#define CHARM_VIC_VECREG12         (0x4B0) /* Vector address register 12     */
#define CHARM_VIC_VECREG13         (0x4B4) /* Vector address register 13     */
#define CHARM_VIC_VECREG14         (0x4B8) /* Vector address register 14     */
#define CHARM_VIC_VECREG15         (0x4BC) /* Vector address register 15     */

/*** Bit definitions *********************************************************/

#define CHARM_VIC_ACTSRC_MASK      (0x01F) /* Active interrupt source 0-31   */
#define CHARM_VIC_NOACTSRC         (0x020) /* No active interrupt source     */

#define CHARM_VIC_RR_ACTPRI        (0x060) /* Active int priority level mask */
#define CHARM_VIC_RR_APLOW         (0x020) /* Active int priority is low     */
#define CHARM_VIC_RR_APMED         (0x040) /* Active int priority is medium  */
#define CHARM_VIC_RR_APHIGH        (0x060) /* Active int priority is high    */
#define CHARM_VIC_RR_ACTIRQ        (0x080) /* Active interrupt request IRQ   */
#define CHARM_VIC_RR_ACTFIQ        (0x100) /* Active interrupt request FIQ   */
#define CHARM_VIC_RR_DISIRQ        (0x200) /* Disable IRQ generation         */
#define CHARM_VIC_RR_DISFIQ        (0x400) /* Disable FIQ generation         */
#define CHARM_VIC_RR_LPNEXT        (0x0000F800) /* Next LP source in LP RR   */
#define CHARM_VIC_RR_LPNOACT       (0x00010000) /* No Active Low Pri src's   */
#define CHARM_VIC_RR_MPNEXT        (0x003E0000) /* Next MP source in MP RR   */
#define CHARM_VIC_RR_MPNOACT       (0x00400000) /* No Active Med Pri src's   */
#define CHARM_VIC_RR_HPNEXT        (0x0F800000) /* Next HP source in HP RR   */
#define CHARM_VIC_RR_HPNOACT       (0x10000000) /* No Active High Pri src's  */
#define CHARM_VIC_RR_AUTOADV       (0x20000000) /* Auto Advance RR's         */

#define CHARM_VIC_VECADDR_MASK     (0xFFFFFFFC) /* Word aligned address mask */

/*** Interrupt source number and bit masks ***********************************/

#define CHARM_VIC_NUM_SELF         (24)    /* Self IRQ                       */
#define CHARM_VIC_NUM_WDOG         (25)    /* Watchdog expiry                */
#define CHARM_VIC_NUM_TIMER2       (26)    /* Timer 2 expiry                 */
#define CHARM_VIC_NUM_TIMER3       (27)    /* Timer 3 expiry                 */
#define CHARM_VIC_NUM_TIMER4       (28)    /* Timer 4 expiry                 */
#define CHARM_VIC_NUM_UART         (29)    /* UART TX empty / RX full        */
#define CHARM_VIC_NUM_COMMRX       (30)    /* ARM ICE character received     */
#define CHARM_VIC_NUM_COMMTX       (31)    /* ARM ICE character sent         */

#define CHARM_VIC_NUM_MAX          (31)    /* Max IRQ value                  */

#define CHARM_VIC_BIT_SELF         (1<<CHARM_VIC_NUM_SELF)
#define CHARM_VIC_BIT_WDOG         (1<<CHARM_VIC_NUM_WDOG)
#define CHARM_VIC_BIT_TIMER2       (1<<CHARM_VIC_NUM_TIMER2)
#define CHARM_VIC_BIT_TIMER3       (1<<CHARM_VIC_NUM_TIMER3)
#define CHARM_VIC_BIT_TIMER4       (1<<CHARM_VIC_NUM_TIMER4)
#define CHARM_VIC_BIT_UART         (1<<CHARM_VIC_NUM_UART)
#define CHARM_VIC_BIT_COMMRX       (1<<CHARM_VIC_NUM_COMMRX)
#define CHARM_VIC_BIT_COMMTX       (1<<CHARM_VIC_NUM_COMMTX)

/*****************************************************************************/
/* Compatibility symbols - for code shared with Helium family devices        */
/*****************************************************************************/
#define HF_PP_CS_NEXTIRQ_MAX       (CHARM_VIC_NUM_COMMTX) /* Max value returned */


/*** Compatibility symbols - for code shared with Helium family devices ******/

#endif




/****************************************************************************/
/*                                                                          */
/* Helium Family NP Hardware Definitions                                    */
/*                                                                          */
/****************************************************************************/

#ifndef HF_NP_H
#define HF_NP_H


/* Network processor memory layout */

#define HF_NP_BOOTROM (0x00000000) /* Start of boot ROM (at reset)          */
#define HF_NP_SRAM    (0x00000000) /* Start of internal SRAM (after reset)  */
#define HF_NP_SRAM2   (0x00002000) /* Second mapping of internal SRAM       */
#define HF_NP_SDRAM   (0x10000000) /* Start of SDRAM (uncached)             */
#define HF_NP_PPAPB   (0x20000000) /* Start of PP ARM peripheral bus space  */
#define HF_NP_NPAPB   (0xff000000) /* Start of NP ARM peripheral bus space  */
#define HF_NP_LOCK    (0x40000000) /* Start of NP lock/sleep register space */
#define HF_NP_PDMARD  (0xa0000000) /* Start of PDMA read port space         */
#define HF_NP_PDMAWR  (0xb0000000) /* Start of PDMA write port space        */
#define HF_NP_FIFO    (0xc0000000) /* Start of FIFO read/write port space   */
#define HF_NP_DPSRAM  (0xd0000000) /* Start of DPSRAM read/write port space */
#define HF_NP_FILTACT (0xe0000000) /* Start of Filter Activate port space   */

#define HF_NP_SRAMSIZE       8192  /* Size of NP's SRAM area                 */
#define HF_NP_SRAM_ADDRBITS    13  /* No. of bits needed for an SRAM address */

#define HF_NP_BOOTROM_UARTBOOT  (0x00000004) /* Start of UART booting code in boot ROM */
#define HF_NP_BOOTROM_FLASHBOOT (0x000000dc) /* Start of flash booting code in boot ROM */

/* These are offsets from NP_NPAPB */


#define HF_NP_CS            (0x0000) /* NP control/status registers       */

#define HF_NP_CS_CONTROL    (0x0008) /* NP control register               */

#define HF_NP_CS_INTSTATUS  (0x0010) /* NP interrupt status register      */
#define HF_NP_CS_NEXTIRQ    (0x0014) /* NP next IRQ register              */

#define HF_NP_CS_BELLTOPP   (0x0020) /* Doorbell/interrupt to PP register */
#define HF_NP_CS_BELLFROMPP (0x0024) /* Doorbell from PP status register  */

#define HF_NP_CS_NEXTPORT   (0x004c) /* Next port register                */

/* Bit definitions for the control register */

#define HF_NP_CS_CONTROL_DRAM32    (0x00000001) /* 0=>16 bit, 1=>32 bit      */
#define HF_NP_CS_CONTROL_DRAMSIZE  (0x00000002) /* 0=>16Mbit, 1=>64Mbit      */
#define HF_NP_CS_CONTROL_DRAMBANKS (0x00000004) /* 0=>1 bank, 1=>2 banks     */
#define HF_NP_CS_CONTROL_NOREFRESH (0x00000008) /* 1=>disable DRAM refresh   */
#define HF_NP_CS_CONTROL_8KHZ      (0x00000010) /* Mask to get 8KHz source   */
#define HF_NP_CS_CONTROL_8KHZ_UTOP (0x00000000) /* 8KHz sourced from Utopia  */
#define HF_NP_CS_CONTROL_8KHZ_INT  (0x00000010) /* 8KHz internally generated */
#define HF_NP_CS_CONTROL_POWERDOWN (0x00000020) /* 1=>DRAM power down        */
                                                /* whenever possible         */
#define HF_NP_CS_CONTROL_ADSLCLK   (0x00000040) /* Disable ADSL clock        */
#define HF_NP_CS_CONTROL_NOBOOTROM (0x00000080) /* 0=>boot ROM mapped at 0,  */
                                                /* 1=>mapped out             */
#define HF_NP_CS_CONTROL_ETHCLK    (0x00000100) /* Disable Ethernet clock    */
#define HF_NP_CS_CONTROL_HDLCCLK   (0x00000200) /* Disable HDLC clock        */
#define HF_NP_CS_CONTROL_UTOPCLK   (0x00000400) /* Disable Utopia clock      */
#define HF_NP_CS_CONTROL_USBCLK    (0x00000800) /* Disable USB clock         */
#define HF_NP_CS_CONTROL_PROCESSOR (0x80000000) /* 0=>code running on PP     */
                                                /* 1=>code running on NP     */
/* Bit definitions for the interrupt status register */

#define HF_NP_CS_INTSTATUS_HDLC     (0x00000001) /* HDLC port is interrupting*/
#define HF_NP_CS_INTSTATUS_USB      (0x00000002) /* USB port is interrupting */
#define HF_NP_CS_INTSTATUS_ETH      (0x00000004) /* Ethernet is interrupting */
#define HF_NP_CS_INTSTATUS_UTOP     (0x00000008) /* Utopia port interrupting */
#define HF_NP_CS_INTSTATUS_ADSL     (0x00000010) /* ADSL port interrupting   */
#define HF_NP_CS_INTSTATUS_TIMER3   (0x00000020) /* Timer 3 is interrupting  */
#define HF_NP_CS_INTSTATUS_DOORBELL (0x00000040) /* NP doorbell interrupting */
#define HF_NP_CS_INTSTATUS_NEXTPORT (0x00000080) /* Next port logic interrupt*/

/* NP interrupt controller device priority list.         */
/* These are the codes returned by the next-IRQ register */
/* to identify the next device to be serviced.           */

#define HF_NP_CS_NEXTIRQ_HDLC     (1)
#define HF_NP_CS_NEXTIRQ_ETHERNET (2)
#define HF_NP_CS_NEXTIRQ_USB      (3)
#define HF_NP_CS_NEXTIRQ_UTOPIA   (4)
#define HF_NP_CS_NEXTIRQ_ADSL     (5)
#define HF_NP_CS_NEXTIRQ_TIMER3   (6)
#define HF_NP_CS_NEXTIRQ_DOORBELL (7)
/* Alias for compatibility with ASICs with more than 2 processors */
#define HF_NP_CS_NEXTIRQ_BELLFROMPP  HF_NP_CS_NEXTIRQ_DOORBELL

/* Values for the next port register.                           */
/* These codes indicated the next port that should be serviced. */

#define HF_NP_CS_NEXTPORT_NONE      (0) /* No port needs servicing */
#define HF_NP_CS_NEXTPORT_ETHRX     (1) /* Ethernet rx port        */
#define HF_NP_CS_NEXTPORT_ETHTX     (2) /* Ethernet tx port        */
#define HF_NP_CS_NEXTPORT_USBRX     (3) /* USB rx port             */
#define HF_NP_CS_NEXTPORT_USBTX     (4) /* USB tx port             */
#define HF_NP_CS_NEXTPORT_HDLCRX    (5) /* HDLC rx port            */
#define HF_NP_CS_NEXTPORT_HDLCTX    (6) /* HDLC tx port            */
#define HF_NP_CS_NEXTPORT_ADSLRX    (7) /* ADSL rx port            */
#define HF_NP_CS_NEXTPORT_ADSLTX    (8) /* ADSL tx port            */
#define HF_NP_CS_NEXTPORT_UTOPRX0   (9) /* Utopia rx port 0        */
#define HF_NP_CS_NEXTPORT_UTOPTX0  (10) /* Utopia tx port 0        */
#define HF_NP_CS_NEXTPORT_UTOPRX1  (11) /* Utopia rx port 1        */
#define HF_NP_CS_NEXTPORT_UTOPTX1  (12) /* Utopia tx port 1        */
#define HF_NP_CS_NEXTPORT_UTOPRX2  (13) /* Utopia rx port 2        */
#define HF_NP_CS_NEXTPORT_UTOPTX2  (14) /* Utopia tx port 2        */
#define HF_NP_CS_NEXTPORT_UTOPRX3  (15) /* Utopia rx port 3        */
#define HF_NP_CS_NEXTPORT_UTOPTX3  (16) /* Utopia tx port 3        */
#define HF_NP_CS_NEXTPORT_UTOPRX4  (17) /* Utopia rx port 4        */
#define HF_NP_CS_NEXTPORT_UTOPTX4  (18) /* Utopia tx port 4        */
#define HF_NP_CS_NEXTPORT_UTOPRX5  (19) /* Utopia rx port 5        */
#define HF_NP_CS_NEXTPORT_UTOPTX5  (20) /* Utopia tx port 5        */
#define HF_NP_CS_NEXTPORT_UTOPRX6  (21) /* Utopia rx port 6        */
#define HF_NP_CS_NEXTPORT_UTOPTX6  (22) /* Utopia tx port 6        */
#define HF_NP_CS_NEXTPORT_UTOPRX7  (23) /* Utopia rx port 7        */
#define HF_NP_CS_NEXTPORT_UTOPTX7  (24) /* Utopia tx port 7        */
#define HF_NP_CS_NEXTPORT_UTOPRX8  (25) /* Utopia rx port 8        */
#define HF_NP_CS_NEXTPORT_UTOPTX8  (26) /* Utopia tx port 8        */
#define HF_NP_CS_NEXTPORT_UTOPRX9  (27) /* Utopia rx port 9        */
#define HF_NP_CS_NEXTPORT_UTOPTX9  (28) /* Utopia tx port 9        */
#define HF_NP_CS_NEXTPORT_UTOPRX10 (29) /* Utopia rx port 10       */
#define HF_NP_CS_NEXTPORT_UTOPTX10 (30) /* Utopia tx port 10       */
#define HF_NP_CS_NEXTPORT_UTOPRX11 (31) /* Utopia rx port 11       */
#define HF_NP_CS_NEXTPORT_UTOPTX11 (32) /* Utopia tx port 11       */
#define HF_NP_CS_NEXTPORT_UTOPRX12 (33) /* Utopia rx port 12       */
#define HF_NP_CS_NEXTPORT_UTOPTX12 (34) /* Utopia tx port 12       */
#define HF_NP_CS_NEXTPORT_UTOPRX13 (35) /* Utopia rx port 13       */
#define HF_NP_CS_NEXTPORT_UTOPTX13 (36) /* Utopia tx port 13       */

#define NEXTPORT_UTOPRX_LAST       HF_NP_CS_NEXTPORT_UTOPRX13
#define NEXTPORT_UTOPTX_LAST       HF_NP_CS_NEXTPORT_UTOPTX13

/*****************************************************************************
 *                                                                           *
 *           CELL/PACKET COUNTERS                                            * 
 *           ====================                                            * 
 *                                                                           *
 *****************************************************************************/

/* Register offsets from NP_NPAPB */
#define HF_NP_COUNT (0x0800) /* Cell/packet count registers */

#define HF_NP_COUNT_UTOP0_TX    (0x800) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP1_TX    (0x804) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP2_TX    (0x808) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP3_TX    (0x80c) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP4_TX    (0x810) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP5_TX    (0x814) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP6_TX    (0x818) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP7_TX    (0x81c) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP8_TX    (0x820) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP9_TX    (0x824) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP10_TX   (0x828) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP11_TX   (0x82c) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP12_TX   (0x830) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_UTOP13_TX   (0x834) /* Utopia port 0 transmit count     */
#define HF_NP_COUNT_ADSLHDLC_TX (0x838) /* ADSL/HDLC transmit count         */
#define HF_NP_COUNT_ETH_TX      (0x83c) /* Ethernet transmit count          */

#define HF_NP_COUNT_UTOP0_RX    (0x840) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP1_RX    (0x844) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP2_RX    (0x848) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP3_RX    (0x84c) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP4_RX    (0x850) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP5_RX    (0x854) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP6_RX    (0x858) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP7_RX    (0x85c) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP8_RX    (0x860) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP9_RX    (0x864) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP10_RX   (0x868) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP11_RX   (0x86c) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP12_RX   (0x870) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_UTOP13_RX   (0x874) /* Utopia port 0 receive count      */
#define HF_NP_COUNT_ADSLHDLC_RX (0x878) /* ADSL/HDLC receive count          */
#define HF_NP_COUNT_ETH_RX      (0x87c) /* Ethernet receive count           */

#define HF_NP_COUNT_MASK    (0x00ffffff) /* Mask of valid bits in counters */

/* PDMA mask */

#define HF_NP_PDMAMASK    (0xfe000000) /* Mask for PDMA control bits */

#endif


#endif /* ! __SOLOS_W_H */
