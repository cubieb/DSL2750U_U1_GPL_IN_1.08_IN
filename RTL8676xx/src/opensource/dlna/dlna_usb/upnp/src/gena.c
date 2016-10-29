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
 * $brcm_Workfile: gena.c $
 * $brcm_Revision: 7 $
 * $brcm_Date: 9/30/09 4:18p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/gena.c $
 * 
 * 7   9/30/09 4:18p dliu
 * SW7405-2482: Fix issue where subscribe pass in the entire URL path
 * 
 * 6   9/23/09 2:52p dliu
 * SW7405-2482: Need to close socket when sending gena event
 * 
 * 5   9/15/09 6:12p dliu
 * SW7405-2482: Add support for lastChange event
 * 
 * 4   8/24/09 5:50p dliu
 * SW7405-2482: Add more parameter checking
* 
***************************************************************************/
#include "upnp_priv.h"
#include "bsocket.h"
#include "debug.h"
#include "device.h"
#include "service.h"
#include "subscription.h"
#include "gena.h"
#include "nanoxml.h"
#include "list.h"
#include "controlpoint.h"

#define GENA_TIMEOUT 30
#define INITIAL_EVENT_DELAY 1000

static int32_t Gena_ParseCallbackUrls( const char* callbacks, BUPnPSubscriptionHandle hSubscription );
static int32_t Gena_ParseTimeout(const char* timeout, char* actualTimeout);
static bool Gena_ProcessNotifyResponse(BUPnPConnectionHandle hConnection, void *args);
static void Gena_SetVariable(struct BUPnPService* pService, const char* name, const char* value, unsigned len);
static void Gena_RenewSubscription(TimerHandle t, void *arg);
static void Gena_XmlData(nxml_t handle, const char *tag_name, unsigned len);
static void Gena_AddSubscription(BUPnPSubscription* pSubscription);
static void Gena_RemoveSubscription(const char* sid);
static BUPnPSubscription* Gena_GetSubscription(const char* sid);
static void Gena_SendInitialEvent(TimerHandle timer, void* arg);

