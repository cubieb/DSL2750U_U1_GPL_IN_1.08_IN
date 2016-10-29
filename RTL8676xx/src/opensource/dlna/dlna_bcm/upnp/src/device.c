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
 * $brcm_Workfile: device.c $
 * $brcm_Revision: 9 $
 * $brcm_Date: 9/30/09 4:16p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/device.c $
 * 
 * 9   9/30/09 4:16p dliu
 * SW7405-2482: Add DMC
 * 
 * 8   9/2/09 8:54p dliu
 * SW7405-2482: Fix compile error
 * 
 * 7   9/2/09 6:09p dliu
 * SW7405-2482: Fix compile error
 * 
 * 6   9/2/09 4:25p dliu
 * SW7405-2482: Fix issue with crashing from notify message with sub-
 *  devices
 * 
 * 5   7/2/09 10:08a dliu
 * PR55615: DLNA Compliance test fixes
* 
***************************************************************************/
#include "service.h"
#include "device.h"
#include "controlpoint.h"
#include "nanoxml.h"
#include "debug.h"

typedef struct BUPnPDeviceParser
{
    bool bFoundRootDevice;
    BUPnPDeviceHandle hDevice;
} BUPnPDeviceParser;

static BUPnPError BUPnPDevice_GetRootDescriptionXml(BUPnPDeviceHandle hDevice, cstring_t* pxml);
static BUPnPError BUPnPDevice_BuildDescriptionXml(BUPnPDeviceHandle hDevice, cstring_t* pxml);
static void BUPnPDevice_Data(nxml_t handle, const char *tag_name, unsigned len);
static void BUPnPDevice_TagBegin(nxml_t handle, const char *tag_name, unsigned len);
static void BUPnPDevice_TagEnd(nxml_t handle, const char *tag_name, unsigned len);

BUPnPDeviceHandle BUPnPDevice_Create(BUPnPDeviceInfo* pDeviceInfo)
{
    const char* pos;
    char location[UPNP_MAX_URI_SIZE];
    BUPnPDeviceHandle hDevice, hEmbeddedDevice;
    BUPnPDeviceInfo **ppDeviceInfo;

    hDevice = (BUPnPDeviceHandle) BUPnPCalloc(1, sizeof(struct BUPnPDevice));
    if ( hDevice == NULL )
        return NULL;

    hDevice->deviceInfo = pDeviceInfo;

    if (pDeviceInfo->udn != NULL)
    {
        pos = strstr(pDeviceInfo->udn, "uuid:");
        assert(pos);

        snprintf(location, UPNP_MAX_URI_SIZE-1, "http://%s:%d/%s", BUPnP_GetIPAddress(), BUPnP_GetPortNumber(), (const char*)(pos+5));
        Url_Create(&(hDevice->url), location);
    }

    /* Create embedded devices */
    if ( hDevice->childDevices )
    {
        for (ppDeviceInfo = hDevice->childDevices; *ppDeviceInfo; ppDeviceInfo++)
        {
            hEmbeddedDevice = (BUPnPDeviceHandle)BUPnPDevice_Create(*ppDeviceInfo);
            hEmbeddedDevice->parent = hDevice;
            BLST_INSERT_HEAD(&hDevice->deviceList, hEmbeddedDevice, link);
        }
    }
    return hDevice;
}

void BUPnPDevice_AddService(BUPnPDeviceHandle hDevice, BUPnPServiceInfo* pServiceInfo)
{
    BUPnPServiceHandle hService;
    hService = BUPnPService_Create(pServiceInfo);
    hService->parent = hDevice;
    BLST_INSERT_HEAD(&hDevice->serviceList, hService, link);
}

