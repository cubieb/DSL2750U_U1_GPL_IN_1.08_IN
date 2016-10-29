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
 * $brcm_Workfile: CDS_Iface.c $
 * $brcm_Revision: 3 $
 * $brcm_Date: 7/24/09 10:42a $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/CDS_Iface.c $
 * 
 * 3   7/24/09 10:42a ajitabhp
 * PR55165: Changes For Compiling DMS as library.
 * 
 * 2   6/29/09 12:13p ajitabhp
 * PR55165: Added Path Input From User.
* 
***************************************************************************/
//#include "server.h"

//#include "action.h"

#include "CDS_Iface.h"
#include "trace.h"

char *browse_type_list[] = {CDS_BROWSE_METADATA, CDS_BROWSE_CHILDREN, 0};
static BUPnPStateVariableInfo SearchCapabilities = { "SearchCapabilities", 0, BUPnPType_String, "", {NULL} };
static BUPnPStateVariableInfo SortCapabilities = { "SortCapabilities", 0, BUPnPType_String, "", {NULL} };
static BUPnPStateVariableInfo SystemUpdateID = { "SystemUpdateID", BUPnPAttribute_Evented, BUPnPType_UI4, "0", {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_ObjectID = { "A_ARG_TYPE_ObjectID", 0, BUPnPType_String, "", {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_Result = { "A_ARG_TYPE_Result", BUPnPAttribute_List, BUPnPType_String, "", {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_BrowseFlag = { "A_ARG_TYPE_BrowseFlag", BUPnPAttribute_List, BUPnPType_String, "", {browse_type_list} };
static BUPnPStateVariableInfo A_ARG_TYPE_Filter = { "A_ARG_TYPE_Filter", 0, BUPnPType_String, "", {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_SortCriteria = { "A_ARG_TYPE_SortCriteria", 0, BUPnPType_String, "", {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_Index = { "A_ARG_TYPE_Index", 0, BUPnPType_UI4, "", {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_Count = { "A_ARG_TYPE_Count", 0, BUPnPType_UI4, "", {NULL} };
static BUPnPStateVariableInfo A_ARG_TYPE_UpdateID = { "A_ARG_TYPE_UpdateID", 0, BUPnPType_UI4, "", {NULL} };

static BUPnPStateVariableInfo* CDSStateVariables[] =
{
    &SearchCapabilities,
    &SortCapabilities,
    &SystemUpdateID,
    &A_ARG_TYPE_ObjectID,
    &A_ARG_TYPE_Result,
    &A_ARG_TYPE_BrowseFlag,
    &A_ARG_TYPE_Filter,
    &A_ARG_TYPE_SortCriteria,
    &A_ARG_TYPE_Index,
    &A_ARG_TYPE_Count,
    &A_ARG_TYPE_UpdateID,
    NULL

};

/* Action argument list */
BUPnPArgumentInfo SearchCaps= {"SearchCaps", BUPnPAttribute_Out, &SearchCapabilities};

static BUPnPArgumentInfo *GetSearchCapabilitiesArguments[] = 
{
    &SearchCaps,
    NULL
};

BUPnPArgumentInfo SortCaps = {"SortCaps", BUPnPAttribute_Out, &SortCapabilities};
static BUPnPArgumentInfo *GetSortCapabilitiesArguments[] = 
{
    &SortCaps,
    NULL
};

BUPnPArgumentInfo Id = {"Id", BUPnPAttribute_Out,  &SystemUpdateID};
static BUPnPArgumentInfo *GetSystemUpdateIDArguments[] = 
{
    &Id,
    NULL
};

static BUPnPArgumentInfo ObjectID = {"ObjectID", BUPnPAttribute_In, &A_ARG_TYPE_ObjectID};
static BUPnPArgumentInfo BrowseFlag = {"BrowseFlag", BUPnPAttribute_In, &A_ARG_TYPE_BrowseFlag};
static BUPnPArgumentInfo Filter = {"Filter", BUPnPAttribute_In, &A_ARG_TYPE_Filter};
static BUPnPArgumentInfo StartingIndex = {"StartingIndex", BUPnPAttribute_In, &A_ARG_TYPE_Index};
static BUPnPArgumentInfo RequestedCount = {"RequestedCount", BUPnPAttribute_In, &A_ARG_TYPE_Count};
static BUPnPArgumentInfo SortCriteria = {"SortCriteria", BUPnPAttribute_In, &A_ARG_TYPE_SortCriteria};
static BUPnPArgumentInfo Result = {"Result", BUPnPAttribute_Out, &A_ARG_TYPE_Result};
static BUPnPArgumentInfo NumberReturned = {"NumberReturned", BUPnPAttribute_Out, &A_ARG_TYPE_Count};
static BUPnPArgumentInfo TotalMatches = {"TotalMatches", BUPnPAttribute_Out, &A_ARG_TYPE_Count};
static BUPnPArgumentInfo UpdateID = {"UpdateID", BUPnPAttribute_Out, &A_ARG_TYPE_UpdateID};
static BUPnPArgumentInfo *BrowseArguments[] = 
{
    &ObjectID,
    &BrowseFlag,
    &Filter,
    &StartingIndex,
    &RequestedCount,
    &SortCriteria,
    &Result,
    &NumberReturned,
    &TotalMatches,
    &UpdateID,
    NULL

};
/************************ Search Is not Implemented yet 
static UPnPArgumentInfo SearchArguments[] = 
{
    {"ContainerID", UPnPAttribute_In, &A_ARG_TYPE_ObjectID},
    {"SearchCriteria", UPnPAttribute_In, &A_ARG_TYPE_SearchCriteria},
    {"Filter", UPnPAttribute_In, &A_ARG_TYPE_Filter},
    {"StartingIndex", UPnPAttribute_In, &A_ARG_TYPE_Index},
    {"RequestCount", UPnPAttribute_In, &A_ARG_TYPE_Count},
    {"SortCriteria", UPnPAttribute_In, &A_ARG_TYPE_SortCriteria},
    {"Result", UPnPAttribute_Out, &A_ARG_TYPE_Result},
    {"NuberReturned", UPnPAttribute_Out, &A_ARG_TYPE_Count},
    {"TotalMatches", UPnPAttribute_Out, &A_ARG_TYPE_Count},
    {"UpdateID", UPnPAttribute_Out, &A_ARG_TYPE_UpdateID},
};
*******************************************************/
BUPnPActionInfo GetSearchCapabilities = {"GetSearchCapabilities", cds_get_search_capabilities_if,GetSearchCapabilitiesArguments};
BUPnPActionInfo GetSortCapabilities = {"GetSortCapabilities", cds_get_sort_capabilities_if, GetSortCapabilitiesArguments};
BUPnPActionInfo GetSystemUpdateID = {"GetSystemUpdateID", cds_get_system_updateid_if, GetSystemUpdateIDArguments};
BUPnPActionInfo Browse = {"Browse", cds_browse_if, BrowseArguments};
static BUPnPActionInfo *cdsActions[] =
{
    &GetSearchCapabilities,
    &GetSortCapabilities,
    &GetSystemUpdateID,
    &Browse,
    //{"Search", cds_serach_if, SearchArguments},
    NULL
};

BUPnPServiceInfo Service_ContentDirectory = 
{
    "urn:schemas-upnp-org:service:ContentDirectory:1",    /* serviceType        */
    "urn:upnp-org:serviceId:ContentDirectory",            /* serviceId        */
    "/cds",                                                /* SCPDURL            */
    "/cds_control",                                        /* ControlURL        */
    "/cds_event",                                        /* EventSubURL        */
    cdsActions,                                            /* Actions            */
    CDSStateVariables,                                    /* state variables    */
    };


BUPnPError
cds_browse_if(BUPnPServiceHandle hService, 
              BUPnPActionHandle hAction)
{
    BC_STATUS    ret_sts = BC_UPNP_E_SUCCESS;
    cds_browse_in_params   Inargs={0,NULL,NULL,0,0,NULL};
    pcds_browse_out_params pOut;
    char *ObjectID;
    
    pOut = (pcds_browse_out_params)malloc(sizeof(cds_browse_out_params));

    if(pOut == NULL)
        return UPNP_E_OUT_OF_MEMORY;
    
    if(!(pOut->ResultOutBuff = buffer_new()))
        return UPNP_E_OUT_OF_MEMORY;

    /* Get Arguments */
    BUPnPAction_GetStringArgument(hAction, 0,(const char **) &ObjectID);
    Inargs.ObjectID = strtoul(ObjectID, NULL, 10);;
    BUPnPAction_GetStringArgument(hAction, 1,(const char **) &Inargs.BrowseFlag);
    BUPnPAction_GetStringArgument(hAction, 2,(const char **) &Inargs.Filter);
    BUPnPAction_GetUInt32Argument(hAction, 3, &Inargs.StartingIndex);
    BUPnPAction_GetUInt32Argument(hAction, 4, &Inargs.RequestedCount);
    BUPnPAction_GetStringArgument(hAction, 5,(const char **) &Inargs.SortCriteria);
            
    ret_sts = cds_browse(&Inargs, pOut);
    if(ret_sts == BC_UPNP_E_SUCCESS)
    {
        BUPnPAction_SetStringArgument(hAction, 6, pOut->ResultOutBuff->buf);
        BUPnPAction_SetUInt32Argument(hAction, 7, pOut->NumberReturned);
        BUPnPAction_SetUInt32Argument(hAction, 8, pOut->TotalMatches);
        BUPnPAction_SetUInt32Argument(hAction, 9, pOut->UpdateID);
    }

    buffer_free(pOut->ResultOutBuff);
    free(pOut);    
    return ret_sts;

}

BUPnPError 
cds_get_search_capabilities_if(BUPnPServiceHandle hService,
                                 BUPnPActionHandle hAction)
{
    BC_STATUS    ret_sts = BC_UPNP_E_SUCCESS;
    char caps[256];
    ret_sts = cds_get_search_caps(&caps[0]);
    BUPnPAction_SetStringArgument(hAction,0,caps);
    return ret_sts;
}

BUPnPError 
cds_get_sort_capabilities_if(BUPnPServiceHandle hService, 
                             BUPnPActionHandle hAction)
{
    BC_STATUS    ret_sts = BC_UPNP_E_SUCCESS;
    char caps[256];
    ret_sts = cds_get_sort_caps(&caps[0]);
    BUPnPAction_SetStringArgument(hAction,0,caps);
    return ret_sts;
}

BUPnPError 
cds_get_system_updateid_if(BUPnPServiceHandle hService, 
                         BUPnPActionHandle hAction)
{
    unsigned int id;
    id = cds_get_system_update_id();
    BUPnPAction_SetUInt32Argument(hAction, 0, id);
    return UPNP_E_SUCCESS;
}

/*Do Any Initialization Here*/
void 
cds_service_start_if(char * baseServerUrl,
                     char *ContentDir)
{
    BC_STATUS    ret_sts = BC_UPNP_E_SUCCESS;
    char cds_url[512];
    sprintf(cds_url,"%s%s",baseServerUrl,Service_ContentDirectory.scpdUrl);
    ret_sts = cds_init(cds_url,ContentDir);
    if(ret_sts != BC_UPNP_E_SUCCESS){
        BcmLogMsg("%s:Failed Initialization\n",__FUNCTION__);
    }

    return;
}

/*Do Any Cleanup Here*/
void cds_service_stop_if()
{
    BC_STATUS    ret_sts = BC_UPNP_E_SUCCESS;
    ret_sts = cds_finish();
    if(ret_sts != BC_UPNP_E_SUCCESS){
        BcmLogMsg("%s:Failed Cleanup\n",__FUNCTION__);
    }
    return;
}




