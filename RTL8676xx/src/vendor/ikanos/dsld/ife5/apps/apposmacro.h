#ifndef	_APPOSMACRO_H_
#define	_APPOSMACRO_H_

/**************************************************************
$Workfile:   apposmacro.h  $
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
$Date: 2008/07/18 11:08:38 $
***************************************************************/
#include <api/bmeconfig.h>
#include <osshim/apiosmacro.h>

#define		MAX_MSG				10
#define		MAX_MSG_LEN		32
#define SUCCESS (0)
typedef void (*taskFunc)(int, ...);

#ifdef USE_VXWORKS

#include "vxWorks.h"
#include "msgQLib.h"
#include "taskLib.h"
#include "tickLib.h"
#include "vme.h"
#include "ioLib.h"
#include "iosLib.h"
#include "bootlib.h"
#include "ftplib.h"
#include "fiolib.h"
#include "remlib.h"
#include "errnoLib.h"
#include "memLib.h"
#include "cacheLib.h"
#include "sysLib.h"
#include "config.h"
#include "intLib.h"
#include "logLib.h"
#include "vxLib.h"
#include "tyLib.h"
#include "arch/ppc/vxPpcLib.h"
#include "private/vmLibP.h"
#include "drv/multi/mdpm860Siu.h"
#include "drv/timer/timestampDev.h"
#include "sysLed.h"
#include "drv/mem/eeprom.h"
#ifdef   INCLUDE_NETWORK
	 #include "sysNet.h"
#endif  /* INCLUDE_NETWORK */
#include "hsiMdpPro8xxT.h"
												  
#define 	TASK_PRIORITY_1			70
#define 	TASK_PRIORITY_2			100
#define		TASK_PRIORITY_3	 		130			
#define		TASK_PRIORITY_4	 		160
#define		TASK_PRIORITY_5	  		190
#define		TASK_PRIORITY_6			220

#define		ipos_msg_id			MSG_Q_ID
#define		MSG_OPTION			MSG_Q_FIFO

/**************************************************************
**
**		This file defines the OS dependencies for vxWorks
**
**
**************************************************************/
static inline  int 	iposTaskSpawn(char *taskName, int priority, int stacksize,
								 taskFunc pTask, int arg1, int arg2, int arg3 )
{


	if(taskSpawn(taskName,priority, VX_SUPERVISOR_MODE, stacksize, (FUNCPTR) pTask, arg1, arg2, arg3 
			  ,0,0,0,0,0,0,0)==ERROR)
	{
		return(OS_ERROR);
	}
	else
	{
		return(OS_SUCCESS);
	}


}

static inline int iposTaskSuspend(int tid)
{
  return ((taskSuspend(tid) == OK)?OS_SUCCESS:OS_ERROR);
}

static inline ipos_msg_id  iposMsgQCreate()
{

	return(msgQCreate(MAX_MSG, MAX_MSG_LEN,MSG_OPTION));

}

static inline int  iposMsgQSend(ipos_msg_id msgId, U8 *pMsgBuf, int msgLen)
{

	if(msgQSend( msgId,  pMsgBuf, msgLen,	NO_WAIT,  MSG_PRI_NORMAL)==OK)
	{
		return (OS_SUCCESS);
	}
	else
	{
		return(OS_ERROR);
	}

}


static inline int iposMsgQReceive(ipos_msg_id msgId, U8 *pMsgBuf, int maxMsgLen)
{

	if(msgQReceive(msgId, pMsgBuf, maxMsgLen, WAIT_FOREVER)==ERROR)
	{
		return(OS_ERROR);

	}else
	{
		return(OS_SUCCESS);
	}

}


static inline U32 iposTickGet()
{
	return( tickGet() );
}
extern int sysClkRateGet (void);
#elif defined(USE_RTEMS)

#include <sys/param.h>
#include <mqueue.h>
#include <sys/fcntl.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sysconf.h>
#include <fusiv_vdsl_common.h>

#define 	TASK_PRIORITY_1			70
#define 	TASK_PRIORITY_2			100
#define		TASK_PRIORITY_3	 		130			
#define		TASK_PRIORITY_4	 		160
#define		TASK_PRIORITY_5	  	190
#define		TASK_PRIORITY_6			220

//#define		ipos_msg_id			rtems_id
#define		ipos_msg_id			mqd_t

#define RENAME_START

extern rtems_task rtemsIposConduitTask( rtems_task_argument arg );
extern void * saveTaskInfo(taskFunc taskEntryPoint, int arg1, int arg2, int arg3);

static inline  int 	iposTaskSpawn(char *taskName, int priority, int stacksize,
                                  taskFunc pTask, int arg1, int arg2, int arg3)
{
  char seqStr[4];
  rtems_id tskId = 0;
  int returnStatus = OS_ERROR;

  genSeqStr('T', seqStr);
  if (rtems_task_create(rtems_build_name('T', seqStr[0], seqStr[1], seqStr[2]),
                        priority, stacksize, RTEMS_TIMESLICE | RTEMS_PREEMPT,
                        RTEMS_NO_FLOATING_POINT | RTEMS_LOCAL, &tskId) ==
      RTEMS_SUCCESSFUL)
  {
    if (rtems_task_start(tskId, rtemsIposConduitTask,
                         (rtems_task_argument)saveTaskInfo(pTask, arg1,
                                                            arg2, arg3)) ==
        RTEMS_SUCCESSFUL)
      returnStatus = OS_SUCCESS;
  }
  return(returnStatus);
}

