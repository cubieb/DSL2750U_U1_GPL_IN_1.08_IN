/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_DEVICE_H
#define _DRV_DSL_CPE_DEVICE_H

#ifdef __cplusplus
   extern "C" {
#endif

#ifndef SWIG
/** \addtogroup DRV_DSL_DEVICE
 @{ */

#if defined (INCLUDE_DSL_CPE_API_DANUBE) || defined (INCLUDE_DSL_CPE_API_AMAZON_SE)
   #include "drv_dsl_cpe_device_danube.h"
#elif defined (INCLUDE_DSL_CPE_API_VINAX)
      #include "drv_dsl_cpe_device_vinax.h"
   #else
      #error "Device is not defined!"
   #endif
typedef DSL_DEV_Device_t* DSL_DEV_Handle_t;

/** This structure represents Device context structure */
typedef struct DSL_DEV_Context
{
   /** Handle to the corresponding low level driver */
   DSL_DEV_Handle_t lowHandle;
#ifdef INCLUDE_DSL_CPE_API_VINAX
   /** Handle to the corresponding low level driver for the autonomous messages processing*/
   DSL_DEV_Handle_t nfc_lowHandle;
#endif /* #ifdef INCLUDE_DSL_CPE_API_VINAX*/
   /** Specifies a usage count of this device  */
   DSL_uint32_t nUsageCount;

   /** Link to the DSL_Context structure instance */
   DSL_Context_t *pContext;

   /** Number of device */
   DSL_int_t nNum;

   /** Firmware loaded flag */
   DSL_boolean_t bFirmwareReady;

   /** Whether this device will be initialized at the first time */
   DSL_boolean_t bFirstPowerOn;

   /** Internal structure for each device implementation */
   DSL_DEV_Data_t data;
} DSL_DEV_Context_t;

typedef DSL_DEV_Context_t DSL_devCtx_t;

extern DSL_devCtx_t ifxDevices[DSL_MAX_DEVICE_INSTANCES];

DSL_DEV_Handle_t
DSL_DEV_DriverHandleGet(
   DSL_int_t nMaj,
   DSL_int_t nNum
);

DSL_Error_t
DSL_DEV_DriverHandleDelete(
   DSL_DEV_Handle_t handle
);

DSL_Error_t DSL_DEV_FwDownload
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

DSL_boolean_t DSL_DEV_ModemIsReady(DSL_Context_t *pContext);

/** Device dependent handling of L3 power state */
DSL_Error_t DSL_DEV_AutobootHandleL3
(
   DSL_Context_t *pContext,
   DSL_boolean_t bL3Forced
);

/** Device dependent part of DSL_AutobootHandleStart() */
DSL_Error_t DSL_DEV_AutobootHandleStart
(
   DSL_Context_t *pContext,
   DSL_boolean_t bLoopTest,
   DSL_boolean_t bShortInit
);

/** Device dependent part of DSL_AutobootHandleTraining() */
DSL_Error_t DSL_DEV_AutobootHandleTraining
(
   DSL_Context_t *pContext
);

/** Device dependent part of DSL_AutobootHandleRestart() */
DSL_Error_t DSL_DEV_AutobootHandleRestart
(
   DSL_Context_t *pContext
);

/** Device dependent part of DSL_AutobootHandleException() */
DSL_Error_t DSL_DEV_AutobootHandleException
(
   DSL_Context_t *pContext
);

DSL_Error_t DSL_DEV_AutobootHandleFwRequest(
   DSL_Context_t *pContext,
   DSL_boolean_t *pbRestart
);

/**
   This routine is a device specific implementation of
   a device initialization

   \param   pContext       pointer to the DSL context
   \param   pInit          pointer to the DSL_Init_t structure
*/

#ifndef SWIG

DSL_Error_t
DSL_DEV_DeviceInit
(
   DSL_Context_t *pContext,
   DSL_Init_t *pData
);

#endif

/**
   This routine resets a DSL line

   \param pContext
      pointer to the DSL context
*/

#ifndef SWIG

DSL_Error_t DSL_DEV_LinkReset
(
   DSL_Context_t *pContext
);

#endif

#ifdef INCLUDE_DSL_CPE_API_VINAX
#ifndef SWIG
DSL_Error_t DSL_DEV_LinePathCounterTotalGet
(
   DSL_Context_t *pContext,
   const DSL_uint8_t nChannel,
   DSL_LinePathCounterTotalData_t *pData
);
#endif
#endif

#ifndef SWIG
DSL_Error_t DSL_DEV_LinkActivate
(
   DSL_Context_t *pContext
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_LOW_LEVEL_CONFIGURATION_SET
*/
#ifndef SWIG
DSL_uint32_t DSL_DEV_LowLevelConfigurationSet(
   DSL_Context_t *pContext,
   DSL_DeviceLowLevelConfig_t *pData);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_SET
*/
#ifndef SWIG
DSL_uint32_t DSL_DEV_SystemInterfaceConfigSet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SystemInterfaceConfig_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_CONFIG_GET
*/
#ifndef SWIG
DSL_uint32_t DSL_DEV_SystemInterfaceConfigGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SystemInterfaceConfig_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_SYSTEM_INTERFACE_STATUS_GET
*/
#ifndef SWIG
DSL_uint32_t DSL_DEV_SystemInterfaceStatusGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_SystemInterfaceStatus_t *pData
);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_UTOPIA_ADDRESS_CONFIG_SET
*/
#ifndef SWIG
DSL_uint32_t DSL_DEV_UtopiaAddressConfigSet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_PhyAddressConfigData_t *pData);
#endif

