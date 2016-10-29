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
 * $brcm_Workfile: ssdp.c $
 * $brcm_Revision: 8 $
 * $brcm_Date: 9/30/09 3:31p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/ssdp.c $
 * 
 * 8   9/30/09 3:31p dliu
 * SW7405-2482: Move some print statements around
 * 
 * 7   9/15/09 5:18p dliu
 * SW7405-2482: increase size of SSDP receive buffer
 * 
 * 6   9/9/09 9:36p dliu
 * SW7405-2482: Change SSDP reply to use new socket rather then reuse the
 *  existing one
 * 
 * 5   7/2/09 10:08a dliu
 * PR55615: Add support for maxAge = 'infinite'
* 
***************************************************************************/
#include <time.h>
#include "upnp_priv.h"
#include "bsocket.h"
#include "timer.h"
#include "debug.h"
#include "service.h"
#include "device.h"
#include "ssdp.h"
#include "controlpoint.h"
#include "http.h"

#define SSDP_BUF_LOWATER         1024
#define SSDP_BUF_INCREMENT        2048

#define SSDP_HOST                "239.255.255.250:1900"

#define SSDP_RANDOMIZE_WAIT_TIME(mx) (rand()/(int32_t)(((unsigned)RAND_MAX + 1) / mx))

typedef struct SsdpMSearchMessage
{
    uint32_t mx;
    char st[UPNP_MAX_LINE_SIZE];

    SOCKET s;
    struct sockaddr toAddr;
    int32_t toAddrLen;

} SsdpMSearchMessage;

typedef void (*SsdpSendMessageCallback)(const char *ntst, const char *usn, const char *location, SOCKET s, struct sockaddr *toAddr, int32_t toAddrLen);

static void Ssdp_ProcessMSearch( HttpContextHandle hContext, struct sockaddr *sourceAddr, int32_t sourceAddrLen  );
static void Ssdp_ProcessNotify( HttpContextHandle hContext );
static void Ssdp_ProcessMSearchResponse( HttpContextHandle hContext );
static bool Ssdp_MSearchResponseHandler(BUPnPConnectionHandle hConnection, void* args);
static void Ssdp_SendAlive(const char *nt, const char *usn, const char *location, SOCKET s, struct sockaddr *toAddr, int32_t toAddrLen);
static void Ssdp_SendByeBye(const char *nt, const char *usn, const char *location, SOCKET s, struct sockaddr *toAddr, int32_t toAddrLen);
static void Ssdp_SendAll(BUPnPDeviceHandle hParent, SsdpSendMessageCallback sendCallback, SOCKET s, struct sockaddr *toAddr, int32_t toAddrLen);
static int32_t Ssdp_ReceiveFrom(HttpContextHandle hContext, struct sockaddr *from, socklen_t *fromlen);
static const char* Ssdp_ParseUuid(const char* usn, char* uuid);
static void Ssdp_SendSingle(BUPnPDeviceHandle hDevice, SsdpSendMessageCallback sendCallback, SOCKET s, struct sockaddr *toAddr, int32_t toAddrLen);

bool Ssdp_RequestHandler(BUPnPConnectionHandle hConnection, void* args)
{
    BUPnPError result;
    HttpContextHandle hContext;
    struct sockaddr_in sourceAddress;
    socklen_t addressLength;
    const char *method;
    UNUSED_ARGUMENT(args);

    hContext = &hConnection->context;

    addressLength = sizeof(sourceAddress);
    memset(&sourceAddress, 0, addressLength);

    result = Ssdp_ReceiveFrom(hContext, (struct sockaddr*)&sourceAddress, &addressLength);
    if ( result != UPNP_E_SUCCESS )
        return true; /* never remove this connection */
    
    result = Http_ParseHeader(hContext);
    if (result == UPNP_E_SUCCESS)
    {
        /* UPnP spec requires the HTTP version to be HTTP/1.1 */
        if ( Http_GetVersion(hContext) > HttpVersion_10 )
        {
            method = Http_GetRequestMethod(hContext);

            if ( IMATCH_PREFIX(method, "M-SEARCH") )
            {
                Ssdp_ProcessMSearch(hContext, (struct sockaddr*)&sourceAddress, addressLength);
            }
            else if ( IMATCH_PREFIX(method, "NOTIFY") )
            {
                if (BUPnPControlPoint_IsEnabled())
                {
                    Ssdp_ProcessNotify(hContext);
                }
            }
        }
    }
    Http_ClearContext(hContext);
    return true;
}

