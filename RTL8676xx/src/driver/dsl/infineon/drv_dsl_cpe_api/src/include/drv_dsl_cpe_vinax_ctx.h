/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef DRV_DSL_CPE_VINAX_CTX_H
#define DRV_DSL_CPE_VINAX_CTX_H

#ifdef __cplusplus
   extern "C" {
#endif

/** \file
   VINAX internal interface
*/

/** \addtogroup DRV_DSL_DEVICE
 @{ */


/**
   Clock output modes at pin CLKO1 */
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error! */
   DSL_DEV_CLOCKOUT_NA = -1,
   /**
      Enable the 35.328MHz clock output at pin CLKO1 */
   DSL_DEV_CLOCKOUT_ON = 0,
   /**
      Disable the 35.328MHz clock output at pin CLKO1 */
   DSL_DEV_CLOCKOUT_OFF = 1,
   /**
      Delimiter only! */
   DSL_DEV_CLOCKOUT_LAST = 2
} DSL_DEV_Clockout_t;

/**
   HDLC Interface modes */
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error! */
   DSL_DEV_HDLCMODE_NA = -1,
   /**
      Select open-drain mode for HDLC-IF  */
   DSL_DEV_HDLCMODE_OD = 0,
   /**
      Select push-pull mode for HDLC-IF  */
   DSL_DEV_HDLCMODE_PP = 1,
   /**
      Delimiter only! */
   DSL_DEV_HDLCMODE_LAST = 2
} DSL_DEV_HdlcMode_t;

/**
   Clock Sources for the MII */
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error! */
   DSL_DEV_MIICLKSOURCE_NA = -1,
   /**
      xMII Clock generation from external source at CLKIN2 */
   DSL_DEV_MIICLKSOURCE_EXT = 0,
   /**
      xMII Clock generation from internal */
   DSL_DEV_MIICLKSOURCE_INT = 1,
   /**
      xMII Clock generation from 25MHz crystal at CLKIN2/CLKO2 */
   DSL_DEV_MIICLKSOURCE_CRY = 2,
   /**
      Delimiter only! */
   DSL_DEV_MIICLKSOURCE_LAST = 3
} DSL_DEV_MiiClockSource_t;

/**
   Clock Chains for the MII */
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error! */
   DSL_DEV_MIICLKCHAIN_NA = -1,
   /**
      Disable the Clock outputs for the MII */
   DSL_DEV_MIICLKCHAIN_OFF = 0,
   /**
      Enable the Clock outputs for the MII */
   DSL_DEV_MIICLKCHAIN_ON = 1,
   /**
      Delimiter only! */
   DSL_DEV_MIICLKCHAIN_LAST = 2
} DSL_DEV_MiiClockChain_t;

/**
   Tc Mode */
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error! */
   DSL_DEV_TCMODE_NA = -1,
   /**
      TC layer mode is: MAC functionality */
   DSL_DEV_TCMODE_MAC = 0,
   /**
      TC layer mode is: virtual PHY functionality */
   DSL_DEV_TCMODE_VPHY = 1,
   /**
      Delimiter only! */
   DSL_DEV_TCMODE_LAST = 2
} DSL_DEV_TcMode_t;

/**
   MII Mode */
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error! */
   DSL_DEV_MIIMODE_NA = -1,
   /**
      MII interface */
   DSL_DEV_MIIMODE_MII = 0,
   /**
      RMII interface */
   DSL_DEV_MIIMODE_RMII = 1,
   /**
      SMII interface */
   DSL_DEV_MIIMODE_SMII = 2,
   /**
      SSSMII interface */
   DSL_DEV_MIIMODE_SSSMII = 3,
   /**
      MII interface off */
   DSL_DEV_MIIMODE_OFF = 4,
   /**
      Delimiter only! */
   DSL_DEV_MIIMODE_LAST = 5
} DSL_DEV_MiiMode_t;