/**
   For a detailed description please refer to the equivalent ioctl
   \ref DSL_FIO_UTOPIA_ADDRESS_CONFIG_GET
*/
#ifndef SWIG
DSL_uint32_t DSL_DEV_UtopiaAddressConfigGet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_PhyAddressConfigData_t *pData);
#endif

/**
   This routine is a device specific implementation of
   the DSL_InitDataPrepare() function

   \param   bIsInKernel    where from the initial call is performed
   \param   pInit          pointer to the DSL_Init_t structure
*/

#ifndef SWIG

DSL_Error_t
DSL_DEV_InitDataPrepare
(
   DSL_Context_t *pContext,
   DSL_boolean_t bIsInKernel,
   DSL_Init_t *pInit
);

#endif

/**
   This routine is a device specific implementation of
   the DSL_InitDataFree() function

   \param   pInit          pointer to the DSL_Init_t structure
*/

#ifndef SWIG

DSL_void_t
DSL_DEV_InitDataFree(
   DSL_Init_t *pInit
);

#endif

/**
   This routine calls low level driver ioctl handler

   \param pContext
      pointer to the DSL context
*/

#ifndef SWIG

DSL_Error_t DSL_DEV_BspIoctl
(
   DSL_Context_t *pContext,
   DSL_uint_t nCommand,
   DSL_uint32_t *nArg
);

#endif

/**
   Device implementation of firmware version string retreive routine

   \param pContext    Pointer to dsl library context structure, [I]
   \param pString     Pointer to FW version string, [O]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_DEVICE
*/

#ifndef SWIG
DSL_Error_t DSL_DEV_FirmwareVersionGet(
   DSL_Context_t *pContext,
   DSL_char_t    *pString);
#endif

/**
   Device implementation of chip version string retreive routine

   \param pContext    Pointer to dsl library context structure, [I]
   \param pString     Pointer to Chip HW version string, [O]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_DEVICE
*/

#ifndef SWIG
DSL_Error_t DSL_DEV_ChipHardwareVersionGet(
   DSL_Context_t *pContext,
   DSL_char_t    *pString);
#endif

/**
   Device implementation of chip name string retreive routine

   \param pContext    Pointer to dsl library context structure, [I]
   \param pString     Pointer to Device Driver version string, [O]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_DEVICE
*/

#ifndef SWIG
DSL_Error_t DSL_DEV_DriverVersionGet(
   DSL_Context_t *pContext,
   DSL_char_t    *pString);
#endif

/**
   Device implementation of line state getting routine

   \param pContext      Pointer to dsl library context structure, [I]
   \param pnLineState   Reference to value of type LineStateValue_t where
                        current line state should be placed

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_DEVICE
*/

#ifndef SWIG

DSL_Error_t DSL_DEV_LineStateGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_LineStateValue_t *pnLineState
);

#endif

/** This function retrieves different information about line in showtime state
    and updates DSL context structure with actual values if needed */
DSL_Error_t DSL_DEV_ShowtimeStatusUpdate
(
   DSL_Context_t *pContext,
   DSL_boolean_t bInit
);

/**
   Device implementation of DSL_FramingParameterStatusGet() routine

   \param pContext      Pointer to dsl library context structure, [I]
   \param pData         Reference to room where framing parameters should be
                        saved

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_DEVICE
*/
DSL_uint32_t DSL_DEV_FramingParameterStatusGet
(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN_OUT DSL_FramingParameterStatus_t *pData
);

/**
   Send device low-level message*/
#ifndef SWIG
DSL_Error_t DSL_DEV_DBG_DeviceMessageSend(
   DSL_Context_t *pContext,
   DSL_DeviceMessageData_t *pMsg);
#endif


#ifndef SWIG
DSL_Error_t DSL_DEV_OnTimeoutEvent(
   DSL_Context_t *pContext,
   DSL_int_t nEventType,
   DSL_uint32_t nTimeoutID);
#endif

/**
   This enum represents the action to be performed by autoboot thread
   after showtime status is updated
*/
typedef enum DSL_DEV_RebootCheckResult
{
   DSL_DEV_REBOOT_UNHANDLED = 0,
   DSL_DEV_REBOOT_PENDING = 1,
   DSL_DEV_REBOOT_NO_ACTION = 2
} DSL_DEV_RebootCheckResult_t;

/**
   This function is called by autoboot thread to check whether additional
   reboot checks should be performed or not
*/
DSL_Error_t DSL_DEV_ShowtimeRebootCheck
(
   DSL_Context_t *pContext,
   DSL_DEV_RebootCheckResult_t *pResult
);

/* LED block */

/**
   Device implementation of LED module firmware initialization routine

   \param pContext Pointer to dsl library context structure, [I]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code

   \ingroup DRV_DSL_CPE_INIT
*/

#ifndef SWIG

DSL_uint32_t
DSL_DEV_LED_FirmwareInit
(
   DSL_Context_t *pContext
);

#endif

/** @} DRV_DSL_DEVICE */

#endif /* #ifndef SWIG*/

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_DEVICE_H */
