/* ###########################################################################
# (c) Copyright GlobespanVirata Limited 2003
#
# GlobespanVirata Limited Confidential and Proprietary
#
# The following software source code ("Software") is strictly confidential
# and is proprietary to GlobespanVirata Limited ("GlobespanVirata").  It may
# only be read, used, copied, adapted, modified or otherwise dealt with by
# you if you have entered into a confidentiality agreement with Virata and
# then subject to the terms of that confidentiality agreement and any other
# applicable agreement between you and GlobespanVirata  If you are in any
# doubt as to whether you are entitled to access, read, use, copy, adapt,
# modify or otherwise deal with the Software or whether you are entitled to
# disclose the Software to any other person you should contact GlobespanVirata.
# If you have not entered into a confidentiality agreement with GlobespanVirata
# granting access to this Software you should forthwith return all media,
# copies and printed listings containing the Software to GlobespanVirata.
#
# GlobespanVirata reserves the right to take legal action against you should
# you breach the above provisions.
#
# If you are unsure, or to report violations, please contact
# support@globespanvirata.com
# ##########################################################################*/

/****************************************************************************
*  bf_config.h                      Last Modified   -   ABP   -   10/14/05  *
*****************************************************************************/

/****************************************************************************/
/*                                                                          */
/* bf_devconfig.h                                                           */
/*                                                                          */
/* Bandit Family Device Configuration Block Hardware Definitions            */
/*                                                                          */
/****************************************************************************/

#ifndef BF_DEVCONF_H
#define BF_DEVCONF_H


/* DEVCONF interface registers */

#define BF_DEVCONF_REGS_CONFIG_REGISTER        (0x00)
#define BF_DEVCONF_REGS_CONFIG_BOOT_CONTROL    (0x04)
#define BF_DEVCONF_REGS_CONFIG_RESET_CONTROL   (0x08)
#define BF_DEVCONF_REGS_CONFIG_PLL_REG_SEL     (0x0C)
#define BF_DEVCONF_REGS_CONFIG_PLL_WRITE_DATA  (0x10)
#define BF_DEVCONF_REGS_CONFIG_PLL_READ_DATA   (0x14)
#define BF_DEVCONF_REGS_CONFIG_PLL_CONTROL     (0x18)
#define BF_DEVCONF_REGS_F_PLL0_STATIC_CONTROL  (0x1C)
#define BF_DEVCONF_REGS_F_PLL0_DIVIDER_CONTROL (0x20)
#define BF_DEVCONF_REGS_F_PLL0_DELTA_CONTROL1  (0x24)
#define BF_DEVCONF_REGS_F_PLL0_DELTA_CONTROL2  (0x28)
#define BF_DEVCONF_REGS_F_PLL0_SPECTRUM_CONTROL (0x2C)
#define BF_DEVCONF_REGS_F_PLL1_STATIC_CONTROL  (0x30)
#define BF_DEVCONF_REGS_F_PLL1_DIVIDER_CONTROL (0x34)
#define BF_DEVCONF_REGS_F_PLL1_DELTA_CONTROL1  (0x38)
#define BF_DEVCONF_REGS_F_PLL1_DELTA_CONTROL2  (0x3C)
#define BF_DEVCONF_REGS_F_PLL1_SPECTRUM_CONTROL (0x40)
#define BF_DEVCONF_REGS_CONFIG_COMMON          (0x44)
#define BF_DEVCONF_REGS_CONFIG_RRM0            (0x48)
#define BF_DEVCONF_REGS_CONFIG_RRM1            (0x4C)
#define BF_DEVCONF_REGS_CONFIG_BBPCK           (0x50)
#define BF_DEVCONF_REGS_CONFIG_ARMCK           (0x54)
#define BF_DEVCONF_REGS_CONFIG_USBCK           (0x58)
#define BF_DEVCONF_REGS_CONFIG_MACCK           (0x5C)
#define BF_DEVCONF_REGS_CONFIG_HSSDCK          (0x60)
#define BF_DEVCONF_REGS_CONFIG_DNOVACK         (0x64)
#define BF_DEVCONF_REGS_CONFIG_REFCK           (0x68)
#define BF_DEVCONF_REGS_CONFIG_STLPCK          (0x6C)
#define BF_DEVCONF_REGS_LOCK_STATUS            (0x70)
#define BF_DEVCONF_REGS_CONFIG_DIAGNOSTIC      (0x74)
#define BF_DEVCONF_REGS_CONFIG_SDRAM_CLK_DEL   (0x78)



