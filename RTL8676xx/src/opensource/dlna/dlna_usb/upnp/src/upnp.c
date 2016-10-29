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
 * $brcm_Workfile: upnp.c $
 * $brcm_Revision: 6 $
 * $brcm_Date: 9/30/09 4:21p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/upnp.c $
 * 
 * 6   9/30/09 4:21p dliu
 * SW7405-2482: Fix bug in removing expire timer
 * 
 * 5   9/2/09 10:56a dliu
 * SW7405-2482: Add interface to UPnP Settings
* 
***************************************************************************/
#include "upnp_priv.h"
#include "device.h"
#include "debug.h"
#include "bsocket.h"
#include "service.h"
#include "device.h"
#include "http.h"
#include "ssdp.h"
#include "list.h"
#include "time.h"
#include "heap.h"

#define UPNP_STOP_TIMEOUT              5
#define UPNP_CONNECTION_TIMEOUT         30

typedef struct BUPnPThread
{
    bthread* threadId;
    BLST_ENTRY(BUPnPThread) link;
}BUPnPThread;

typedef struct BUPnPModule
{
    bthread* hThread;

    int32_t exitFlag;

    char hostInfo[UPNP_MAX_LINE_SIZE];

    Url url;

    struct BUPnPDeviceList deviceList;
    BLST_HEAD(Connections, BUPnPConnection) connections;
    BLST_HEAD(ThreadList, BUPnPThread) threads;
}BUPnPModule;

static void BUPnP_AdvertiseTimerCallback(TimerHandle handle, void *arg);
static bool BUPnP_NewConnectionHandler(BUPnPConnectionHandle hConnection, void* args);
static int BUPnP_GetDescriptors(fd_set *fds);
static void BUPnP_ProcessReceive(fd_set *fds);
static void BUPnP_RemoveExpiredConnections(void);
static void BUPnP_ThreadProc(void *pParam);
static void BUPnP_RemoveExpiredThread(void);

static struct BUPnPModule* g_upnpModule = NULL;

extern bool Http_RequestHandler(BUPnPConnectionHandle hConnection, void* args);
extern bool Ssdp_RequestHandler(BUPnPConnectionHandle hConnection, void* args);

/* make this into structure with ControlpointHandle */
BUPnPError BUPnP_Initialize(BUPnPOpenSettings* pSettings)
{
    char host[32];
    struct utsname uts;

    assert(g_upnpModule == NULL);
    BUPnPHeapInitialize();

    g_upnpModule = (struct BUPnPModule*) BUPnPCalloc(1, sizeof(struct BUPnPModule));
    if ( g_upnpModule == NULL )
        return UPNP_E_OUT_OF_MEMORY;

    if ( pSettings->ipAddress == NULL )
    {
        host_getipaddress(pSettings->networkInterface, host, 31);
        g_upnpModule->url.host = BUPnPStrndup(host, 31);
    }
    else 
        g_upnpModule->url.host = BUPnPStrdup(pSettings->ipAddress);
    if (g_upnpModule->url.host == NULL)
        return UPNP_E_OUT_OF_MEMORY;

    g_upnpModule->url.port = pSettings->portNumber;

    uname(&uts);
    snprintf(g_upnpModule->hostInfo, UPNP_MAX_LINE_SIZE-1, 
                "%s/%s UPnP/%s China Telecom UPNP/%s", uts.sysname, uts.version, UPNP_VERSION, BLIB_VERSION); 

    Timer_Initialize();

    return UPNP_E_SUCCESS;
}

void BUPnP_Close()
{
    BUPnPDeviceHandle hDevice;

    if (!g_upnpModule->exitFlag)
        BUPnP_Stop();


    while( (hDevice = BLST_FIRST(&g_upnpModule->deviceList)) != NULL )
    {
        BLST_REMOVE_HEAD(&g_upnpModule->deviceList, link);
        BUPnPDevice_Destroy(hDevice);
    }

    Timer_Close();
    BUPnP_RemoveExpiredThread();

    BUPnPFree(g_upnpModule->url.host);
    BUPnPFree(g_upnpModule);

    BUPnPHeapClose();

    g_upnpModule = NULL;
}

