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
 * $brcm_Workfile: upnp.h $
 * $brcm_Revision: 7 $
 * $brcm_Date: 9/15/09 10:22p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/include/upnp.h $
 * 
 * 7   9/15/09 10:22p dliu
 * SW7405-2482: Add DMC implementation
 * 
 * 6   9/2/09 10:50a dliu
 * SW7405-2482: Add network interface to UPnP settings
 * 
 * 5   6/26/09 6:19p ajitabhp
 * PR55165: Some typedefinition changes for MIPS compilation.
* 
***************************************************************************/
#ifndef UPNP_H__
#define UPNP_H__

#if defined(LINUX)
#include <sys/types.h>
#endif

#if defined(WIN32)
#include <winsock2.h>        
#include <io.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define UPNP_VERSION    "1.0"
#define BLIB_VERSION    "0.9"

#if defined(WIN32)
typedef             __int8  int8_t;
typedef             __int16 int16_t;
typedef             __int32 int32_t;
typedef unsigned    __int8  uint8_t;
typedef unsigned    __int16 uint16_t;
typedef unsigned    __int32 uint32_t;
#endif

#if defined(LINUX)
typedef int SOCKET;

#ifndef uint8_t
typedef unsigned char uint8_t;
#endif

#ifndef uint16_t
typedef unsigned short uint16_t;
#endif

#ifndef uint32_t
typedef unsigned int  uint32_t;
#endif

#endif

#ifndef bool
#define bool int8_t
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif

/* Helper macro */
#define MATCH_PREFIX(a, b)  (strncmp((a),(b),cmsUtl_strlen(b))==0)
#define MATCH(a, b)   (strcmp((a),(b))==0)
 
#define IMATCH_PREFIX(a, b)  (strncasecmp((a),(b),cmsUtl_strlen(b))==0)
#define IMATCH(a, b)   (strcasecmp((a),(b))==0)

#define UNUSED_ARGUMENT(x) (void)x

/* Action Errors */
#define UPNP_E_SUCCESS                             0
#define UPNP_E_INVALID_ACTION                     401
#define UPNP_E_INVALID_ARG                         402
#define UPNP_E_ACTION_FAILED                     501
#define UPNP_E_ARGUMENT_VALUE_INVALID             600
#define UPNP_E_ARGUMENT_VALUE_OUT_OF_RANGE         601
#define UPNP_E_OPTIONAL_ACTION_NOT_IMPLEMENTED  602
#define UPNP_E_OUT_OF_MEMORY                     603
#define UPNP_E_HUMAN_INTERVENTION_REQUIRED         604
#define UPNP_E_STRING_ARGUMENT_TOO_LONG         605
#define UPNP_E_ACTION_NOT_AUTHORIZED             606
#define UPNP_E_SIGNATURE_FAILURE                 607
#define UPNP_E_SIGNATURE_MISSING                 608
#define UPNP_E_NOT_ENCRYPTED                     609
#define UPNP_E_INVALID_SEQUENCE                 610
#define UPNP_E_INVALID_CONTROL_URL                 611
#define UPNP_E_NO_SUCH_SESSION                     612
#define UPNP_E_SERVICE_NOT_FOUND                613

#define UPNP_E_TIMEOUT                            1000
#define UPNP_E_THREAD                            1001
#define UPNP_E_NO_DEVICE                        1002
#define UPNP_E_NO_ICON                            1003

typedef unsigned BUPnPError;

typedef struct BUPnPDevice *BUPnPDeviceHandle;

typedef struct BUPnPRemoteDevice
{
    BUPnPDeviceHandle device;
} BUPnPRemoteDevice, * BUPnPRemoteDeviceHandle;

typedef struct BUPnPService *BUPnPServiceHandle;

typedef struct BUPnPAction *BUPnPActionHandle;

typedef struct BUPnPArgument *BUPnPArgumentHandle;

typedef struct BUPnPStateVariable *BUPnPStateVariableHandle;

