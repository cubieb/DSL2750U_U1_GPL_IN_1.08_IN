/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DSL_CPE_CONTROL_H
#define _DSL_CPE_CONTROL_H

/*
Includes
*/
#include <errno.h>
#include <string.h>
#include <sys/types.h>

#include "dsl_cpe_os.h"

/** ND: This is for testing only --> */
#define INCLUDE_DSL_MIB_ADSL_TEST 1
#define INCLUDE_DSL_ADSL_MIB
/** ND: <-- This is for testing only */

extern DSL_char_t *g_sFirmwareName1;

#ifdef LINUX

#if defined (INCLUDE_DSL_CPE_API_DANUBE) || defined (INCLUDE_DSL_CPE_API_AMAZON_SE)
#if (INCLUDE_DSL_MIB_ADSL_TEST == 0)
   #include "asm/danube/danube_mei_app.h"
   #include "asm/danube/danube_mei_app_ioctl.h"
#endif
#endif

#if defined (INCLUDE_DSL_CPE_API_DANUBE) || defined (INCLUDE_DSL_CPE_API_AMAZON_SE) || \
    defined (INCLUDE_DSL_CPE_API_VINAX)
   /* Include file system functionality for Danube LINUX by default */
   #define INCLUDE_DSL_CPE_FILESYSTEM_SUPPORT
#endif /* #if defined (INCLUDE_DSL_CPE_API_DANUBE) || defined (INCLUDE_DSL_CPE_API_AMAZON_SE) */

#include "errno.h"

#endif /* LINUX */

#if defined(DSL_DEBUG_TOOL_INTERFACE) && (!defined(INCLUDE_DSL_CPE_FILESYSTEM_SUPPORT) || !defined(LINUX))
#error "DSL Debug Interface can be used with Linux and file \
system support enabled only"
#endif

#ifdef DSL_DEBUG_TOOL_INTERFACE
#define DSL_DEBUG_TOOL_INTERFACE_DEFAULT_IFACE "eth0"
#endif

#if defined (INCLUDE_DSL_CPE_API_DANUBE) || defined (INCLUDE_DSL_CPE_API_AMAZON_SE)
   #define DSL_CPE_IFX_LOW_DEV "/dev/ifx_mei"
#else
   #if defined (INCLUDE_DSL_CPE_API_VINAX)
      #define DSL_CPE_IFX_LOW_DEV "/dev/vinax"
      #ifndef DSL_DEV_VINAX_CPE_DEFAULT_LINE
         #define DSL_DEV_VINAX_CPE_DEFAULT_LINE   0
      #endif
   #endif
#endif

#include "drv_dsl_cpe_api.h"
#include "drv_dsl_cpe_api_types.h"
#include "drv_dsl_cpe_api_error.h"

#ifdef WIN32
   #include "dsl_cpe_control_version.h"
#endif /* WIN32 */

#ifdef INCLUDE_DSL_CPE_CLI_SUPPORT
   #include "dsl_cpe_cli.h"
#endif /* INCLUDE_DSL_CPE_CLI_SUPPORT */

#define DSL_DEV_TIMEOUT_SELECT   1000

#define DSL_CPE_FW_CHUNK_SIZE    (64 * 1024)

/*#define INCLUDE_DSL_API_CLI_LEGACY*/

/*
   Security checks for configure options
*/
#ifdef INCLUDE_DSL_CPE_SOAP_SUPPORT
   #ifndef INCLUDE_DSL_CPE_CLI_SUPPORT
      #error "SOAP support requires CLI please define 'INCLUDE_DSL_CPE_CLI_SUPPORT'"
   #endif
#endif /* INCLUDE_DSL_CPE_SOAP_SUPPORT */


#ifdef DSL_DEBUG_TOOL_INTERFACE
   #ifndef INCLUDE_DSL_CPE_CLI_SUPPORT
      #error "Debug Tool Interface requires CLI please define 'INCLUDE_DSL_CPE_CLI_SUPPORT'"
   #endif
#endif /* INCLUDE_DSL_CPE_SOAP_SUPPORT */


#ifdef INCLUDE_DSL_CPE_SOAP_SUPPORT
#define DSL_CPE_SOAP_PORT           8080
   /* Enable firmware update functionality in case of SOAP support by default */
   #define DSL_CPE_SOAP_FW_UPDATE

