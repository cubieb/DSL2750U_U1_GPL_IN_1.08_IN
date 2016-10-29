/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_INTERN_H
#define _DRV_DSL_CPE_INTERN_H

#ifdef __cplusplus
   extern "C" {
#endif

#include "drv_dsl_cpe_api.h"
#include "drv_dsl_cpe_autoboot.h"

#include "drv_dsl_cpe_fifo.h"

/** \file
   This file specifies the internal functions that are used for common
   implementation of the ioctl interface.
   It is intendet to be used within the DSL CPE_API driver ONLY.
*/

/** \addtogroup DRV_DSL_CPE_COMMON
 @{ */

#if defined(INCLUDE_DSL_PM)
   #include "drv_dsl_cpe_api_pm.h"
   #include "drv_dsl_cpe_intern_pm.h"
   #include "drv_dsl_cpe_device_pm.h"
#endif

#if defined(INCLUDE_DSL_ADSL_MIB)
   #include "drv_dsl_cpe_intern_mib.h"
#endif

#ifdef INCLUDE_DSL_CPE_API_VINAX
   #include "drv_dsl_cpe_intern_sar.h"
#endif

/*
   Automatic generation of wrapper function for cli interfcae is not working
   correctly at the moment. Therefore some functions are excluded from automatic
   generation at the moment using the following preprocessor definition.
*/
#ifdef SWIG
#define SWIG_TMP
#endif

/**
   This structure is intended to save upper-software instance data
*/
typedef struct DSL_OpenContext
{
   /** member for list organization */
   struct DSL_OpenContext *pNext;
   /** Device context pointer */
   DSL_devCtx_t *pDevCtx;
   /** Event mechanism */
   /** Event access mutex */
   DSL_mutex_t eventMutex;
   /** Wait queue for the event mechanism */
   DSL_waitQueueHead_t eventWaitQueue;
   /** FIFO for the event mechanism */
   DSL_FIFO *eventFifo;
   /** FIFO buffer */
   DSL_uint8_t *eventFifoBuf;
   /** Enable/Disable event handling*/
   DSL_boolean_t bEventActivation;
   /** Configured event mask */
   DSL_uint32_t nEventMask;
   /**
   Specifies the resource activation mask. */
   DSL_BF_ResourceActivationType_t nResourceActivationMask;
   /** CEOC instance specific data */
#ifdef INCLUDE_DSL_CEOC
   /** CEOC FIFO for the SNMP protocol*/
   DSL_FIFO *rxSnmpFifo;
   /** Rx FIFO buffer for the SNMP protocol*/
   DSL_uint8_t *rxSnmpFifoBuf;
   /** Event access mutex */
   DSL_mutex_t rxSnmpFifoMutex;
#endif /** #ifdef INCLUDE_DSL_CEOC*/
} DSL_OpenContext_t;

#include "drv_dsl_cpe_intern_g997.h"

#if defined(INCLUDE_DSL_CEOC)
   #include "drv_dsl_cpe_intern_ceoc.h"
   #include "drv_dsl_cpe_device_ceoc.h"
#endif /** #if defined(INCLUDE_DSL_CEOC)*/

/**
   Defines all possible xDSL transmission modes
*/
typedef enum
{
   /**
   Zero has been chosen to indicate 'not initialized' after memset of context
   structure after startup for example */
   DSL_XDSLMODE_UNKNOWN = 0,
   /**
   ITU-T G.992.1, ADSL1 */
   DSL_XDSLMODE_G_992_1,
   /**
   ANSI 1.413, (ADSL1 only) */
   DSL_XDSLMODE_T1_413,
   /**
   ITU-T G.992.2, ADSL1 lite */
   DSL_XDSLMODE_G_992_2,
   /**
   ITU-T G.992.3, ADSL2 */
   DSL_XDSLMODE_G_992_3,
   /**
   ITU-T G.992.4, ADSL2 lite */
   DSL_XDSLMODE_G_992_4,
   /**
   ITU-T G.992.5, ADSL2+ */
   DSL_XDSLMODE_G_992_5,
   /**
   ITU-T G.993.1, VDSL2 */
   DSL_XDSLMODE_G_993_1,
   DSL_XDSLMODE_LAST
} DSL_xDslMode_t;

/**
   Defines all possible annex types
*/
typedef enum
{
   /**
   Zero has been chosen to indicate 'not initialized' after memset of context
   structure after startup for example */
   DSL_ANNEX_UNKNOWN = 0,
   /**
   Annex A is used for ADSL and VDSL */
   DSL_ANNEX_A = 1,
   /**
   Annex B is used for ADSL and VDSL */
   DSL_ANNEX_B = 2,
   /**
   Annex C is used for VDSL only */
   DSL_ANNEX_C = 3,
   /**
   Annex I is used for ADSL only */
   DSL_ANNEX_I = 4,
   /**
   Annex I is used for ADSL only */
   DSL_ANNEX_J = 5,
   /**
   Annex I is used for ADSL only */
   DSL_ANNEX_L = 6,
   /**
   Annex I is used for ADSL only */
   DSL_ANNEX_M = 7,
   DSL_ANNEX_LAST
} DSL_AnnexType_t;

/**
   The driver context contains global information.
*/
struct DSL_Context
{
   /** Was DSL CPE API initialized? */
   DSL_boolean_t bInitComplete;
   /** Back pointer to the device context structure */
   DSL_devCtx_t *pDevCtx;
   /** Device mutex */
   DSL_mutex_t  bspMutex;
   /** Data access mutex */
   DSL_mutex_t dataMutex;
   /** Initialization mutex */
   DSL_mutex_t  initMutex;
   /** Last error code */
   DSL_Error_t nErrNo;
   /** Pointer to the stored FW binary*/
   DSL_uint8_t *pFirmware;
   /** Size of the stored firmware binary*/
   DSL_uint32_t nFirmwareSize;
   /** Pointer to the 2'nd stored FW binary*/
   DSL_uint8_t *pFirmware2;
   /** Size of the 2nd stored firmware binary*/
   DSL_uint32_t nFirmwareSize2;
   /** External Trigger to resume autoboot handling*/
   DSL_boolean_t bAutobootContinue;
   /** Autoboot queue */
   DSL_waitQueueHead_t autobootWaitQueue;
   /** Autoboot completion */
   DSL_completion_t autobootThreadExit;
   /** Autoboot thread activity flag */
   DSL_boolean_t bAutobootThreadStarted;
   /** Autoboot thread shutdown flag */
   DSL_boolean_t bAutobootThreadShutdown;
   /** Autoboot thread poll time */
   DSL_uint32_t nAutobootPollTime;
   /** Autoboot state */
   DSL_Autoboot_State_t nAutobootState;
   /** Autoboot status*/
   DSL_AutobootStatusData_t nAutobootStatus;
   /** Autoboot configuration data*/
   DSL_AutobootConfigData_t nAutobootConfig;
   /** Autoboot stop request */
   DSL_boolean_t bAutobootStopPending;
   /** Firmware load request flag */
   DSL_boolean_t bAutobootFwLoadPending;
   /** Autoboot start time */
   DSL_TimeVal_t autobootStartTime;
   /** Autoboot timeout in the current state */
   DSL_int_t nAutobootTimeoutLimit;
   /** Autoboot has got a response from a remote modem*/
   DSL_boolean_t bGotResponse;
   /** Autoboot has got a shortinit response */
   DSL_boolean_t bGotShortInitResponse;
   /** Autoboot startup mode */
   DSL_AutobootCtrlSet_t nAutobootStartupMode;
   /* Flag to inform the Autoboot that the FW request has been handeld*/
   DSL_boolean_t bFwRequestHandled;
   /* Flag to inform the Autoboot that the FW was reloaded*/
   DSL_boolean_t bFwReLoaded;
   /** L3 Power state forced flag */
   DSL_boolean_t bPowerManagementL3Forced;

   /** Firmware unavailable duration */
   DSL_uint32_t nFwUnavailableTime;

   /** Timeout lists context includes information of a specified
       timeout list. */
   DSL_TimeoutContext_t TimeoutListsContext;
   /** Flag that indicates about presence of firmware load handler */
   DSL_boolean_t bFirmwareEventAssigned;

   /** Line features configuration data. for the UPSTREAM and DOWNSTREM direction*/
   DSL_LineFeatureData_t lineFeatureDataCfg[2];
   /** Line features Status data. for the UPSTREAM and DOWNSTREM direction*/
   DSL_LineFeatureData_t lineFeatureDataSts[2];

   /** Line state */
   DSL_LineStateValue_t nLineState;
   /** Whether connection with a far end is established or not */
   /** Disconnection time */
   DSL_TimeVal_t disconnectTime;
   /** Showtime state reached time */
   DSL_TimeVal_t showtimeReachedTime;

   /** Test Mode Control*/
   DSL_TestModeControlSet_t nTestModeControl;

   /** Line configuration */
   DSL_boolean_t bLoopDiagnosticsCompleted;
   /** The loop diagnostic auto mode counter */
   DSL_uint8_t nLoopAutoCount;
   /** The minimal value of SNRM. Autoboot will be restarted in case of actual
       SNRM value becomes less than this minimal SNRM value */
   DSL_int16_t nMinSnrmDs;

   /** Near end inventory information */
   DSL_G997_LineInventoryData_t lineInventoryNe;
   /** Far end inventory information */
   DSL_G997_LineInventoryData_t lineInventoryFe;
#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
   /**
   Auxiliary inventory information according to
   ITU G.993.2 chapter 11.2.3.6 */
   DSL_AuxInventoryNe_t auxInventoryNe;
   /**
      Far-end Auxiliary inventory information
   */
   DSL_AuxLineInventoryData_t auxInventoryFe;
#endif

   /** Line activation configuration. It will be applied during next
       link activation */
   DSL_G997_LineActivateData_t lineActivateConfig;

   /** Power management status of link. Updated from firmware
       and autoboot thread state */
   DSL_G997_PowerManagementStatusData_t powerMgmtStatus;

   /** Line Init Status updated from firmware
       during training */
   DSL_G997_LineInitStatusData_t lineInitStatus;
   /** Number of link initialization retries */
   DSL_uint32_t lineInitRetryCount;

   /** Channel status */
   DSL_uint32_t ActualInterleaveDelayUs[DSL_CHANNELS_PER_LINE];
   DSL_uint32_t ActualInterleaveDelayDs[DSL_CHANNELS_PER_LINE];
   DSL_uint8_t ActualImpulseNoiseProtectionUs[DSL_CHANNELS_PER_LINE];
   DSL_uint8_t ActualImpulseNoiseProtectionDs[DSL_CHANNELS_PER_LINE];

   /** Upstream Actual Data Rate per channel */
   DSL_uint32_t nChannelActualDataRateUs[DSL_CHANNELS_PER_LINE];
   DSL_uint32_t nChannelActualDataRatePrevUs[DSL_CHANNELS_PER_LINE];
   /** Upstream Previous Data Rate per channel */
   DSL_uint32_t nChannelPreviousDataRateUs[DSL_CHANNELS_PER_LINE];
   /** Downstream Actual Data Rate per channel */
   DSL_uint32_t nChannelActualDataRateDs[DSL_CHANNELS_PER_LINE];
   DSL_uint32_t nChannelActualDataRatePrevDs[DSL_CHANNELS_PER_LINE];
   /** Downstream Previous Data Rate per channel */
   DSL_uint32_t nChannelPreviousDataRateDs[DSL_CHANNELS_PER_LINE];
   /** Previous Data Rate valid flag */
   DSL_boolean_t bPrevDataRateValid;
   /** Current Latency path for channel */
   DSL_LatencyPath_t nLPath[DSL_CHANNELS_PER_LINE];
   /** Upstream Channel Data Rate thresholds*/
   DSL_G997_ChannelDataRateThresholdData_t channelDataRateThresholdUs;
   /** Downstream Channel Data Rate thresholds*/
   DSL_G997_ChannelDataRateThresholdData_t channelDataRateThresholdDs;

   /** G997 block */
   /** XTSE configuration data */
   DSL_uint8_t xtseCfg[DSL_G997_NUM_XTSE_OCTETS];
   /** XTSE configuration status  */
   DSL_uint8_t xtseCurr[DSL_G997_NUM_XTSE_OCTETS];
   /** Pointer to DELT data storage */
   DSL_G997_DeltData_t *DELT;
   /** Internal used decoded values derived from xSTE octets */
   /** Current mode */
   DSL_xDslMode_t nXDslMode;
   /** Current Annex type */
   DSL_AnnexType_t nAnnexType;

   /** LED */
//#if defined(INCLUDE_ADSL_LED) && defined(INCLUDE_DSL_CPE_API_AMAZON_SE)
#if defined(INCLUDE_ADSL_LED)
   /** Led polling queue */
   DSL_waitQueueHead_t ledPollingWaitQueue;
   /** LED thread exit */
   DSL_completion_t ledThreadExit;
   /** This flag indicates that Status Led should be turned on */
   DSL_boolean_t bLedStatusOn;
   /** This flag indicates that Status Led should flash */
   DSL_boolean_t bLedNeedToFlash;
#endif

   /** Statistics */
   /** Software UAS counter */
   DSL_uint16_t nUAS;

   /** Current SNR margin*/
   DSL_int16_t nSnrmDs;
   /** Current Near End failures */
   DSL_G997_BF_LineFailures_t nLineFailuresNe;
   /** Current Far End failures */
   DSL_G997_BF_LineFailures_t nLineFailuresFe;
   /** Near End failures event generation mask */
   DSL_G997_BF_LineFailures_t nLineFailuresNeAlarmMask;
   /** Far End failures event generation mask */
   DSL_G997_BF_LineFailures_t nLineFailuresFeAlarmMask;
   DSL_G997_BF_DataPathFailures_t nDataPathFailuresNe[DSL_CHANNELS_PER_LINE];
   DSL_G997_BF_DataPathFailures_t nDataPathFailuresFe[DSL_CHANNELS_PER_LINE];
   DSL_G997_BF_DataPathFailures_t nDataPathFailuresNeAlarmMask;
   DSL_G997_BF_DataPathFailures_t nDataPathFailuresFeAlarmMask;

#ifdef INCLUDE_DSL_CPE_TRACE_BUFFER
   /** Showtime event logging buffer */
   DSL_uint16_t loggingBuffer[DSL_DEV_SHOWTIME_EVENT_LOGGING_BUFFER_LENGTH];
#endif

#if defined(INCLUDE_DSL_CEOC)
   DSL_void_t *CEOC;
#endif /** #if defined(INCLUDE_DSL_CEOC)*/

#if defined(INCLUDE_DSL_PM)
   /** PM module Context*/
   DSL_void_t *PM;
#if defined (INCLUDE_DSL_CPE_API_DANUBE) || defined (INCLUDE_DSL_CPE_API_AMAZON_SE)
   /* PM data */
   /** Channel counters */
   DSL_PM_ChannelData_t channelCounters[DSL_CHANNELS_PER_LINE][DSL_FAR_END + 1];

   /** Datapath counters */
   DSL_PM_DataPathData_t datapathCounters[DSL_CHANNELS_PER_LINE][DSL_FAR_END + 1];
#endif
#endif /* #if defined(INCLUDE_DSL_PM) */

/* ADSL MIB Module stuff */
#if defined(INCLUDE_DSL_ADSL_MIB)
   /** ADSL Mib context */
   DSL_MIB_ADSL_Context_t MibAdslCtx;
#endif
};

#define DSL_EVENT2MASK(evt) (0x1 << ((DSL_uint32_t)evt))

#ifdef DRV_DSL_CPE_FORCE_MACROS
   /** Macro to read values from the context in the safe way */
   #define _DSL_CTX_READ(ctx, err, attr, val) \
      do { \
         if (sizeof(ctx->attr) != sizeof(val)) \
         { \
            DSL_DEBUG( DSL_DBG_ERR, (ctx, DSL_CRLF""DSL_CRLF"DSL_CTX_READ - Context field " \
               "and value size mismatch! Data will be NOT copied!!!"DSL_CRLF)); \
            err = DSL_ERR_MEMORY; \
         } \
         else \
         { \
            if(DSL_MUTEX_LOCK(ctx->dataMutex)) \
            { \
              DSL_DEBUG( DSL_DBG_ERR, (ctx, "Couldn't lock data mutex"DSL_CRLF)); \
              err = DSL_ERR_SEMAPHORE_GET; \
            } \
            else \
            { \
               memcpy(&val, &ctx->attr, sizeof(ctx->attr); \
               DSL_MUTEX_UNLOCK(ctx->dataMutex); \
               err = DSL_SUCCESS; \
            } \
         } \
      } while (0)

   /** Macro to read scalar values from the context in the safe way */
   #define DSL_CTX_READ_SCALAR(ctx, err, attr, val) \
      _DSL_CTX_READ(ctx, err, attr, val)

   /* Macro to read values from the context in the safe way */
   #define DSL_CTX_READ(ctx, err, attr, val) \
      _DSL_CTX_READ(ctx, err, attr, val)

   #define _DSL_CTX_WRITE(ctx, err, attr, newval) \
      do { \
         if (sizeof(ctx->attr) != sizeof(val)) \
         { \
            DSL_DEBUG( DSL_DBG_ERR, (ctx, DSL_CRLF""DSL_CRLF"DSL_CTX_READ - Context field " \
               "and value size mismatch! Data will be NOT copied!!!"DSL_CRLF)); \
            err = DSL_ERR_MEMORY; \
         } \
         else \
         { \
            if(DSL_MUTEX_LOCK(ctx->dataMutex)) \
            { \
               DSL_DEBUG( DSL_DBG_ERR, (ctx, "Couldn't lock data mutex " \
                  "(pContext=%p, nOffset=%08x"DSL_CRLF, ctx, &ctx->attr - ctx)); \
               err = DSL_ERR_SEMAPHORE_GET; \
            } \
            else \
            { \
               memcpy(&ctx->attr, &newval, sizeof(ctx->attr); \
               err = DSL_SUCCESS; \
               DSL_MUTEX_UNLOCK(ctx->dataMutex); \
            } \
         } \
      } while (0)

   /** Macro to set scalar values to context fields in the safe way */
   #define DSL_CTX_WRITE_SCALAR(ctx, err, attr, newval) \
      _DSL_CTX_WRITE(ctx, err, attr, newval)

   /** Macro to set values to context fields in the safe way */
   #define DSL_CTX_WRITE(ctx, err, attr, newval) \
      _DSL_CTX_WRITE(ctx, err, attr, newval)
#else
   DSL_Error_t
   _DSL_CTX_ASSIGN_8(
      DSL_Context_t *pContext,
      DSL_uint8_t nFrom,
      DSL_void_t *pTo
   );

   DSL_Error_t
   _DSL_CTX_ASSIGN_16(
      DSL_Context_t *pContext,
      DSL_uint16_t nFrom,
      DSL_void_t *pTo
   );

   DSL_Error_t
   _DSL_CTX_ASSIGN_32(
      DSL_Context_t *pContext,
      DSL_uint32_t nFrom,
      DSL_void_t *pTo
   );

   DSL_Error_t
   _DSL_CTX_ASSIGN_ANY(
      DSL_Context_t *pContext,
      DSL_void_t *pFrom,
      DSL_void_t *pTo,
      DSL_uint32_t nSize
   );

   /* Macro to read values from the context in the safe way */
   #define DSL_CTX_READ_SCALAR(ctx, err, attr, val) \
      if (sizeof(ctx->attr) == 1) \
      { \
         err = _DSL_CTX_ASSIGN_8(ctx, (DSL_uint8_t)(ctx->attr), &(val)); \
      } \
      if (sizeof(ctx->attr) == 2) \
      { \
         err = _DSL_CTX_ASSIGN_16(ctx, (DSL_uint16_t)(ctx->attr), &(val)); \
      } \
      if (sizeof(ctx->attr) == 4) \
      { \
         err = _DSL_CTX_ASSIGN_32(ctx, (DSL_uint32_t)(ctx->attr), &(val)); \
      } \
      if (sizeof(ctx->attr) != 1 && sizeof(ctx->attr) != 2 \
         && sizeof(ctx->attr) != 4) \
      { \
         DSL_DEBUG( DSL_DBG_ERR, (ctx, "Couldn't read scalar " \
            "data with size different from 1, 2 or 4 bytes!!!"DSL_CRLF)); \
      }

   /* Macro to read values from the context in the safe way */
   #define DSL_CTX_READ(ctx, err, attr, val)  do { \
         if (sizeof(ctx->attr) != sizeof(val)) \
         { \
            DSL_DEBUG( DSL_DBG_ERR, (ctx, DSL_CRLF""DSL_CRLF"DSL_CTX_READ - Context field " \
               "and value size mismatch! Data will be NOT copied!!!"DSL_CRLF)); \
            err = DSL_ERR_MEMORY; \
         } \
         else \
         { \
            err = _DSL_CTX_ASSIGN_ANY(ctx, &(ctx->attr), &(val), sizeof(ctx->attr)); \
         } \
      } while (0)

   /* Macro to set values to context fields in the safe way */
   #define DSL_CTX_WRITE(ctx, err, attr, val) do { \
         if (sizeof(ctx->attr) != sizeof(val)) \
         { \
            DSL_DEBUG( DSL_DBG_ERR, (ctx, DSL_CRLF""DSL_CRLF"DSL_CTX_WRITE - Context field " \
               "and value size mismatch! Data will be NOT copied!!!"DSL_CRLF)); \
            err = DSL_ERR_MEMORY; \
         } \
         else \
         { \
            err = _DSL_CTX_ASSIGN_ANY(ctx, &(val), &(ctx->attr), sizeof(ctx->attr)); \
         } \
      } while (0)

   /* Macro to set scalar values to context fields in the safe way */
   #define DSL_CTX_WRITE_SCALAR(ctx, err, attr, val) \
      if (sizeof(ctx->attr) == 1) \
      { \
         err = _DSL_CTX_ASSIGN_8(ctx, (DSL_uint8_t)(val), &(ctx->attr)); \
      } \
      if (sizeof(ctx->attr) == 2) \
      { \
         err = _DSL_CTX_ASSIGN_16(ctx, (DSL_uint16_t)(val), &(ctx->attr)); \
      } \
      if (sizeof(ctx->attr) == 4) \
      { \
         err = _DSL_CTX_ASSIGN_32(ctx, (DSL_uint32_t)(val), &(ctx->attr)); \
      } \
      if (sizeof(ctx->attr) != 1 && sizeof(ctx->attr) != 2 \
         && sizeof(ctx->attr) != 4) \
      { \
         DSL_DEBUG( DSL_DBG_ERR, (ctx, "Couldn't write scalar " \
            "data with size different from 1, 2 or 4 bytes!!!"DSL_CRLF)); \
      }
#endif

/**
   Returns the number of elements of the array.

   \param array   the array variable name, [I]
*/
#define DSL_ARRAY_LENGTH(array) ((DSL_uint32_t)(sizeof(array)/sizeof((array)[0])))


/* ************************************************************************** */
/* * This functions are directly used from ioctl interface                  * */
/* * ==> All of this functions needs wrapper code for cli which has to be   * */
/* *     generated automatically or manual if automatic generation is not   * */
/* *     possible                                                           * */
/* ************************************************************************** */

/**
   This function generates Event according to the nEventType.

   \param pContext   - Pointer to dsl cpe library context structure, [I]
   \param nChannel   - Bearer channel, [I]
   \param nAccessDir - Access direction, DSL_UPSTREAM or DSL_DOWNSTREAM, [I]
   \param nXtuDir    - XTU direction, DSL_FAR_END or DSL_NEAR_END, [I]
   \param nEventType - Event type, [I]
   \param pData      - pointer to the event data. If no data present, specify DSL_NULL, [I]
   \param nDataSize  - Size of the event data. sizeof(one of the event union element size), [I]


   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
#ifndef SWIG
DSL_Error_t DSL_EventGenerate(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_AccessDir_t nAccessDir,
   DSL_XTUDir_t nXtuDir,
   DSL_EventType_t nEventType,
   DSL_EventData_Union_t *pData,
   DSL_uint16_t nDataSize);
#endif

#ifndef SWIG
DSL_Error_t DSL_HandleLinitValue(
   DSL_Context_t *pContext,
   const DSL_G997_LineInit_t nLinit,
   const DSL_G997_LineInitSubStatus_t nSub);
#endif

#ifndef SWIG
DSL_uint32_t DSL_AutobootStatusSet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_AutobootStatGet_t nStatus,
   DSL_IN DSL_FirmwareRequestType_t nFirmwareRequestType);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_INIT
*/
#ifndef SWIG
DSL_uint32_t DSL_Init
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_Init_t *pData
);
#endif

