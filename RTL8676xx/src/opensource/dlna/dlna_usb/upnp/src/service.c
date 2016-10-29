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
 * $brcm_Workfile: service.c $
 * $brcm_Revision: 6 $
 * $brcm_Date: 9/30/09 4:17p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/service.c $
 * 
 * 6   9/30/09 4:17p dliu
 * SW7405-2482: Add support for dynamic service creation
 * 
 * 5   9/15/09 5:48p dliu
 * SW7405-2482: Add function to dynamically add StateVariables
 * 
 ***************************************************************************/
#include "upnp_priv.h"
#include "osl.h"
#include "device.h"
#include "service.h"
#include "gena.h"
#include "nanoxml.h"
#include "debug.h"

#define PARSING_VARIABLES 1
#define PARSING_ACTIONS 2
#define PARSING_ARGUMENTS 3
#define PARSING_ALLOWED_VALUE 4

struct BUPnPArgumentHolder
{
    BUPnPArgumentInfo* info;
    BLST_ENTRY(BUPnPArgumentHolder) link;
};

BLST_HEAD(BUPnPArgumentList, BUPnPArgumentHolder);

typedef struct BUPnPServiceParser
{
    int state;
    BUPnPServiceHandle hService;
    BUPnPStateVariableInfo* pVariableInfo;
    BUPnPActionInfo* pActionInfo;
    struct BUPnPArgumentList argumentList;
    BUPnPArgumentInfo* pArgument;
} BUPnPServiceParser;



static const char* BUPnPService_TypeToString(unsigned type);
static BUPnPError BUPnPService_BuildModeratedEventText(BUPnPServiceHandle hService, BUPnPStateVariableHandle hModerateVariable, bool bSentAll);
static void BUPnPService_UpdateEvent(TimerHandle hTimer, void *arg);
static void BUPnPService_Data(nxml_t handle, const char *tag_name, unsigned len);
static void BUPnPService_TagBegin(nxml_t handle, const char *tag_name, unsigned len);
static void BUPnPService_TagEnd(nxml_t handle, const char *tag_name, unsigned len);

BUPnPServiceHandle BUPnPService_Create(BUPnPServiceInfo* pServiceInfo)
{
    BUPnPStateVariableInfo **ppVarInfo;
    BUPnPActionInfo **ppActionInfo;
    BUPnPActionHandle hAction;
    BUPnPStateVariableHandle hStateVariable;
    BUPnPServiceHandle hService;
    bool bCreateModerateTimer = false;
    TimerSettings timerSettings;

    hService = (BUPnPServiceHandle)BUPnPCalloc(1, sizeof(struct BUPnPService));
    if ( hService == NULL )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Description, ("Out of memory:Create service"));
        return NULL;
    }

    /* Create the actions */
    if (pServiceInfo->actions != NULL)
    {
        for (ppActionInfo = pServiceInfo->actions; *ppActionInfo; ppActionInfo++)
        {
            hAction = BUPnPAction_Create(*ppActionInfo);
            if (hAction == NULL)
                return NULL;
            hAction->service = hService;
            BLST_INSERT_TAIL(&hService->actions, hAction, link);
        }
    }

    /* Create the state variables */
    if (pServiceInfo->variables != NULL)
    {
        for (ppVarInfo = pServiceInfo->variables; *ppVarInfo; ppVarInfo++)
        {
            hStateVariable = BUPnPStateVariable_Create(*ppVarInfo);
            if (hStateVariable == NULL)
                return NULL;
            hStateVariable->service = hService;
            BLST_INSERT_TAIL(&hService->stateVariables, hStateVariable, link);

            if ((hStateVariable->variableInfo->attributes & BUPnPAttribute_Moderated) == BUPnPAttribute_Moderated)
                bCreateModerateTimer = true;

        }
    }

    if (bCreateModerateTimer)
    {
        timerSettings.period = UPNP_DEFAULT_MODERATE_TIME;
        timerSettings.callback = BUPnPService_UpdateEvent;
        timerSettings.param = (void*)hService;
        timerSettings.executeOnce = false;
        hService->moderateTimer = Timer_Create(&timerSettings);
    }

    hService->serviceInfo = pServiceInfo;

    return hService;
}

