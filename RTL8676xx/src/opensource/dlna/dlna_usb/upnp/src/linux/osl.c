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
 * $brcm_Workfile: osl.c $
 * $brcm_Revision: 7 $
 * $brcm_Date: 9/2/09 10:55a $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/linux/osl.c $
 * 
 * 7   9/2/09 10:55a dliu
 * SW7405-2482: Add interface to UPnP Settings
 * 
 * 6   7/7/09 2:41p dliu
 * PR55165: Add stristr parser function
*
***************************************************************************/
#include <pthread.h>
#include <sys/time.h>
#include "osl.h"
#include "upnp.h"
#include "upnp_priv.h"
#include "debug.h"

#define MS_PER_SEC 1000        /* 1000ms per second */
#define US_PER_SEC 1000000    /* 1000000us per second */
#define US_PER_MS  1000        /* 1000us per ms */
#define UCLOCKS_PER_SEC 1000000 /* Clock ticks per second */
#define DCE_VERSION 1
#define DCE_VARIANT 0x80
#define DEFAULT_STACK_SIZE (64*1024*8)
#define DEFAULT_INTERFACE "br0"

static uint64_t lastTime;

static void uuid_seed_random(void);
static void uuid_get_system_time(uint64_t *uuid_time);
static int uuid_create_random_number(void);
static void uuid_generate_random(char* out, size_t length);
static void uuid_get_random_bytes(void *buf, int nbytes);
static int uuid_get_random_fd(void);

int host_getipaddress(const char* interface, char *ipAddress, size_t length)
{
    int sockfd;
    struct sockaddr_in *sockaddr;
    struct ifreq ifreq;

    if ((sockfd = socket( AF_INET, SOCK_DGRAM, 0 )) < 0) {
        perror("socket");
        return -1;
    }

    if (interface == NULL)
        cmsUtl_strncpy(ifreq.ifr_name, DEFAULT_INTERFACE, IFNAMSIZ);
    else
        cmsUtl_strncpy(ifreq.ifr_name, interface, IFNAMSIZ);

    if (ioctl(sockfd, SIOCGIFADDR, &ifreq) < 0) {
        cmsUtl_strcpy(ipAddress, "");
    } else {
        sockaddr = (struct sockaddr_in*)&ifreq.ifr_addr;
        cmsUtl_strncpy(ipAddress, inet_ntoa( *(struct in_addr *)(&sockaddr->sin_addr)), length);
        printf("IP: %s\n", ipAddress);
    }

    close(sockfd);
    return -1;
}

static void* pthreadProc(void *data)
{
    int threadID;
    bthread* thread = (bthread*)data;
    threadID = thread->threadID;
    thread->pThreadFunc(thread->pContext);
    thread->state = bthreadState_eStopped;

    return NULL;
}

bthread* bthread_create(void (*startAddress)(void*), void* arg)
{
    bthread* thread;
    pthread_attr_t threadAttr;

    thread = (bthread*)calloc(1, sizeof(*thread));
    thread->pThreadFunc = startAddress;
    thread->pContext = arg;
    pthread_attr_init(&threadAttr);
    pthread_attr_setstacksize(&threadAttr, DEFAULT_STACK_SIZE);

    BUPnP_RegisterThread(thread);

    if ( pthread_create(&thread->threadID,  &threadAttr, pthreadProc, thread) != 0 )
        return NULL;

    return thread;
}

void bthread_destroy(bthread *thread)
{
    (void)pthread_cancel(thread->threadID);
    (void)pthread_join(thread->threadID, NULL);
    free(thread);
}

int bthread_join(bthread *thread, int timeout)
{
    UNUSED_ARGUMENT(timeout);
    pthread_join(thread->threadID, NULL);
    return 0;
}

pthread_mutex_t* bthread_mutex_create(void)
{
    int result;
    pthread_mutex_t* pMutex = (pthread_mutex_t*)malloc(sizeof(pthread_mutex_t));

    if (NULL == pMutex)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("Out of memory"));
        return NULL;
    }

    result = pthread_mutex_init(pMutex, NULL);
    if (result)
    {
        free(pMutex);
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("bthread_mutex_create error: %d", result));
        return NULL;
    }
    return pMutex;
}

void bthread_mutex_destroy( pthread_mutex_t *m )
{
    pthread_mutex_destroy(m);
    free(m);
}

int bthread_mutex_lock( pthread_mutex_t *m )
{
    return pthread_mutex_lock(m);
}

int bthread_mutex_unlock( pthread_mutex_t *m )
{
    return pthread_mutex_unlock(m);
}

static void uuid_seed_random(void)
{
    struct timeval    tv;
    gettimeofday( &tv, NULL );

    srand((getpid() << 16) ^ getuid() ^ tv.tv_sec ^ tv.tv_usec);
}


static void uuid_get_system_time(uint64_t *uuid_time)
{
    struct timeval tv;
    gettimeofday( &tv, NULL );

    /* This offset is the difference between UUID formatted time
       (Oct 15, 1582) and Unix formatted time (Jan 1, 1970) */
     *uuid_time = ( tv.tv_sec * 10000000 ) + ( tv.tv_usec * 10 ) +
        (unsigned long long)(0x01B21DD213814000);
}

static int uuid_create_random_number(void)
{
    return rand();
}