void BUPnPDevice_Destroy(BUPnPDeviceHandle hDevice)
{
    BUPnPServiceHandle hService;
    BUPnPDeviceHandle hEmbDevice;
    struct BUPnPCustomDescription* pCustomDesc;

    /* allow NULL pointers to come in */
    if (hDevice == NULL)
        return;

    /* Destroy the embedded devices */
    while( (hEmbDevice = BLST_FIRST(&hDevice->deviceList)) != NULL )
    {
        BLST_REMOVE_HEAD(&hDevice->deviceList, link);
        BUPnPDevice_Destroy(hEmbDevice);
    }

    /* destroy services for this device */
    while( (hService = BLST_FIRST(&hDevice->serviceList)) != NULL )
    {
        BLST_REMOVE_HEAD(&hDevice->serviceList, link);
        BUPnPService_Destroy(hService);
    }

    if (hDevice->deviceType)
        BUPnPFree(hDevice->deviceType);

    while ((pCustomDesc = BLST_FIRST(&hDevice->descriptionList)) != NULL)
    {
        BUPnPFree((void*)pCustomDesc->name);
        BUPnPFree((void*)pCustomDesc->value);
        BUPnPFree((void*)pCustomDesc->attribute);
        BLST_REMOVE_HEAD(&hDevice->descriptionList, link);
        BUPnPFree(pCustomDesc);
    }

    Url_Destroy(&(hDevice->url));
    BUPnPFree(hDevice);
    hDevice = NULL;
}

BUPnPError BUPnPDevice_GetDescriptionXml(const char* pUrl, cstring_t* pxml)
{
    BUPnPDeviceHandle hDevice;
    BUPnPServiceHandle hService;

    for ( hDevice = BLST_FIRST(BUPnP_GetDeviceList()); hDevice; hDevice = BLST_NEXT(hDevice, link) )
    {
        if ( IMATCH(hDevice->url.path, pUrl) )
            return BUPnPDevice_GetRootDescriptionXml(hDevice, pxml);

        for ( hService = BLST_FIRST(&hDevice->serviceList); hService; hService = BLST_NEXT(hService, link) )
        {
            if ( IMATCH(hService->serviceInfo->scpdUrl, pUrl) )
                return BUPnPService_GetDescriptionXml(hService, pxml);
        }
    }

    return UPNP_E_SUCCESS;
}

const char* BUPnPDevice_GetLocation(BUPnPDeviceHandle hDevice)
{
    return Url_ToString(&hDevice->url);
}

BUPnPDeviceHandle BUPnPDevice_GetFromLocation(const char* location)
{
    BUPnPDeviceHandle hDevice;

    for ( hDevice = BLST_FIRST(BUPnP_GetDeviceList()); hDevice; hDevice = BLST_NEXT(hDevice, link) )
    {
        if (strcmp(BUPnPDevice_GetLocation(hDevice), location) == 0)
            return hDevice;
    }

    return NULL;
}

BUPnPDeviceHandle BUPnPDevice_Iterator(BUPnPDeviceHandle hDevice)
{
    BUPnPDeviceHandle nextDevice;

    if (hDevice == NULL)
        nextDevice = BLST_FIRST(BUPnP_GetDeviceList());
    else
    {
        nextDevice = BLST_FIRST(&hDevice->deviceList);
        if ( !nextDevice )
        {
            nextDevice = hDevice;
            while (nextDevice)
            {
                nextDevice = BLST_NEXT(nextDevice, link);
                if ( nextDevice )
                    break;
                else
                    nextDevice = hDevice->parent;
            }
        } 
    }

    return nextDevice;
}

static BUPnPError BUPnPDevice_GetRootDescriptionXml(BUPnPDeviceHandle hDevice, cstring_t* pxml)
{
    BUPnPError result;

    CString_Append(pxml, 
        "<?xml version=\"1.0\"?>\r\n"
        "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\r\n"
        "<specVersion>\r\n"
        "<major>1</major>\r\n"
        "<minor>0</minor>\r\n"
        "</specVersion>\r\n");

    result = BUPnPDevice_BuildDescriptionXml(hDevice, pxml);

    CString_Append(pxml, "</root>\r\n");

    return result;
}