void BUPnPService_Destroy(BUPnPServiceHandle hService)
{
    BUPnPActionHandle hAction;
    BUPnPStateVariableHandle hStateVariable;
    BUPnPSubscriptionHandle hSubscription;

    if (hService->moderateTimer)
        Timer_Destroy(hService->moderateTimer);

    while( (hAction = BLST_FIRST(&hService->actions)) != NULL )
    {
        BLST_REMOVE_HEAD(&hService->actions, link);
        BUPnPAction_Destroy(hAction);
    }

    while( (hStateVariable = BLST_FIRST(&hService->stateVariables)) != NULL )
    {
        BLST_REMOVE_HEAD(&hService->stateVariables, link);
        BUPnPStateVariable_Destroy(hStateVariable);
    }

    while( (hSubscription = BLST_FIRST(&hService->subscriptionList)) != NULL )
    {
        BLST_REMOVE_HEAD(&hService->subscriptionList, link);
        BUPnPSubscription_Destroy(hSubscription);
    }

    if (hService->flag & BUPNP_SERVICE_REQUIRESDESTROY)
    {
        if ( hService->subscriptionId != NULL)
            Gena_RemoveSubscriptionByService(hService);

        BUPnPFree((void*)(hService->serviceInfo->serviceId));
        BUPnPFree((void*)(hService->serviceInfo->scpdUrl));
        BUPnPFree((void*)(hService->serviceInfo->controlUrl));
        BUPnPFree((void*)(hService->serviceInfo->eventSubUrl));
    }

    BUPnPFree(hService);
    hService = NULL;
}


BUPnPSubscriptionHandle BUPnPService_AddSubscription(BUPnPServiceHandle hService, int32_t timeout)
{
    BUPnPSubscriptionHandle hSubscription;

    hSubscription = BUPnPSubscription_Create(NULL, timeout, NULL);
    if ( hSubscription != NULL )
        BLST_INSERT_HEAD(&hService->subscriptionList, hSubscription, link);

    /* Save the service pointer for initial eventing */
    hSubscription->service = hService;

    return hSubscription;
}

void BUPnPService_RemoveSubscription(BUPnPServiceHandle hService, BUPnPSubscriptionHandle hSubscription) 
{
    BLST_REMOVE(&hService->subscriptionList, hSubscription, BUPnPSubscription, link);
    BUPnPSubscription_Destroy(hSubscription);
}

BUPnPSubscriptionHandle BUPnPService_GetSubscription(BUPnPServiceHandle hService, const char *sid)
{
    BUPnPSubscriptionHandle hSubscription;

    BLST_DICT_FIND_BY_STR(&hService->subscriptionList, hSubscription, sid, sid, link);

    return hSubscription;
}

BUPnPServiceHandle BUPnPService_GetFromUrl(const char *url)
{
    BUPnPDeviceHandle hDevice;
    BUPnPServiceHandle hService;

    for ( hDevice = BUPnPDevice_Iterator(NULL); hDevice; hDevice = BUPnPDevice_Iterator(hDevice) )
    {
        for ( hService = BLST_FIRST(&hDevice->serviceList); hService; hService = BLST_NEXT(hService, link) )
        {
            if ( IMATCH(hService->serviceInfo->controlUrl, url) )
                return hService;
            if ( IMATCH(hService->serviceInfo->eventSubUrl, url) )
                return hService;
            if ( IMATCH(hService->serviceInfo->scpdUrl, url) )
                return hService;
        }
    }
    return NULL;
}

BUPnPActionHandle BUPnPService_GetAction(BUPnPServiceHandle hService, const char* name)
{
    BUPnPActionHandle hAction;

    BLST_DICT_FIND_BY_STR(&hService->actions, hAction, name, name, link);

    return hAction;
}

