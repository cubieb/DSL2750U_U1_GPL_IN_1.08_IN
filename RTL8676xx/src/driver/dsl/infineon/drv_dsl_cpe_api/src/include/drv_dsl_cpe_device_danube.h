/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_DEVICE_DANUBE_H
#define _DRV_DSL_CPE_DEVICE_DANUBE_H

#ifdef __cplusplus
   extern "C" {
#endif

#include "drv_dsl_cpe_api.h"
#include "drv_dsl_cpe_cmv_danube.h"

/* Include for the low level driver interface header file */
#if defined (INCLUDE_DSL_CPE_API_DANUBE)
   #include "asm/danube/danube_mei_bsp.h"
#elif defined (INCLUDE_DSL_CPE_API_AMAZON_SE)
   #include "asm/amazon_se/amazon_se_mei_bsp.h"
#else
   #error "Device is not defined!"
#endif

#define DSL_MAX_LINE_NUMBER 0
#ifdef INCLUDE_DSL_CPE_TRACE_BUFFER
   /** The length of showtome event logging buffer in the 16-bit words */
   #define DSL_DEV_SHOWTIME_EVENT_LOGGING_BUFFER_LENGTH 120
#endif

#if defined (INCLUDE_DSL_CPE_API_DANUBE)
   #define DSL_CHIPSET_TYPE_STRING "Ifx-Danube"
#elif defined (INCLUDE_DSL_CPE_API_AMAZON_SE)
   #define DSL_CHIPSET_TYPE_STRING "Ifx-Amazon-SE"
#endif

/** Timeout setting for far end status in seconds */
#define DSL_DEV_TIMEOUT_FE_STATUS               12

/** Timeout setting for HDLC CEOC data, ms */
#define DSL_DEV_TIMEOUT_HDLC_CEOC               5000

/** \addtogroup DRV_DSL_DEVICE
 @{ */


#if defined (INCLUDE_DSL_CPE_API_DANUBE)
   #define DSL_MIN_FW_VERSION_PTM   2, 1, 2, 12
   #define DSL_MIN_FW_VERSION_CB    2, 2, -1, -1
#elif defined (INCLUDE_DSL_CPE_API_AMAZON_SE)
   #define DSL_MIN_FW_VERSION_PTM   3, 1, 0, 5
   #define DSL_MIN_FW_VERSION_CB    3, 2, -1, -1
#endif

#define DSL_CPE_CMV_BYTE_SIZE   1 << 14
#define DSL_CPE_CMV_DWORD_SIZE  2 << 14
#define DSL_CPE_CMV_WORD_SIZE   0 << 14


/**
   Defines the return types of version processing functions
   \ref DSL_DEV_CheckFirmwareVersion
   \ref DSL_DEV_CheckDeviceVersion
*/
typedef enum
{
   /** Version processing is not possible due to an error */
   DSL_VERSION_ERROR = 0,
   /** Version is not compared because of different common firmware types
       Currently this indicates a mismatch of the ApplicationType (Annex) only */
   DSL_VERSION_MISMATCH = 1,
   /** The version actually used is smaller than the version to check */
   DSL_VERSION_SMALLER = 2,
   /** The version actually used is equal to the version to check */
   DSL_VERSION_EQUAL = 3,
   /** The version actually used is bigger than the version to check */
   DSL_VERSION_BIGGER = 4,
   /** Delimiter only */
   DSL_VERSION_LAST = 5
} DSL_DEV_VersionCheck_t;

/**
   Includes version information for used firmware.
   The complete version information is extractzed from CMV INFO 54 0/1
*/
typedef struct
{
   /**
   Defines whether the included version information is valid or not */
   DSL_boolean_t bValid;
   /*
   Information source
      CMV INFO 54 0, Bit 0..7
   Meaning
      - 1: Amazon
      - 2: Danube
      - 3: Amazon-SE */
   DSL_uint8_t nMajorVersion;
   /*
   Information source
      CMV INFO 54 0, Bit 8..15
   Meaning
      Defines the feature set (FS) which indicates the main feature release
      versions */
   DSL_uint8_t nMinorVersion;
   /*
   Information source
      CMV INFO 54 1, Bit 14..15
   Meaning
      - 0: Released
      - 1: Internal version */
   DSL_uint8_t nReleaseState;
   /*
   Information source
      CMV INFO 54 1, Bit 8..13
   Meaning
      - 1: Annex A firmware version
      - 2: Annex B firmware version */
   DSL_uint8_t nApplicationType;
   /*
   Information source
      CMV INFO 54 1, Bit 4..7
   Meaning
      Major number that will be increased for consecutive firmware version
      during development within one feature set. */
   DSL_uint8_t nExternalVersion;
   /*
   Information source
      CMV INFO 54 1, Bit 0..3
   Meaning
      Minor number that will be increased for consecutive firmware version
      during development within one feature set. */
   DSL_uint8_t nInternalVersion;
} DSL_FwVersion_t;

/**
   Includes version information for used chip.
*/
typedef struct
{
   /**
   Defines whether the included version information is valid or not */
   DSL_boolean_t bValid;
   /**
   Major version of used device */
   DSL_uint8_t nMajorVersion;
   /**
   Minor version of used device */
   DSL_uint8_t nMinorVersion;
} DSL_ChipVersion_t;

/**
   Includes version information for used Low Level Mei BSP Driver.
*/
typedef struct
{
   /**
   Defines whether the included version information is valid or not */
   DSL_boolean_t bValid;
   /**
   Major version of used low level driver */
   DSL_uint8_t nMajorVersion;
   /**
   Minor version of used low level driver */
   DSL_uint8_t nMinorVersion;
   /**
   Build revision of used low level driver */
   DSL_uint8_t nBuild;
} DSL_MeiSwVersion_t;

/**
   Version information structure that includes version information about the
   firmware, the device and the low level driver.
   The parts will be updated at different point
   - firmware version after successful firmware download
   - device and low level driver version at open of the DSL CPE API
*/
typedef struct
{
   /**
   Detailed firmware version information */
   DSL_FwVersion_t fwVersion;
   /**
   Detailed chip version information */
   DSL_ChipVersion_t chipVersion;
   /**
   Detailed MEI BSP Low level driver version information */
   DSL_MeiSwVersion_t meiSwVersion;
} DSL_Version_t;

typedef struct
{
   /** Saved counters */
   DSL_uint16_t nErrS_L_count[4];

#if defined(INCLUDE_DSL_PM)
   /** Line inititalization counters */
   DSL_PM_LineInitData_t lineInitCounters;
#endif

   /** internal adsl mode values for Danube */
   DSL_uint16_t nAdslMode, nAdslMode1;

#ifdef INCLUDE_DSL_CEOC
   /** flag will be set if CEOC data is received */
   DSL_boolean_t bCeocRx;
   DSL_uint16_t nCeocReadIndex;
#endif
   DSL_uint8_t nUtopiaAddr[DSL_CHANNELS_PER_LINE];
   DSL_SystemInterfaceConfigData_t sysCIF;
   DSL_SystemInterfaceConfigData_t sysCIFSts;

   DSL_Version_t version;
} DSL_DEV_Data_t;

/*
   Structure used to build table which includes a list of Danube message IDs
*/
typedef struct
{
   /**
   Goup ID of message(s) */
   DSL_int_t nGroupId;
   /**
   16 Bit address (start of range) */
   DSL_uint16_t nAdrFirst;
   /**
   16 Bit address (stop of range) */
   DSL_uint16_t nAdrLast;
} DSL_DEV_MsgIdRange_t;

/** Dummy types. Not used for the DANUBE device*/
typedef struct
{
   DSL_uint8_t dummy;
} DSL_DEV_Clockout_t;

typedef struct
{
   DSL_uint8_t dummy;
} DSL_DEV_LineMode_t;

typedef struct
{
   DSL_uint8_t dummy;
} DSL_DEV_RxTxGainSelection_t;

DSL_void_t DSL_DANUBE_CmvPrepare
(
   DSL_Context_t *pContext,
   DSL_uint8_t nOpcode,
   DSL_uint8_t nGroup,
   DSL_uint16_t nAddress,
   DSL_uint16_t nIndex,
   DSL_int_t nSize,
   DSL_uint16_t * pData,
   DSL_uint16_t *pMsg
);

DSL_Error_t DSL_DANUBE_CmvSend
(
   DSL_Context_t *pContext,
   DSL_uint16_t *pTxMsg,
   DSL_boolean_t bReply,
   DSL_uint16_t *pRxMsg
);

DSL_Error_t DSL_DANUBE_CmvRead
(
   DSL_Context_t *pContext,
   DSL_uint8_t nGroup,
   DSL_uint16_t nAddress,
   DSL_uint16_t nIndex,
   DSL_int_t nSize,
   DSL_uint16_t * pData
);

DSL_Error_t DSL_DANUBE_CmvWrite
(
   DSL_Context_t *pContext,
   DSL_uint8_t nGroup,
   DSL_uint16_t nAddress,
   DSL_uint16_t nIndex,
   DSL_int_t nSize,
   DSL_uint16_t * pData
);

/*
   Returns the current latency path configuration.

   \param pContext Pointer to DSL CPE API context structure, [I]
   \param nChannel Channel for which the configuration has to be returned, [I]
   \param nLPath   Returns the current configuration of the latency path, [O]

   \return error code

   \ingroup Internal
*/
DSL_Error_t DSL_DANUBE_ActLatencyGet
(
   DSL_Context_t *pContext,
   DSL_uint16_t nChannel,
   DSL_LatencyPath_t *nLPath
);

DSL_Error_t DSL_DANUBE_TotalCountersWrite
(
   DSL_Context_t *pContext
);

DSL_uint32_t DSL_DANUBE_ChannelStatusGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN DSL_uint16_t nChannel,
   DSL_IN DSL_AccessDir_t nDirection,
   DSL_IN_OUT DSL_G997_ChannelStatusData_t *pData
);

