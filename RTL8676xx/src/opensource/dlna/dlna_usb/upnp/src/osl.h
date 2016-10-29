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
 * $brcm_Workfile: osl.h $
 * $brcm_Revision: 7 $
 * $brcm_Date: 9/15/09 5:50p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/osl.h $
 * 
 * 7   9/15/09 5:50p dliu
 * SW7405-2482: Can't use the key word interface
 * 
 * 6   9/2/09 10:56a dliu
 * SW7405-2482: Add network interface to UPnP settings
 * 
 * 5   7/7/09 2:40p dliu
 * PR55165: Add stristr parser function
 * 
 * 4   6/26/09 6:25p ajitabhp
 * PR55165 : Additional Status Codes and Include files for OSL layer
* 
***************************************************************************/
#ifndef OSL_H__
#define OSL_H__

#include <stdio.h>        
#include <stdlib.h>        
#include <stddef.h>
#include <string.h>        
#include <stdarg.h>
#include <signal.h>
#include <limits.h>
#include <errno.h>
#include <assert.h>



#if defined(LINUX)
#include <arpa/inet.h>        
#include <ctype.h>        
#include <fcntl.h>        
#include <netdb.h>        
#include <netinet/tcp.h>    
#include <sys/socket.h>        
#include <sys/stat.h>        
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>
#include <sys/poll.h> 
#include <net/if.h>
#include <unistd.h>        
#include <time.h>
#elif defined(WIN32)
#include <winsock2.h>        
#include <ws2tcpip.h>
#include <time.h>
#include <io.h>
#else
#error "Unsupported Operating System!"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if defined(WIN32)

#define snprintf    _snprintf
#define strcasecmp    _stricmp
#define strncasecmp    _strnicmp
#define strdup        _strdup
#define UTSINFO_LEN    32

typedef unsigned bthread_mutex_t;
typedef unsigned bthread_t;
typedef HANDLE bthread_event;

struct utsname {
    char sysname[UTSINFO_LEN];
    char version[UTSINFO_LEN];
};

int uname (struct utsname *uts);

char* strsep(char **stringp, const char *delim);

char* strndup(const char *src, size_t count);

#define BUPNP_WAIT_TIMEOUT WAIT_TIMEOUT
#define BUPNP_EVENT_SIGNALED WAIT_OBJECT_0

#elif defined(LINUX)

#define closesocket close

uint32_t GetTickCount(void);

typedef pthread_t bthread_t;
typedef pthread_mutex_t bthread_mutex_t;

typedef struct bthread_event
{
    pthread_cond_t event;
    bthread_mutex_t* mutex;
}bthread_event;

#define BUPNP_WAIT_TIMEOUT ETIMEDOUT
#define BUPNP_EVENT_SIGNALED 0

#endif

char* _BUPnPStrndup(const char *src, size_t count, char* file, int line);

char* _BUPnPStrdup(const char *src, char* file, int line);


#define BUPnPStrndup(a,b) _BUPnPStrndup(a,b, __FILE__, __LINE__)
#define BUPnPStrdup(a)    _BUPnPStrdup(a,__FILE__,__LINE__)

typedef enum bthreadState
{
    bthreadState_eRunning,
    bthreadState_eStopped
} bthreadState;

typedef struct bthread
{
    bthread_t threadID;
    void (*pThreadFunc)(void *);
    void* pContext;
    bthreadState state;
} bthread;

char *stristr ( const char *str, const char *subStr);

int host_getipaddress(const char* ipInterface, char *ipAddress, size_t length);

int uuid_create(char *uuid, size_t length);

bthread* bthread_create(void (*startAddress)(void*), void* arg);

void bthread_destroy(bthread *thread);

int bthread_join(bthread *thread, int timeout);

bthread_mutex_t* bthread_mutex_create(void);

void bthread_mutex_destroy( bthread_mutex_t *m ); 

int bthread_mutex_lock( bthread_mutex_t *m ); 

int bthread_mutex_unlock( bthread_mutex_t *m ); 

bthread_event* bthread_CreateEvent(void);

void bthread_SetEvent(bthread_event*);

void bthread_ResetEvent(bthread_event*);

int bthread_WaitForEvent(bthread_event* event, int waitTime);

void bthread_DestroyEvent(bthread_event*);


void bsleep(int ms);

#ifdef __cplusplus
}
#endif

#endif
