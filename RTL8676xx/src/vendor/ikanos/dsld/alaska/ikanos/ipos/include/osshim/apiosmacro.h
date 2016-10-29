#ifndef	_APIOSMACRO_H_
#define	_APIOSMACRO_H_

/**************************************************************
$Workfile:   apiosmacro.h  $
***************************************************************
**
** FIRMWARE, Copyright (C) 2000-2002
** Ikanos Communications, Fremont, CA
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**
$Revision: 1.1 $
$Date: 2008/07/18 11:09:32 $
***************************************************************/
#include "bmeconfig.h"
#include "ipos.h"
			   
#define		MAX_MSG				100
#define		MAX_MSG_LEN			32

typedef void (*taskFunc)(int, ...);

#ifdef USE_VXWORKS

/**************************************************************
**
**		This section defines the OS dependencies for vxWorks
**
**
**************************************************************/
#include <vxWorks.h>
#include <sysLib.h>
#include <semLib.h>
#include <taskLib.h>
#include <time.h>
#include <intLib.h>

#define		OS_SUCCESS			OK
#define		OS_ERROR			ERROR

#define		ipos_sem_id			SEM_ID	
			  
#define		IPOS_NO_WAIT		NO_WAIT
#define		IPOS_WAIT_FOREVER	WAIT_FOREVER			    			  

static inline	ipos_sem_id iposSemCreate()
{

	return (semBCreate(SEM_Q_FIFO,SEM_FULL)); 

}

static inline int iposSemTake(ipos_sem_id semId, int waitOption)
{
	return (semTake(semId, waitOption));

}

static inline int iposSemGive(ipos_sem_id semId)
{
	return semGive(semId);
}

static inline int	iposIntLock()
{
	return(intLock());
}

static inline void iposIntUnlock(int intLevel )
{

	intUnlock(intLevel);
}

static inline void iposTaskDelay(int delay )
{
  taskDelay(delay);
}

#else

/**************************************************************
**
**		This section must be modified to operate with other RTOS
**
**
**************************************************************/
#include <sys/types.h> /*Header files for IXDP425. Everything below else part is for non VxWorks*/
#include <vxw_hdrs.h>
#include <unistd.h>

#define		OS_SUCCESS  0
#define		OS_ERROR    -1

#define		ipos_sem_id int	
			  
#define		IPOS_NO_WAIT      0
#define		IPOS_WAIT_FOREVER	-1			    			  

//#define CLOCKS_PER_SEC	500 /* system clock ticks per second */

static inline	ipos_sem_id iposSemCreate()
{
	return (semBCreate(SEM_Q_FIFO,SEM_FULL)); 
  //return (0); 
}

static inline int iposSemTake(ipos_sem_id semId, int waitOption)
{
	//return (OS_SUCCESS);
	return (semTake(semId, waitOption));
}

static inline int iposSemGive(ipos_sem_id semId)
{
	//return (OS_SUCCESS);
	return semGive(semId);
}

static inline int	iposIntLock()
{
	return(OS_SUCCESS);
}

static inline void iposIntUnlock(int intLevel )
{
  return;
}

static inline void iposTaskDelay(int delay )
{
  //return;
  usleep(delay);
}

#endif

#endif