#ifndef SWIG
DSL_Error_t DSL_ModulesInit(
   DSL_IN DSL_Context_t *pContext);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_AUTOBOOT_LOAD_FIRMWARE
*/
#ifndef SWIG
DSL_uint32_t DSL_AutobootLoadFirmware
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_AutobootLoadFirmware_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_AUTOBOOT_CONTROL_SET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_AutobootControlSet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_AutobootControl_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_AUTOBOOT_CONFIG_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_AutobootConfigGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_AutobootConfig_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_AUTOBOOT_CONFIG_SET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_AutobootConfigSet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_OUT DSL_AutobootConfig_t *pData
);
#endif


/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_AUTOBOOT_STATUS_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_AutobootStatusGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_AutobootStatus_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_TEST_MODE_CONTROL_SET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_TestModeControlSet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_TestModeControl_t *pData
);
#endif

/** @} DRV_DSL_CPE_INIT */


/** \addtogroup DRV_DSL_CPE_COMMON
 @{ */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LINE_PATH_COUNTER_TOTAL_GET
*/
DSL_Error_t DSL_LinePathCounterTotalGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LinePathCounterTotal_t *pData
);
#endif /** #if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)*/

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_VERSION_INFORMATION_GET
*/
DSL_uint32_t DSL_VersionInformationGet
(
    DSL_IN DSL_Context_t *pContext,
    DSL_IN_OUT DSL_VersionInformation_t *pData
);

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LINE_STATE_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_LineStateGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LineState_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LINE_FEATURE_CONFIG_SET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_LineFeatureConfigSet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LineFeature_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LINE_FEATURE_CONFIG_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_LineFeatureConfigGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LineFeature_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LINE_FEATURE_STATUS_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_LineFeatureStatusGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LineFeature_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_FRAMING_PARAMETER_STATUS_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_FramingParameterStatusGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_FramingParameterStatus_t *pData
);
#endif

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LOW_LEVEL_CONFIGURATION_SET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_LowLevelConfigurationSet
(
   DSL_Context_t *pContext,
   DSL_LowLevelConfiguration_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LOW_LEVEL_CONFIGURATION_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_LowLevelConfigurationGet
(
   DSL_Context_t *pContext,
   DSL_LowLevelConfiguration_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_AUX_LINE_INVENTORY_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_AuxLineInventoryGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_AuxLineInventory_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_BAND_PLAN_SUPPORT_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_BandPlanStatusGet(
   DSL_Context_t *pContext,
   DSL_BandPlanStatus_t *pData);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_BAND_PLAN_SUPPORT_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_BandPlanSupportedGet
(
   DSL_Context_t *pContext,
   DSL_BandPlanSupport_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_EFM_MAC_CONFIG_SET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_EfmMacConfigSet
(
   DSL_Context_t *pContext,
   DSL_EFM_MacConfig_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_EFM_MAC_CONFIG_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_EfmMacConfigGet
(
   DSL_Context_t *pContext,
   DSL_EFM_MacConfig_t *pData
);
#endif
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LOOP_LENGTH_STATUS_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_LoopLengthStatusGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LoopLengthStatus_t *pData
);
#endif
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_UTOPIA_ADDRESS_CONFIG_SET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_UtopiaAddressConfigSet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_PhyAddressConfig_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_UTOPIA_ADDRESS_CONFIG_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_UtopiaAddressConfigGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_PhyAddressConfig_t *pData
);
#endif

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_POSPHY_ADDRESS_CONFIG_SET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_PosphyAddressConfigSet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_PhyAddressConfig_t *pData
);
#endif
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

#if (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1)
/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_POSPHY_ADDRESS_CONFIG_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_PosphyAddressConfigGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_PhyAddressConfig_t *pData
);
#endif
#endif /* (INCLUDE_DSL_CPE_API_VDSL_SUPPORT == 1) */

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_SystemInterfaceConfigSet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SystemInterfaceConfig_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_SystemInterfaceConfigGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SystemInterfaceConfig_t *pData
);
#endif