static BUPnPError BUPnPDevice_BuildDescriptionXml(BUPnPDeviceHandle hDevice, cstring_t* pxml)
{
    BUPnPDeviceInfo *pDescription;
    BUPnPServiceHandle hService;
    BUPnPServiceInfo *pServiceInfo;
    struct BUPnPCustomDescription* pCustomDesc;

    pDescription = hDevice->deviceInfo;

    CString_Append(pxml, "<device>\r\n");
    CString_Append(pxml, "<deviceType>%s</deviceType>\r\n", hDevice->deviceType);
    CString_Append(pxml, "<friendlyName>%s</friendlyName>\r\n", pDescription->friendlyName);
    CString_Append(pxml, "<manufacturer>%s</manufacturer>\r\n", pDescription->manufacturer);
    if (pDescription->manufacturerUrl)
        CString_Append(pxml, "<manufacturerURL>%s</manufacturerURL>\r\n", pDescription->manufacturerUrl);
    if (pDescription->modelDescription)
        CString_Append(pxml, "<modelDescription>%s</modelDescription>\r\n", pDescription->modelDescription);
    CString_Append(pxml, "<modelName>%s</modelName>\r\n", pDescription->modelName);
    if (pDescription->modelNumber)
        CString_Append(pxml, "<modelNumber>%s</modelNumber>\r\n", pDescription->modelNumber);
    if (pDescription->modelUrl)
        CString_Append(pxml, "<modelURL>%s</modelURL>\r\n", pDescription->modelUrl);
    if (pDescription->serialNumber)
        CString_Append(pxml, "<serialNumber>%s</serialNumber>\r\n", pDescription->serialNumber);
    CString_Append(pxml, "<UDN>%s</UDN>\r\n", pDescription->udn);
    if (pDescription->upc)
        CString_Append(pxml, "<UPC>%s</UPC>\r\n", pDescription->upc);

    /* loop through any additional description defined by application */
    for (pCustomDesc = BLST_FIRST(&hDevice->descriptionList); pCustomDesc; )
    {
        if (!MATCH(pCustomDesc->attribute, ""))
            CString_Append(pxml, "<%s %s>%s</%s>", pCustomDesc->name, pCustomDesc->attribute, pCustomDesc->value, pCustomDesc->name);
        else
            CString_Append(pxml, "<%s>%s</%s>", pCustomDesc->name, pCustomDesc->value, pCustomDesc->name);
        pCustomDesc = BLST_NEXT(pCustomDesc, link);
    }

    /* generate XML for any services in this device. */
    if (!BLST_EMPTY(&hDevice->serviceList))
    {
        CString_Append(pxml, "<serviceList>\r\n");
        for (hService = BLST_FIRST(&hDevice->serviceList); hService; )
        {
            pServiceInfo = hService->serviceInfo;

            CString_Append(pxml, "<service>\r\n");
            CString_Append(pxml, "<serviceType>%s</serviceType>\r\n", pServiceInfo->serviceType);
            CString_Append(pxml, "<serviceId>%s</serviceId>\r\n", pServiceInfo->serviceId);
            CString_Append(pxml, "<SCPDURL>%s</SCPDURL>\r\n", pServiceInfo->scpdUrl);
            CString_Append(pxml, "<controlURL>%s</controlURL>\r\n", pServiceInfo->controlUrl);
            CString_Append(pxml, "<eventSubURL>%s</eventSubURL>\r\n", pServiceInfo->eventSubUrl);
            CString_Append(pxml, "</service>\r\n");
            hService = BLST_NEXT(hService, link);
        }
        CString_Append(pxml, "</serviceList>\r\n");
    }

    /* generate XML for any subdevices in this device. */
    if (hDevice->childDevices)
    {
        CString_Append(pxml, "<deviceList>\r\n");
        for ( hDevice = BLST_FIRST(&hDevice->deviceList); hDevice; hDevice = BLST_NEXT(hDevice, link) )
            BUPnPDevice_BuildDescriptionXml(hDevice, pxml);
        CString_Append(pxml, "</deviceList>\r\n");
    }
    
    if (pDescription->presentationUrl)
        CString_Append(pxml, "<presentationURL>%s</presentationURL>\r\n", pDescription->presentationUrl);

    CString_Append(pxml, "</device>\r\n");

    return UPNP_E_SUCCESS;
}

static void BUPnPDevice_TagEnd(nxml_t handle, const char *tag_name, unsigned len)
{
    BUPnPDeviceParser* parserSettings = (BUPnPDeviceParser*)handle->context;
    UNUSED_ARGUMENT(len);

    if (true == parserSettings->bFoundRootDevice)
    {
        if (IMATCH_PREFIX(tag_name, "serviceList>"))
        {
            BUPnPControlPoint_AddSubDevice(parserSettings->hDevice);
        }
    }
}

