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
 * $brcm_Workfile: buffer.c $
 * $brcm_Revision: 2 $
 * $brcm_Date: 7/24/09 7:35p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/dlna/dms/buffer.c $
 * 
 * 2   7/24/09 7:35p ajitabhp
 * PR55165: Fixed the variable arguments problem we were seeing on some
 *  builds.
 * 
 *****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#ifdef WIN32
#include <minmax.h>
#endif
#include "buffer.h"

#ifndef max
#define max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define BUFFER_DEFAULT_CAPACITY 32768

struct buffer_t *
buffer_new (void)
{
  struct buffer_t *buffer = NULL;

  buffer = (struct buffer_t *) malloc (sizeof (struct buffer_t));
  if (!buffer)
    return NULL;

  buffer->buf = NULL;
  buffer->len = 0;
  buffer->capacity = 0;

  return buffer;
}

void
buffer_append (struct buffer_t *buffer, const char *str)
{
  size_t len;

  if (!buffer || !str)
    return;

  if (!buffer->buf)
  {
    buffer->capacity = BUFFER_DEFAULT_CAPACITY;
    buffer->buf = (char *) malloc (buffer->capacity * sizeof (char));
    memset (buffer->buf, '\0', buffer->capacity);
  }

  len = buffer->len + cmsUtl_strlen (str);
  if (len >= buffer->capacity)
  {
    buffer->capacity =max (len + 1, 2 * buffer->capacity);
    buffer->buf = realloc (buffer->buf, buffer->capacity);
  }

  cmsUtl_strcat (buffer->buf, str);
  buffer->len += cmsUtl_strlen (str);
}

void
buffer_appendf_new (struct buffer_t *buffer, const char *format, ...)
{
  char str[BUFFER_DEFAULT_CAPACITY];
  int size;
  va_list va;

  if (!buffer || !format)
    return;

  va_start (va, format);
  size = vsnprintf (str, BUFFER_DEFAULT_CAPACITY, format, va);
  if (size >= BUFFER_DEFAULT_CAPACITY)
  {
    char* dynstr = (char *) malloc (size + 1);
    vsnprintf (dynstr, size + 1, format, va);
    buffer_append (buffer, dynstr);
    free (dynstr);
  }
  else
    buffer_append (buffer, str);
  va_end (va);
}

void
buffer_free (struct buffer_t *buffer)
{
  if (!buffer)
    return;

  if (buffer->buf)
    free (buffer->buf);
  free (buffer);
}