#endif /* INCLUDE_DSL_CPE_SOAP_SUPPORT */

extern DSL_boolean_t g_bWaitBeforeLinkActivation;
extern DSL_boolean_t g_bWaitBeforeConfigWrite;

#ifdef INCLUDE_DSL_CPE_FILESYSTEM_SUPPORT
#ifdef INCLUDE_DSL_CPE_CMV_SCRIPTS_SUPPORT
extern DSL_char_t *g_sAdslScript;
extern DSL_char_t *g_sVdslScript;
extern DSL_boolean_t g_bAutoContinue;
#endif
#endif


#define DSL_CPE_DEVICE_NAME         "/dev/dsl_cpe_api"
#define DSL_CPE_PREFIX              "DSL_CPE: "

#ifdef INCLUDE_DSL_CPE_FILESYSTEM_SUPPORT
   #define DSL_CPE_SHOWTIME_EVENT_LOGGING_FILENAME "/ramdisk/post_mortem_data.txt"
#endif


#ifndef _MKSTR_1
#define _MKSTR_1(x)    #x
#define _MKSTR(x)      _MKSTR_1(x)
#endif


typedef unsigned int DSL_size_t;

typedef void (DSL_CliFunction_t) (DSL_int32_t fd,
                                  DSL_char_t *pCommands,
                                  DSL_File_t *out);

typedef struct
{
   char *sShortCommand;
   char *sLongCommand;
   DSL_CliFunction_t *pFunction;
} DSL_CliCommand_t;


DSL_int32_t DSL_sscanf (DSL_char_t * buf, DSL_char_t const *fmt, ...);

#ifdef INCLUDE_DSL_API_CLI_LEGACY

#define GET_ADSL_LINE_STATUS        0
#define DANUBE_MEI_CMV_WINHOST      0
#define LINE_RATE_DATA_RATEDS_FLAG  1
#define LINE_RATE_DATA_RATEUS_FLAG  2

typedef struct
{
   DSL_uint32_t dummy;
} adslLineStatusInfo;

typedef struct
{
   DSL_uint32_t flags;
   DSL_uint32_t adslDataRateus;
   DSL_uint32_t adslDataRateds;
} adslLineRateInfo;

#endif /* INCLUDE_DSL_API_CLI_LEGACY */


typedef struct
{
   /**
   File name is only used in case of using firmware binary that is located
   within file system. If firmware that is used which is already stored
   within context of DSL CPE Control Application (for example in case of
   using SOAP firmware download function) this pointer shall be DSL_NULL. */
   DSL_char_t *pFileName;
   /**
   Pointer to firmware binary in case of using data stored within context of
   DSL CPE Control Application. In case of using reference to file instead
   (pFileName does not equal DSL_NULL) this pointer shall be DSL_NULL. */
   DSL_uint8_t *pData;
   /**
   Size of firmware binary in case of using data stored within context of
   DSL CPE Control Application. In case of using reference to file instead
   (pFileName does not equal DSL_NULL) this parameter shall be zero. */
   DSL_uint32_t nSize;
} DSL_CPE_Firmware_t;

typedef struct
{
   DSL_int_t fd;
   DSL_boolean_t bRun;
   DSL_int_t nTIdEvent;
   DSL_boolean_t bEvtRun;
#if defined (DSL_DEBUG_TOOL_INTERFACE)
   DSL_int_t nTcpMsgHandler;
   DSL_int_t nTcpCliHandler;
#endif
#ifdef DSL_CPE_SOAP_FW_UPDATE
   /** Semaphore used in case of updating the firmware binar(y/ies) via SOAP */
   DSL_Semaphore_t semFwUpdate;
   /**
   Specifies an firmware that might be updated using SOAP */
   DSL_CPE_Firmware_t firmware;
   /**
   Specifies an firmware that might be updated using SOAP */
   DSL_CPE_Firmware_t firmware2;
#endif /* DSL_CPE_SOAP_FW_UPDATE */

#ifdef INCLUDE_DSL_CPE_CLI_SUPPORT
   DSL_CLI_Context_t *pCLI_List_head;
#endif /* INCLUDE_DSL_CPE_CLI_SUPPORT */

#ifdef INCLUDE_DSL_CPE_FILESYSTEM_SUPPORT
#ifdef INCLUDE_DSL_CPE_CMV_SCRIPTS_SUPPORT
   DSL_FirmwareRequestType_t nFwReqType;
#endif
#endif

} DSL_CPE_Control_Context_t;

