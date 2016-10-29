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
 * $brcm_Workfile: bdlna_didl.c $
 * $brcm_Revision: 5 $
 * $brcm_Date: 9/30/09 4:35p $
 *
 * Module Description:
 *
 * Revision History:
 *
 * $brcm_Log: /AppLibs/dlna/core/src/bdlna_didl.c $
 * 
 * 5   9/30/09 4:35p dliu
 * SW7405-2482: Remove unneeded comments
 * 
 * 4   9/21/09 5:18p dliu
 * SW7405-2482: Add more type checking when parsing protocol info
 * 
 * 3   7/28/09 4:55p dliu
 * PR55165: Add parsing for protocol info
 *
 * 2   7/7/09 5:23p dliu
 * PR55165: Add DTCP-IP support
*
***************************************************************************/
#include "osl.h"
#include "upnp.h"
#include "bdlna.h"
#include "action.h"
#include "nanoxml.h"
#include "bdlna_debug.h"
#include "bdlna_didl.h"

char* lastAttribute = NULL;
int didlIndex = -1;

#define DIDL_NAMESPACE \
    "xmlns=\"urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/\" " \
    "xmlns:dc=\"http://purl.org/dc/elements/1.1/\" " \
    "xmlns:upnp=\"urn:schemas-upnp-org:metadata-1-0/upnp/\""

#define GT    "&gt;"
#define LT    "&lt;"
#define QUOTE "&quot;"
#define AMP "&amp;"
#define APOS "&apos;"

static void BDlna_Didl_XmlTagBegin(nxml_t handle, const char *tag_name, unsigned len);
static void BDlna_Didl_XmlAttrBegin(nxml_t handle, const char *tag_name, unsigned len);
static void BDlna_Didl_XmlData(nxml_t handle, const char *tag_name, unsigned len);
static void BDlna_Didl_XmlAttrValue(nxml_t handle, const char *tag_name, unsigned len);

char* BDlna_Didl_GetId(BDlna_DidlHandle didlObjects, int index)
{
    /* ID is a default parameter */
    return didlObjects->items[index].id;
}

char* BDlna_Didl_GetTitle(BDlna_DidlHandle didlObjects, int index)
{
    /* title is a default parameter */
    return didlObjects->items[index].title;
}

const char* BDlna_Didl_GetContentUri(BDlna_DidlHandle didlObjects, int resourceIndex)
{
    struct BDlna_DidlResource* pResource;

    /* App can only call this after GetMetaData which returns one element */
    pResource = BLST_FIRST(&(didlObjects->items[0].resourceList));
    for( ; pResource; pResource = BLST_NEXT(pResource, link))
    {
        if (pResource->index == resourceIndex)
            return pResource->contentUri;
    }
    return NULL;
}

char* BDlna_Didl_GetProtocolInfo(BDlna_DidlHandle didlObjects, int resourceIndex)
{
    struct BDlna_DidlResource* pResource;
    /* App can only call this after GetMetaData which returns one element */
    pResource = BLST_FIRST(&(didlObjects->items[0].resourceList));
    for( ; pResource; pResource = BLST_NEXT(pResource, link))
    {
        if (pResource->index == resourceIndex)
            return pResource->protocolInfo;
    }
    return NULL;
}

