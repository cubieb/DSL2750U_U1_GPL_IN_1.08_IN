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
 * $brcm_Workfile: nanoxml.c $
 * $brcm_Revision: 3 $
 * $brcm_Date: 8/24/09 5:48p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/nanoxml.c $
 * 
 * 3   8/24/09 5:48p dliu
 * SW7405-2482: Don't escape line return and carriage feeds
* 
***************************************************************************/
#include "upnp_priv.h"
#include "nanoxml.h"

int nxml_open(int clientId, nxml_t *handle, const nxml_settings *settings)
{
    if ((*handle = (nxml_t)BUPnPCalloc(sizeof(**handle),1)) != NULL) {
        (*handle)->settings = *settings;
        (*handle)->namecachesize = 0;
        (*handle)->skipwhitespace = 1;
        (*handle)->state = state_begin_tag;
        (*handle)->clientId = clientId;
        (*handle)->error    = 0;
        (*handle)->dataCount = 0;
    }
    return 0;
}

void nxml_close(nxml_t handle)
{
    BUPnPFree(handle);
}

#define WHITESPACE " \t\r\n"
#define strskip(DATA,CHARS) ((const char *)(strspn(DATA,CHARS) + (DATA)))

/* Append data to the namecache. This is used for tag and attribute names.
If the nocopy parameters are specified, then the caller regards the name
as complete, and if there's nothing already in the cache, then there's no
need for a copy. */
static void nxml_add_namecache(nxml_t handle, const char *data, int len,
    const char **data_nocopy, int *len_nocopy)
{
    /* if the nocopy parameters are supplied, and there's nothing in the cache, 
    then don't copy. just pass them back. */
    if (data_nocopy && !handle->namecachesize) {
        *data_nocopy = data;
        *len_nocopy = len;
    }
    else {
        if (len > NXML_MAX_NAME_SIZE - handle->namecachesize)
            len = NXML_MAX_NAME_SIZE - handle->namecachesize;
        if (len) {
            cmsUtl_strncpy(&handle->namecache[handle->namecachesize], data, len);
            handle->namecachesize += len;
        }
        if (data_nocopy) {
            *data_nocopy = handle->namecache;
            *len_nocopy = handle->namecachesize;
        }
    }
}

int nxml_write(nxml_t handle, const char *data, unsigned len)
{
    int psiEnd=0; 
    int attrTypeStr = 0;

    const char *enddata;
    enddata = data + len;
    while ((data < enddata) && (!psiEnd)) {
        const char *s; /* temp value for capturing search results */

        /* skip whitespace */
        if (handle->skipwhitespace) {
            s = strskip(data, WHITESPACE);
            if (s != data) {
                data = s;
                continue;
            }
        }

        switch (handle->state) {
        case state_begin_tag:
            s = (const char *)strchr(data, '<');
            if (!s) {
                /* it's all data */
                if (NULL != *handle->settings.data)
                    (*handle->settings.data)(handle, data, enddata-data);
                handle->dataCount++;
                return 0;
            }
            else if (data != s) {
                /* we have some data, then a tag */
                if (NULL != *handle->settings.data)
                    (*handle->settings.data)(handle, data, s-data);
                handle->dataCount++;
                if (attrTypeStr)
                    attrTypeStr = 0;
            }
            else if((data == s) && attrTypeStr)
            {
                /* Special condition attr type string with str lenght = 0 */
                /* we have some data, then a tag */
                if (NULL != *handle->settings.data)
                    (*handle->settings.data)(handle, data, 0);
                handle->dataCount++;
                attrTypeStr = 0;
            }

            /* skip over the tag begin and process the tag name */
            data = s+1;
            handle->state = state_tag_name;
            handle->namecachesize = 0;
            break;

        case state_finish_tag:
            /* we don't care about anything but the end of a tag */
            s = (const char *)strchr(data, '>');
            if (!s) return 0;

            /* we found it, so start looking for the next tag */
            data = s+1;
            handle->state = state_begin_tag;
            break;

        case state_end_tag_name:
            s = (const char *)strpbrk(data, WHITESPACE ">");
            if (!s) {
                /* it's all name, and we're not done */
                nxml_add_namecache(handle, data, enddata-data, NULL, NULL);
                handle->skipwhitespace = 0;
                return 0;
            }
            else {
                const char *name;
                int len;
                nxml_add_namecache(handle, data, s-data, &name, &len);

                if (strncmp(name,"psitree",len) == 0)
                    psiEnd = 1;

                if (NULL!=*handle->settings.tag_end)
                    (*handle->settings.tag_end)(handle, name, len);
                handle->state = state_finish_tag;
                data = s;
            }
            break;

        case state_tag_name:
        case state_attr_name:
            if (*data == '/') {
                /* this tag is done */
                if (handle->state == state_tag_name && !handle->namecachesize) {
                    /* we can still parse the end tag name so that the uppperlevel app
                    can validate if it cares */
                    handle->state = state_end_tag_name;
                    data++;
                    break;
                }
                else if (handle->state == state_attr_name) {
                    /* we had an attribute, so this tag is just done */
                    if (NULL!=*handle->settings.tag_end)
                        (*handle->settings.tag_end)(handle, handle->namecache, handle->namecachesize);
                    handle->state = state_finish_tag;
                    data++;
                    break;
                }
            }
            else if (*data == '>') {
                handle->state = state_begin_tag;
                data++;
                break;
            }

            /* TODO: is = a valid in a tag? I don't think so. */
            s = (const char *)strpbrk(data, WHITESPACE "=/>");
            if (!s) {
                /* it's all name, and we're not done */
                nxml_add_namecache(handle, data, enddata-data, NULL, NULL);
                handle->skipwhitespace = 0;
                return 0;
            }
            else {
                /* we have the entire name */
                const char *name;
                int len;
                nxml_add_namecache(handle, data, s-data, &name, &len);

                if(len == 0)
                    printf("s=%s data = %s\n",s,data);


                if (handle->state == state_tag_name) {

                    /* don't callback on comments */
                    if (strncmp(name, "!--", 3) != 0)
                    {
                        if (NULL!=*handle->settings.tag_begin)
                            (*handle->settings.tag_begin)(handle, name, len);
                    }
                    handle->lastStartTag = name;
                    handle->lastStartTagLen = len;
                    handle->state = state_attr_name;
                }
                else {
                    if (NULL!=*handle->settings.attribute_begin)
                        (*handle->settings.attribute_begin)(handle, name, len);
                    handle->state = state_attr_value_equals;
                }
                handle->namecachesize = 0;
                data = s;
            }
            break;

        case state_attr_value:
            s = (const char *)strchr(data, '"');
            if (!s) {
                /* it's all attribute_value, and we're not done */
                if (NULL!=*handle->settings.attribute_value)
                    (*handle->settings.attribute_value)(handle, data, enddata-data);
                handle->skipwhitespace = 0;
                return 0;
            }
            else {
                /* we have some data, then a tag */
                if (NULL!=*handle->settings.attribute_value)
                    (*handle->settings.attribute_value)(handle, data, s-data);
                /* Detect special condition -->attr type string but string length = 0*/
                if(!strncmp(data, "string", s-data))
                    attrTypeStr = 1;
                else
                    attrTypeStr = 0;

            }
            /* skip over the quote and look for more attributes */
            data = s+1;
            handle->state = state_attr_name;
            handle->namecachesize = 0;

            break;


        case state_attr_value_equals:
            if (*data == '>') {
                handle->state = state_begin_tag;
                data++;
            }
            else if (*data == '=') {
                handle->state = state_attr_value_quote;
                data++;
            }
            else
                handle->state = state_attr_name;
            break;

        case state_attr_value_quote:
            if (*data == '"')
                data++;
            handle->state = state_attr_value;
            break;
        }
        handle->skipwhitespace = 1;

        /* The uppper layer detect error */
        /* Stop parsing and return */
        if(handle->error)
            return -1;

    }
    return 0;
}