/* CONFIG VERSION NUMBER */
/* INDICATES SILICON REV */

// CONFIG_REGISTER
#define BF_DEVCONF_CONFIG_STUN_ULPI_MUX_EN  (0x40000000)
#define BF_DEVCONF_CONFIG_STUNG_DBG_ENABLE  (0x20000000)
#define BF_DEVCONF_CONFIG_OTDM_IF_ENABLE    (0x10000000)
#define BF_DEVCONF_CONFIG_OTDM_CK_ENABLE    (0x08000000)
#define BF_DEVCONF_CONFIG_ULPI_CK_ENABLE    (0x04000000)
#define BF_DEVCONF_CONFIG_EPHY_MODE         (0x02000000)
#define BF_DEVCONF_CONFIG_VPN_ENABLE        (0x01000000)
#define BF_DEVCONF_CONFIG_SERIAL_MODE       (0x00800000)
#define BF_DEVCONF_CONFIG_PARALLEL_MODE     (0x00400000)
#define BF_DEVCONF_CONFIG_EXT_USB_PHY_EN    (0x00200000)
#define BF_DEVCONF_CONFIG_SUBS_VER_MASK     (0x001E0000)
#define BF_DEVCONF_CONFIG_CHIP_ID_MASK      (0x00010000)
#define BF_DEVCONF_CONFIG_VER_NUM_MASK      (0x0000FFFF)

// CONFIG_BOOT_CONTROL
#define BF_DEVCONF_OUT_OF_RESET         (0x00000010)
#define BF_DEVCONF_RTL_SIM_MODE         (0x00000002)
#define BF_DEVCONF_RELEASE_IF_RESET     (0x00000001)

// CONFIG_RESET_CONTROL
#define BF_DEVCONF_RESET_PRE5CONF_MASK  (0xFF000000)
#define BF_DEVCONF_RESET_PRE5CONF_SHIFT (24)
#define BF_DEVCONF_TRIGGER_N_RESET_OUT  (0x00000001)

// CONFIG_PLL_REG_SEL
#define BF_DEVCONF_SEL_REG_0            (0x00000000)
#define BF_DEVCONF_SEL_REG_1            (0x00000001)
#define BF_DEVCONF_SEL_REG_2            (0x00000002)
#define BF_DEVCONF_SEL_REG_3            (0x00000003)
#define BF_DEVCONF_SEL_REG_4            (0x00000004)
#define BF_DEVCONF_SEL_REG_5            (0x00000005)
#define BF_DEVCONF_SEL_REG_6            (0x00000006)
#define BF_DEVCONF_SEL_REG_7            (0x00000007)
#define BF_DEVCONF_SEL_REG_8            (0x00000008)
#define BF_DEVCONF_SEL_REG_9            (0x00000009)
#define BF_DEVCONF_SEL_REG_10           (0x0000000A)
#define BF_DEVCONF_SEL_REG_11           (0x0000000B)
#define BF_DEVCONF_SEL_REG_12           (0x0000000C)
#define BF_DEVCONF_SEL_REG_13           (0x0000000D)
#define BF_DEVCONF_SEL_REG_14           (0x0000000E)
#define BF_DEVCONF_SEL_REG_15           (0x0000000F)

// Calculation : Fvco = (M + (P/D)) * Fref ; Fout = Fvco * Divider

#define BF_DEVCONF_AFE_PLL_D_VAL        (0x00000271)            // D=625
#define BF_DEVCONF_AFE_PLL_P_VAL        (0x000000A4)            // P=164
#define BF_DEVCONF_AFE_PLL_M_VAL        (0x0000001C)            // M=28
#define BF_DEVCONF_AFE_PLL_SET_KFAC     (0x00000100)            

// CONFIG_PLL_CONTROL
#define BF_DEVCONF_DLL_CLK_ENABLE       (0x00000010)
#define BF_DEVCONF_AFE_CLK_ENABLE       (0x00000008)
#define BF_DEVCONF_PLL_DLL_NRST         (0x00000004)

