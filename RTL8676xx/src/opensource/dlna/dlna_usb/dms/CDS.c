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
 * $brcm_Workfile: CDS.c $
 * $brcm_Revision: 5 $
 * $brcm_Date: 9/25/09 3:37p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/CDS.c $
 * 
 * 5   9/25/09 3:37p ismailk
 * SW7405-3080: DMS IP Streaming (Live channel) Feature.
 * 
 * 4   7/25/09 7:16p ajitabhp
 * PR55165: Variable arguments problem.
 * 
 * 3   7/25/09 7:06p ajitabhp
 * PR55165: Variable arguments was not processed properly.
 * 
 * 2   6/29/09 12:12p ajitabhp
 * PR55165: Added Path Input From User.
 * 
 *****************************************************************************/

#include "CmsCdsShare.h"
#include "CDS.h"
#include "trace.h"

static void * virdir_context=NULL;

/* DIDL parameters */
/* Represent the CDS DIDL Message Header Namespace. */
#define DIDL_NAMESPACE \
    "xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\" " \
    "xmlns:dc=\"http://purl.org/dc/elements/1.1/\" " \
    "xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\""

/* Represent the CDS DIDL Message Header Tag. */
#define DIDL_LITE "DIDL-Lite"

/* Represent the CDS DIDL Message Item value. */
#define DIDL_ITEM "item"

/* Represent the CDS DIDL Message Item ID value. */
#define DIDL_ITEM_ID "id"

/* Represent the CDS DIDL Message Item Parent ID value. */
#define DIDL_ITEM_PARENT_ID "parentID"

/* Represent the CDS DIDL Message Item Restricted value. */
#define DIDL_ITEM_RESTRICTED "restricted"

/* Represent the CDS DIDL Message Item UPnP Class value. */
#define DIDL_ITEM_CLASS "upnp:class"

/* Represent the CDS DIDL Message Item Title value. */
#define DIDL_ITEM_TITLE "dc:title"

/* Represent the CDS DIDL Message broadcast item channel number. */
#define DIDL_ITEM_CHANNEL_NUMBER "upnp:channelNr"

/* Represent the CDS DIDL Message broadcast item channel name. */
#define DIDL_ITEM_CHANNEL_NAME "upnp:channelName"


/* Represent the CDS DIDL Message Item Resource value. */
#define DIDL_RES "res"

/* Represent the CDS DIDL Message Item Protocol Info value. */
#define DIDL_RES_INFO "protocolInfo"

/* Represent the CDS DIDL Message Item Resource Size value. */
#define DIDL_RES_SIZE "size"

/* Represent the CDS DIDL Message Container value. */
#define DIDL_CONTAINER "container"

/* Represent the CDS DIDL Message Container ID value. */
#define DIDL_CONTAINER_ID "id"

/* Represent the CDS DIDL Message Container Parent ID value. */
#define DIDL_CONTAINER_PARENT_ID "parentID"

/* Represent the CDS DIDL Message Container number of children value. */
#define DIDL_CONTAINER_CHILDS "childCount"

/* Represent the CDS DIDL Message Container Restricted value. */
#define DIDL_CONTAINER_RESTRICTED "restricted"

/* Represent the CDS DIDL Message Container Searchable value. */
#define DIDL_CONTAINER_SEARCH "searchable"

/* Represent the CDS DIDL Message Container UPnP Class value. */
#define DIDL_CONTAINER_CLASS "upnp:class"

/* Represent the CDS DIDL Message for Container type value*/
#define DIDL_CONTAINER_TYPE "dlna:containerType"


/* Represent the CDS DIDL Message Container Title value. */
#define DIDL_CONTAINER_TITLE "dc:title"

/*Escaped XML Characters*/
#define GT    "&gt;"
#define LT    "&lt;"