/** @} DRV_DSL_CPE_COMMON */


/** \addtogroup DRV_DSL_CPE_EVENT
 @{ */

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_EVENT_STATUS_MASK_CONFIG_SET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_EventStatusMaskConfigSet
(
   DSL_OpenContext_t *pOpenCtx,
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_EventStatusMask_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_EVENT_STATUS_MASK_CONFIG_GET
*/
#ifndef SWIG_TMP
DSL_uint32_t DSL_EventStatusMaskConfigGet
(
   DSL_OpenContext_t *pOpenCtx,
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_EventStatusMask_t *pData
);
#endif

/** @} DRV_DSL_CPE_EVENT */


/** \addtogroup DRV_DSL_CPE_DEBUG
 @{ */

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_DBG_MODULE_LEVEL_SET

   \note CLI Debug functions should be not generated automatically
*/
#ifndef SWIG
DSL_uint32_t DSL_DBG_ModuleLevelSet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_DBG_ModuleLevel_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_DBG_MODULE_LEVEL_GET

   \note CLI Debug functions should be not generated automatically
*/
#ifndef SWIG
DSL_uint32_t DSL_DBG_ModuleLevelGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_DBG_ModuleLevel_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_DBG_DEVICE_MESSAGE_SEND

   \note CLI Debug functions should be not generated automatically
*/
#ifndef SWIG
DSL_uint32_t DSL_DBG_DeviceMessageSend
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_DeviceMessage_t *pMsg
);
#endif

