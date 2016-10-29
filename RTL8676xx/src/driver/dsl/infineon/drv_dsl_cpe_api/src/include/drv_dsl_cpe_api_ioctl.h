/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_API_IOCTL_H
#define _DRV_DSL_CPE_API_IOCTL_H

#ifdef __cplusplus
   extern "C" {
#endif

#ifdef LINUX
#include <asm/ioctl.h>
#endif

#include "drv_dsl_cpe_api.h"
#include "drv_dsl_cpe_api_pm.h"

#ifdef INCLUDE_DSL_ADSL_MIB
   /**
      Specifies to include ADSL Line Extension MIB (RFC3440).
      \ note The Line Extension MIB will be automatically included if ADSL MIB
             feature is included (no possibility to include RFC2662 only at the
             moment) */
   #define INCLUDE_ADSL_MIB_RFC3440

   #ifndef DSL_DOC_GENERATION_EXCLUDE_ADSL_MIB
      #include "drv_dsl_cpe_api_adslmib.h"
      #include "drv_dsl_cpe_api_adslmib_ioctl.h"
   #endif /* DSL_DOC_GENERATION_EXCLUDE_ADSL_MIB */
#endif

/** \file
   This file specifies all ioctls that could be used by an DSL Management
   application for configuration and control of the DSL CPE API.
*/

#ifndef DSL_CPE_API_USE_KERNEL_INTERFACE

/* ************************************************************************** */
/* *** ioctl structures                                                   *** */
/* ************************************************************************** */

/** encapsulate all ioctl command arguments */
typedef union
{
   /* special member */
   DSL_AccessCtl_t                   accessCtl;

   DSL_Init_t                        init;
   DSL_AutobootLoadFirmware_t        autobootLoadFirmware;
   DSL_AutobootControl_t             autobootControl;
   DSL_AutobootConfig_t              autobootConfig;
   DSL_VersionInformation_t          versionInformation;
   DSL_LineState_t                   lineState;
   DSL_LineFeature_t                 lineFeature;
   DSL_FramingParameterStatus_t      framingParameterStatus;
   DSL_SystemInterfaceConfig_t       systemInterfaceConfig;
   DSL_SystemInterfaceStatus_t       systemInterfaceStatus;

   DSL_DBG_ModuleLevel_t             dbg_moduleLevel;
   DSL_ShowtimeLogging_t             showtimeLogging;

   DSL_EventStatus_t                 event;
   DSL_EventStatusMask_t             eventStatusMask;
   DSL_InstanceControl_t             instanceControl;
   DSL_InstanceStatus_t              instanceStatus;

   DSL_DeviceMessage_t               dbgMsg;
#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   DSL_LinePathCounterTotal_t        linePathCountersTotal;
   DSL_LowLevelConfiguration_t       lowLevelConfig;
   DSL_AuxLineInventory_t            auxLineInventory;
   DSL_BandPlanSupport_t             bandPlanSupport;
   DSL_BandPlanStatus_t              bandPlanStatus;
   DSL_PhyAddressConfig_t            utopiaPhyAddress;
   DSL_EFM_MacConfig_t               efmMacConfig;
   /** SAR args*/
   DSL_SAR_Config_t                  sarConfig;
   DSL_SAR_SegmentationTable_t       sarSegmentationTable;
   DSL_SAR_ReassemblyTable_t         sarReassemblyTable;
   DSL_SAR_SegmentationCounters_t    sarSegmentationCounters;
   DSL_SAR_ReassemblyCounters_t      sarReassemblyCounters;
#endif /* #if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)*/

   /* G997 args */
   DSL_G997_PowerManagementStateForcedTrigger_t g997PowerMgmtStateForcedTrigger;
   DSL_G997_PowerManagementStatus_t    g997PowerMgmtStatus;
   DSL_G997_FramingParameterStatus_t   g997FramingParamsStatus;
   DSL_G997_XTUSystemEnabling_t        g997XtuSystemEnabling;
   DSL_G997_LineFailures_t             g997LineFailures;
   DSL_G997_LineActivate_t             g997lineActivateConfig;
   DSL_G997_LineInventory_t            g997LineInvertory;
   DSL_G997_LineInventoryNe_t          g997LineInvertoryNe;
   DSL_G997_LineStatus_t               g997LineStatus;
   DSL_G997_LineInitStatus_t           g997LineInitStatus;
   DSL_G997_LineTransmissionStatus_t   g997LineTransmissionStatus;
   DSL_G997_LastStateTransmitted_t     g997LastStateTransmitted;
   DSL_G997_ChannelDataRateThreshold_t g997ChannelDataRateThreshold;
#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   DSL_G997_LineStatusPerBand_t        g997LineStatusPerBand;
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */
   DSL_G997_ChannelStatus_t            g997ChannelStatus;
   DSL_G997_DataPathFailures_t         g997DataPathFailures;
   DSL_G997_BitAllocationNsc_t         g997BitAllocationNsc;
   DSL_G997_SnrAllocationNsc_t         g997SnrAllocationNsc;
   DSL_G997_GainAllocationNsc_t        g997GainAllocationNsc;
   DSL_G997_DeltHlin_t                 g997DeltHlin;
   DSL_G997_DeltHlinScale_t            g997DeltHlinScale;
   DSL_G997_DeltHlog_t                 g997DeltHlog;
   DSL_G997_DeltQln_t                  g997DeltQln;
#if defined(INCLUDE_DSL_CEOC)
   DSL_G997_Snmp_t                     g997SnmpMsg;
#endif

/* PM args */
#if defined(INCLUDE_DSL_PM)
#ifdef INCLUDE_DSL_CPE_PM_HISTORY
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   DSL_PM_ElapsedTimeReset_t        pmElapsedTimeReset;
   DSL_PM_BurninMode_t              pmBurninMode;
   DSL_PM_SyncMode_t                pmSyncMode;
   DSL_PM_ElapsedExtTrigger_t       pmExtTrigger;
   #endif /** #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
   DSL_PM_Reset_t                   pmReset;
#endif /** #ifdef INCLUDE_DSL_CPE_PM_HISTORY*/

#ifdef INCLUDE_DSL_CPE_PM_CHANNEL_COUNTERS
   DSL_PM_ChannelCounters_t         pmChannelCounters;
   #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
   DSL_PM_ChannelCountersTotal_t    pmChannelCountersTotal;
   #endif
   #ifdef INCLUDE_DSL_CPE_PM_HISTORY
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   DSL_PM_HistoryStatsChDir_t       pmChannelHistoryStats;
   #ifdef INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS
   DSL_PM_ChannelThreshold_t        pmChannelThresholds;
   #endif /** #ifdef INCLUDE_DSL_CPE_PM_CHANNEL_THRESHOLDS*/
   #endif /** #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
   #endif /** #ifdef INCLUDE_DSL_CPE_PM_HISTORY*/
#endif

#ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_COUNTERS
   DSL_PM_DataPathCounters_t        pmDataPathCounters;
   #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
   DSL_PM_DataPathCountersTotal_t   pmDataPathCountersTotal;
   #endif
   #ifdef INCLUDE_DSL_CPE_PM_HISTORY
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   DSL_PM_HistoryStatsChDir_t       pmDataPathHistoryStats;
   #ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS
   DSL_PM_DataPathThreshold_t       pmDataPathThresholds;
   #endif /** #ifdef INCLUDE_DSL_CPE_PM_DATA_PATH_THRESHOLDS*/
   #endif /** #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
   #endif /** #ifdef INCLUDE_DSL_CPE_PM_HISTORY*/
#endif

#ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS
   DSL_PM_LineSecCounters_t         pmLineSecCounters;
   #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS
   DSL_PM_LineSecCountersTotal_t    pmLineSecCountersTotal;
   DSL_PM_LineInitCountersTotal_t   pmLineInitCountersTotal;
   #endif /** #ifdef INCLUDE_DSL_CPE_PM_TOTAL_COUNTERS*/

   #ifdef INCLUDE_DSL_CPE_PM_HISTORY
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   DSL_PM_HistoryStatsDir_t         pmLineSecHistoryStats;
   DSL_PM_HistoryStats_t            pmLineInitHistoryStats;
   #endif /* #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
   DSL_PM_LineInitCounters_t        pmLineInitCounters;
   #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS
   #ifdef INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS
   DSL_PM_LineSecThreshold_t        pmLineSecThresholds;
   DSL_PM_LineInitThreshold_t       pmLineInitThresholds;
   #endif /** #ifdef INCLUDE_DSL_CPE_PM_LINE_THRESHOLDS*/
   #endif /** #ifdef INCLUDE_DSL_CPE_PM_OPTIONAL_PARAMETERS*/
   #endif /** #ifdef INCLUDE_DSL_CPE_PM_HISTORY*/
#endif /** #ifdef INCLUDE_DSL_CPE_PM_LINE_COUNTERS*/
#endif
} DSL_IOCTL_arg_t;


/* ************************************************************************** */
/* *** ioctl commands                                                     *** */
/* ************************************************************************** */

/** Magic number for DSL CPE_API ioctls */
#define DSL_IOC_MAGIC_CPE_API    'r'

/** Magic number for MIB related ioctls of DSL CPE_API */
#define DSL_IOC_MAGIC_MIB        'm'

/**
   Defines all common ioctls.
   \remarks All ioctls shall be decoded using macros
            - _IOC_DIR
            - _IOC_TYPE,
            - _IOC_NR
            - _IOC_SIZE.
            If _IOC_SIZE returns a value >0 this is the size that can be used
            for copying the data between user and kernel space.
            If  _IOC_SIZE return 0 a special handling is necessary because the
            data size may exceed the max. size of 256 byte. */
#define DSL_IOC_BASE_CPE_API_COMMON          0
/**
   Defines all G.997.1 related ioctls. */
#define DSL_IOC_BASE_CPE_API_SAR             55
/**
   Defines all G.997.1 related ioctls. */
#define DSL_IOC_BASE_CPE_API_G997            70
/**
   Defines all PM related ioctls. */
#define DSL_IOC_BASE_CPE_API_PM              140

/**
   Defines all MIB related ioctls.
   \remarks Only one MIB can be included at one time (ADSL1, ADSL2/2+, VDSL) */
#define DSL_IOC_BASE_MIB                  0


/* ************************************************************************** */
/* * Ioctl interface definitions for common functions                       * */
/* ************************************************************************** */

/**
   This ioctl initializes the DSL CPE_API.

   \param DSL_Init_t*
      The parameter points to a \ref DSL_Init_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_Init_t init;

      memset(&init, 0x00, sizeof(DSL_Init_t));
      init. = ;
      ret = ioctl(fd, DSL_FIO_INIT, &init)
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define DSL_FIO_INIT \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 0), DSL_Init_t)

/**
   This ioctl downloads a firmware.

   \param DSL_AutobootLoadFirmware_t*
      The parameter points to a \ref DSL_AutobootLoadFirmware_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_AutobootLoadFirmware_t fw;

      memset(&fw, 0x00, sizeof(DSL_AutobootLoadFirmware_t));
      fw. = ;
      ret = ioctl(fd, DSL_FIO_AUTOBOOT_LOAD_FIRMWARE, &fw)
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define DSL_FIO_AUTOBOOT_LOAD_FIRMWARE \
   _IOWR( DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 1), DSL_AutobootLoadFirmware_t )

/**
   This ioctl makes it possible to control the autoboot handling.

   \param DSL_AutobootControl_t*
      The parameter points to a \ref DSL_AutobootControl_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_AutobootControl_t ctrl;

      memset(&ctrl, 0x00, sizeof(DSL_AutobootControl_t));
      ctrl. = ;
      ret = ioctl(fd, DSL_FIO_AUTOBOOT_CONTROL_SET, &ctrl)
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define DSL_FIO_AUTOBOOT_CONTROL_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 2), DSL_AutobootControl_t)

/**
   This ioctl makes it possible to get the current state of the autoboot
   handling.

   \param DSL_AutobootStatus_t*
      The parameter points to a \ref DSL_AutobootStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_AutobootStatus_t status;

      memset(&status, 0x00, sizeof(DSL_AutobootStatus_t));
      ret = ioctl(fd, DSL_FIO_AUTOBOOT_STATUS_GET, &status)
   \endcode

   \ingroup DRV_DSL_CPE_INIT */
#define DSL_FIO_AUTOBOOT_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 3), DSL_AutobootStatus_t)

/**
   This ioctl requests the version information of all used DSL software and
   hardware components below Software CPE_API level.

   \param DSL_VersionInformation_t*
      The parameter points to a \ref DSL_VersionInformation_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_VersionInformation_t version;

      memset(&version, 0x00, sizeof(DSL_VersionInformation_t));
      ret = ioctl(fd, DSL_FIO_VERSION_INFORMATION_GET, &version)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_VERSION_INFORMATION_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 4), DSL_VersionInformation_t)

/**
   This ioctl requests information about the current line state.

   \param DSL_LineState_t*
      The parameter points to a \ref DSL_LineState_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_LineState_t lineState;

      memset(&lineState, 0x00, sizeof(DSL_LineState_t));
      ret = ioctl(fd, DSL_FIO_LINE_STATE_GET, &lineState)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LINE_STATE_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 5), DSL_LineState_t)

/**
   This ioctl configures various common line specific features.

   \param DSL_LineFeature_t*
      The parameter points to a \ref DSL_LineFeature_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that a change of this configuration setting(s) will be only
   effective after restarting the autoboot handling using ioctl
   \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_LineFeature_t lineFeatureConfig;

      memset(&lineFeatureConfig, 0x00, sizeof(DSL_LineFeature_t));
      ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_SET, &lineFeatureConfig)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LINE_FEATURE_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 6), DSL_LineFeature_t)

/**
   This ioctl returns the current configurations for various common line specific
   features.

   \param DSL_LineFeature_t*
      The parameter points to a \ref DSL_LineFeature_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_LineFeature_t lineFeatureConfig;

      memset(&lineFeatureConfig, 0x00, sizeof(DSL_LineFeature_t));
      ret = ioctl(fd, DSL_FIO_LINE_FEATURE_CONFIG_GET, &lineFeatureConfig)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LINE_FEATURE_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 7), DSL_LineFeature_t)

/**
   This ioctl returns the current status of various common line specific features.

   \param DSL_LineFeature_t*
      The parameter points to a \ref DSL_LineFeature_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that the state of this status values are only updated if showtime
   is reached otherwise they might reflect inconsistent states.
   To check if the line is in showtime please use ioctl
   \ref DSL_FIO_LINE_STATE_GET before using this functionality if you are not
   sure about the line state.
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_LineFeatureStatusGet_t lineFeatureConfig;

      memset(&lineFeatureConfig, 0x00, sizeof(DSL_LineFeature_t));
      ret = ioctl(fd, DSL_FIO_LINE_FEATURE_STATUS_GET, &lineFeatureConfig)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LINE_FEATURE_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 8), DSL_LineFeature_t)

/**
   This ioctl requests information about the current status for various common
   framing parameters.

   \param DSL_FramingParameterStatus_t*
      The parameter points to a \ref DSL_FramingParameterStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that the state of this status values are only updated if showtime
   is reached otherwise they might reflect inconsistent values.
   To check if the line is in showtime please use ioctl
   \ref DSL_FIO_LINE_STATE_GET before using this functionality if you are not
   sure about the line state.
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_FramingParameterStatus_t framingParam;

      memset(&framingParam, 0x00, sizeof(DSL_LineFeatureStatusGet_t));
      ret = ioctl(fd, DSL_FIO_FRAMING_PARAMETER_STATUS_GET, &framingParam)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_FRAMING_PARAMETER_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 9), DSL_FramingParameterStatus_t)

/**
   This debug function sets a the debug level of the given module number.
   \note Implementation only available if preprocessor directive
         'DSL_DEBUG_DISABLE' is not set.

   \param DSL_DBG_ModuleLevel_t*
      The parameter points to a \ref DSL_DBG_ModuleLevel_t structure
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \return
      0 if successful and -1 in case of an error/warning

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_DBG_ModuleLevel_t dbgModuleLevel;

      memset(&dbgModuleLevel, 0x00, sizeof(DSL_DBG_ModuleLevel_t));
      ret = ioctl(fd, DSL_FIO_DBG_MODULE_LEVEL_SET, &dbgModuleLevel)
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG */
#define DSL_FIO_DBG_MODULE_LEVEL_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 10), DSL_DBG_ModuleLevel_t)

/**
   This debug function returns the debug level from the given module number.
   \note Implementation only available if preprocessor directive
         'DSL_DEBUG_DISABLE' is not set.

   \param DSL_DBG_ModuleLevel_t*
      The parameter points to a \ref DSL_DBG_ModuleLevel_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_DBG_ModuleLevel_t dbgModuleLevel;

      memset(&dbgModuleLevel, 0x00, sizeof(DSL_DBG_ModuleLevel_t));
      ret = ioctl(fd, DSL_FIO_DBG_MODULE_LEVEL_GET, &dbgModuleLevel)
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG */
#define DSL_FIO_DBG_MODULE_LEVEL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 11), DSL_DBG_ModuleLevel_t)

/**
   This debug function makes it possible to send a low level device message.
   \note Implementation only available if preprocessor directive
         'DSL_DEBUG_DISABLE' is not set.

   \param DSL_DeviceMessage_t*
      The parameter points to a \ref DSL_DeviceMessage_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_DeviceMessage_t devMsg;

      memset(&devMsg, 0x00, sizeof(DSL_DeviceMessage_t));
      // Initialize the device message here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_DBG_DEVICE_MESSAGE_SEND, &devMsg)
   \endcode

   \ingroup DRV_DSL_CPE_DEBUG */
#define DSL_FIO_DBG_DEVICE_MESSAGE_SEND \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 12), DSL_DeviceMessage_t)

/**
   This function provides the possibility to activate a testmode.

   \param DSL_TestModeControl_t*
      The parameter points to a \ref DSL_TestModeControl_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_TestModeControl_t testModeCtrl;

      memset(&testModeCtrl, 0x00, sizeof(DSL_TestModeControl_t));
      // Set testmode here before calling the ioctl...
      testModeCtrl.data,nTestMode = DSL_TESTMODE_SHOWTIME_LOCK;
      ret = ioctl(fd, DSL_FIO_TEST_MODE_CONTROL_SET, &testModeCtrl)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_TEST_MODE_CONTROL_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 13), DSL_TestModeControl_t)

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function retrieves the auxiliary line inventory information according
   to G.993.2 (see chapter 11.2.3 EOC commands and responses) directly from the
   device/CO. As this field is defined without a limit in size, it is not stored
   inside the DSL CPE API.
   The auxiliary inventory includes the VTU equipment ID auxiliary inventory
   information and self-test results.
   To set this information use \ref DSL_FIO_INIT or
   \ref DSL_FIO_G997_LINE_INVENTORY_SET.
   \note For Vinax the far end information is available when the callback event
         \ref DSL_EVENT_S_FE_INVENTORY_AVAILABLE was indicated.

   \param DSL_AuxLineInventory_t*
      The parameter points to a \ref DSL_AuxLineInventory_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_AuxLineInventory_t auxLineInventory;

      memset(&auxLineInventory, 0x00, sizeof(DSL_AuxLineInventory_t));
      ret = ioctl(fd, DSL_FIO_AUX_LINE_INVENTORY_GET, &auxLineInventory)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_AUX_LINE_INVENTORY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 14), DSL_AuxLineInventory_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function returns the measured loop length for different line types
   in meters.
   The implementation of this function is device specific!

   \param DSL_LoopLengthStatus_t*
      The parameter points to a \ref DSL_LoopLengthStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_LoopLengthStatus_t loopLength;

      memset(&loopLength, 0x00, sizeof(DSL_LoopLengthStatus_t));
      ret = ioctl(fd, DSL_FIO_LOOP_LENGTH_STATUS_GET, &loopLength)
      // Read and/or process results here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LOOP_LENGTH_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 15), DSL_LoopLengthStatus_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   This function writes the UTOPIA address configuration settings.

   \param DSL_PhyAddressConfig_t*
      The parameter points to a \ref DSL_PhyAddressConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PhyAddressConfig_t phyCfg;

      memset(&phyCfg, 0x00, sizeof(DSL_PhyAddressConfig_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_UTOPIA_ADDRESS_CONFIG_SET, &phyCfg)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_UTOPIA_ADDRESS_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 16), DSL_PhyAddressConfig_t)

/**
   This function reads the UTOPIA address configuration settings from the
   DSL CPE API internal configuration memory.

   \param DSL_PhyAddressConfig_t*
      The parameter points to a \ref DSL_PhyAddressConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PhyAddressConfig_t phyCfg;

      memset(&phyCfg, 0x00, sizeof(DSL_PhyAddressConfig_t));
      ret = ioctl(fd, DSL_FIO_UTOPIA_ADDRESS_CONFIG_GET, &phyCfg)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_UTOPIA_ADDRESS_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 17), DSL_PhyAddressConfig_t)

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function writes the POSPHY address configuration settings.
   The POSPHY Address assignment will be written
   automatically before writing the G.997 configuration settings if
   activating a line.

   \param DSL_PhyAddressConfig_t*
      The parameter points to a \ref DSL_PhyAddressConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_PhyAddressConfig_t phyCfg;

      memset(&phyCfg, 0x00, sizeof(DSL_PhyAddressConfig_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_POSPHY_ADDRESS_CONFIG_SET, &phyCfg)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_POSPHY_ADDRESS_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 18), DSL_PhyAddressConfig_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function reads the POSPHY address configuration settings from the
   DSL CPE API internal configuration memory.

   \param DSL_PhyAddressConfig_t*
      The parameter points to a \ref DSL_PhyAddressConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_PhyAddressConfig_t phyCfg;

      memset(&phyCfg, 0x00, sizeof(DSL_PhyAddressConfig_t));
      ret = ioctl(fd, DSL_FIO_POSPHY_ADDRESS_CONFIG_GET, &phyCfg)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_POSPHY_ADDRESS_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 19), DSL_PhyAddressConfig_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   This function configures the System Interface.

   \param DSL_SystemInterfaceConfig_t*
      The parameter points to a \ref DSL_SystemInterfaceConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_SystemInterfaceConfig_t ifCfg;

      memset(&ifCfg, 0x00, sizeof(DSL_SystemInterfaceConfig_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET, &ifCfg)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 20), DSL_SystemInterfaceConfig_t)

/**
   This function returns the selected System Interface mode.

   \param DSL_SystemInterfaceConfig_t*
      The parameter points to a \ref DSL_SystemInterfaceConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PhyAddressConfig_t ifCfg;

      memset(&ifCfg, 0x00, sizeof(DSL_SystemInterfaceConfig_t));
      ret = ioctl(fd, DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET, &ifCfg)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 21), DSL_SystemInterfaceConfig_t)

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function reads back the selected Bandplan.

   \param DSL_BandPlanStatus_t*
      The parameter points to a \ref DSL_BandPlanStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_BandPlanStatus_t bpCfg;

      memset(&bpCfg, 0x00, sizeof(DSL_BandPlanStatus_t));
      ret = ioctl(fd, DSL_FIO_BAND_PLAN_STATUS_GET, &bpCfg)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_BAND_PLAN_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 22), DSL_BandPlanStatus_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function returns all VDSL2 bandplan/profile combinations supported by
   the device.

   \param DSL_BandPlanSupport_t*
      The parameter points to a \ref DSL_BandPlanSupport_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_BandPlanSupport_t bpSupport;

      memset(&bpSupport, 0x00, sizeof(DSL_BandPlanSupport_t));
      ret = ioctl(fd, DSL_FIO_BAND_PLAN_SUPPORT_GET, &bpSupport)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_BAND_PLAN_SUPPORT_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 23), DSL_BandPlanSupport_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function returns all VDSL2 bandplan/profile combinations supported by
   the device.

   \param DSL_EFM_MacConfig_t*
      The parameter points to a \ref DSL_EFM_MacConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_EFM_MacConfig_t macCfg;

      memset(&macCfg, 0x00, sizeof(DSL_EFM_MacConfig_t));
      ret = ioctl(fd, DSL_FIO_EFM_MAC_CONFIG_SET, &macCfg)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_EFM_MAC_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 24), DSL_EFM_MacConfig_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function returns all VDSL2 bandplan/profile combinations supported by
   the device.

   \param DSL_EFM_MacConfig_t*
      The parameter points to a \ref DSL_EFM_MacConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_EFM_MacConfig_t macCfg;

      memset(&macCfg, 0x00, sizeof(DSL_EFM_MacConfig_t));
      ret = ioctl(fd, DSL_FIO_EFM_MAC_CONFIG_GET, &macCfg)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_EFM_MAC_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 25), DSL_EFM_MacConfig_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   This ioctl reads the showtime event logging data.

   \param DSL_ShowtimeLogging_t*
      The parameter points to a \ref DSL_ShowtimeLogging_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_ShowtimeLogging_t data;

      memset(&data, 0x00, sizeof(DSL_ShowtimeLogging_t));
      fw. = ;
      ret = ioctl(fd, DSL_FIO_SHOWTIME_LOGGING_DATA_GET, &data)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_SHOWTIME_LOGGING_DATA_GET \
   _IOWR( DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 26), DSL_ShowtimeLogging_t )



/**
   This function configures the mask for one given the status event.
   For a description on status events please refer to the description of
   \ref DSL_EventType_t.

   \param DSL_EventStatusMask_t*
      The parameter points to a \ref DSL_EventStatusMask_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \note
   The function call provides the possibility to set a line specific mask for a
   given event type.
   If events are direction specific both directions are set together.
   Some events does not provide to possibility to set this mask line specific.
   Therefore please take the following restrictions into account.
   DSL_CBS_INIT_READY
      This is a common event which is available one ONCE per DSL API instance.
      If this setting will be changed for one line the settings from ALL other
      available lines will be set automatically to the same value.
   DSL_CBS_SYSTEM_STATUS
      This is a device specific event which is available one per device.
      If this setting will be changed for one line the settings from ALL other
      lines from the same device will be set automatically to the same value.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_EventStatusMask_t statusEventMaskCfgSet;

      memset(&statusEventMaskCfgSet, 0x00, sizeof(DSL_EventStatusMask_t));
      ret = ioctl(fd, DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET, &statusEventMaskCfgSet)
   \endcode

   \ingroup DRV_DSL_CPE_EVENT */
#define DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 27), DSL_EventStatusMask_t)

/**
   This function returns current configuration of the mask for one given status
   event. For a description on status events please refer to the description of
   \ref DSL_EventType_t.

   \param DSL_EventStatusMask_t*
      The parameter points to a \ref DSL_EventStatusMask_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_EventStatusMask_t statusEventMaskCfgGet;

      memset(&statusEventMaskCfgGet, 0x00, sizeof(DSL_EventStatusMask_t));
      ret = ioctl(fd, DSL_FIO_EVENT_STATUS_MASK_CONFIG_GET, &statusEventMaskCfgGet)
   \endcode

   \ingroup DRV_DSL_CPE_EVENT */
#define DSL_FIO_EVENT_STATUS_MASK_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 28), DSL_EventStatusMask_t)

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This ioctl gets VINAX device specific low level configuration.

   \param DSL_DeviceLowLevelConfig_t*
      The parameter points to a \ref DSL_LowLevelConfiguration_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_LowLevelConfiguration_t data;

      memset(&data, 0x00, sizeof(DSL_LowLevelConfiguration_t));
      data. = ;
      ret = ioctl(fd, DSL_FIO_LOW_LEVEL_CONFIGURATION_GET, &data)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LOW_LEVEL_CONFIGURATION_GET \
    _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 29), DSL_LowLevelConfiguration_t)