#define BF_DEVCONF_PLL_WRITE_SETUP      (0x00000003)
#define BF_DEVCONF_PLL_WRITE            (0x00000002)
#define BF_DEVCONF_PLL_READ             (0x00000001)
#define BF_DEVCONF_PLL_WRITE_STOP       (0x00000000)

// ------------------
#define BF_DEVCONF_PLL_CONTROL_HIREQ    (0x00000001)
#define BF_DEVCONF_PLL_CONTROL_HIW_RN   (0x00000002)

#define BF_DEVCONF_PLL_DIV_1            (0x00000000)
#define BF_DEVCONF_PLL_DIV_2            (0x00001000)
#define BF_DEVCONF_PLL_DIV_3            (0x00002000)
#define BF_DEVCONF_PLL_DIV_4            (0x00003000)
#define BF_DEVCONF_PLL_DIV_5            (0x00004000)
#define BF_DEVCONF_PLL_DIV_6            (0x00005000)
#define BF_DEVCONF_PLL_DIV_7            (0x00006000)
#define BF_DEVCONF_PLL_DIV_8            (0x00007000)

#define BF_DEVCONF_PLL_XLOL_0           (0x00000000)
#define BF_DEVCONF_PLL_XLOL_1           (0x00000100)
#define BF_DEVCONF_PLL_XLOL_2           (0x00000200)
#define BF_DEVCONF_PLL_XLOL_3           (0x00000300)
#define BF_DEVCONF_PLL_XLOL_4           (0x00000400)
#define BF_DEVCONF_PLL_XLOL_5           (0x00000500)
#define BF_DEVCONF_PLL_XLOL_6           (0x00000600)
#define BF_DEVCONF_PLL_XLOL_7           (0x00000700)
// ------------------

// F_PLL_STATIC_CONTROL
#define BF_DEVCONF_FPLL_FOUT_EN        (0x00001000)
#define BF_DEVCONF_FPLL_FIL_CTRL       (0x00000E00)
#define BF_DEVCONF_FPLL_CP_CTRL        (0x00000180)
#define BF_DEVCONF_FPLL_TST_EN         (0x00000040)
#define BF_DEVCONF_FPLL_TST_CTRL       (0x00000038)
#define BF_DEVCONF_FPLL_BYPASS_PLL     (0x00000004)
#define BF_DEVCONF_FPLL_SOFT_NRST      (0x00000002)
#define BF_DEVCONF_FPLL_PD_PLL         (0x00000001)

// F_PLL_DIVIDER_CONTROL
#define BF_DEVCONF_POSTDIV_VAL_2        (0x00000000)
#define BF_DEVCONF_POSTDIV_VAL_3        (0x00000010)
#define BF_DEVCONF_POSTDIV_VAL_4        (0x00000020)
#define BF_DEVCONF_POSTDIV_VAL_6        (0x00000030)
#define BF_DEVCONF_DIFF_OUT_EN          (0x00000040)

#define BF_DEVCONF_PREDIV_VAL_1         (0x00000002)
#define BF_DEVCONF_PREDIV_VAL_2         (0x00000004)
#define BF_DEVCONF_PREDIV_VAL_3         (0x00000006)
#define BF_DEVCONF_PREDIV_VAL_4         (0x00000008)
#define BF_DEVCONF_PREDIV_VAL_5         (0x0000000A)
#define BF_DEVCONF_PREDIV_VAL_6         (0x0000000C)
#define BF_DEVCONF_PREDIV_VAL_7         (0x0000000E)

#define BF_DEVCONF_SEL_VCO_UPPER        (0x00000000)
#define BF_DEVCONF_SEL_VCO_LOWER        (0x00000001)

// F_PLL_DELTA_CONTROL1
#define BF_DEVCONF_INT_DIVISOR_MASK     (0x0001FF00)
#define BF_DEVCONF_INT_DIVISOR_SHIFT    (8)
#define BF_DEVCONF_SD_CTRL_MASK         (0x000000FE)
#define BF_DEVCONF_SD_CTRL_SHIFT        (1)
#define BF_DEVCONF_INT_MODE             (0x00000001)