/****************************************************************************** 
Section: 4.1.1 Eventing: Subscribing: SUBSCRIBE with NT and CALLBACK
*******************************************************************************/
BUPnPError Gena_ProcessSubscribe( HttpContextHandle hContext )
{
    BUPnPServiceHandle hService;
    BUPnPSubscriptionHandle hSubscription;
    int32_t timeoutVal, result;
    TimerSettings timerSettings;
    char headers[UPNP_MAX_PACKET_SIZE], date[UPNP_MAX_DATE_SIZE], actualTimeout[20];
    const char *uri, *callback = NULL, *sid = NULL, *timeout = NULL, *nt = NULL;

    if (hContext == NULL)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Invalid HttpContextHandle passed as parameter"));
        return UPNP_E_ARGUMENT_VALUE_INVALID;
    }

    uri = Http_GetRequestUri(hContext);

    if (IMATCH_PREFIX(uri, "http://"))
    {
        uri = strrchr(uri, '/');
    }

    hService = BUPnPService_GetFromUrl(uri);
    if (hService == NULL)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Service Unavailable"));
        return Http_SendResponse(hContext, HTTP_SERVICE_UNAVAILABLE, NULL, NULL);
    }

    callback = Http_GetHeaderValue(hContext, "CALLBACK");
         sid = Http_GetHeaderValue(hContext, "SID");
     timeout = Http_GetHeaderValue(hContext, "TIMEOUT");
          nt = Http_GetHeaderValue(hContext, "NT");

    if ( sid && (nt || callback) )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Subscribe - Incompatible Headers"));
        return Http_SendResponse(hContext, HTTP_BAD_REQUEST, NULL, NULL);
    }

    if ( !sid ) /* New subscription */
    {
        if( !IMATCH(nt, "upnp:event" ) )
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Subscribe - Invalid NT"));
            return Http_SendResponse(hContext, HTTP_PRECONDITION_FAILED, NULL, NULL);
        }

        if ( !callback )
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Subscribe - Missing Callback"));
            return Http_SendResponse(hContext, HTTP_PRECONDITION_FAILED, NULL, NULL);
        }

        timeoutVal = Gena_ParseTimeout(timeout, actualTimeout);
        if ( timeoutVal == -1 )
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Subscribe - Invalid Timeout"));
            return Http_SendResponse(hContext, HTTP_INTERNAL_SERVER_ERROR, NULL, NULL);
        }

        hSubscription = BUPnPService_AddSubscription(hService, timeoutVal);
        UPNP_DEBUG_TRACE(BUPnPDebugCategory_Eventing, ("Received new subscription\n: callback = %s\n sid = %s\n timeout = %d", 
            callback, BUPnPSubscription_GetSid(hSubscription),  timeoutVal));
        if ( !hSubscription )
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Subscribe - Unable to accept subscription"));
            return Http_SendResponse(hContext, HTTP_INTERNAL_SERVER_ERROR, NULL, NULL);
        }

        result = Gena_ParseCallbackUrls(callback, hSubscription);
        if ( result != UPNP_E_SUCCESS )
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Subscribe - Invalid Callback"));
            BUPnPService_RemoveSubscription(hService, hSubscription);
            return Http_SendResponse(hContext, HTTP_PRECONDITION_FAILED, NULL, NULL);
        }

        /* Prepare to send initial event. Cannot be sent before a response to Subscribe is sent (Section 4.1) */
        timerSettings.period = INITIAL_EVENT_DELAY;
        timerSettings.callback = Gena_SendInitialEvent;
        timerSettings.param = (void*)hSubscription;
        timerSettings.executeOnce = true;
        hSubscription->timer= Timer_Create(&timerSettings);
    }
    else /* Renew subscription */
    {
        hSubscription = BUPnPService_GetSubscription(hService, sid);
        UPNP_DEBUG_TRACE(BUPnPDebugCategory_Eventing, ("Received renewal: sid = %s", sid));
        if ( hSubscription == NULL )
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Subscribe - Invalid or Missing SID %s", sid));
            return Http_SendResponse(hContext, HTTP_PRECONDITION_FAILED, NULL, NULL);
        }

        /* Make sure it didn't expire already */
        if (hSubscription->expires < time(NULL))
        {
            BUPnPService_RemoveSubscription(hService, hSubscription);
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Subscribe - renewing a expired subscription"));
            return Http_SendResponse(hContext, HTTP_PRECONDITION_FAILED, NULL, NULL);
        }

        BUPnPSubscription_SetTimeout(hSubscription, Gena_ParseTimeout(timeout, actualTimeout));
    }

    snprintf(headers, sizeof(headers)-1, 
                "DATE: %s\r\n"
                "SERVER: %s\r\n"
                "SID: %s\r\n"
                "CONTENT-LENGTH: 0\r\n" /* no content */
                "TIMEOUT: %s\r\n\r\n",
                    BUPnP_GetRfc1123Date(date, sizeof(date)), 
                    BUPnP_GetDescription(),
                    BUPnPSubscription_GetSid(hSubscription), 
                    actualTimeout); 
    return Http_SendResponse(hContext, HTTP_OK, headers, NULL);
}