/**
   This ioctl sets VINAX device specific low level configuration.

   \param DSL_LowLevelConfiguration_t*
      The parameter points to a \ref DSL_LowLevelConfiguration_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_LowLevelConfiguration_t data;

      memset(&data, 0x00, sizeof(DSL_LowLevelConfiguration_t));
      data. = ;
      ret = ioctl(fd, DSL_FIO_LOW_LEVEL_CONFIGURATION_GET, &data)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LOW_LEVEL_CONFIGURATION_SET \
    _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 30), DSL_DeviceLowLevelConfig_t)
#endif /** #if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)*/

/**
   This function provides the possibility to get the status for the currently
   activated testmode.

   \param DSL_TestModeStatus_t*
      The parameter points to a \ref DSL_TestModeStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_TestModeStatus_t testModeStatus;

      memset(&testModeStatus, 0x00, sizeof(DSL_TestModeStatus_t));
      ret = ioctl(fd, DSL_FIO_TEST_MODE_STATUS_GET, &testModeStatus)
      // Read and/or process testmode here...
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_TEST_MODE_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 31), DSL_TestModeStatus_t)

/**
   This function returns the currently activated System Interface mode.

   \param DSL_SystemInterfaceStatus_t*
      The parameter points to a \ref DSL_SystemInterfaceStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE

   \code
      DSL_SystemInterfaceStatus_t systemInterfaceStatus;

      memset(&systemInterfaceStatus, 0x00, sizeof(DSL_SystemInterfaceStatus_t));
      ret = ioctl(fd, DSL_FIO_SYSTEM_INTERFACE_STATUS_GET, &systemInterfaceStatus)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_SYSTEM_INTERFACE_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 32), DSL_SystemInterfaceStatus_t)

/**
   This function has to be used to get information on an event that has been
   occurred. The function is designed to be called by the upper layer software
   in case of using event (interrupt) handling and in case of a wakeup of the
   upper layer software by the DSL CPE API.
   There is always an event identifier included within the returned information.
   According the event type there might be also additional event data included.

   \param DSL_EventStatus_t*
      The parameter points to a \ref DSL_EventStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_EventStatus_t eventStatus;

      memset(&eventStatus, 0x00, sizeof(DSL_EventStatus_t));
      ret = ioctl(fd, DSL_FIO_EVENT_STATUS_GET, &eventStatus)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_EVENT_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 33), DSL_EventStatus_t)

/**
   This function has to be used to initialize instance specific handling.
   Usually this function shall be called directly after using \ref DSL_FIO_INIT
   ioctl.
   \note After opening an instance (and using \ref DSL_FIO_INIT ioctl) the
         default configuration of instance control parameters is as follows
         - event handling is OFF
         - NO additional event specific resources are allocated
         This configuration ensures that a minimum of runtime memory is used for
         according functionality.

   \param DSL_InstanceControl_t*
      The parameter points to a \ref DSL_InstanceControl_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_InstanceControl_t instanceControl;

      memset(&instanceControl, 0x00, sizeof(DSL_InstanceControl_t));
      instanceControl.data.bEventActivation = DSL_TRUE;
      instanceControl.data.nResourceActivationMask = DSL_RESOURCE_ACTIVATION_SNMP;
      ret = ioctl(fd, DSL_FIO_INSTANCE_CONTROL_SET, &instanceControl)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_INSTANCE_CONTROL_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 34), \
      DSL_InstanceControl_t)

/**
   This function has to be used to get information about current instance
   specific configurations.

   \param DSL_InstanceStatus_t*
      The parameter points to a \ref DSL_InstanceStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_InstanceStatus_t instanceStatus;

      memset(&instanceStatus, 0x00, sizeof(DSL_InstanceStatus_t));
      ret = ioctl(fd, DSL_FIO_EVENT_ACTIVATION_STATUS_GET, &instanceStatus)
      // instanceStatus.data... includes current status
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_INSTANCE_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 35), \
      DSL_InstanceStatus_t)

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function reads several data path counters since system startup.
   \note For Vinax the counters are related to the System Interface.

   \param DSL_LinePathCounterTotal_t*
      The parameter points to a \ref DSL_LinePathCounterTotal_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_LinePathCounterTotal_t linePathCounter;

      memset(&linePathCounter, 0x00, sizeof(DSL_LinePathCounterTotal_t));
      ret = ioctl(fd, DSL_FIO_LINE_PATH_COUNTER_TOTAL_GET, &linePathCounter)
      // linePathCounter includes counter values to process
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_LINE_PATH_COUNTER_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 36), \
      DSL_LinePathCounterTotal_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   This function has to be used to get the autoboot configuration besides the
   init configuration at runtime

   \param DSL_AutobootConfig_t*
      The parameter points to a \ref DSL_AutobootConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_AutobootConfig_t autobootConfig;

      memset(&autobootConfig, 0x00, sizeof(DSL_AutobootConfig_t));
      ret = ioctl(fd, DSL_FIO_AUTOBOOT_CONFIG_SET, &autobootConfig)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_AUTOBOOT_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 37), \
      DSL_AutobootConfig_t)

/**
   This function has to be used to change the autoboot configuration besides the
   init configuration at runtime

   \param DSL_AutobootConfig_t*
      The parameter points to a \ref DSL_AutobootConfig_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_AutobootConfig_t autobootConfig;

      memset(&autobootConfig, 0x00, sizeof(DSL_AutobootConfig_t));
      autobootConfig.data.nStateMachineOptions = DSL_TRUE;
      ret = ioctl(fd, DSL_FIO_AUTOBOOT_CONFIG_GET, &autobootConfig)
   \endcode

   \ingroup DRV_DSL_CPE_COMMON */
