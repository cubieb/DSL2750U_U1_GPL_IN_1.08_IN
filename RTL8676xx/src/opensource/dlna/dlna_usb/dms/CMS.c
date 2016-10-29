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
 * $brcm_Workfile: CMS.c $
 * $brcm_Revision: 2 $
 * $brcm_Date: 7/24/09 10:42a $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/CMS.c $
 * 
 * 2   7/24/09 10:42a ajitabhp
 * PR55165: Reducing the number of Params for CMS_INIT
 * 
 *****************************************************************************/
#include <string.h>
#include <stdio.h>
#include<stdlib.h>
#include "bcm_types.h"
#include "CmsCdsShare.h"
#include "cms.h"

static cms_context *CMS_Context=NULL;

//state variables

static cms_core_state_variables g_cms_core_state_variables={{SOURCE_PROTOCOL_INFO, SINK_PROTOCOL_INFO},""};
                                                            

BOOL cms_allocate_ConnectionID(pcms_connection_ctx    pNewConnectCtx);

BOOL
cms_update_state_variables(pcms_core_state_variables sVars)
{
    if(!sVars){
        BcmLogMsg("CMS:%s:Input arguments pointer is NULL\n",__FUNCTION__);    
        return FALSE;
    }
    //This is for testsing;
    //cmsUtl_strcat(&g_cms_core_state_variables.currconnectionIds[0],",5");

    memcpy(&sVars->dms_protocol_info,CMS_Context->pDmsPrototocolInfo, sizeof(cms_protocolinfo_params));
    cmsUtl_strcpy(sVars->currconnectionIds,CMS_Context->pCurrConnectionIds);
    return TRUE;

}
 
BOOL cms_get_protocol_info(pcms_protocolinfo_outparams pProtocolInfo)
{
    
    if(pProtocolInfo==NULL){
        BcmLogMsg("CMS:%s:Input arguments pointer is NULL\n",__FUNCTION__);    
        return FALSE;
    }

    
    memcpy(pProtocolInfo, &g_cms_core_state_variables.dms_protocol_info,sizeof(g_cms_core_state_variables.dms_protocol_info));

    pProtocolInfo->UpdateStateVariables = 1;

    return TRUE;

}


BOOL cms_get_current_connection_IDs(char *pId)
{
    
    if(pId==NULL){
        BcmLogMsg("CMS:%s:Output parameter pointer is NULL\n",__FUNCTION__);        
        return FALSE;
    }
    if(CMS_Context->NumOfConnections == 0)
        cmsUtl_strcpy(pId,"0");
    else
        cmsUtl_strcpy(pId,CMS_Context->pCurrConnectionIds);
         
    return TRUE;
}


BC_STATUS
cms_get_current_connection_info( unsigned int connection_id,pcms_connectioninfo_outparams pConInfo)
{
    
    if(pConInfo==NULL){
        BcmLogMsg("CMS:%s:Output parameters pointer is NULL\n",__FUNCTION__);        
        return FALSE;
    }
    if((CMS_Context->NumOfConnections == 0)||((CMS_Context->ConnectionListPtrs[connection_id]==NULL)&&(connection_id==0))){
        pConInfo->connectioninfo_params.AVTransportID=-1;
        pConInfo->connectioninfo_params.RcsID =-1;
        pConInfo->connectioninfo_params.ConnectionID=0;
        cmsUtl_strcpy(pConInfo->connectioninfo_params.ConnectionStatus,"Unknown");
        cmsUtl_strcpy(pConInfo->connectioninfo_params.Direction,"Output");
        pConInfo->connectioninfo_params.PeerConnectionID =-1;
        cmsUtl_strcpy(pConInfo->connectioninfo_params.PeerConnectionManager,"");
        cmsUtl_strcpy(pConInfo->connectioninfo_params.ProtocolInfo,"");
        return BC_UPNP_E_SUCCESS;
    }

    if(connection_id>(MAX_CONNECTIONS-1)) //No valid connections
    //actually INAVLID_CONNECTION_REFERENCE 706 needs to be returned. 
    //but that error is not yet defined in the framework. so for now returning 
    //BC_UPNP_E_ARGUMENT_VALUE_INVALID; 
            return BC_UPNP_E_ARGUMENT_VALUE_INVALID; 

    if(CMS_Context->ConnectionListPtrs[connection_id]==NULL)
        return BC_UPNP_E_ARGUMENT_VALUE_INVALID; //INAVLID_CONNECTION_REFERENCE


    //There are some active connection. verify if the in arg is one of them
    pConInfo->UpdateStateVariables =0;
    memcpy(&pConInfo->connectioninfo_params,&CMS_Context->ConnectionListPtrs[connection_id]->Connection,sizeof(cms_connectioninfo_params));
    return BC_UPNP_E_SUCCESS;
            

}

