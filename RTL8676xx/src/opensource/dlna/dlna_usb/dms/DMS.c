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
 * $brcm_Workfile: DMS.c $
 * $brcm_Revision: 8 $
 * $brcm_Date: 9/28/09 8:59p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/DMS.c $
 * 
 * 8   9/28/09 8:59p ismailk
 * SW7405-3080: Set a unique friendly name (appended by IP addr) and UDN
 *  is generated.
 * 
 * 7   9/25/09 3:38p ismailk
 * SW7405-3080: DMS IP Streaming (Live channel) Feature.
 * 
 * 6   9/3/09 12:52p dliu
 * SW7405-2482: Add interface parameter to host_getipaddress
 * 
 * 5   7/24/09 8:42p ajitabhp
 * PR55165: UPNP device Infor if not passed will be filled by server.
 * 
 * 4   7/24/09 3:55p dliu
 * PR55165: Add Deviceinfo structure to dmsSettings
 * 
 * 3   7/24/09 10:44a ajitabhp
 * PR55165: Compile DMA as library.
 * 
 * 2   6/29/09 12:13p ajitabhp
 * PR55165: Added Path Input From User.
* 
***************************************************************************/
#include "bdlna.h"
#include "DMS.h"
#include "debug.h"
#include "trace.h"
#include "device.h"

static char terminate = 0;
DMS_CONTEXT    gDmsContext;
extern BUPnPServiceInfo Service_ConnectionManager;
extern BUPnPServiceInfo Service_ContentDirectory;

#define MAX_MEDIA_PATH 80

#define BCM_DEVICE_TYPE "urn:schemas-upnp-org:device:MediaServer:1"            /* DeviceType */

#define DEVICE_INFO_BRCM_NAME "China Telecom Digital Media Server "
#define DEVICE_INFO_OLD_UUID "uuid:6C2B7799-CD05-4bc0-86CF-C016F1B926E6"
#define FRIENDLY_NAME_SIZE (64)

BUPnPDeviceInfo BcmMediaServerDevice = {
    NULL,  /* friendlyName    */
    "China Telecom",      /* manufacturer    */
    "China Telecom",    /* manufacturerUrl    */
    "China Telecom Digital Media Server Ver 1.0",    /* modelDescription    */
    "Settop Box Revision",    /* modelName    */
    "China Telecom",    /* modelNumber    */
    "China Telecom",    /* modelUrl    */
    "12345678",    /* serialNumber        */
    NULL,    /* udn    */
    "upc code",    /* upc    */
    NULL,        /* iconList    */
    "China Telecom",    /*presentationUrl*/
};


static void
bcm_dms_start_services(PDMS_CONTEXT DmsContext,
                       char *ContentDir)
{
    /*Start the Logging Service*/
    BcmOpenLog();    
    BcmLogMsg("%s:Starting Services\n",__FUNCTION__);

    /*Start the CMS Service*/
    cms_service_start_if();

    /*Start the CDS Service*/
    cds_service_start_if(DmsContext->ContentXferbaseurl,ContentDir);

    /*http_streamer_init(DmsContext->ContentXferbaseurl, 
                        gDmsContext.ContentXferPortNum);*/

    return;
}

static void
bcm_dms_stop_services()
{
    BcmLogMsg("%s:Starting Services\n",__FUNCTION__);

    /*Stop the CMS Service*/
    cms_service_stop_if();

    /*Stop the CDS Service*/
    cds_service_stop_if();
    

    /*Stop the Logging Service*/
    BcmCloseLog();    
    return;
}

static void
bcm_dms_sigterm_handler()
{
    terminate = 1;
    return;
}