BUPnPStateVariableHandle BUPnPService_GetStateVariable(BUPnPServiceHandle hService, const char* name)
{
    BUPnPStateVariableHandle hStateVariable;

    BLST_DICT_FIND_BY_STR(&hService->stateVariables, hStateVariable, name, name, link);

    return hStateVariable;
}

void BUPnPService_Notify(BUPnPServiceHandle hService)
{
    BUPnPSubscriptionHandle hSubscription;
    BUPnPSubscriptionHandle hSubscriptionNext;
    cstring_t xml;
    bool bFoundModified = false;

    CString_Create(&xml);

    BUPnPService_BuildSubscriptionPropertySet(hService, false, &xml, &bFoundModified);

    if (bFoundModified)
    {
        hSubscription = BLST_FIRST(&hService->subscriptionList);
        while (hSubscription)
        {
            hSubscriptionNext = BLST_NEXT(hSubscription, link);

            /* check that subscription has not expired */
            if (hSubscription->expires < time(NULL))
            {
                BUPnPService_RemoveSubscription(hService, hSubscription);
            }
            else
            {
                BUPnPSubscription_Notify(hSubscription, xml.bytes);
            }
            hSubscription = hSubscriptionNext;
        }
        hService->lastNotifyTime = GetTickCount();
    }

    CString_Destroy(&xml);
}

BUPnPError BUPnPService_GetDescriptionXml(BUPnPServiceHandle hService, cstring_t* xml)
{
    BUPnPStateVariableInfo *pVariable, **ppVariable;
    BUPnPActionInfo *pAction, **ppAction;
    BUPnPArgumentInfo *pArgument, **ppArgument;

    CString_Append(xml, 
            "<?xml version=\"1.0\"?>\r\n"
            "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\r\n"
            "<specVersion>\r\n"
            "<major>1</major>\r\n"
            "<minor>0</minor>\r\n"
            "</specVersion>\r\n");

    CString_Append(xml, "<actionList>\r\n");
    for (ppAction = hService->serviceInfo->actions; *ppAction; ppAction++)
    {
        pAction = *ppAction;

        CString_Append(xml, "<action>\r\n");
        CString_Append(xml, "<name>%s</name>\r\n",pAction->name);

        /* don't print any <argumentList> if there are no args. */
        if (pAction->arguments != NULL)
        {
            CString_Append(xml, "<argumentList>\r\n");
            for (ppArgument = pAction->arguments; *ppArgument; ppArgument++)
            {
                pArgument = *ppArgument;

                CString_Append(xml, "<argument>\r\n");
                CString_Append(xml, "<name>%s</name>\r\n", pArgument->name);
                CString_Append(xml, "<direction>%s</direction>\r\n", (UPNP_GET_ATTRIBUTE(pArgument, BUPnPAttribute_Out) ? "out" : "in"));
                if ( UPNP_GET_ATTRIBUTE(pArgument, BUPnPAttribute_RetVal) )
                    CString_Append(xml, "<retval />");
                CString_Append(xml, "<relatedStateVariable>%s</relatedStateVariable>\r\n",    pArgument->relatedStateVariable->name);
                CString_Append(xml, "</argument>\r\n");
            }
            CString_Append(xml, "</argumentList>\r\n");
        }
        CString_Append(xml, "</action>\r\n");
    }
    CString_Append(xml, "</actionList>\r\n");

    CString_Append(xml, "<serviceStateTable>\r\n");

    for (ppVariable = hService->serviceInfo->variables; *ppVariable; ppVariable++)
    {
        pVariable = *ppVariable;

        CString_Append(xml, "<stateVariable sendEvents=\"%s\">\r\n",(UPNP_GET_ATTRIBUTE(pVariable, BUPnPAttribute_Evented) ? "yes" : "no"));
        CString_Append(xml, "<name>%s</name>\r\n", pVariable->name);
        CString_Append(xml, "<dataType>%s</dataType>\r\n", BUPnPService_TypeToString(pVariable->dataType));

        if (UPNP_GET_ATTRIBUTE(pVariable, BUPnPAttribute_List) && pVariable->allowedValues.list )
        {
            char **avl;
            CString_Append(xml, "<allowedValueList>\r\n");
            for (avl = (char**)pVariable->allowedValues.list; *avl; avl++)
                CString_Append(xml, "<allowedValue>%s</allowedValue>\r\n", *avl);
            CString_Append(xml, "</allowedValueList>\r\n");
        } 
        else if (UPNP_GET_ATTRIBUTE(pVariable, BUPnPAttribute_Range) && pVariable->allowedValues.range )
        {
            BUPnPAllowedValueRange* avr =  pVariable->allowedValues.range;
            CString_Append(xml, "<allowedValueRange>\r\n");
            CString_Append(xml, "<minimum>%s</minimum>\r\n", avr->minimum);
            CString_Append(xml, "<maximum>%s</maximum>\r\n", avr->maximum);
            CString_Append(xml, "<step>%s</step>\r\n", avr->step);
            CString_Append(xml, "</allowedValueRange>\r\n");
        }

        if (pVariable->defaultValue != NULL)
        {
            if (MATCH(pVariable->defaultValue, "") == false)
                CString_Append(xml, "<defaultValue>%s</defaultValue>\r\n", pVariable->defaultValue);
        }

        CString_Append(xml, "</stateVariable>\r\n");
    }
    CString_Append(xml, "</serviceStateTable>\r\n");

    CString_Append(xml, "</scpd>\r\n");

    return UPNP_E_SUCCESS;
}

