/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_DEVICE_VINAX_H
#define _DRV_DSL_CPE_DEVICE_VINAX_H

#ifdef __cplusplus
   extern "C" {
#endif

/** \addtogroup DRV_DSL_DEVICE
 @{ */

#include "drv_dsl_cpe_api.h"

#include "drv_dsl_cpe_vinax_msg_const.h"
#include "drv_dsl_cpe_vinax_msg.h"
#include "drv_dsl_cpe_vinax_msg_xtc.h"
#include "drv_dsl_cpe_vinax_msg_tc_counters.h"
#include "drv_dsl_cpe_vinax_msg_utopia_posphy.h"
#include "drv_dsl_cpe_vinax_mdio.h"
#include "drv_dsl_cpe_vinax_msg_dmt_scope.h"

/** VINAX driver interface header file*/
#include "drv_vinax_api_intern.h"

#define DSL_MAX_LINE_NUMBER 0
#define DSL_CHIPSET_TYPE_STRING "Ifx-Vinax"

/** Timeout setting for far end status in seconds */
#define DSL_DEV_TIMEOUT_FE_STATUS               12

/** Defines the Minimum possible Data Rate in bit/s */
#define DSL_DEV_MIN_DATARATE              16000
/** Defines the Maximum possible Data Rate for ADSL2+ in bit/s (DS/US) */
#define DSL_DEV_MAX_DATARATE_DS_ADSL2P    32736000
#define DSL_DEV_MAX_DATARATE_US_ADSL2P    2048000
/** Defines the Maximum possible Data Rates for ADSL and ADSL2 in bit/s (DS/US) */
#define DSL_DEV_MAX_DATARATE_DS_ADSL2     16352000
#define DSL_DEV_MAX_DATARATE_US_ADSL2     2048000

/**
   This bit of the Message ID identifies 32 bit aligned TC messages */
#define VDSL2_MBX_MSG_ID_IFX_MASK 0x0010

typedef VINAX_DYN_CNTRL_T DSL_DEV_Device_t;

/**
   Structure used to build table which includes a list of 32bit message IDs
*/
typedef struct
{
   /** 16 Bit message ID */
   DSL_uint16_t nMsgId;
} VNX_MsgId_t;

/** Structure to keep the decoded Firmware Verion information
   contained in the 32bit Version number */
typedef struct
{
   /** Vinax Revision 1, 2, ... */
   DSL_uint8_t nPlatform;
   /** */
   DSL_uint8_t nFeatureSet;
   /** */
   DSL_uint8_t nMajor;
   /** */
   DSL_uint8_t nMinor;
   /** Pre, Verification, Development */
   DSL_uint8_t nReleaseStatus;
   /** VDSL1/2, ADSL, ... */
   DSL_uint8_t nApplication;
} DSL_VNX_FwVersion_t;

/**
 * A structure for code<->string conversion tables.
 * used mostly during FW/HW version retrieval.
 */
typedef struct
{
   /** version code */
   DSL_uint32_t code;
   /** version string */
   DSL_char_t *string;
} VNX_CodeString_t;


/**
   Vinax FW Features for use inside the DSL CPE API
*/
typedef enum
{
   /**
      FW supports VDSL1 */
   DSL_VNX_FW_VDSL1,
   /**
      FW supports VDSL2 */
   DSL_VNX_FW_VDSL2,
   /**
      FW supports ADSL Annex A */
   DSL_VNX_FW_ADSLA,
   /**
      FW supports ADSL Annex B */
   DSL_VNX_FW_ADSLB
} DSL_VNX_Feature_t;


/** Internal identifier for swapping a combined FW between
   ADSL and VDSL */
typedef enum
{
   /** VDSL part of FW */
   FWMODE_VDSL,
   /** ADSL part of FW */
   FWMODE_ADSL
} DSL_VNX_FwMode_t;


/**
 * VINAX application version codes
 */
typedef enum
{
   DSL_APP_TEST               = 0x00, /* "Test          */
   DSL_APP_VDSL1              = 0x01,  /* "VDSL1 only    */
   DSL_APP_VDSL1_2            = 0x02,  /* "VDSL1/2       */
   DSL_APP_VDSL1_2_ADSLA      = 0x03,  /* "VDSL1/2 + ADSL Annex A (CO + CPE with-host)*/
   DSL_APP_VDSL1_2_ADSLB      = 0x04,  /* "VDSL1/2 + ADSL Annex B (CO + CPE with-host)*/
   DSL_APP_ADSLA              = 0x05,  /* "ADSL Annex A (CO + CPE with-host)*/
   DSL_APP_ADSLB              = 0x06,  /* "ADSL Annex B (CO + CPE with-host)*/
   DSL_APP_VDSL1_HL           = 0x07,  /* "VDSL1 (CPE hostless)*/
   DSL_APP_VDSL1_2_HL         = 0x08,  /* "VDSL1/2 (CPE hostless)*/
   DSL_APP_VDSL1_2_ADSLA_HL   = 0x09,  /* "VDSL1/2 + ADSL Annex A (CPE hostless)*/
   DSL_APP_VDSL1_2_ADSLB_HL   = 0x0A,  /* "VDSL1/2 + ADSL Annex B (CPE hostless)*/
   DSL_APP_ADSLA_HL           = 0x0B,  /* "ADSL Annex A (CPE hostless)*/
   DSL_APP_ADSLB_HL           = 0x0C,  /* "ADSL Annex B (CPE hostless)*/
   DSL_APP_DMTS               = 0x0D,  /* "DMT scope*/
   DSL_APP_UNKNOWN,
   DSL_APP_LAST = DSL_APP_UNKNOWN
} DSL_VNX_ApplicationCode_t;


/** VDSL2 profile 8a */
#define DSL_VNX_PROFILES_8A     0x01
/** VDSL2 profile 8b */
#define DSL_VNX_PROFILES_8B     0x02
/** VDSL2 profile 8c */
#define DSL_VNX_PROFILES_8C     0x04
/** VDSL2 profile 8d */
#define DSL_VNX_PROFILES_8D     0x08
/** VDSL2 profile 12a */
#define DSL_VNX_PROFILES_12A    0x10
/** VDSL2 profile 12b */
#define DSL_VNX_PROFILES_12B    0x20
/** VDSL2 profile 17a */
#define DSL_VNX_PROFILES_17A    0x40
/** VDSL2 profile 30a */
#define DSL_VNX_PROFILES_30A    0x80

/**
   Firmware application and feature information extracted from
   the FW version. */
typedef struct
{
   /**
   Applications supported by FW  */
   DSL_VNX_ApplicationCode_t nApplicationType;
} DSL_FwSupport_t;

/**
   Structure to handle DELT Aux test parameters
*/
typedef struct
{
   /** HLIN Scaling Factor*/
   DSL_uint16_t HLINSC;
   /* HLIN Subcarrier Group Size */
   DSL_uint16_t HLING;
   /* HLOG Measurement Time */
   DSL_uint16_t HLOGMT;
   /* HLOG Subcarrier Group Size */
   DSL_uint16_t HLOGG;
   /* QLN Measurment Time */
   DSL_uint16_t QLNMT;
   /* QLN Subcarrier Group Size */
   DSL_uint16_t QLNG;
   /* SNR Measurement Time */
   DSL_uint16_t SNRMT;
   /* SNR Subcarrier Group Size */
   DSL_uint16_t SNRG;
} DSL_VNX_DeltTestParamsAux_t;


/**
   Number activation retries for the
   CPE Auto Mode (CAM) state machine. */
#define DSL_CAM_MAX_REINITS      5

/**
   States for the CPE Auto Mode (CAM) state machine
   to toggle between ADSL and VDSL activations */
typedef enum
{
   /** CPE Automode feature is not used */
   DSL_CAM_DISABLED = 0,
   /** CPE Automode feature initialization state */
   DSL_CAM_INIT = 1,
   /** CPE Automode feature restricted to VDSL */
   DSL_CAM_VDSL_FORCED = 2,
   /** CPE Automode feature multi mode */
   DSL_CAM_MULTI = 3,
   /** CPE Automode feature VDSL only mode with retries */
   DSL_CAM_VDSL_RETRY = 4,
   /** CPE Automode feature ADSL only mode with retries */
   DSL_CAM_ADSL_RETRY = 5,
   /** CPE Automode delimiter */
   DSL_CAM_LAST = DSL_CAM_ADSL_RETRY
} DSL_DEV_CamStates_t;

/**
   Structure that stores global variables for function calls that are waiting
   in a blocking call for direct response from the device.
*/
typedef struct
{
   /**
   Semaphore used to wait for availability of message waiting procedure. */
   DSL_mutex_t MsgTransferMutex;
   /**
   Flag which is used to indicate that a function call waits for data */
   DSL_boolean_t bFctWaiting;
   /**
   Flag which is used to indicate that the data for a waiting function has been
   received */
   DSL_boolean_t bDataReceived;
   /**
   Specifies for which message type the function is waiting */
   DSL_uint8_t nMsgClassifier;
   /**
   Specifies for which message ID the function is waiting */
   DSL_uint16_t nMsgId;
   /**
   Memory to store message data that is requested from the waiting function */
   DSL_uint8_t nData[512];
   /**
   Size of stored message data */
   DSL_uint16_t nLen;
} DSL_FctWaiting_t;


/**
   VINAX specific device data*/
typedef struct {
   /**
   Defines device specific configuration information.*/
   DSL_DeviceConfig_t deviceCfg;
   /**
   Downloaded FW support features*/
   DSL_VNX_FwVersion_t fwFeatures;
   DSL_DEV_CamStates_t nCamState;
   DSL_uint8_t nCamReinits;
   DSL_uint8_t nSpar1;
   DSL_uint8_t nSpar2;
   DSL_uint8_t nSpar3;
   DSL_uint8_t nSpar4;
   DSL_uint8_t nSpar5;
   DSL_boolean_t bCamFwReload;
   /**
   Structure that stores information about a waiting function call and its 
   received data */
   DSL_FctWaiting_t fctWait;
} DSL_DEV_Data_t;

typedef enum
{
   /** No transmission*/
   DSL_CEOC_TX_IDLE = 0,
   /** Transmission in progress*/
   DSL_CEOC_TX_PROG = 1,
   /** Transmission error*/
   DSL_CEOC_TX_ERR  = 3,
} DSL_VNX_ClearEocTxStatus_t;

typedef enum
{
   /** No transmission*/
   DSL_CEOC_RX_IDLE = 0,
   /** Reception in progress*/
   DSL_CEOC_RX_PROG = 1,
   /** Reception done. Data is available in RX buffer.*/
   DSL_CEOC_RX_DONE = 2,
   /** Reception error*/
   DSL_CEOC_RX_ERR  = 3,
} DSL_VNX_ClearEocRxStatus_t;

typedef struct
{
   /** CEOC Tx Status*/
   DSL_VNX_ClearEocTxStatus_t tx;
   /** CEOC Rx Status*/
   DSL_VNX_ClearEocRxStatus_t rx;
} DSL_VNX_ClearEocStatus_t;

/** Maximal number of bands handled for one direction (US or DS) */
#define DSL_MAX_NUMBER_OF_BANDS    9

/**
   This structure shall carry the complete PSD shape for all tones
*/
typedef struct
{
   /**
   Number of first tone index of the band */
   DSL_uint16_t nFirstToneIndex;
   /**
   Number of last tone index of the band */
   DSL_uint16_t nLastToneIndex;
} DSL_Band_t;

/**
   This structure defines a set of carrier bands
*/
typedef struct
{
   /**
   Number of used data elements */
   DSL_uint16_t nNumData;
   /**
   band definitions */
   DSL_Band_t nBand[DSL_MAX_NUMBER_OF_BANDS];
}DSL_BandList_t;

/** Value out of Q8.8 range. Means "no value". */
#define Q88_NOVALUE 0x8000

/** Define the VINAX device default line for CPE*/
#ifndef DSL_DEV_VINAX_CPE_DEFAULT_LINE
#define DSL_DEV_VINAX_CPE_DEFAULT_LINE   0x0
#endif

/**
   to supporte the first ADSL firmware some special treatments are necessary */
#define PRELIMINARY_ADSL_SUPPORT       1
#define ADSL_ALPHEUS_SUPPORT           0

/**
   VINAX Mailbox error codes */
#define D2H_ERROR_BKGD_TASK_WAITING    0xE1 /* 1110 0001b */
#define D2H_ERROR_GENERAL_FAILURE      0xE2 /* 1110 0010b */
#define D2H_ERROR_CMD_NOT_ALLOWED      0xE3 /* 1110 0011b */
#define D2H_ERROR_PARAMETER_WRONG      0xE4 /* 1110 0100b */
#define D2H_ERROR_RESOURCE_OCCUPIED    0xE5 /* 1110 0101b */
#define D2H_DBG_VISP_RUNNING           0xE6 /* 1110 0110b */
#define D2H_DBG_VISP_STOPPED           0xE7 /* 1110 0111b */
#define D2H_CMV_CURRENTLY_UNAVAILABLE  0x91


/**
   Calculate and return the lenght field of 16Bit Get/Set messages:
   16 bit length field = (sizeof(msg) - sizeof(index + length)) / 2
   This is valid for all messages of the PMD layer (16bit parameters)
   \note The "get" messages require the length of the ACK-msg in the length field */
#define DSL_VNX_16BIT_RD_MSG_LEN_GET(msgStruct)   ((sizeof(msgStruct) - 4) / 2)

/**
   Calculate and return the lenght field of 32Bit Get messages:
   32 bit length field = (sizeof(msg) - sizeof(index + length)) / 4
   This is valid for xTC "set" messages (32bit parameters) and "get" messages
   which contain not the channel parameter "LineNo" additionally */
#define DSL_VNX_32BIT_RD_MSG_LEN_GET(msgStruct)   ((sizeof(msgStruct) - 4) / 4)

/**
   Calculate and return the lenght field of 32Bit Channel Get messages:
   32 bit length field = (sizeof(msg) - sizeof(index + length + LineNo)) / 4
   This is valid for xTC "set" messages (32bit parameters) and "get" messages
   which contain the channel parameter "LineNo" additionally */
#define DSL_VNX_32BIT_CH_RD_MSG_LEN_GET(msgStruct) ((sizeof(msgStruct) - 8) / 4)

/**
   Calculate and return the lenght field of 32Bit Channel Set messages:
   This is the same as for the Get messages without channel */
#define DSL_VNX_32BIT_CH_WR_MSG_LEN_GET DSL_VNX_32BIT_RD_MSG_LEN_GET

/**
   Calculate and return the lenght field of 32Bit Set messages:
   This is the same as for the Get messages */
#define DSL_VNX_32BIT_WR_MSG_LEN_GET DSL_VNX_32BIT_RD_MSG_LEN_GET

/**
   Returns the smaller of the two parameters.
   Take care to use compatible types!

   \param a first element to compare, [I]
   \param b second element to compare, [I]
*/
#define DSL_MIN(a,b) ((a)<(b)?(a):(b))


/** Processes a given bit value of a firmware and maps it into the bit field
    value (and bit field mask) if the state of the bit has been changed since
    last occurence of the macro call.
    \param MSGBIT        Name of bit in "RSP_Line_Failures" message, [I]
    \param nPreBitField  Previous value of line failure bit mask from data
                         base, [I]
    \param nBitMask      Definition of line failure bit as defined in enum type
                         of DSL library, [I]
    \param nActBitField  Actual value of line failure bit mask from GMX-D
                         (mapped from MSGBIT).
                         This value will be generated by the macro, [O]
    \param nBitFieldMask Returns a bit mask that indicates which failure bits
                         has been toggled from previous to actual line failure
                         status. This value will be generated by the macro, [O]
*/
#define DSL_FAILUREBIT_SET(MSGBIT, nPreBitField, nBitMask, nActBitField, nBitFieldMask) \
   if (sAck.MSGBIT) \
   { \
      nActBitField |= nBitMask; \
   } \
   if ( (nActBitField & nBitMask) != (nPreBitField & nBitMask) ) \
   { \
      nBitFieldMask |= nBitMask; \
   }


/**
   This function triggers to send a specified message which requests data from
   the local device and waits for appropriate answer.

   \param pContext Pointer to dsl cpe drive context structure, [I]
   \param nMsgID   Specifies the message ID as defined in the VINAX firmware
                   message specification. It includes the message type and
                   subtype, [I]
   \param nLength  number of bytes of the message payload, [I]
   \param nData    pointer to the message payload data, [I]
   \param nLenAck  available buffer size for received ack, [I]
   \param pDataAck pointer to buffer for receiving ack message, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed or message ID of acknowledge wrong
*/
DSL_Error_t DSL_VNX_SendMessage(
   DSL_Context_t *pContext,
   const DSL_uint32_t nMsgID,
   const DSL_uint16_t nLength,
   const DSL_uint8_t *pData,
   const DSL_uint16_t nLenAck,
   DSL_uint8_t *pDataAck);

/**
   Retrieve the current bit allocation table and/or
   the number of bits per symbol
   by evaluation of the bit allocation table.
   This function causes relevant message traffic.

   \param pContext - Pointer to DSL library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      if this pointer is not NULL, it returns the current
                     bit allocation table, [O]
   \param pBitsPerSymbol
                     if this pointer is not NULL, it returns the current
                     bits per symbol, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_VNX_BitAllocationTableGet (
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_G997_NSCData8_t *pData,
   DSL_uint32_t *pBitsPerSymbol);

/**
   Retrieve the current gain allocation table
   by evaluation of the bit allocation table.
   This function causes relevant message traffic.

   \param pContext - Pointer to DSL library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      if this pointer is not NULL, it returns the current
                     gain allocation table, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_VNX_GainAllocationTableGet(
   DSL_IN DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_OUT DSL_G997_NSCData16_t *pData);

/**
   Retrieve the current SNR allocation table
   by evaluation of the bit allocation table.
   This function causes relevant message traffic.

   \param pContext - Pointer to DSL library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      if this pointer is not NULL, it returns the current
                     SNR allocation table, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_VNX_SnrAllocationTableGet(
   DSL_IN DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_OUT DSL_G997_NSCData8_t *pData);

/**
   This function sends the VINAX firmware message
   "CMD_LineStatusPerBandUS/DS_Get" and waits for the answer to get the per
   band values for LATN, SATN and SNR.
   This information is necessary for the G.997.1 API function
   DSL_DEV_G997_LineStatusPerBandGet.
   Instead of the direct request here the values might be retrieved
   when reaching showtime and read from the database in this function.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      Returns the paramter pData->SNR, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_VNX_LineStatusPerBandGet(
   DSL_Context_t *pContext,
   DSL_AccessDir_t nDirection,
   DSL_G997_LineStatusPerBandData_t *pData);

/**
   This function sends the VINAX firmware message "CMD_LineStatusUS/DS_Get"
   and waits for the answer to get the values for SNR.
   This information is necessary for the G.997.1 API function
   DSL_DEV_G997_LineStatusGet.
   Instead of the direct request here the values might be retrieved
   when reaching showtime and read from the database in this function.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]
   \param pData      Returns the paramter pData->SNR, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_VNX_LineStatusGet(
   DSL_Context_t *pContext,
   DSL_AccessDir_t nDirection,
   DSL_G997_LineStatusData_t * pData);

/**
   This function sends the VINAX firmware message CMD_LED_Control.

   \param pContext - Pointer to DSL library context structure, [I]
   \param nLine    - Line number for which the data should be written, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_VNX_SendMsgLedControl(
   DSL_Context_t *pContext);

DSL_boolean_t DSL_VNX_FwFeatureCheck(
   DSL_Context_t           *pContext,
   const DSL_VNX_Feature_t nFeature);

DSL_Error_t DSL_VNX_SendMsgTr1Control(
   DSL_Context_t *pContext,
   DSL_boolean_t bTR1Enable,
   DSL_boolean_t bTR1EventEnable,
   DSL_uint16_t nTR1Period,
   DSL_uint16_t nTR1Control,
   DSL_uint16_t nTR1Adjust );


/**
   This function gets the Measurement time and group sizes for
   DELT related measurements.

   \param pContext   Pointer to dsl library context structure, [I]
   \param nDirection Specifies the direction (upstream/downstream) to which
                     the function will apply. [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_VNX_SendMsgTestParamsAuxUpdate(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection);

/**
   This function gets performance counters values for AAL5 segmentation statistics.

   \param pContext   Pointer to dsl library context structure, [I]
   \param pData      Pointer to the Segmentation counters data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_SAR_VNX_SegmentationCountersGet(
   DSL_Context_t *pContext,
   DSL_SAR_SegmentationCountersData_t *pData);

/**
   This function gets performance counters values for AAL5 reassembly statistics.

   \param pContext   Pointer to dsl library context structure, [I]
   \param pData      Pointer to the Reassembly counters data, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
   - or any other defined specific error code
*/
DSL_Error_t DSL_SAR_VNX_ReassemblyCountersGet(
   DSL_Context_t *pContext,
   DSL_SAR_ReassemblyCountersData_t *pData);

/**
   This function returns all VDSL2 bandplan/profile combinations
   supported by the device.

   \param pContext Pointer to dsl cpe library context structure, [I]
   \param pData    Returns the support matrix, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_VNX_BandPlanSupportedGet(
   DSL_Context_t *pContext,
   DSL_BandPlanSupportData_t *pData);

/**
   This function returns VDSL2 actual bandplan/profile information.

   \param pContext Pointer to dsl cpe library context structure, [I]
   \param pData    Returns the actual bandplan/profile information, [O]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_VNX_BandPlanStatusGet(
   DSL_Context_t *pContext,
   DSL_BandPlanConfigData_t *pData);


/**
   This function sends the firmware message "CMD_FrameDataExt2XX_Get".

   \param pContext Pointer to dsl library context structure, [I]
   \param nDirection Upstream or Downstream, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_VNX_SendMsgFrameDataExt2Get(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck);

/**
   This function sends the firmware message "CMD_FrameDataExt1XX_Get".

   \param pContext Pointer to dsl library context structure, [I]
   \param nDirection Upstream or Downstream, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS (0) in case of success
   - DSL_ERROR (-1) if operation failed
*/
DSL_Error_t DSL_VNX_SendMsgFrameDataExt1Get(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint8_t *pAck);

/**
   This function handles the VINAX firmware message
   for the ADSL framing parameters

   \param pContext Pointer to dsl library context structure, [I]
   \param nDir     Direction to retrieve data for, US/DS, [I]

   \return
   Return values are defined within the DSL_Error_t definition
   - DSL_SUCCESS in case of success
   - DSL_ERROR if operation failed
*/
DSL_Error_t DSL_VNX_SendMsgAdslFrameDataGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDir,
   DSL_uint8_t *pAck);