/**
   Send hdlc packets

   \param   pContext      pointer to the DSL context
   \param   bDeviceLock   whether device should be locked or not
   \param   pHdlcPkt      pointer to hdlc packet
   \param   nHdlcPktLen   the number of bytes to send
   \return  success or failure.
   \ingroup Internal
 */
DSL_Error_t DSL_DANUBE_HdlcWrite
(
   DSL_Context_t *pContext,
   DSL_boolean_t bDeviceLock,
   DSL_uint8_t *pHdlcPkt,
   DSL_int_t nHdlcPktLen
);

/**
   Read the hdlc packets

   \param   pContext       pointer to the DSL context
   \param   bDeviceLock    whether device should be locked or not
   \param   pHdlcPkt       pointer to hdlc packet
   \param   nMaxHdlcPktLen The maximum number of bytes to read
   \param   pnRead         pointer to a variable to save the number of bytes
                           which was read to
   \return  success or failure.
   \ingroup Internal
*/
DSL_Error_t DSL_DANUBE_HdlcRead
(
   DSL_Context_t *pContext,
   DSL_boolean_t bDeviceLock,
   DSL_uint8_t *pHdlcPkt,
   DSL_int_t nMaxHdlcPktLen,
   DSL_int_t *pnRead
);

#define DSL_ME_HDLC_IDLE 0
#define DSL_ME_HDLC_MSG_QUEUED 2
#define DSL_ME_HDLC_MSG_SENT 3
#define DSL_ME_HDLC_RESP_RCVD 4
#define DSL_ME_HDLC_UNRESOLVED 1
#define DSL_ME_HDLC_RESOLVED 2

