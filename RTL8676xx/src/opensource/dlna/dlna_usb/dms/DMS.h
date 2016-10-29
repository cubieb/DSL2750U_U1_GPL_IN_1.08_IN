/******************************************************************************
 *    (c)2009 Broadcom Corporation
 * 
 * This program is the proprietary software of Broadcom Corporation and/or its licensors,
 * and may only be used, duplicated, modified or distributed pursuant to the terms and
 * conditions of a separate, written license agreement executed between you and Broadcom
 * (an "Authorized License").  Except as set forth in an Authorized License, Broadcom grants
 * no license (express or implied), right to use, or waiver of any kind with respect to the
 * Software, and Broadcom expressly reserves all rights in and to the Software and all
 * intellectual property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
 * HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
 * NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.  
 *  
 * Except as expressly set forth in the Authorized License,
 *  
 * 1.     This program, including its structure, sequence and organization, constitutes the valuable trade
 * secrets of Broadcom, and you shall use all reasonable efforts to protect the confidentiality thereof,
 * and to use this information only in connection with your use of Broadcom integrated circuit products.
 *  
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS" 
 * AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR 
 * WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT TO 
 * THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES 
 * OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, 
 * LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION 
 * OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF 
 * USE OR PERFORMANCE OF THE SOFTWARE.
 * 
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS 
 * LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR 
 * EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO YOUR 
 * USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN ADVISED OF 
 * THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT 
 * ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE 
 * LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF 
 * ANY LIMITED REMEDY.
 *
 * $brcm_Workfile: DMS.h $
 * $brcm_Revision: 3 $
 * $brcm_Date: 9/25/09 3:38p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/DMS.h $
 * 
 * 3   9/25/09 3:38p ismailk
 * SW7405-3080: DMS IP Streaming (Live channel) Feature.
 * 
 * 2   7/24/09 10:44a ajitabhp
 * PR55165: Media Server port number removed.
 * 
 *****************************************************************************/
#ifndef _DMS_H_
#define _DMS_H_

#include "upnp.h"
#include "http_streamer.h"
#include "CDS_Iface.h"
#include "CMS_Iface.h"

#define DMS_MAJOR_VER 1
#define DMS_MINOR_VER 3
#define DMS_BUILD_DATE    __DATE__
#define DMS_BUILD_TIME    __TIME__

//#define MEDIA_SERVER_PORT    2869
#define CONTENT_XFER_PORT    4321

typedef struct _DMS_CONTEXT_{
    //unsigned int        MServerPortNumber;        /* The port number for Our Media Server*/
    unsigned int        ContentXferPortNum;        /* The content Transfer Request comes to a different port*/
    char                ipAddr[64];                /* IP Address is common*/
    //char                MServerbaseurl[50];        /* Base URL http://IPADDR:MServerPortNum*/
    char                ContentXferbaseurl[50]; /* Base URL For With Port*/
    BUPnPDeviceHandle    hRootDev;
    BUPnPConnectionHandle    pHttpStreamerHandle;
}DMS_CONTEXT, *PDMS_CONTEXT;

#endif




