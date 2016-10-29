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
 * $brcm_Workfile: cms.h $
 * $brcm_Revision: 2 $
 * $brcm_Date: 7/24/09 10:43a $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/cms.h $
 * 
 * 2   7/24/09 10:43a ajitabhp
 * PR55165: Reducing the varaibles from CMS.h
 * 
 *****************************************************************************/
#ifndef _CMS_H_
#define _CMS_H_
#include "bcm_types.h"
#include "buffer.h"
#include "trace.h"
//typedef enum { false, true } bool;

//#define SOURCE_PROTOCOL_INFO "http-get:*:audio/mp3:*,http-get:*:video/mpeg:*"
#define SOURCE_PROTOCOL_INFO "http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_SM,http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_MED,http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_LRG,http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_TN,http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_SM_ICO,http-get:*:image/jpeg:DLNA.ORG_PN=JPEG_LRG_ICO,http-get:*:image/png:DLNA.ORG_PN=PNG_TN,http-get:*:image/png:DLNA.ORG_PN=PNG_SM_ICO,http-get:*:image/png:DLNA.ORG_PN=PNG_LRG_ICO,http-get:*:image/png:DLNA.ORG_PN=PNG_LRG,http-get:*:audio/mpeg:DLNA.ORG_PN=MP3,http-get:*:audio/L16:DLNA.ORG_PN=LPCM,http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMABASE,http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMAFULL,http-get:*:audio/x-ms-wma:DLNA.ORG_PN=WMAPRO,http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO,http-get:*:audio/mp4:DLNA.ORG_PN=AAC_ISO_320,http-get:*:audio/vnd.dolby.dd-raw:DLNA.ORG_PN=AC3,http-get:*:audio/3gpp:DLNA.ORG_PN=AAC_ISO,http-get:*:audio/3gpp:DLNA.ORG_PN=AAC_ISO_320,http-get:*:audio/vnd.dlna.adts:DLNA.ORG_PN=AAC_ADTS,http-get:*:audio/vnd.dlna.adts:DLNA.ORG_PN=AAC_ADTS_320,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG1,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_NTSC,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_PS_PAL,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_NA_ISO,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_HD_NA_ISO,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_EU_ISO,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_SD_KO_ISO,http-get:*:video/mpeg:DLNA.ORG_PN=MPEG_TS_HD_KO_ISO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_NA,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_NA_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_NA_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_EU,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_EU_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_KO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_SD_KO_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_KO,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_HD_KO_T,http-get:*:video/vnd.dlna.mpeg-tts:DLNA.ORG_PN=MPEG_TS_JP_T,http-get:*:video/mp4:DLNA.ORG_PN=MPEG4_P2_MP4_SP_AAC,http-get:*:video/mp4:DLNA.ORG_PN=AVC_MP4_BL_CIF15_AAC,http-get:*:video/mp4:DLNA.ORG_PN=AVC_MP4_BL_CIF15_AAC_520,http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVMED_BASE,http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVMED_FULL,http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVMED_PRO,http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVHIGH_FULL,http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVHIGH_PRO,http-get:*:video/x-ms-wmv:DLNA.ORG_PN=WMVHM_BASE,http-get:*:image/gif:*,http-get:*:audio/x-wav:*,http-get:*:audio/lpcm:*,http-get:*:audio/x-aac:*,http-get:*:audio/m4a:*,http-get:*:application/ogg:*,http-get:*:video/avi:*,http-get:*:video/mp1s:*,http-get:*:video/mp2p:*,http-get:*:video/mp2t:*,http-get:*:video/mpeg2:*,http-get:*:video/dvd:*,http-get:*:video/quicktime:*,http-get:*:video/mpeg4:*,http-get:*:video/mp4:*,http-get:*:video/m4t:*,http-get:*:audio/x-mpegurl:*,http-get:*:audio/x-ms-wax:*,http-get:*:audio/x-scpls:*,http-get:*:application/t5:*,http-get:*:video/x-matroska:*"
#define SINK_PROTOCOL_INFO    ""
#define CONNECTION_ID_0 "0"
#define MAX_CONNECTIONS 5
#define EMPTY_STRING    ""
typedef struct _cms_protocolinfo_params_{
    char *SourceProtocolInfo;        /*protocols supported by source*/
    char * SinkProtocolInfo;        /*protocols supported by sink*/
}cms_protocolinfo_params,*pcms_protocolinfo_params;

typedef struct _cms_protocolinfo_out_params_{
    cms_protocolinfo_params protocolinfo_params;
    unsigned int UpdateStateVariables;
    
}cms_protocolinfo_outparams,*pcms_protocolinfo_outparams;


typedef struct _cms_connectioninfo_params_{
    char ConnectionStatus[25];
    char PeerConnectionManager[256];
    char Direction[8];
    char ProtocolInfo[3*1024];
    unsigned int PeerConnectionID;
    int AVTransportID;
    int RcsID;
    unsigned int ConnectionID;
}cms_connectioninfo_params,*pcms_connectioninfo_params;

 
typedef struct _cms_connectioninfo_outparams_{
    cms_connectioninfo_params connectioninfo_params;
    unsigned int UpdateStateVariables;
}cms_connectioninfo_outparams,*pcms_connectioninfo_outparams;

typedef struct _cms_prepconnection_inparams{
    char* rProtocolInfo;
    char* PeerConnectionManager;
    unsigned int PeerConnectionID;
    char* Direction;
}cms_prepconnection_inparams, *pcms_prepconnection_inparams;

typedef struct _cms_prepconnection_outparams{
    unsigned int ConnectionID;
    int        AVTransportID;
    int        RcsID;
}cms_prepconnection_outparams,*pcms_prepconnection_outparams;

typedef struct _cms_prepare_connection_params_{
        cms_prepconnection_inparams inparams;
        cms_prepconnection_outparams outparams;
        BOOL UpdateStateVariables;
}cms_prepare_connection_params,*pcms_prepare_connection_params;



typedef struct _cms_core_state_variables_{
    cms_protocolinfo_params dms_protocol_info;
    char currconnectionIds[10];
    
}cms_core_state_variables, *pcms_core_state_variables;



typedef struct _cms_connection_ctx{
    
    cms_connectioninfo_params   Connection;
    BOOL                    InUse;
    char                    streamingBuf[1024];
    unsigned int            Reserved;
}cms_connection_ctx,*pcms_connection_ctx;

typedef struct _cms_context_{
    unsigned int            NumOfConnections;
    unsigned int            NextConnectionID;
    pcms_connection_ctx        ConnectionListPtrs[MAX_CONNECTIONS];
    pcms_protocolinfo_params pDmsPrototocolInfo;
    char                     *pCurrConnectionIds;    

}cms_context, *pcms_context;





BOOL cms_update_state_variables(pcms_core_state_variables sVars);
BOOL cms_get_protocol_info(pcms_protocolinfo_outparams pProtocolInfo);
BOOL cms_get_current_connection_IDs(char *pId);
BC_STATUS cms_get_current_connection_info( unsigned int connection_id,pcms_connectioninfo_outparams pConInfo);
BC_STATUS cms_prepare_for_connection(pcms_prepare_connection_params pPrepInfo);
BC_STATUS cms_connection_complete(unsigned int connection_id,BOOL *UpdateSvars);
BC_STATUS cms_init();
BC_STATUS cms_cleanup();



#endif