void BUPnPDevice_TagBegin(nxml_t handle, const char *tag_name, unsigned len)
{
    struct BUPnPDevice* pDevice;
    struct BUPnPDevice* pCurrentDevice;
    struct BUPnPDevice* hParentDevice;
    struct BUPnPIcon* pIcon;
    BUPnPDeviceInfo *hDeviceInfo;
    BUPnPDeviceParser* parserSettings = (BUPnPDeviceParser*)handle->context;
    UNUSED_ARGUMENT(len);
    
    if (handle->lastStartTag != NULL)
    {
        if (IMATCH_PREFIX(tag_name, "device>") || (IMATCH_PREFIX(tag_name, "device ")))
        {
            if (false == parserSettings->bFoundRootDevice)
                parserSettings->bFoundRootDevice = true;
            else
            {
                hDeviceInfo = (BUPnPDeviceInfo*)BUPnPCalloc(1, sizeof(*hDeviceInfo));
                if ( hDeviceInfo == NULL )
                {
                    UPNP_DEBUG_ERROR(BUPnPDebugCategory_Description, ("Out of memory:Create device info"));
                    return;
                }
                pDevice = BUPnPDevice_Create(hDeviceInfo);
                pCurrentDevice = parserSettings->hDevice;

                if (pCurrentDevice->parent != NULL)
                    hParentDevice = pCurrentDevice->parent;
                else
                    hParentDevice = pCurrentDevice;

                pDevice->parent = hParentDevice;
                Url_Create(&pDevice->url, BUPnPDevice_GetLocation(hParentDevice));
                assert(hParentDevice->url.buf != NULL);
                parserSettings->hDevice = (void*)pDevice;
                pDevice->urlBase = BUPnPStrdup(pCurrentDevice->urlBase);
            }
        }
        
        if (IMATCH_PREFIX(tag_name, "icon>"))
        {
            pDevice = parserSettings->hDevice;
            pIcon = (struct BUPnPIcon*)BUPnPCalloc(1, sizeof(*pIcon));
            pIcon->iconInfo = (BUPnPIconInfo*)BUPnPCalloc(1, sizeof(*pIcon->iconInfo));
            BLST_INSERT_HEAD(&pDevice->iconList, pIcon, link);
        }
    }
}