BUPnPError Ssdp_SendStartupAdvertisement()
{
    BUPnPError result;
    /* Send byebye to clear previous instances */
    result = Ssdp_SendAdvertisement(SsdpAdvertisementType_ByeBye, NULL);
    if (result != UPNP_E_SUCCESS)
        return result;

    /* DLNA specifies no more than 10 message per 200 ms period */
    /* TODO: Check devicelist is not empty, make to helper function */
    if ((result = Ssdp_SendAdvertisement(SsdpAdvertisementType_Alive, NULL)) != UPNP_E_SUCCESS)
        return result;
    bsleep(200);
    if ((result = Ssdp_SendAdvertisement(SsdpAdvertisementType_Alive, NULL)) != UPNP_E_SUCCESS)
        return result;
    bsleep(200);
    if ((result = Ssdp_SendAdvertisement(SsdpAdvertisementType_Alive, NULL)) != UPNP_E_SUCCESS)
        return result;

    return UPNP_E_SUCCESS;
}

BUPnPError Ssdp_SendAdvertisement(SsdpAdvertisementType advertisement, BUPnPDeviceHandle hDevice)
{
    SOCKET s;
    struct sockaddr_in toAddr;

    s = BSocket_CreateUpdClient(BUPnP_GetIPAddress());
    if ( s <= 0 )
        return UPNP_E_SOCKET;

    toAddr.sin_family = AF_INET;
    toAddr.sin_addr.s_addr = inet_addr(SSDP_ADDRESS);
    toAddr.sin_port = htons(SSDP_PORT);

    switch (advertisement)
    {
    case SsdpAdvertisementType_Alive:
        if (hDevice != NULL)
            Ssdp_SendSingle(hDevice, Ssdp_SendAlive, s, (struct sockaddr*)&toAddr, sizeof(toAddr));
        else
            Ssdp_SendAll(NULL, Ssdp_SendAlive, s, (struct sockaddr*)&toAddr, sizeof(toAddr));
        break;

    case SsdpAdvertisementType_ByeBye:
        if (hDevice != NULL)
            Ssdp_SendSingle(hDevice, Ssdp_SendByeBye, s, (struct sockaddr*)&toAddr, sizeof(toAddr));
        else
            Ssdp_SendAll(NULL, Ssdp_SendByeBye, s, (struct sockaddr*)&toAddr, sizeof(toAddr));
        break;
    }
    closesocket(s);

    return UPNP_E_SUCCESS;
}

BUPnPError Ssdp_SendSearchRequest(uint32_t mx, const char* st)
{
    SOCKET socket;
    char msg[UPNP_MAX_PACKET_SIZE];
    struct sockaddr_in srcAddr;

    /* Create a new socket for each M-Search request */
    socket = BSocket_CreateUpdClient(BUPnP_GetIPAddress());
    if ( socket <= 0 )
        return UPNP_E_SOCKET;

    /* Use connection thread to handle responses */
    BUPnP_AddConnection( socket, 0, Ssdp_MSearchResponseHandler, NULL );

    /* Section: 1.2.2 Discovery: Search: Request with M-SEARCH */
    snprintf(msg, sizeof(msg)-1, 
                "M-SEARCH * HTTP/1.1\r\n"
                "HOST: %s\r\n"
                "MAN: \"ssdp:discover\"\r\n"
                "MX: %d\r\n"
                "ST: %s\r\n\r\n", SSDP_HOST, mx, st);

    assert(cmsUtl_strlen(msg) < UPNP_MAX_PACKET_SIZE);

    srcAddr = BUPnPControlPoint_GetSsdpAddr();

    if ( sendto(socket, msg, cmsUtl_strlen(msg), 0, (struct sockaddr*)&srcAddr, sizeof(srcAddr)) == -1 )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("Ssdp_SendSearchRequest - sendto Error %d", errno));
        return UPNP_E_SOCKET;
    }

    return UPNP_E_SUCCESS;
}