static 
BOOL
filter_has_val (const char *filter, 
                const char *val)
{
    char *x = NULL, *token = NULL;
    char *m_buffer = NULL, *buffer;
    int len = cmsUtl_strlen (val);
    BOOL ret = FALSE;

    if (!strcmp (filter, "*"))
        return TRUE;

    x = strdup (filter);
    if (x)
    {
        m_buffer = (char*) malloc (cmsUtl_strlen (x));
        if (m_buffer)
        {
            buffer = m_buffer;
            token = strtok_r(x, ",", &buffer);
            while (token)
            {
                if (*val == '@')
                    token = strchr (token, '@');
                if (token && !strncmp (token, val, len))
                {
                    ret = TRUE;
                    break;
                }
                token = strtok_r (NULL, ",", &buffer);
            }
            free (m_buffer);
        }
        free (x);
    }
    return ret;
}


static void
didl_add_header (struct buffer_t *out)
{
    buffer_appendf (out, "%s%s %s%s", LT,DIDL_LITE, DIDL_NAMESPACE,GT);
}

static void
didl_add_footer (struct buffer_t *out)
{
    buffer_appendf (out, "%s/%s%s",LT,DIDL_LITE,GT);
}

static void
didl_add_tag (struct buffer_t *out, 
              char *tag, 
              char *value)
{
    char *value_utf8;
    if (value)
        //buffer_appendf (out, "<%s>%s</%s>",tag,value,tag);
        value_utf8=EncodeToUTF8(value);
        buffer_appendf (out, "%s%s%s%s%s/%s%s",LT,tag,GT,value_utf8,LT,tag,GT);
}

static void
didl_add_tag_value (struct buffer_t *out, 
              char *tag, 
              int value)
{
    if (value)
        //buffer_appendf (out, "<%s>%s</%s>",tag,value,tag);
        buffer_appendf (out, "%s%s%s%d%s/%s%s",LT,tag,GT,value,LT,tag,GT);
}


static void
didl_add_param (struct buffer_t *out, 
                char *param, 
                char *value)
{
    if (value)
        buffer_appendf (out, " %s=\"%s\"", param, value);
}

static void
didl_add_value (struct buffer_t *out, 
                char *param, 
                int value)
{
    buffer_appendf (out, " %s=\"%d\"", param, value);
}

static void
didl_add_item (struct buffer_t *out, 
               int item_id,
               int parent_id, 
               char *restricted, 
               char *class, 
               char *title,
               char *protocol_info, 
               int size, 
               char *url, 
               char *filter)
{
    buffer_appendf (out, "%s%s",LT, DIDL_ITEM);
    didl_add_value (out, DIDL_ITEM_ID, item_id);
    didl_add_value (out, DIDL_ITEM_PARENT_ID, parent_id);
    didl_add_param (out, DIDL_ITEM_RESTRICTED, restricted);
    buffer_appendf (out, "%s",GT);

    didl_add_tag (out, DIDL_ITEM_CLASS, class);
    didl_add_tag (out, DIDL_ITEM_TITLE, title);

    if (filter_has_val (filter, DIDL_RES))
    {
        buffer_appendf (out, "%s%s",LT,DIDL_RES);
        // protocolInfo is required :
        didl_add_param (out, DIDL_RES_INFO, protocol_info);
        if (filter_has_val (filter, "@"DIDL_RES_SIZE))
            didl_add_value (out, DIDL_RES_SIZE, size);
        buffer_appendf (out, "%s",GT);
        if (url)
        {
            buffer_appendf(out,"%s",url);
        }
        buffer_appendf (out, "%s/%s%s",LT,DIDL_RES,GT);
    }
    

    buffer_appendf (out, "%s/%s%s",LT,DIDL_ITEM,GT);
}

