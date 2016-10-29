/***************************************************************************
*     (c)2008-2009 Broadcom Corporation
*  
*  This program is the proprietary software of Broadcom Corporation and/or its licensors,
*  and may only be used, duplicated, modified or distributed pursuant to the terms and
*  conditions of a separate, written license agreement executed between you and Broadcom
*  (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
*  no license (express or implied), right to use, or waiver of any kind with respect to the
*  Software, and Broadcom expressly reserves all rights in and to the Software and all
*  intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
*  HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
*  NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.  
*   
*  Except as expressly set forth in the Authorized License,
*   
*  1.     This program, including its structure, sequence and organization, constitutes the valuable trade
*  secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
*  and to use this information only in connection with your use of Broadcom integrated circuit products.
*   
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
*  AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
*  WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO 
*  THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES 
*  OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, 
*  LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION 
*  OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF 
*  USE OR PERFORMANCE OF THE SOFTWARE.
*  
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS 
*  LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR 
*  EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR 
*  USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF 
*  THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT 
*  ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE 
*  LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF 
*  ANY LIMITED REMEDY.
 *
 * $brcm_Workfile: bdlna_debug.c $
 * $brcm_Revision: 1 $
 * $brcm_Date: 7/28/09 4:53p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/core/src/bdlna_debug.c $
 * 
 * 1   7/28/09 4:53p dliu
 * PR55165: Add debug support
* 
***************************************************************************/
#include "bdlna.h"
#include "upnp_priv.h"
#include "bdlna_debug.h"

#if defined(_DEBUG) || defined(DEBUG)

#define DEBUG_ERROR_STRING        "Error"
#define DEBUG_WARNING_STRING    "Warning"

static uint32_t _debuglevel = BDlnaDebugLevel_Error | BDlnaDebugLevel_Warning;
static uint32_t _debugCategory = BDlnaDebugCategory_All;

void BDlnaDebug_Print( BDlnaDebugLevel level, BDlnaDebugCategory category, const char* fileName, int32_t lineNumber, const char *format, ... )
{
    const char* pstrLevel = NULL;

    va_list args;

    va_start(args, format);

    if ((_debuglevel & level) != 0)
    {
        if ( (level & BDlnaDebugLevel_Error) == BDlnaDebugLevel_Error )
            pstrLevel = DEBUG_ERROR_STRING;
        else if ( (level & BDlnaDebugLevel_Warning) == BDlnaDebugLevel_Warning )
            pstrLevel = DEBUG_WARNING_STRING;

        if  ((_debugCategory & category) != 0)
        {
            if ( pstrLevel != NULL )
                fprintf(stderr, "%s: ", pstrLevel);

            if ( fileName != NULL )
                fprintf(stderr, "%s(%d): ", fileName, lineNumber);

            vfprintf(stderr, format, args);
            fprintf(stderr, "\n");
        }
    }

    va_end(args);
}

void BDlnaDebug_SetSettings(uint32_t level, uint32_t category)
{
    _debuglevel = level;
    _debugCategory = category;
}


#endif

