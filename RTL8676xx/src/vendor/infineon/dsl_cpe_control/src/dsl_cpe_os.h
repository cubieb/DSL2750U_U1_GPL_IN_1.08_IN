/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef DSL_CPE_API_OS_H
#define DSL_CPE_API_OS_H

#ifdef __cplusplus
   extern "C" {
#endif

/** \file
   Operating System depending files, definitions and functions
*/
#include "drv_dsl_cpe_api_types.h"
#include "drv_dsl_cpe_api.h"

#ifdef LINUX
#include "dsl_cpe_linux.h"
#elif WIN32
#include "dsl_cpe_win32.h"
#else
#error please define your OS for the CPE Control adaptation
#endif

#ifndef __BIG_ENDIAN
   #error please define the __BIG_ENDIAN macro
#endif

#ifndef __BYTE_ORDER
   #error please specify the endianess of your target system
#endif

/**
   System definitions
*/
/** Default stack size */
#define DSL_OS_DEFAULT_STACK_SIZE   (4096)

/** Control task stack size */
#define DSL_OS_CONTROL_STACK_SIZE   (4096)

/** SOAP task stack size */
#define DSL_OS_SOAP_STACK_SIZE      (20000)

/** Pipe task stack size */
#define DSL_OS_PIPE_STACK_SIZE      (8192)

/**
   Carriage Return + Line Feed, maybe overwritten by compile switches
   or OS-specific adaptation */
#ifndef DSL_CRLF
#define DSL_CRLF                    "\n\r"
#endif

/** thread entry function */
typedef DSL_int_t (*DSL_ThreadFunction_t)(DSL_uint32_t, DSL_uint32_t);

/**
   Creates a new thread.

   \param
      pName             - specifies the 8-char name of the task
   \param
      pThreadFunction   - specifies the entry function of the task
   \param
      nStackSize        - specifies the size of the thread stack
   \param
      nArg1             - first argument passed to task entry function
   \param
      nArg2             - second argument passed to task entry function

   \return
   Returns DSL_ERROR in case of an error, otherwise the thread identifier.

   \return
   This functions disables timeslicing and enables preemption of
   the task by default.
*/
DSL_Thread_t DSL_ThreadCreate(DSL_char_t *pName,
                              DSL_ThreadFunction_t pThreadFunction,
                              DSL_uint32_t nStackSize,
                              DSL_uint32_t nArg1,
                              DSL_uint32_t nArg2);

/**
   Delete a given thread.

   \param
      pid - Thread identifier which should be deleted.

   \return
      - DSL_SUCCESS thread was successful deleted
      - DSL_ERROR thread was not deleted
*/
DSL_Error_t DSL_ThreadDelete(DSL_Thread_t pid);

DSL_void_t DSL_ThreadWait(DSL_Thread_t tid);

/**
   Get current thread identifier.

   \return
   Returns current thread identifier.

   \remarks
   In case of Linux this routine returns the thread id.
*/
DSL_Thread_t DSL_CurrentThreadGet(DSL_void_t);

/**
   Open a device
*/
DSL_int_t DSL_Open(const DSL_char_t *pName);

/**
   Close a device
*/
DSL_int_t DSL_Close(const DSL_int_t fd);

/**
   Write to a device
*/
DSL_int_t DSL_Write(const DSL_int_t fd, const DSL_void_t *pData, const DSL_uint32_t nSize);

/**
   Read from a device
*/
DSL_int_t DSL_Read(const DSL_int_t fd, DSL_void_t *pData, const DSL_uint32_t nSize);

/**
   Control a device
*/
DSL_int_t DSL_Ioctl(const DSL_int_t fd, const DSL_uint32_t cmd, DSL_int_t param);

/**
   Wait for a device wake up
*/
DSL_int_t DSL_Select(const DSL_int_t max_fd,
   const DSL_fd_set_t *read_fd_in,
   DSL_fd_set_t *read_fd_out,
   const DSL_uint32_t timeout_msec);

/**
   Fill a descriptor set with zeros.
*/
DSL_void_t DSL_FD_ZERO(DSL_fd_set_t *set);

/**
   Mark a descriptor in use.
*/
DSL_void_t DSL_FD_SET(DSL_int_t fd, DSL_fd_set_t *set);

/**
   Clear a descriptor in use flag.
*/
DSL_void_t DSL_FD_CLR(DSL_int_t fd, DSL_fd_set_t *set);

/**
   Check if a descriptor is set.
*/
DSL_int_t DSL_FD_ISSET(DSL_int_t fd, const DSL_fd_set_t *set);

/**
   Get a byte from stdin .
*/
DSL_int_t DSL_GetChar(DSL_void_t);

/**
   Write a byte stdout .
*/
DSL_int_t DSL_PutChar(DSL_char_t c, DSL_File_t *stream);

/**
   Create a pipe.

   \param
      pName - pipe name
   \return
      - DSL_SUCCESS on success
      - DSL_ERROR on failure
 */
DSL_Error_t DSL_PipeCreate(DSL_char_t *pName);

/**
   Open a pipe.

   \param
      pName - pipe name
   \return
      - pointer to FILE structure
      - in case of error the return value is NULL
 */
DSL_File_t *DSL_PipeOpen(DSL_char_t *pName, DSL_boolean_t reading, DSL_boolean_t blocking);

/**
   Close a pipe.

   \param
      pipe - pipe stream
   \return
      - (0) on success
      - (-1) on failure
*/
DSL_Error_t DSL_PipeClose(DSL_File_t *file);

/**
   Create a semaphore.

   \param pName semaphore name

   \return
   The semaphore identifier will be returned.
*/
DSL_Semaphore_t DSL_SemaphoreCreate(DSL_char_t *pName);

/**
   Delete a semaphore.

   \param sem semaphore identifier

   \return
      - DSL_SUCCESS on success
      - DSL_ERROR on failure
 */
DSL_int_t DSL_SemaphoreDelete(DSL_Semaphore_t sem);

/**
   Gets a semaphore.

   \param sem        semaphore identifier
   \param nTimeout   timeout value
                     - 0: no wait
                     - -1: wait forever
                     - any other value: waiting for specified amount of milliseconds

   \return
      - (0) on success
      - (-1) on failure
*/
DSL_int_t DSL_SemaphoreGet(DSL_Semaphore_t sem, DSL_uint32_t nTimeout);

/**
   Return the semaphore

   \param sem        semaphore identifier

   \return
      - (0) on success
      - (-1) on failure
 */
DSL_int_t DSL_SemaphoreSet(DSL_Semaphore_t sem);

/**
   Sleep specified number of seconds.

   \param nSeconds Number of seconds to sleep
*/
DSL_void_t DSL_Sleep(DSL_uint32_t nSeconds);

/**
   Sleep specified number of milliseconds.

   \param nMs Number of milliseconds to sleep
*/
DSL_void_t DSL_MSleep(DSL_uint32_t nMs);

/**
   Installs system dependant handlers (e.g. signal handler on linux)
*/
DSL_void_t DSL_HandlerInstall(DSL_void_t);

/**
   Allocates a memory block

   \param size Bytes to allocate

   \return
   returns a DSL_void_t pointer to the allocated space, NULL if there's not
   sufficient memory space available.
*/
DSL_void_t *DSL_Malloc(DSL_uint32_t size);

/**
   Deallocates a memory block

   \param memblock Previously allocated memory block that should be freed

*/
void DSL_Free(DSL_void_t *memblock);

/**
   Print to a file, pipe, stdout, stderr or memory file.
*/
DSL_int_t DSL_FPrintf(DSL_File_t *stream, const DSL_char_t *format, ...)
#ifdef __GNUC__
   __attribute__ ((format (printf, 2, 3)))
#endif
   ;

/**
   This function implements debug printouts.

   \param fmt      Pointer to format string (please refer to documentation of
                   'sprintf' function), [I]

   \return
   return the number of characters written, not including the terminating null
   character, or a negative value if an output error occurs.
*/
int DSL_debug_printf(DSL_char_t *fmt, ...);

/**
   Open a file.
*/
DSL_File_t *DSL_FOpen(const DSL_char_t *name,  const DSL_char_t *mode);

#ifdef INCLUDE_DSL_CPE_SOAP_SUPPORT
/**
   Open a memory file. The size is limited to 32 kBytes.

   \param buf     temporary buffer for for fprintf operation.
   \param size    buffer size
   \param mode    not used

   \return
      - pointer to FILE structure
      - in case of error the return value is NULL
*/
DSL_File_t *DSL_FMemOpen(DSL_char_t *buf, const DSL_uint32_t size, const DSL_char_t *mode);

#endif /* #ifdef INCLUDE_DSL_CPE_SOAP_SUPPORT */
/**
   Close the file or memory file.
*/
DSL_int_t DSL_FClose(DSL_File_t *fd);

/**
   Flush a file or pipe.
*/
DSL_int_t DSL_FFlush(DSL_File_t *fd);

/**
   Read from file, stdin .
*/
DSL_int_t DSL_FRead(DSL_void_t *buf, DSL_uint32_t size,  DSL_uint32_t count, DSL_File_t *stream);

/**
   Write to a file, pipe, stdout.
*/
DSL_int_t DSL_FWrite(const DSL_void_t *buf, DSL_uint32_t size, DSL_uint32_t count, DSL_File_t *stream);

/**
   End of file test of a file.
*/
DSL_int_t DSL_Feof(DSL_File_t *stream);

/**
   Read a string from the file.
*/
DSL_char_t *DSL_FGets(DSL_char_t *str, DSL_int_t n, DSL_File_t *stream);

/**
  get file status
*/
DSL_int_t DSL_FStat(DSL_char_t *str, DSL_stat_t *st);

/**
   Set environment variable
*/
DSL_Error_t DSL_SetEnv(const DSL_char_t *sName, const DSL_char_t *sValue);

/**
   Execute a shell command
*/
DSL_Error_t DSL_System(const DSL_char_t *sCommand);

#ifdef __cplusplus
}
#endif

#endif /* DSL_CPE_API_OS_H */
