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
 * $brcm_Workfile: timer.c $
 * $brcm_Revision: 5 $
 * $brcm_Date: 9/30/09 3:31p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/timer.c $
 * 
 * 5   9/30/09 3:31p dliu
 * SW7405-2482: Add function to get timer parameter
* 
***************************************************************************/
#include "upnp_priv.h"
#include "list.h"
#include "osl.h"
#include "upnp.h"
#include "timer.h"
#include "debug.h"

#define MAX_TIME 0x7FFFFFFF

struct _Timer
{
    Timer_EventCallback callback;
    void* param;
    uint32_t period;
    uint32_t timeLeft;
    bool executeOnce;
    bool executeCount;
    bool deleted;
    BLST_ENTRY(_Timer) link;
};

struct Scheduler *scheduler;
BLST_HEAD(BUPnPTimerList, _Timer);

struct Scheduler
{
    bthread_event* controlEvent;
    bthread_mutex_t *mutex;
    bthread_mutex_t *callbackMutex;
    uint32_t waitTime;
    bthread* pThreadId;
    struct BUPnPTimerList timerList;
    bool exiting;
};

static void Timer_threadProc(void* data)
{
    int result;
    uint32_t minTime = MAX_TIME;
    TimerHandle node, removedTimer;
    UNUSED_ARGUMENT(data);

    while(1)
    {
        result = bthread_WaitForEvent(scheduler->controlEvent, scheduler->waitTime);
        if (result == BUPNP_WAIT_TIMEOUT)
        {
            bthread_mutex_lock(scheduler->mutex);
            node = BLST_FIRST(&scheduler->timerList);

            if (node == NULL)
            {
                bthread_mutex_unlock(scheduler->mutex);
                continue;
            }

            if (scheduler->exiting == true)
            {
                bthread_mutex_unlock(scheduler->mutex);
                return;
            }
            minTime = node->timeLeft;
            while(node)
            {
                UPNP_DEBUG_TRACE( BUPnPDebugCategory_Timer, ("%d:Time Left %d\n", node, node->timeLeft));
                node->timeLeft -= scheduler->waitTime;

                /* First check if another thread request to remove this timer */
                if (node->deleted == true)
                {
                    removedTimer = node;

                    node = BLST_NEXT(node, link);

                    BLST_REMOVE(&scheduler->timerList, removedTimer, _Timer, link);
                    BUPnPFree(removedTimer);

                    continue;
                }

                /* Check if timer expired */
                if (node->timeLeft <= 0)
                {
                    node->timeLeft = node->period;
                    if ((node->executeOnce==false) || (node->executeCount == 0))
                    {
                        node->executeCount++;

                        bthread_mutex_unlock(scheduler->mutex);
                        bthread_mutex_lock(scheduler->callbackMutex);
                        node->callback(node, node->param);
                        bthread_mutex_unlock(scheduler->callbackMutex);
                        bthread_mutex_lock(scheduler->mutex);
                    }
                }

                /* Finally find the next wait time */
                if (node->timeLeft < minTime)
                    minTime = node->timeLeft;

                node = BLST_NEXT(node, link);
            }
            if (scheduler->waitTime != minTime)
            {
                scheduler->waitTime = minTime;
            }
            bthread_mutex_unlock(scheduler->mutex);
        }
        else if (result == BUPNP_EVENT_SIGNALED)
        {
            bthread_mutex_lock(scheduler->mutex);
            if (scheduler->exiting == true)
            {
                bthread_mutex_unlock(scheduler->mutex);
                return;
            }
            bthread_mutex_unlock(scheduler->mutex);
            bthread_ResetEvent(scheduler->controlEvent);
        }
        else
        {
            assert(0);
        }

    }
}

void Timer_Initialize()
{
    /* allocate scheduler resource */
    scheduler = (struct Scheduler*)BUPnPCalloc(1, sizeof(struct Scheduler));

    scheduler->mutex = bthread_mutex_create();
    if (scheduler->mutex == NULL)
    {
        UPNP_DEBUG_TRACE( BUPnPDebugCategory_Timer, ("Unable to create mutex\n"));
        assert(0);
    }

    scheduler->callbackMutex = bthread_mutex_create();
    if (scheduler->callbackMutex == NULL)
    {
        UPNP_DEBUG_TRACE( BUPnPDebugCategory_Timer, ("Unable to create callback mutex\n"));
        assert(0);
    }

    BLST_INIT(&scheduler->timerList);

    scheduler->controlEvent = bthread_CreateEvent();
    if (scheduler->controlEvent == NULL)
    {
        UPNP_DEBUG_TRACE( BUPnPDebugCategory_Timer, ("Unable to create control event\n"));
        assert(0);
    }
    scheduler->waitTime = MAX_TIME;

    /* Launching the thread should be the last thing we do here */
    scheduler->pThreadId = bthread_create(Timer_threadProc, NULL);
}

void Timer_Close()
{
    TimerHandle node, timer;

    bthread_mutex_lock(scheduler->mutex);
    scheduler->exiting = true;
    bthread_SetEvent(scheduler->controlEvent);

    /* remove all existing timers */
    node = BLST_FIRST(&scheduler->timerList);

    while(node)
    {
        timer = node;
        node = BLST_NEXT(node, link);
        BUPnPFree(timer);
    }
    bthread_mutex_unlock(scheduler->mutex);

    bthread_join(scheduler->pThreadId, 5000);
    bthread_mutex_destroy(scheduler->mutex);
    bthread_mutex_destroy(scheduler->callbackMutex);
    bthread_DestroyEvent(scheduler->controlEvent);
    BUPnPFree(scheduler);
}

void* Timer_GetParam(TimerHandle hTimer)
{
    return (void*)hTimer->param;
}

TimerHandle Timer_Create(const TimerSettings *pSettings)
{
    TimerHandle pTimer;

    pTimer = (TimerHandle)BUPnPCalloc(1, sizeof(*pTimer));
    if (pTimer == NULL)
    {
       printf("Cannot allocate timer\n");
    }
    pTimer->callback = pSettings->callback;
    pTimer->param = pSettings->param;
    pTimer->period = pSettings->period;
    pTimer->timeLeft = pSettings->period;
    pTimer->executeOnce = pSettings->executeOnce;
     
    bthread_mutex_lock(scheduler->mutex);
    
    if (pSettings->period < scheduler->waitTime)
    {
        scheduler->waitTime = pSettings->period;
        bthread_SetEvent(scheduler->controlEvent);
    }

    BLST_INSERT_HEAD(&scheduler->timerList, pTimer, link);
    bthread_mutex_unlock(scheduler->mutex);
    return pTimer;
}

void Timer_Destroy( TimerHandle hTimer )
{
    TimerHandle node;

    bthread_mutex_lock(scheduler->mutex);
    node = BLST_FIRST(&scheduler->timerList);

    while(node)
    {
        if (node == hTimer)
            node->deleted = true;
        node = BLST_NEXT(node, link);
    }
    bthread_mutex_unlock(scheduler->mutex);
    return;
}

void Timer_BlockEvents()
{
    bthread_mutex_lock(scheduler->callbackMutex);
}

void Timer_UnblockEvents()
{
    bthread_mutex_unlock(scheduler->callbackMutex);
}