/**
   This function writes inventory info to the device
*/
DSL_uint32_t DSL_DANUBE_LineInventoryNeWrite(
   DSL_IN DSL_Context_t *pContext
);

/**
   This function reads inventory info from the device
*/
DSL_uint32_t DSL_DANUBE_LineInventoryNeRead(
   DSL_IN DSL_Context_t *pContext
);

/**
   This function reads inventory info from the device
*/
DSL_uint32_t DSL_DANUBE_LineInventoryFeRead(
   DSL_IN DSL_Context_t *pContext
);

/**
   This function updates channel status information within DSL Context
*/
DSL_uint32_t DSL_DANUBE_ChannelStatusUpdate(
   DSL_IN DSL_Context_t *pContext);

/* G997 block */
/**
   This function stores DELT data to the DSL Context structure for futher readings
*/
DSL_Error_t DSL_DANUBE_G997_LoopDiagnosticCompleted
(
   DSL_Context_t *pContext
);

#if defined(INCLUDE_DSL_CEOC)
/* CEOC internal functions */
/** CEOC module low level driver events handler */
DSL_uint32_t DSL_CEOC_EventCB
(
   DSL_Context_t *pContext
);
#endif

/** @} DRV_DSL_DEVICE */

#ifdef __cplusplus
}
#endif

#endif /* _DRV_DSL_CPE_DANUBE_H */