#include "dsl_cpe_debug.h"

/**
  A structure for event type<->string conversion tables.
*/
typedef struct
{
   /** event type */
   DSL_EventType_t eventType;
   /** event string */
   DSL_char_t const *string;
} DSL_CPE_EVT_CodeString_t;

DSL_CPE_Control_Context_t *DSL_GetGlobalContext(DSL_void_t);

DSL_Error_t DSL_CPE_LoadFirmwareFromFile
(
   DSL_char_t *psFirmwareName,
   DSL_uint8_t **pFirmware,
   DSL_uint32_t *pnFirmwareSize
);

DSL_Error_t DSL_CPE_DownloadFirmware
(
   DSL_char_t *pcFw,
   DSL_char_t *pcFw2
);

#ifdef INCLUDE_DSL_CPE_CLI_SUPPORT
   typedef enum
   {
      /* Format as hey values */
      DSL_ARRAY_FORMAT_HEX,
      /* Format as string */
      DSL_ARRAY_FORMAT_STRING
   } DSL_CPE_ArrayPrintFormat_t;

   DSL_void_t DSL_CPE_ArraySPrintF
   (
      DSL_char_t *pDst,
      DSL_void_t *pSrc,
      DSL_uint16_t nSrcSize,
      DSL_uint16_t nSrcElementSize,
      DSL_CPE_ArrayPrintFormat_t nFormat
   );

   DSL_Error_t DSL_CPE_MoveCharPtr
   (
      DSL_char_t *pCommands,
      DSL_int_t nParamNr,
      DSL_char_t *pSeps,
      DSL_char_t **pCmdOffset
   );

#ifdef INCLUDE_DSL_CPE_API_VINAX
   typedef struct
   {
      DSL_uint8_t nAdr[DSL_MAC_ADDRESS_OCTETS];
   } DSL_CPE_MacAddress_t;

   DSL_Error_t DSL_CPE_GetMacAdrFromString
   (
      DSL_char_t *pString,
      DSL_CPE_MacAddress_t *pMacAdr
   );
#endif /* INCLUDE_DSL_CPE_API_VINAX */
#endif /* INCLUDE_DSL_CPE_CLI_SUPPORT */

/*
   some declarations for modules without own header
*/
#ifdef INCLUDE_DSL_CPE_SOAP_SUPPORT
   DSL_int32_t DSL_CPE_SoapInit(DSL_int_t fd);
   DSL_int32_t DSL_CPE_SoapExit(DSL_void_t);

   #ifdef DSL_CPE_SOAP_FW_UPDATE
      DSL_Error_t DSL_CPE_SoapFirmwareUpdate
      (
         DSL_CPE_Firmware_t *pFirmware,
         DSL_CPE_Firmware_t *pFirmware2
      );

      DSL_Error_t DSL_CPE_SoapFirmwareStore
      (
         DSL_CPE_Control_Context_t *pContext,
         DSL_CPE_Firmware_t *pSrcFirmware,
         DSL_CPE_Firmware_t *pDestFirmware
      );
   #endif /* DSL_CPE_SOAP_FW_UPDATE */
#endif /* INCLUDE_DSL_CPE_SOAP_SUPPORT */

/**
   Start Event handler thread
*/
DSL_Error_t DSL_CPE_EventHandlerStart (
   DSL_CPE_Control_Context_t * pContext
);

#ifdef INCLUDE_DSL_CPE_CMV_SCRIPTS_SUPPORT
DSL_void_t DSL_Echo
(
   DSL_char_t *buf
);

DSL_Error_t DSL_CPE_ScriptExecute
(
   DSL_CPE_Control_Context_t * pContext,
   DSL_File_t *pFile
);

DSL_Error_t DSL_CPE_ScriptFileParse
(
   DSL_CPE_Control_Context_t * pContext,
   DSL_char_t *sFileName
);
#endif /* INCLUDE_DSL_CPE_CMV_SCRIPTS_SUPPORT */


#endif /* _DSL_CPE_CONTROL_H */