/****************************************************************************** 
Section: 4.1.3 Eventing: Canceling a subscription: UNSUBSCRIBE
*******************************************************************************/
BUPnPError Gena_ProcessUnsubscribe( HttpContextHandle hContext )
{
    BUPnPServiceHandle hService;
    BUPnPSubscriptionHandle hSubscription;
    const char *uri, *callback, *nt, *sid;

    if (hContext == NULL)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Invalid HttpContextHandle passed as parameter"));
        return UPNP_E_ARGUMENT_VALUE_INVALID;
    }

    uri = Http_GetRequestUri(hContext);

    hService = BUPnPService_GetFromUrl(uri);
    if (hService == NULL)
        return Http_SendResponse(hContext, HTTP_SERVICE_UNAVAILABLE, NULL, NULL);

    callback = Http_GetHeaderValue(hContext, "CALLBACK");
          nt = Http_GetHeaderValue(hContext, "NT");
         sid = Http_GetHeaderValue(hContext, "SID");

    if ( sid && (nt || callback) )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Unsubscribe - Incompatible headers"));
        return Http_SendResponse(hContext, HTTP_BAD_REQUEST, NULL, NULL);
    }

    hSubscription = BUPnPService_GetSubscription(hService, sid);
    if ( !hSubscription )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Unsubscribe - Invalid or Missing SID"));
        return Http_SendResponse(hContext, HTTP_PRECONDITION_FAILED, NULL, NULL);
    }

    /* Remove initial timer in case this functions gets called before initial event is sent */
    if (hSubscription->timer != NULL)
        Timer_Destroy(hSubscription->timer);

    BUPnPService_RemoveSubscription(hService, hSubscription);

    return Http_SendResponse(hContext, HTTP_OK, NULL, NULL);
}

/****************************************************************************** 
Section: 4.2.1 Eventing: Event messages: NOTIFY
*******************************************************************************/
BUPnPError Gena_ProcessNotify( HttpContextHandle hContext )
{
    int result, clientId = 0;
    nxml_t handle;
    nxml_settings settings;
    struct BUPnPService* pService;
    BUPnPSubscription* pSubscription = NULL;
    const char *msg = NULL, *nts = NULL, *sid = NULL, *seq = NULL, *nt = NULL;

    if (hContext == NULL)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Invalid HttpContextHandle passed as parameter"));
        return UPNP_E_ARGUMENT_VALUE_INVALID;
    }

    memset(&settings, 0, sizeof(settings));

     nt = Http_GetHeaderValue(hContext, "NT");
    nts = Http_GetHeaderValue(hContext, "NTS");
    sid = Http_GetHeaderValue(hContext, "SID");
    seq = Http_GetHeaderValue(hContext, "SEQ");

    if (!nt || !nts)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("ProcessNotify - Missing NT or NTS header"));
        return Http_SendResponse(hContext, HTTP_BAD_REQUEST, NULL, NULL);
    }

    if (!sid || !seq || !MATCH_PREFIX(nt, "upnp:event") || !MATCH_PREFIX(nts, "upnp:propchange"))
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("ProcessNotify - Precondition error"));
        return Http_SendResponse(hContext, HTTP_PRECONDITION_FAILED, NULL, NULL);
    }

    /* If we got a SID we don't know about, we are suppose to flag it */
    pSubscription = Gena_GetSubscription(sid);
    if (!pSubscription)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Received a notify that we didn't subscibe to. SID=%s", sid));
        return Http_SendResponse(hContext, HTTP_PRECONDITION_FAILED, NULL, NULL);
    }

    pService = pSubscription->service;
    assert(pService);

    UPNP_DEBUG_TRACE(BUPnPDebugCategory_Eventing, ("Received notify: %s\n%s", sid, Http_GetBody(hContext)));

    /* If we don't care about this property then we also ignore it */
    if (BLST_EMPTY(&pService->stateVariables))
        return Http_SendResponse(hContext, HTTP_OK, NULL, NULL);

    result = Http_SendResponse(hContext, HTTP_OK, NULL, NULL);
    if ( result != UPNP_E_SUCCESS)
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Send GENA response failed 0x%x\n", result));

    /* Feed content to nano */
    msg = Http_GetBody(hContext);
    settings.data = Gena_XmlData;
    nxml_open(clientId, &handle,&settings);
    handle->context = (void*)pService;
    nxml_write(handle, msg, cmsUtl_strlen(msg));
    nxml_close(handle);

    return UPNP_E_SUCCESS;
}