void BDlna_Didl_ParseProtocolInfo(char* protocolInfo, BDlna_ProtocolInfo* dlnaProtocol)
{
    char* temp;
    char flag[9];
    char* fourthField;

    if (dlnaProtocol == NULL)
    {
        DLNA_DEBUG_ERROR(BDlnaDebugCategory_Dmp, ("Invalid protocol"));
        return;
    }

    memset(dlnaProtocol, 0, sizeof(*dlnaProtocol));

    DLNA_DEBUG_TRACE(BDlnaDebugCategory_Dmp, ("Protocol: %s", protocolInfo));

    dlnaProtocol->IsDlna15 = false;
    fourthField = strrchr(protocolInfo, ':');

    /* parse for DLNA.ORG_FLAG */
    temp = stristr(fourthField, "DLNA.ORG_FLAGS=");
    if (temp != NULL)
    {
        temp += strlen("DLNA.ORG_FLAGS=");
        strncpy(flag, temp, 8); /* grab first 32-bits only for now */
        flag[8] = '\0';
        dlnaProtocol->orgFlag = strtoul(flag, NULL, 16);
        if ( dlnaProtocol->orgFlag & ORG_FLAG_DLNA_V15)
        dlnaProtocol->IsDlna15 = true;
    }

    /* parse for DLNA.ORG_CI */
    temp = stristr(fourthField, "DLNA.ORG_CI=");
    if (temp != NULL)
    {
        temp += strlen("DLNA.ORG_CI=");
        if (MATCH_PREFIX(temp,"0"))
            dlnaProtocol->conversionIndicator = false;
        else
            dlnaProtocol->conversionIndicator = true;
        dlnaProtocol->IsDlna15 = true;
    }
    else
    {
        /* If value is omitted, must infer content is not converted */
        dlnaProtocol->conversionIndicator = false;
    }

    /* parse for DLNA.ORG_PS */
    temp = stristr(fourthField, "DLNA.ORG_PS=");
    if (temp != NULL)
    {
        temp += strlen("DLNA.ORG_PS=");
        dlnaProtocol->playSpeed = temp;
        dlnaProtocol->IsDlna15 = true;
    }

    /* parse for DLNA.ORG_OP */
    temp = stristr(fourthField, "DLNA.ORG_OP=");
    if (temp != NULL)
    {
        temp += strlen("DLNA.ORG_OP=");
        dlnaProtocol->supportTimeRange = MATCH_PREFIX(temp,"0")?false:true;
        temp++;
        dlnaProtocol->supportByteRange = MATCH_PREFIX(temp,"0")?false:true;
        dlnaProtocol->IsDlna15 = true;
    }

    /* parse for DLNA.ORG_PN */
    temp = stristr(fourthField, "DLNA.ORG_PN=");
    if (temp != NULL)
    {
        temp += strlen("DLNA.ORG_PN=");
        dlnaProtocol->orgPN = temp;
        temp = strpbrk(temp, ";");
        if (temp != NULL)
            temp[0] = '\0';

        dlnaProtocol->IsDlna15 = true;
    }

    temp = strrchr(protocolInfo, ':');
    temp[0] = '\0';
    dlnaProtocol->mime = strrchr(protocolInfo, ':');
    if (dlnaProtocol->mime != NULL)
    {
        dlnaProtocol->mime++; /* advance pass the ':' */

        if (IMATCH_PREFIX(dlnaProtocol->mime, "image"))
            dlnaProtocol->isImage = true;
        else
            dlnaProtocol->isImage = false;
    }
    dlnaProtocol->networkID = protocolInfo;
    temp = strpbrk(protocolInfo, ":");
    temp[0] = '\0';
}

BDlna_DidlMediaType BDlna_Didl_GetMediaType(BDlna_DidlHandle didlObjects, int index)
{
    return didlObjects->items[index].mediaType;
}

int BDlna_Didl_GetResourceCount(BDlna_DidlHandle didlObjects, int index)
{
    return didlObjects->items[index].resourceCount;
}

char* BDlna_Didl_GetClass(BDlna_DidlHandle didlObjects, int index)
{
    return didlObjects->items[index].classInfo;
}

uint32_t BDlna_Didl_GetChildrenCount(BDlna_DidlHandle didlObjects, int index)
{
    return didlObjects->items[index].childCount;
}

static void BDlna_Didl_XmlTagBegin(nxml_t handle, const char *tag_name, unsigned len)
{
    struct BDlna_DmpMediaItem* pMedia;
    struct BDlna_DidlResource* pResource;
    char* end;

    UNUSED_ARGUMENT(len);
    if (IMATCH_PREFIX(tag_name, "container"))
    {
        didlIndex++;
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pMedia[didlIndex].mediaType = BDlna_DidlMediaType_eMediaContainer;
    }
    else if (IMATCH_PREFIX(tag_name, "item"))
    {
        didlIndex++;
        end = strstr(tag_name, "</item>");
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pMedia[didlIndex].metaData = strndup(tag_name-1, end-tag_name+1+strlen("</item>"));
        pMedia[didlIndex].mediaType = BDlna_DidlMediaType_eMediaItem;
    }
    else if (IMATCH_PREFIX(tag_name, "res"))
    {
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);

        /* The resource list has to be dynamic because we don't know the size */
        pResource = (struct BDlna_DidlResource*)BUPnPCalloc(1, sizeof(struct BDlna_DidlResource));
        BLST_INSERT_TAIL(&(pMedia[didlIndex].resourceList), pResource, link);
        pResource->index = pMedia[didlIndex].resourceCount;
        pMedia[didlIndex].resourceCount++;
    }
}

static void BDlna_Didl_XmlAttrBegin(nxml_t handle, const char *tag_name, unsigned len)
{
    UNUSED_ARGUMENT(handle);
    UNUSED_ARGUMENT(len);
    lastAttribute = (char*)tag_name;
}