// F_PLL_SPECTRUM_CONTROL
#define BF_DEVCONF_SS_COUNT_FINAL_MASK     (0x003FE000)
#define BF_DEVCONF_SS_COUNT_FINAL_SHIFT    (13)
#define BF_DEVCONF_SS_COUNT_START_MASK     (0x00001FF0)
#define BF_DEVCONF_SS_COUNT_START_SHIFT    (4)
#define BF_DEVCONF_SS_DELTA_MASK           (0x0000000E)
#define BF_DEVCONF_SS_DELTA_SHIFT          (1)
#define BF_DEVCONF_SS_ENABLE               (0x00000001)
#define BF_DEVCONF_SS_DISABLE              (0x00000000)

// CONFIG_COMMON
#define BF_DEVCONF_USB_GFM_NORMAL_MODE  (0x00000C00)
#define BF_DEVCONF_GFM_NORMAL_MODE      (0x000003FE)
#define BF_DEVCONF_COMDIV_SELECT_FPLL0  (0x00000000)
#define BF_DEVCONF_COMDIV_SELECT_FPLL1  (0x00000001)

// CONFIG_RRM
#define BF_DEVCONF_RRM_LOW_JITTER_EN   (0x00200000)
#define BF_DEVCONF_RRM_SYM_MOD_EN      (0x00100000)
#define BF_DEVCONF_RRM_INPUT_SEL_MASK  (0x000F0000)
#define BF_DEVCONF_RRM_INPUT_SEL_SHIFT (16)
#define BF_DEVCONF_RRM_NUMERATOR_MASK  (0x0000FF00)
#define BF_DEVCONF_RRM_NUMERATOR_SHIFT (8)
#define BF_DEVCONF_RRM_DIVIDER_MASK    (0x000000FF)

// CONFIG VALUES FOR OTHER BLOCKS
#define BF_DEVCONF_BBPCK_DIVIDER_BYPASS   (0x00000080)
#define BF_DEVCONF_ARMCK_DIVIDER_BYPASS   (0x00000040)
#define BF_DEVCONF_USBCK_DISABLE          (0x00000080)
#define BF_DEVCONF_USBCK_DIVIDER_BYPASS   (0x00000040)
#define BF_DEVCONF_MIICK_DISABLE          (0x00000080)
#define BF_DEVCONF_MACCK_DIVIDER_BYPASS   (0x00000040)
#define BF_DEVCONF_HSSDCK_DIVIDER_BYPASS  (0x00000100)
#define BF_DEVCONF_DNOVACK_DIVIDER_BYPASS (0x00000040)
#define BF_DEVCONF_REFCK_DIVIDER_BYPASS   (0x00000008)
#define BF_DEVCONF_STLPCK_DIVIDER_BYPASS  (0x00000004)

#define BF_DEVCONF_BBPCK_DIV_1            (0x00000000)
#define BF_DEVCONF_BBPCK_DIV_2            (0x00000010)
#define BF_DEVCONF_BBPCK_DIV_3            (0x00000020)
#define BF_DEVCONF_BBPCK_DIV_4            (0x00000030)
#define BF_DEVCONF_BBPCK_DIV_5            (0x00000040)
#define BF_DEVCONF_BBPCK_DIV_6            (0x00000050)
#define BF_DEVCONF_BBPCK_DIV_7            (0x00000060)
#define BF_DEVCONF_BBPCK_DIV_8            (0x00000070)

#define BF_DEVCONF_ARMCK_DIV_1            (0x00000000)
#define BF_DEVCONF_ARMCK_DIV_2            (0x00000010)
#define BF_DEVCONF_ARMCK_DIV_3            (0x00000020)
#define BF_DEVCONF_ARMCK_DIV_4            (0x00000030)

#define BF_DEVCONF_USBCK_DIV_1            (0x00000000)
#define BF_DEVCONF_USBCK_DIV_2            (0x00000010)
#define BF_DEVCONF_USBCK_DIV_3            (0x00000020)
#define BF_DEVCONF_USBCK_DIV_4            (0x00000030)

#define BF_DEVCONF_MACCK_DIV_1            (0x00000000)
#define BF_DEVCONF_MACCK_DIV_2            (0x00000010)
#define BF_DEVCONF_MACCK_DIV_3            (0x00000020)
#define BF_DEVCONF_MACCK_DIV_4            (0x00000030)