/**
   Specifies to use special RX-/TX-gain calibration values.
*/
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error! */
   DSL_DEV_RX_TX_GAINS_NA = -1,
   /**
      Configuration setting for usage of internal default values for RX-/TX-gain
      calibration. In this case the following system parameters/configurations
      will be used to set different values that fits best:
      + Device type (GEMINAX-D MAX / GEMINAX-D.bis)
      + Annex type (AnnexA/B)
      + Line driver selection (linear / AL4)
        also configurable by low level device configuration (see parameter
        nLineDriver)
      \attention This is the recommended setting! Only change this setting in
                 special cases for test purpose. */
   DSL_DEV_RX_TX_GAINS_DEFAULT = 0,
   /**
      Configuration setting for usage of user defined values for RX-/TX-gain
      calibration. In this case the values has to be configured using the
      low level device configuration structure \ref DSL_DeviceLowLevelConfig_t */
   DSL_DEV_RX_TX_GAINS_USER = 1,
   /**
      Delimiter only! */
   DSL_DEV_RX_TX_GAINS_LAST = 2
} DSL_DEV_RxTxGainSelection_t;

/**
   Low level configuration settings for user defined RX-/TX-gains
   \attention This settings are only valid in case of using
              nGainSelection = \ref DSL_DEV_RX_TX_GAINS_USER
              in \ref DSL_DEV_RxTxGainSelection_t
*/
typedef struct
{
   /**
      User defined setting for RX gain calibration.*/
   DSL_int16_t nRxGain;
   /**
      User defined setting for TX gain calibration.*/
   DSL_int16_t nTxGain;
} DSL_DEV_RxTxGainSettings_t;

/**
   VDSL mode on the copper line.
   This configuration should be used to handle VDSL over ISDN and similar modes.
   The selections influences the handshake tone set according to
   G.994.1 Amendment 2, Table 1/G.994.1.
   Refer also to \ref DSL_DEV_Hybrid_t and \ref DSL_DEV_Filter_t to ensure
   consistent configurations.
   */
typedef enum
{
   /**
      Not applicable/not initialized.
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error!
      */
   DSL_DEV_LINEMODE_NA = -1,
   /**
      line is used for VDSL2 only.
      Carrier set B43 for VDSL2 with US0. */
   DSL_DEV_LINEMODE_VDSL2_B43   = 0x0001,
   /**
      line is used for VDSL2 only.
      Carrier set A43 for VDSL2 with US0 (No ISDN). */
   DSL_DEV_LINEMODE_VDSL2_A43   = 0x0002,
   /**
      line is used for VDSL2 without US0.
      Carrier set V43 for VDSL2 without US0. */
   DSL_DEV_LINEMODE_VDSL2_V43   = 0x0004,
   /**
      line is used for VDSL1.
      Carrier set V43P for VDSL1 over POTS. */
   DSL_DEV_LINEMODE_VDSL1_V43P   = 0x0008,
   /**
      line is used for VDSL1.
      Carrier set V43P for VDSL1 over ISDN. */
   DSL_DEV_LINEMODE_VDSL1_V43I   = 0x0010,
   /**
      line is used for ADSL(1).
      Carrier set C43 for Annex C/J. */
   DSL_DEV_LINEMODE_ADSL1_C43    = 0x0020,
   /**
      line is used for ADSL2.
      Carrier set J43 for ADSL2/2+ Annex J/M with US0 (No ISDN) */
   DSL_DEV_LINEMODE_ADSL2_J43    = 0x0040,
   /**
      line is used for ADSL2.
      Carrier set B43C for ADSL2+ with US0 */
   DSL_DEV_LINEMODE_ADSL2_B43C   = 0x0080,
   /**
      line is used for ADSL2.
      Carrier set A43C for ADSL2+ with US0 (No ISDN) */
   DSL_DEV_LINEMODE_ADSL2_A43C   = 0x0100,
   /**
      Delimiter only! */
   DSL_DEV_LINEMODE_LAST  = 0x0200
} DSL_DEV_LineMode_t;

/**
   Parity type for UTOPIA and POSPHY parity
*/
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error!
      */
   DSL_DEV_PARITY_NA = -1,
   /**
      Interface parity is odd */
   DSL_DEV_PARITY_ODD    = 0,
   /**
      Interface parity is even */
   DSL_DEV_PARITY_EVEN   = 1,
   /**
      Delimiter only! */
   DSL_DEV_PARITY_LAST
} DSL_Parity_t;