/** @} DRV_DSL_CPE_DEBUG */


/* ************************************************************************** */
/* * This functions are used internally only                                * */
/* * No generation of cli wrapper functions necessary.                      * */
/* * ==> SWIG preprocessor define hat to be set for ALL.                    * */
/* ************************************************************************** */

/**
   This function implements a driver internal (kernel space) specific cleanup
   functionality which is only necessary in case of using this DSL CPE API
   driver from within the kernel space.

   \param pOpenContext
      Pointer of type 'DSL_OpenContext_t' to be released.
      If the function return with DSL_SUCCESS the pointer and all associated
      data are successfully released.

   Return values are defined within the \ref DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code

   \remarks
   Supported by
   - Danube: ADSL-CPE
*/
#ifndef SWIG
DSL_void_t DSL_DriverCleanup(DSL_void_t);
#endif

/**
   This function implements a driver internal (kernel space) specific open
   functionality which is only necessary in case of using this DSL CPE API driver
   from within the kernel space. It refers to the fd open in case of using ioctl
   from application layer.

   \param nNum
      Device number
   \param pRefContext
      Reference to NULL pointer of type 'DSL_OpenContext_t'. The memory allocation
      and deletion will be done by the driver.
      If the function return with DSL_SUCCESS the pointer points to successfully
      allocated memory and has to be used for calling the DSL CPE_API functions
      later on.
      \attention For ALL later API function calls the pointer has to be used
                 (NOT the reference!!!)

   \return
   Return values are defined within the \ref DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code

   \remarks
   Supported by
   - Danube: ADSL-CPE
*/
#ifndef SWIG
DSL_Error_t DSL_DriverHandleGet
(
   DSL_IN     DSL_int_t     nNum,
   DSL_IN_OUT DSL_OpenContext_t **pRefOpenContext
);
#endif