void BUPnPDevice_Data(nxml_t handle, const char *tag_name, unsigned len)
{
    struct BUPnPDevice* pDevice;
    struct BUPnPService* pService;
    struct BUPnPIcon* pIcon;
    BUPnPServiceInfo *serviceInfo;
    const char* serviceVersion;
    const char* serviceName;
    char* deviceVersion;
    char temp[UPNP_MAX_LINE_SIZE];
    char url[UPNP_MAX_URI_SIZE];
    int sep = ':';
    BUPnPDeviceParser* parserSettings = (BUPnPDeviceParser*)handle->context;
    pDevice = parserSettings->hDevice;

    if (IMATCH_PREFIX(handle->lastStartTag, "deviceType"))
    {
        UPNP_DEBUG_TRACE( BUPnPDebugCategory_Description, ("Found device %s", pDevice->deviceType));
        pDevice->deviceType = BUPnPStrndup(tag_name, len);
        deviceVersion = strrchr(pDevice->deviceType, sep);
        pDevice->version = atoi(deviceVersion+1); /* advance pass the ':' */

        /* throw away the version info from device type because we need to support newer versions */
        deviceVersion[0] = '\0';
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "URLBase"))
    {
        /* If device specifies URLBase then we should use it instead of the one from LOCATION, 7.2.24.8 */
        BUPnPFree((void*)(pDevice->urlBase));
        pDevice->urlBase = BUPnPStrndup(tag_name, len);
        UPNP_DEBUG_TRACE(BUPnPDebugCategory_Description, ("Device specifed URLBase %s", pDevice->urlBase));
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "UDN"))
    {
        if (NULL == pDevice->deviceInfo->udn)
            pDevice->deviceInfo->udn = BUPnPStrndup(tag_name+5, len-5); /* move pass "uuid:" tag */
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "modelDescription"))
    {
        pDevice->deviceInfo->modelDescription = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "modelName"))
    {
        pDevice->deviceInfo->modelName = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "modelNumber"))
    {
        pDevice->deviceInfo->modelNumber = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "modelURL"))
    {
        pDevice->deviceInfo->modelUrl = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "friendlyName"))
    {
        pDevice->deviceInfo->friendlyName = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "manufacturerURL>")) 
    {
        pDevice->deviceInfo->manufacturerUrl = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "manufacturer")) /* use bracket to prevent getting url */
    {
        pDevice->deviceInfo->manufacturer = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "modelName"))
    {
        pDevice->deviceInfo->modelName = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "serialNumber"))
    {
        pDevice->deviceInfo->serialNumber = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "UPC"))
    {
        pDevice->deviceInfo->upc = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "mimeType"))
    {
        pIcon = BLST_FIRST(&pDevice->iconList);
        pIcon->iconInfo->mimeType = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "height"))
    {
        pIcon = BLST_FIRST(&pDevice->iconList);
        pIcon->iconInfo->height = atoi(tag_name);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "width"))
    {
        pIcon = BLST_FIRST(&pDevice->iconList);
        pIcon->iconInfo->width = atoi(tag_name);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "depth"))
    {
        pIcon = BLST_FIRST(&pDevice->iconList);
        pIcon->iconInfo->depth = atoi(tag_name);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "url"))
    {
        pIcon = BLST_FIRST(&pDevice->iconList);

        strncpy(temp, tag_name, len);
        temp[len] = '\0';
        if (temp[0] != '/')
            sprintf(url, "http://%s:%d%s%s", pDevice->url.host, pDevice->url.port, pDevice->urlBase, temp);
        else
            sprintf(url, "http://%s:%d%s", pDevice->url.host, pDevice->url.port, temp);
        pIcon->iconInfo->url = BUPnPStrndup(url, strlen(url));
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "serviceType"))
    {
        serviceInfo = (struct BUPnPServiceInfo*)BUPnPCalloc(1, sizeof(*serviceInfo));
        if ( serviceInfo == NULL )
        {
            UPNP_DEBUG_ERROR(BUPnPDebugCategory_Description, ("Out of memory:Create service info"));
            return;
        }
        
        pService = BUPnPService_Create(serviceInfo);
        if (pService == NULL)
            return;

        /* For now we don't care about non-UPNP services */
        if (IMATCH_PREFIX(tag_name, UPNP_HEADER))
        {
            serviceName = tag_name + strlen(UPNP_HEADER);
            serviceVersion = (char*)strchr(serviceName, sep);
            pService->serviceInfo->serviceId = BUPnPStrndup(serviceName, serviceVersion-serviceName);
            pService->version = atoi(serviceVersion+1); /* advance pass the ':' */

            /* It is okay if version is newer, but we can only support known functionaity (7.2.1.4) */
            if (pService->version > SERVICE_VERSION)
                pService->version = SERVICE_VERSION;
        }
        else
            pService->serviceInfo->serviceId = BUPnPStrndup(tag_name, len);

        UPNP_DEBUG_TRACE(BUPnPDebugCategory_Description, ("Found service %s", pService->serviceInfo->serviceId));

        /* use INSERT_HEAD instead of DICT_ADD, so we can always grab last service added */
        BLST_INSERT_HEAD(&pDevice->serviceList, pService, link);
        UPNP_DEBUG_TRACE( BUPnPDebugCategory_Description, ("Service %s: inserted to service list"));
    }

    else if (IMATCH_PREFIX(handle->lastStartTag, "SCPDURL"))
    {
        pService = BLST_FIRST(&pDevice->serviceList);
        /* If URLBase is an absolute path, then we must use it. */
        if (IMATCH_PREFIX(pDevice->urlBase, "http://") ||
            ((tag_name[0] != '/') && (!IMATCH_PREFIX(tag_name, "http://"))))
        {
            /* need to manually prepend UrlBase to this url */
            strncpy(temp, tag_name, len);
            temp[len] = '\0';
            pService->serviceInfo->scpdUrl = (char*)BUPnPCalloc(1, strlen(pDevice->urlBase)+len+1);
            sprintf(pService->serviceInfo->scpdUrl, "%s%s", pDevice->urlBase, temp);
        }
        else
            pService->serviceInfo->scpdUrl = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "controlURL"))
    {
        pService = BLST_FIRST(&pDevice->serviceList);
        /* If URLBase is an absolute path, then we must use it. */
        if (IMATCH_PREFIX(pDevice->urlBase, "http://") ||
            ((tag_name[0] != '/') && (!IMATCH_PREFIX(tag_name, "http://"))))
        {
            /* need to manually prepend UrlBase to this url */
            strncpy(temp, tag_name, len);
            temp[len] = '\0';
            pService->serviceInfo->controlUrl = (char*)BUPnPCalloc(1, strlen(pDevice->urlBase)+len+1);
            sprintf(pService->serviceInfo->controlUrl, "%s%s", pDevice->urlBase, temp);
        }
        else
            pService->serviceInfo->controlUrl = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "eventSubURL"))
    {
        pService = BLST_FIRST(&pDevice->serviceList);
        /* If URLBase is an absolute path, then we must use it. */
        if (IMATCH_PREFIX(pDevice->urlBase, "http://") ||
            ((tag_name[0] != '/') && (!IMATCH_PREFIX(tag_name, "http://"))))
        {
            /* need to manually prepend UrlBase to this url */
            strncpy(temp, tag_name, len);
            temp[len] = '\0';
            pService->serviceInfo->eventSubUrl = (char*)BUPnPCalloc(1, strlen(pDevice->urlBase)+len+1);
            sprintf(pService->serviceInfo->eventSubUrl, "%s%s", pDevice->urlBase, temp);
        }
        else
            pService->serviceInfo->eventSubUrl = BUPnPStrndup(tag_name, len);
    }
    return;
}

