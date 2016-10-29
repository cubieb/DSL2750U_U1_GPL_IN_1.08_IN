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
***************************************************************************/
#include <time.h>
#include "upnp_priv.h"
#include "list.h"
#include "device.h"
#include "gena.h"
#include "subscription.h"

BUPnPSubscriptionHandle BUPnPSubscription_Create(const char* callbackUrl, int32_t timeout, const char* sid)
{
    BUPnPSubscriptionHandle hSubscription = (BUPnPSubscriptionHandle)BUPnPCalloc(1, sizeof(BUPnPSubscription));
    if ( hSubscription == NULL )
        return NULL;

    memset(hSubscription, 0, sizeof(BUPnPSubscription));

    /* Generate SID */
    if (sid == NULL)
    {
        cmsUtl_strcpy(hSubscription->sid, "uuid:");
        uuid_create((char*)&hSubscription->sid[5], UPNP_MAX_SID_SIZE);
    }
    else
    {
        cmsUtl_strcpy(hSubscription->sid, sid);
    }

    hSubscription->expires = time(NULL) + timeout;
    hSubscription->renewTimeSeconds = timeout;

    if ( callbackUrl != NULL )
        BUPnPSubscription_AddCallbackUrl(hSubscription, callbackUrl);

    return hSubscription;
}

void BUPnPSubscription_Destroy(BUPnPSubscriptionHandle hSubscription) 
{
    BUPnPSubscriptionCallback* pCallback;

    while( (pCallback = BLST_FIRST(&hSubscription->callbackList)) != NULL )
    {
        BLST_REMOVE_HEAD(&hSubscription->callbackList, link);
        Url_Destroy(&pCallback->url);
        BUPnPFree(pCallback);
    }

    if (hSubscription->url.path)
        BUPnPFree(hSubscription->url.path);
    BUPnPFree(hSubscription);
}

BUPnPError BUPnPSubscription_AddCallbackUrl(BUPnPSubscriptionHandle hSubscription, const char* url) 
{
    BUPnPSubscriptionCallback* pCallback;
    UNUSED_ARGUMENT(hSubscription);

    pCallback = (BUPnPSubscriptionCallback*)BUPnPCalloc(1, sizeof(BUPnPSubscriptionCallback));
    if ( pCallback == NULL )
        return UPNP_E_OUT_OF_MEMORY;

    memset((char*) pCallback, 0, sizeof(BUPnPSubscriptionCallback));

    Url_Create(&pCallback->url, url);

    BLST_INSERT_HEAD(&hSubscription->callbackList, pCallback, link);

    return UPNP_E_SUCCESS;
}

const char* BUPnPSubscription_GetSid(BUPnPSubscriptionHandle hSubscription) 
{
    return hSubscription->sid;
}

void BUPnPSubscription_SetTimeout(BUPnPSubscriptionHandle hSubscription, time_t timeout) 
{
    hSubscription->expires = time(NULL) + timeout;
}

BUPnPError BUPnPSubscription_Notify(BUPnPSubscriptionHandle hSubscription, const char* propertySet)
{
    BUPnPSubscriptionCallback* pCallback;
    BUPnPError result = UPNP_E_SUCCESS;

    /* Try each url and exit once a successful NOTIFY is completed */
    for ( pCallback = BLST_FIRST(&hSubscription->callbackList); pCallback; pCallback = BLST_NEXT(pCallback, link) )
    {
        result = Gena_SendNotify(pCallback->url.host, pCallback->url.port, pCallback->url.path, hSubscription->sid, hSubscription->key, propertySet);
        if ( result == UPNP_E_SUCCESS )
        {
            if (++hSubscription->key == 0)
                hSubscription->key = 1;
            break;
        }
    }

    return result;
}