static void Ssdp_SendAlive(const char *nt, const char *usn, const char *location, SOCKET s, struct sockaddr *toAddr, int32_t toAddrLen)
{
    char msg[UPNP_MAX_PACKET_SIZE];

    /* Section: 1.1.2 Discovery: Advertisement: Device available - NOTIFY with ssdp:alive */
    snprintf(msg, sizeof(msg)-1, 
                "NOTIFY * HTTP/1.1\r\n"
                "HOST: %s\r\n"
                "CACHE-CONTROL: max-age=%d\r\n"
                "LOCATION: %s\r\n"
                "NT: %s\r\n"
                "NTS: ssdp:alive\r\n"
                "SERVER: %s\r\n"
                "USN: %s\r\n\r\n", SSDP_HOST, SSDP_REFRESH, location, nt, BUPnP_GetDescription(), usn);

    assert(cmsUtl_strlen(msg) < UPNP_MAX_PACKET_SIZE);

    if ( sendto(s, msg, cmsUtl_strlen(msg), 0, toAddr, toAddrLen) == -1 )
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("Ssdp_SendByeBye - sendto Error %d", errno));
}

static void Ssdp_SendByeBye(const char *nt, const char *usn, const char *location, SOCKET s, struct sockaddr *toAddr, int32_t toAddrLen)
{
    char msg[UPNP_MAX_PACKET_SIZE];
    UNUSED_ARGUMENT(location);

    /* Section: 1.1.3 Discovery: Advertisement: Device unavailable - NOTIFY with ssdp:byebye */
    snprintf(msg, sizeof(msg)-1, 
                "NOTIFY * HTTP/1.1\r\n"
                "HOST: %s\r\n"
                "NT: %s\r\n"
                "NTS: ssdp:byebye\r\n"
                "USN: %s\r\n\r\n", SSDP_HOST, nt, usn);

    assert(cmsUtl_strlen(msg) < UPNP_MAX_PACKET_SIZE);

    if ( sendto(s, msg, cmsUtl_strlen(msg), 0, toAddr, toAddrLen) == -1 )
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("Ssdp_SendByeBye - sendto Error %d", errno));
}

static void Ssdp_SendMSearchResponse(const char *st, const char *usn, const char *location, SOCKET s, struct sockaddr *toAddr, int32_t toAddrLen)
{
    SOCKET socket;
    char msg[UPNP_MAX_PACKET_SIZE], date[UPNP_MAX_LINE_SIZE];
    UNUSED_ARGUMENT(s);

    socket = BSocket_CreateUpdClient(BUPnP_GetIPAddress());

    /* Section: 1.2.3 Discovery: Search: Response */
    snprintf(msg, sizeof(msg)-1, 
                "HTTP/1.1 200 OK\r\n"
                "CACHE-CONTROL: max-age=%d\r\n"
                "DATE: %s\r\n"
                "EXT:\r\n"
                "LOCATION: %s\r\n"
                "SERVER: %s\r\n"
                "ST: %s\r\n"
                "USN: %s\r\n\r\n", SSDP_REFRESH, BUPnP_GetRfc1123Date(date, sizeof(date)), location, BUPnP_GetDescription(), st, usn);

    assert(cmsUtl_strlen(msg) < UPNP_MAX_PACKET_SIZE);

    if ( sendto(socket, msg, cmsUtl_strlen(msg), 0, toAddr, toAddrLen) == -1 )
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("Ssdp_SendMSearchResponse - sendto Error %d", errno));

    closesocket(socket);
}

