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
 * $brcm_Workfile: dirdbase.h $
 * $brcm_Revision: 6 $
 * $brcm_Date: 9/28/09 8:56p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/dirdbase.h $
 * 
 * 6   9/28/09 8:56p ismailk
 * SW7405-3080: Set the QAM mode from the channel_info.txt file.
 * 
 * 5   9/25/09 3:38p ismailk
 * SW7405-3080: DMS IP Streaming (Live channel) Feature.
 * 
 * 4   7/15/09 3:03p ajitabhp
 * PR55165: Connection Stalling Support.
 * 
 * 3   6/30/09 7:46p ajitabhp
 * PR55165: MP4 was not parsed correctly.
 * 
 * 2   6/29/09 12:13p ajitabhp
 * PR55165: Added Path Input From User.
 * 
 *****************************************************************************/

#ifndef _DIR_DBASE_H_
#define _DIR_DBASE_H_

#include "bcm_types.h"
#include "ScanDir.h"
#include "mimes.h"
#include "buffer.h"
#include "osl.h"

#ifdef HAVE_DLNA    
#include "dlna.h"
#endif

/*
 * Directory data base implementation header file.
 * This is just a preliminary implementation of the   
 * directory structure of content directory. IT WILL'
 * BE MODIFIED LATER as the Media Server code gets matured.
 * We are just using the mocked up version of the database
 * to make sure that we advertise the services properly.
 */

#ifdef WIN32
//#define CONTENT_DIRECTORY        "o:\\DLNA_Content"    /* This should Come From the GUI/UserInput*/
#define CONTENT_DIRECTORY        "C:\\DLNA_Content"
#else
/*Define the Linux Content Directory here*/
#define CONTENT_DIRECTORY               "/home/media_streams/DLNA_Content_small"
#endif

#define VIRTUAL_DIRECTORY        "/ContentDir"
#define MAX_CONTAINERS            8
#define MAX_TUNER_CONTAINERS    4
#define    MAX_CHANNEL_FIELDS        10
/*Will be used to generate container IDs in jumps of 1000*/
#define ROOT_CONTAINER_ID        0
#define START_ITEM_ID            ROOT_CONTAINER_ID+MAX_CONTAINERS
#define MAX_PATH_LENGTH            256                
#define MAX_TITLE                256    
#define MAX_CLASS_LEN            128
#define MAX_EXT_LEN                8
#define MAX_PROTOCOL_LEN        64
#define TUNER_CONTAINER_ID        4

typedef enum _MEDIA_TYPE_{
    MTYPE_UNKNOWN,
    MTYPE_AUDIO=1,
    MTYPE_VIDEO=2,
    MTYPE_AV=4,
    MTYPE_IMAGE=8
}CONTENT_TYPE;

typedef enum CHANNEL_TYPE_{
    IP,
    QAM
}CHANNEL_TYPE;

typedef enum XFER_PROTOCOL_{
    UDP,
    RTP
}XFER_PROTOCOL;

typedef enum QAM_MODE_{
    QAM256,
    QAM64
}QAM_MODE;

typedef struct _file_info_{
    char            fullpath[MAX_PATH_LENGTH];    /* Path on the local storage */    
    char            protocol[MAX_PATH_LENGTH]; /* Max protocol Length*/
    char            url[MAX_PATH_LENGTH];            
    CONTENT_TYPE    MediaType;            /*Audio/Video or what??*/
    size_t            itemSize;            /* The size of item */

}file_info,*pfile_info;

typedef struct _channel_info_{
    unsigned int    channel_no;    
    char            channel_name[MAX_TITLE];
    char            ipaddr[64];
    unsigned int    port_no;
    char            xferurl[256]; 
    unsigned int    video_pid;
    unsigned int    audio_pid;
    unsigned int    stream_type;
    unsigned int    freq;
    unsigned int    type;    
    char            qam_mode[32];
    unsigned int    xfer_protocol;
    char            fullpath[MAX_PATH_LENGTH];        /* Path on the local storage */    
    char            mime_protocol[MAX_PATH_LENGTH]; /* Max protocol Length*/
    char            url[MAX_PATH_LENGTH];            
    CONTENT_TYPE    MediaType;            /*Audio/Video or what??*/
    size_t            itemSize;            /* The size of item */

}channel_info,*pchannel_info;