BUPnPError BUPnP_Start()
{
    g_upnpModule->exitFlag = false;

    g_upnpModule->hThread = bthread_create(BUPnP_ThreadProc, NULL);
    if ( g_upnpModule->hThread == NULL )
        return UPNP_E_THREAD;

    return UPNP_E_SUCCESS;
}

BUPnPError BUPnP_Stop()
{
    g_upnpModule->exitFlag = true;

    Ssdp_SendAdvertisement(SsdpAdvertisementType_ByeBye, NULL);

    /* Remove the http listener.  This will interrupt the and force an exit */
    BUPnP_RemoveConnection( BLST_FIRST(&g_upnpModule->connections) );

    /* can't call bthread_join in Linux because select function won't get 
     * interruped by socket close */
    /* result = bthread_join(g_upnpModule->hThread, UPNP_STOP_TIMEOUT*1000);
    bthread_destroy(g_upnpModule->hThread);
    return (result == 0) ? UPNP_E_SUCCESS : UPNP_E_THREAD;
    */
    return UPNP_E_SUCCESS;
}

BUPnPDeviceHandle BUPnP_RegisterDevice(struct BUPnPDeviceInfo* pBUPnPDeviceInfo)
{
    BUPnPDeviceHandle hDevice;
    
    hDevice = BUPnPDevice_Create(pBUPnPDeviceInfo);
    if ( hDevice == NULL )
        return NULL;

    BLST_INSERT_HEAD(&g_upnpModule->deviceList, hDevice, link);

    /* server is already running send alive just for this device */
    if (g_upnpModule->hThread != NULL)
    {
        Ssdp_SendAdvertisement(SsdpAdvertisementType_Alive, hDevice);
        bsleep(200);
        Ssdp_SendAdvertisement(SsdpAdvertisementType_Alive, hDevice);
        bsleep(200);
        Ssdp_SendAdvertisement(SsdpAdvertisementType_Alive, hDevice);
    }
    return hDevice;
}

void BUPnP_UnregisterDevice(BUPnPDeviceHandle hDevice)
{
     /* If server is still runnning send byebye for just this device here */
    if (g_upnpModule->exitFlag == false)
    {
        Ssdp_SendAdvertisement(SsdpAdvertisementType_ByeBye, hDevice);
        bsleep(200);
        Ssdp_SendAdvertisement(SsdpAdvertisementType_ByeBye, hDevice);
    }

    BLST_REMOVE(&g_upnpModule->deviceList, hDevice, BUPnPDevice, link);
    BUPnPDevice_Destroy(hDevice);
}

void BUPnP_RegisterThread(bthread* hThread)
{
    BUPnPThread* thread = BUPnPCalloc(1, sizeof(BUPnPThread));
    thread->threadId = hThread;
    BLST_INSERT_HEAD(&g_upnpModule->threads, thread, link);
}

const char* BUPnP_GetIPAddress()
{
    return g_upnpModule->url.host;
}

uint32_t BUPnP_GetPortNumber()
{
    return g_upnpModule->url.port;
}

const char* BUPnP_GetDescription()
{
    return g_upnpModule->hostInfo;
}

BUPnPDeviceListHandle BUPnP_GetDeviceList()
{
    return &g_upnpModule->deviceList;
}

BUPnPConnectionHandle BUPnP_AddConnection( SOCKET s, int32_t timeout, BUPnPReceiveCallback callback, void* args )
{
    BUPnPConnectionHandle pConnection;

    pConnection = (BUPnPConnectionHandle)BUPnPCalloc(1, sizeof(*pConnection));
    if ( pConnection == NULL )
        return NULL;

    Http_CreateContext(&pConnection->context);
        
    pConnection->socket = s;
    pConnection->context.s = s;
    pConnection->callback = callback;
    pConnection->args = args;

    if ( timeout > 0 )
    {
        pConnection->timeout = timeout;
        pConnection->expireTime = time(NULL) + timeout;
    }

    BLST_INSERT_HEAD(&g_upnpModule->connections, pConnection, link);

    return pConnection;
}