void BUPnPService_BuildSubscriptionPropertySet(BUPnPServiceHandle hService, bool bForceSendAll, cstring_t* pXml, bool* bFoundModified)
{
    const char* pszValue;
    BUPnPStateVariableHandle hStateVariable;

    CString_Append(pXml, "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
            "<e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\">");

    for(hStateVariable=BLST_FIRST(&hService->stateVariables); hStateVariable; hStateVariable=BLST_NEXT(hStateVariable, link)) 
    {
        if ((hStateVariable->variableInfo->attributes & BUPnPAttribute_Evented) == BUPnPAttribute_Evented)
        {
            if ((hStateVariable->variableInfo->attributes & BUPnPAttribute_Moderated) == BUPnPAttribute_Moderated)
            {
                BUPnPService_BuildModeratedEventText(hService, hStateVariable, bForceSendAll);
            }

            if (( hStateVariable->modified ) || (bForceSendAll))
            {
                pszValue = CString_GetText(&hStateVariable->value);

                CString_Append(pXml, "<e:property><%s>%s</%s></e:property>", hStateVariable->name, pszValue, hStateVariable->name);
                if (bFoundModified != NULL)
                    *bFoundModified = true;

                /* Don't clear the modified flag if it is an initial event */
                if (!bForceSendAll)
                    hStateVariable->modified = false;
            }
        }
    }

    CString_Append(pXml, "</e:propertyset>");

}

void BUPnPService_AddAction(BUPnPServiceHandle hService, BUPnPActionHandle hAction)
{
    BLST_INSERT_TAIL(&hService->actions, hAction, link);
}

void BUPnPService_AddVariables(BUPnPServiceHandle hService, BUPnPStateVariableInfo** hStateVariableInfo)
{
    BUPnPStateVariableInfo **ppVarInfo;
    BUPnPStateVariableHandle hStateVariable;

    for (ppVarInfo = hStateVariableInfo; *ppVarInfo; ppVarInfo++)
    {
        hStateVariable = BUPnPStateVariable_Create(*ppVarInfo);
        if (hStateVariable == NULL)
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Out of memory"));
            return;
        }
        hStateVariable->service = hService;
        BLST_INSERT_TAIL(&hService->stateVariables, hStateVariable, link);
    }
}

void BUPnPService_SetModerateNamespace(BUPnPServiceHandle hService, const char* ns)
{
    cmsUtl_strcpy(hService->moderateNamespace, ns);
}

