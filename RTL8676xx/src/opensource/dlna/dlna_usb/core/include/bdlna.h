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
 * $brcm_Workfile: bdlna.h $
 * $brcm_Revision: 11 $
 * $brcm_Date: 9/30/09 4:37p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/core/include/bdlna.h $
 * 
 * 11   9/30/09 4:37p dliu
 * SW7405-2482: Add DMC implementation
 * 
 * 10   9/21/09 5:11p dliu
 * SW7405-2482: Add DMC API's
 * 
 * 9   9/2/09 11:00a dliu
 * SW7405-2482: Add network interface to UPnP settings
 * 
 * 8   9/2/09 10:59a dliu
 * SW7405-2482: Add code for DMC
 * 
 * 7   7/28/09 4:57p dliu
 * PR55165: change audio to use settings
 * 
 * 6   7/24/09 3:54p dliu
 * PR55165: Add DeviceInfo structure to dmsSettings
 * 
 * 6   7/24/09 3:54p dliu
 * PR55165: Add DeviceInfo structure to dmsSettings
 * 
 * 5   7/24/09 10:42a ajitabhp
 * PR55165: DMS library Compilation.
 * 
 * 4   7/7/09 5:21p dliu
 * PR55165: Add error code for DTCP
 * 
 * 3   7/2/09 10:12a dliu
 * PR55165: Rework GetRootContainer
 * 
 * 2   6/28/09 11:13p dliu
 * PR55165: Change parameter for Browse command
* 
***************************************************************************/
#ifndef BDLNA_H_
#define BDLNA_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef LINUX
#include "stdint.h"
#endif

#include <stdio.h>        
#include <stdlib.h>        
#include <stddef.h>
#include <string.h>        
#include <stdarg.h>
#include <signal.h>
#include <errno.h>
#include <assert.h>
#include "upnp.h"

#include "bdlna_types.h"

typedef int BDlna_Error;

#define BDLNA_E_SUCCESS 0
#define BDLNA_E_ACTION_NOT_ALLOWED 0x3000
#define BDLNA_E_SERVICE_NOT_FOUND 0x3001
#define BDLNA_E_INVALID_DIDL 0x4000
#define BDLNA_E_PAUSE_NOT_SUPPORTED 0x4001
#define BDLNA_E_DTCP_FAILED 0x4002
#define BDLNA_E_SEEK_NOT_SUPPORTED 0x4003
#define BDLNA_E_FORMAT_MISMATCH 0x4004
#define BDLNA_E_INVALID_URI 0x4005
#define BDLNA_E_NO_DEVICE 0x4006

#define BDLNA_MAX_URI_SIZE 256

#define UNUSED_ARGUMENT(x) (void)x

typedef enum BDlna_DmpBrowseType { 
    BDlna_DmpBrowseType_eBrowseMetaData, 
    BDlna_DmpBrowseType_eBrowseDirectChildren 
} BDlna_DmpBrowseType;

typedef enum BDlna_DmcBrowseType { 
    BDlna_DmcBrowseType_eBrowseMetaData, 
    BDlna_DmcBrowseType_eBrowseDirectChildren 
} BDlna_DmcBrowseType;

typedef enum BDlna_DidlMediaType {
    BDlna_DidlMediaType_eMediaContainer, 
    BDlna_DidlMediaType_eMediaItem
} BDlna_DidlMediaType;

typedef struct BDlna_DidlMediaItem* BDLna_DidlMediaItemHandle;
typedef struct BDlna_Didl* BDlna_DidlHandle;

/*******************************************************************************
 Media Manager related structures and functions
*******************************************************************************/

/* define callbacks */
typedef struct BDlna_Mmi* BDlna_MmiHandle;
typedef void (* BDlna_Mmi_UriChanged)(BDlna_MmiHandle hMmi, void* context, const char* uri);
typedef void (* BDlna_Mmi_StateChanged)(BDlna_MmiHandle hMmi, void* context, BDlna_MediaState state);

typedef struct BDlna_MmiOpenSettings
{
    BDlna_Mmi_UriChanged uriChangedCallback;
    void* uriChangedContext;
    BDlna_Mmi_StateChanged stateChangedCallback;
    void* stateChangedContext;
    void* context;
} BDlna_MmiOpenSettings;

typedef struct BDlna_MmiAudioSettings
{
    int32_t leftVolume;
    int32_t rightVolume;
    bool muted;
} BDlna_MmiAudioSettings;