static void BDlna_Didl_XmlAttrValue(nxml_t handle, const char *tag_name, unsigned len)
{
    struct BDlna_DmpMediaItem* pMedia;
    struct BDlna_DidlResource* pResource;

    if (IMATCH_PREFIX(lastAttribute, "parentID"))
    {
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pMedia[didlIndex].parentId = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(lastAttribute, "childCount"))
    {
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pMedia[didlIndex].childCount = atoi(tag_name);
    }
    else if (IMATCH_PREFIX(lastAttribute, "id"))
    {
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pMedia[didlIndex].id = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(lastAttribute, "restricted"))
    {
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        if (IMATCH_PREFIX(tag_name, "true") || (IMATCH_PREFIX(tag_name, "1")))
            pMedia[didlIndex].resticted = 1;
        else
            pMedia[didlIndex].resticted = 0;
    }
    else if (IMATCH_PREFIX(lastAttribute, "protocolInfo"))
    {
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pResource = BLST_LAST(&(pMedia[didlIndex].resourceList));
        pResource->protocolInfo = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(lastAttribute, "duration"))
    {
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pResource = BLST_LAST(&(pMedia[didlIndex].resourceList));
        pResource->duration = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(lastAttribute, "size"))
    {
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pResource = BLST_LAST(&(pMedia[didlIndex].resourceList));
        pResource->size = atoi(tag_name);
    }
}

static void BDlna_Didl_XmlData(nxml_t handle, const char *tag_name, unsigned len)
{
    struct BDlna_DmpMediaItem* pMedia;
    struct BDlna_DidlResource* pResource;

    if (handle->lastStartTag == NULL)
        return;

    /* DIDL Max length is 1024 (DLNA: 7.3.17) */
    /*
    if (len > 1024)
        handle->error = BDLNA_E_INVALID_DIDL;
    */

    if (IMATCH_PREFIX(handle->lastStartTag, "upnp:class"))
    {
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pMedia[didlIndex].classInfo = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "dc:title"))
    {
        /* TODO:some file name can have escape characters embedded, ie &amp;quot;
          * so we have to call unescape again here */
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pMedia[didlIndex].title = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "dc:date"))
    {
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pMedia[didlIndex].date = BUPnPStrndup(tag_name, len);
    }
    else if (IMATCH_PREFIX(handle->lastStartTag, "res")) /* URL */
    {
        pMedia = ((struct BDlna_DmpMediaItem*)handle->context);
        pResource = BLST_LAST(&(pMedia[didlIndex].resourceList));
        pResource->contentUri = BUPnPStrndup(tag_name, len);
    }
}

BDlna_DidlHandle BDlna_Didl_Create()
{
    BDlna_DidlHandle pDidlObjects;
    pDidlObjects = (BDlna_DidlHandle)BUPnPCalloc(1, sizeof(struct BDlna_Didl));

    return pDidlObjects;
}

void BDlna_Didl_ClearItems(BDlna_DidlHandle didlObjects)
{
    size_t idx = 0;
    struct BDlna_DidlResource* pResource, *pPrevResource;

    for(idx=0;idx<didlObjects->itemCount;idx++)
    {
        BUPnPFree(didlObjects->items[idx].id);
        BUPnPFree(didlObjects->items[idx].parentId);
        BUPnPFree(didlObjects->items[idx].classInfo);
        BUPnPFree(didlObjects->items[idx].title);
        BUPnPFree(didlObjects->items[idx].metaData);

        pResource = BLST_FIRST(&didlObjects->items[idx].resourceList);
        while(pResource)
        {
            BUPnPFree(pResource->protocolInfo);
            BUPnPFree(pResource->contentUri);
            BUPnPFree(pResource->resolution);
            BUPnPFree(pResource->duration);
            pPrevResource = pResource;
            pResource = BLST_NEXT(pResource, link);
            BUPnPFree(pPrevResource);
        }
    }
    BUPnPFree(didlObjects->items);
    didlObjects->items = NULL;
}

void BDlna_Didl_Destroy(BDlna_DidlHandle didlObjects)
{
    BDlna_Didl_ClearItems(didlObjects);
    BUPnPFree(didlObjects);
}

char* BDlna_Didl_GetMetaData(BDlna_DidlHandle didlObjects, int index)
{
    return didlObjects->items[index].metaData;
}

BDlna_Error BDlna_Didl_ParseResult(const char* in, BDlna_DidlHandle didlObjects, uint32_t index)
{
    BDlna_Error result;
    int clientId = 0;
    nxml_t handle;
    nxml_settings settings;
    char* soapEnvelope = BUPnPStrdup(in);

    memset(&settings, 0, sizeof(settings));

    settings.tag_begin = BDlna_Didl_XmlTagBegin;
    settings.attribute_begin = BDlna_Didl_XmlAttrBegin;
    settings.attribute_value = BDlna_Didl_XmlAttrValue;
    settings.data = BDlna_Didl_XmlData;

    didlIndex = index-1;

    nxml_unescape(in, soapEnvelope);

    nxml_open(clientId, &handle,&settings);
    handle->context = (void*)didlObjects->items;

    nxml_write(handle, soapEnvelope, strlen(soapEnvelope));
    result = handle->error;
    didlObjects->itemCount = didlIndex + 1;

    nxml_close(handle);
    BUPnPFree(soapEnvelope);

    return result;
}


