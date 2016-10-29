#ifndef 	IPOSPROTO
#define IPOSPROTO
/**************************************************************
$Workfile:   apiproto.h  $
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

/***************************************************************
** This C header file containes the function prototype for IPOS API
**
***************************************************************/


/*******************************************************
	
	callback prototype
	
*******************************************************/

/* generic callback format	*/

typedef	void 	(*iposCbCtl)(ipos_port_id portId, U8 UID, U32 uPar,  int status, void *par);
																			/* generic callback format	*/
/* Autonomous call back function prototypes	*/

 /* EOC_RELAY_DATA_STREAM	*/
typedef	void	(* iposCbEocDataRelay)(ipos_port_id	pid, U8 uid, U32 upar,int len, U8 *data);

/* EOC_MSG_RCV	 */
typedef	void	(* iposCbEocMsgRcv) (ipos_port_id	pid, U8 uid, U32 upar,int opCode, U8 *data);					
 
/* EOC Trace Message */
typedef	void	(* iposCbEocEventLog)(ipos_port_id	pid, U8 *data);

/* periodic report			*/
typedef	void	(* iposCbCntReport ) (ipos_port_id pid, U8 uid, U32 upar, int status, ipos_port_counters *pREPORT); 	

/* PM Report  */
typedef void  (* iposCbPmCounters ) (ipos_port_id pid, U8 uid, U32 upar, int status, ipos_pm_counters *pPmCntr);

/* PM Alert      */
typedef void  (* iposCbPmAlert ) (ipos_port_id pid, U8 uid, U32 upar, int status, ipos_pm_alert *pALERT);

typedef	void	(* iposCbStatusReport) (ipos_port_id pid, U8 uid, U32 upar, int status, ipos_system_status *pREPORT); 

struct tSystemProv; /* forward declaration */
typedef	void	(* iposCbRetrieveSys) (ipos_port_id pid, U8 uid, U32 upar, int status, struct tSystemProv *rxSysParms, 
                                                                                       struct tSystemProv *currSysParms); 
struct tPortProfile; /* forward declaration */
typedef	void	(* iposCbRetrievePort) (ipos_port_id pid, U8 uid, U32 upar, int status, struct tPortProfile *rxPortProfile,
                                                                                        struct tPortProfile *currPortProfile);

/* updated report			*/
typedef	void	(* iposCbUReport ) (ipos_port_id pid, U8 uid, U32 upar, int status, ipos_port_status_brief *pREPORT);   				

typedef void	(* iposCbError) (ipos_port_id pid, U8 uid, U32 upar, int errCode, int len, U8 *data);
/*******************************************************
	
	IPOS initialization:
		
		initialize iposAPI datastructures, callbacks,etc.
		
		initialize msgBuf
		
		initialize direct APIs
		
		Configure BME parameters based on definitions in  bmeconfig.h
	
*******************************************************/

int iposSysInit(void);


/**************************************************
 
	system entry point for API routines	
	
***************************************************/	

/* system entry point for receving polling */
int	iposEntryRcvPoll(int tick, int bmeNum);			/* tick is the polling period in msec	*/

/* system entry point for API expiration: used only when expiration needed		*/
int	iposEntryApiExp(int tick);			/* tick is the polling period in 100msec	*/


/*****************************************************

	utility functions: convert Bme Index to Bme Handle
	
******************************************************/

U32 iposGetBmeHandle(int i);



/********************************************************

	Autonomous Callback Registration		
	
*******************************************************/

/* EOC relay stream	*/
int iposSetCbEocDataStream(ipos_port_id	pid, U8 UID, iposCbEocDataRelay pCallbackFunc); 

/* EOC message		*/
int iposSetCbEocMsg(ipos_port_id  pid, U8 UID, iposCbEocMsgRcv pCallbackFunc); 

/* Eoc Event Log */
int iposSetCbEocEventLog(ipos_port_id pid, U8 UID, iposCbEocEventLog pCallbackFunc);

/* counter Report	*/
int iposSetCbCntReport(ipos_port_id	pid, U8 UID,  iposCbCntReport pCbReport);


