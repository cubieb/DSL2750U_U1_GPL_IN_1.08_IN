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
 * $brcm_Workfile: upnp_priv.h $
 * $brcm_Revision: 3 $
 * $brcm_Date: 9/30/09 3:27p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/upnp_priv.h $
 * 
 * 3   9/30/09 3:27p dliu
 * SW7405-2482: Add define for GENA_UPDATE_RATE
* 
***************************************************************************/
#ifndef UPNP_PRIV_H__
#define UPNP_PRIV_H__

#include "osl.h"
#include "upnp.h"
#include "list.h"
#include "http.h"
#include "heap.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UPNP_MAX_PACKET_SIZE    2048
#define UPNP_MAX_LINE_SIZE         512
#define UPNP_MAX_URI_SIZE         256
#define UPNP_MAX_DATE_SIZE          64
#define GENA_UPDATE_RATE         200

/* Internal Errors */
#define UPNP_E_IPADDRESS_UNRESOLVED        2001
#define UPNP_E_PARSER                    2002
#define UPNP_E_SOCKET                    2003
#define UPNP_E_SOCKET_CLOSED            2004
#define UPNP_E_BAD_REQUEST                2005
#define UPNP_E_INCOMPLETE                2006
#define UPNP_E_SUBSCRIBE_FAILED            2007
#define UPNP_E_UNSUBSCRIBE_FAILED        2008
#define UPNP_E_SELECT_FAILED            2009

#define UPNP_DEFAULT_MODERATE_TIME        1000

#define UPNP_HEADER "urn:schemas-upnp-org:service:"
#define UPNP_DEVICE_HEADER "urn:schemas-upnp-org:device:"
#define SERVICE_VERSION 1

#define UPNP_GET_ATTRIBUTE(s,a)( (s->attributes & a) == a )

typedef struct BUPnPDeviceList *BUPnPDeviceListHandle;

typedef struct BUPnPConnection
{
    SOCKET socket;
    uint32_t timeout;
    time_t expireTime;
    BUPnPReceiveCallback callback;
    HttpContext context;
    void* args;
    BLST_ENTRY(BUPnPConnection) link;
}BUPnPConnection;

BUPnPDeviceListHandle BUPnP_GetDeviceList(void);

const char* BUPnP_GetRfc1123Date(char *buffer, size_t length);
void BUPnP_RegisterThread(bthread* hThread);


#ifdef __cplusplus
}
#endif

#endif