void BDlna_Mmi_GetSettings(BDlna_MmiHandle hMmi, BDlna_MmiOpenSettings* pMmiSettings);
void BDlna_Mmi_SetSettings(BDlna_MmiHandle hMmi, BDlna_MmiOpenSettings* pMmiSettings);
void BDlna_Mmi_GetAudioSettings(BDlna_MmiHandle hMmi, BDlna_MmiAudioSettings* pMmiSettings);
void BDlna_Mmi_SetAudioSettings(BDlna_MmiHandle hMmi, BDlna_MmiAudioSettings* pMmiSettings);
bool BDlna_Mmi_ItemIsImage(BDlna_MmiHandle hMmi);

/*******************************************************************************
 DMR related structures and functions 
*******************************************************************************/
typedef void* BDlna_DmrHandle;

typedef struct BDlna_IconInfo
{
    int32_t width;
    int32_t height;
    int32_t depth;
    char *url;
    char *mimeType;
}BDlna_IconInfo;

typedef BUPnPDeviceInfo BDlna_DeviceInfo;
typedef BUPnPDeviceHandle BDlna_DeviceHandle;

typedef struct BDlna_Dmr_Settings
{
    BDlna_DeviceInfo* pDeviceInfo;
    BDlna_MmiHandle hMmi;
    const char* protocolInfo;
} BDlna_Dmr_Settings;

void BDlna_Dmr_GetDefaultSettings(BDlna_Dmr_Settings* pSettings);
BDlna_DmrHandle BDlna_Dmr_Create(BDlna_Dmr_Settings* pSettings);
void BDlna_Dmr_Destroy(BDlna_DmrHandle hDmr);

/*******************************************************************************
 DMS related structures and functions 
*******************************************************************************/
typedef struct _BDlna_Dms_OpenSettings_
{
    BDlna_DeviceInfo* pDeviceInfo;
    char            *contentDir;
    unsigned int    ContentXferPort;
} BDlna_Dms_OpenSettings;

typedef BUPnPDeviceHandle BDlnaDmsHandle;
BDlnaDmsHandle BDlna_Dms_Create(BDlna_Dms_OpenSettings* pSettings);
void BDlna_Dms_Destroy(BUPnPDeviceHandle pDevhandle);


/*******************************************************************************
 DMP related structures, functions and callbacks
*******************************************************************************/
typedef void (* BDlna_Dmp_DeviceRemoved)(void* context, const char* uuid);
typedef void (* BDlna_Dmp_DeviceAdded)(void* context, const char* uuid);
typedef void (* BDlna_Dmp_PropertyChanged)(void* context, char* variable, char* value);

typedef struct BDlna_Dmp* BDlna_DmpHandle;
typedef BUPnPRemoteDevice* BDlna_DmpDeviceHandle;

typedef struct BDlna_Dmp_Settings
{
    BDlna_Dmp_DeviceAdded deviceAddedCallback;
    BDlna_Dmp_DeviceRemoved deviceRemovedCallback;
    BDlna_Dmp_PropertyChanged propertyChangedCallback;
    void* context;
} BDlna_Dmp_Settings;

struct BDlna_Dmp
{
    BDlna_Dmp_Settings settings;
};