/**
   This function implements a driver internal (kernel space) specific close
   functionality which is only necessary in case of using this DSL CPE API
   driver from within the kernel space. It refers to the fd close in case of
   using ioctl from application layer.

   \param pOpenContext
      Pointer of type 'DSL_OpenContext_t' to be released.
      If the function return with DSL_SUCCESS the pointer and all associated
      data are successfully released.

   Return values are defined within the \ref DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code

   \remarks
   Supported by
   - Danube: ADSL-CPE
*/
#ifndef SWIG
DSL_Error_t DSL_DriverHandleDelete
(
   DSL_IN DSL_OpenContext_t *pOpenContext
);
#endif

/**
   This function implements a driver internal (kernel space) specific cleanup
   functionality. It releases all memory allocated by structures linked with
   passed device context.

   \param pRefContext
      pointer to the device context
   \param bForce
      force decrement module usage count to 1 and memory release

   Return values are defined within the \ref DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code

   \remarks
   Supported by
   - Danube: ADSL-CPE
*/
#ifndef SWIG
DSL_Error_t DSL_DriverHandleCleanup
(
   DSL_IN DSL_devCtx_t *pDevContext,
   DSL_IN DSL_boolean_t bForce
);
#endif

/**
   This routine resets a DSL line

   \param pContext
      pointer to the DSL context
*/
#ifndef SWIG
DSL_Error_t DSL_LinkReset
(
   DSL_Context_t *pContext
);
#endif