BC_STATUS cms_prepare_for_connection(pcms_prepare_connection_params pPrepInfo)
{

    pcms_connection_ctx        pNewConnectCtx;
    unsigned int i=0;
    char *ret=NULL;    
    if(pPrepInfo==NULL){
        BcmLogMsg("CMS:%s:Output parameters pointer is NULL\n",__FUNCTION__);        
        return FALSE;
    }
    
    // do i need make sure HTTP SERVER is up and running

    //validate input args
    if(strcmp(pPrepInfo->inparams.Direction,"Output")){
        if(!strcmp(pPrepInfo->inparams.Direction,"Input"))
            return 702; //INCOMPATIBLE_DIRECTIONS
        else
            return BC_UPNP_E_INVALID_ARG;
    }

    //if(!(i=_strnicmp(CMS_Context->pDmsPrototocolInfo->SourceProtocolInfo,pPrepInfo->inparams.rProtocolInfo,sizeof(pPrepInfo->inparams.rProtocolInfo))))
    //    return UPNP_E_INVALID_ARG; //INCOMPATIBLE_PROTOCOL_INFO

    if(!cmsUtl_strstr(CMS_Context->pDmsPrototocolInfo->SourceProtocolInfo,pPrepInfo->inparams.rProtocolInfo))
        return UPNP_E_INVALID_ARG;



    if(CMS_Context->NumOfConnections>=MAX_CONNECTIONS){
        BcmLogMsg("CMS:%s:Connection table overflow\n",__FUNCTION__);    
        return BC_UPNP_E_ACTION_FAILED;

        //Actual error code should be CONNECTION_TABLE_OVERFLOW
    
    }
    
    //allocate memory for connection ctx and initialize connection context with ID etc return INTERNAl_MEMORY_RESOURCES_EXCEEDED incase of failure
    pNewConnectCtx = (pcms_connection_ctx)malloc(sizeof(cms_connection_ctx));
    if(!pNewConnectCtx){
        BcmLogMsg("CMS:%s:malloc failed for CMS_CONTEXT\n",__FUNCTION__);
        return BC_UPNP_E_OUT_OF_MEMORY; //INTERNAL_MEM_RES_EXCEEDED
    }

    //allocate connection ID
    if(!cms_allocate_ConnectionID(pNewConnectCtx))
        return BC_UPNP_E_ACTION_FAILED; //may be LOCAL_RESTRICTIONS error
    
    //update NextConnection ID
    for(i=0;i<MAX_CONNECTIONS;i++){
        if(CMS_Context->ConnectionListPtrs[i]==NULL){
            CMS_Context->NextConnectionID =i;
            break;
        }

    }
    

    //update connection with input params

    ret = cmsUtl_strcpy(pNewConnectCtx->Connection.ConnectionStatus,(const char *)"unknown");
    cmsUtl_strcpy(pNewConnectCtx->Connection.ProtocolInfo,pPrepInfo->inparams.rProtocolInfo);
    cmsUtl_strcpy(pNewConnectCtx->Connection.PeerConnectionManager,pPrepInfo->inparams.PeerConnectionManager);
    cmsUtl_strcpy(pNewConnectCtx->Connection.Direction,pPrepInfo->inparams.Direction);
    pNewConnectCtx->Connection.PeerConnectionID=pPrepInfo->inparams.PeerConnectionID;
    
    //populate outparams
    pPrepInfo->outparams.ConnectionID = pNewConnectCtx->Connection.ConnectionID;
    pPrepInfo->outparams.AVTransportID = pNewConnectCtx->Connection.AVTransportID =-1;
    pPrepInfo->outparams.RcsID =     pNewConnectCtx->Connection.RcsID = -1;


    

    //update current connection ids csv list, NumOfConnections and connectionlistPtrs in the Ctx 
    if(!strcmp(CMS_Context->pCurrConnectionIds,""))
        sprintf(CMS_Context->pCurrConnectionIds, "0,%d",pNewConnectCtx->Connection.ConnectionID);
    else
        sprintf(CMS_Context->pCurrConnectionIds, "%s,%d",CMS_Context->pCurrConnectionIds,pNewConnectCtx->Connection.ConnectionID);

    pPrepInfo->UpdateStateVariables = TRUE;
    return BC_UPNP_E_SUCCESS;
}