void BUPnP_RemoveConnection(BUPnPConnectionHandle hConnection)
{
    if (hConnection == NULL)
        return;

    if (!BLST_EMPTY(&g_upnpModule->connections))
    BLST_REMOVE(&g_upnpModule->connections, hConnection, BUPnPConnection, link);

    if ( hConnection->socket > 0 )
        closesocket(hConnection->socket);


    Http_DestroyContext(&hConnection->context);

    BUPnPFree(hConnection);
    hConnection = NULL;
}

const char* BUPnP_GetRfc1123Date(char *buffer, size_t length)
{
    time_t now;
    now = time(NULL);
    strftime( buffer, length, "%a, %d %b %Y %H:%M:%S GMT", (const struct tm*)gmtime( &now ) );
    return buffer;
}

static int BUPnP_GetDescriptors(fd_set *fds)
{
    BUPnPConnectionHandle pConnection;
    SOCKET max = 0;
            
    FD_ZERO(fds);
    for ( pConnection = BLST_FIRST(&g_upnpModule->connections); pConnection; pConnection = BLST_NEXT(pConnection, link) )
    {
        FD_SET(pConnection->socket, fds);
        if ( max < pConnection->socket)
            max = pConnection->socket;
    }

    return (max + 1);
}

static void BUPnP_RemoveExpiredConnections(void)
{
    BUPnPConnectionHandle hConnection, hNextConnection;
    time_t now = time(NULL);

    for ( hConnection = BLST_FIRST(&g_upnpModule->connections); hConnection; )
    {
        hNextConnection = BLST_NEXT(hConnection, link);
        if ((hConnection->expireTime) && (hConnection->expireTime < now))
        {
            BUPnP_RemoveConnection(hConnection);
        }

        hConnection = hNextConnection;
    }
}

static void BUPnP_RemoveExpiredThread(void)
{
    int count = 0;
    BUPnPThread *thread, *nextThread;
    thread = BLST_FIRST(&g_upnpModule->threads);
   
    while(thread)
    {
        nextThread = BLST_NEXT(thread, link);
        if (thread->threadId->state == bthreadState_eStopped)
        {
            bthread_join(thread->threadId, 0);
            BLST_REMOVE(&g_upnpModule->threads, thread, BUPnPThread, link);
            BUPnPFree(thread->threadId);
            BUPnPFree(thread);
        }
        else
            count++;
        thread = nextThread;
    }

}

static void BUPnP_ProcessReceive(fd_set *fds)
{
    bool keepAlive;
    BUPnPConnectionHandle pConnection;

    for ( pConnection = BLST_FIRST(&g_upnpModule->connections); pConnection; pConnection = BLST_NEXT(pConnection, link) )
    {
        if (FD_ISSET(pConnection->socket, fds))
        {
            keepAlive = (*(pConnection->callback))(pConnection, pConnection->args);
            if ( keepAlive == false )
            {
                BUPnP_RemoveConnection(pConnection);
                break;
            }

            if ( pConnection->expireTime != 0 ) /* reset the expiration counter each time we receive some data. */
                pConnection->expireTime = time(NULL) + pConnection->timeout;
        }
    }
}

static bool BUPnP_NewConnectionHandler(BUPnPConnectionHandle hConnection, void* args)
{
    SOCKET s;
    UNUSED_ARGUMENT(args);

    s = accept(hConnection->socket, NULL, NULL);
    if (s != -1)
    {
        if ( BUPnP_AddConnection(s, UPNP_CONNECTION_TIMEOUT, Http_RequestHandler, NULL) == NULL )
            closesocket(s);
    }
    else
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_Protocol, ("BUPnP - Socket Accept Error %d", errno));

    return true;
}