static void Ssdp_ProcessNotify( HttpContextHandle hContext )
{
    char uuid[UPNP_MAX_LINE_SIZE];
    const char *maxAge, *location, *nt, *usn, *nts, *dev;
    int nMaxAge = 0;

    nts = Http_GetHeaderValue(hContext, "NTS");
    usn = Http_GetHeaderValue(hContext, "USN");
    dev = Ssdp_ParseUuid(usn, uuid);

    if (MATCH(nts, "ssdp:byebye"))
    {
        BUPnPControlPoint_RemoveDevice(uuid);
        return;
    }

    if (MATCH(nts, "ssdp:alive"))
    {
        location = Http_GetHeaderValue(hContext, "LOCATION");

        maxAge  = Http_GetHeaderValue(hContext, "CACHE-CONTROL");

        maxAge = strchr(maxAge, '=');
        if (maxAge != NULL)
        {
            maxAge++; /* advance pass '=' */
            if (IMATCH_PREFIX(maxAge, "infinite"))
                nMaxAge = 0;
            else
                nMaxAge = atoi(maxAge);
        }
        nt = Http_GetHeaderValue(hContext, "NT");

        /* Figure out if the device belongs to us */
        if ( BUPnPDevice_GetFromLocation(location) != NULL )
            return;

        if (!maxAge || !location || !nt || !nts ) 
        {
            UPNP_DEBUG_ERROR( BUPnPDebugCategory_Discovery,  ("Process a Notify that is missing the required header!\n"));
            return;
        }

        if (dev == NULL)
            return; /* Not a root device, we don't care about it */

        /* For notifys we only want to get root device */
        if (!IMATCH_PREFIX(dev, "::upnp:rootdevice"))
            return;

        BUPnPControlPoint_AddDevice(uuid, nMaxAge, location);
    }
    else
        UPNP_DEBUG_ERROR( BUPnPDebugCategory_Discovery, ("Receive a Notify message with unknown NTS header: %s", nts));
}

static void Ssdp_ProcessMSearchRootDevice(SsdpMSearchMessage *pMsg)
{
    char usn[UPNP_MAX_LINE_SIZE];
    BUPnPDeviceHandle hDevice; 

    for ( hDevice = BUPnPDevice_Iterator(NULL); hDevice; hDevice = BUPnPDevice_Iterator(hDevice) )
    {
        if ( hDevice->parent == NULL )
        {
            snprintf(usn, sizeof(usn)-1, "%s::upnp:rootdevice", hDevice->deviceInfo->udn);
            Ssdp_SendMSearchResponse("upnp:rootdevice", usn, BUPnPDevice_GetLocation(hDevice), pMsg->s, &pMsg->toAddr, pMsg->toAddrLen);
        }
    }
}

static void Ssdp_ProcessMSearchUuid(SsdpMSearchMessage *pMsg)
{
    BUPnPDeviceHandle hDevice;

    for ( hDevice = BUPnPDevice_Iterator(NULL); hDevice; hDevice = BUPnPDevice_Iterator(hDevice) )
    {
        if (MATCH(hDevice->deviceInfo->udn, pMsg->st)) 
            Ssdp_SendMSearchResponse(hDevice->deviceInfo->udn, hDevice->deviceInfo->udn, BUPnPDevice_GetLocation(hDevice), 
                pMsg->s, &pMsg->toAddr, pMsg->toAddrLen);
    }
}

static void Ssdp_ProcessMSearchDeviceType(SsdpMSearchMessage *pMsg)
{
    char usn[UPNP_MAX_LINE_SIZE];
    BUPnPDeviceHandle hDevice;

    for ( hDevice = BUPnPDevice_Iterator(NULL); hDevice; hDevice = BUPnPDevice_Iterator(hDevice) )
    {
        if (MATCH(hDevice->deviceType, pMsg->st)) 
        {
            snprintf(usn, sizeof(usn)-1, "%s::%s", hDevice->deviceInfo->udn, hDevice->deviceType);
            Ssdp_SendMSearchResponse(hDevice->deviceType, usn, BUPnPDevice_GetLocation(hDevice), 
                pMsg->s, &pMsg->toAddr, pMsg->toAddrLen);
        }
    }
}
 
static void Ssdp_ProcessMSearchServiceType(SsdpMSearchMessage *pMsg)
{
    char usn[UPNP_MAX_LINE_SIZE];
    BUPnPDeviceHandle hDevice;
    BUPnPServiceHandle hService;
    
    for ( hDevice = BUPnPDevice_Iterator(NULL); hDevice; hDevice = BUPnPDevice_Iterator(hDevice) )
    {
        for ( hService = BLST_FIRST(&hDevice->serviceList); hService; hService = BLST_NEXT(hService, link) )
        {
            if (MATCH(hService->serviceInfo->serviceType, pMsg->st)) 
            {
                snprintf(usn, sizeof(usn)-1, "%s::%s", hDevice->deviceInfo->udn, hService->serviceInfo->serviceType);
                Ssdp_SendMSearchResponse(hService->serviceInfo->serviceType, usn, BUPnPDevice_GetLocation(hDevice), 
                    pMsg->s, &pMsg->toAddr, pMsg->toAddrLen);
            }
        }
    }
}