/**
   This routine initializes a device

   \param   pContext       pointer to the DSL context
   \param   pInit          pointer to the DSL_Init_t structure
*/
#ifndef SWIG
DSL_Error_t DSL_DeviceInit
(
   DSL_Context_t *pContext,
   DSL_Init_t *pData
);
#endif

/**
   This routine prepares pointers in the DSL_Init_t structure
   to further use in the DSL_Init()

   \param   bIsInKernel    where from the initial call is performed
   \param   pInit          pointer to the DSL_Init_t structure
*/
#ifndef SWIG
DSL_Error_t DSL_InitDataPrepare
(
   DSL_Context_t *pContext,
   DSL_boolean_t bIsInKernel,
   DSL_Init_t *pInit
);
#endif

/**
   This routine releases a memory allocated by DSL_InitDataPrepare()

   \param   pInit          pointer to the DSL_Init_t structure
*/
#ifndef SWIG
DSL_void_t DSL_InitDataFree
(
   DSL_Context_t *pContext,
   DSL_Init_t *pInit
);
#endif

/**
   This routine releases a memory used by DSL context structure

   \param pContext
      pointer to the DSL context that should be released
*/
#ifndef SWIG
DSL_void_t DSL_Cleanup
(
   DSL_IN DSL_Context_t *pContext
);
#endif


/** \addtogroup DRV_DSL_CPE_COMMON
 @{ */

/**
   IO dispatcher routine for DSL CPE API common modules

   \param   bIsInKernel    where from the call is performed
   \param   nCommand       the ioctl command.
   \param   nArg           The address of data.

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_COMMON
 */
#ifndef SWIG
DSL_uint32_t DSL_IoctlHandle
(
   DSL_OpenContext_t *pOpenCtx,
   DSL_Context_t *pContext,
   DSL_boolean_t bIsInKernel,
   DSL_uint_t nCommand,
   DSL_uint32_t nArg
);
#endif

/**
   Updates nLineState field in pContext structure with current
   state of a modem

   \param pContext Pointer to dsl library context structure, [I]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_COMMON
 */
#ifndef SWIG
DSL_uint32_t DSL_LineStateUpdate
(
   DSL_Context_t *pContext
);
#endif

/**
   This function returns a handle to low-level device driver

   \param   nNum           The number of a device

   \return  The handle to the device

   \ingroup DRV_DSL_CPE_COMMON
 */
#ifndef SWIG
DSL_DEV_Handle_t DSL_LowDeviceGet(DSL_devCtx_t *pDevCtx);
#endif

/**
   Initialization routine to enable dual latency before firmware start

   \param pContext Pointer to dsl library context structure, [I]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_INIT
*/
#ifndef SWIG
DSL_uint32_t DSL_DualLatency_FirmwareInit
(
   DSL_Context_t *pContext
);
#endif

/**
   This routine handles a timeout that occured.

   \param pContext   DSL CPE library context
   \param nEventType Include information on which timeout has been occured.
                     The possible values are defined by
                     \ref DSL_TimeoutEvent_t, [I]
   \param nTimeoutID Includes the timeout element id. This unique value
                     identifies the timeout event and might be compared to
                     a stored value returned from \ref DSL_Timeout_AddEvent, [I]

   \return
   - DSL_Success Timeout event handled successfully
   - DSL_Error Error during handling of timeout event
*/
#ifndef SWIG
DSL_Error_t DSL_OnTimeoutEvent(
   DSL_Context_t *pContext,
   DSL_int_t nEventType,
   DSL_uint32_t nTimeoutID);
#endif

/** @} DRV_DSL_CPE_COMMON */