DSL_Error_t DSL_VNX_PowerManagementStatusSet(
   DSL_Context_t *pContext,
   const DSL_G997_PowerManagement_t nPMMode);

DSL_Error_t DSL_VNX_AdslDeltAllDataGet(
   DSL_Context_t *pContext);

DSL_Error_t DSL_VNX_DeltQlnGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint16_t nDataNum,
   DSL_G997_NSCData8_t * pData);

DSL_Error_t DSL_VNX_DeltSnrGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint16_t nDataNum,
   DSL_G997_NSCData8_t *pData);

DSL_Error_t DSL_VNX_DeltHlinGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint16_t nDataNum,
   DSL_G997_NSCComplexData_t *pData);

DSL_Error_t DSL_VNX_DeltHlogGet(
   DSL_Context_t *pContext,
   const DSL_AccessDir_t nDirection,
   DSL_uint16_t nDataNum,
   DSL_G997_NSCData16_t *pData);

DSL_Error_t DSL_VNX_ChannelStatusGet(
   DSL_Context_t *pContext,
   DSL_uint8_t nChannel,
   DSL_AccessDir_t nDirection,
   DSL_G997_ChannelStatusData_t *pData);

DSL_uint32_t DSL_VNX_G997_FramingParameterStatusAdslGet(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN const DSL_uint8_t nChannel,
   DSL_OUT DSL_G997_FramingParameterStatusData_t *pData);

DSL_uint32_t DSL_VNX_G997_FramingParameterStatusVdsl2Get(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN const DSL_uint8_t nChannel,
   DSL_OUT DSL_G997_FramingParameterStatusData_t *pData);

DSL_uint32_t DSL_VNX_G997_FramingParameterStatusVdsl1Get(
   DSL_IN DSL_Context_t *pContext,
   DSL_IN const DSL_AccessDir_t nDirection,
   DSL_IN const DSL_uint8_t nChannel,
   DSL_OUT DSL_G997_FramingParameterStatusData_t *pData);

/** @} DRV_DSL_DEVICE */

#ifdef __cplusplus
}
#endif

#endif