static inline int iposTaskSuspend(int tid)
{
  rtems_id id = (tid == 0)?RTEMS_SELF:(rtems_id)tid;

  return ((rtems_task_suspend(id) == RTEMS_INVALID_ID)?OS_ERROR:OS_SUCCESS);
}

static inline ipos_msg_id  iposMsgQCreate()
{
#ifdef USE_POSIX_INTERFACE
  char seqStr[8];
  mqd_t msgId = 0;
  struct mq_attr attr = {0, MAX_MSG, MAX_MSG_LEN, 0};

  seqStr[0] = 'M';
  genSeqStr('M', &(seqStr[1]));
  if ((msgId = mq_open(seqStr,  O_RDWR | O_CREAT, 0x777, &attr)) == -1)
    msgId = 0;
#else
  char seqStr[4];
  rtems_id msgId = 0;

  genSeqStr('M', seqStr);
  rtems_message_queue_create(rtems_build_name('M', seqStr[0],
                                              seqStr[1], seqStr[2]),
                             MAX_MSG, MAX_MSG_LEN,
                             RTEMS_FIFO | RTEMS_LOCAL, &msgId);
#endif
	return(msgId);
}

static inline int  iposMsgQSend(ipos_msg_id msgId, U8 *pMsgBuf, int msgLen)
{
#ifdef USE_POSIX_INTERFACE
  if (mq_send(msgId, pMsgBuf, msgLen, 0) == 0)
#else
	if (rtems_message_queue_send(msgId, pMsgBuf, msgLen) == RTEMS_SUCCESSFUL)
#endif
		return (OS_SUCCESS);
	else
		return(OS_ERROR);
}


static inline int iposMsgQReceive(ipos_msg_id msgId, U8 *pMsgBuf, int maxMsgLen)
{
#ifdef USE_POSIX_INTERFACE
  unsigned int qPri;

  if (mq_receive(msgId, pMsgBuf, maxMsgLen, &qPri) != -1)
#else
	if (rtems_message_queue_receive(msgId, pMsgBuf, &maxMsgLen, RTEMS_WAIT,
                                  RTEMS_NO_TIMEOUT)==RTEMS_SUCCESSFUL)
#endif
		return(OS_SUCCESS);
	else
		return(OS_ERROR);
}

static inline U32 iposTickGet()
{
  rtems_interval ticks = 0;

  rtems_clock_get(RTEMS_CLOCK_GET_TICKS_SINCE_BOOT, &ticks);
	return(ticks);
}
extern int sysClkRateGet (void);
#elif defined(USE_FUSIV)
#include <sys/types.h>
#include <linux/param.h>
#include <v2pthread.h>
#include <sys/times.h>
// Need to check in Linux #include <sysconf.h>
// Need to check in Linux #include <fusiv_vdsl_common.h>

#define VX_SUPERVISOR_MODE      0x0001

#define 	TASK_PRIORITY_1			70
#define 	TASK_PRIORITY_2			100
#define		TASK_PRIORITY_3	 		130			
#define		TASK_PRIORITY_4	 		160
#define		TASK_PRIORITY_5	  		190
#define		TASK_PRIORITY_6			220

#define		ipos_msg_id			MSG_Q_ID
#define		MSG_OPTION			MSG_Q_FIFO

/**************************************************************
**
**		This file defines the OS dependencies for LinuxS
**
**
**************************************************************/
static inline  int 	iposTaskSpawn(char *taskName, int priority, int stacksize,
								 taskFunc pTask, int arg1, int arg2, int arg3 )
{


	if(taskSpawn(taskName,priority, VX_SUPERVISOR_MODE, stacksize, (FUNCPTR) pTask, arg1, arg2, arg3 
			  ,0,0,0,0,0,0,0)==ERROR)
	{
		return(OS_ERROR);
	}
	else
	{
		return(OS_SUCCESS);
	}


}

static inline int iposTaskSuspend(int tid)
{
  return ((tskSuspend(tid) == SUCCESS)?OS_SUCCESS:OS_ERROR);
}

static inline ipos_msg_id  iposMsgQCreate()
{

	return(msgQCreate(MAX_MSG, MAX_MSG_LEN,MSG_OPTION));

}

static inline int  iposMsgQSend(ipos_msg_id msgId, U8 *pMsgBuf, int msgLen)
{

	if(msgQSend( msgId,  pMsgBuf, msgLen,	NO_WAIT,  MSG_PRI_NORMAL)==OK)
	{
		return (OS_SUCCESS);
	}
	else
	{
		return(OS_ERROR);
	}

}


static inline int iposMsgQReceive(ipos_msg_id msgId, U8 *pMsgBuf, int maxMsgLen)
{

	if(msgQReceive(msgId, pMsgBuf, maxMsgLen, WAIT_FOREVER)==ERROR)
	{
		return(OS_ERROR);

	}else
	{
		return(OS_SUCCESS);
	}

}


static inline U32 iposTickGet()
{
	struct tms buf;

        buf.tms_utime = 0;
        buf.tms_stime = 0;
        buf.tms_cutime = 0;
        buf.tms_cstime = 0;
        return ( times(&buf) );
}

#define sysClkRateGet()         sysconf(_SC_CLK_TCK)
#endif


#endif

