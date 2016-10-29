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
 * $brcm_Workfile: CMS_Iface.c $
 * $brcm_Revision: 2 $
 * $brcm_Date: 7/24/09 10:43a $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/CMS_Iface.c $
 * 
 * 2   7/24/09 10:43a ajitabhp
 * PR55165: Reducing the number of parameters for CMS
* 
***************************************************************************/
#include "CMS_Iface.h"
#include "trace.h"

//BOOL gExitFlag = FALSE;
/* State Variable Info */
//BUPnPAllowedValueList connection_status_list = {"OK", "ContentFormatMismatch", "InsufficientBandwidth", "UnrelaibleChannel", "Unknown", 0};
BUPnPAllowedValueList connection_status_list = {"OK", "ContentFormatMismatch", "InsufficientBandwidth", "UnreliableChannel", "Unknown", 0};
char *direction_list[] = {"Input", "Output", 0};



static BUPnPStateVariableInfo SourceProtocolInfo = { "SourceProtocolInfo", BUPnPAttribute_Evented , BUPnPType_String, SOURCE_PROTOCOL_INFO, {NULL} };
static BUPnPStateVariableInfo SinkProtocolInfo = { "SinkProtocolInfo", BUPnPAttribute_Evented , BUPnPType_String, SINK_PROTOCOL_INFO, {NULL} };
static BUPnPStateVariableInfo CurrentConnectionIDs = { "CurrentConnectionIDs", BUPnPAttribute_Evented , BUPnPType_String, CONNECTION_ID_0, {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_ConnectionStatus = { "A_ARG_TYPE_ConnectionStatus", BUPnPAttribute_List, BUPnPType_String, "", {connection_status_list} };
static BUPnPStateVariableInfo A_ARG_TYPE_ConnectionManager = { "A_ARG_TYPE_ConnectionManager", 0, BUPnPType_String, "", {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_Direction = { "A_ARG_TYPE_Direction", BUPnPAttribute_List, BUPnPType_String, "", {direction_list} };
static BUPnPStateVariableInfo A_ARG_TYPE_ProtocolInfo = { "A_ARG_TYPE_ProtocolInfo", 0, BUPnPType_String, "", {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_ConnectionID = { "A_ARG_TYPE_ConnectionID", 0, BUPnPType_I4, "", {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_AVTransportID = { "A_ARG_TYPE_AVTransportID", 0, BUPnPType_I4, "", {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_RcsID = { "A_ARG_TYPE_RcsID", 0, BUPnPType_I4, "", {NULL} };

static BUPnPStateVariableInfo* CMSStateVariables[] =
{
    &SourceProtocolInfo,
    &SinkProtocolInfo,
    &CurrentConnectionIDs,
    &A_ARG_TYPE_ConnectionStatus,
    &A_ARG_TYPE_ConnectionManager,
    &A_ARG_TYPE_Direction,
    &A_ARG_TYPE_ProtocolInfo,
    &A_ARG_TYPE_ConnectionID,
    &A_ARG_TYPE_AVTransportID,
    &A_ARG_TYPE_RcsID,
    NULL
};

/* Action argument list */
BUPnPArgumentInfo Source = {"Source", BUPnPAttribute_Out, &SourceProtocolInfo};
BUPnPArgumentInfo Sink = {"Sink", BUPnPAttribute_Out, &SinkProtocolInfo};

static BUPnPArgumentInfo *GetProtocolInfoArguments[] = 
{
    &Source,
    &Sink,
    NULL
};

BUPnPArgumentInfo ConnectionIDs = {"ConnectionIDs", BUPnPAttribute_Out, &CurrentConnectionIDs};
static BUPnPArgumentInfo *GetCurrentConnectionIDArgument[] = 
{
    &ConnectionIDs,
    NULL
};


BUPnPArgumentInfo ConnectionID = {"ConnectionID", BUPnPAttribute_In, &CurrentConnectionIDs};
BUPnPArgumentInfo RcsID ={"RcsID", BUPnPAttribute_Out, &A_ARG_TYPE_RcsID};
BUPnPArgumentInfo AVTransportID = {"AVTransportID", BUPnPAttribute_Out, &A_ARG_TYPE_AVTransportID};
BUPnPArgumentInfo ProtocolInfo = {"ProtocolInfo", BUPnPAttribute_Out, &A_ARG_TYPE_ProtocolInfo};
BUPnPArgumentInfo PeerConnectionManager = {"PeerConnectionManager", BUPnPAttribute_Out, &A_ARG_TYPE_ConnectionManager};
BUPnPArgumentInfo PeerConnectionID = {"PeerConnectionID", BUPnPAttribute_Out, &A_ARG_TYPE_ConnectionID};
BUPnPArgumentInfo Direction = {"Direction", BUPnPAttribute_Out, &A_ARG_TYPE_Direction};
BUPnPArgumentInfo Status = {"Status", BUPnPAttribute_Out, &A_ARG_TYPE_ConnectionStatus};
static BUPnPArgumentInfo *GetCurrentConnectionInfoArguments[] = 
{
    &ConnectionID,
    &RcsID,
    &AVTransportID,
    &ProtocolInfo,
    &PeerConnectionManager,
    &PeerConnectionID,
    &Direction,
    &Status,
    NULL
};

BUPnPArgumentInfo ConnectionIDOut = {"ConnectionID", BUPnPAttribute_Out, &A_ARG_TYPE_ConnectionID};
//BUPnPArgumentInfo ProtocolInfo = {"ProtocolInfo", BUPnPAttribute_Out, &A_ARG_TYPE_ProtocolInfo};
BUPnPArgumentInfo PeerConnectionManagerIn = {"PeerConnectionManager", BUPnPAttribute_In, &A_ARG_TYPE_ConnectionManager};
BUPnPArgumentInfo PeerConnectionIDIn = {"PeerConnectionID", BUPnPAttribute_In, &A_ARG_TYPE_ConnectionID};
BUPnPArgumentInfo DirectionIn = {"Direction", BUPnPAttribute_In, &A_ARG_TYPE_Direction};
BUPnPArgumentInfo RemoteProtocolInfo = {"RemoteProtocolInfo", BUPnPAttribute_In, &A_ARG_TYPE_ProtocolInfo};
static BUPnPArgumentInfo *GetCurrentPrepForconnectionArguments[] = 
{
    &RemoteProtocolInfo,
    &PeerConnectionManagerIn,
    &PeerConnectionIDIn,
    &DirectionIn,
    &ConnectionIDOut,
    &AVTransportID,
    &RcsID,
    NULL
};


static BUPnPArgumentInfo *GetConnectionCompleteArgument[] = 
{
    &ConnectionID,
    NULL
};

BUPnPActionInfo GetProtocolInfo ={"GetProtocolInfo", cms_get_protocol_info_if, GetProtocolInfoArguments};
BUPnPActionInfo GetCurrentConnectionIDs ={"GetCurrentConnectionIDs", cms_get_current_connection_IDs_if, GetCurrentConnectionIDArgument};
BUPnPActionInfo GetCurrentConnectionInfo = {"GetCurrentConnectionInfo", cms_get_current_connection_info_if, GetCurrentConnectionInfoArguments};
BUPnPActionInfo PrepareForConnection = {"PrepareForConnection", cms_prepare_for_connection_if, GetCurrentPrepForconnectionArguments};
BUPnPActionInfo ConnectionComplete = {"ConnectionComplete", cms_connection_complete_if, GetConnectionCompleteArgument};

static BUPnPActionInfo *CMSActions[] =
{
    &GetProtocolInfo,
    &GetCurrentConnectionIDs,
    &GetCurrentConnectionInfo,
    /*&PrepareForConnection,
    &ConnectionComplete,*/
    NULL
};

BUPnPServiceInfo Service_ConnectionManager = 
{
    "urn:schemas-upnp-org:service:ConnectionManager:1",    /* serviceType        */
    "urn:upnp-org:serviceId:ConnectionManager",            /* serviceId        */
    "/cms",                                        /* SCPDURL            */
    "/cms_control",                                        /* ControlURL        */
    "/cms_event",                                        /* EventSubURL        */
    CMSActions,                                        /* Actions            */
    CMSStateVariables,                                    /* state variables    */
    
};


static bool cms_update_state_variable(BUPnPServiceHandle hService)
{
    pcms_core_state_variables svars;
    BUPnPStateVariableHandle hSourceProtocolInfo;
    BUPnPStateVariableHandle hSinkProtocolInfo;
    BUPnPStateVariableHandle hCurreConnectionIDS;

    if(!hService){
        BcmLogMsg("CMS_Iface:%s:Sevice handle is NULL\n",__FUNCTION__);        
        return FALSE;
    }
    svars = (pcms_core_state_variables) malloc(sizeof(cms_core_state_variables));
    
    if(svars == NULL){
        BcmLogMsg("CMS_Iface:%s:Malloc failed\n",__FUNCTION__);    
        return FALSE;
    }
    
    if(!cms_update_state_variables(svars)){
        BcmLogMsg("CMS_Iface:%s:the Core function, cms_update_state_variables() returned false\n",__FUNCTION__);        
        return FALSE;
    }

    //The Frameworks sends event notifications when UPnPStateVariable_Set is called
    
    hSourceProtocolInfo = BUPnPService_GetStateVariable(hService, "SourceProtocolInfo");
    BUPnPStateVariable_SetString(hSourceProtocolInfo,svars->dms_protocol_info.SourceProtocolInfo);

    hSinkProtocolInfo = BUPnPService_GetStateVariable(hService, "SinkProtocolInfo");
    BUPnPStateVariable_SetString(hSinkProtocolInfo,svars->dms_protocol_info.SinkProtocolInfo);
    
    hCurreConnectionIDS = BUPnPService_GetStateVariable(hService, "CurrentConnectionIDs");
    BUPnPStateVariable_SetString(hCurreConnectionIDS,svars->currconnectionIds);

    return TRUE;

    
}

BUPnPError cms_get_protocol_info_if(BUPnPServiceHandle hService, BUPnPActionHandle hAction)
{
    pcms_protocolinfo_outparams pAct;
    
    pAct = (pcms_protocolinfo_outparams) malloc (sizeof(cms_protocolinfo_outparams));

    if(pAct==NULL){
        BcmLogMsg("CMS_Iface:%s:Malloc failed\n",__FUNCTION__);            
        return UPNP_E_OUT_OF_MEMORY;
    }
    
    if(!cms_get_protocol_info(pAct)){
        BcmLogMsg("CMS_Iface:%s:the Core function, cms_get_protocol_info() returned false\n",__FUNCTION__);        
        free(pAct);    
        return UPNP_E_ACTION_FAILED;
    }

    BUPnPAction_SetStringArgument(hAction, 0, pAct->protocolinfo_params.SourceProtocolInfo);
    BUPnPAction_SetStringArgument(hAction, 1, pAct->protocolinfo_params.SinkProtocolInfo);
    
    if(pAct->UpdateStateVariables)
    {
        BcmLogMsg("CMS_Iface:%s:Update State Variables\n",__FUNCTION__);        
        cms_update_state_variable(hService);

    }

    free(pAct);    
    return UPNP_E_SUCCESS;
}
    

BUPnPError cms_get_current_connection_IDs_if(BUPnPServiceHandle hService, BUPnPActionHandle hAction)
{
    char *conID;
    UNUSED_ARGUMENT(hService);
        
    conID=(char*)malloc(10*sizeof(char));
    if(conID==NULL){
        BcmLogMsg("CMS_Iface:%s:Malloc failed\n",__FUNCTION__);    
        return UPNP_E_OUT_OF_MEMORY;
    }

    if(!cms_get_current_connection_IDs(conID)){
        BcmLogMsg("CMS_Iface:%s:the Core function returned false\n",__FUNCTION__);        
        free(conID);    
        return UPNP_E_ACTION_FAILED;
    }
    BUPnPAction_SetStringArgument(hAction, 0, conID);
    free(conID);
    return UPNP_E_SUCCESS;
}

BUPnPError cms_get_current_connection_info_if(BUPnPServiceHandle hService, BUPnPActionHandle hAction)
{
    BC_STATUS sts = BC_UPNP_E_SUCCESS;
    pcms_connectioninfo_outparams conInfo;
    int conid;
    conInfo = (pcms_connectioninfo_outparams ) malloc (sizeof(cms_connectioninfo_outparams ));
    memset(conInfo,0,(sizeof(cms_connectioninfo_outparams )));
    if(conInfo==NULL){
        BcmLogMsg("CMS_Iface:%s:Malloc failed\n",__FUNCTION__);        
        return UPNP_E_OUT_OF_MEMORY;
    }

    BUPnPAction_GetInt32Argument(hAction, 0, &conid);

    sts = cms_get_current_connection_info(conid,conInfo);
    if(sts!=BC_UPNP_E_SUCCESS){
        BcmLogMsg("CMS_Iface:%s:Core function returned False\n",__FUNCTION__);    
        free(conInfo);    
        return sts;
    }
    
     
    BUPnPAction_SetInt32Argument(hAction, 1, conInfo->connectioninfo_params.RcsID);
    BUPnPAction_SetInt32Argument(hAction, 2, conInfo->connectioninfo_params.AVTransportID);
    BUPnPAction_SetStringArgument(hAction, 3,conInfo->connectioninfo_params.ProtocolInfo);
    BUPnPAction_SetStringArgument(hAction, 4,conInfo->connectioninfo_params.PeerConnectionManager);
    BUPnPAction_SetInt32Argument(hAction,5, conInfo->connectioninfo_params.PeerConnectionID);
    BUPnPAction_SetStringArgument(hAction, 6,conInfo->connectioninfo_params.Direction);
    BUPnPAction_SetStringArgument(hAction, 7,conInfo->connectioninfo_params.ConnectionStatus);

    if(conInfo->UpdateStateVariables)
    {
        BcmLogMsg("CMS_Iface:%s:Update State Variables\n",__FUNCTION__);        
        cms_update_state_variable(hService);
    
    }

    free(conInfo);    
    return sts;
}

BUPnPError cms_prepare_for_connection_if(BUPnPServiceHandle hService, BUPnPActionHandle hAction)
{
    BC_STATUS sts=BC_UPNP_E_SUCCESS;
    pcms_prepare_connection_params pPrepinfo;
        
    pPrepinfo = (pcms_prepare_connection_params ) malloc (sizeof(cms_prepare_connection_params));

    if(pPrepinfo==NULL){
        BcmLogMsg("CMS_Iface:%s:Malloc failed\n",__FUNCTION__);        
        return UPNP_E_OUT_OF_MEMORY;
    }

    BUPnPAction_GetStringArgument(hAction, 0,(const char **) &pPrepinfo->inparams.rProtocolInfo);
    BUPnPAction_GetStringArgument(hAction, 1,(const char **)  &pPrepinfo->inparams.PeerConnectionManager);
    BUPnPAction_GetUInt32Argument(hAction, 2, &pPrepinfo->inparams.PeerConnectionID);
    BUPnPAction_GetStringArgument(hAction, 3,(const char **)  &pPrepinfo->inparams.Direction);
    
    sts = cms_prepare_for_connection(pPrepinfo);
    if(BC_UPNP_E_SUCCESS!=sts ){
        BcmLogMsg("CMS_Iface:%s:Core function returned False\n",__FUNCTION__);    
        free(pPrepinfo);    
        return sts;
    }
    
     
    BUPnPAction_SetInt32Argument(hAction, 6, pPrepinfo->outparams.RcsID);
    BUPnPAction_SetInt32Argument(hAction, 5, pPrepinfo->outparams.AVTransportID);
    BUPnPAction_SetUInt32Argument(hAction,4, pPrepinfo->outparams.ConnectionID);
   

    if(pPrepinfo->UpdateStateVariables)
    {
        BcmLogMsg("CMS_Iface:%s:Update State Variables\n",__FUNCTION__);        
        cms_update_state_variable(hService);
    
    }

    free(pPrepinfo);    
    return sts;


}


BUPnPError cms_connection_complete_if(BUPnPServiceHandle hService, BUPnPActionHandle hAction)
{
    BC_STATUS sts=BC_UPNP_E_SUCCESS;
    unsigned int conid;
    BOOL UpdateSvars = FALSE;
    BUPnPAction_GetUInt32Argument(hAction, 0, &conid);

    sts = cms_connection_complete(conid,&UpdateSvars);
    if(sts!=BC_UPNP_E_SUCCESS){
        BcmLogMsg("CMS_Iface:%s:Core function returned False\n",__FUNCTION__);    
        return sts;
    }
    
    if(UpdateSvars)
    {
        BcmLogMsg("CMS_Iface:%s:Update State Variables\n",__FUNCTION__);        
        cms_update_state_variable(hService);
    
    }

    return sts;
}


/*Do Any initialization here*/
void cms_service_start_if()
{
    BC_STATUS    ret_sts = BC_UPNP_E_SUCCESS;
    //char cms_url[512]; 
    //sprintf(cms_url,"%s%s",baseServerUrl,Service_ConnectionManager.scpdUrl);

    ret_sts = cms_init();
    if(ret_sts != BC_UPNP_E_SUCCESS){
        BcmLogMsg("%s:Failed Initialization\n",__FUNCTION__);
    }
    
}

/*Do Any Cleanup Here*/
void cms_service_stop_if()
{
    
    BC_STATUS    ret_sts = BC_UPNP_E_SUCCESS;
    ret_sts = cms_cleanup();
    if(ret_sts != BC_UPNP_E_SUCCESS){
        BcmLogMsg("%s:Failed Cleanup\n",__FUNCTION__);
    }
    return;
}





//}