void BUPnPService_SetModerateEventCreateCallback(BUPnPServiceHandle hService, BUPnPCreateModerateEvent moderateEventCallback)
{
    hService->moderateEventCallback = moderateEventCallback;
}

BUPnPStateVariableHandle BUPnPService_GetStateVariableByIndex(BUPnPServiceHandle hService, int index)
{
    BUPnPStateVariableHandle hStateVariable;
    int count = 0;

    for(hStateVariable=BLST_FIRST(&hService->stateVariables); hStateVariable; hStateVariable=BLST_NEXT(hStateVariable, link)) 
    {
        if (count == index)
            return hStateVariable;
        count++;
    }
    return NULL;
}

static BUPnPError BUPnPService_BuildModeratedEventText(BUPnPServiceHandle hService, BUPnPStateVariableHandle hModerateVariable, bool bSentAll)
{
    char* escapedMessage;
    cstring_t msg;
    const char *name, *value;
    BUPnPStateVariableHandle hStateVariable;
    bool bFoundModified = false;
    uint32_t count;

    /* The update rate is controlled here, but 7.3.93 was written incorrectly, LastChange event 
       should be updated every 200 ms */

    if (hService->lastNotifyTime != 0)
    {
        count = GetTickCount();
        if ((count - hService->lastNotifyTime) < GENA_UPDATE_RATE)
            return UPNP_E_SUCCESS;
    }

    if (hService->moderateEventCallback != NULL)
    {
        return hService->moderateEventCallback(hService, hModerateVariable, bSentAll);
    }

    if (CString_Create(&msg) == false)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Out of memory"));
        return UPNP_E_OUT_OF_MEMORY;
    }

    CString_Append(&msg, "<Event xmlns=\"urn:schemas-upnp-org:metadata-1-0/%s/\">", hService->moderateNamespace);
    CString_Append(&msg, "<InstanceID val=\"0\">");
    for(hStateVariable=BLST_FIRST(&hService->stateVariables); hStateVariable; hStateVariable=BLST_NEXT(hStateVariable, link)) 
    {
        if (hStateVariable->variableInfo->attributes & BUPnPAttribute_IndirectEvent)
        {
            if ( BUPnPStateVariable_IsModified(hStateVariable) || (bSentAll) )
            {
                name = BUPnPStateVariable_GetName(hStateVariable);
                value = BUPnPStateVariable_GetValue(hStateVariable);
                CString_Append(&msg, "<%s val=\"%s\"/>\r\n", name, value);
                bFoundModified = true;
                if (!bSentAll)
                {
                    hStateVariable->modified = false;
            }
        }
    }
    }
    CString_Append(&msg, "</InstanceID></Event>");
    escapedMessage = nxml_escape(CString_GetText(&msg));

    /* Don't set it unless we found something */
    if (bFoundModified)
        BUPnPStateVariable_SetString(hModerateVariable, escapedMessage);
    else
        hModerateVariable->modified = false;

    BUPnPFree(escapedMessage);
    CString_Destroy(&msg);
    return UPNP_E_SUCCESS;
}

void BUPnPService_SuspendUpdate(BUPnPServiceHandle hService)
{
    hService->disableUpdate = true;
}

void BUPnPService_ResumeUpdate(BUPnPServiceHandle hService)
{
    hService->disableUpdate = false;
    /* BUPnPService_Notify(hService); */
}

static void BUPnPService_UpdateEvent(TimerHandle hTimer, void *arg)
{
    BUPnPServiceHandle hService;
    UNUSED_ARGUMENT(hTimer);

    hService = (BUPnPServiceHandle)arg;
    BUPnPService_Notify(hService);
}