typedef struct BUPnPIcon *BUPnPIconHandle;

typedef struct BUPnPConnection *BUPnPConnectionHandle;

typedef BUPnPError (*BUPnPActionHandler)(BUPnPServiceHandle hService, BUPnPActionHandle hAction);

typedef bool (*BUPnPReceiveCallback)(BUPnPConnectionHandle hConnection, void* args);

typedef BUPnPError (*BUPnPCreateModerateEvent)(BUPnPServiceHandle hService, BUPnPStateVariableHandle hModerateVariable, bool bSentAll);

typedef struct BUPnPIconList* BUPnPIconListHandle;

typedef enum BUPnPAttribute
{
    BUPnPAttribute_In =    0x01,
    BUPnPAttribute_Out = 0x02,
    BUPnPAttribute_RetVal =    0x04,
    BUPnPAttribute_Evented = 0x08,
    BUPnPAttribute_Range = 0x10,
    BUPnPAttribute_List = 0x20,
    BUPnPAttribute_Moderated = 0x40,
    BUPnPAttribute_IndirectEvent = 0x80
}BUPnPAttribute;

/* Do not rearrange the order here */
typedef enum BUPnPType
{
    BUPnPType_UI1,
    BUPnPType_UI2,
    BUPnPType_UI4,
    BUPnPType_I1,
    BUPnPType_I2,
    BUPnPType_I4,
    BUPnPType_Int,
    BUPnPType_R4,
    BUPnPType_R8,
    BUPnPType_Number, 
    BUPnPType_Fixed_14_4,
    BUPnPType_Float,
    BUPnPType_Char,
    BUPnPType_Boolean,
    BUPnPType_BinBase64,
    BUPnPType_BinHex,
    BUPnPType_Uuid,
    BUPnPType_String,
    BUPnPType_Date,
    BUPnPType_DateTime,
    BUPnPType_DateTimeTz,
    BUPnPType_Time,
    BUPnPType_TimeTz,
    BUPnPType_Uri
}BUPnPType;

typedef enum BUPnPDebugLevel {
    BUPnPDebugLevel_None =    0x00,
    BUPnPDebugLevel_Error =    0x01,
    BUPnPDebugLevel_Warning = 0x02,
    BUPnPDebugLevel_Trace =    0x04,
    BUPnPDebugLevel_All =    0xFF
} BUPnPDebugLevel;

typedef enum BUPnPDebugCategory {
    BUPnPDebugCategory_Protocol = 0x01,
    BUPnPDebugCategory_Discovery = 0x02,
    BUPnPDebugCategory_Description = 0x04,
    BUPnPDebugCategory_Control = 0x08,
    BUPnPDebugCategory_Eventing = 0x10,
    BUPnPDebugCategory_Timer = 0x20,
    BUPnPDebugCategory_All = 0xFF
} BUPnPDebugCategory;

typedef enum BUPnPControlPointEvent{

    BUPnPControlPointEvent_eDeviceDisconnected, 
    BUPnPControlPointEvent_eDeviceConnected,
    BUPnPControlPointEvent_ePropertyChanged,
    BUPnPControlPointEvent_eActionComplete
} BUPnPControlPointEvent;

typedef enum ControlPointSearchOption{
    eAllDevices, 
    eSpecificDevice 
} ControlPointSearchOption;

typedef struct 
{
    char* minimum;
    char* maximum;
    char* step;
} BUPnPAllowedValueRange;

typedef char *BUPnPAllowedValueList[];

typedef union BUPnPAllowedValues
{
    char** list;
    BUPnPAllowedValueRange *range;
} BUPnPAllowedValues;

typedef struct BUPnPStateVariableInfo
{
    const char *name;
    unsigned char attributes;
    BUPnPType dataType;
    const char *defaultValue;
    BUPnPAllowedValues allowedValues;
} BUPnPStateVariableInfo;