/* Not really part of nanoxml, but put in here for now */
void nxml_unescape(const char* input, char* out)
{
    unsigned int i=0;
    out[0] = '\0';
    while (i<cmsUtl_strlen(input))
    {
        if (strncmp(input+i, "&lt;", 4) == 0) {
            cmsUtl_strcat(out, "<");
            i   +=4;
        } else if (strncmp(input+i, "&gt;", 4) == 0) {
            cmsUtl_strcat(out, ">");
            i   += 4;
        } else if (strncmp(input+i, "&amp;", 5) == 0) {
            cmsUtl_strcat(out, "&");
            i   += 5;
        } else if (strncmp(input+i, "&quot;", 6) == 0) {
            cmsUtl_strcat(out, "\"");
            i   += 6;
        } else if (strncmp(input+i, "&apos;", 6) == 0) {
            cmsUtl_strcat(out, ">");
            i   += 6;
        } else {
            strncat(out, (input+i), 1);
            i++;
        }
    }
}

static xml_convert_t xml_convert[] = {
  {'"' , "&quot;"},
  {'&' , "&amp;"},
  {'\'', "&apos;"},
  {'<' , "&lt;"},
  {'>' , "&gt;"},
  {0, NULL},
};

static char *nxml_getConvert(int c)
{
  int j;
  for (j = 0; xml_convert[j].xml; j++)
  {
    if (c == xml_convert[j].charac)
      return xml_convert[j].xml;
  }
  return NULL;
}

char *nxml_escape(const char *title)
{
    char *newtitle, *s, *t, *xml;
    int nbconvert = 0;

    /* calculate extra size needed */
    for (t = (char*) title; *t; t++)
    {
        xml = nxml_getConvert (*t);
        if (xml)
            nbconvert += cmsUtl_strlen (xml) - 1;
    }
    if (!nbconvert)
        return NULL;

    newtitle = s = (char*)BUPnPCalloc(1, cmsUtl_strlen (title) + nbconvert + 1);

    for (t = (char*) title; *t; t++)
    {
        xml = nxml_getConvert (*t);
        if (xml)
        {
            cmsUtl_strcpy (s, xml);
            s += cmsUtl_strlen (xml);
        }
        else
            *s++ = *t;
    }
    *s = '\0';

    return newtitle;
}


