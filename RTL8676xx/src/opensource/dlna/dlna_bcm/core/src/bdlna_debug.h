/******************************************************************************
 *    (c)2009 Broadcom Corporation
 * 
 * This program is the proprietary software of Broadcom Corporation and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.  
 *  
 * Except as expressly set forth in the Authorized License,
 *  
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *  
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO 
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES 
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, 
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION 
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF 
 * USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS 
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR 
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR 
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT 
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE 
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF 
 * ANY LIMITED REMEDY.
 *
 * $brcm_Workfile: bdlna_debug.h $
 * $brcm_Revision: 1 $
 * $brcm_Date: 7/28/09 4:53p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/core/src/bdlna_debug.h $
 * 
 * 1   7/28/09 4:53p dliu
 * PR55165: Add debug support
 * 
 *****************************************************************************/
#ifndef DEBUG_H
#define DEBUG_H

#ifdef __cplusplus
extern "C" {
#endif


#if defined(_DEBUG) || defined(DEBUG)

# define BDLNA_P_UNWRAP(...) __VA_ARGS__ 

#define DLNA_DEBUG_ERROR(category, fmt) BDlnaDebug_Print(BDlnaDebugLevel_Error, category, __FILE__, __LINE__, BDLNA_P_UNWRAP fmt) 
#define DLNA_DEBUG_WARNING(category, fmt) BDlnaDebug_Print(BDlnaDebugLevel_Warning, category, __FUNC__, __LINE__, BDLNA_P_UNWRAP fmt)
#define DLNA_DEBUG_TRACE(category, fmt) BDlnaDebug_Print(BDlnaDebugLevel_Trace, category, NULL, 0, BDLNA_P_UNWRAP fmt)

void BDlnaDebug_Print( BDlnaDebugLevel level, BDlnaDebugCategory category, const char* fileName, int32_t lineNumber, const char *format, ... );

#else

#define DLNA_DEBUG_ERROR(category, fmt, ...)((void)0)
#define DLNA_DEBUG_WARNING(category, fmt, ...)((void)0)
#define DLNA_DEBUG_TRACE(category, fmt, ...)((void)0)

#endif

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_H */