static void
didl_add_broadcast_item (struct buffer_t *out, 
               int item_id,
               int parent_id, 
               char *restricted, 
               char *class, 
               char *title,
               char *protocol_info,                
               char *url,
               unsigned int channel_number,
               char* channel_name,
               char *filter               
               )
{
    buffer_appendf (out, "%s%s",LT, DIDL_ITEM);
    didl_add_value (out, DIDL_ITEM_ID, item_id);
    didl_add_value (out, DIDL_ITEM_PARENT_ID, parent_id);
    didl_add_param (out, DIDL_ITEM_RESTRICTED, restricted);
    buffer_appendf (out, "%s",GT);

    didl_add_tag (out, DIDL_ITEM_CLASS, class);
    didl_add_tag (out, DIDL_ITEM_TITLE, title);
    didl_add_tag_value(out, DIDL_ITEM_CHANNEL_NUMBER, channel_number);
    didl_add_tag (out, DIDL_ITEM_CHANNEL_NAME, channel_name);

    if (filter_has_val (filter, DIDL_RES))
    {
        buffer_appendf (out, "%s%s",LT,DIDL_RES);
        // protocolInfo is required :
        didl_add_param (out, DIDL_RES_INFO, protocol_info);
        buffer_appendf (out, "%s",GT);
        if (url)
        {
            buffer_appendf(out,"%s",url);
        }
        buffer_appendf (out, "%s/%s%s",LT,DIDL_RES,GT);
    }
    

    buffer_appendf (out, "%s/%s%s",LT,DIDL_ITEM,GT);
    //printf("didl_add_broadcast_item xml = %s\n",out->buf);
}


static void
didl_add_container (struct buffer_t *out, int id, int parent_id,
                    int child_count, char *restricted, char *searchable,
                    char *title, char *class, char *filter)
{

    buffer_appendf (out, "%s%s",LT,DIDL_CONTAINER);
    didl_add_value (out, DIDL_CONTAINER_ID, id);
    didl_add_value (out, DIDL_CONTAINER_PARENT_ID, parent_id);

    if( (child_count >= 0) && (filter_has_val(filter,"@"DIDL_CONTAINER_CHILDS)))
        didl_add_value (out, DIDL_CONTAINER_CHILDS, child_count);


    didl_add_param (out, DIDL_CONTAINER_RESTRICTED, restricted);
    didl_add_param (out, DIDL_CONTAINER_SEARCH, searchable);
    buffer_appendf (out, "%s",GT);
    
    if(id >= TUNER_CONTAINER_ID)
    {
        didl_add_tag (out, DIDL_CONTAINER_TYPE, "Tuner_1_0");
    }

    didl_add_tag (out, DIDL_CONTAINER_CLASS, class);
    didl_add_tag (out, DIDL_CONTAINER_TITLE, title);

    buffer_appendf (out, "%s/%s%s",LT,DIDL_CONTAINER,GT);
}


/* Our Metadata will be structured in such a way that 
 * there will always be a ROOT entry, This will be the
 * only entry returned in the BrowseMetadata directory.
 *      
 */
static 
BC_STATUS
cds_browse_metadata(pbcm_upnp_object        pobject,
                    unsigned int            max_count,
                    char                    *filter,
                    pcds_browse_out_params  out_params)
{

    struct buffer_t            *buff_out=NULL;

    if(!out_params || !out_params->ResultOutBuff){
        return BC_UPNP_E_INVALID_ARG;
    }

    buff_out = out_params->ResultOutBuff;

    out_params->TotalMatches = 1;
    out_params->NumberReturned = 1; 
    out_params->UpdateID = GET_SYSTEM_UPDATE_ID; /*Should come from the database*/
    didl_add_header(buff_out);
    if(IS_CONTAINER(pobject))
    {
        didl_add_container(buff_out,
                            GET_OBJECT_ID(pobject),
                            GetParentID(pobject),
                            GetChildCount(pobject),
                            "true",
                            NULL,
                            GET_TITLE(pobject),
                            GetMimeClass(pobject),
                            filter);
    }else{
        if(GetParentID(pobject) >= TUNER_CONTAINER_ID){
                didl_add_broadcast_item(buff_out,
                GET_OBJECT_ID(pobject),
                GetParentID(pobject),
                "true",
                GetMimeClass(pobject),
                GET_TITLE(pobject),
                ITEM_GET_CHANNEL_PROTOCOL(pobject),                
                ITEM_GET_CHANNEL_URL(pobject),
                ITEM_GET_CHANNEL_NUMBER(pobject),
                ITEM_GET_CHANNEL_NAME(pobject),
                filter);    
        }
        else{
                didl_add_item(buff_out,
                GET_OBJECT_ID(pobject),
                GetParentID(pobject),
                "true",
                GetMimeClass(pobject),
                GET_TITLE(pobject),
                ITEM_GET_FILE_PROTOCOL(pobject),
                ITEM_GET_SIZE(pobject),
                ITEM_GET_FILE_URL(pobject),
                filter);    
        }
    }    

    didl_add_footer(buff_out);
    
    return BC_UPNP_E_SUCCESS;
}