static bool Gena_ProcessNotifyResponse(BUPnPConnectionHandle hConnection, void *args)
{
    HttpContext *pContext;
    int32_t statusCode;
    UNUSED_ARGUMENT(args);

    pContext = &hConnection->context;

    if ( Http_GetResponse(pContext) != UPNP_E_SUCCESS )
        return false;

    statusCode = Http_GetStatusCode(pContext);

    switch (statusCode)
    {
    case HTTP_OK:
        UPNP_DEBUG_TRACE(BUPnPDebugCategory_Eventing, ("Notify - Received HTTP_OK response"));
        break;

    case HTTP_PRECONDITION_FAILED:
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Notify - Missing/Invalid SID or Invalid NT/NTS header"));
        break;

    case HTTP_BAD_REQUEST:
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Notify - Missing NT or NTS header"));
        break;

    default:
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Notify - HTTP Error = %d", statusCode));
        break;
    }

    /* Always return false to destory the socket because we are creating a new socket each time */
    return false;
}

/****************************************************************************** 
Section: 4.2.1 Eventing: Event messages: NOTIFY
*******************************************************************************/
BUPnPError Gena_SendNotify(const char *ipAddress, uint16_t port, 
                          const char *deliveryPath, const char *sid, uint32_t seq, const char* body)
{
    BUPnPError result;
    HttpContext context;
    char headers[UPNP_MAX_PACKET_SIZE];

    if (ipAddress == NULL)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Invalid IP address passed to Gena_SendNotify"));
        return UPNP_E_ARGUMENT_VALUE_INVALID;
    }

    if (port == 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Invalid port passed to Gena_SendNotify"));
        return UPNP_E_ARGUMENT_VALUE_INVALID;
    }

    if (deliveryPath == 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Invalid deliveryPath passed to Gena_SendNotify"));
        return UPNP_E_ARGUMENT_VALUE_INVALID;
    }

    if (body == NULL)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Invalid body passed to Gena_SendNotify"));
        return UPNP_E_ARGUMENT_VALUE_INVALID;
    }

    result = Http_CreateContext(&context);
    if ( result != UPNP_E_SUCCESS )
        return result;

    context.s = BSocket_CreateTcpClient(ipAddress, port);
    if ( context.s <= 0 )
    {
        Http_DestroyContext(&context);
        return UPNP_E_SOCKET;
    }
    
    snprintf(headers, sizeof(headers)-1, 
        "HOST: %s:%d\r\n"
        "CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n"
        "CONTENT-LENGTH: %d\r\n"
        "NT: upnp:event\r\n"
        "NTS: upnp:propchange\r\n"
        "SID: %s\r\n"
        "CONNECTION: close\r\n"
        "SEQ: %d\r\n\r\n", ipAddress, port, cmsUtl_strlen(body), sid, seq);

    result = Http_SendRequest(&context, "NOTIFY", deliveryPath, headers, body); 

    UPNP_DEBUG_TRACE(BUPnPDebugCategory_Eventing, ("Sending Notify to %s: %s", ipAddress, body));

    if ( BUPnP_AddConnection( context.s, GENA_TIMEOUT, Gena_ProcessNotifyResponse, NULL ) == NULL )
        result = UPNP_E_OUT_OF_MEMORY;

    Http_DestroyContext(&context);

    return result;
}

static void Gena_SendInitialEvent(TimerHandle timer, void* arg)
{
    BUPnPSubscriptionHandle hSubscription;
    BUPnPServiceHandle hService;
    cstring_t xml;

    hSubscription = (BUPnPSubscriptionHandle)arg;
    hService = hSubscription->service;

    CString_Create(&xml);

    BUPnPService_BuildSubscriptionPropertySet(hService, true, &xml, NULL);

    BUPnPSubscription_Notify((BUPnPSubscriptionHandle)arg, xml.bytes);

    CString_Destroy(&xml);

    hSubscription->timer = NULL;

    Timer_Destroy(timer);
}