#define BF_DEVCONF_HSSDCK_DIV_1            (0x00000000)
#define BF_DEVCONF_HSSDCK_DIV_2            (0x00000010)
#define BF_DEVCONF_HSSDCK_DIV_3            (0x00000020)
#define BF_DEVCONF_HSSDCK_DIV_4            (0x00000030)
#define BF_DEVCONF_HSSDCK_DIV_5            (0x00000040)
#define BF_DEVCONF_HSSDCK_DIV_6            (0x00000050)
#define BF_DEVCONF_HSSDCK_DIV_7            (0x00000060)
#define BF_DEVCONF_HSSDCK_DIV_8            (0x00000070)
#define BF_DEVCONF_HSSDCK_DIV_9            (0x00000080)
#define BF_DEVCONF_HSSDCK_DIV_10           (0x00000090)
#define BF_DEVCONF_HSSDCK_DIV_11           (0x000000A0)
#define BF_DEVCONF_HSSDCK_DIV_12           (0x000000B0)
#define BF_DEVCONF_HSSDCK_DIV_13           (0x000000C0)
#define BF_DEVCONF_HSSDCK_DIV_14           (0x000000D0)
#define BF_DEVCONF_HSSDCK_DIV_15           (0x000000E0)
#define BF_DEVCONF_HSSDCK_DIV_16           (0x000000F0)

#define BF_DEVCONF_DNOVACK_DIV_1            (0x00000000)
#define BF_DEVCONF_DNOVACK_DIV_2            (0x00000010)
#define BF_DEVCONF_DNOVACK_DIV_3            (0x00000020)
#define BF_DEVCONF_DNOVACK_DIV_4            (0x00000030)

#define BF_DEVCONF_REFCK_DIV_1            (0x00000000)
#define BF_DEVCONF_REFCK_DIV_2            (0x00000001)
#define BF_DEVCONF_REFCK_DIV_3            (0x00000002)
#define BF_DEVCONF_REFCK_DIV_4            (0x00000003)
#define BF_DEVCONF_REFCK_DIV_5            (0x00000004)
#define BF_DEVCONF_REFCK_DIV_6            (0x00000005)
#define BF_DEVCONF_REFCK_DIV_7            (0x00000006)
#define BF_DEVCONF_REFCK_DIV_8            (0x00000007)

#define BF_DEVCONF_STLPCK_DIV_1            (0x00000000)
#define BF_DEVCONF_STLPCK_DIV_2            (0x00000001)
#define BF_DEVCONF_STLPCK_DIV_3            (0x00000002)
#define BF_DEVCONF_STLPCK_DIV_4            (0x00000003)

#define BF_DEVCONF_INPUT_XTAL_IN          (0x00000000)
#define BF_DEVCONF_INPUT_FPLL0_FOUT_DIV2  (0x00000001)
#define BF_DEVCONF_INPUT_FPLL0_FOUT_DIV5  (0x00000002)
#define BF_DEVCONF_INPUT_FPLL0_FVCO_DIV5  (0x00000003)
#define BF_DEVCONF_INPUT_FPLL1_FOUT_DIV2  (0x00000004)
#define BF_DEVCONF_INPUT_RRM0             (0x00000005)
#define BF_DEVCONF_INPUT_RRM1             (0x00000006)
#define BF_DEVCONF_INPUT_EXT_CLK1         (0x00000007)
#define BF_DEVCONF_INPUT_EXT_CLK2         (0x00000008)
#define BF_DEVCONF_INPUT_GND              (0x00000009)

#define BF_DEVCONF_INPUT_FPLL_120       BF_DEVCONF_INPUT_FPLL0_FOUT_DIV5 
#define BF_DEVCONF_INPUT_FPLL_240       BF_DEVCONF_INPUT_FPLL0_FVCO_DIV5 
#define BF_DEVCONF_INPUT_FPLL_300       BF_DEVCONF_INPUT_FPLL0_FOUT_DIV2 
#define BF_DEVCONF_INPUT_FPLL_264       BF_DEVCONF_INPUT_FPLL1_FOUT_DIV2 

