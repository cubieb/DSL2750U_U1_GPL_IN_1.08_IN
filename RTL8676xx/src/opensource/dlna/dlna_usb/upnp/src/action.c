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
 * $brcm_Workfile: action.c $
 * $brcm_Revision: 4 $
 * $brcm_Date: 9/30/09 4:14p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/action.c $
 * 
 * 4   9/30/09 4:14p dliu
 * SW7405-2482: Change to support dynamic service creation
 * 
 * 3   7/27/09 10:42a dliu
 * PR55165: Add include of upnp.h
* 
***************************************************************************/
#include "upnp.h"
#include "upnp_priv.h"
#include "action.h"

typedef struct BUPnPActionErrorType
{
    BUPnPError code;
    const char* text;
}BUPnPActionErrorType;

static BUPnPActionErrorType g_upnpErrors[] =
{
    { UPNP_E_INVALID_ACTION,                    "Invalid Action" },
    { UPNP_E_INVALID_ARG,                        "Invalid Args" },
    { UPNP_E_ACTION_FAILED,                        "Action Failed" },
    { UPNP_E_ARGUMENT_VALUE_INVALID,            "Argument Value Invalid" },
    { UPNP_E_ARGUMENT_VALUE_OUT_OF_RANGE,        "Argument Value Out Of Range" },
    { UPNP_E_OPTIONAL_ACTION_NOT_IMPLEMENTED,    "Optional Action Not Implemented" },
    { UPNP_E_OUT_OF_MEMORY,                        "Out Of Memory" },
    { UPNP_E_HUMAN_INTERVENTION_REQUIRED,        "Human Intervention Required" },
    { UPNP_E_STRING_ARGUMENT_TOO_LONG,            "String Argument Too Long" },
    { UPNP_E_ACTION_NOT_AUTHORIZED,                "Action Not Authorized" },
    { UPNP_E_SIGNATURE_FAILURE,                    "Signature Failure" },
    { UPNP_E_SIGNATURE_MISSING,                    "Signature Missing" },
    { UPNP_E_NOT_ENCRYPTED,                        "Not Encrypted" },
    { UPNP_E_INVALID_SEQUENCE,                    "Invalid Sequence" },
    { UPNP_E_INVALID_CONTROL_URL,                "Invalid Control URL" },
    { UPNP_E_NO_SUCH_SESSION,                    "No Such Session" },
    { UPNP_E_SUCCESS,                            NULL }
};


BUPnPActionHandle BUPnPAction_Create(BUPnPActionInfo* pActionInfo)
{
    size_t i, argumentCount;
    BUPnPActionHandle hAction;
    BUPnPArgumentInfo **ppArgumentInfo;

    hAction = (BUPnPActionHandle)BUPnPCalloc(1, sizeof(struct BUPnPAction));
    if ( hAction == NULL )
        return NULL;

    hAction->actionInfo = pActionInfo;
    hAction->name = pActionInfo->name;
    hAction->handler = pActionInfo->handler;

    argumentCount = 0;
    
    if (pActionInfo->arguments != NULL)
    {
        for (ppArgumentInfo = pActionInfo->arguments; *ppArgumentInfo; ppArgumentInfo++)
        {
            /* in arguments always come before out */
            if ((*ppArgumentInfo)->attributes & BUPnPAttribute_In)
                hAction->inArgumentCount++;
            argumentCount++;
        }

        hAction->arguments = (struct BUPnPArgument*)BUPnPCalloc(argumentCount, sizeof(struct BUPnPArgument));
        if ( hAction->arguments == NULL )
            return NULL;

        hAction->argumentCount = argumentCount;


        i=0;
        for (ppArgumentInfo = pActionInfo->arguments; *ppArgumentInfo; ppArgumentInfo++, i++)
        {
            hAction->arguments[i].argumentInfo = *ppArgumentInfo;
            hAction->arguments[i].name = (*ppArgumentInfo)->name;
            if ( !CString_Create(&hAction->arguments[i].value) )
                return NULL;
        }
    }

    return hAction;
}

void BUPnPAction_Destroy(BUPnPActionHandle hAction)
{
    size_t i;

    for ( i=0; i<hAction->argumentCount; i++ )
        CString_Destroy(&hAction->arguments[i].value);

    BUPnPFree(hAction->arguments);
    BUPnPFree(hAction);
    hAction = NULL;
}

BUPnPError BUPnPAction_Invoke(BUPnPActionHandle hAction)
{
    return (hAction->handler)(hAction->service, hAction);
}