static const char* BUPnPService_TypeToString(unsigned type)
{
    switch (type)
    {
        case BUPnPType_UI1:         return "ui1";
        case BUPnPType_UI2:         return "ui2";
        case BUPnPType_UI4:         return "ui4";
        case BUPnPType_I1:         return "i1";
        case BUPnPType_I2:         return "i2";
        case BUPnPType_I4:         return "i4";
        case BUPnPType_Int:         return "int";
        case BUPnPType_R4:         return "r4";
        case BUPnPType_R8:         return "r8";
        case BUPnPType_Number:     return "number"; 
        case BUPnPType_Fixed_14_4:return "fixed.14.4";
        case BUPnPType_Float:     return "float";
        case BUPnPType_Char:         return "char";
        case BUPnPType_String:     return "string";
        case BUPnPType_Date:         return "date";
        case BUPnPType_DateTime:  return "dateTime";
        case BUPnPType_DateTimeTz:return "dateTime.tz";
        case BUPnPType_Time:         return "time";
        case BUPnPType_TimeTz:     return "time.tz";
        case BUPnPType_Boolean:     return "boolean";
        case BUPnPType_BinBase64: return "bin.base64";
        case BUPnPType_BinHex:     return "bin.hex";
        case BUPnPType_Uri:         return "uri";
        case BUPnPType_Uuid:         return "uuid";
    }

    return NULL;
}

void* BUPnPService_GetContext(BUPnPServiceHandle hService)
{
    return hService->parent->context;
}

BUPnPError BUPnPService_ParseDescriptipn(BUPnPServiceHandle hService, const char* msg)
{
    int retCode, clientId = 0;
    nxml_t handle;
    nxml_settings settings;
    BUPnPServiceParser serviceParser;

    UPNP_DEBUG_TRACE( BUPnPDebugCategory_Description, ("ParseDescription Enter"));

    memset(&settings, 0, sizeof(settings));
    settings.tag_begin = BUPnPService_TagBegin;
    settings.tag_end = BUPnPService_TagEnd;
    settings.data = BUPnPService_Data;

    memset(&serviceParser, 0, sizeof(serviceParser));
    serviceParser.hService = hService;

    nxml_open(clientId, &handle,&settings);
    handle->context = (void*)&serviceParser;

    nxml_write(handle, msg, cmsUtl_strlen(msg));
    retCode = handle->error;

    nxml_close(handle);

    UPNP_DEBUG_TRACE( BUPnPDebugCategory_Description, ("ParseDescription Complete"));
    return retCode;
}

void BUPnPService_Data(nxml_t handle, const char *tag_name, unsigned len)
{
    struct BUPnPArgumentHolder* pArgument;
    BUPnPServiceParser* parserSettings = (BUPnPServiceParser*)handle->context;

    if (parserSettings->state == PARSING_VARIABLES)
    {
        if (IMATCH_PREFIX(handle->lastStartTag, "name"))
        {
            parserSettings->pVariableInfo = (BUPnPStateVariableInfo*)BUPnPCalloc(1, sizeof(BUPnPStateVariableInfo));
            parserSettings->pVariableInfo->name = BUPnPStrndup(tag_name, len);
        }
        else if (IMATCH_PREFIX(handle->lastStartTag, "dataType"))
        {
        }
        else if (IMATCH_PREFIX(handle->lastStartTag, "allowedValue"))
        {

        }
    }
    else if (parserSettings->state == PARSING_ACTIONS)
    {
        if (IMATCH_PREFIX(handle->lastStartTag, "name"))
        {
            parserSettings->pActionInfo = (BUPnPActionInfo*)BUPnPCalloc(1, sizeof(BUPnPActionInfo));
            parserSettings->pActionInfo->name = BUPnPStrndup(tag_name, len);
        }
    }
    else if (parserSettings->state == PARSING_ARGUMENTS)
    {
        if (IMATCH_PREFIX(handle->lastStartTag, "name"))
        {
            pArgument = (struct BUPnPArgumentHolder*)BUPnPCalloc(1, sizeof(struct BUPnPArgumentHolder));
            pArgument->info = (struct BUPnPArgumentInfo*)BUPnPCalloc(1, sizeof(struct BUPnPArgumentInfo));
            pArgument->info->name = BUPnPStrndup(tag_name, len);
            BLST_INSERT_TAIL(&parserSettings->argumentList, pArgument, link);
        }
        else if (IMATCH_PREFIX(handle->lastStartTag, "direction"))
        {
            pArgument = BLST_LAST(&parserSettings->argumentList);
            if (IMATCH_PREFIX(tag_name, "out"))
                pArgument->info->attributes = BUPnPAttribute_Out;
            else
                pArgument->info->attributes = BUPnPAttribute_In;
        }
    }
}

