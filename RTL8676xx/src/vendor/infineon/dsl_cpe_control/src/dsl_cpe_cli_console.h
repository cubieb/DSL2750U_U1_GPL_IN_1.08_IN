/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef DSL_CPE_CLI_CONSOLE_H
#define DSL_CPE_CLI_CONSOLE_H

/** \file
   DSL daemon command line interface for console (single key input)
*/

/** \defgroup LIB_DSL_CLI_CON_CONF Configuration of DSL CPE API console
  @{ */

/** maximum length of command line */
#define DSL_MAX_COMMAND_LINE_LENGTH    256
/** maximum length of command names */
#define DSL_MAX_FUNCTION_NAME_LENGTH   64
/** maximum of history elements */
#define MAX_OLD_COMMAND                16

/** @} */

/** Context for console (including simple "editor") */
typedef struct DSL_CPE_Console_Context DSL_CPE_Console_Context_t;

DSL_Error_t DSL_CPE_Console_Init(DSL_CPE_Console_Context_t **pConsoleRefContext,
                                 DSL_int_t fd, DSL_File_t *in, DSL_File_t *out);
DSL_Error_t DSL_CPE_Console_Shutdown(DSL_CPE_Console_Context_t *pConsoleContext);
DSL_Error_t DSL_CPE_Handle_Console(DSL_CPE_Console_Context_t *pConsoleContext);

#endif /* DSL_CPE_CLI_CONSOLE_H */