static void uuid_generate_random(char* out, size_t length)
{
    static uint8_t buf[12];
    static int inited = 0;
    static int clockSequence;
    uint64_t uuidTime;

    uuid_get_system_time(&uuidTime);

    if (!inited)
    {
        inited = 1;
        uuid_seed_random(); /* this will see for clock sequence and node */
        clockSequence = uuid_create_random_number();
        uuid_get_random_bytes(buf, sizeof(buf));
    }
    else if (uuidTime < lastTime)
    {
        /* detects that the local value of UTC has gone backward; this may be due
        to normal functioning of the DCE Time Service.  */
        clockSequence++;
    }

    snprintf(out, length-1, "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        (uint32_t)(uuidTime & 0xFFFFFFFF),
        (uint16_t)((uuidTime >> 32 ) & 0xFFFF),
        (uint16_t)((DCE_VERSION<<12) | ((uuidTime >> 48 ) &0x0FFF)),
        (((clockSequence & 0x3F00) >> 8) | DCE_VARIANT),
        (uint8_t)(clockSequence & 0xFF),
        buf[0],
        buf[1],
        buf[2],
        buf[3],
        buf[4],
        buf[5]
        );
}

int uuid_create( char* uuid, size_t length )
{
    uuid_generate_random(uuid, length);
    return 0;
}


static int uuid_get_random_fd(void)
{
    static int    fd = -2;
    int    result;

    if (fd == -2)
    {
        fd = open("/dev/urandom", O_RDONLY);
        if (fd == -1)
            fd = open("/dev/random", O_RDONLY | O_NONBLOCK);
        if (fd >= 0)
        {
            result = fcntl(fd, F_GETFD);
            if (result >= 0)
                fcntl(fd, F_SETFD, result | FD_CLOEXEC);
        }
    }
    return fd;

}

static void uuid_get_random_bytes(void *buf, int nbytes)
{
    int i, n = nbytes, fd;
    int count = 0;
    unsigned char *buffer = (unsigned char *) buf;

    fd = uuid_get_random_fd();

    if (fd >= 0)
    {
        while ((n > 0) && (count <= 16))
        {
            i = read(fd, buffer, n);
            if (i <= 0)
            {
                count++;
                continue;
            }
            n -= i;
            buffer += i;
            count = 0;
        }
    }

    /* Run the software rand function in case the /dev/random failed */
    buffer = (unsigned char*)buf;
    for (i = 0; i < nbytes; i++)
        *buffer++ ^= (rand() >> 7) & 0xFF;

    return;
}

uint32_t GetTickCount(void)
{
    struct timeval tv;

    gettimeofday(&tv, NULL);

    /* returns ms */
    return ((((tv.tv_sec * US_PER_SEC) + tv.tv_usec)) / MS_PER_SEC);
}

void bsleep(int ms)
{
    usleep(ms*1000);
}

int bthread_WaitForEvent(bthread_event* bEvent, int waitTimeMs)
{
    struct timeval now;
    struct timespec target;
    int result, status;

    gettimeofday(&now, NULL);

    target.tv_nsec = now.tv_usec * 1000 + (waitTimeMs%1000)*1000000;
    target.tv_sec = now.tv_sec + (waitTimeMs/1000);
    if (target.tv_nsec >= 1000000000) {
        target.tv_nsec -=  1000000000;
        target.tv_sec ++;
    }

    result = bthread_mutex_lock(bEvent->mutex);
    if (result != 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("bthread_mutex_lock: %d\n", result));
        assert(0);
    }

    status = pthread_cond_timedwait((pthread_cond_t*)&bEvent->event,
                bEvent->mutex,
                &target);

    result = bthread_mutex_unlock(bEvent->mutex);
    if (result != 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("bthread_mutex_unlock: %d\n", result));
        assert(0);
    }
    
    return status;
}

bthread_event* bthread_CreateEvent(void)
{
    int result;
    bthread_event *bEvent;
    pthread_condattr_t attr;

    bEvent = (bthread_event*)calloc(1, sizeof(*bEvent));

    bEvent->mutex = bthread_mutex_create();
    result = pthread_condattr_init(&attr);
    if (result!=0) {

        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("pthread_condattr_init: %d\n", result));
        assert(0);
    }

    result = pthread_cond_init( &bEvent->event, &attr);
    if (result!=0) {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("pthread_cond_init: %d\n", result));
        assert(0);
    }
    return bEvent;
}

void bthread_DestroyEvent(bthread_event* bEvent)
{
    int result;
    bthread_mutex_destroy(bEvent->mutex);

    result = pthread_cond_destroy(&bEvent->event);
    if (result != 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("pthread_cond_destroy: %d\n", result));
        assert(0);
    }
    free(bEvent);

}

void bthread_SetEvent(bthread_event* bEvent)
{
    int result;

    result = bthread_mutex_lock(bEvent->mutex);
    if (result != 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("bthread_mutex_lock: %d\n", result));
        assert(0);
    }

    result = pthread_cond_signal(&bEvent->event);
    if (result != 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("pthread_cond_signal: %d\n", result));
        assert(0);
    }

    result = bthread_mutex_unlock(bEvent->mutex);
    if (result != 0)
    {
        UPNP_DEBUG_ERROR(BUPnPDebugCategory_All, ("bthread_mutex_unlock: %d\n", result));
        assert(0);
    }
}

void bthread_ResetEvent(bthread_event* event)
{
    UNUSED_ARGUMENT(event);
    /* No work is needed here */
}

char *stristr ( const char *str, const char *subStr)
{
   char *p, *startn = 0, *np = 0;

    for (p = (char*)str; *p; p++) {
        if (np) {
            if (toupper(*p) == toupper(*np)) {
                if (!*++np)
                    return startn;
            } else
                np = 0;
        } else if (toupper(*p) == toupper(*subStr)) {
            np = (char*)subStr + 1;
            startn = p;
        }
    }
    return 0;
}