static void BUPnP_AdvertiseTimerCallback(TimerHandle hTimer, void *arg)
{
    UNUSED_ARGUMENT(hTimer);
    UNUSED_ARGUMENT(arg);

    /* Send out NOTIFY if we have devices registered */
    if (!BLST_EMPTY(&g_upnpModule->deviceList))
        Ssdp_SendAdvertisement(SsdpAdvertisementType_Alive, NULL);
}

static void BUPnP_ThreadProc(void *pParam)
{
    SOCKET s;
    int result, n=0;
    fd_set rfds;
    struct timeval timeout;
    TimerHandle hTimer;
    TimerSettings timerSettings;
    BUPnPConnectionHandle hConnection;
    UNUSED_ARGUMENT(pParam);

    UPNP_DEBUG_TRACE( BUPnPDebugCategory_All, ("Starting BUPnP Host '%s'...", BUPnP_GetDescription() ));

    s = BSocket_CreateTcpListener(g_upnpModule->url.host, g_upnpModule->url.port);
    if ( s <= 0 )
        perror("Socket Error");
    BUPnP_AddConnection(s, 0, BUPnP_NewConnectionHandler, NULL);
    
    s = BSocket_CreateUdpListener(SSDP_ADDRESS, SSDP_PORT);
    if ( s <= 0 )
        perror("Socket Error");
    BUPnP_AddConnection(s, 0, Ssdp_RequestHandler, NULL);

    timerSettings.period = ((SSDP_REFRESH * 2)/3)*1000;
    timerSettings.callback = BUPnP_AdvertiseTimerCallback;
    timerSettings.executeOnce = false;
    hTimer = Timer_Create(&timerSettings);
    if ( hTimer ==  NULL )
        perror("Out Of Memory");

    if ( BUPnPControlPoint_IsEnabled() == true )
        BUPnPControlPoint_SearchDevices();

    if (!BLST_EMPTY(&g_upnpModule->deviceList))
    {
        if ((result = Ssdp_SendStartupAdvertisement()) != UPNP_E_SUCCESS)
        {
            UPNP_DEBUG_TRACE( BUPnPDebugCategory_All, ("SSDP initial advertisment failed. Error = %d", result));
            return;
        }
    }

    timeout.tv_sec = UPNP_CONNECTION_TIMEOUT;
    timeout.tv_usec = 0;

    g_upnpModule->exitFlag = false;
    while ( !g_upnpModule->exitFlag )
    {
        n = BUPnP_GetDescriptors(&rfds);

        /* Work around problem in Linux where timeout gets reset to 0 after select */
        timeout.tv_sec = UPNP_CONNECTION_TIMEOUT;

        result = select(n, &rfds, NULL, NULL, &timeout);
        if ( !g_upnpModule->exitFlag )
        {
            Timer_BlockEvents();

            if (result > 0)
            {
                BUPnP_ProcessReceive(&rfds);
            }
            else if (result < 0)
            {
                perror("select failed\n"); /* change to DEBUG_ERROR */
                assert(0);
            }
            BUPnP_RemoveExpiredThread();
            BUPnP_RemoveExpiredConnections();

            Timer_UnblockEvents();
        }
    }

    Timer_Destroy(hTimer);

    /* Remove all connections */
    while( (hConnection = BLST_FIRST(&g_upnpModule->connections)) != NULL )
    {
        BLST_REMOVE_HEAD(&g_upnpModule->connections, link);
        if ( hConnection->socket > 0 )
            closesocket(hConnection->socket);
        Http_DestroyContext(&hConnection->context);
        BUPnPFree(hConnection);
    }

    UPNP_DEBUG_TRACE( BUPnPDebugCategory_All, ("BUPnP Host '%s' Stopped.", BUPnP_GetDescription() ));
}