/**
   Type of the used Hybrid module.
   Select this according to your hardware.
   Refer also to \ref DSL_DEV_LineMode_t and \ref DSL_DEV_Filter_t to ensure
   consistent configurations.
*/
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occupy and indicated an internal error!
      */
   DSL_DEV_HYBRID_NA = -1,
   /**
      CO-Hybrid:
      Adaptive Hybrid HYB_AD1_138_17 for 138kHz - 17MHz
   */
   DSL_DEV_HYBRID_AD1_138_17 = 0,
   /**
      CO-Hybrid:
      Adaptive Hybrid HYB_AD1_25_17  for 25kHz - 17MHz
   */
   DSL_DEV_HYBRID_AD1_25_17 = 1,
   /**
      CO-Hybrid:
      Adaptive Hybrid HYB_AD1_138_30 for 138kHz - 17 MHz
   */
   DSL_DEV_HYBRID_AD1_138_30 = 2,
   /**
      CPE-Hybrid:
      Adaptive Hybrid for CPE HYB_AD1_138_17_CPE for 138kHz - 17MHz
   */
   DSL_DEV_HYBRID_AD1_138_17_CPE = 3,
   /**
      CPE-Hybrid:
      Filter Hybrid, HYB_F1_998.
      Filter hybrid for bandplan 998 without US0, 138kHz - 12MHz
   */
   DSL_DEV_HYBRID_F1_998 = 4,
   /**
      CPE-Hybrid:
      Filter Hybrid, HYB_F1_998_U0.
      Filter hybrid for bandplan 998 with US0, 25kHz - 12MHz, VDSL over POTS
   */
   DSL_DEV_HYBRID_F1_998_U0 = 5,
   /**
      CPE-Hybrid:
      Filter Hybrid, HYB_F1_998_U017_o_ISDN.
      Filter hybrid for bandplan 998 with US0,
      138kHz - 17MHz, DS3 (12MHz up to 17MHz), VDSL over ISDN.
   */
   DSL_DEV_HYBRID_F1_998_U017_o_ISDN = 6,
   /**
      CPE-Hybrid:
      Filter Hybrid, HYB_F1_JAP30.
      Filter hybrid Japan: 640kHz - 30MHz for VDSL over ISDN
   */
   DSL_DEV_HYBRID_F1_JAP30 = 7,

   DSL_DEV_HYBRID_F1_998E17_U0 = 8,
   /**
      Delimiter only! */
   DSL_DEV_HYBRID_LAST
} DSL_DEV_Hybrid_t;

/**
   Type for toggling the Clock Gating.
   This is for Debugging purpose only, leave it to default.
*/
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error!
      */
   DSL_DEV_CLK_GATING_NA = -1,
   /**
      Clock gating switch is off (default) */
   DSL_DEV_CLK_GATING_OFF    = 0,
   /**
      Clock gating switch is on */
   DSL_DEV_CLK_GATING_ON   = 1,
   /**
      Delimiter only! */
   DSL_DEV_CLK_GATING_LAST
} DSL_ClockGating_t;

/**
   Usage of additional filters for ISDN and POTS.
*/
typedef enum
{
   /**
      Not applicable/not initialized
      In case of writing the configuration the actual setting will NOT be
      changed, in case of reading the configuration this setting shall not
      occur and indicated an internal error!
      */
   DSL_DEV_FILTER_NA = -1,
   /**
      No additional filter is used. */
   DSL_DEV_FILTER_OFF = 0,
   /**
      Additional filter for XDSL over ISDN is used. */
   DSL_DEV_FILTER_ISDN = 1,
   /**
      Additional filter for xDSL over POTS is used
      (VDSL2 with PSD limitations). */
   DSL_DEV_FILTER_POTS = 2,
   /**
      Additional filter for VDSL over POTS is used. */
   DSL_DEV_FILTER_POTS_2 = 3,
   /** VDSL over POTS (Suggested mode for VDSL over
       POTS for double US0 and quad US0)*/
   DSL_DEV_FILTER_POTS_3 = 4,
   /**
      Delimiter only! */
   DSL_DEV_FILTER_LAST
} DSL_DEV_Filter_t;

