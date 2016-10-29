 
/******************************************************************************
		Workfile:   pmhandler.h   
*******************************************************************************
 
 FIRMWARE,	Copyright (C) 2000-2002 Ikanos Communication, Fremont, CA 
			All rights reserved. 
 
			The information and contents of this file are the proprietary information
      of Ikanos Communication and may not be disclosed or used without the
      formal written approval of Ikanos Communication. 
*******************************************************************************
$Revision: 1.1 $ 
$Date: 2008/07/18 11:08:39 $ 
******************************************************************************/
 
 
#ifndef _PMHANDLER_H_ 
#define _PMHANDLER_H_ 
 
 
/* Include files */ 
#include "apistruct.h" 

/* Start : For PM Application */
#include "ipostimer.h"
/* End : For PM Application */
extern void InitPMHandler(int BmeNum, int PortNum);

extern void copyPMCountersToOamp (ipos_pm_counters *pPmCntr,ipos_port_id pid);
 
extern int getIniMap(ipos_port_id pid, int *IniIndex, int *IniSlow, int *IniFast);
	
extern UINT32 GetINICrcThresStatusFlag(uPortStruct *uEPortStatus, int ethnetPortNum, int ptmfPort,  U8 lineHold6ptOk); 


/* Start : For PM Application */

extern int		getLineNum(ipos_port_id, int *lineNum);			/* portId->lineNum   */

void updatePMCounters24Hrs (ipos_pm_counters *pPm24hrs, ipos_pm_counters *pPmCntr, ipos_port_id pid);

void displayPMCounters24Hrs (ipos_port_id pid);

void sendPmTimer15minsComplete(U8 UID, U32 uPar);

int tmr15minsExp(U32 arg1, U32 arg2);

int CreateTimer15Mins();

int KillTimer15Mins(int bmenum);

/* End : For PM Application */
 
#endif	/* _PMHANDLER_H_ */ 