typedef struct _item_{

    union{
        channel_info channel;
        file_info file;    
    }u;    

#ifdef HAVE_DLNA
    char                        *DlnaFeatures;
    dlna_protocol_info_type_t    ProtocolID;        /* Protocol Identifier */
    dlna_org_play_speed_t        PlaySpeedID;    /* Playspeed Identifier PS*/
    dlna_org_conversion_t        ConvIndicator;    /* Conversion Indicator CI*/
    dlna_org_operation_t        Operations;        /* OP PARAM*/
    dlna_org_flags_t            DlnaFlags;            /* The Flags Field*/
    dlna_profile_t    *dlna_profile;                /*Get the DLNA Profile for this item*/
#endif
}item,*pitem;

typedef struct _item_container{
    unsigned int Container_cnt;        /* Count of Containers in this Container*/
    unsigned int item_cnt;            /* Count of Items in This container*/
    
}container, *pcontainer;

typedef struct _cleanup_item_{
    void *Address;
    struct _cleanup_item_ *next_item;
}cleanup_item, *pcleanup_item;

/*
 * 
 */
typedef enum _CONT_FLAGS_{
    OBJ_TYPE_CONT    = 1,                        /* Specifies that This object is a container.*/
}CONT_FLAGS;

typedef struct _bcm_upnp_object_{
    unsigned int Id;                            /* Generated Dynamically but use ROOT_CONTAINER_ID 
                                                * for root*/
    CONT_FLAGS    cont_flags;                        /* Depending on this flag we will access the union*/
    char    title[MAX_TITLE];                    /* Name of the directory/File */
    char    mime_class[MAX_CLASS_LEN];            /* Class is common for Container and Title*/

    struct _bcm_upnp_object_ *parent;            /* ONLY For ROOT this will be null.*/
    struct _bcm_upnp_object_ *next_container;    /* List of Containers*/
    struct _bcm_upnp_object_ *next_item;        /* List of Items in container*/

    union{
        item item_info;
        container container_info;    
    }u;    

}bcm_upnp_object,*pbcm_upnp_object;


typedef struct _dirdbase_context_{

    pcleanup_item        freelist;        /* Keep Track of Memory to free*/
    unsigned char        freecnt;
    char                base_file_url[128]; /* Base URL for file hosting*/
    char                base_broadcast_url[128]; /* Base URL for broadcoast hosting*/
    pbcm_upnp_object    DirRoot;            /*  The Root of our Virtual Directory. 
                                                The Root Element/Head of our data base*/
    /* CTT Requirement is to list all Dlna Profiels first and then 
       non DLNA compliant protocols.
     */
    struct buffer_t        *SrcProtocol;        /*Buffer to maintain the Source Protocol Info for CMS*/        
    struct buffer_t        *NonDlnaProtocols;    /**/
#ifdef HAVE_DLNA
    dlna_t                *dlna_context;    /*Context For LibDLNA functions*/
#endif

}dirdbase_context,*pdirdbase_context;


typedef struct _cb_context_{
    char                *mime_class;    /* Insert Objects For this Mime Class*/
    pbcm_upnp_object    Container;        /* Container in which the items should be Inserted*/    
    unsigned int        StartID;        /* ID To be assigned to each element*/
    pdirdbase_context    pdb_context;
}cb_context,*pcb_context;


/*API Interfaces for accessing the directory data base*/

/*Object ACCESS Macros*/
#define IS_CONTAINER(_object_) (_object_->cont_flags & OBJ_TYPE_CONT)
#define IS_ROOT(_object_) (_object_->Id == ROOT_CONTAINER_ID)
#define GET_OBJECT_ID(_object_) _object_->Id
#define GET_CONT_CNT(_object_) _object_->u.container_info.Container_cnt
#define GET_ITEM_CNT(_object_) _object_->u.container_info.item_cnt
#define GET_TITLE(_object_) _object_->title 

