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
#ifndef UPNPDEVICE_H__
#define UPNPDEVICE_H__

#include "upnp_priv.h"
#include "cstring.h"
#include "list.h"
#include "timer.h"
#include "url.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Device flag definition */
#define BUPNP_DEVICE_REQUIRESDESTROY 1

/* Service flag definition */
#define BUPNP_SERVICE_REQUIRESDESTROY 1

struct BUPnPIcon
{
    BUPnPIconInfo *iconInfo;
    BLST_ENTRY(BUPnPIcon) link;
};

BLST_HEAD(BUPnPIconList, BUPnPIcon);

struct BUPnPCustomDescription
{
    const char* name;
    const char* value;
    const char* attribute;
    BLST_ENTRY(BUPnPCustomDescription) link;
};
BLST_HEAD(BUPnPCustomDescriptionList, BUPnPCustomDescription);

struct BUPnPDevice
{
    /* Parent device or NULL if this is a root device */
    BUPnPDeviceHandle parent;

    /* Defines the url for the device description including the host and port information */
    Url url;

    /* Defines the base URL. All relative URLs that appear elsewhere in the description are 
       combined with this base URL (if specified) according to the rules in RFC 2396. */
    char *urlBase;

    /* device type for device descrip. */
    char* deviceType;                    

    /* list of embedded devices */
    BLST_HEAD(Devices, BUPnPDevice) deviceList;

    /* list of device services */
    BLST_HEAD(Services, BUPnPService) serviceList;
    
    /* list of available icons for the device */
    struct BUPnPIconList iconList;

    /* list of additional description */
    struct BUPnPCustomDescriptionList descriptionList;

    struct BUPnPDeviceInfo **childDevices;

    TimerHandle expirationTimer;

    uint32_t flag;

    uint32_t version; 
    
    BUPnPDeviceInfo *deviceInfo;

    void* context;

    BLST_ENTRY(BUPnPDevice) link;
};

BLST_HEAD(BUPnPDeviceList, BUPnPDevice);


BUPnPDeviceHandle BUPnPDevice_Create(BUPnPDeviceInfo* pTemplate);

void BUPnPDevice_Destroy(BUPnPDeviceHandle hDevice);

const char* BUPnPDevice_GetLocation(BUPnPDeviceHandle hDevice);

BUPnPDeviceHandle BUPnPDevice_GetFromLocation(const char* location);

BUPnPDeviceHandle BUPnPDevice_Iterator(BUPnPDeviceHandle hDevice);

BUPnPError BUPnPDevice_GetDescriptionXml(const char* pUrl, cstring_t* pxml);

BUPnPError BUPnPDevice_ParseDescription(struct BUPnPDevice* device, const char* msg);

#ifdef __cplusplus
}
#endif

#endif
