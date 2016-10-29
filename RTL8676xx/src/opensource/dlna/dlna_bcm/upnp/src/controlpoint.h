/***************************************************************************
*     (c)2004-2009 Broadcom Corporation
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
 * $brcm_Workfile: controlpoint.h $
 * $brcm_Revision: 4 $
 * $brcm_Date: 9/16/09 10:30a $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/controlpoint.h $
 * 
 * 4   9/16/09 10:30a dliu
 * SW7405-2482: Change char to const char
 * 
 * 3   7/2/09 10:07a dliu
 * PR55615: Fix for maxAge = 'infinite'
* 
***************************************************************************/
#ifndef CONTROLPOINT_H__
#define CONTROLPOINT_H__
#include "list.h"
#include "timer.h"
#include "variable.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ControlPoint* ControlPointHandle;

#define SSDP_ALL "ssdp:all"

struct ControlPoint
{
    /* Variables to track server */
    struct BUPnPDeviceList deviceList;
    bthread_mutex_t* deviceListMutex;
    struct BUPnPDevice* MediaServer;
    struct BUPnPDevice* MediaRenderer;
    size_t deviceCount;

    /* SSDP related variables */
    struct sockaddr_in SsdpAddr;

    /* Subscription related variables */
    struct BUPnPSubscriptionList subscriptionList;
    bthread_mutex_t* subscriptionListMutex;

    BUPnPControlPoint_DeviceRemoved deviceRemoved;
    BUPnPControlPoint_DeviceAdded deviceAdded;
    BUPnPControlPoint_PropertyChanged propertyChanged;
    ControlPointSearchOption searchOption;
    char *searchType;
    char *userAgent;

    void* context;
};


bthread_mutex_t* BUPnPControlPoint_GetSubscriptionsMutex(void);

struct BUPnPSubscriptionList* BUPnPControlPoint_GetSubscriptionList(void);

struct sockaddr_in BUPnPControlPoint_GetSsdpAddr(void);

void BUPnPControlPoint_AddDevice(const char* uuid, int maxAge, const char* location);

void BUPnPControlPoint_AddSubDevice(struct BUPnPDevice* pDevice);

void BUPnPControlPoint_RemoveDevice(const char* uuid);

void BUPnPControlPoint_InvokeCallback(const char* serviceName, const char* variable);

void BUPnPControlPoint_GetDefaultOpenSettings(BUPnPControlPointOpenSettings* pOpenSettings);

const char* BUPnPControlPoint_GetUserAgent(void);

#ifdef __cplusplus
}
#endif

#endif
