/**************************************************************
$Workfile:   eventlog.h  $
***************************************************************
**
** FIRMWARE, Copyright (C) 2000-2002
** Ikanos Communication, Fremont, CA
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**
***************************************************************/

#ifndef  _EVENTLOG_H_    
#define _EVENTLOG_H_

#include "time.h"

#define MAX_EVENT_LOGS	1000
#define MAX_EVENT_PARMS 5

void EventLog(const char *output, ...);

typedef struct tEvent
	{
	U32		time;
	char	*EventString;
	S32	VarList[MAX_EVENT_PARMS];
	struct tm  	TimeStamp;
	} tEvent;

extern tEvent EventBuffer[MAX_EVENT_LOGS];

extern int FirstEvent;
extern int LastEvent;

void PrintEventLogs(send_ack_info_t *psend_ack_info);

void InitEventLogs(send_ack_info_t *psend_ack_info);

// User event logs
#define MAX_USER_LOGS	100
#define MAX_USER_PARMS 5

void UserLog(const char *output, ...);

extern tEvent UserLogBuffer[MAX_EVENT_LOGS];

extern int FirstUserEvent;
extern int LastUserEvent;

void PrintUserLogs(send_ack_info_t *psend_ack_info);

void InitUserLogs(send_ack_info_t *psend_ack_info);

#endif