void BUPnPService_TagBegin(nxml_t handle, const char *tag_name, unsigned len)
{
    BUPnPServiceParser* parserSettings = (BUPnPServiceParser*)handle->context;
    UNUSED_ARGUMENT(len);

    if (IMATCH_PREFIX(tag_name, "serviceStateTable"))
        parserSettings->state = PARSING_VARIABLES;
    else if (IMATCH_PREFIX(tag_name, "action"))
        parserSettings->state = PARSING_ACTIONS;
    else if (IMATCH_PREFIX(tag_name, "argument"))
        parserSettings->state = PARSING_ARGUMENTS;
//    else if (IMATCH_PREFIX(tag_name, "allowedValueList"))
//        parserSettings->state = PARSING_ALLOWED_VALUE;
}

void BUPnPService_TagEnd(nxml_t handle, const char *tag_name, unsigned len)
{
    BUPnPStateVariableHandle hStateVariable;
    BUPnPActionHandle hAction;
    struct BUPnPArgumentHolder* pArgument, *pArgumentPrev;
    BUPnPServiceParser* parserSettings = (BUPnPServiceParser*)handle->context;
    size_t i,argumentCount;
    UNUSED_ARGUMENT(len);

    if (IMATCH_PREFIX(tag_name, "stateVariable"))
    {
        hStateVariable = BUPnPStateVariable_Create(parserSettings->pVariableInfo);
        if (hStateVariable == NULL)
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Description, ("Out of memory:parsing service"));
            handle->error = UPNP_E_OUT_OF_MEMORY;
        }
        else
        {
            hStateVariable->service = parserSettings->hService;
            BLST_INSERT_TAIL(&parserSettings->hService->stateVariables, hStateVariable, link);
        }
    }
    else if (IMATCH_PREFIX(tag_name, "argumentList"))
    {
        hAction = BUPnPAction_Create(parserSettings->pActionInfo);

        if (hAction == NULL)
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Description, ("Out of memory:parsing service"));
            handle->error = UPNP_E_OUT_OF_MEMORY;
        }
        hAction->service = parserSettings->hService;
        BLST_INSERT_TAIL(&parserSettings->hService->actions, hAction, link);

        argumentCount = 0;
        pArgument = BLST_FIRST(&parserSettings->argumentList);
        while (pArgument)
        {
            /* in arguments always come before out */
            if (pArgument->info->attributes & BUPnPAttribute_In)
                hAction->inArgumentCount++;
            argumentCount++;
            pArgument = BLST_NEXT(pArgument, link);
        }

        hAction->arguments = (struct BUPnPArgument*)BUPnPCalloc(argumentCount, sizeof(struct BUPnPArgument));
        if ( hAction->arguments == NULL )
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Description, ("Out of memory:parsing service"));
            handle->error = UPNP_E_OUT_OF_MEMORY;
        }
        hAction->argumentCount = argumentCount;

        i=0;
        pArgument = BLST_FIRST(&parserSettings->argumentList);
        while (pArgument)
        {
            hAction->arguments[i].name = pArgument->info->name;
            hAction->arguments[i].argumentInfo = pArgument->info;
            if ( !CString_Create(&hAction->arguments[i].value) )
            {
                UPNP_DEBUG_ERROR(BUPnPDebugCategory_Description, ("Out of memory:parsing service"));
                handle->error = UPNP_E_OUT_OF_MEMORY;
            }
            i++;
            pArgumentPrev = pArgument;
            pArgument = BLST_NEXT(pArgument, link);
            BUPnPFree(pArgumentPrev);
        }
        BLST_INIT(&parserSettings->argumentList);
        

    }
}