BDlnaDmsHandle
BDlna_Dms_Create(BDlna_Dms_OpenSettings* pSettings)
{
    BUPnPDeviceHandle pDevhandle=NULL;
    char *ipAddr=NULL;
    BUPnPDeviceInfo *deviceInfo;

    if(pSettings->ContentXferPort)
        gDmsContext.ContentXferPortNum = pSettings->ContentXferPort;
    else 
        gDmsContext.ContentXferPortNum = CONTENT_XFER_PORT;

    ipAddr = (char *) BUPnP_GetIPAddress();
    if(!ipAddr)
        host_getipaddress(NULL, gDmsContext.ipAddr,31);
    else{
        cmsUtl_strcpy(gDmsContext.ipAddr,ipAddr);
    }

    sprintf(gDmsContext.ContentXferbaseurl,
        "http://%s:%d",gDmsContext.ipAddr,
        gDmsContext.ContentXferPortNum);    


    /* perform a shallow copy */
    deviceInfo = (BUPnPDeviceInfo*) BUPnPCalloc(1, sizeof(*deviceInfo));
    if(pSettings->pDeviceInfo)
        memcpy(deviceInfo, pSettings->pDeviceInfo, sizeof(*deviceInfo));
    else 
        memcpy(deviceInfo,&BcmMediaServerDevice,sizeof(*deviceInfo));

    /* Add IP address to "friendlyname" */
    deviceInfo->friendlyName = malloc(FRIENDLY_NAME_SIZE);
    if (deviceInfo->friendlyName) {
        cmsUtl_strcpy(deviceInfo->friendlyName, DEVICE_INFO_BRCM_NAME);
        cmsUtl_strcat(deviceInfo->friendlyName, gDmsContext.ipAddr);
    }

    /* Get the UUID */
    deviceInfo->udn = malloc(UPNP_MAX_SID_SIZE);
    if (deviceInfo->udn) {
        cmsUtl_strcpy(deviceInfo->udn, "uuid:");
        uuid_create((deviceInfo->udn+5), UPNP_MAX_SID_SIZE);
    }
    printf("\nFriendlyName=%s\nUdn=%s\n", deviceInfo->friendlyName, deviceInfo->udn);

    /*
    * Register the root device with the Upnp library.
    */
    pDevhandle = BUPnP_RegisterDevice((BUPnPDeviceInfo*)deviceInfo);
    if(!pDevhandle)
    {
        printf("upnp_init: Upnp RegisterRootDevice failed \n");
        return pDevhandle;
    }


    /*Set the Device Type*/
    BUPnPDevice_SetDeviceType(pDevhandle,BCM_DEVICE_TYPE);

    /*
    * Add the service to the device.
    */
    BUPnPDevice_AddService(pDevhandle,&Service_ContentDirectory);
    BUPnPDevice_AddService(pDevhandle,&Service_ConnectionManager);

    gDmsContext.hRootDev = pDevhandle;
    
    /*Start The Services Here.*/
    bcm_dms_start_services(&gDmsContext, pSettings->contentDir);

    /* Start the old http streamer for file playback */
    http_streamer_init(gDmsContext.ipAddr, gDmsContext.ContentXferPortNum, &gDmsContext.pHttpStreamerHandle);

    /* Start the IP streamer for broadcast playback */
    /* For broadcast the server will listen on the (ContentXferPortNum+1) */
    //initIpStreamer(gDmsContext.ipAddr, gDmsContext.ContentXferPortNum+1);

    return pDevhandle;
}

/*
* Function: BDlna_Dms_Destroy
*
* Description: This function is called when you want to clean
*              up all resources associated with the upnp device.
*              It unregisters the device, which will cause bye
*              announcements to be sent.
*/

void BDlna_Dms_Destroy(BUPnPDeviceHandle pDevhandle)
{
    /* Close IP Streamer lib */
    //unInitIpStreamer();

    if (pDevhandle->deviceInfo->friendlyName) {
        free(pDevhandle->deviceInfo->friendlyName);
    }
    if (pDevhandle->deviceInfo->udn) {
        free(pDevhandle->deviceInfo->udn);
    }

    /*
    * UnRegister the device
    */
    BUPnPFree(pDevhandle->deviceInfo);
    BUPnP_UnregisterDevice(pDevhandle);
    /*
    * Allow the library to clean-up, even if the unregister failed
    */
    bcm_dms_stop_services();    
    printf("BDlna_Dms_Destroy: end\n");
    return;
}


#ifdef COMPILE_AS_EXECUTABLE
int main(int argc, char **argv)
{
    char option;
    char ContentDir[256];
    char ipAddr[31];
    BUPnPOpenSettings            UPnPOpenSettings;
    BDlna_Dms_OpenSettings        DmsOpenSettings;    
    BDlnaDmsHandle                DmsHandle;
    BUPnPError                    ret;

#ifdef WIN32
    WORD wVersionRequested;
    WSADATA wsaData;
    wVersionRequested = MAKEWORD( 2, 2 );
    WSAStartup( wVersionRequested, &wsaData );
#endif

    if ( argc < 2 )
    {
       printf("Usage: dlna_usb mediaPath\n");
       return 1;
    }
    
    /* Register handler for clean exit upon receiving SIGTERM for SMD */
    signal(SIGTERM, bcm_dms_sigterm_handler);

    memset(&DmsOpenSettings,0,sizeof(DmsOpenSettings));
    printf("Digital Media Server Version [%d.%d Build %s %s]\n", DMS_MAJOR_VER, DMS_MINOR_VER, DMS_BUILD_DATE, DMS_BUILD_TIME);
    cmsUtl_strncpy(ContentDir, argv[1], MAX_MEDIA_PATH);
    printf("Content Dir Path Set To: %s\n",ContentDir);
    host_getipaddress(NULL,ipAddr,31);


    DmsOpenSettings.contentDir = ContentDir;/*Content Directory path as needed by Media server*/    
    DmsOpenSettings.ContentXferPort = CONTENT_XFER_PORT;    /*This the content transfer port for server*/
    UPnPOpenSettings.ipAddress = ipAddr;    /*This is the IP address that we run on*/
    UPnPOpenSettings.portNumber = 2468;        /*This is the media server port number*/

    /*Initialize the UPnP library*/    
    ret = BUPnP_Initialize(&UPnPOpenSettings);
    if(UPNP_E_SUCCESS != ret)
    {
        printf("Failed to Initialize the Framwork\n");
        return 1; 
    }

    DmsHandle = BDlna_Dms_Create(&DmsOpenSettings);
    if(!DmsHandle)
    {
        printf("Failed to Create DMS\n");
        return 1; 
    }

    BUPnP_Start();
    while(!terminate)
    {
       /* Wait for a signal in loop */
       pause();
    }

    printf("Closing the Device\n");    
    BUPnP_Stop();
    BDlna_Dms_Destroy(DmsHandle);

    return 0;
}
#endif