const char* BUPnPAction_GetErrorText(BUPnPError errorCode)
{
    BUPnPActionErrorType* pError = NULL;
    for (pError = g_upnpErrors; pError->text; pError++)
    {
        if (pError->code == errorCode)
            return pError->text;
    }
    return g_upnpErrors[0].text;
}

BUPnPError BUPnPAction_GetInt32Argument(BUPnPActionHandle hAction, uint32_t index, int32_t* value)
{
    BUPnPArgumentHandle hArg;
    hArg = BUPnPAction_GetArgumentByIndex(hAction, index);
    if ( hArg == NULL )
        return UPNP_E_INVALID_ARG;
        
    *value = CString_ToInt32(&hArg->value);
    
    return UPNP_E_SUCCESS;
}

BUPnPError BUPnPAction_GetUInt32Argument(BUPnPActionHandle hAction, uint32_t index, uint32_t* value)
{
    BUPnPArgumentHandle hArg;
    hArg = BUPnPAction_GetArgumentByIndex(hAction, index);
    if ( hArg == NULL )
        return UPNP_E_INVALID_ARG;
        
    *value = CString_ToUInt32(&hArg->value);
    
    return UPNP_E_SUCCESS;
}

BUPnPError BUPnPAction_GetDoubleArgument(BUPnPActionHandle hAction, uint32_t index, double* value)
{
    BUPnPArgumentHandle hArg;
    hArg = BUPnPAction_GetArgumentByIndex(hAction, index);
    if ( hArg == NULL )
        return UPNP_E_INVALID_ARG;
        
    *value = CString_ToDouble(&hArg->value);

    return UPNP_E_SUCCESS;
}

BUPnPError BUPnPAction_GetStringArgument(BUPnPActionHandle hAction, uint32_t index, const char** value)
{
    BUPnPArgumentHandle hArg;
    hArg = BUPnPAction_GetArgumentByIndex(hAction, index);
    if ( hArg == NULL )
        return UPNP_E_INVALID_ARG;
        
    *value = CString_GetText(&hArg->value);

    return UPNP_E_SUCCESS;
}

BUPnPError BUPnPAction_SetInt32Argument(BUPnPActionHandle hAction, uint32_t index, int32_t value)
{
    BUPnPArgumentHandle hArg;
    hArg = BUPnPAction_GetArgumentByIndex(hAction, index);
    if ( hArg == NULL )
        return UPNP_E_INVALID_ARG;

    CString_SetTextFmt(&hArg->value, "%d", value);

    return UPNP_E_SUCCESS;
}

BUPnPError BUPnPAction_SetUInt32Argument(BUPnPActionHandle hAction, uint32_t index, uint32_t value)
{
    BUPnPArgumentHandle hArg;
    hArg = BUPnPAction_GetArgumentByIndex(hAction, index);
    if ( hArg == NULL )
        return UPNP_E_INVALID_ARG;

    CString_SetTextFmt(&hArg->value, "%u", value);

    return UPNP_E_SUCCESS;
}

BUPnPError BUPnPAction_SetDoubleArgument(BUPnPActionHandle hAction, uint32_t index, double value)
{
    BUPnPArgumentHandle hArg;
    hArg = BUPnPAction_GetArgumentByIndex(hAction, index);
    if ( hArg == NULL )
        return UPNP_E_INVALID_ARG;

    CString_SetTextFmt(&hArg->value, "%lf", value);

    return UPNP_E_SUCCESS;
}

BUPnPError BUPnPAction_SetStringArgument(BUPnPActionHandle hAction, uint32_t index, const char* value)
{
    BUPnPArgumentHandle hArg;
    hArg = BUPnPAction_GetArgumentByIndex(hAction, index);
    if ( hArg == NULL )
        return UPNP_E_INVALID_ARG;

    CString_SetText(&hArg->value, value);

    return UPNP_E_SUCCESS;
}

BUPnPArgumentHandle BUPnPAction_GetArgumentByName(BUPnPActionHandle hAction, const char* actionName)
{
    size_t index;
    for (index=0;index<hAction->argumentCount;index++)
    {
        if (MATCH(actionName, hAction->arguments[index].name))
        {
            return &hAction->arguments[index];
        }
    }
    return NULL;
}
 
BUPnPArgumentHandle BUPnPAction_GetArgumentByIndex(BUPnPActionHandle hAction, size_t index)
{
    return ( index < hAction->argumentCount ) ? &hAction->arguments[index] : NULL;
}