typedef struct BUPnPArgumentInfo
{
    const char* name;
    unsigned char attributes;
    BUPnPStateVariableInfo* relatedStateVariable; 
}BUPnPArgumentInfo;

typedef struct BUPnPActionInfo
{
    const char* name;
    BUPnPActionHandler handler;
    BUPnPArgumentInfo **arguments;
}BUPnPActionInfo;

typedef struct BUPnPIconInfo
{
    int32_t width;
    int32_t height;
    int32_t depth;
    char *url;
    char *mimeType;
}BUPnPIconInfo;

typedef struct BUPnPOpenSettings
{
    const char* ipAddress;
    const char* networkInterface;
    uint16_t portNumber;
} BUPnPOpenSettings;

typedef struct BUPnPServiceInfo
{
    char* serviceType;                    /* Required.  BUPnP service type. */
    char* serviceId;                    /* Required.  Service identifier.  Must be unique within the device description */
    char* scpdUrl;                        /* Required.  URL for service description.  May be relative to base URL. */
    char* controlUrl;                    /* Required.  URL for control. May be relative to base URL. */
    char* eventSubUrl;                    /* Required.  URL for eventing.  May be relative. Must be unique within the device. */
    BUPnPActionInfo **actions;            /* Optional.  List of actions implemented by the service */
    BUPnPStateVariableInfo **variables;    /* Required.  Each service must have > 0 state variables */
}BUPnPServiceInfo;

typedef struct BUPnPDeviceInfo
{
    char* friendlyName;                    /* Required. Short description for end user. Should be localized and < 64 characters. */
    char* manufacturer;                    /* Required. Manufacturer's name. May be localized. Should be < 64 characters. */
    char* manufacturerUrl;                /* Optional. Web site for Manufacturer. May be localized. May be relative to base URL. */
    char* modelDescription;                /* Recommended. Long description for end user. Should be localized and < 128 characters. */
    char* modelName;                    /* Required. Model name. May be localized. Should be < 32 characters. */
    char* modelNumber;                    /* Recommended. Model number. May be localized. Should be < 32 characters. */
    char* modelUrl;                        /* Optional. Web site for model. May be localized. May be relative to base URL. */
    char* serialNumber;                    /* Recommended. Serial number. May be localized. Should be < 64 characters. */
    char* udn;                            /* Required. Unique Device Name in UUID format. */
    char* upc;                            /* Optional. Universal Product Code. 12-digit, all-numeric code */
    BUPnPIconInfo **iconList;            /* Required if and only if device has one or more icons */
    const char* presentationUrl;        /* Recommended. URL to presentation for device. May be relative to base URL. */
}BUPnPDeviceInfo;

/* BUPnP Engine API */
BUPnPError BUPnP_Initialize(BUPnPOpenSettings* pSettings);
void BUPnP_Close(void);
BUPnPError BUPnP_Start(void);
BUPnPError BUPnP_Stop(void);
BUPnPDeviceHandle BUPnP_RegisterDevice(struct BUPnPDeviceInfo* pDevice);
void BUPnP_UnregisterDevice(BUPnPDeviceHandle hDevice);
BUPnPError BUPnP_AddCustomDescription(BUPnPDeviceHandle hDevice, const char* name, const char* value, const char* attribute);
const char* BUPnP_GetIPAddress(void);
uint32_t BUPnP_GetPortNumber(void);
const char* BUPnP_GetDescription(void);
BUPnPConnectionHandle BUPnP_AddConnection(SOCKET s, int32_t timeout, BUPnPReceiveCallback callback, void* args );
void BUPnP_RemoveConnection(BUPnPConnectionHandle hConnection);

