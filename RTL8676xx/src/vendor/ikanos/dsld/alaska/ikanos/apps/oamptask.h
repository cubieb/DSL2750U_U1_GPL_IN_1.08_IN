/**************************************************************
$Workfile:   oamptask.h  $
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

#ifndef	APP_OAMPTASK
#define	APP_OAMPTASK

#define	APP_OAMP_TASK_NUM	3		/* one oamp task for the sample app */

/* OAMP task function prototype	*/

typedef	 void ( *appOampFunc)(U32 arg1, U32 arg2, ipos_msg_buf *pBuf); 
                                                                                       
extern int	appOampJobAdd(int oampId, appOampFunc pFunc, U32 arg1, U32 arg2, ipos_msg_buf *par3);

extern int	appOampTaskInit(int numOampTasks);

#endif