static void Ssdp_MSearchRequestHandler(TimerHandle hTimer, void *arg)
{
    SsdpMSearchMessage *pMsg;

    UPNP_DEBUG_TRACE(BUPnPDebugCategory_Discovery, ("Ssdp_MSearchRequestHandler Enter"));
    Timer_Destroy(hTimer);
    
    pMsg = (SsdpMSearchMessage*)arg;

    if (MATCH(pMsg->st, "ssdp:all"))
        Ssdp_SendAll(NULL, Ssdp_SendMSearchResponse, pMsg->s, &pMsg->toAddr, pMsg->toAddrLen);
    else if (MATCH(pMsg->st, "upnp:rootdevice"))
        Ssdp_ProcessMSearchRootDevice(pMsg);
    else if (MATCH_PREFIX(pMsg->st, "uuid:"))
        Ssdp_ProcessMSearchUuid(pMsg);
    else if (MATCH_PREFIX(pMsg->st, "urn:") && cmsUtl_strstr(pMsg->st, ":device:"))
        Ssdp_ProcessMSearchDeviceType(pMsg);
    else if (MATCH_PREFIX(pMsg->st, "urn:") && cmsUtl_strstr(pMsg->st, ":service:"))
        Ssdp_ProcessMSearchServiceType(pMsg);
    else
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("M-SEARCH - invalid ST header"));

    BUPnPFree(pMsg);
}

static void Ssdp_ProcessMSearch( HttpContextHandle hContext, struct sockaddr *toAddr, int32_t toAddrLen )
{
    char *mxend;
    const char *st, *mx, *man, *uri;
    int32_t mxval;
    TimerHandle hTimer;
    TimerSettings timerSettings;
    SsdpMSearchMessage *pMsg;
    UPNP_DEBUG_TRACE(BUPnPDebugCategory_Discovery, ("Ssdp_ProcessMSearch Enter"));

    uri = Http_GetRequestUri(hContext);
    if ( uri == NULL || uri[0] != '*' )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("M-SEARCH - Invalid HTTP request line"));
        return;
    }

    st  = Http_GetHeaderValue(hContext, "ST");
    mx  = Http_GetHeaderValue(hContext, "MX");
    man = Http_GetHeaderValue(hContext, "MAN");

    if (!st || !mx || !man) 
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("M-SEARCH - Missing ST, MX, or MAN header"));
        return;
    }
    
    if (!MATCH(man, "\"ssdp:discover\"")) 
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("M-SEARCH - Invalid MAN header"));
        return;
    }
    
    mxval = (int32_t)strtol(mx, &mxend, 10);
    if (mxend == mx || *mxend != '\0' || mxval < 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("M-SEARCH - Invalid MX header"));
        return;
    }

    pMsg = (struct SsdpMSearchMessage*)BUPnPCalloc(1, sizeof(SsdpMSearchMessage));
    if ( pMsg == NULL )
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("M-SEARCH - Out Of Memory"));
        return;
    }

    cmsUtl_strncpy(pMsg->st, st, 255);
    pMsg->s = hContext->s;
    pMsg->toAddr = *toAddr;
    pMsg->toAddrLen = toAddrLen;

    /* generate a random sequence of integers between 0 and mxval-1 */
    if ( mxval > 120 )
        mxval = 120;

    pMsg->mx = SSDP_RANDOMIZE_WAIT_TIME(mxval);
    if (pMsg->mx == 0 )
        pMsg->mx = 1;

    UPNP_DEBUG_TRACE(BUPnPDebugCategory_Discovery, ("Ssdp_ProcessMSearch:%s waiting %d to send response", man, pMsg->mx));

    timerSettings.period = pMsg->mx * 1000;
    timerSettings.callback = Ssdp_MSearchRequestHandler;
    timerSettings.param = (void*)pMsg;
    timerSettings.executeOnce = true;
    hTimer = Timer_Create(&timerSettings);

    if ( hTimer ==  NULL )
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("M-SEARCH - Timer Create Error"));
}