#define BF_DEVCONF_INPUT_FPLL_90        BF_DEVCONF_INPUT_FPLL0_FOUT_DIV5 
#define BF_DEVCONF_INPUT_FPLL_180       BF_DEVCONF_INPUT_FPLL0_FVCO_DIV5 
#define BF_DEVCONF_INPUT_FPLL_225       BF_DEVCONF_INPUT_FPLL0_FOUT_DIV2 

// LOCK_STATUS
#define BF_DEVCONF_FPLL1_LOCK_STATUS    (0x00000004) /* Lock status for FPLL1   */
#define BF_DEVCONF_FPLL0_LOCK_STATUS    (0x00000002) /* Lock status for FPLL0   */
#define BF_DEVCONF_PLL_DLL_LOCK_STATUS  (0x00000001) /* Lock status for PLL/DLL */

// CONFIG_DIAGNOSTIC
#define BF_DEVCONF_DIAGNOSTIC_ZERO               (0x00000000)
#define BF_DEVCONF_DIAGNOSTIC_XTAL_CK            (0x00000001)
#define BF_DEVCONF_DIAGNOSTIC_AFE_CK             (0x00000002)
#define BF_DEVCONF_DIAGNOSTIC_DLL_CK             (0x00000003)
#define BF_DEVCONF_DIAGNOSTIC_FPLL0_FOUT_DIV5    (0x00000004)
#define BF_DEVCONF_DIAGNOSTIC_FPLL0_FOUT_DIV2    (0x00000005)
#define BF_DEVCONF_DIAGNOSTIC_FPLL0_FVCO_DIV5    (0x00000006)
#define BF_DEVCONF_DIAGNOSTIC_FPLL1_FOUT_DIV2    (0x00000007)
#define BF_DEVCONF_DIAGNOSTIC_RRM0_CK            (0x00000008)
#define BF_DEVCONF_DIAGNOSTIC_RRM1_CK            (0x00000009)
#define BF_DEVCONF_DIAGNOSTIC_USB_CK             (0x0000000A)
#define BF_DEVCONF_DIAGNOSTIC_MAC_CK             (0x0000000B)
#define BF_DEVCONF_DIAGNOSTIC_HSSD_CK            (0x0000000C)
#define BF_DEVCONF_DIAGNOSTIC_8MHZ_REF_CK        (0x0000000D)
#define BF_DEVCONF_DIAGNOSTIC_STLP_CK            (0x0000000E)
#define BF_DEVCONF_DIAGNOSTIC_DNOVA_CK           (0x0000000F)
#define BF_DEVCONF_DIAGNOSTIC_BBP_CK             (0x00000010)
#define BF_DEVCONF_DIAGNOSTIC_ARM10_CK           (0x00000011)
#define BF_DEVCONF_DIAGNOSTIC_AHB_CK             (0x00000012)
#define BF_DEVCONF_DIAGNOSTIC_SDRAM_CLK1         (0x00000013)
#define BF_DEVCONF_DIAGNOSTIC_SDRAM_CLK2         (0x00000014)
#define BF_DEVCONF_DIAGNOSTIC_ONE                (0x00000015)
#define BF_DEVCONF_DIAGNOSTIC_USB11_ENABLE       (0x00400000)

// CONFIG_SDRAM_CLK_DEL
#define BF_DEVCONF_SDRAM_DEL1_000                (0x00000001)
#define BF_DEVCONF_SDRAM_DEL1_100                (0x00000002)
#define BF_DEVCONF_SDRAM_DEL1_200                (0x00000004)
#define BF_DEVCONF_SDRAM_DEL1_300                (0x00000008)
#define BF_DEVCONF_SDRAM_DEL1_400                (0x00000010)
#define BF_DEVCONF_SDRAM_DEL2_000                (0x00210000)
#define BF_DEVCONF_SDRAM_DEL2_100                (0x00220000)
#define BF_DEVCONF_SDRAM_DEL2_200                (0x00240000)
#define BF_DEVCONF_SDRAM_DEL2_300                (0x00280000)
#define BF_DEVCONF_SDRAM_DEL2_400                (0x00300000)
#define BF_DEVCONF_SDRAM_DEL2_500                (0x00500000)
#define BF_DEVCONF_SDRAM_DEL2_600                (0x00900000)
#define BF_DEVCONF_SDRAM_DEL2_700                (0x01100000)
#define BF_DEVCONF_SDRAM_DEL2_800                (0x02100000)

#endif
