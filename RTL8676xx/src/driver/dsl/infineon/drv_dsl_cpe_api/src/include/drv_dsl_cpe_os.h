/******************************************************************************

                               Copyright (c) 2007
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  For licensing information, see the file 'LICENSE' in the root folder of
  this software module.

******************************************************************************/

#ifndef _DRV_DSL_CPE_OS_H
#define _DRV_DSL_CPE_OS_H

#ifdef __cplusplus
   extern "C" {
#endif

/** \addtogroup DRV_DSL_CPE_COMMON
 @{ */

#ifdef __LINUX__
#ifdef __KERNEL__
#include "drv_dsl_cpe_os_linux.h"
#else
#error "This file could not be included from the user space application sources"
#endif
#else
#error "Please define OS"
#endif

/**
   This function is a special implementation to realize both printing to the
   standard output (console) or writing the data to the callback function
   \ref DSL_MessageDump_Callback_t which will be done if a callback function
   was registered before including a appropriate configuration during
   registering.
   \attention For VxWorks there is no functionality to call a registered
   callback function instead of printing to the console.
   It will be printed to the console in any case (missing 'vsnprintf'
   in VxWorks)!

   \param pContext Pointer to dsl library context structure, [I]
   \param fmt      Pointer to format string (please refer to documentation of
                   'sprintf' function), [I]

   \return
   return the number of characters written, not including the terminating null
   character, or a negative value if an output error occurs.
*/
#ifndef SWIG

int DSL_debug_printf(DSL_Context_t const *pContext, DSL_char_t const *fmt, ...);

#endif /* SWIG */

/**
   This function converts DSL CPE API error code to the appropriate error code
   typical for the OS.

   \param nError   DSL CPE API error code, [I]

   \return
   return the appropriate OS error code for the passed with nError parameter
   DSL CPE API error code.
*/
#ifndef SWIG

int DSL_ErrorToOS(DSL_Error_t nError);

#endif /* SWIG */

/**
   This function copies input memory block from user/kernel space to kernel space.

   \param bIsInKernel whether the memory block is already in the kernel space, [I]
   \param pDest       Destination address, [I]
   \param pSrc        Source address, [I]
   \param nSize       Memory block size, [I]

   \return
   pointer equal to pDest
*/
#ifndef SWIG

DSL_void_t* DSL_IoctlMemCpyFrom
(
   DSL_boolean_t bIsInKernel,
   DSL_void_t    *pDest,
   DSL_void_t    *pSrc,
   DSL_size_t    nSize
);

#endif /* SWIG */

/**
   This function copies output memory block to user/kernel space from kernel space.

   \param bIsInKernel whether the memory block should be copied to kernel space, [I]
   \param pDest       Destination address, [I]
   \param pSrc        Source address, [I]
   \param nSize       Memory block size, [I]

   \return
   pointer equal to pDest
*/
#ifndef SWIG

DSL_void_t* DSL_IoctlMemCpyTo
(
   DSL_boolean_t bIsInKernel,
   DSL_void_t    *pDest,
   DSL_void_t    *pSrc,
   DSL_size_t    nSize
);

#endif /* SWIG */

/**
   This function puts the calling process to sleep for a give number
   of milliseconds.

   \param msec       time to sleep (msec) , [I]

   \return
   None
*/
#ifndef SWIG
DSL_void_t DSL_MSleep(DSL_uint32_t msec);
#endif /* SWIG */

/**
   This function allocates a memory.

   \param nSize       Memory block size, [I]

   \return
   pointer to allocated memory block, DSL_NULL in case of error
*/
#ifndef SWIG

DSL_void_t* DSL_Malloc
(
   DSL_size_t    nSize
);

#endif /* SWIG */

/**
   This function releases a memory.

   \param pPtr       Memory block to release. [I]
*/
#ifndef SWIG

DSL_void_t DSL_Free
(
   DSL_void_t*    pPtr
);

#endif /* SWIG */

/**
   This function allocates a memory as a continuous block
   in the phisical memory.

   \param nSize       Memory block size, [I]

   \return
   pointer to allocated memory block, DSL_NULL in case of error
*/
#ifndef SWIG

DSL_void_t* DSL_PMalloc
(
   DSL_size_t    nSize
);

#endif /* SWIG */

/**
   This function releases a memory, allocated by DSL_PMalloc.

   \param pPtr       Memory block to release. [I]
*/
#ifndef SWIG

DSL_void_t DSL_PFree
(
   DSL_void_t*    pPtr
);

#endif /* SWIG */

/**
   This function allocates a memory, resulting block may be non-continuous
   in the phisical memory.

   \param nSize       Memory block size, [I]

   \return
   pointer to allocated memory block, DSL_NULL in case of error
*/
#ifndef SWIG

DSL_void_t* DSL_VMalloc
(
   DSL_size_t    nSize
);

#endif /* SWIG */

/**
   This function releases a virtual memory.

   \param pPtr       Memory block to release. [I]
*/
#ifndef SWIG

DSL_void_t DSL_VFree
(
   DSL_void_t*    pPtr
);

#endif /* SWIG */

/**
   This function fills a memory block.

   \param pPtr        Memory block address, [I]
   \param nChar       Specifies a byte which should be used to fill memory block, [I]
   \param nSize       Memory block size to fill, [I]

   \return
   equals to pPtr
*/
#ifndef SWIG

DSL_void_t* DSL_MemSet
(
   DSL_void_t    *pPtr,
   DSL_char_t    nChar,
   DSL_size_t    nSize
);

#endif /* SWIG */

/**
   This function implements snprintf OS-specific analog for DSL CPE API.

   \param pPtr       Memory block to release. [I]
*/
#ifndef SWIG

DSL_int_t DSL_snprintf
(
   DSL_char_t        *pStr,
   DSL_size_t        nStrSz,
   const DSL_char_t  *pFormat,  ...
);

#endif /* SWIG */

/**
   This function implements sscanf OS-specific analog for DSL CPE API.

   \param pPtr       Memory block to release. [I]
*/
#ifndef SWIG

DSL_int_t DSL_sscanf( DSL_char_t *buf, DSL_char_t const *fmt, ...);

#endif /* SWIG */

/**
   This function creates and starts a thread.

   \param pPtr       Memory block to release. [I]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code
*/
#ifndef SWIG

DSL_uint32_t DSL_Thread( DSL_ThreadFunc_t func, DSL_void_t *pArg);

#endif /* SWIG */

/**
   This function does operating system specific actions after new thread is started.

   \param sName       Name of a thread. [I]

   \return  Return values are defined within the \ref DSL_Error_t definition
    - DSL_SUCCESS in case of success
    - DSL_ERROR if operation failed
    - or any other defined specific error code
*/
#ifndef SWIG
DSL_void_t DSL_ThreadInit(DSL_char_t *sName);
#endif /* SWIG */

/**
   This function sets completion event for thread and finishes it

   \param pCompletion      Completion event to set [I],
   \param nCode            exit code for thread [I]
*/
#ifndef SWIG
DSL_void_t DSL_ThreadCompleteExit
(
   DSL_completion_t *pCompletion,
   DSL_int32_t      nCode
);
#endif /* SWIG */

/** @} DRV_DSL_CPE_COMMON */

#ifdef __cplusplus
}
#endif

#endif