static void Ssdp_ProcessMSearchResponse( HttpContextHandle hContext )
{
    char uuid[UPNP_MAX_LINE_SIZE];
    const char *maxAge, *location, *usn, *dev = NULL;
    int nMaxAge = 0;


    maxAge  = Http_GetHeaderValue(hContext, "CACHE-CONTROL");
        maxAge = strchr(maxAge, '=');
        if (maxAge != NULL)
    {
        maxAge++; /* advance pass '=' */
        if (IMATCH_PREFIX(maxAge, "infinite"))
            nMaxAge = 0;
        else
            nMaxAge = atoi(maxAge);
    }

    location = Http_GetHeaderValue(hContext, "LOCATION");

    /* Figure out if the device belongs to us */
    if ( BUPnPDevice_GetFromLocation(location) != NULL )
        return;

    /* Figure out UUID and do more error checking */
    usn = Http_GetHeaderValue(hContext, "USN");
    dev = Ssdp_ParseUuid(usn, uuid);

    if (dev == NULL)
        return; /* Not a root device, we don't care about it */

    /* For M-SEARCH result we look for devices in case the app wants to only search for 
       certain device type */
    if (!cmsUtl_strstr(dev, ":device:"))
        return;

    if (!maxAge || !location)
    {
        UPNP_DEBUG_ERROR( BUPnPDebugCategory_Discovery,  ("Notify is missing the required header!\n"));
        return;
    }

    UPNP_DEBUG_TRACE(BUPnPDebugCategory_Discovery, ("Receive M-Search response: %s\n %s\n", location, dev));
    BUPnPControlPoint_AddDevice(uuid, nMaxAge, location);
}

static bool Ssdp_MSearchResponseHandler(BUPnPConnectionHandle hConnection, void* args)
{
    HttpContextHandle hContext;
    int32_t result = UPNP_E_SUCCESS;
    struct sockaddr_in sourceAddress;
    socklen_t addressLength;
    UNUSED_ARGUMENT(args);

    hContext = &hConnection->context;

    addressLength = sizeof(sourceAddress);
    memset(&sourceAddress, 0, addressLength);

    /* Response will not contain any header */
    result = Ssdp_ReceiveFrom(hContext, (struct sockaddr*)&sourceAddress, &addressLength);
    if (result == UPNP_E_SUCCESS)
    {
        result = Http_ParseHeader(hContext);
        if ( result == UPNP_E_SUCCESS )
        {
            /* Device should not send back error messages Section 1.2.3 */
            Ssdp_ProcessMSearchResponse(hContext);
        }
    }
    return true;
}