#if (defined(INCLUDE_DSL_ADSL_MIB))
   #include "drv_dsl_cpe_intern_mib.h"
#endif

#if (defined(INCLUDE_ADSL_LED))
   #include "drv_dsl_cpe_intern_led.h"
#endif

#if (defined(INCLUDE_DSL_CEOC))
   #include "drv_dsl_cpe_intern_ceoc.h"
#endif

#if (defined(INCLUDE_DSL_PM))
   #include "drv_dsl_cpe_intern_pm.h"
#endif

#define DSL_DEVICE_LOWHANDLE(X) (X)->pDevCtx->lowHandle

/**
   Macro for checking if the given pointer is valid, which means NOT equal
   DSL_NULL.
   \param pPtr   Pointer which will be checked against DSL_NULL, [I]
*/
#ifdef DRV_DSL_CPE_FORCE_MACROS
   #define _DSL_CHECK_CTX_POINTER(pPtr) do { \
      if ((pPtr) == DSL_NULL) { DSL_DEBUG_SET_ERROR(DSL_ERR_POINTER); \
           DSL_DEBUG_HDR(DSL_DBG_ERR, (pPtr, "Invalid context pointer!")); \
           nErrCode |= DSL_ERR_POINTER; } } while (0)

   #define DSL_CHECK_CTX_POINTER(pPtr) _DSL_CHECK_CTX_POINTER(pPtr)
#else
   DSL_Error_t
   _DSL_CHECK_CTX_POINTER(
      DSL_Context_t *pContext
   );

   #define DSL_CHECK_CTX_POINTER(pPtr) nErrCode |= _DSL_CHECK_CTX_POINTER(pPtr)
#endif

/**
   Macro for checking if the given pointers are valid, which means NOT equal
   DSL_NULL.
   The first pointer should be the library context pointer and the second a
   various one.
   \param pCtx   Context pointer which will be checked against DSL_NULL, [I]
   \param pPtr   Additional pointer which will be checked against DSL_NULL, [I]
*/
#ifdef DRV_DSL_CPE_FORCE_MACROS
   #define DSL_CHECK_POINTER(pCtx, pPtr) \
      do { DSL_CHECK_CTX_POINTER(pCtx); \
           if (nErrCode == DSL_SUCCESS && pPtr == DSL_NULL) { \
           pCtx->nErrNo = DSL_ERR_POINTER; \
           DSL_DEBUG_HDR(DSL_DBG_ERR, (pCtx, "Invalid data pointer!")); \
           nErrCode |= DSL_ERR_POINTER; } } while(0)

   #define DSL_CHECK_POINTER(pCtx, pPtr) _DSL_CHECK_POINTER(pCtx, pPtr)
#else
   DSL_Error_t
   _DSL_CHECK_POINTER(
      DSL_Context_t *pContext,
      DSL_void_t *pPtr
   );

   #define DSL_CHECK_POINTER(pCtx, pPtr) nErrCode |= _DSL_CHECK_POINTER(pCtx, pPtr)
#endif

