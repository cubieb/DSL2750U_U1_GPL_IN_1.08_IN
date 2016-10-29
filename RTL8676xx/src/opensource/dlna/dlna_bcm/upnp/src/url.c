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
***************************************************************************/
#include "upnp_priv.h"
#include "url.h"

static int Url_Parse (Url *url, const char *pszUrl);

int Url_Create (Url *pUrl, const char *pszUrl)
{
    memset(pUrl, 0, sizeof(Url));
    strcpy(pUrl->scheme, "http");
    pUrl->port = 80;
    return Url_Parse(pUrl, pszUrl);
}

void Url_Destroy(Url *pUrl)
{
    BUPnPFree(pUrl->buf);
    BUPnPFree(pUrl->host);
    BUPnPFree(pUrl->path);
    BUPnPFree(pUrl->query);

    pUrl->buf = NULL;
    pUrl->host = NULL;
    pUrl->path = NULL;
    pUrl->query = NULL;
}

const char* Url_ToString(Url *pUrl)
{
    char *p;
    size_t length;

    assert(pUrl->host);
    assert(pUrl->path);

    length = strlen(pUrl->scheme) + strlen(pUrl->host) + strlen(pUrl->path);
    if ( pUrl->query != NULL )
        length += strlen(pUrl->query);
    length+=16;/* pad for extra chars ://, :, portlength */

    if ( pUrl->buf == NULL )
        pUrl->buf = (char*)BUPnPCalloc(length, 1);
    else if ( strlen(pUrl->buf) > length )
    {
        p = (char*)BUPnPRealloc(pUrl->buf, length);
        if ( p==NULL )
            return NULL;
        pUrl->buf = p;
    }
        
    /* Don't need an extra slash before path */
    sprintf(pUrl->buf, "%s://%s:%d%s", pUrl->scheme, pUrl->host, pUrl->port, pUrl->path);
    if ( pUrl->query )
    {
        strcat(pUrl->buf, "?");
        strcat(pUrl->buf, pUrl->query);
    }

    return pUrl->buf;
}

static int Url_Parse (Url *pUrl, const char *pszUrl)
{
    size_t len;
    char *start, *end;

    /************************************************************************
     Parse the Scheme.
     ***********************************************************************/
    start = end =(char*)pszUrl;
    if ( *end != '/' )
    {
        end = strchr(start, ':');
        if (end == NULL)
            return -1;
    
        strncpy(pUrl->scheme, start, end-start);

        end++;
        if ( *end++ != '/' || *end++ != '/' ) /* Must have '//' after the ':' */
            return -1;
    }
    else if ( *(end+1) == '/' )
        end++;

    /************************************************************************
     Parse the Host / Port.
     ***********************************************************************/
    start = end;
    len = strcspn(start, ":/?");
    if ( len < strlen(start) )
    {
        end = start+len;
        if (*end == '?')
            return -1; /* illegal to have a query marker before the path marker */

        if ( end > start )
        {
            pUrl->host = BUPnPStrndup(start, end-start);
            if (*end == ':')
                pUrl->port = (uint16_t)strtoul(end+1, NULL, 10);
        }
    }

    /************************************************************************
     Parse the /Path?Query
     ***********************************************************************/
    start = (*end == '/') ? end : strchr(end, '/');
    if (start == NULL)
        return -1;

    end = strchr(start, '?');
    if ( end == NULL )
        pUrl->path = BUPnPStrdup(start);
    else
    {
        pUrl->path = BUPnPStrndup(start, end-start);

        pUrl->query = BUPnPStrdup(end+1);
        DecodeHex(pUrl->query, pUrl->query);
    }
        
    /* Check for and remove trailing '/' according to RFC 2396 */
    end = pUrl->path + strlen(pUrl->path);
    if ( *end == '/' )
        *end = '\0';

    DecodeHex(pUrl->path, pUrl->path);
    
    return 0;
}

static char Url_ToHex( char c )
{
    if ( c >= '0' && c <= '9' )
        return c - '0';
    if ( c >= 'a' && c <= 'f' )
        return c - 'a' + 10;
    if ( c >= 'A' && c <= 'F' )
        return c - 'A' + 10;
    return 0; /* shouldn't happen, we're guarded by isxdigit() */
}

/* Decodes all hex characters in string and converts to proper form */
void DecodeHex( char* to, char* from )
{
    for ( ; *from != '\0'; ++to, ++from )
    {
        if ( from[0] == '%' && isxdigit( (int) from[1] ) && isxdigit( (int) from[2] ) )
        {
            *to = Url_ToHex( from[1] ) * 16 + Url_ToHex( from[2] );
            from += 2;
        }
        else
            *to = *from;
    }
    *to = '\0';
}