static int32_t Gena_ParseCallbackUrls( const char* callbacks, BUPnPSubscriptionHandle hSubscription )
{
    char *p, *end;

    p = (char*)callbacks;
    end = p + cmsUtl_strlen(callbacks);

    while ( p < end )
    {
        p = strchr(p, '<');
        if ( p == NULL )
            goto error;
        
        end = strchr(p, '>');
        if ( end == NULL )
            goto error;

        *end = '\0';

        BUPnPSubscription_AddCallbackUrl(hSubscription, p+1);

        p = end+1;
    }

    return UPNP_E_SUCCESS;

error:
    return -1;
}

static int32_t Gena_ParseTimeout(const char* timeout, char* actualTimeout)
{
    char *p, *endp;
    int expireTime;

    p = (char*)timeout;

    if (p && IMATCH_PREFIX(p, "second-"))
    {
        p += 7;
        if (IMATCH(p, "infinite"))
            expireTime = 0;
        else
        {
            expireTime = strtoul(p, &endp, 10);
            if (*endp || p == endp)
                expireTime = 1800;
        }
    } 
    else
        expireTime = 1800;

    if ( NULL != actualTimeout )
    {
        /* DLNA specifes max subscribe time of 300 seconds 7.2.22 */
        if ( (expireTime == 0) || (expireTime > 300) )
        {
            sprintf(actualTimeout, "Second-300");
            expireTime = 300;
        }
        else
            sprintf(actualTimeout, "Second-%d", expireTime);
    }

    return expireTime;
}

/* ControlPoint function starts here */
BUPnPError Gena_Subscribe(struct BUPnPDevice *pDevice, struct BUPnPService* pService)
{
    BUPnPError result;
    HttpContext context;
    int32_t statusCode;
    const char *sid, *timeout;
    char headers[UPNP_MAX_PACKET_SIZE], path[UPNP_MAX_URI_SIZE], uri[UPNP_MAX_URI_SIZE];
    BUPnPSubscription* pSubscription;
    uint32_t renewTime;
    TimerSettings timerSettings;

    if (pDevice == NULL)
    {
        UPNP_DEBUG_TRACE(BUPnPDebugCategory_Eventing, ("Invalid device parameter"));
        return UPNP_E_ARGUMENT_VALUE_INVALID;
    }

    if (pService == NULL)
    {
        UPNP_DEBUG_TRACE(BUPnPDebugCategory_Eventing, ("Invalid service parameter"));
        return UPNP_E_SERVICE_NOT_FOUND;
    }

    result = Http_CreateContext(&context);
    if ( result != UPNP_E_SUCCESS )
        return result;

    context.s = BSocket_CreateTcpClient(pDevice->url.host, pDevice->url.port);
    if ( context.s <= 0 )
    {
        Http_DestroyContext(&context);
        return UPNP_E_SOCKET;
    }

    assert(cmsUtl_strlen(pDevice->url.path) + cmsUtl_strlen(pService->serviceInfo->eventSubUrl) < sizeof(uri) - 1);

    cmsUtl_strncpy(uri, pService->serviceInfo->eventSubUrl, sizeof(uri)-1);

    snprintf(path, sizeof(path)-1, "<http://%s:%d/%s/%s>", BUPnP_GetIPAddress(), BUPnP_GetPortNumber(), pDevice->deviceInfo->udn, pService->serviceInfo->serviceId);

    snprintf(headers, sizeof(headers)-1,
        "HOST: %s\r\n"
        "CALLBACK: %s\r\n"
        "NT: upnp:event\r\n"
        "CONNECTION: CLOSE\r\n"
        "TIMEOUT: Second-1800\r\n\r\n", pDevice->url.host, path);


    UPNP_DEBUG_TRACE(BUPnPDebugCategory_Eventing, ("Subscribe to : %s\n%s", uri, headers));

    /* Hold off any incoming events till we have a chance to add it to subscription list */
    /* if we don't then we can easily miss the initial event */
    bthread_mutex_lock(BUPnPControlPoint_GetSubscriptionsMutex());
    if ((result = Http_Invoke(&context, "SUBSCRIBE", uri, headers, NULL)) != UPNP_E_SUCCESS)
    {
        Http_DestroyContext(&context);
        goto error;
    }

    /* Error checking */
    statusCode = Http_GetStatusCode(&context);
    if (statusCode != 200)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Control, ("Subscribe failed with HTTP response: %d", statusCode));
        Http_DestroyContext(&context);
        result = UPNP_E_SUBSCRIBE_FAILED;
        goto error;
    }

    /* Check to see if we got a SID */
    sid = Http_GetHeaderValue(&context, "SID");
    if ( sid == NULL )
    {
        UPNP_DEBUG_ERROR( BUPnPDebugCategory_Eventing, ("Subscribe failed to return valid SID"));
        Http_DestroyContext(&context);
        result = UPNP_E_SUBSCRIBE_FAILED;
        goto error;
    }

    /* get actual timeout value */
    timeout = Http_GetHeaderValue(&context, "TIMEOUT");
    renewTime = Gena_ParseTimeout(timeout, NULL);

    /* Store infor for unsubscribe and renew */
    pSubscription = BUPnPSubscription_Create(NULL, renewTime, (const char*)sid);
    pSubscription->url.host = pDevice->url.host;
    pSubscription->url.port = pDevice->url.port;
    pSubscription->url.path = BUPnPStrdup(uri); 
    pSubscription->service = pService;
    pService->subscriptionId = pSubscription->sid; 

    UPNP_DEBUG_TRACE(BUPnPDebugCategory_Eventing, ("Adding subscription: %s from %s with timeout: %d", pSubscription->sid, pDevice->url.host, pSubscription->renewTimeSeconds));
    Gena_AddSubscription(pSubscription);
    bthread_mutex_unlock(BUPnPControlPoint_GetSubscriptionsMutex());

    /* Make the time smaller to make sure we renew before it expires */
    timerSettings.period = (pSubscription->renewTimeSeconds * 1000)*2/3;
    timerSettings.callback = Gena_RenewSubscription;
    timerSettings.param = (void*)pSubscription;
    timerSettings.executeOnce = false;
    pSubscription->timer = Timer_Create(&timerSettings);

    Http_DestroyContext(&context);
    closesocket(context.s);

    return UPNP_E_SUCCESS;