/**
   Low level configuration that is device specific
*/
typedef struct
{
   /** Selection of RX-/TX-gain usage */
   DSL_DEV_RxTxGainSelection_t nGainSelection;
   /** Setting of RX-/TX-gain values in case of selecting user values */
   DSL_DEV_RxTxGainSettings_t nUserGainSettings;
   /** Enable or disable the 35.328MHz clock output at pin CLKO1.
      Note: This pin influences the power consumption. If not
         needed it shall be disabled */
   DSL_DEV_Clockout_t nClockoutEnable;
   /** Usage of additional filters for ISDN and POTS*/
   DSL_DEV_Filter_t nFilter;
   /** Select push-pull / open-drain for HDLC-IF  */
   DSL_DEV_HdlcMode_t nHdlcIfMode;
   /** Select the Clock source for the MII  */
   DSL_DEV_MiiClockSource_t nMiiClockSource;
   /** Select the Clock chain for the MII
      Enable Output of of Clock and SYNC signals for RMII, SSMII, SSSMII
      SSSMII:
	      SYNC_CLK_OUT_SSSMIIP
	      SYNC_CLK_OUT_SSSMIIM
	      SYSTEM_SYNC_OUT_SSSMIIP
	      SYSTEM_SYNC_OUT_SSSMIIM
      SSMII:
	      SYNCCLK_OUT_SSMIIP
	      SYNCCLK_OUT_SSMIIM
	      SYSTEM_SYNC_OUT_SSMIIP
	      SYSTEM_SYNC_OUT_SSMIIM
      RMII:
	      REFCLK_OUT_RMIIP
	      REFCLK_OUT_RMIIM
	      SYNCCLK_OUT_RMIIP
	      SYNCCLK_OUT_RMIIM
*/
   DSL_DEV_MiiClockChain_t nMiiClockChain;
   /** TC Mode: MAC or virtual PHY */
   DSL_DEV_TcMode_t nTcMode;
   /** MII Interface Mode */
   DSL_DEV_MiiMode_t nMiiMode;
   /** Operating modes of the physical line:
      Selection of VDSL over POTS and similar
      can be done here */
   DSL_DEV_LineMode_t nLineMode;
   /** Device Hybrid*/
   DSL_DEV_Hybrid_t nHybrid;
   /** Base Address of the VINAX device */
   DSL_uint32_t nBaseAddr;
   /** IRQ of the VINAX device */
   DSL_int8_t nIrqNum;
   /** UTOPIA parity*/
   DSL_Parity_t nUtopiaParity;
   /** UTOPIA PHY address*/
   DSL_uint8_t nUtopiaPhyAdr;
   /** POSPHY parity*/
   DSL_Parity_t nPosPhyParity;
   /** POSPHY address*/
   DSL_uint8_t nPosPhyAdr;
  /** Chunk size in which packets will be transported
      over the POS-PHY interface. */
   DSL_int32_t    nPosphyChunkSize;
   /**
      Structure to return the current type of firmware request.
      It defines which kind of firmware is needed for the next reboot sequence.
   */
   DSL_FirmwareRequestType_t nFwType;
} DSL_DeviceLowLevelConfig_t;


/**
   Device configuration
*/
struct DSL_DeviceConfig
{
   /**
      Low level device configuration structure includes specific
      settings*/
   DSL_DeviceLowLevelConfig_t cfg;
   /**
   System interface configuration*/
   DSL_SystemInterfaceConfigData_t sysCIF;
   /** MAC configuration settings*/
   DSL_EFM_MacConfigData_t MacCfg;
   /** SAR Configuration*/
   DSL_SAR_ConfigData_t SARcfg;
   /** Segmentation Table*/
   DSL_SAR_SegmentationTableData_t SegmentationTable;
   /** Reassembly Table*/
   DSL_SAR_ReassemblyTableData_t ReassemblyTable;
   /** Common channel configuration parameters
       (see chapter 7.3.2.1 of G.997.1), for the UPSTREAM and DOWNSTREM direction
       Only for the bearer channel 0*/
   DSL_G997_ChannelConfigData_t ChannelConfigData[2];
   /** Actual selected Band Plan/Profile*/
   DSL_BandPlanConfigData_t bandPlanStatusNe;
};


/** @} DRV_DSL_DEVICE */

#ifdef __cplusplus
}
#endif

#endif /* DRV_DSL_CPE_VINAX_CTX_H */


