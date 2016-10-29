/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef DSL_CPE_CLI_H
#define DSL_CPE_CLI_H

/** \file
   DSL CPE API command line interface
*/
#include "drv_dsl_cpe_api_error.h"

/** maximum arguments, used for spawning a shell with external commands */
#define DSL_MAX_ARGS                   16

#ifdef DSL_CPE_DEBUG_DISABLE
#define USAGE "" DSL_CRLF
#endif /* DSL_CPE_DEBUG_DISABLE */

#if defined(INCLUDE_DSL_CPE_CLI_SUPPORT) | 1

/** Context for a registered CLI */
typedef struct DSL_CLI_Context DSL_CLI_Context_t;
/** buffer for preparing event text decoding */
extern DSL_char_t CLI_EventText[16000];

/** Exit callback with private pointer.
   (pointer can be defined with \ref DSL_CLI_Register) */
typedef DSL_Error_t (*DSL_CPE_Exit_Callback_t) ( DSL_void_t* );
/** Event callback with private pointer (from \ref DSL_CLI_Register)
    and text of decoded event */
typedef DSL_Error_t (*DSL_CLI_Event_Callback_t) ( DSL_void_t*, DSL_char_t* );

DSL_Error_t DSL_CLI_CommandAdd
(
   DSL_char_t *name,
   DSL_char_t *long_form,
   DSL_int_t (*func)(DSL_int_t, DSL_char_t*, DSL_File_t*),
   const DSL_char_t *psHelp
);

DSL_Error_t DSL_CLI_CommandClear(DSL_void_t);
DSL_void_t DSL_CLI_AccessCommandsRegister(DSL_void_t);

DSL_int_t DSL_CLI_CommandExecute
(
   DSL_int_t fd,
   DSL_char_t *cmd,
   DSL_char_t *arg,
   DSL_File_t *out
);

DSL_int_t DSL_CLI_HelpPrint
(
   DSL_int_t fd,
   DSL_char_t *,
   DSL_File_t*
);

typedef enum
{
   DSL_CLI_EQUALS = 0,
   DSL_CLI_MIN,
   DSL_CLI_MAX
} DSL_CLI_ParamCheckType_t;

DSL_boolean_t DSL_CLI_CheckParamNumber
(
   DSL_char_t *pCommands,
   DSL_int_t nParams,
   DSL_CLI_ParamCheckType_t nCheckType
);

DSL_Error_t DSL_CLI_Init(DSL_int_t fd);
DSL_Error_t DSL_CLI_Shutdown(DSL_int_t fd);
DSL_Error_t DSL_CLI_HandleEvent(DSL_char_t *pMsg);

DSL_Error_t DSL_CLI_Register
(
   DSL_CLI_Context_t **pNewCLIContext,
   DSL_void_t *pCBContext,
   DSL_CPE_Exit_Callback_t pExitCallback,
   DSL_CLI_Event_Callback_t pEventCallback
);

DSL_Error_t DSL_CPE_Unregister(DSL_CLI_Context_t *pCLIContext);

/**
   Read the firmware binary from file system.

   \param sFirmwareName Firmware binary file name [I]
   \param pFirmware Firmware binary data [O]
   \param nFirmwareSize Firmware size [O]

   \remark
   The data memory will be allocated via DSL_Malloc() .
   It should be freed after usage with DSL_Free() .

   \return
   - DSL_ERROR On error
   - DSL_SUCCESS Binary loaded successfully into the memory
*/
DSL_Error_t DSL_FirmwareLoad
(
   DSL_char_t const *sFirmwareName,
   DSL_uint8_t **pFirmware,
   DSL_uint32_t *nFirmwareSize
);

/**
   Write a binary to the file system.

   \param sFirmwareName Firmware binary file name [I]
   \param pFirmware Firmware binary data [I]
   \param nFirmwareSize Firmware size [I]

   \return
   - DSL_ERROR On error
   - DSL_SUCCESS Binary loaded successfully into the memory
*/
DSL_Error_t DSL_FirmwareWrite
(
   DSL_char_t const *sFirmwareName,
   DSL_uint8_t *pFirmware,
   DSL_uint32_t nFirmwareSize
);
#endif /* INCLUDE_DSL_CPE_CLI_SUPPORT */

#endif /* DSL_CPE_CLI_H */

