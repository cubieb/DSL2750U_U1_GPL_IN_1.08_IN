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
#include <api/bmeconfig.h>
#if defined(USE_RTEMS) || defined(USE_FUSIV)
// #include <bspdefines.h>
#endif

#include <api/ipos.h>
#include <dapi/dapi_parms.h>
			   
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
#include <config.h>

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

#elif defined(USE_RTEMS)
#include <rtems.h>
#include <string.h>

#define		OS_SUCCESS  0
#define		OS_ERROR    -1

#define		ipos_sem_id			rtems_id	
			  
#define		IPOS_NO_WAIT		0
#define		IPOS_WAIT_FOREVER	0xFFFFFFFF			    			  

#undef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC  100
#define SYS_CLK_RATE     480

extern void genSeqStr (char seqTag, char* seqStr);

static inline	ipos_sem_id iposSemCreate()
{
  char seqStr[4];
  rtems_id semId = 0;

  genSeqStr('B', seqStr);
  rtems_semaphore_create(rtems_build_name('B', seqStr[0], seqStr[1], seqStr[2]),
                         1, RTEMS_FIFO | RTEMS_BINARY_SEMAPHORE | RTEMS_LOCAL,
                         1, &semId);
  return(semId);
}

static inline int iposSemTake(ipos_sem_id semId, int waitOption)
{
  rtems_unsigned32 option_set;

  if (rtems_semaphore_obtain(semId, (waitOption == IPOS_NO_WAIT)?RTEMS_NO_WAIT:RTEMS_WAIT,
                             (waitOption == IPOS_WAIT_FOREVER)?RTEMS_NO_TIMEOUT:waitOption) ==
      RTEMS_SUCCESSFUL)
    return(OS_SUCCESS);
  else
    return(OS_ERROR);
}

static inline int iposSemGive(ipos_sem_id semId)
{
	if (rtems_semaphore_release(semId) == RTEMS_SUCCESSFUL)
    return(OS_SUCCESS);
  else
    return(OS_ERROR);
}

#if 0
static inline int	iposIntLock()
{
	return(splimp());
}

static inline void iposIntUnlock(int intLevel )
{

	splx(intLevel);
}
#else
static inline int	iposIntLock()
{
  rtems_interrupt_level intLevel;

  rtems_interrupt_disable(intLevel);
	return((int)intLevel);
}

static inline void iposIntUnlock(int intLevel )
{
  rtems_interrupt_enable((rtems_interrupt_level)intLevel);
}
#endif

static inline void iposTaskDelay(int delay )
{
  rtems_task_wake_after((rtems_interval)delay);
}

static inline int iposU32HwRead (tBmeStruct *bmeStruct, UINT32 srcAddr,
                                 UINT32 *dataPtr)
{
  *dataPtr = *(UINT32 *)srcAddr;
  return (OS_SUCCESS);
}

static inline int iposU32HwWrite (tBmeStruct *bmeStruct, UINT32 destAddr,
                                  UINT32 *dataPtr)
{
  *(volatile UINT32 *)destAddr = *dataPtr;
  return (OS_SUCCESS);
}

static inline int iposBulkHwRead (tBmeStruct *bmeStruct, UINT32 srcAddr,
                                  BYTE *dataPtr, UINT32 numBytes)
{
  memcpy(dataPtr, (const void *)srcAddr, numBytes);
  return (OS_SUCCESS);
}

static inline int iposBulkHwWrite (tBmeStruct *bmeStruct, UINT32 destAddr,
                                   BYTE *dataPtr, UINT32 numBytes)
{
  memcpy((char *)destAddr, (const void *)dataPtr, numBytes);
  return (OS_SUCCESS);
}

#elif defined(USE_FUSIV)

#include <sys/types.h> 
#include <vxw_hdrs.h>
#include <unistd.h>

#define         OS_SUCCESS  0
#define         OS_ERROR    -1

#define         ipos_sem_id int

#define         IPOS_NO_WAIT      0
#define         IPOS_WAIT_FOREVER       -1

#define CLOCKS_PER_SEC   100
#define SYS_CLK_RATE     480

static inline   ipos_sem_id iposSemCreate()
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

static inline int       iposIntLock()
{
        return(OS_SUCCESS);
}

static inline void iposIntUnlock(int intLevel )
{
  return;
}

static inline void iposTaskDelay(int delay )
{
  usleep(delay*10000); /* one unit of delay = 10 msec */ 
  //usleep(delay);
}

static inline int iposU32HwRead (tBmeStruct *bmeStruct, UINT32 srcAddr,
                                 UINT32 *dataPtr)
{
  fusiv_bme_read((unsigned int *)srcAddr, (unsigned int *)dataPtr); 
  return (OS_SUCCESS);
}

static inline int iposU32HwWrite (tBmeStruct *bmeStruct, UINT32 destAddr,
                                  UINT32 *dataPtr)
{
  fusiv_bme_write(destAddr, dataPtr); 
  return (OS_SUCCESS);
}

static inline int iposBulkHwRead (tBmeStruct *bmeStruct, UINT32 srcAddr,
                                  BYTE *dataPtr, UINT32 numBytes)
{
  fusiv_bme_bulk_read((unsigned int *)srcAddr, numBytes, (unsigned int *)dataPtr); 
  return (OS_SUCCESS);
}

static inline int iposBulkHwWrite (tBmeStruct *bmeStruct, UINT32 destAddr,
                                   BYTE *dataPtr, UINT32 numBytes)
{
  fusiv_bme_bulk_write((unsigned int *)destAddr, numBytes, (unsigned int *)dataPtr); 
  return (OS_SUCCESS);
}

#else

/**************************************************************
**
**		This section must be modified to operate with other RTOS
**
**
**************************************************************/

#define		OS_SUCCESS  0
#define		OS_ERROR    -1

#define		ipos_sem_id int	
			  
#define		IPOS_NO_WAIT      0
#define		IPOS_WAIT_FOREVER	-1			    			  

#define CLOCKS_PER_SEC	500 /* system clock ticks per second */
#define SYS_CLK_RATE     480

static inline	ipos_sem_id iposSemCreate()
{
  return (0); 
}

static inline int iposSemTake(ipos_sem_id semId, int waitOption)
{
	return (OS_SUCCESS);
}

static inline int iposSemGive(ipos_sem_id semId)
{
	return (OS_SUCCESS);
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
  return;
}

#endif

#endif