#ifdef DRV_DSL_CPE_FORCE_MACROS
   #define _DSL_CHECK_MODEM_IS_READY() \
      do { DSL_CHECK_POINTER(pContext, pContext->pDevCtx); \
         if (nErrCode == DSL_SUCCESS && DSL_MUTEX_LOCK(pContext->bspMutex)) \
         { \
            DSL_DEBUG(DSL_DBG_ERR, (pContext, DSL_CRLF"getting mei driver semaphore " \
               "failed!")); \
            nErrCode |= DSL_ERROR; \
         } \
         if ((nErrCode == DSL_SUCCESS) && (DSL_DEV_ModemIsReady(pContext) == DSL_FALSE) \
         { \
            pContext->nErrNo = DSL_ERR_MODEM_NOT_READY; \
            DSL_DEBUG_HDR(DSL_DBG_ERR, (pContext, "Modem is not ready!")); \
            DSL_MUTEX_UNLOCK(pContext->bspMutex); \
            nErrCode |= DSL_ERR_MODEM_NOT_READY; } DSL_MUTEX_UNLOCK(pContext->bspMutex); } \
      while (0)

   #define DSL_CHECK_MODEM_IS_READY() _DSL_CHECK_MODEM_IS_READY()
#else
   DSL_Error_t
   _DSL_CHECK_MODEM_IS_READY(
      DSL_Context_t *pContext
   );

   #define DSL_CHECK_MODEM_IS_READY() nErrCode |= _DSL_CHECK_MODEM_IS_READY(pContext)
#endif

/**
   Macro for checking the given bearer channel number. If the channel number
   is out of the possible range it returns an nErrCode and prints a debug
   message.
   \param nChannel  Bearer channel number which will be checked for valid range, [I]
*/
#ifdef DRV_DSL_CPE_FORCE_MACROS
   #define _DSL_CHECK_CHANNEL_RANGE(nChannel) \
     do { \
        if (nChannel >= DSL_CHANNELS_PER_LINE) \
        { \
           DSL_DEBUG_SET_ERROR(DSL_ERR_CHANNEL_RANGE); \
           DSL_DEBUG(DSL_DBG_ERR, \
              (pContext, "DSL: invalid bearer channel number %d! (only bearer " \
               "channels in the range of 0..%d are supported)" DSL_CRLF, \
              nChannel, (DSL_CHANNELS_PER_LINE - 1))); \
           nErrCode |= DSL_ERR_CHANNEL_RANGE; \
        } \
     } while(0)
   #define DSL_CHECK_CHANNEL_RANGE(nChannel) _DSL_CHECK_CHANNEL_RANGE(nChannel)
#else
   DSL_Error_t
   _DSL_CHECK_CHANNEL_RANGE(
      DSL_Context_t *pContext,
      DSL_uint16_t nChannel
   );

   #define DSL_CHECK_CHANNEL_RANGE(nChannel) nErrCode |= \
      _DSL_CHECK_CHANNEL_RANGE(pContext, nChannel)
#endif

#ifdef DRV_DSL_CPE_FORCE_MACROS
   #define _DSL_CHECK_ATU_DIRECTION(nDirection) \
      do { \
         if ((nDirection != DSL_NEAR_END) && (nDirection != DSL_FAR_END)) \
         { \
            DSL_DEBUG(DSL_DBG_ERR, \
               (pContext, "DSL: invalid ATU direction!" DSL_CRLF)); \
            nErrCode |= DSL_ERR_DIRECTION; \
         } \
      } while(0)
   #define DSL_CHECK_ATU_DIRECTION(nDirection) _DSL_CHECK_ATU_DIRECTION(nDirection)
#else
   DSL_Error_t
   _DSL_CHECK_ATU_DIRECTION(
      DSL_Context_t *pContext,
      DSL_XTUDir_t nDirection
   );

   #define DSL_CHECK_ATU_DIRECTION(nDirection) nErrCode |= \
      _DSL_CHECK_ATU_DIRECTION(pContext, nDirection)
#endif

#ifdef DRV_DSL_CPE_FORCE_MACROS
   #define _DSL_CHECK_DIRECTION(nDirection) \
      do { \
         if ((nDirection != DSL_DOWNSTREAM) && (nDirection != DSL_UPSTREAM)) \
         { \
            DSL_DEBUG(DSL_DBG_ERR, \
               (pContext, "DSL: invalid direction!" DSL_CRLF)); \
            nErrCode |= DSL_ERR_DIRECTION; \
         } \
      } while(0)
   #define DSL_CHECK_DIRECTION(nDirection) _DSL_CHECK_DIRECTION(nDirection)
#else
   DSL_Error_t
   _DSL_CHECK_DIRECTION(
      DSL_Context_t *pContext,
      DSL_AccessDir_t nDirection
   );

   #define DSL_CHECK_DIRECTION(nDirection) nErrCode |= \
      _DSL_CHECK_DIRECTION(pContext, nDirection)
#endif

#define DSL_CHECK_ERR_CODE() if (nErrCode != DSL_SUCCESS) { return nErrCode; }

/**
   Firmware download routine

   \param pContext      Pointer to dsl library context structure, [I]
   \param pFirmware1    1st firmware pointer, [I]
   \param nSize1        Size of 1st firmware, [I]
   \param pFirmware2    2nd firmware pointer, [I]
   \param nSize2        Size of 2nd firmware, [I]
   \param pLoff         Offset from firmware beginning, [I]
   \param pCurrentOff   Offset from block beggining after operation is done, [O]
   \param bLastChunk    Last chunk flag, [I]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_INIT
*/
#ifndef SWIG
DSL_Error_t DSL_FwDownload
(
   DSL_Context_t *pContext,
   const DSL_char_t *pFw1,
   DSL_uint32_t nSize1,
   const DSL_char_t *pFw2,
   DSL_uint32_t nSize2,
   DSL_int32_t *pLoff,
   DSL_int32_t *pCurrentOff,
   DSL_boolean_t bLastChunk
);
#endif

/**
   This function is intended for internal use to have possibility to
   set line state from different modules
*/
#ifndef SWIG
DSL_Error_t DSL_LineStateSet
(
   DSL_Context_t *pContext,
   DSL_LineStateValue_t nNewLineState
);
#endif

/**
   This function retrieves different information about line in showtime state
   and updates DSL context structure with actual values if needed
*/
#ifndef SWIG
DSL_Error_t DSL_ShowtimeStatusUpdate
(
   DSL_Context_t *pContext,
   DSL_boolean_t bInit
);
#endif

/**
   This function resets DSL Context data to its default values
*/
#ifndef SWIG
DSL_Error_t DSL_CtxDataUpdate
(
   DSL_Context_t *pContext
);
#endif

/* Events stuff */

/**
   This function places an event into FIFO. It also wakes up a poll routine.

   \param pEvent        Pointer to event structure to be added to FIFO [I]
   \param nDataSize     The size of event data [I]

   Return values are defined within the \ref DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code

   \remarks
   Supported by
   - Danube: ADSL-CPE
*/
#ifndef SWIG
DSL_Error_t DSL_EventQueue
(
   DSL_IN DSL_EventStatusData_t *pEvent,
   DSL_IN DSL_uint32_t nDataSize
);
#endif

/**
   This function gets an event from upper layer software FIFO

   \param pOpenContext  Pointer to upper layer context structure, [I]
   \param pEvent        Pointer to store event structure from FIFO [I]

   Return values are defined within the \ref DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code

   \remarks
   Supported by
   - Danube: ADSL-CPE
*/
#ifndef SWIG
DSL_Error_t DSL_EventUnqueue
(
   DSL_IN DSL_OpenContext_t *pOpenContext,
   DSL_IN DSL_EventStatusData_t *pEvent
);
#endif

/* Helpers stuff */

/**
   This function converts a given value in unsigned 16-bit value in 2'complement
   format to its according signed 16-bit format.
   The type of 2'complement can be specified, for example:
   nsVal16 = 0x200, nBits = 10 ==> nuVal16 = -512

   \param nsVal16
      Specifies the unsigned 16-bit value that shall be converted, [I]
   \param nBits
      Specifies how many bits are used for 2'complement conversion.
      The bit that includes the sign is included, [I]
      \note It makes only sense to use this function in a range of
            2 <= nBits <= 15
            In case of nBits = 16 a simple cast will do the right thing.
   \param nuVal16
      Returns the converted 16-bit int value, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
#ifndef SWIG
DSL_Error_t DSL_TwosComplement16_HexToInt
(
   DSL_Context_t *pContext,
   DSL_uint16_t nuVal16,
   DSL_char_t nBits,
   DSL_int16_t *nsVal16
);
#endif

/*
   DSL IOCTL handler helpers stuff
*/

/** This type represents handler function signature */
typedef DSL_Error_t (*DSL_IoctlHandlerHelperFunc_t)(
   DSL_Context_t *pContext,
   DSL_void_t *pArg);

typedef DSL_Error_t (*DSL_IoctlInstanceHandlerHelperFunc_t)(
   DSL_OpenContext_t *pOpenCtx,
   DSL_Context_t *pContext,
   DSL_void_t *pArg);

/** The type represents handler function type */
typedef enum DSL_IoctlHandlerHelperType {
   DSL_IOCTL_HELPER_UNKNOWN = 0,
   DSL_IOCTL_HELPER_GET = 1,
   DSL_IOCTL_HELPER_SET = 2
} DSL_IoctlHandlerHelperType_t;

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_INTERN_H */
