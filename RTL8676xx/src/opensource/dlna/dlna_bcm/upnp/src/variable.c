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
 * $brcm_Workfile: variable.c $
 * $brcm_Revision: 4 $
 * $brcm_Date: 9/30/09 3:27p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/variable.c $
 * 
 * 4   9/30/09 3:27p dliu
 * SW7405-2482: Split SetText function into two
 * 
 * 3   9/15/09 5:28p dliu
 * SW7405-2482: Add implementation for BUPnPStateVariable_SetStringN
* 
***************************************************************************/
#include "service.h"
#include "variable.h"
#include "timer.h"


BUPnPStateVariableHandle BUPnPStateVariable_Create(BUPnPStateVariableInfo *pVariableInfo)
{
    BUPnPStateVariableHandle hStateVariable;

    hStateVariable = (BUPnPStateVariableHandle)BUPnPCalloc(1, sizeof(struct BUPnPStateVariable));
    if ( hStateVariable == NULL )
        return NULL;

    hStateVariable->variableInfo = pVariableInfo;
    hStateVariable->name = pVariableInfo->name;
    if ( !CString_Create(&hStateVariable->value) )
        return NULL;

    if (pVariableInfo->defaultValue)
        CString_SetTextFmt(&hStateVariable->value, "%s", pVariableInfo->defaultValue);

    return hStateVariable;
}

void BUPnPStateVariable_Destroy(BUPnPStateVariableHandle hStateVariable) 
{
    CString_Destroy(&hStateVariable->value);
    BUPnPFree(hStateVariable);
    hStateVariable = NULL;
}

int32_t BUPnPStateVariable_GetInt32(BUPnPStateVariableHandle hStateVariable)
{
    return CString_ToInt32(&hStateVariable->value);
}

uint32_t BUPnPStateVariable_GetUInt32(BUPnPStateVariableHandle hStateVariable)
{
    return CString_ToUInt32(&hStateVariable->value);
}

const char* BUPnPStateVariable_GetString(BUPnPStateVariableHandle hStateVariable)
{
    return CString_GetText(&hStateVariable->value);
}

BUPnPError BUPnPStateVariable_SetInt32(BUPnPStateVariableHandle hStateVariable, int32_t value)
{
    if ( !CString_SetTextFmt(&hStateVariable->value, "%d", value) )
        return UPNP_E_OUT_OF_MEMORY;

    BUPnPStateVariable_SetModified(hStateVariable, true);

    return UPNP_E_SUCCESS;
}

BUPnPError BUPnPStateVariable_SetUInt32(BUPnPStateVariableHandle hStateVariable, uint32_t value)
{
    if ( !CString_SetTextFmt(&hStateVariable->value, "%u", value) )
        return UPNP_E_OUT_OF_MEMORY;

    BUPnPStateVariable_SetModified(hStateVariable, true);

    return UPNP_E_SUCCESS;
}

BUPnPError BUPnPStateVariable_SetString(BUPnPStateVariableHandle hStateVariable, const char* value)
{
    if ( !CString_SetText(&hStateVariable->value, value) )
        return UPNP_E_OUT_OF_MEMORY;

    BUPnPStateVariable_SetModified(hStateVariable, true);

    return UPNP_E_SUCCESS;
}

BUPnPError BUPnPStateVariable_SetStringN(BUPnPStateVariableHandle hStateVariable, const char* value, int length)
{
    if ( !CString_SetTextN(&hStateVariable->value, length, value) )
        return UPNP_E_OUT_OF_MEMORY;

    BUPnPStateVariable_SetModified(hStateVariable, true);

    return UPNP_E_SUCCESS;
}

bool BUPnPStateVariable_IsModified(BUPnPStateVariableHandle hStateVariable)
{
    return (hStateVariable->modified) ? true : false;
}

const char* BUPnPStateVariable_GetName(BUPnPStateVariableHandle hStateVariable)
{
    return hStateVariable->name;
}

unsigned char BUPnPStateVariable_GetAttribute(BUPnPStateVariableHandle hStateVariable)
{
    return hStateVariable->variableInfo->attributes;
}

const char* BUPnPStateVariable_GetValue(BUPnPStateVariableHandle hStateVariable)
{
    return (const char*)hStateVariable->value.bytes;
}

void BUPnPStateVariable_SetModified(BUPnPStateVariableHandle hStateVariable, bool modified)
{
    hStateVariable->modified = modified;

    if ( modified )
    {
        /* If it is a moderated event, we let the timer catch it */
        if ((hStateVariable->variableInfo->attributes & BUPnPAttribute_Evented) == BUPnPAttribute_Evented)
        {
            if ((hStateVariable->variableInfo->attributes & BUPnPAttribute_Moderated) != BUPnPAttribute_Moderated)
                BUPnPService_Notify(hStateVariable->service);
        }
        else if ((hStateVariable->variableInfo->attributes & BUPnPAttribute_IndirectEvent) == BUPnPAttribute_IndirectEvent)
        {
            if (!hStateVariable->service->disableUpdate)
                BUPnPService_Notify(hStateVariable->service);
        }
    }
}