static void Ssdp_SendSingle(BUPnPDeviceHandle hDevice, SsdpSendMessageCallback sendCallback, SOCKET s, struct sockaddr *toAddr, int32_t toAddrLen)
{
    char usn[UPNP_MAX_LINE_SIZE];
    BUPnPServiceHandle hService;
    BUPnPDeviceHandle hEmbDevice;

    assert( hDevice->parent == NULL );

    snprintf(usn, sizeof(usn)-1, "%s::upnp:rootdevice", hDevice->deviceInfo->udn);
    sendCallback("upnp:rootdevice", usn, BUPnPDevice_GetLocation(hDevice), s, toAddr, toAddrLen);

    sendCallback(hDevice->deviceInfo->udn, hDevice->deviceInfo->udn, BUPnPDevice_GetLocation(hDevice), s, toAddr, toAddrLen);

    snprintf(usn, sizeof(usn)-1, "%s::%s", hDevice->deviceInfo->udn, hDevice->deviceType);
    sendCallback(hDevice->deviceType, usn, BUPnPDevice_GetLocation(hDevice), s, toAddr, toAddrLen);

    for ( hService = BLST_FIRST(&hDevice->serviceList); hService; hService = BLST_NEXT(hService, link) )
    {
        snprintf(usn, sizeof(usn)-1, "%s::%s", hDevice->deviceInfo->udn, hService->serviceInfo->serviceType);
        sendCallback(hService->serviceInfo->serviceType, usn, BUPnPDevice_GetLocation(hDevice), s, toAddr, toAddrLen);
    }

    for ( hEmbDevice = BLST_FIRST(&hDevice->deviceList); hEmbDevice; hEmbDevice = BLST_NEXT(hEmbDevice, link) )
        Ssdp_SendAll(hEmbDevice, sendCallback, s, toAddr, toAddrLen);
}

        
static void Ssdp_SendAll(BUPnPDeviceHandle hParent, SsdpSendMessageCallback sendCallback, SOCKET s, struct sockaddr *toAddr, int32_t toAddrLen)
{
    char usn[UPNP_MAX_LINE_SIZE];
    BUPnPServiceHandle hService;
    BUPnPDeviceHandle hDevice, hEmbDevice, hFirstDevice;

    if ( hParent == NULL )
        hFirstDevice = BLST_FIRST(BUPnP_GetDeviceList());
    else
        hFirstDevice = BLST_FIRST(&hParent->deviceList);

    for ( hDevice = hFirstDevice; hDevice; hDevice = BLST_NEXT(hDevice, link) )
    {
        if ( hDevice->parent == NULL )
        {
            snprintf(usn, sizeof(usn)-1, "%s::upnp:rootdevice", hDevice->deviceInfo->udn);
            sendCallback("upnp:rootdevice", usn, BUPnPDevice_GetLocation(hDevice), s, toAddr, toAddrLen);
        }
        
        sendCallback(hDevice->deviceInfo->udn, hDevice->deviceInfo->udn, BUPnPDevice_GetLocation(hDevice), s, toAddr, toAddrLen);
        
        snprintf(usn, sizeof(usn)-1, "%s::%s", hDevice->deviceInfo->udn, hDevice->deviceType);
        sendCallback(hDevice->deviceType, usn, BUPnPDevice_GetLocation(hDevice), s, toAddr, toAddrLen);
        
        for ( hService = BLST_FIRST(&hDevice->serviceList); hService; hService = BLST_NEXT(hService, link) )
        {
            snprintf(usn, sizeof(usn)-1, "%s::%s", hDevice->deviceInfo->udn, hService->serviceInfo->serviceType);
            sendCallback(hService->serviceInfo->serviceType, usn, BUPnPDevice_GetLocation(hDevice), s, toAddr, toAddrLen);
        }
        
        for ( hEmbDevice = BLST_FIRST(&hDevice->deviceList); hEmbDevice; hEmbDevice = BLST_NEXT(hEmbDevice, link) )
            Ssdp_SendAll(hEmbDevice, sendCallback, s, toAddr, toAddrLen);
    }
}

static int32_t Ssdp_ReceiveFrom(HttpContextHandle hContext, struct sockaddr *from, socklen_t *fromlen)
{
    char *p;
    int nbytes;
    HttpStream *pStream;

    pStream = &hContext->stream;

    if ((pStream->capacity - pStream->length) < SSDP_BUF_LOWATER)
    {
        p = (char*)BUPnPRealloc(pStream->buffer, pStream->capacity + SSDP_BUF_INCREMENT);
        if ( p == NULL )
            return UPNP_E_OUT_OF_MEMORY;

        pStream->buffer = p;
        pStream->capacity += SSDP_BUF_INCREMENT;
    }

    nbytes = recvfrom(hContext->s, pStream->buffer + pStream->length, pStream->capacity - pStream->length - 1, 0, from, fromlen);
    if (nbytes == 0)
    {
        UPNP_DEBUG_TRACE(BUPnPDebugCategory_Discovery, ("Ssdp_ReceiveFrom - Socket Closed"));
        return UPNP_E_SOCKET_CLOSED;
    }
    else if (nbytes < 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Discovery, ("Ssdp_ReceiveFrom - Socket Error %d", errno));
        return UPNP_E_SOCKET;
    }

    pStream->length += nbytes;
    pStream->buffer[pStream->length] = 0;

    return UPNP_E_SUCCESS;
}

static const char* Ssdp_ParseUuid(const char* usn, char* uuid)
{
    const char* uuidStart, *dev=NULL;

    if ((uuidStart = strchr(usn, ':')) != NULL)
    {
        uuidStart += 1;
        if ((dev = cmsUtl_strstr(usn, "::")) != NULL)
        {
            cmsUtl_strncpy(uuid, uuidStart, dev-uuidStart);
            uuid[dev-uuidStart] = '\0';
        }
        else
            cmsUtl_strncpy(uuid, uuidStart, UPNP_MAX_LINE_SIZE-1);
    }
    return dev;
}