void BDlna_Dmp_GetDefaultSettings(BDlna_Dmp_Settings* pSettings);
BDlna_DmpHandle BDlna_Dmp_Create(BDlna_Dmp_Settings* pSettings);
void BDlna_Dmp_Destroy(BDlna_DmpHandle);
BDlna_DmpDeviceHandle BDlna_Dmp_CreateDevice(const char* uuid);
void BDlna_Dmp_DestroyDevice(BDlna_DmpDeviceHandle hDevice);
BDlna_Error BDlna_Dmp_Browse(BDlna_DmpHandle hDmp, BDlna_DmpDeviceHandle hDevice, const char* ObjectID, uint32_t startingIndex, uint32_t requestCount,  uint32_t *count, uint32_t* totalMatches, uint32_t* updateID, BDlna_DidlHandle didlObjects);
BDlna_Error BDlna_Dmp_Search(BDlna_DmpHandle hDmp, BDlna_DmpDeviceHandle hDevice, const char* ObjectID, const char* searchCriteria, uint32_t startingIndex, uint32_t requestCount, uint32_t* count, uint32_t* totalMatches, uint32_t* updateID, BDlna_DidlHandle didlObjects);
BDlna_Error BDlna_Dmp_GetDevices(BDlna_DmpHandle, BDlna_DmpDeviceHandle** hDeviceMap, int* count);
BDlna_Error BDlna_Dmp_GetDeviceInfo(BDlna_DmpHandle, BDlna_DmpDeviceHandle hDevice, BUPnPDeviceInfo* pDeviceInfo);
BDlna_Error BDlna_Dmp_GetIconCount(BDlna_DmpHandle hDmp, BDlna_DmpDeviceHandle hDevice, size_t* count);
BDlna_Error BDlna_Dmp_GetDeviceIconInfo(BDlna_DmpHandle hDmp, BDlna_DmpDeviceHandle hDevice, int index, struct BUPnPIconInfo* pIconInfo);
BDlna_Error BDlna_Dmp_SubscribeServer(BDlna_DmpHandle hDmp, BDlna_DmpDeviceHandle hDevice);
BDlna_Error BDlna_Dmp_GetRootContainer(BDlna_DmpHandle hDmp, BDlna_DmpDeviceHandle hDevice, uint32_t* childCount);
BDlna_Error BDlna_Dmp_GetMetaData(BDlna_DmpHandle hDmp, BDlna_DmpDeviceHandle hDevice, const char* objectID, BDlna_DidlHandle didlObjects);
BDlna_Error BDlna_Dmp_GetSearchCapabilities(BDlna_DmpHandle hDmp, BDlna_DmpDeviceHandle hDevice, char** searchCap);
BDlna_Error BDlna_Dmp_BrowseRootContainer(BDlna_DmpHandle hDmp, BDlna_DmpDeviceHandle hDevice, uint32_t startingIndex, uint32_t requestCount,  uint32_t *count, uint32_t* totalMatches, uint32_t* updateID, BDlna_DidlHandle didlObjects);
void BDlna_Dmp_DownloadFile(BDlna_DmpDeviceHandle hDevice, char* uri, char* filename);
void BDlna_Dmp_GetSettings(BDlna_DmpHandle hDmp, BDlna_Dmp_Settings* pSettings);
void BDlna_Dmp_SetSettings(BDlna_DmpHandle hDmp, BDlna_Dmp_Settings* pSettings);



/*******************************************************************************
 General DIDL structures and functions 
*******************************************************************************/

#define ORG_FLAG_SENDER_PACED (1<<31)
#define ORG_FLAG_TIME_SEEK (1<<30)
#define ORG_FLAG_BYTE_SEEK (1<<29)
#define ORG_FLAG_PLAYCONTAINER (1<<28)
#define ORG_FLAG_S0_INCREASING (1<<27)
#define ORG_FLAG_SN_INCREASING (1<<26)
#define ORG_FLAG_RTSP_PAUSE (1<<25)
#define ORG_FLAG_STREAMING_MODE (1<<24)
#define ORG_FLAG_INTERACTIVE_MODE (1<<23)
#define ORG_FLAG_BACKGROUND_MODE (1<<22)
#define ORG_FLAG_HTTP_STALLING (1<<21)
#define ORG_FLAG_DLNA_V15 (1<<20)
#define ORG_FLAG_LINK_PROTECTED (1<<16)
#define ORG_FLAG_CLEAR_TEXT_SEEK (1<<15)
#define ORG_FLAG_CLEAR_BYTE (1<<14)

typedef struct BDlna_ProtocolInfo
{
    bool IsDlna15;
    bool supportTimeRange;
    bool supportByteRange;
    char* orgPN;
    bool conversionIndicator;
    char* playSpeed;
    unsigned long orgFlag;
    char* mime;
    bool isImage;
    char* networkID;
} BDlna_ProtocolInfo;

void BDlna_Didl_ParseProtocolInfo(
    char* protocolInfo, 
    BDlna_ProtocolInfo* dlnaProtocol
    );

BDlna_Error BDlna_Didl_ParseResult(
    const char* in, 
    BDlna_DidlHandle didlObjects,
    uint32_t index
    );

BUPnPActionHandle BDlna_Didl_GetAction(
    BDlna_DidlHandle didlObjects
    );

const char* BDlna_Didl_GetContentUri(
    BDlna_DidlHandle didlObjects, 
    int resourceIndex
    );

int BDlna_Didl_GetResourceCount(
    BDlna_DidlHandle didlObjects, 
    int index
    );

char* BDlna_Didl_GetProtocolInfo(
    BDlna_DidlHandle didlObjects, 
    int resourceIndex
    );