#ifdef HAVE_DLNA
#define HAS_DLNA_PROFILE(_object_) (NULL!=_object_->u.item_info.dlna_profile) 
#define GET_DLNA_MIME(_object_) _object_->u.item_info.dlna_profile->mime
#define GET_OPERATIONS(_object_) _object_->u.item_info.Operations
#define GET_DLNA_FEATURES(_object_) _object_->u.item_info.DlnaFeatures
#define GET_PLAY_SPEED(_object_) _object_->u.item_info.PlaySpeedID
#define IS_STREAMING_XFER_MODE(_object_) (_object_->u.item_info.DlnaFlags & DLNA_ORG_FLAG_STREAMING_TRANSFER_MODE)
#define IS_INTERACTIVE_XFER_MODE(_object_) (_object_->u.item_info.DlnaFlags & DLNA_ORG_FLAG_INTERACTIVE_TRANSFERT_MODE)
#define IS_BACKGROUND_XFER_MODE(_object_) (_object_->u.item_info.DlnaFlags & DLNA_ORG_FLAG_BACKGROUND_TRANSFERT_MODE)
#define HAS_CONN_STALL(_object_) (_object_->u.item_info.DlnaFlags & DLNA_ORG_FLAG_CONNECTION_STALL)
#define IS_DLNA_CLASS_IMAGE(_object_) (_object_->u.item_info.dlna_profile->class==DLNA_CLASS_IMAGE)
#define IS_DLNA_CLASS_AUDIO(_object_) (_object_->u.item_info.dlna_profile->class==DLNA_CLASS_AUDIO)
#define IS_DLNA_CLASS_AV(_object_) (_object_->u.item_info.dlna_profile->class==DLNA_CLASS_AV)
#define HAS_TIMESEEK_SUPPORT(_object_)(_object_->u.item_info.Operations & DLNA_ORG_OPERATION_TIMESEEK)
#define HAS_RANGE_SUPPORT(_object_)(_object_->u.item_info.Operations & DLNA_ORG_OPERATION_RANGE)
#endif 

#define ITEM_GET_FILE_PROTOCOL(_object_) _object_->u.item_info.u.file.protocol
#define ITEM_GET_CHANNEL_PROTOCOL(_object_) _object_->u.item_info.u.channel.mime_protocol
#define ITEM_GET_SIZE(_object_) _object_->u.item_info.u.file.itemSize
#define ITEM_GET_FILE_URL(_object_) _object_->u.item_info.u.file.url
#define ITEM_GET_CHANNEL_URL(_object_) _object_->u.item_info.u.channel.url
#define ITEM_GET_XFER_URL(_object_) _object_->u.item_info.u.channel.xferurl
#define ITEM_GET_CHANNEL_NUMBER(_object_) _object_->u.item_info.u.channel.channel_no
#define ITEM_GET_CHANNEL_NAME(_object_) _object_->u.item_info.u.channel.channel_name

#define GET_SYSTEM_UPDATE_ID    0 
/* 
 * init_database : do the initial scan and create the database
 * return value: A handle to created database context for access later on.
 */
char* 
GetMimeFromProtocol(pbcm_upnp_object pobject);

#define GET_MIME_FROM_PROTO(_object_) GetMimeFromProtocol(_object_)

void * 
init_database(char*  cds_url,char *ContentDir);


int GetParentID(pbcm_upnp_object pobject);
unsigned int
GetChildCount(pbcm_upnp_object pobject);

char *
GetMimeClass(pbcm_upnp_object pobject);

pbcm_upnp_object 
dbget_item_from_cont(pbcm_upnp_object pcont, 
                     unsigned int index);

/* 
 * get_container : Get the pointer to container object.
 * could be a root container, or specified container.
 */
//pbcm_upnp_object
//dbget_contianer(void * db_context, unsigned int ID);

pbcm_upnp_object
dbget_object(void * db_context, unsigned int ObjectID);


char *
dbget_src_protocol_string(pdirdbase_context db_ctx);

/*Performs the Cleanup*/
void
finish_database(pdirdbase_context db_ctx);

#endif

