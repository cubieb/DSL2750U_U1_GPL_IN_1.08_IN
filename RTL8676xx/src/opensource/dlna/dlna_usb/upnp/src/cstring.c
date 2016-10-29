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
 * $brcm_Workfile: cstring.c $
 * $brcm_Revision: 3 $
 * $brcm_Date: 9/30/09 4:14p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/cstring.c $
 * 
 * 3   9/30/09 4:14p dliu
 * SW7405-2482: Change SetText to two separate function. One with optional
 *  parameter and one without
* 
***************************************************************************/
#include "upnp_priv.h"
#include "cstring.h"

#define CSTRING_DEFAULT_GRANULARITY 256
#define CSTRING_DEFAULT_MAXCAPACITY 65536

static bool _AppendText(cstring_t* pstr, const char* fmt, va_list arglist);

bool CString_Create(cstring_t* pstr)
{
    memset(pstr, 0, sizeof(cstring_t));
    pstr->granularity = CSTRING_DEFAULT_GRANULARITY;
    pstr->maxCapacity = CSTRING_DEFAULT_MAXCAPACITY;
    return CString_SetCapacity(pstr, CSTRING_DEFAULT_GRANULARITY);
}

void CString_Clear(cstring_t* pstr)
{
    if (pstr->bytes != NULL)
        pstr->bytes[0] = 0;
    pstr->length = 0;
    pstr->capacity = 0;
}

void CString_Destroy(cstring_t* pstr)
{
    BUPnPFree(pstr->bytes);
    pstr=NULL;
}

void CString_SetBufferGranularity(cstring_t* pstr, size_t granularity)
{
    pstr->granularity = granularity;
}

bool CString_SetCapacity(cstring_t* pstr, size_t capacity)
{
    char* p;

    if ( capacity > pstr->maxCapacity )
        return false;

    p = (char*)BUPnPRealloc(pstr->bytes, capacity);
    if ( p != NULL )
    {
        pstr->bytes = p;
        pstr->capacity = capacity;
        if ( pstr->length == 0 )
            pstr->bytes[0] = 0;
        if ( pstr->length > capacity )
            pstr->length = capacity;
        return true;
    }
    else
        return false;
}

bool CString_Append(cstring_t* pstr, const char *fmt, ... )
{
    bool result;
    va_list arglist;

    va_start(arglist, fmt);
    result = _AppendText(pstr, fmt, arglist);
    va_end(arglist);

    return result;
}

bool CString_SetTextN(cstring_t* pstr, unsigned n, const char* value)
{
    if (pstr->capacity > n)
        cmsUtl_strncpy(pstr->bytes, value, n);
    else
    {
        if ( !CString_SetCapacity(pstr, n+1) )
            return false;
        cmsUtl_strncpy(pstr->bytes, value, n);
    }
    pstr->bytes[n] = '\0';
    pstr->length = n;
    return true;
}
bool CString_SetText(cstring_t* pstr, const char *value)
{
    if (pstr->capacity > cmsUtl_strlen(value))
        cmsUtl_strcpy(pstr->bytes, value);
    else
    {
        if ( !CString_SetCapacity(pstr, cmsUtl_strlen(value)+1) )
            return false;
        cmsUtl_strcpy(pstr->bytes, value);
    }
    pstr->length = cmsUtl_strlen(value);
    return true;
}

bool CString_SetTextFmt(cstring_t* pstr, const char *fmt, ... )
{
    bool result;
    va_list arglist;

    pstr->length = 0;

    va_start(arglist, fmt);
    result = _AppendText(pstr, fmt, arglist);
    va_end(arglist);

    return result;
}

const char* CString_GetText(cstring_t* pstr)
{
    return pstr->bytes;
}

int32_t CString_ToInt32(cstring_t* pstr)
{
    return (int32_t)strtol(pstr->bytes, NULL, 10);
}

uint32_t CString_ToUInt32(cstring_t* pstr)
{
    return (uint32_t)strtoul(pstr->bytes, NULL, 10);
}

double CString_ToDouble(cstring_t* pstr)
{
    return strtod(pstr->bytes, NULL);
}

static bool _AppendText(cstring_t* pstr, const char* fmt, va_list arglist)
{
    char* p;
    int n, size;

    while (true)
    {
        p = &pstr->bytes[pstr->length];
        size = pstr->capacity - pstr->length;

        n = vsnprintf(p, size, fmt, arglist);
        if (n > -1 && n < size)
        {
            pstr->length += n;
            break;
        }
        
        if ( !CString_SetCapacity(pstr, pstr->capacity + pstr->granularity) )
            return false;
    }

    return true;
}