BUPnPError BUPnPDevice_ParseDescription(struct BUPnPDevice* device, const char* msg)
{
    int retCode, clientId = 0;
    nxml_t handle;
    nxml_settings settings;
    struct Url urlType;
    BUPnPDeviceParser parserSettings;

    parserSettings.bFoundRootDevice = false;
    parserSettings.hDevice = device;
    memset(&settings, 0, sizeof(settings));

    UPNP_DEBUG_TRACE( BUPnPDebugCategory_Description, ("ParseDescription Enter"));
    settings.tag_begin = BUPnPDevice_TagBegin;
    settings.tag_end = BUPnPDevice_TagEnd;
    settings.data = BUPnPDevice_Data;

    nxml_open(clientId, &handle,&settings);
    handle->context = (void*)&parserSettings;

    nxml_write(handle, msg, strlen(msg));
    retCode = handle->error;

    nxml_close(handle);

    /* Make sure URLBase matches what we get from SSDP */
    if (IMATCH_PREFIX(device->urlBase, "http"))
    {
        Url_Create(&urlType, device->urlBase);

        /* we assume server name is not gonna change */
        device->url.port = urlType.port;
        Url_Destroy(&urlType);
    }

    UPNP_DEBUG_TRACE( BUPnPDebugCategory_Description, ("ParseDescription Complete"));
    return retCode;
}

BUPnPError BUPnPDevice_AddCustomDescription(BUPnPDeviceHandle hDevice, const char* name, const char* value, const char* attribute)
{
    struct BUPnPCustomDescription* description;

    description = (struct BUPnPCustomDescription*)BUPnPCalloc(1, sizeof(struct BUPnPCustomDescription));
    if ( description == NULL )
        return UPNP_E_OUT_OF_MEMORY;

    description->name = BUPnPStrdup(name);
    description->value = BUPnPStrdup(value);
    if (attribute != NULL)
        description->attribute = BUPnPStrdup(attribute);

    BLST_INSERT_HEAD(&hDevice->descriptionList, description, link);

    return UPNP_E_SUCCESS;
}

void BUPnPDevice_SetContext(BUPnPDeviceHandle hDevice, void* pContext)
{
    hDevice->context = pContext;
}

void* BUPnPDevice_GetContext(BUPnPDeviceHandle hDevice)
{
    return hDevice->context;
}

void BUPnPDevice_SetDeviceType(BUPnPDeviceHandle hDevice, const char* deviceType)
{
    hDevice->deviceType = BUPnPStrdup(deviceType);
}

BUPnPDeviceInfo* BUPnPDevice_GetDeviceInfo(BUPnPDeviceHandle hDevice)
{
    return hDevice->deviceInfo;
}