error:
    bthread_mutex_unlock(BUPnPControlPoint_GetSubscriptionsMutex());
    return result;
}

BUPnPError Gena_Unsubscribe(const char* sid)
{
    HttpContext context;
    BUPnPError result;
    char headers[UPNP_MAX_PACKET_SIZE];
    BUPnPSubscription *pSubscription = NULL;

    pSubscription = Gena_GetSubscription(sid);
    if (NULL == pSubscription)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Unable to find subscription based on sid"));
        return UPNP_E_UNSUBSCRIBE_FAILED; 
    }

    result = Http_CreateContext(&context);
    if ( result != UPNP_E_SUCCESS )
        return result;

    context.s = BSocket_CreateTcpClient(pSubscription->url.host, pSubscription->url.port);
    if ( context.s <= 0 )
    {
        Http_DestroyContext(&context);
        return UPNP_E_SOCKET;
    }

    snprintf(headers, sizeof(headers)-1, 
        "HOST: %s\r\n"
        "SID: %s\r\n\r\n", pSubscription->url.host, sid);

    /* This may fail, but we want to destroy the timer anyways */
    result = Http_Invoke(&context, "UNSUBSCRIBE", pSubscription->url.path, headers, NULL);

    Timer_Destroy(pSubscription->timer);

    Gena_RemoveSubscription(sid);
    BUPnPSubscription_Destroy(pSubscription);

    Http_DestroyContext(&context);

    return result;
}

