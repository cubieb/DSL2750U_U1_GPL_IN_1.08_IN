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
 * $brcm_Workfile: service.h $
 * $brcm_Revision: 2 $
 * $brcm_Date: 9/30/09 4:16p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/service.h $
 * 
 * 2   9/30/09 4:16p dliu
 * SW7405-2482: Add support for dynamic service creation
* 
***************************************************************************/
#ifndef UPNPSERVICE_H__
#define UPNPSERVICE_H__

#include "upnp_priv.h"
#include "cstring.h"
#include "list.h"
#include "action.h"
#include "variable.h"
#include "subscription.h"

#ifdef __cplusplus
extern "C" {
#endif

struct BUPnPService
{
    BUPnPDeviceHandle parent;
    
    BLST_HEAD(BUPnPActionList, BUPnPAction) actions;
    
    BLST_HEAD(BUPnPStateVariableList, BUPnPStateVariable) stateVariables;

    struct BUPnPSubscriptionList subscriptionList;

    const char *subscriptionId; /* Subscriptions have its own list, but store the ID so the service can find it */
    
    uint32_t flag;
    
    uint32_t version;
    
    BUPnPServiceInfo* serviceInfo;

    TimerHandle moderateTimer;

    char moderateNamespace[4]; /* This is the three letter abbreviation used in moderate event */

    uint32_t lastNotifyTime;

    BUPnPCreateModerateEvent moderateEventCallback;

    void* context;

    bool disableUpdate;

    BLST_ENTRY(BUPnPService) link;

};

BUPnPServiceHandle BUPnPService_Create(BUPnPServiceInfo* pServiceInfo);

void BUPnPService_Destroy(BUPnPServiceHandle hService);

BUPnPSubscriptionHandle BUPnPService_AddSubscription(BUPnPServiceHandle hService, int32_t timeout);

void BUPnPService_RemoveSubscription(BUPnPServiceHandle hService, BUPnPSubscriptionHandle hSubscription); 

BUPnPSubscriptionHandle BUPnPService_GetSubscription(BUPnPServiceHandle hService, const char *sid);

BUPnPError BUPnPService_GetDescriptionXml(BUPnPServiceHandle hService, cstring_t* xml);

void BUPnPService_Notify(BUPnPServiceHandle hService);

void BUPnPService_BuildSubscriptionPropertySet(BUPnPServiceHandle hService, bool bForceSendAll, cstring_t* pXml, bool* bFoundModified);

BUPnPError BUPnPService_ParseDescriptipn(BUPnPServiceHandle hService, const char* msg);


#ifdef __cplusplus
}
#endif

#endif /* UPNPSERVICE_H__ */