/* BUPnPDevice API */
void BUPnPDevice_SetContext(BUPnPDeviceHandle hDevice, void* pContext);
void* BUPnPDevice_GetContext(BUPnPDeviceHandle hDevice);
void BUPnPDevice_AddService(BUPnPDeviceHandle hDevice, BUPnPServiceInfo* pServiceInfo);
BUPnPError BUPnPDevice_AddCustomDescription(BUPnPDeviceHandle hDevice, const char* name, const char* value, const char* attribute);
void BUPnPDevice_SetDeviceType(BUPnPDeviceHandle hDevice, const char* deviceType);
BUPnPDeviceInfo* BUPnPDevice_GetDeviceInfo(BUPnPDeviceHandle hDevice);

/* BUPnPAction API */
BUPnPError BUPnPAction_GetInt32Argument(BUPnPActionHandle hAction, uint32_t index, int32_t* value);
BUPnPError BUPnPAction_GetUInt32Argument(BUPnPActionHandle hAction, uint32_t index, uint32_t* value);
BUPnPError BUPnPAction_GetDoubleArgument(BUPnPActionHandle hAction, uint32_t index, double* value);
BUPnPError BUPnPAction_GetStringArgument(BUPnPActionHandle hAction, uint32_t index, const char** value);
BUPnPError BUPnPAction_SetInt32Argument(BUPnPActionHandle hAction, uint32_t index, int32_t value);
BUPnPError BUPnPAction_SetUInt32Argument(BUPnPActionHandle hAction, uint32_t index, uint32_t value);
BUPnPError BUPnPAction_SetDoubleArgument(BUPnPActionHandle hAction, uint32_t index, double value);
BUPnPError BUPnPAction_SetStringArgument(BUPnPActionHandle hAction, uint32_t index, const char* value);

/* BUPnPStateVariable API */
int32_t BUPnPStateVariable_GetInt32(BUPnPStateVariableHandle hStateVariable); 
uint32_t BUPnPStateVariable_GetUInt32(BUPnPStateVariableHandle hStateVariable); 
const char* BUPnPStateVariable_GetString(BUPnPStateVariableHandle hStateVariable); 
BUPnPError BUPnPStateVariable_SetInt32(BUPnPStateVariableHandle hStateVariable, int32_t value); 
BUPnPError BUPnPStateVariable_SetUInt32(BUPnPStateVariableHandle hStateVariable, uint32_t value); 
BUPnPError BUPnPStateVariable_SetString(BUPnPStateVariableHandle hStateVariable, const char* value); 
BUPnPError BUPnPStateVariable_SetStringN(BUPnPStateVariableHandle hStateVariable, const char* value, int length);
void BUPnPStateVariable_SetModified(BUPnPStateVariableHandle hStateVariable, bool modified);
const char* BUPnPStateVariable_GetName(BUPnPStateVariableHandle hStateVariable);
const char* BUPnPStateVariable_GetValue(BUPnPStateVariableHandle hStateVariable);
bool BUPnPStateVariable_IsModified(BUPnPStateVariableHandle hStateVariable);
unsigned char BUPnPStateVariable_GetAttribute(BUPnPStateVariableHandle hStateVariable);

/* BUPnPService API */
BUPnPStateVariableHandle BUPnPService_GetStateVariable(BUPnPServiceHandle hService, const char* name);
BUPnPStateVariableHandle BUPnPService_GetStateVariableByIndex(BUPnPServiceHandle hService, int index);
BUPnPServiceHandle BUPnPService_GetFromUrl(const char *uri);
void BUPnPService_SetModerateEventCreateCallback(BUPnPServiceHandle hService, BUPnPCreateModerateEvent moderateEventCallback);
void* BUPnPService_GetContext(BUPnPServiceHandle hService);
void BUPnPService_AddAction(BUPnPServiceHandle hService, BUPnPActionHandle hAction);
void BUPnPService_SetModerateNamespace(BUPnPServiceHandle hService, const char* ns);
void BUPnPService_SuspendUpdate(BUPnPServiceHandle hService);
void BUPnPService_ResumeUpdate(BUPnPServiceHandle hService);
BUPnPActionHandle BUPnPService_GetAction(BUPnPServiceHandle hService, const char* name);
BUPnPActionHandle BUPnPAction_Create(BUPnPActionInfo* pActionInfo);
void BUPnPService_AddVariables(BUPnPServiceHandle hService, BUPnPStateVariableInfo** hStateVariableInfo);

