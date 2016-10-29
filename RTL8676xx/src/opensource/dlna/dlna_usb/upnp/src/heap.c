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
 * $brcm_Workfile: heap.c $
 * $brcm_Revision: 4 $
 * $brcm_Date: 9/30/09 3:25p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/heap.c $
 * 
 * 4   9/30/09 3:25p dliu
 * SW7405-2482: Add heap.h
 * 
 * 3   9/15/09 10:22p dliu
 * SW7405-2482: Fix compiler warnings about unused arguments
 * 
 *****************************************************************************/
#include "upnp_priv.h"
#include "upnp.h"
#include "list.h"
#include "heap.h"
#include "osl.h"


#if defined(HEAP_DEBUG)
struct BUPnPMemory
{
    void* ptr;
    size_t length;
    char source[256];
    int line;
    BLST_ENTRY(BUPnPMemory) link;
};

BLST_HEAD(BUPnPMemoryList, BUPnPMemory );
struct BUPnPMemoryList gMemoryList;
uint32_t gCurrentMemoryUsage;
uint32_t gPeakMemoryUsage;
bthread_mutex_t* gMemoryListMutex;
#endif

void BUPnPHeapInitialize()
{
#if defined(HEAP_DEBUG) 
    gMemoryListMutex = bthread_mutex_create();
#endif
}

void BUPnPHeapClose()
{
#if defined(HEAP_DEBUG) 
    struct BUPnPMemory* pMemory;
    pMemory = BLST_FIRST(&gMemoryList);
    while( pMemory)
    {
        printf("MEMORY LEAK DETECTED\n");
        printf("FILE: %s\n", pMemory->source);
        printf("LINE: %d\n", pMemory->line);
        printf("SIZE: %d\n", pMemory->length);
        pMemory = BLST_NEXT(pMemory, link);
    }
#endif
}

void* BUPnPCallocPriv(size_t numberElements, size_t sizeElements, char* source, int line)
{
#if defined(HEAP_DEBUG) 
    struct BUPnPMemory* pMemory = (struct BUPnPMemory*)calloc(1, sizeof(struct BUPnPMemory));
    bthread_mutex_lock(gMemoryListMutex);
    pMemory->ptr = calloc(numberElements, sizeElements);
    pMemory->length = numberElements * sizeElements;
    cmsUtl_strcpy(pMemory->source, source);
    pMemory->line = line;
    BLST_INSERT_TAIL(&gMemoryList, pMemory, link);
    gCurrentMemoryUsage += pMemory->length;
    if (gPeakMemoryUsage < gCurrentMemoryUsage)
        gPeakMemoryUsage = gCurrentMemoryUsage;
    bthread_mutex_unlock(gMemoryListMutex);
    return pMemory->ptr;
#else
    UNUSED_ARGUMENT(source);
    UNUSED_ARGUMENT(line);
    return calloc(numberElements, sizeElements);
#endif
}

void BUPnPFree(void* ptr)
{
#if defined(HEAP_DEBUG)
    struct BUPnPMemory* pMemory;
    struct BUPnPMemory* pTemp;

    if (ptr == NULL)
        return;

    bthread_mutex_lock(gMemoryListMutex);
    pMemory = BLST_FIRST(&gMemoryList);
    while( pMemory)
    {
        if (pMemory->ptr == ptr)
        {
            pTemp = BLST_NEXT(pMemory, link);
            gCurrentMemoryUsage -= pMemory->length;
            free(pMemory->ptr);
            BLST_REMOVE(&gMemoryList, pMemory, BUPnPMemory, link);
            free(pMemory);
            pMemory = pTemp;
            break;;
        }
        else
            pMemory = BLST_NEXT(pMemory, link);
    }
    bthread_mutex_unlock(gMemoryListMutex);
#else
    if (ptr != NULL)
        free(ptr);
#endif
}


void *BUPnPReallocPriv(void* memory, size_t newSize, char* source, int line)
{
#if defined(HEAP_DEBUG) 
    int count = 0;
    /* Remove previous entry */
    struct BUPnPMemory* pMemory;

    if (memory == NULL)
    {
        return BUPnPCallocPriv(1, newSize, source, line);
    }

    bthread_mutex_lock(gMemoryListMutex);
    pMemory = BLST_FIRST(&gMemoryList);
    while( pMemory)
    {
        count++;
        if (pMemory->ptr == memory)
        {
            pMemory->ptr = realloc(memory, newSize);
            gCurrentMemoryUsage -= pMemory->length;
            pMemory->length = newSize;
            gCurrentMemoryUsage += pMemory->length;
            if (gPeakMemoryUsage < gCurrentMemoryUsage)
                gPeakMemoryUsage = gCurrentMemoryUsage;
            bthread_mutex_unlock(gMemoryListMutex);
            return pMemory->ptr;
        }
        else
            pMemory = BLST_NEXT(pMemory, link);
    }
    bthread_mutex_unlock(gMemoryListMutex);
    return realloc(memory, newSize);
#else
    UNUSED_ARGUMENT(line);
    UNUSED_ARGUMENT(source);
    return realloc(memory, newSize);
#endif
}

char* _BUPnPStrndup(const char *src, size_t count, char* file, int line)
{
    char* pBuffer = (char*)BUPnPCallocPriv(1, count+1, file, line);
    cmsUtl_strncpy(pBuffer, src, count);
    pBuffer[count] = 0;
    return pBuffer;
}

char* _BUPnPStrdup(const char *src, char* file, int line)
{
    char* pBuffer = (char*)BUPnPCallocPriv(1, cmsUtl_strlen(src)+1, file, line);
    cmsUtl_strcpy(pBuffer, src);
    pBuffer[cmsUtl_strlen(src)] = 0;
    return pBuffer;
}



