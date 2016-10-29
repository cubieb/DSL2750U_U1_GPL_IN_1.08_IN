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
 * $brcm_Workfile: CDS.h $
 * $brcm_Revision: 2 $
 * $brcm_Date: 6/29/09 12:13p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/MediaServer/CDS.h $
 * 
 * 2   6/29/09 12:13p ajitabhp
 * PR55165: Added Path Input From User.
 * 
 *****************************************************************************/

#ifndef _CDS_H_
#define _CDS_H_
#include <malloc.h>
#include <string.h>
#include "bcm_types.h"
#include "buffer.h"
#include "dirdbase.h"

/* Represent the CDS DIDL Message Metadata Browse flag argument. */
#define CDS_BROWSE_METADATA "BrowseMetadata"

/* Represent the CDS DIDL Message DirectChildren Browse flag argument. */
#define CDS_BROWSE_CHILDREN "BrowseDirectChildren"

#define SEARCH_CAPS        ""
#define SORT_CAPS        ""

#ifdef WIN32
#define strtok_r    strtok_s
#endif

typedef struct _cds_browse_in_params_{
    unsigned int ObjectID;        /*String Value uniquely identifies the object */
    char *BrowseFlag;            /*Uniquely identifies the type of browse information */
    char *Filter;                /*CSV string to filter the contents of response */
    unsigned int StartingIndex; /*The starting index of the object to return */
    unsigned int RequestedCount;        /*Requested number of entries under the object*/
    char *SortCriteria;            /*Sort the result based on specified field*/
}cds_browse_in_params,*pcds_browse_in_params;

typedef struct _cds_browse_out_params_{
    struct buffer_t    *ResultOutBuff;    /*DIDL output of the function in this buffer*/
    unsigned int NumberReturned;    /*Number of objects returned in result field*/
    unsigned int TotalMatches;        /*Total sub objects in the object specified */
    unsigned int UpdateID;            /*Optional Not Used for Now*/
    unsigned int UpdateStateVariables;
}cds_browse_out_params,*pcds_browse_out_params;

extern char* EncodeToUTF8(const char* mbcsStr);
extern char* DecodeFromUTF8(const char* utf8Str);


/*Export APIs for the interface layer to call*/
/* 
 * cds_init:
 * Perform any initialization needed.
 * For now we will have this function prepare some 
 * dummy metadata and use it to test that
 */
BC_STATUS cds_init(char *cds_url,char *ContentDir);
BC_STATUS cds_finish();
/* 
 * cds_init:
 * The Browse Function needed by the CDS service.
 */

BC_STATUS
cds_browse(pcds_browse_in_params   in_params, 
            pcds_browse_out_params  out_params);

BC_STATUS
cds_get_search_caps(char *search_caps);

BC_STATUS
cds_get_sort_caps(char *sort_caps);

unsigned int
cds_get_system_update_id();

pbcm_upnp_object
GetFileFromUrl(const char *FileUrl);



#endif