void Gena_RenewSubscription(TimerHandle t, void *arg)
{
    HttpContext context;
    uint32_t result;
    char headers[UPNP_MAX_PACKET_SIZE];
    BUPnPSubscription* pSubscription;

    pSubscription = (BUPnPSubscription*)arg;
    if (NULL == pSubscription)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Eventing, ("Renew failed because subsciption no longer exists"));
        Timer_Destroy(t);
        return; 
    }

    result = Http_CreateContext(&context);
    if ( result != UPNP_E_SUCCESS )
    {
        Timer_Destroy(t);
        return;
    }

    context.s = BSocket_CreateTcpClient(pSubscription->url.host, pSubscription->url.port);
    if ( context.s <= 0 )
    {
        Timer_Destroy(t);
        Http_DestroyContext(&context);
        return;
    }

    snprintf(headers, sizeof(headers)-1,
        "HOST: %s\r\n"
        "SID: %s\r\n"
        "CONNECTION: close\r\n"
        "TIMEOUT: Second-%d\r\n\r\n", pSubscription->url.host, pSubscription->sid, pSubscription->renewTimeSeconds);

    UPNP_DEBUG_TRACE(BUPnPDebugCategory_Eventing, ("Renew: %s", headers));

    Http_Invoke(&context, "SUBSCRIBE", pSubscription->url.path, headers, NULL);

    Http_DestroyContext(&context);

    /* TODO look into if we can reuse the socket */
    closesocket(context.s);
}

void Gena_XmlData(nxml_t handle, const char *tag_name, unsigned len)
{
    char name[256];
    struct BUPnPService* pService;

    pService = ((struct BUPnPService*)handle->context);
    cmsUtl_strncpy(name, handle->lastStartTag, handle->lastStartTagLen);
    name[handle->lastStartTagLen] = '\0';

    Gena_SetVariable(pService, name, tag_name, len);
}

void Gena_SetVariable(struct BUPnPService* pService, const char* name, const char* value, unsigned len)
{
    BUPnPStateVariableHandle hStateVariable;

    hStateVariable = BUPnPService_GetStateVariable(pService, name);
    if ( hStateVariable != NULL )
    {
        CString_SetTextN(&hStateVariable->value, len, value);
        BUPnPControlPoint_InvokeCallback(pService->serviceInfo->serviceId, name);
    }
}

void Gena_AddSubscription(BUPnPSubscription* pSubscription)
{
    /* mutex locking is done in Gena_Subscribe */
    BLST_DICT_ADD(BUPnPControlPoint_GetSubscriptionList(), pSubscription, BUPnPSubscription, sid, link, error);
error:
    assert(true);
}

BUPnPSubscription* Gena_GetSubscription(const char* sid)
{
    BUPnPSubscription* pSubscription;

    bthread_mutex_lock(BUPnPControlPoint_GetSubscriptionsMutex());
    BLST_DICT_FIND_BY_STR(BUPnPControlPoint_GetSubscriptionList(), pSubscription, sid, sid, link);
    bthread_mutex_unlock(BUPnPControlPoint_GetSubscriptionsMutex());

    return pSubscription;
}

void Gena_RemoveSubscriptionByService(struct BUPnPService* pService)
{
    BUPnPSubscription *pSubscription;

    for(pSubscription=BLST_FIRST(BUPnPControlPoint_GetSubscriptionList()); pSubscription; pSubscription=BLST_NEXT(pSubscription, link))
    {
        if (pSubscription->service == pService)
        {
            Timer_Destroy(pSubscription->timer);
            Gena_RemoveSubscription(pSubscription->sid);
            BUPnPFree(pSubscription->url.path);
            BUPnPFree(pSubscription);
            break;
        }
    }
}

void Gena_RemoveSubscription(const char* sid)
{
    BUPnPSubscription *pSubscription;

    bthread_mutex_lock(BUPnPControlPoint_GetSubscriptionsMutex());
    BLST_DICT_REMOVE_BY_STR(BUPnPControlPoint_GetSubscriptionList(), pSubscription, sid, BUPnPSubscription, sid, link);
    bthread_mutex_unlock(BUPnPControlPoint_GetSubscriptionsMutex());
}
