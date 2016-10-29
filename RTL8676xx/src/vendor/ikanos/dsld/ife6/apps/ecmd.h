#ifndef	ECMDH
#define ECMDH

/**************************************************************
$Workfile:   ecmd.h  $
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
$Date: 2008/07/18 11:08:36 $
***************************************************************/

/* Return values for Host APIs */
typedef enum eApiRv
	{
	IN_PROGRESS = 1,	/* Message has been successfully sent. Response from BME is 
							expected later.*/
	FAIL,				/* API execution fails */
	PARMS_UNDEFINED,	/* Some API parameters undefined */
	TxCmdTIMEOUT,		/* DoTxCmd fails, timeout */
	COMPLETE,			/* API has been executed successfully. If any BME messages 
							were sent, the transmission was successful and the action was 
							performed successfully on the BME*/
	CMD_TIMER_EXPIRE,	/* Maximum response time specified for the API command expired.
							No response from BME. Time out the command at the 
							APPLICATION level */
	AUTONOMOUS,			/* Autonomous message from BME, no ApiRv related to message*/
	TxCmdFAIL
    } eApiRv;

			 
#endif			 
