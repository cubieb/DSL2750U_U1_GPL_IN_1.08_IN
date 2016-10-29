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
$Date: 2008/07/18 11:08:39 $
***************************************************************/
#ifdef USE_VXWORKS /* commented VxWorks related headers and included Linux related headers - IXDP425 */
#include "vxWorks.h"
#include "msgQLib.h"
#include "taskLib.h"
#include "tickLib.h"
#else

#include <sys/types.h>
#include <linux/param.h>
#include <v2pthread.h>
#include <sys/times.h>

#define VX_SUPERVISOR_MODE	0x0001
#endif
#include "apiosmacro.h"
												  
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


static inline U32 iposTickGet(void) /* IXDP425 */
{
#ifdef USE_VXWORKS
	return( tickGet() );
#else
	struct tms buf;

	buf.tms_utime = 0;
	buf.tms_stime = 0;
	buf.tms_cutime = 0;
	buf.tms_cstime = 0;
	return ( times(&buf) );
#endif
}

#ifdef USE_VXWORKS /* IXDP425 */
extern int sysClkRateGet (void);
#else
#define sysClkRateGet() 	sysconf(_SC_CLK_TCK)	
#endif

#endif