static 
BC_STATUS
cds_browse_children(pbcm_upnp_object pobject,
                    unsigned int  start_ind,
                    unsigned int req_count,
                    char *         filter,
                    pcds_browse_out_params out_params)
{
    struct buffer_t            *buff_out=NULL;
    unsigned int            count,result_count;
    pbcm_upnp_object        child_entry=NULL;

    if(!pobject || !out_params)
        return BC_UPNP_E_INVALID_ARG;
    

    if(!req_count)
        req_count = GetChildCount(pobject);

    buff_out = out_params->ResultOutBuff;
    didl_add_header(buff_out);

    result_count=0;    
    for(count=0;count < req_count;count++)
    {
        child_entry = dbget_item_from_cont(pobject,start_ind+count);
        if(!child_entry) 
            break; /*We reached the end*/
    
        if(IS_CONTAINER(child_entry)){
            didl_add_container(buff_out,
                                GET_OBJECT_ID(child_entry),
                                GetParentID(child_entry),
                                GetChildCount(child_entry),
                                "true",
                                NULL,
                                GET_TITLE(child_entry),
                                GetMimeClass(child_entry),
                                filter);
        }else{
            if(GetParentID(child_entry) >= TUNER_CONTAINER_ID){                
                didl_add_broadcast_item(buff_out,
                GET_OBJECT_ID(child_entry),
                GetParentID(child_entry),
                "true",
                GetMimeClass(child_entry),
                GET_TITLE(child_entry),
                ITEM_GET_CHANNEL_PROTOCOL(child_entry),                
                ITEM_GET_CHANNEL_URL(child_entry),
                ITEM_GET_CHANNEL_NUMBER(child_entry),
                ITEM_GET_CHANNEL_NAME(child_entry),
                filter);
            }
            else{                
                didl_add_item(buff_out,
                GET_OBJECT_ID(child_entry),
                GetParentID(child_entry),
                "true",
                GetMimeClass(child_entry),
                GET_TITLE(child_entry),
                ITEM_GET_FILE_PROTOCOL(child_entry),
                ITEM_GET_SIZE(child_entry),
                ITEM_GET_FILE_URL(child_entry),
                filter);
            }
        }

        result_count++;
    }

    /* The total matches should return the total match found depending on filter
     * right now we are just filling everything, later we will have to acknowledge the filter.
     */
    out_params->TotalMatches = GetChildCount(pobject);
    out_params->NumberReturned = result_count; 
    out_params->UpdateID = GET_SYSTEM_UPDATE_ID; /*Should come from the database*/
    didl_add_footer(buff_out);
    return BC_UPNP_E_SUCCESS;
}


static
unsigned int 
GetIdFromUrl(const char *FileUrl)
{
    unsigned int id=0;
    char len;
    const char *peek=NULL;
    peek =  &FileUrl[cmsUtl_strlen(FileUrl) - 1];
    len = cmsUtl_strlen(FileUrl);
    while(peek >= (FileUrl + 2))
    {
        if(*peek == '=' && *(peek-1) == 'd' && *(peek-2) == 'i')
        {
            id = atoi(peek+1);
            break;
        }
        peek--;
    }

    return id;
}

/************************************************************************/
/*            Exported API                                                */
/************************************************************************/

