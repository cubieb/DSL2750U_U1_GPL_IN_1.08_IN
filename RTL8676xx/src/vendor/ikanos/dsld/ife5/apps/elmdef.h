/**************************************************************
$Workfile:   elmdef.h  $
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

#ifndef		ELMDEFH
#define		ELMDEFH

/* put definitions related to Event Log Management
   used outside of the implementation here */

#include <api/apitype.h>
#include <api/apiproto.h>

#define	 DEFAULT_EVENT_LOG_SIZE		98000

#define  ELM_ERROR_BASE			0xa000
#define  ELM_ERROR			(ELM_ERROR_BASE)
#define  ELM_ERROR_CKSUM		(ELM_ERROR_BASE + 1)

/* do not confuse with eBmeState in ipos/include/dapi/dapi_parms.h */

typedef enum eELMBmeState
{
	eELM_BME_IS_RESPONSIVE,
	eELM_BME_IS_HUNG
} eELMBmeState;

typedef enum eELMTypeEventLog
{
	eELM_NEW_BME_LOG = 1,
	eELM_SAVED_BME_LOG,
	eELM_HOST_CPU_LOG,
	eELM_CURRENT_BME_LOG = 90
} eELMTypeEventLog;

typedef	void (* iposELMCmdCb)(ipos_port_id pid, U8 uid, int status, void *pPar);

extern U8 *sysGetEventLogAddr( int portNum );		/* in elm/elmsys.c */

extern U32 iposSizeofEventLog( char *pEventLog, U32 eventLogSize );
extern U16 iposCountofEntriesInEventLog( char *pEventLog, U32 eventLogSize );

/* Do not call this from the BME Handler or other time critical task
   Do not call this from an ISR
   Brief test showed it took over 500 ms to complete */

extern int iposGetBmeEventLog(
	int bmeNum,
	char *pEventLog,
	U32 eventLogSize,
	eELMBmeState eCurrentBmeState,
	U8 uid,	
	U32 uPar,
	iposCbCtl pCompleteFunc
);

extern int	iposELMXferInProgress( int portIndex );

#ifdef VTU_O
extern int
iposGetRemoteEventLog
(
	ipos_port_id pid,
	char * pEventLog,
	unsigned int eventLogSize,
	eELMTypeEventLog typeOfLog,
	U8 uid,
	U32 uPar,
	iposELMCmdCb pCompleteFunc
);

extern int iposSetCbRemoteBmeReset( ipos_port_id pid, U8 uid, iposELMCmdCb pCb );
#endif

#ifdef VTU_R
extern int iposNotifyBmeCrash( ipos_port_id pid, int bmeErrorCode, U8 uid, U32 uPar, iposELMCmdCb pCompleteFunc );
#endif
#endif   /* ELMDEFH */