#define DSL_FIO_AUTOBOOT_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_COMMON + 38), \
      DSL_AutobootConfig_t)


/* ************************************************************************** */
/* * Ioctl interface definitions for SAR                                    * */
/* ************************************************************************** */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function writes the configuration parameter for the SAR
   to the DSL CPE API internal memory.

   \param DSL_SAR_Config_t*
      The parameter points to a \ref DSL_SAR_Config_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_SAR_Config_t sarCfg;

      memset(&sarCfg, 0x00, sizeof(DSL_SAR_Config_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_SAR_CONFIG_SET, &sarCfg)
   \endcode

   \ingroup DRV_DSL_CPE_SAR */
#define DSL_FIO_SAR_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_SAR + 0), DSL_SAR_Config_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function reads the configuration parameter for the SAR from the
   DSL CPE API internal memory.

   \param DSL_SAR_Config_t*
      The parameter points to a \ref DSL_SAR_Config_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_SAR_Config_t sarCfg;

      memset(&sarCfg, 0x00, sizeof(DSL_SAR_Config_t));
      ret = ioctl(fd, DSL_FIO_SAR_CONFIG_GET, &sarCfg)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_SAR */
#define DSL_FIO_SAR_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_SAR + 1), DSL_SAR_Config_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function writes the SAR Segmentation Forwarding table to the
   DSL CPE API internal memory.

   \param DSL_SAR_SegmentationTable_t*
      The parameter points to a \ref DSL_SAR_SegmentationTable_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_SAR_SegmentationTable_t sarSegTab;

      memset(&sarSegTab, 0x00, sizeof(DSL_SAR_SegmentationTable_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_SAR_SEGMENTATION_TABLE_SET, &sarSegTab)
   \endcode

   \ingroup DRV_DSL_CPE_SAR */
#define DSL_FIO_SAR_SEGMENTATION_TABLE_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_SAR + 2), DSL_SAR_SegmentationTable_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function reads the SAR Segmentation Forwarding table from the
   DSL CPE API internal memory.

   \param DSL_SAR_SegmentationTable_t*
      The parameter points to a \ref DSL_SAR_SegmentationTable_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_SAR_SegmentationTable_t sarSegTab;

      memset(&sarCfg, 0x00, sizeof(DSL_SAR_SegmentationTable_t));
      ret = ioctl(fd, DSL_FIO_SAR_SEGMENTATION_TABLE_GET, &sarSegTab)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_SAR */
#define DSL_FIO_SAR_SEGMENTATION_TABLE_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_SAR + 3), DSL_SAR_SegmentationTable_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function writes the SAR Reassembly Forwarding table to the
   DSL CPE API internal memory.

   \param DSL_SAR_ReassemblyTable_t*
      The parameter points to a \ref DSL_SAR_ReassemblyTable_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_SAR_ReassemblyTable_t sarReasTab;

      memset(&sarReasTab, 0x00, sizeof(DSL_SAR_SegmentationTable_t));
      // Set configuration settings here before calling the ioctl...
      ret = ioctl(fd, DSL_FIO_SAR_SEGMENTATION_TABLE_SET, &sarReasTab)
   \endcode

   \ingroup DRV_DSL_CPE_SAR */
#define DSL_FIO_SAR_REASSEMBLY_TABLE_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_SAR + 4), DSL_SAR_ReassemblyTable_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function reads the SAR Reassembly Forwarding table from the
   DSL CPE API internal memory.

   \param DSL_SAR_ReassemblyTable_t*
      The parameter points to a \ref DSL_SAR_ReassemblyTable_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_SAR_ReassemblyTable_t sarReasTab;

      memset(&sarReasTab, 0x00, sizeof(DSL_SAR_SegmentationTable_t));
      ret = ioctl(fd, DSL_FIO_SAR_SEGMENTATION_TABLE_GET, &sarReasTab)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_SAR */
#define DSL_FIO_SAR_REASSEMBLY_TABLE_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_SAR + 5), DSL_SAR_ReassemblyTable_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function requests the current Segmentation-Counters
   of the AAL5 block (Packets on the system Interface <-> ATM TC-Layer).

   \param DSL_SAR_SegmentationCounters_t*
      The parameter points to a \ref DSL_SAR_SegmentationCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_SAR_SegmentationCounters_t sarSegCount;

      memset(&sarSegCount, 0x00, sizeof(DSL_SAR_SegmentationCounters_t));
      ret = ioctl(fd, DSL_FIO_SAR_SEGMENTATION_COUNTERS_GET, &sarSegCount)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_SAR */
#define DSL_FIO_SAR_SEGMENTATION_COUNTERS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_SAR + 6), DSL_SAR_SegmentationCounters_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function requests the current Reassembly-Counters
   of the device (Packets on the system Interface <-> ATM TC-Layer).

   \param DSL_SAR_ReassemblyCounters_t*
      The parameter points to a \ref DSL_SAR_ReassemblyCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_SAR_ReassemblyCounters_t sarReaCount;

      memset(&sarReaCount, 0x00, sizeof(DSL_SAR_ReassemblyCounters_t));
      ret = ioctl(fd, DSL_FIO_SAR_REASSEMBLY_COUNTERS_GET, &sarReaCount)
      // Read and/or process configuration settings here...
   \endcode

   \ingroup DRV_DSL_CPE_SAR */
#define DSL_FIO_SAR_REASSEMBLY_COUNTERS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_SAR + 7), DSL_SAR_ReassemblyCounters_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */


/* ************************************************************************** */
/* * Ioctl interface definitions for G.997.1                                * */
/* ************************************************************************** */

/**
   This function configures special line activation settings.
   \attention All configurations that are done within context of this function
              are persistent!
              This means in case of activating the diagnostic mode (DELT) for
              example this configuration will be used for consecutive line
              activations by the autoboot handling until this configuration
              option is disabled again.

   \param DSL_G997_LineActivate_t*
      The parameter points to a \ref DSL_G997_LineActivate_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that a change of this configuration setting(s) will be only
   effective after restarting the autoboot handling using ioctl
   \ref DSL_FIO_AUTOBOOT_CONTROL_SET with command \ref DSL_AUTOBOOT_CTRL_RESTART
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LineActivate_t lineActivate;

      memset(&lineActivate, 0x00, sizeof(DSL_G997_LineActivate_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_ACTIVATE_CONFIG_SET, &lineActivate)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_ACTIVATE_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 0), DSL_G997_LineActivate_t)

/**
   This function returns the current configuration for special line activation
   settings.

   \param DSL_G997_LineActivate_t*
      The parameter points to a \ref DSL_G997_LineActivate_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LineActivate_t lineActivate;

      memset(&lineActivate, 0x00, sizeof(DSL_G997_LineActivate_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_ACTIVATE_CONFIG_GET, &lineActivate)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_ACTIVATE_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 1), DSL_G997_LineActivate_t)

/**
   This function allows to change the configuration of the xTU transmission
   system modes that are supported by the CPE.
   It is usual to set any combination of bits that are allowed by the current
   hardware/firmware/software combination.

   \param DSL_G997_XTUSystemEnabling_t*
      The parameter points to a \ref DSL_G997_XTUSystemEnabling_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_XTUSystemEnabling_t xtuSystemEnabling;

      memset(&xtuSystemEnabling, 0x00, sizeof(DSL_G997_XTUSystemEnabling_t));
      ret = ioctl(fd, DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_SET, &xtuSystemEnabling)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 2), DSL_G997_XTUSystemEnabling_t)

/**
   This function returns the configuration of the xTU transmission system modes
   that are currently activated to be used by the CPE.

   \param DSL_G997_XTUSystemEnabling_t*
      The parameter points to a \ref DSL_G997_XTUSystemEnabling_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_XTUSystemEnabling_t xtuSystemEnabling;

      memset(&xtuSystemEnabling, 0x00, sizeof(DSL_G997_XTUSystemEnabling_t));
      ret = ioctl(fd, DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_GET, &xtuSystemEnabling)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 3), DSL_G997_XTUSystemEnabling_t)

/**
   This function returns the status of the currently used xTU transmission
   system mode. In opposite to the functionality provided by
   \ref DSL_FIO_G997_XTU_SYSTEM_ENABLING_CONFIG_SET only ONE bit will be set
   at a time.

   \param DSL_G997_XTUSystemEnabling_t*
      The parameter points to a \ref DSL_G997_XTUSystemEnabling_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that the state of this status values are only updated if showtime
   is reached otherwise they might reflect inconsistent states.
   To check if the line is in showtime please use ioctl
   \ref DSL_FIO_LINE_STATE_GET before using this functionality if you are not
   sure about the line state.
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_XTUSystemEnabling_t xtuSystemEnabling;

      memset(&xtuSystemEnabling, 0x00, sizeof(DSL_G997_XTUSystemEnabling_t));
      ret = ioctl(fd, DSL_FIO_G997_XTU_SYSTEM_ENABLING_STATUS_GET, &xtuSystemEnabling)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_XTU_SYSTEM_ENABLING_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 4), DSL_G997_XTUSystemEnabling_t)

/**
   This function allows to change the configuration of channel data rate
   thresholds.

   \param DSL_G997_ChannelDataRateThreshold_t*
      The parameter points to a \ref DSL_G997_ChannelDataRateThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_ChannelDataRateThreshold_t chDataRateThres;

      memset(&chDataRateThres, 0x00, sizeof(DSL_G997_ChannelDataRateThreshold_t));
      ret = ioctl(fd, DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_SET, &chDataRateThres)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 5), DSL_G997_ChannelDataRateThreshold_t)

/**
   This function returns the current configuration of channel data rate
   thresholds.

   \param DSL_G997_ChannelDataRateThreshold_t*
      The parameter points to a \ref DSL_G997_ChannelDataRateThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_ChannelDataRateThreshold_t chDataRateThres;

      memset(&chDataRateThres, 0x00, sizeof(DSL_G997_ChannelDataRateThreshold_t));
      ret = ioctl(fd, DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_GET, &chDataRateThres)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_CHANNEL_DATA_RATE_THRESHOLD_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 6), DSL_G997_ChannelDataRateThreshold_t)

/**
   This function returns the current status for the line transmission status.

   \param DSL_G997_LineTransmissionStatus_t*
      The parameter points to a \ref DSL_G997_LineTransmissionStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LineTransmissionStatus_t lineTransStatus;

      memset(&lineTransStatus, 0x00, sizeof(DSL_G997_LineTransmissionStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_TRANSMISSION_STATUS_GET, &lineTransStatus)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_TRANSMISSION_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 7), DSL_G997_LineTransmissionStatus_t)

/**
   This function returns the current status for the line initialization status.

   \param DSL_G997_LineInitStatus_t*
      The parameter points to a \ref DSL_G997_LineInitStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LineInitStatus_t lineInitStatus;

      memset(&lineTransStatusGet, 0x00, sizeof(DSL_G997_LineInitStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_INIT_STATUS_GET, &lineInitStatus)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_INIT_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 8), DSL_G997_LineInitStatus_t)

/**
   This function returns the current status for the line.

   \param DSL_G997_LineStatus_t*
      The parameter points to a \ref DSL_G997_LineStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LineStatus_t lineStatus;

      memset(&lineStatus, 0x00, sizeof(DSL_G997_LineStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_STATUS_GET, &lineStatus)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 9), DSL_G997_LineStatus_t)

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   This function returns the current per band status for the line.

   \param DSL_G997_LineStatusPerBand_t*
      The parameter points to a \ref DSL_G997_LineStatusPerBand_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LineStatusPerBand_t lineStatusPerBand;

      memset(&lineStatusPerBand, 0x00, sizeof(DSL_G997_LineStatusPerBand_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_STATUS_PER_BAND_GET, &lineStatusPerBand)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_STATUS_PER_BAND_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 10), DSL_G997_LineStatusPerBand_t)
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   This function returns the current status for the (bearer channel).

   \param DSL_G997_ChannelStatus_t*
      The parameter points to a \ref DSL_G997_ChannelStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that the state of this status values are only updated if showtime
   is reached otherwise they might reflect inconsistent states.
   To check if the line is in showtime please use ioctl
   \ref DSL_FIO_LINE_STATE_GET before using this functionality if you are not
   sure about the line state.
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_ChannelStatus_t channelStatus;

      memset(&channelStatus, 0x00, sizeof(DSL_G997_ChannelStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_CHANNEL_STATUS_GET, &channelStatus)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_CHANNEL_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 11), DSL_G997_ChannelStatus_t)

/**
   This function triggers a forced power management state on the line.

   \param DSL_G997_PowerManagementStateForcedTrigger_t*
      The parameter points to a \ref DSL_G997_PowerManagementStateForcedTrigger_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_PowerManagementStateForcedTrigger_t pwrMngStateForcedTrigger;

      memset(&pwrMngStateForcedTrigger, 0x00, sizeof(DSL_G997_PowerManagementStateForcedTrigger_t));
      ret = ioctl(fd, DSL_FIO_G997_POWER_MANAGEMENT_STATE_FORCED_TRIGGER, &pwrMngStateForcedTrigger)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_POWER_MANAGEMENT_STATE_FORCED_TRIGGER \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 12), DSL_G997_PowerManagementStateForcedTrigger_t)

/**
   This function returns the current power management status for the line.

   \param DSL_G997_PowerManagementStatus_t*
      The parameter points to a \ref DSL_G997_PowerManagementStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_PowerManagementStatus_t pwrMngStatus;

      memset(&pwrMngStatus, 0x00, sizeof(DSL_G997_PowerManagementStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_POWER_MANAGEMENT_STATUS_GET, &pwrMngStatus)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_POWER_MANAGEMENT_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 13), DSL_G997_PowerManagementStatus_t)

/**
   This function returns the last successfully transmitted state of the line
   for the specified direction.

   \param DSL_G997_LastStateTransmitted_t*
      The parameter points to a \ref DSL_G997_LastStateTransmitted_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LastStateTransmitted_t lastStateTransmitted;

      memset(&lastStateTransmitted, 0x00, sizeof(DSL_G997_LastStateTransmitted_t));
      ret = ioctl(fd, DSL_FIO_G997_LAST_STATE_TRANSMITTED_GET, &lastStateTransmitted)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LAST_STATE_TRANSMITTED_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 14), DSL_G997_LastStateTransmitted_t)

/**
   This function returns the current bit allocation per subcarrier table of
   the line for the specified direction.

   \param DSL_G997_BitAllocationNsc_t*
      The parameter points to a \ref DSL_G997_BitAllocationNsc_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_BitAllocationNsc_t bitAllocationNsc;

      memset(&bitAllocationNsc, 0x00, sizeof(DSL_G997_BitAllocationNsc_t));
      ret = ioctl(fd, DSL_FIO_G997_BIT_ALLOCATION_NSC_GET, &bitAllocationNsc)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_BIT_ALLOCATION_NSC_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 15), DSL_G997_BitAllocationNsc_t)

/**
   This function returns the current gain allocation per subcarrier table of
   the line for the specified direction.

   \param DSL_G997_GainAllocationNsc_t*
      The parameter points to a \ref DSL_G997_GainAllocationNsc_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_GainAllocationNsc_t gainAllocationNsc;

      memset(&gainAllocationNsc, 0x00, sizeof(DSL_G997_GainAllocationNsc_t));
      ret = ioctl(fd, DSL_FIO_G997_BIT_ALLOCATION_NSC_GET, &gainAllocationNsc)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_GAIN_ALLOCATION_NSC_GET \
   _IO(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 16))

/**
   This function returns the current SNR allocation per subcarrier table of
   the line for the specified direction.

   \param DSL_G997_SnrAllocationNsc_t*
      The parameter points to a \ref DSL_G997_SnrAllocationNsc_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_SnrAllocationNsc_t snrAllocationNsc;

      memset(&snrAllocationNsc, 0x00, sizeof(DSL_G997_SnrAllocationNsc_t));
      ret = ioctl(fd, DSL_FIO_G997_SNR_ALLOCATION_NSC_GET, &snrAllocationNsc)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_SNR_ALLOCATION_NSC_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 17), DSL_G997_SnrAllocationNsc_t)

/**
   This function configures the mask for the autonomous line failure event
   signalling.

   \param DSL_G997_LineFailures_t*
      The parameter points to a \ref DSL_G997_LineFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LineFailures_t alarmMaskLineFailuresCfgSet;

      memset(&alarmMaskLineFailuresCfgSet, 0x00, sizeof(DSL_G997_LineFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_SET, &alarmMaskLineFailuresCfgSet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 18), DSL_G997_LineFailures_t)

/**
   This function returns the current configuration of the mask for the
   autonomous line failure event signalling.

   \param DSL_G997_LineFailures_t*
      The parameter points to a \ref DSL_G997_LineFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LineFailures_t alarmMaskLineFailuresCfgGet;

      memset(&alarmMaskLineFailuresCfgGet, 0x00, sizeof(DSL_G997_LineFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_GET, &alarmMaskLineFailuresCfgGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_ALARM_MASK_LINE_FAILURES_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 19), DSL_G997_LineFailures_t)

/**
   This function returns the current status of the line failures.

   \param DSL_G997_LineFailures_t*
      The parameter points to a \ref DSL_G997_LineFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LineFailures_t lineFailuresStatusGet;

      memset(&lineFailuresStatusGet, 0x00, sizeof(DSL_G997_LineFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_FAILURES_STATUS_GET, &lineFailuresStatusGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_FAILURES_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 20), DSL_G997_LineFailures_t)


/**
   This function configures the mask for the autonomous data path failure event
   signalling.

   \param DSL_G997_DataPathFailures_t*
      The parameter points to a \ref DSL_G997_DataPathFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_DataPathFailures_t alarmMaskDataPathFailuresCfgSet;

      memset(&alarmMaskDataPathFailuresCfgSet, 0x00, sizeof(DSL_G997_DataPathFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_SET,
         &alarmMaskDataPathFailuresCfgSet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 21), DSL_G997_DataPathFailures_t)

/**
   This function returns the current configuration of the mask for the
   autonomous data path failure event signalling.

   \param DSL_G997_DataPathFailures_t*
      The parameter points to a \ref DSL_G997_DataPathFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_DataPathFailures_t alarmMaskDataPathFailuresCfgGet;

      memset(&alarmMaskDataPathFailuresCfgGet, 0x00, sizeof(DSL_G997_DataPathFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_GET,
         &alarmMaskDataPathFailuresCfgGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_ALARM_MASK_DATA_PATH_FAILURES_CONFIG_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 22), DSL_G997_DataPathFailures_t)

/**
   This function returns the current status of the data path failures.

   \param DSL_G997_DataPathFailures_t*
      The parameter points to a \ref DSL_G997_DataPathFailures_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_DataPathFailures_t dataPathFailuresStatusGet;

      memset(&dataPathFailuresStatusGet, 0x00, sizeof(DSL_G997_DataPathFailures_t));
      ret = ioctl(fd, DSL_FIO_G997_DATA_PATH_FAILURES_STATUS_GET, &dataPathFailuresStatusGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DATA_PATH_FAILURES_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 23), DSL_G997_DataPathFailures_t)

/**
   This function reads framing parameters according to ITU G997.1

   \param DSL_G997_FramingParameterStatus_t*
      The parameter points to a \ref DSL_G997_FramingParameterStatus_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Please note that the state of this status values are only updated if showtime
   is reached otherwise they might reflect inconsistent states.
   To check if the line is in showtime please use ioctl
   \ref DSL_FIO_LINE_STATE_GET before using this functionality if you are not
   sure about the line state.
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_FramingParameterStatus_t g997FramingParamStatusGet;

      memset(&g997FramingParamStatusGet, 0x00, sizeof(DSL_G997_FramingParameterStatus_t));
      ret = ioctl(fd, DSL_FIO_G997_FRAMING_PARAMETER_STATUS_GET, &g997FramingParamStatusGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_FRAMING_PARAMETER_STATUS_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 24), DSL_G997_FramingParameterStatus_t)

/**
   This function returns the line specific inventory information for the
   NearEnd or FarEnd.
   \attention The line inventory information of the far end side will be
              requested via EOC/OHC handling. This is only possible in showtime
              state and may take several seconds.
              The callback event DSL_EVENT_S_FE_INVENTORY_AVAILABLE will be called
              after ALL far end inventory data has been received.

   \param DSL_G997_LineInventory_t*
      The parameter points to a \ref DSL_G997_LineInventory_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LineInventory_t lineInventoryGet;

      memset(&lineInventoryGet, 0x00, sizeof(DSL_G997_LineInventory_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_INVENTORY_GET, &lineInventoryGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_INVENTORY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 25), DSL_G997_LineInventory_t)

/**
   This function modifies the line specific inventory information for the
   NearEnd.

   \param DSL_G997_LineInventoryNe_t*
      The parameter points to a \ref DSL_G997_LineInventoryNe_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_LineInventoryNe_t lineInventorySet;

      memset(&lineInventorySet, 0x00, sizeof(DSL_G997_LineInventoryNe_t));
      ret = ioctl(fd, DSL_FIO_G997_LINE_INVENTORY_SET, &lineInventorySet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_LINE_INVENTORY_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 26), DSL_G997_LineInventoryNe_t)

/**
   This function returns the Hlin scale value measured during diagnostic.

   \param DSL_G997_DeltHlinScale_t*
      The parameter points to a \ref DSL_G997_DeltHlinScale_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE

   \code
      DSL_G997_DeltHlinScale_t deltHlinScaleGet;

      memset(&deltHlinScaleGet, 0x00, sizeof(DSL_G997_DeltHlinScale_t));
      ret = ioctl(fd, DSL_FIO_G997_DELT_HLIN_SCALE_GET, &deltHlinScaleGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_HLIN_SCALE_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 27), DSL_G997_DeltHlinScale_t)

/**
   This function returns the Hlin per subcarrier data measured during diagnostic.

   \param DSL_G997_DeltHlin_t*
      The parameter points to a \ref DSL_G997_DeltHlin_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE

   \code
      DSL_G997_DeltHlin_t deltHlinGet;

      memset(&deltHlinGet, 0x00, sizeof(DSL_G997_DeltHlin_t));
      ret = ioctl(fd, DSL_FIO_G997_DELT_HLIN_GET, &deltHlinGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_HLIN_GET \
   _IO(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 28))

/**
   This function returns the Hlog per subcarrier measured during diagnostic.

   \param DSL_G997_DeltHlog_t*
      The parameter points to a \ref DSL_G997_DeltHlog_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE (The values are reported per tone group, not per tone),
             only for the DOWNSTREAM

   \code
      DSL_G997_DeltHlog_t deltHlogGet;

      memset(&deltHlogGet, 0x00, sizeof(DSL_G997_DeltHlog_t));
      ret = ioctl(fd, DSL_FIO_G997_DELT_HLOG_GET, &deltHlogGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_HLOG_GET \
   _IO(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 29))

/**
   This function returns the Signal to Noise Ratio (SNR) per subcarrier measured
   during diagnostic.

   \param DSL_G997_DeltSnr_t*
      The parameter points to a \ref DSL_G997_DeltSnr_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE, only for the DOWNSTREAM
   - Vinax:  VDSL-CPE (The values are reported per tone group, not per tone),
             only for the DOWNSTREAM

   \code
      DSL_G997_DeltSnr_t deltSnrGet;

      memset(&deltSnrGet, 0x00, sizeof(DSL_G997_DeltSnr_t));
      ret = ioctl(fd, DSL_FIO_G997_DELT_SNR_GET, &deltSnrGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_SNR_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 30), DSL_G997_DeltSnr_t)

/**
   This function returns the Quiet Line Noise (QLN) per subcarrier measured
   during diagnostic.

   \param DSL_G997_DeltQln_t*
      The parameter points to a \ref DSL_G997_DeltQln_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE (The values are reported per tone group, not per tone),
             only for the DOWNSTREAM

   \code
      DSL_G997_DeltQln_t deltQlnGet;

      memset(&deltQlnGet, 0x00, sizeof(DSL_G997_DeltQln_t));
      ret = ioctl(fd, DSL_FIO_G997_DELT_QLN_GET, &deltQlnGet)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_QLN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 31), DSL_G997_DeltQln_t)

/**
   This function releases memory allocated for DELT before.

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE

   \code
      ret = ioctl(fd, DSL_FIO_G997_DELT_FREE_RESOURCES, 0)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_DELT_FREE_RESOURCES \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 32), DSL_G997_DeltFreeResources_t)

/**
   This function will copy the SNMP message to an internal buffer.
   This buffer will be transmitted asynchronous.

   \param DSL_G997_Snmp_t*
      The parameter points to a \ref DSL_G997_Snmp_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_Snmp_t snmpData;

      memset(&snmpData, 0x00, sizeof(DSL_G997_Snmp_t));
      // Set data within SNMP structure here...

      ret = ioctl(fd, DSL_FIO_G997_SNMP_MESSAGE_SEND, &snmpData)
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_SNMP_MESSAGE_SEND \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 33), DSL_G997_Snmp_t)

/**
   This function will retrieve the last received SNMP message.

   \param DSL_G997_Snmp_t*
      The parameter points to a \ref DSL_G997_Snmp_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_G997_Snmp_t snmpData;

      memset(&snmpData, 0x00, sizeof(DSL_G997_Snmp_t));

      ret = ioctl(fd, DSL_FIO_G997_SNMP_MESSAGE_RECEIVE, &snmpData)
      // Read data from SNMP structure here...
   \endcode

   \ingroup DRV_DSL_CPE_G997 */
#define DSL_FIO_G997_SNMP_MESSAGE_RECEIVE \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_G997 + 34), DSL_G997_Snmp_t)


/* ************************************************************************** */
/* * Ioctl interface definitions for PM                                     * */
/* ************************************************************************** */

/**
   This function returns DSL line 15-minute second counter history statistics.

   \param DSL_PM_HistoryStatsDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_HistoryStatsDir_t pmHistoryStats;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsDir_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_HISTORY_STATS_15MIN_GET, &pmHistoryStats)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 0), DSL_PM_HistoryStatsDir_t)

/**
   This function returns DSL line 1-day second counter history statistics.

   \param DSL_PM_HistoryStatsDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_HistoryStatsDir_t pmHistoryStats;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsDir_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_HISTORY_STATS_1DAY_GET, &pmHistoryStats)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 1), DSL_PM_HistoryStatsDir_t)

/**
   This function returns DSL line 15-minute second counters of specified history
   interval.

   \param DSL_PM_LineSecCounters_t*
      The parameter points to a \ref DSL_PM_LineSecCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineSecCounters_t pmLineSecCounter;

      memset(&pmLineSecCounter, 0x00, sizeof(DSL_PM_LineSecCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_15MIN_GET, &pmLineSecCounter)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 2), DSL_PM_LineSecCounters_t)

/**
   This function returns DSL line 1-day second counters of specified history
   interval.

   \param DSL_PM_LineSecCounters_t*
      The parameter points to a \ref DSL_PM_LineSecCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineSecCounters_t pmLineSecCounters;

      memset(&pmLineSecCounters, 0x00, sizeof(DSL_PM_LineSecCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_1DAY_GET, &pmLineSecCounters)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 3), DSL_PM_LineSecCounters_t)

/**
   This function returns DSL line total second counters accumulated since system
   startup.

   \param DSL_PM_LineSecCountersTotal_t*
      The parameter points to a \ref DSL_PM_LineSecCountersTotal_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineSecCountersTotal_t pmLineSecCountersTotal;

      memset(&pmLineSecCountersTotal, 0x00, sizeof(DSL_PM_LineSecCountersTotal_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_TOTAL_GET, &pmLineSecCountersTotal)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 4), DSL_PM_LineSecCountersTotal_t)

/**
   This function sets DSL line 15-minute second counter thresholds.

   \param DSL_PM_LineSecThreshold_t*
      The parameter points to a \ref DSL_PM_LineSecThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineSecThreshold_t pmLineSecThresholds;

      memset(&pmLineSecThresholds, 0x00, sizeof(DSL_PM_LineSecThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_SET, &pmLineSecThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 5), DSL_PM_LineSecThreshold_t)

/**
   This function sets DSL line 1-day second counter thresholds.

   \param DSL_PM_LineSecThreshold_t*
      The parameter points to a \ref DSL_PM_LineSecThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineSecThreshold_t pmLineSecThresholds;

      memset(&pmLineSecThresholds, 0x00, sizeof(DSL_PM_LineSecThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_SET, &pmLineSecThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 6), DSL_PM_LineSecThreshold_t)

/**
   This function returns DSL line 15-minute second counter thresholds.

   \param DSL_PM_LineSecThreshold_t*
      The parameter points to a \ref DSL_PM_LineSecThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineSecThreshold_t pmLineSecThresholds;

      memset(&pmLineSecThresholds, 0x00, sizeof(DSL_PM_LineSecThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_GET, &pmLineSecThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_THRESHOLDS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 7), DSL_PM_LineSecThreshold_t)

/**
   This function returns DSL line 1-day second counter thresholds.

   \param DSL_PM_LineSecThreshold_t*
      The parameter points to a \ref DSL_PM_LineSecThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineSecThreshold_t pmLineSecThresholds;

      memset(&pmLineSecThresholds, 0x00, sizeof(DSL_PM_LineSecThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_GET, &pmLineSecThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_THRESHOLDS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 8), DSL_PM_LineSecThreshold_t)

/**
   This function returns DSL line 15-minute initialization counter history
   statistics.

   \param DSL_PM_HistoryStats_t*
      The parameter points to a \ref DSL_PM_HistoryStats_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_HistoryStats_t pmHistoryStats;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStats_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_HISTORY_STATS_15MIN_GET, &pmHistoryStats)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 9), DSL_PM_HistoryStats_t)

/**
   This function returns DSL line 1 day initialization counter history
   statistics.

   \param DSL_PM_HistoryStats_t*
      The parameter points to a \ref DSL_PM_HistoryStats_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_HistoryStats_t pmHistoryStats;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStats_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_HISTORY_STATS_1DAY_GET, &pmHistoryStats)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 10), DSL_PM_HistoryStats_t)

/**
   This function returns DSL line 15-minute initialization counters of specified
   history interval.

   \param DSL_PM_LineInitCounters_t*
      The parameter points to a \ref DSL_PM_LineInitCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineInitCounters_t pmLineInitCounters;

      memset(&pmLineInitCounters, 0x00, sizeof(DSL_PM_LineInitCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_COUNTERS_15MIN_GET, &pmLineInitCounters)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 11), DSL_PM_LineInitCounters_t)

/**
   This function returns DSL line 1-day initialization counters of specified
   history interval.

   \param DSL_PM_LineInitCounters_t*
      The parameter points to a \ref DSL_PM_LineInitCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineInitCounters_t pmLineInitCounters;

      memset(&pmLineInitCounters, 0x00, sizeof(DSL_PM_LineInitCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_COUNTERS_1DAY_GET, &pmLineInitCounters)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 12), DSL_PM_LineInitCounters_t)

/**
   This function returns DSL line total initialization counters accumulated
   since system startup.

   \param DSL_PM_LineInitCountersTotal_t*
      The parameter points to a \ref DSL_PM_LineInitCountersTotal_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineInitCountersTotal_t pmLineInitCountersTotal;

      memset(&pmLineInitCountersTotal, 0x00, sizeof(DSL_PM_LineInitCountersTotal_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_COUNTERS_TOTAL_GET, &pmLineInitCountersTotal)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 13), DSL_PM_LineInitCountersTotal_t)

/**
   This function sets DSL line 15-minute initialization counter thresholds.

   \param DSL_PM_LineInitThreshold_t*
      The parameter points to a \ref DSL_PM_LineInitThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineInitThreshold_t pmLineInitThresholds;

      memset(&pmLineInitThresholds, 0x00, sizeof(DSL_PM_LineInitThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_SET, &pmLineInitThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 14), DSL_PM_LineInitThreshold_t)

/**
   This function sets DSL line 1-day initialization counter thresholds.

   \param DSL_PM_LineInitThreshold_t*
      The parameter points to a \ref DSL_PM_LineInitThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineInitThreshold_t pmLineInitThresholds;

      memset(&pmLineInitThresholds, 0x00, sizeof(DSL_PM_LineInitThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_SET, &pmLineInitThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 15), DSL_PM_LineInitThreshold_t)

/**
   This function returns DSL line 15-minute initialization counter thresholds.

   \param DSL_PM_LineInitThreshold_t*
      The parameter points to a \ref DSL_PM_LineInitThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineInitThreshold_t pmLineInitThresholds;

      memset(&pmLineInitThresholds, 0x00, sizeof(DSL_PM_LineInitThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_GET, &pmLineInitThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_THRESHOLDS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 16), DSL_PM_LineInitThreshold_t)

/**
   This function returns DSL line 1-day initialization counter thresholds.

   \param DSL_PM_LineInitThreshold_t*
      The parameter points to a \ref DSL_PM_LineInitThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineInitThreshold_t pmLineInitThresholds;

      memset(&pmLineInitThresholds, 0x00, sizeof(DSL_PM_LineInitThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_GET, &pmLineInitThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_THRESHOLDS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 17), DSL_PM_LineInitThreshold_t)

/**
   This function returns DSL channel 15-minute counter history statistics.

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_HistoryStatsChDir_t pmHistoryStats;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_HISTORY_STATS_15MIN_GET, &pmHistoryStats)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 18), DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL channel 1-day counter history statistics.

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_HistoryStatsChDir_t pmHistoryStats;

      memset(&pmHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_HISTORY_STATS_1DAY_GET, &pmHistoryStats)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 19), DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL channel 15-minute counters of specified history interval.

   \param DSL_PM_ChannelCounters_t*
      The parameter points to a \ref DSL_PM_ChannelCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_ChannelCounters_t pmChannelCounters;

      memset(&pmChannelCounters, 0x00, sizeof(DSL_PM_ChannelCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET, &pmChannelCounters)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 20), DSL_PM_ChannelCounters_t)

/**
   This function returns DSL channel 1-day counters of specified history interval.

   \param DSL_PM_ChannelCounters_t*
      The parameter points to a \ref DSL_PM_ChannelCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_ChannelCounters_t pmChannelCounters;

      memset(&pmChannelCounters, 0x00, sizeof(DSL_PM_ChannelCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_1DAY_GET, &pmChannelCounters)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 21), DSL_PM_ChannelCounters_t)

/**
   This function returns DSL channel total counters accumulated since system startup.

   \param DSL_PM_ChannelCountersTotal_t*
      The parameter points to a \ref DSL_PM_ChannelCountersTotal_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_ChannelCountersTotal_t pmChannelCountersTotal;

      memset(&pmChannelCountersTotal, 0x00, sizeof(DSL_PM_ChannelCountersTotal_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_TOTAL_GET, &pmChannelCountersTotal)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 22), DSL_PM_ChannelCountersTotal_t)

/**
   This function sets DSL channel 15-minute counter thresholds.

   \param DSL_PM_ChannelThreshold_t*
      The parameter points to a \ref DSL_PM_ChannelThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_ChannelThreshold_t pmChannelThresholds;

      memset(&pmChannelThresholds, 0x00, sizeof(DSL_PM_ChannelThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_SET, &pmChannelThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 23), DSL_PM_ChannelThreshold_t)

/**
   This function sets DSL channel 1-day counter thresholds.

   \param DSL_PM_ChannelThreshold_t*
      The parameter points to a \ref DSL_PM_ChannelThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_ChannelThreshold_t pmChannelThresholds;

      memset(&pmChannelThresholds, 0x00, sizeof(DSL_PM_ChannelThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_SET, &pmChannelThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 24), DSL_PM_ChannelThreshold_t)

/**
   This function returns DSL channel 15-minute counter thresholds.

   \param DSL_PM_ChannelThreshold_t*
      The parameter points to a \ref DSL_PM_ChannelThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_ChannelThreshold_t pmChannelThresholds;

      memset(&pmChannelThresholds, 0x00, sizeof(DSL_PM_ChannelThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_GET, &pmChannelThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_THRESHOLDS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 25), DSL_PM_ChannelThreshold_t)

/**
   This function returns DSL channel 1-day counter thresholds.

   \param DSL_PM_ChannelThreshold_t*
      The parameter points to a \ref DSL_PM_ChannelThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_ChannelThreshold_t pmChannelThresholds;

      memset(&pmChannelThresholds, 0x00, sizeof(DSL_PM_ChannelThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_GET, &pmChannelThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_THRESHOLDS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 26), DSL_PM_ChannelThreshold_t)

/**
   This function returns DSL data path 15-minute counter history statistics.

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_HistoryStatsChDir_t pmDataPathHistoryStats;

      memset(&pmDataPathHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_HISTORY_STATS_15MIN_GET, &pmDataPathHistoryStats)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_HISTORY_STATS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 27), DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL data path 1-day counter history statistics.

   \param DSL_PM_HistoryStatsChDir_t*
      The parameter points to a \ref DSL_PM_HistoryStatsChDir_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_HistoryStatsChDir_t pmDataPathHistoryStats;

      memset(&pmDataPathHistoryStats, 0x00, sizeof(DSL_PM_HistoryStatsChDir_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_HISTORY_STATS_1DAY_GET, &pmDataPathHistoryStats)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_HISTORY_STATS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 28), DSL_PM_HistoryStatsChDir_t)

/**
   This function returns DSL data path 15-minute counters of specified history
   interval.

   \param DSL_PM_DataPathCounters_t*
      The parameter points to a \ref DSL_PM_DataPathCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_DataPathCounters_t pmDataPathCounters;

      memset(&pmDataPathCounters, 0x00, sizeof(DSL_PM_DataPathCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_15MIN_GET, &pmDataPathCounters)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_COUNTERS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 29), DSL_PM_DataPathCounters_t)

/**
   This function returns DSL data path 1-day counters of specified history
   interval.

   \param DSL_PM_DataPathCounters_t*
      The parameter points to a \ref DSL_PM_DataPathCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_DataPathCounters_t pmDataPathCounters;

      memset(&pmDataPathCounters, 0x00, sizeof(DSL_PM_DataPathCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_1DAY_GET, &pmDataPathCounters)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_COUNTERS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 30), DSL_PM_DataPathCounters_t)

/**
   This function returns DSL data path total counters accumulated since system
   startup.

   \param DSL_PM_DataPathCountersTotal_t*
      The parameter points to a \ref DSL_PM_DataPathCountersTotal_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_DataPathCountersTotal_t pmDataPathCountersTotal;

      memset(&pmDataPathCountersTotal, 0x00, sizeof(DSL_PM_DataPathCountersTotal_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_TOTAL_GET, &pmDataPathCountersTotal)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_COUNTERS_TOTAL_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 31), DSL_PM_DataPathCountersTotal_t)

/**
   This function sets DSL data path 15-minute counter thresholds.

   \param DSL_PM_DataPathThreshold_t*
      The parameter points to a \ref DSL_PM_DataPathThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_DataPathThreshold_t pmDataPathThresholds;

      memset(&pmDataPathThresholds, 0x00, sizeof(DSL_PM_DataPathThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_SET, &pmDataPathThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 32), DSL_PM_DataPathThreshold_t)

/**
   This function sets DSL data path 1-day counter thresholds.

   \param DSL_PM_DataPathThreshold_t*
      The parameter points to a \ref DSL_PM_DataPathThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_DataPathThreshold_t pmDataPathThresholds;

      memset(&pmDataPathThresholds, 0x00, sizeof(DSL_PM_DataPathThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_SET, &pmDataPathThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 33), DSL_PM_DataPathThreshold_t)

/**
   This function returns DSL data path 15-minute counter thresholds.

   \param DSL_PM_DataPathThreshold_t*
      The parameter points to a \ref DSL_PM_DataPathThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_DataPathThreshold_t pmDataPathThresholds;

      memset(&pmDataPathThresholds, 0x00, sizeof(DSL_PM_DataPathThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_GET, &pmDataPathThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_THRESHOLDS_15MIN_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 34), DSL_PM_DataPathThreshold_t)

/**
   This function returns DSL data path 1-day counter thresholds.

   \param DSL_PM_DataPathThreshold_t*
      The parameter points to a \ref DSL_PM_DataPathThreshold_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_DataPathThreshold_t pmDataPathThresholds;

      memset(&pmDataPathThresholds, 0x00, sizeof(DSL_PM_DataPathThreshold_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_GET, &pmDataPathThresholds)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_THRESHOLDS_1DAY_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 35), DSL_PM_DataPathThreshold_t)

/**
   This function reset ALL PM data including history values except the total
   counters.

   \param DSL_PM_Reset_t*
      The parameter points to a \ref DSL_PM_Reset_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_Reset_t pmReset;

      pmReset = DSL_PM_RESET_ALL;
      memset(&pmReset, 0x00, sizeof(DSL_PM_Reset_t));
      ret = ioctl(fd, DSL_FIO_PM_RESET, &pmReset)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_RESET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 36), DSL_PM_Reset_t)

/**
   Setup a trigger event to signal that the 15 minutes timer has been elapsed.
   In addition the elapsed day interval can be marked.

   The trigger should be used with care. After completion of the update of the
   PM data base the event DSL_CBS_PM_SYNC is send to the user application.
   The next synchronization trigger should be issued at least after receiving
   of the previous DSL_CBS_PM_SYNC event.

   \param DSL_PM_ElapsedExtTrigger_t*
      The parameter points to a \ref DSL_PM_ElapsedExtTrigger_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_ElapsedExtTrigger_t pmTrigger;

      pmTrigger->bOneDayElapsed = DSL_FALSE;
      memset(&pmTrigger, 0x00, sizeof(DSL_PM_ElapsedExtTrigger_t));
      ret = ioctl(fd, DSL_FIO_PM_15MIN_ELAPSED_EXT_TRIGGER, &pmTrigger)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_15MIN_ELAPSED_EXT_TRIGGER \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 37), DSL_PM_ElapsedExtTrigger_t)

/**
   This function resets PM elapsed 15 min and 1 day time.

   \param DSL_PM_ElapsedTimeReset_t*
      The parameter points to a \ref DSL_PM_ElapsedTimeReset_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_ElapsedTimeReset_t pmReset;

      memset(&pmReset, 0x00, sizeof(DSL_PM_ElapsedTimeReset_t));
      ret = ioctl(fd, DSL_FIO_PM_ELAPSED_TIME_RESET, &pmReset)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_ELAPSED_TIME_RESET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 38), DSL_PM_ElapsedTimeReset_t)

/**
   This function sets PM synchronization mode.

   \param DSL_PM_SyncMode_t*
      The parameter points to a \ref DSL_PM_SyncMode_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_SyncMode_t pmSyncMode;

      pmSyncMode->nMode = DSL_PM_SYNC_MODE_EXTERNAL;
      memset(&pmSyncMode, 0x00, sizeof(DSL_PM_SyncMode_t));
      ret = ioctl(fd, DSL_FIO_PM_SYNC_MODE_SET, &pmSyncMode)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_SYNC_MODE_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 39), DSL_PM_SyncMode_t)

/**
   This function enables the PM burnin mode.
   This function can be used for test purposes by changing the default
   time base settings of the PM.

   \param DSL_PM_BurninMode_t*
      The parameter points to a \ref DSL_PM_BurninMode_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_BurninMode_t pmBurninMode;

      pmBurninMode->nMode->nPmTick = 10;
      pmBurninMode->nMode->nPm15Min = 100;
      pmBurninMode->nMode->nPm15MinPerDay = 2;
      memset(&pmBurninMode, 0x00, sizeof(DSL_PM_BurninMode_t));
      ret = ioctl(fd, DSL_FIO_PM_BURNIN_MODE_SET, &pmBurninMode)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_BURNIN_MODE_SET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 40), DSL_PM_BurninMode_t)

#if defined(DSL_PM_DEBUG_MODE_ENABLE) && (DSL_PM_DEBUG_MODE_ENABLE > 0)
/**
   This function print the PM data of all lines to the console.
   To show the data of a dedicated line please use the function DSL_PM_LineDump.
   This function is available if the compile switch DSL_PM_DEBUG_MODE_ENABLE
   is set to 1 (default mode).

   \param DSL_PM_Dump_t*
      The parameter points to a \ref DSL_PM_Dump_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_Dump_t pmDump;

      pmDump->dumpConfig->bPrintMemStatOnly = DSL_TRUE;
      memset(&pmDump, 0x00, sizeof(DSL_PM_Dump_t));
      ret = ioctl(fd, DSL_FIO_PM_DUMP, &pmDump)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DUMP \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 41), DSL_PM_Dump_t)
#endif /* defined(DSL_PM_DEBUG_MODE_ENABLE) && (DSL_PM_DEBUG_MODE_ENABLE > 0) */

/**
   This function returns returns DSL second counter statistics accumulated since
   the specified showtime interval.
   Implementation is done according to specification within TR-98.

   \param DSL_PM_LineSecCounters_t*
      The parameter points to a \ref DSL_PM_LineSecCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineSecCounters_t pmLineSecCounter;

      memset(&pmLineSecCounter, 0x00, sizeof(DSL_PM_LineSecCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_SEC_COUNTERS_15MIN_GET, &pmLineSecCounter)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_SEC_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 42), DSL_PM_LineSecCounters_t)

/**
   This function returns returns DSL initialization counter statistics
   accumulated since the specified showtime interval.
   Implementation is done according to specification within TR-98.

   \param DSL_PM_LineInitCounters_t*
      The parameter points to a \ref DSL_PM_LineInitCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_LineInitCounters_t pmLineInitCounters;

      memset(&pmLineInitCounters, 0x00, sizeof(DSL_PM_LineInitCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_LINE_INIT_COUNTERS_15MIN_GET, &pmLineInitCounters)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_LINE_INIT_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 43), DSL_PM_LineInitCounters_t)

/**
   This function returns returns DSL channel counter statistics accumulated
   since the specified showtime interval.
   Implementation is done according to specification within TR-98.

   \param DSL_PM_ChannelCounters_t*
      The parameter points to a \ref DSL_PM_ChannelCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_ChannelCounters_t pmChannelCounters;

      memset(&pmChannelCounters, 0x00, sizeof(DSL_PM_ChannelCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_CHANNEL_COUNTERS_15MIN_GET, &pmChannelCounters)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_CHANNEL_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 44), DSL_PM_ChannelCounters_t)

/**
   This function returns returns DSL data path counter statistics accumulated
   since the specified showtime interval.
   Implementation is done according to specification within TR-98.

   \param DSL_PM_DataPathCounters_t*
      The parameter points to a \ref DSL_PM_DataPathCounters_t structure

   \return
      0 if successful and -1 in case of an error/warning
      In case of an error/warning please refer to the value of 'nReturn' which
      is included within accessCtl structure of user data.

   \remarks
   Supported by
   - Danube: ADSL-CPE
   - Vinax:  VDSL-CPE

   \code
      DSL_PM_DataPathCounters_t pmDataPathCounters;

      memset(&pmDataPathCounters, 0x00, sizeof(DSL_PM_DataPathCounters_t));
      ret = ioctl(fd, DSL_FIO_PM_DATA_PATH_COUNTERS_15MIN_GET, &pmDataPathCounters)
   \endcode

   \ingroup DRV_DSL_CPE_PM */
#define DSL_FIO_PM_DATA_PATH_COUNTERS_SHOWTIME_GET \
   _IOWR(DSL_IOC_MAGIC_CPE_API, (DSL_IOC_BASE_CPE_API_PM + 45), DSL_PM_DataPathCounters_t)


#endif /* DSL_CPE_API_USE_KERNEL_INTERFACE */

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_API_IOCTL_H */