BC_STATUS
cds_browse(pcds_browse_in_params   in_params, 
           pcds_browse_out_params  out_params)
{
    BC_STATUS            ret_sts=BC_UPNP_E_SUCCESS;
    pbcm_upnp_object    pobject=NULL;
    BOOL                metadata=FALSE;

    if(!in_params || !out_params)
        return BC_UPNP_E_INVALID_ARG;

    if(!in_params->Filter || !in_params->BrowseFlag)
        return BC_UPNP_E_INVALID_ARG;

    /*make sure that we have a DIDL buffer for output*/
    if(!out_params->ResultOutBuff)
        return BC_UPNP_E_INVALID_ARG;

    if(!strcmp(in_params->BrowseFlag, CDS_BROWSE_METADATA))
    {
        /*Browse metadata is only for the root*/
        if(in_params->StartingIndex)
            return BC_UPNP_E_INVALID_ARG;
    
        metadata=TRUE;
                    
    }else if(!strcmp(in_params->BrowseFlag, CDS_BROWSE_CHILDREN)){
        metadata=FALSE;
    }else{
        return BC_UPNP_E_INVALID_ARG;
    }
    
    /*Get the object to Browse*/
    pobject = dbget_object(virdir_context, in_params->ObjectID);
    if(!pobject)
        return BC_UPNP_E_ARGUMENT_VALUE_INVALID;

    if(metadata)
    {
        ret_sts = cds_browse_metadata(pobject,
                                            in_params->RequestedCount,
                                            in_params->Filter,
                                            out_params);
    }else{
        ret_sts = cds_browse_children(pobject,
                                            in_params->StartingIndex,
                                            in_params->RequestedCount,
                                            in_params->Filter,
                                            out_params);
    }

    if(ret_sts != BC_UPNP_E_SUCCESS)
        BcmLogMsg("Browse Error\n");
    return ret_sts;
}

BC_STATUS
cds_get_search_caps(char *search_caps)
{
    if(!search_caps)
        return BC_UPNP_E_INVALID_ARG;

    cmsUtl_strcpy(search_caps,SEARCH_CAPS);
    return BC_UPNP_E_SUCCESS;
}

BC_STATUS
cds_get_sort_caps(char *sort_caps)
{
    if(!sort_caps)
        return BC_UPNP_E_INVALID_ARG;

    cmsUtl_strcpy(sort_caps,SORT_CAPS);
    return BC_UPNP_E_SUCCESS;
}

unsigned int
cds_get_system_update_id()
{
    /*This should come from the data base*/
    return GET_SYSTEM_UPDATE_ID;
}

BC_STATUS
cds_init(char *cds_url,
         char *ContentDir)
{
    char *SrcProtocol=NULL;
    if(!virdir_context)
    {
        virdir_context = init_database(cds_url,ContentDir);
        if(!virdir_context) /*Database initialization Failed*/
            return  BC_UPNP_E_ACTION_FAILED;
    }
    
    SrcProtocol = dbget_src_protocol_string(virdir_context);
    if(SrcProtocol) CmsUpdateSrcProtocolInfo(SrcProtocol);
    
     return BC_UPNP_E_SUCCESS;
}

BC_STATUS
cds_finish()
{
    if(virdir_context)
        finish_database(virdir_context);

    return BC_UPNP_E_SUCCESS;
}


pbcm_upnp_object
GetFileFromUrl(const char *FileUrl)
{
    unsigned int FileId=0;
    pbcm_upnp_object    pUpnpFileObject=NULL;
    if(!FileUrl) return NULL;
    FileId = GetIdFromUrl(FileUrl);    
    if(!FileId)    return NULL;
    pUpnpFileObject = dbget_object(virdir_context,FileId);

    if(!pUpnpFileObject)
        return NULL;

    if(IS_CONTAINER(pUpnpFileObject))    
        return NULL;
    
    return pUpnpFileObject;
}

char *
getXferURIfromCDS(char *url)
{
    unsigned int uriId = 0;
    pbcm_upnp_object pUpnpObject = NULL;
    char *uri = NULL;

    if (!url) return NULL;
    uriId = GetIdFromUrl(url);    
    if(!uriId) return NULL;

    pUpnpObject = dbget_object(virdir_context, uriId);
    if (!pUpnpObject) return NULL;

    if (IS_CONTAINER(pUpnpObject)) return NULL;

    uri = ITEM_GET_XFER_URL(pUpnpObject);
    return uri;
}