/* PM Counters */
int iposSetCbPmCounters(ipos_port_id pid, U8 UID,  iposCbPmCounters pCbPmCntr);


/* PM Alert */
int iposSetCbPmAlert(ipos_port_id pid, U8 UID,  iposCbPmAlert pCbAlert);


/* status report	*/
int iposSetCbStatusReport(ipos_port_id	pid, U8 UID, iposCbStatusReport pCbReport);

int iposSetCbHostRetrievePort(ipos_port_id pid, U8 UID, iposCbRetrievePort pCbPortConfig);
int iposSetCbHostRetrieveBme(ipos_port_id pid, U8 UID, iposCbRetrieveSys pCbSystemConfig);



/* Updated Report	*/
int iposSetCbUReport(ipos_port_id	pid, U8 UID, iposCbUReport pCallbackFunc); 


/* error report	*/
extern int iposSetCbError(ipos_port_id	pid, U8 UID, iposCbError pCbError);


/********************************************************

	System	APIs		
	
*******************************************************/

/* control API	*/
int iposBmeStartBlk(int BmeNum, ipos_bme_load  *pBmeLoad,U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposBmeResetBlk(int BmeNum, ipos_bme_load  *pBmeLoad,U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

#ifdef	BME_CPE_20
int iposBmeConfigMemBlk(int BmeNum, ipos_emi_config	*pBmeCfg);

/* BME-CPE 2.0 GPIO APIs  */
int iposBmeGpioConfig(int BmeNum, U8 uiGpioPinNumber, U32 uiGpioDirection);
int iposBmeGpioWrite(int BmeNum, U8 uiGpioPinNumber, U32 uiGpioOut);
U32 iposBmeGpioRead(int BmeNum, U8 uiGpioPinNumber);
int iposBmeGpioMaskRead(int BmeNum, U32 *uiGpioMaskValue);

#endif

int iposBmeProvision(int bmeNum,ipos_bme_provision *pBmeProvision, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);


int iposBmeClearAllCounters(ipos_port_id pid, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);


int iposDeviceVersion(int BmeNum, U8 UID, U32 uPar, 
						void (*pCompleteFunc)(ipos_port_id pid, U8 uid, U32 uPar, int status, ipos_bme_ver *par));       

#ifdef	NOTIMPLEMENTED
int iposGetSelfTestResults(int BmeNum, U8 UID, U32 uPar,  
						void (* pCompleteFunc)ipos_port_id pid, U8 uid, U32 uPar, int status, ipos_tst_reaults *par));
#endif

int iposGetBmeStatus(int bmeNum, U8 UID, U32 uPar,  
						void (*pCompleteFunc)(ipos_port_id pid, U8 uid, U32 uPar, int status, ipos_system_status *par));       


int iposGetQosLevel(int bmeNum, U8 UID, U32 uPar,  
						void (*pCompleteFunc)(ipos_port_id pid, U8 uid, U32 uPar, int status, ipos_bme_qos_type *par));       


/********************************************************

	Port	APIs		
	
*******************************************************/

/* Port Control API */
int iposPortInstantiate(ipos_port_id pid, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposPortProvision(ipos_port_id pid, ipos_port_profile *pPortProvision, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposPortStart(ipos_port_id pid, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposPortStop(ipos_port_id pid, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposPortDestroy(ipos_port_id pid, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposPortReset(ipos_port_id pid, U8 portResetParm,U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

/* Enable periodic report	*/
int iposPeriodicReportsEnable(ipos_port_id	pid, ipos_report_config *pReportCfg, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposUpdatedReportEnable(ipos_port_id pid, U8 reportEnableParm, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposPmConfig (ipos_port_id pid, ipos_pm_config *pPmConfig, U8 UID, U32 uPar, iposCbCtl pCompleteFunc);

int iposPmTimer (ipos_port_id pid, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposPortPmTriggerFailures(ipos_port_id pid, ipos_pm_trigger *pTrigger, U8 UID, U32 uPar,
                              iposCbCtl pCompleteFunc);

int iposUpdateCounter(ipos_port_id pid, ipos_update_counter_req *pUpdateCounterReq, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

/* EOC API */
int iposEocControlSend(ipos_port_id pid, U8 opCode, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposEocSetData(ipos_port_id pid, U8 opCode, U8 *regData, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposEocDataReply(ipos_port_id pid,U8 opCode, U8 len, U8 *regData, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposEocSendDataStream(ipos_port_id pid, U8 len, U8 *regData, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposEocGetData(ipos_port_id pid, U8 opCode, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int	iposEocRelayData(ipos_port_id pid,  U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

int iposEocProtocolSet(ipos_port_id pid, U8 opCode, U8 *regData, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);
int iposEocProtocolGet(ipos_port_id pid, U8 opCode, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);
	
/* Bit Swap API		*/

int iposBitSwapReq(ipos_port_id pid, U8 opCode, U8 *msg, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

/* status API		*/

int iposPortStatusBrief(ipos_port_id pid, U8 UID, U32 uPar,  
						void (*pCompleteFunc)(ipos_port_id pid, U8 uid, U32 uPar, int status, ipos_port_status_brief *par));       

int iposPortStatusExtended(ipos_port_id pid, U8 UID, U32 uPar, 
						void (*pCompleteFunc)(ipos_port_id pid, U8 uid, U32 uPar, int status, ipos_port_status *par));       

int iposPortGetCounters(ipos_port_id pid, U8 UID, U32 uPar,  
						void (*pCompleteFunc)(ipos_port_id pid, U8 uid, U32 uPar, int status, ipos_port_counters *par));       

int iposPortGetPMCounters(ipos_port_id pid, U8 UID, U32 uPar,  
						void (*pCompleteFunc)(ipos_port_id pid, U8 uid, U32 uPar, int status, ipos_pm_counters *par));       

int iposPortQosGetCounters(ipos_port_id pid, U8 UID, U32 uPar,  
						void (*pCompleteFunc)(ipos_port_id pid, U8 uid, U32 uPar, int status, ipos_qos_counters *par));

int iposGetNeSNRAttn(ipos_port_id pid, U8 UID, U32 uPar,  
						void (*pCompleteFunc)(ipos_port_id pid, U8 uid, U32 uPar, int status, ipos_port_nesnrattn *par));       

int iposOptionalCmd(ipos_port_id pid, U8 UID, U32 buf, U32 uPar,  
		void (*pCompleteFunc)(ipos_port_id pid, U8 uid, U32 uPar, int status, ipos_opCmd_status *par));


/* Test API */
int iposLbStart(ipos_port_id pid, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);

extern int iposSendTestSignals(ipos_port_id, ipos_test_signals *, U8, U32, iposCbCtl);
extern int iposDetectTestSignals(ipos_port_id, ipos_test_signals *, U8, U32,  iposCbCtl);

/* QoS API */

int iposPortClassifierEnable(ipos_port_id pid,ipos_port_protocoltype_config *pClassEnable,U8 UID, U32 uPar, iposCbCtl pCompleteFunc);
int iposSystemProtocol2ClassMap(int bmeNum, ipos_bme_Protocol2ClassMap *pSysProtocol2ClassMap, U8 UID, U32 uPar,  iposCbCtl pCompleteFunc);
int iposPortProtocolConfig(ipos_port_id pid,ipos_port_protocoltype_config *pProtocolCfg,U8 UID, U32 uPar, iposCbCtl pCompleteFunc); 
int iposPortClass2QueueMap(ipos_port_id pid,iposClass2QueueParms *pClass2QueueMap,U8 UID, U32 uPar,iposCbCtl pCompleteFunc); 
int iposPortQueueConfig(ipos_port_id pid,ipos_queue_config *pPortQueuecfg,U8 UID, U32 uPar,iposCbCtl pCompleteFunc);


/* Debug API */
extern int iposBmeMon(ipos_port_id pid, U8 UID, U32 uPar,
                      iposCbCtl pCompleteFunc);

extern void iPosPrintVer(int bmeNum);
#endif