/* BUPnPDebug API */
void BUPnPDebug_SetSettings(uint32_t level, uint32_t category);

/* Controlpoint API */
typedef BUPnPIconInfo* BUPnPIconInfoHandle;

typedef void (* BUPnPControlPoint_DeviceRemoved)(void* context, const char* uuid);
typedef void (* BUPnPControlPoint_DeviceAdded)(void* context, const char* uuid, const char* deviceType);
typedef void (* BUPnPControlPoint_PropertyChanged)(void* context, const char* serviceName, const char* variable);

typedef struct BUPnPControlPointOpenSettings
{
    BUPnPControlPoint_DeviceAdded deviceAdded;
    BUPnPControlPoint_DeviceRemoved deviceRemoved;
    BUPnPControlPoint_PropertyChanged propertyChanged;
    ControlPointSearchOption searchOption;
    char *searchType;
    char *userAgent;
    void* context;
} BUPnPControlPointOpenSettings;

void BUPnPControlPoint_GetDefaultOpenSettings(BUPnPControlPointOpenSettings* pOpenSettings);

bool BUPnPControlPoint_CheckDeviceAlive(BUPnPDeviceHandle selectedDevice);

bool BUPnPControlPoint_IsEnabled(void);

BUPnPError BUPnPControlPoint_UnsubscribeDevice(BUPnPRemoteDeviceHandle hDevice);

BUPnPError BUPnPControlPoint_SubscribeDevice(BUPnPRemoteDeviceHandle hDevice);

bool BUPnPControlPoint_CheckServiceExist(const char* uuid, const char* serviceName);

void BUPnPControlPoint_Destroy(void);


/**
* Summary:
*    This function initializes the UPNP Control point. This function should be 
*    call after the main UPNPServer initialize but before BUPnP_Start
* Input: BUPnPControlPointOpenSettings
* Returns:
*    Error code
**/
BUPnPError BUPnPControlPoint_Initialize(BUPnPControlPointOpenSettings* settings);

/**
* Summary:
*     This function issue a broadcast of M-Search. When a device responds, the 
*     application will get a callback. The control point will internally issue 
*     a M-search on system startup
* Input: None
* Returns:
*   Error code
**/
void BUPnPControlPoint_SearchDevices(void);

BUPnPError BUPnPControlPoint_GetIconCount(BUPnPRemoteDeviceHandle hDevice, size_t* count);

BUPnPError BUPnPControlPoint_GetDeviceIcons(BUPnPRemoteDeviceHandle hDevice, BUPnPIconListHandle* pIconList, size_t* count);

BUPnPError BUPnPControlPoint_GetDeviceInfo(BUPnPRemoteDeviceHandle hDevice, BUPnPDeviceInfo* pDeviceInfo);

BUPnPError BUPnPControlPoint_GetDeviceIconInfo(BUPnPRemoteDeviceHandle hDevice, int index, struct BUPnPIconInfo* pIconInfo);

BUPnPError BUPnPControlPoint_GetService(BUPnPRemoteDeviceHandle hDevice, const char* serviceName, struct BUPnPService** ppService);

BUPnPError BUPnPControlPoint_Invoke(BUPnPRemoteDeviceHandle hDevice, BUPnPServiceHandle pService, BUPnPActionHandle hAction);

BUPnPError BUPnPControlPoint_DownloadFile(BUPnPRemoteDeviceHandle hDevice, const char* url, const char* filename);

BUPnPRemoteDeviceHandle BUPnPControlPoint_CreateDevice(const char* uuid);

void BUPnPControlPoint_DestroyDevice(BUPnPRemoteDeviceHandle pDevice);

#ifdef __cplusplus
}
#endif

#endif