BC_STATUS cms_connection_complete(unsigned int connection_id, BOOL *UpdateSVars)
{
    BC_STATUS sts = BC_UPNP_E_SUCCESS ;
    unsigned int i=0;
    //validate input args
    if((CMS_Context->NumOfConnections == 0)||(connection_id>(MAX_CONNECTIONS-1))) //No valid connections
    //actually INAVLID_CONNECTION_REFERENCE 706 needs to be returned. 
    //but that error is not yet defined in the framework. so for now returning 
    //BC_UPNP_E_ARGUMENT_VALUE_INVALID; 
            return BC_UPNP_E_ARGUMENT_VALUE_INVALID; 

    if(CMS_Context->ConnectionListPtrs[connection_id]==NULL)
        return BC_UPNP_E_ARGUMENT_VALUE_INVALID; //INAVLID_CONNECTION_REFERENCE

    //update current connection ids csv list, NumOfConnections and connectionlistPtrs in the Ctx 
    CMS_Context->NumOfConnections--;
    free(CMS_Context->ConnectionListPtrs[connection_id]);
    CMS_Context->ConnectionListPtrs[connection_id]=NULL;
    
    //updatecsv

    
   sprintf(CMS_Context->pCurrConnectionIds,"%s","");

    for(i=0;i<MAX_CONNECTIONS;i++){
        if(CMS_Context->ConnectionListPtrs[i]){
            if(!strcmp(CMS_Context->pCurrConnectionIds,""))
                sprintf(CMS_Context->pCurrConnectionIds, "0,%d",i);
            else
                sprintf(CMS_Context->pCurrConnectionIds, "%s,%d",CMS_Context->pCurrConnectionIds,i);
        }
    }
#if 0
    if(CMS_Context->ConnectionListPtrs[CMS_Context->NextConnectionID])
            CMS_Context->NextConnectionID = connection_id;
#endif
    *UpdateSVars = TRUE;
    
    return sts;

}


BOOL cms_allocate_ConnectionID(pcms_connection_ctx    pNewConnectCtx)
{
    unsigned int i=0;

    for(i=0;i<MAX_CONNECTIONS;i++){
        if((CMS_Context->ConnectionListPtrs[i]==NULL) && (CMS_Context->NextConnectionID ==i)){
            pNewConnectCtx->Connection.ConnectionID = i;
            pNewConnectCtx->InUse =1;
            CMS_Context->ConnectionListPtrs[i]=pNewConnectCtx;
            CMS_Context->NumOfConnections++;
#if 0            
            if(i<4)
                CMS_Context->NextConnectionID = i+1;
            else
                CMS_Context->NextConnectionID = 0;
            
#endif
            return TRUE;
        }
    }

    return FALSE;
}


BC_STATUS
cms_init()
{
    cms_context *Ctx=NULL;
    char *pstr;

    pstr = (char*)malloc(16);
    Ctx = (pcms_context)malloc(sizeof(cms_context));
    if(!Ctx){
        BcmLogMsg("CMS:%s:malloc failed for CMS_CONTEXT\n",__FUNCTION__);
        return BC_UPNP_E_OUT_OF_MEMORY;
    }
    
    memset(Ctx,0,sizeof(cms_context));
    //Initialize context 
    Ctx->pDmsPrototocolInfo = &g_cms_core_state_variables.dms_protocol_info;
    Ctx->NumOfConnections =0;
    cmsUtl_strcpy(pstr,g_cms_core_state_variables.currconnectionIds);
    Ctx->pCurrConnectionIds = pstr ;
    CMS_Context=Ctx;
    return BC_UPNP_E_SUCCESS;
}

BC_STATUS
cms_cleanup()
{
    if(CMS_Context)
        free(CMS_Context);

    return BC_UPNP_E_SUCCESS;
}

/************************************************/
/* Functions exposed by CMS for use by CDS        */
/************************************************/
void CmsUpdateSrcProtocolInfo(char * SrcProtocolInfo)
{

    //cmsUtl_strcpy(g_cms_core_state_variables.dms_protocol_info.SourceProtocolInfo,SrcProtocolInfo);
    /*TO DO This is Just A Hack...Need to clean it up later*/
    g_cms_core_state_variables.dms_protocol_info.SourceProtocolInfo = SrcProtocolInfo;
}