char* BDlna_Didl_GetClass(
    BDlna_DidlHandle didlObjects, 
    int index
    );

char* BDlna_Didl_GetId(
    BDlna_DidlHandle didlObjects, 
    int index
    );


char* BDlna_Didl_GetTitle(
    BDlna_DidlHandle didlObjects,
    int index
    );

BDlna_DidlMediaType BDlna_Didl_GetMediaType(
    BDlna_DidlHandle didlObjects, 
    int index
    );

char* BDlna_Didl_GetMetaData(
    BDlna_DidlHandle didlObjects, 
    int index
    );

BDlna_DidlHandle BDlna_Didl_Create(void);

void BDlna_Didl_Destroy(
    BDlna_DidlHandle DidlObjects
    );

void BDlna_Didl_ClearItems(
    BDlna_DidlHandle didlObjects
    );

uint32_t BDlna_Didl_GetChildrenCount(
    BDlna_DidlHandle DidlObjects, 
    int index
    );

/*******************************************************************************
 General DLNA structures and functions 
*******************************************************************************/
typedef struct BDlna_OpenSettings
{
    char* ipAddress; /* IP address of the system. If NULL, the library will automatically discover the IP address */
    char* networkInterface; /* Specify which interface the library will get the IP address from */
    int port;         /* Port number which the library will annouce its device on */
} BDlna_OpenSettings;

BDlna_Error BDlna_Initialize(
    const BDlna_OpenSettings* pSettings
    );
void BDlna_Uninitialize(void);
void BDlna_GetDefaultOpenSettings(
    BDlna_OpenSettings* pSetting
    );
BDlna_Error BDlna_Start(void);
void BDlna_Stop(void);

/*******************************************************************************
 DMC related structures and functions
*******************************************************************************/
typedef void (* BDlna_Dmc_DeviceRemoved)(void* context, const char* uuid);
typedef void (* BDlna_Dmc_DeviceAdded)(void* context, const char* uuid);
typedef void (* BDlna_Dmc_PropertyChanged)(void* context, char* variable, char* value);
typedef void (* BDlna_Dmc_RendererStateChanged)(void* context, BDlna_MediaState state);

typedef struct BDlna_Dmc* BDlna_DmcHandle;
typedef BUPnPRemoteDevice* BDlna_DmcDeviceHandle;

typedef struct BDlna_Dmc_Settings
{
    BDlna_Dmc_DeviceAdded dmrAddedCallback;
    BDlna_Dmc_DeviceAdded dmsAddedCallback;
    BDlna_Dmc_DeviceRemoved deviceRemovedCallback;
    BDlna_Dmc_RendererStateChanged rendererStateChanged;
    BDlna_Dmc_PropertyChanged propertyChangedCallback;
    void* context;
} BDlna_Dmc_Settings;

typedef struct BDlna_Dmc_PlaybackStatus
{
    uint32_t track;
    uint32_t relTimeSeconds;
    uint32_t durationSeconds;
    BDlna_MediaState state;
    const char* trackUri;
    const char* trackMetadata;
    const char* currentTransportAction;
    int speed;
} BDlna_Dmc_PlaybackStatus;

struct BDlna_Dmc
{
    BDlna_DmcDeviceHandle dmrSelected;
    BDlna_DmcDeviceHandle dmsSelected;
    BDlna_Dmc_Settings settings;

    BDlna_ProtocolInfo* renderProtocol;
    int renderProtocolCount;
    char playspeed[256];
};


