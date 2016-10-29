/**************************************************************
$Workfile:   dapi_cmds.h  $
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

/**** Prototypes ***/

/******************************************/ 
/* Host Processor API Commands Prototypes */
/******************************************/

extern eApiRv  	BmeStructInit(tBmeStruct	*pBme);		/* initilization of the BME structure	*/

extern eApiRv 	BmeStart(tBmeStruct *pBme);
extern eApiRv 	BmeReset(tBmeStruct *pBme);
extern eApiRv	BmeMemoryRead(tBmeStruct *pBme);	
extern eApiRv	BmeMemoryWrite(tBmeStruct *pBme);

extern eApiRv	SystemProvision(tBmeStruct *pBme);

extern eApiRv	PortProvision (tBmeStruct *pBme, int PortNum);
extern eApiRv	PortStart (tBmeStruct *pBme, int PortNum);
extern eApiRv	PortStop (tBmeStruct *pBme, int PortNum);	
extern eApiRv	PortDestroy (tBmeStruct *pBme, int PortNum);
extern eApiRv	PortReset(tBmeStruct *pBme, int PortNum);
extern eApiRv	PortInstantiate (tBmeStruct *pBme, int PortNum);
extern eApiRv	ClearPrimitiveCounts(tBmeStruct *pBme, int PortNum);
extern eApiRv	EmiConfig(tBmeStruct *pBme, int PortNum);
extern eApiRv   GetNeSnrAttn(tBmeStruct *pBme, int PortNum);
extern eApiRv	SystemStatus (tBmeStruct *pBme);
extern eApiRv	PortStatusBrief(tBmeStruct *pBme, int PortNum);		
extern eApiRv	GetCounters(tBmeStruct *pBme, int PortNum);
extern eApiRv	GetPMCounters(tBmeStruct *pBme, int PortNum);
extern eApiRv	PmConfig(tBmeStruct *pBme, int PortNum);
extern eApiRv	PmSendTimer(tBmeStruct *pBme);
extern eApiRv	PmTriggerFailures(tBmeStruct *pBme, int PortNum);
extern eApiRv	PeriodicReportsEnable	(tBmeStruct *pBme, int PortNum);
extern eApiRv	UpdatedReportsEnabled(tBmeStruct *pBme, int PortNum);
extern eApiRv	PortStatusExtended(tBmeStruct *pBme, int PortNum);	
extern eApiRv	DeviceVersions (tBmeStruct *pBme, int PortNum);
extern eApiRv	GetSelftestResults(tBmeStruct *pBme, int PortNum);
extern eApiRv	EocControlSend (tBmeStruct *pBme, int PortNum);
extern eApiRv	EocSetData(tBmeStruct *pBme, int PortNum);	
extern eApiRv	BitSwapReq(tBmeStruct *pBme, int PortNum);	
extern eApiRv	EocDataReply (tBmeStruct *pBme, int PortNum);
extern eApiRv	EocSendDataStream (tBmeStruct *pBme, int PortNum);
extern eApiRv	EocGetData (tBmeStruct *pBme, int PortNum);
extern eApiRv	SendSignals (tBmeStruct *pBme, int PortNum);
extern eApiRv	DetectSignals(tBmeStruct *pBme, int PortNum);
extern eApiRv	SetLoopback (tBmeStruct *pBme, int PortNum);
extern eApiRv 	SendDebugMon (tBmeStruct *pBme, int PortNum);
extern eApiRv 	GetOpCmdStatus(tBmeStruct *pDABme, int portNum);
extern eApiRv   EocProtocolSet(tBmeStruct *pBme, int PortNum);
extern eApiRv   EocProtocolGet(tBmeStruct *pBme, int PortNum);
extern int      UpdateCounter( tBmeStruct *pBme, int portNum, tUpdateCounterReq *pUpdateCounterReq );
extern	int pollTxMsgReady (tBmeStruct *pBme);	/* polling for BME ready for TX	*/
extern	int pollRxMsgReady (tBmeStruct *pBme);	/* polling for any rcv msg ready	*/
extern	int pollDmaDone (tBmeStruct *pBme);		/* polling for dma ready	*/


// extern HWAPI_RET_VAL ReceiveBmeMessage(tBmeStruct *pBme);	/* receive a msg	*/




