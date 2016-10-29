/**************************************************************
$Workfile:   elmapp.h  $
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

#ifndef		ELMAPPH
#define		ELMAPPH

#include "elmdef.h"
#include "elmlocal.h"

#define DEFAULT_EVENT_LOG_COUNT  1000

typedef enum eELMEventLogState
{
	eELM_VALID_EVENT_LOG,
	eELM_INVALID_EVENT_LOG
} eELMEventLogState;

/* instantiate one of these per event log buffer in your system */

typedef struct						/* app_elm */
{
	ipos_port_id	remoteSourceEventLog;
	int		localSourceEventLog;
	int		fromFlashMemory;

	int		countEventsToDisplay;
} app_elm;


typedef	struct						  /* app_co_elm */
{
	ipos_port_id	 pid;

	ipos_elm_co	*pCoPe;		/* ELM object			*/

/* 	local file info		*/
	
	/* U8		*fileAddr;	*/
	iposelm_event_log_info	localFileInfo;

	iposelm_event_log_info	rmFileInfo;	/* remote file info	*/

	int	displayLevel;

} app_co_elm;


typedef	struct						 /* app_cpe_elm */
{
	ipos_port_id	pid;

	ipos_elm_cpe	*pCpePe;	/* ELM object			*/

	/* BME connection info		*/
	int		currConnectionState;
	int		cntConnectionAttempt;
	
	/* file info in progress	*/
	iposelm_event_log_info	 fileInfo;
	/*U8			*fileAddr;		  file address	*/

	/* display option		*/
	int	displayLevel;		/* 0: minimum display, 1: maximum display	*/

} app_cpe_elm;

#endif  /*  ELMAPPH */