void BDlna_Dmc_GetDefaultSettings(BDlna_Dmc_Settings* pSettings);
BDlna_DmcHandle BDlna_Dmc_Create(BDlna_Dmc_Settings* pSettings);
void BDlna_Dmc_Destroy(BDlna_DmcHandle);
BDlna_DmcDeviceHandle BDlna_Dmc_CreateDevice(const char* uuid);
void BDlna_Dmc_DestroyDevice(BDlna_DmcDeviceHandle hDevice);
BDlna_Error BDlna_Dmc_Browse(BDlna_DmcHandle hDmc, BDlna_DmcDeviceHandle hDevice, const char* ObjectID, uint32_t startingIndex, uint32_t requestCount,  uint32_t *count, uint32_t* totalMatches, uint32_t* updateID, BDlna_DidlHandle didlObjects);
BDlna_Error BDlna_Dmc_Search(BDlna_DmcHandle hDmc, BDlna_DmcDeviceHandle hDevice, const char* ObjectID, const char* searchCriteria, uint32_t startingIndex, uint32_t requestCount, uint32_t* count, uint32_t* totalMatches, uint32_t* updateID, BDlna_DidlHandle didlObjects);
BDlna_Error BDlna_Dmc_GetDevices(BDlna_DmcHandle, BDlna_DmcDeviceHandle** hDeviceMap, int* count);
BDlna_Error BDlna_Dmc_GetDeviceInfo(BDlna_DmcHandle, BDlna_DmcDeviceHandle hDevice, BUPnPDeviceInfo* pDeviceInfo);
BDlna_Error BDlna_Dmc_GetIconCount(BDlna_DmcHandle hDmc, BDlna_DmcDeviceHandle hDevice, size_t* count);
BDlna_Error BDlna_Dmc_GetDeviceIconInfo(BDlna_DmcHandle hDmc, BDlna_DmcDeviceHandle hDevice, int index, struct BUPnPIconInfo* pIconInfo);
BDlna_Error BDlna_Dmc_SubscribeServer(BDlna_DmcHandle hDmc, BDlna_DmcDeviceHandle hDevice);
BDlna_Error BDlna_Dmc_GetRootContainer(BDlna_DmcHandle hDmc, BDlna_DmcDeviceHandle hDevice, uint32_t* childCount);
BDlna_Error BDlna_Dmc_GetMetaData(BDlna_DmcHandle hDmc, BDlna_DmcDeviceHandle hDevice, const char* objectID, BDlna_DidlHandle didlObjects);
BDlna_Error BDlna_Dmc_GetSearchCapabilities(BDlna_DmcHandle hDmc, BDlna_DmcDeviceHandle hDevice, char** searchCap);
BDlna_Error BDlna_Dmc_BrowseRootContainer(BDlna_DmcHandle hDmc, BDlna_DmcDeviceHandle hDevice, uint32_t startingIndex, uint32_t requestCount,  uint32_t *count, uint32_t* totalMatches, uint32_t* updateID, BDlna_DidlHandle didlObjects);
BDlna_Error BDlna_Dmc_SubscribeRender(BDlna_DmcHandle hDmc, BDlna_DmcDeviceHandle hDevice);
BDlna_Error BDlna_Dmc_Play(BDlna_DmcDeviceHandle hDevice, int speed);
BDlna_Error BDlna_Dmc_Stop(BDlna_DmcDeviceHandle hDevice);
BDlna_Error BDlna_Dmc_Pause(BDlna_DmcDeviceHandle hDevice);
BDlna_Error BDlna_Dmc_TimeSeek(BDlna_DmcDeviceHandle hDevice, int seektimeSeconds);
BDlna_Error BDlna_Dmc_GetPlaybackStatus(BDlna_DmcDeviceHandle hDevice, BDlna_Dmc_PlaybackStatus *pStatus);
BDlna_Error BDlna_Dmc_SetUri(BDlna_DmcHandle hDmc, BDlna_DmcDeviceHandle hDevice, const char* uri, const char* metadata, char* protocolInfo);
void BDlna_Dmc_DownloadFile(BDlna_DmcDeviceHandle hDevice, char* uri, char* filename);
void BDlna_Dmc_GetSettings(BDlna_DmcHandle hDmc, BDlna_Dmc_Settings* pSettings);
void BDlna_Dmc_SetSettings(BDlna_DmcHandle hDmc, BDlna_Dmc_Settings* pSettings);

/*******************************************************************************
 General DLNA structures and functions 
*******************************************************************************/
typedef enum BDlnaDebugLevel {
    BDlnaDebugLevel_None =    0x00,
    BDlnaDebugLevel_Error =    0x01,
    BDlnaDebugLevel_Warning = 0x02,
    BDlnaDebugLevel_Trace =    0x04,
    BDlnaDebugLevel_All =    0xFF
} BDlnaDebugLevel;

typedef enum BDlnaDebugCategory {
    BDlnaDebugCategory_Mmi = 0x01,
    BDlnaDebugCategory_Dmp = 0x02,
    BDlnaDebugCategory_Dmr = 0x04,
    BDlnaDebugCategory_Dms = 0x08,
    BDlnaDebugCategory_Dmc = 0x10,
    BDlnaDebugCategory_All = 0xFF
} BDlnaDebugCategory;
void BDlnaDebug_SetSettings(uint32_t level, uint32_t category);

int ConvertTime(const char* timeTarget);

#ifdef __cplusplus
}
#endif

#endif /* BDLNA_H_ */


