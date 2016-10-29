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
 * $brcm_Revision: 4 $
 * $brcm_Date: 9/30/09 3:26p $
 * 
 * Module Description:
 * 
 * Revision History:
 * 
 * $brcm_Log: /AppLibs/upnp/src/win32/osl.c $
 * 
 * 4   9/30/09 3:26p dliu
 * SW7405-2482: Add stristr function
* 
***************************************************************************/
#include "upnp.h"
#include <process.h>
#include "osl.h"    

#if !defined(UNUSED_ARGUMENT)
#define UNUSED_ARGUMENT(x) (void)x
#endif

void bsleep(int ms)
{
	Sleep(ms);
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


char* strndup(const char *src, size_t count)
{
	char* pBuffer = (char*)malloc(count+1);
	cmsUtl_strncpy(pBuffer, src, count);
	pBuffer[count] = 0;
	return pBuffer;
}

char* strsep(char **stringp, const char *delim)
{ 
	char *s; 
	const char *spanp; 
	int c, sc; 
	char *tok;  
	if ((s = *stringp) == NULL) 
		return (NULL); 
	for (tok = s;;) 
	{ 
		c = *s++; spanp = delim;
		do 
		{ 
			if ((sc = *spanp++) == c)
			{ 
				if (c == 0) 
					s = NULL; 
				else s[-1] = 0; 
				*stringp = s; 
				return (tok);
			}
		} while (sc != 0);
	}
} 

int uname (struct utsname *uts)
{
	OSVERSIONINFO osver;

	memset (uts, 0, sizeof (*uts));

	osver.dwOSVersionInfoSize = sizeof (osver);
	GetVersionEx (&osver);

	switch (osver.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_NT: /* NT, Windows 2000 or Windows XP */
		if (osver.dwMajorVersion == 6)
			cmsUtl_strcpy (uts->sysname, "Windows Vista");   /* Vista */
		else if (osver.dwMajorVersion == 5)
		{
			if (osver.dwMinorVersion == 1)
				cmsUtl_strcpy (uts->sysname, "Windows XP");   /* XP */
			else
				cmsUtl_strcpy (uts->sysname, "Windows 2000"); /* 2k */
		}
		if (osver.dwMajorVersion <= 4)
			cmsUtl_strcpy (uts->sysname, "Windows NT"); /* NT */
		break;

	case VER_PLATFORM_WIN32_WINDOWS: /* Win95, Win98 or WinME */
	case VER_PLATFORM_WIN32s: /* Windows 3.x */
		cmsUtl_strcpy (uts->sysname, "Windows");
		break;
	}

	sprintf (uts->version, "%ld.%02ld", osver.dwMajorVersion, osver.dwMinorVersion);

	return 0;
}

int host_getipaddress(const char* ipInterface, char *ipAddress, size_t length)
{
	char hostName[256];
	struct hostent* hostEnt;

	UNUSED_ARGUMENT(ipInterface);

	if ( gethostname(hostName, 255) == -1 )
		return -1;

	hostEnt = gethostbyname(hostName);
	if ( hostEnt == NULL )
		return -1;

	cmsUtl_strncpy( ipAddress, inet_ntoa( *(struct in_addr *)hostEnt->h_addr_list[0] ), length-1 );

	return 0;
}

int uuid_create( char* str, size_t length )
{
	RPC_STATUS status;
	UUID uuid;
	char* rpcstr = NULL;
	status = UuidCreate(&uuid);
	if ( status == RPC_S_OK )
	{
		status = UuidToStringA(&uuid, (RPC_CSTR*)&rpcstr);
		if ( status == RPC_S_OK )
			cmsUtl_strncpy(str, rpcstr, length-1);
	}

	RpcStringFreeA((RPC_CSTR*)&rpcstr);

	return status;
}

void pThreadProc( void *data )
{
    bthread* thread = (bthread*)data;
    thread->pThreadFunc(thread->pContext);
}


bthread* bthread_create(void (*startAddress)(void*), void* arg)
{
	bthread* thread;

	thread = (bthread*)calloc(1, sizeof(*thread));
    thread->pThreadFunc = startAddress;
    thread->pContext = arg;

	thread->threadID = _beginthread(pThreadProc, 0, thread);
	return thread;
}

void bthread_destroy(bthread *thread)
{
	UNUSED_ARGUMENT(thread);
	/* Don't cal CloseHandle since the os will do it automatically */
	/* CloseHandle(thread); */
}

int bthread_join(bthread *thread, int mstimeout)
{
	if ( WaitForSingleObject( (HANDLE)thread->threadID, mstimeout ) == WAIT_TIMEOUT )
	{
		/* unsafe call but at this point we have no choice */
		TerminateThread((HANDLE)thread, 0);
		return -1;
	}

	return 0;
}

bthread_mutex_t* bthread_mutex_create()
{
	return (bthread_mutex_t*)CreateMutex(NULL, FALSE, NULL);
}

void bthread_mutex_destroy( bthread_mutex_t *m ) 
{
	CloseHandle(m);
}

int bthread_mutex_lock( bthread_mutex_t *m )
{
	return ( WaitForSingleObject(m, INFINITE) == WAIT_OBJECT_0 ) ? 0 : -1;
}

int bthread_mutex_unlock( bthread_mutex_t *m )
{
	return (ReleaseMutex(m)) ? 0 : -1;
}

int bthread_WaitForEvent(bthread_event* event, int waitTime)
{
	return WaitForSingleObject((HANDLE)event, waitTime);
}

bthread_event* bthread_CreateEvent()
{
	return (bthread_event*)CreateEvent(NULL, true, false, NULL);
}

void bthread_DestroyEvent(bthread_event* event)
{
	CloseHandle((HANDLE)event);
}

void bthread_SetEvent(bthread_event* event)
{
	SetEvent((HANDLE)event);
}

void bthread_ResetEvent(bthread_event* event)
{
	ResetEvent((HANDLE)event);
}


