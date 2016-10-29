#ifndef	OAMPPROTOH
#define	OAMPPROTOH

/**************************************************************
$Workfile:   oampproto.h  $
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


extern oamp_bme	*getBmeOampHandle(int bmeNum);

/* oamp functions and definitions used by the UI	*/

extern int oampBmeStart(int bmeNum, U8 *pCodeAddr, U32 codeLen, U8 cmdTag, send_ack_info_t *psend_ack_info);

extern int oampBmeReset(int bmeNum, U8 *pCodeAddr, U32 codeLen, U8 cmdTag, send_ack_info_t *psend_ack_info);

extern int oampBmeProvosion(int bmeNum, ipos_bme_provision *pBmeProvision, U8 cmdTag, U32 cntRpt); 

extern int oampBmeGetVersion(int bmeNum, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int	oampPortProvision(ipos_port_id	*pPid,ipos_port_profile *pPortProvision, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int oampPortStart(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int oampPortInstantiate(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);


extern int oampPortStop(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int oampPortDestroy(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int oampPortReset(ipos_port_id *pPid, U8 resetParm, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

/* Enable periodic report   */
extern int	oampPeriodicReportsEnable(ipos_port_id	*pPid, ipos_report_config *pReport, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);


/* oamp functions for status report */
extern int oampPortGetStatusBrief(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);
extern int oampPortGetStatus(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);


/* oamp functions for get counters  */
extern int oampPortGetCounters(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

/* oamp function for Get Near END SNR attenuation   */
extern int oampPortGetNeSNRAttn(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

/* oamp functions for Bit Swap	*/
extern int oampBitSwapReq(ipos_port_id *pPid, U8 opCode, U8 *msg, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

/* oamp functions for EOC   */
extern int oampEocSetData(ipos_port_id *pPid, U8 opCode, U8 *regData, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int oampEocControlSend(ipos_port_id *pPid, U8 opCode, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int	oampEocDataReply(ipos_port_id	*pPid,U8 opCode, U8 len, U8 *regData, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int oampEocSendDataStream(ipos_port_id *pPid, U8 len, U8 *regData, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);


/* oamp functions for port statue, port counters and near end SNR   */

extern int oampPortGetStatusBrief(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int oampPortGetStatus(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int oampPortGetNeSnr(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);
extern int oampPortClrNeCntr(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int oampOptionalCmd(ipos_port_id *pPid, U8 cmdTag, U32 tag, int length, send_ack_info_t *psend_ack_info);


/* oamp functions for Performance Management (PM) */
extern int oampPortGetPMCounters(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int oampPmConfig(ipos_port_id *pPid, ipos_pm_config *pmConfig, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int	oampPortPmTriggerFailures(ipos_port_id	*pPid, ipos_pm_trigger *pmTrigger,
                                      U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);

extern int oampLbStart(ipos_port_id *pPid, U8 cmdTag, U32 cntRpt, int lpbkType, send_ack_info_t *psend_ack_info);
extern int oampSendSignal(ipos_port_id *pPid, ipos_test_signals *pTestSignals, U8 cmdTag, U32 cntRpt, send_ack_info_t *psend_ack_info);


/*  common functions used in OMAp functions		*/

/* functions related to the port and bme states	*/

extern void	oampStateResetAll(void);

extern void	oampPortStateReset(ipos_port_id pid);

extern void	oampBmeStateReset(int bmeNum);

extern eoamp_port_state	oampStateUpdate(ipos_port_id pid,eoamp_event	event);

extern eoamp_bme_state	oampGetBmeState(int bmeNum);

extern eoamp_port_state oampGetPortState(ipos_port_id pid);

extern eoamp_cmd_state	oampGetCmdState(int bmeNum);
				
extern void oampPortStateStop( int bmeNum, int portNum);

/* functions related to the performance handling	*/



#if 0
/* functions related to the port mapping: To be implemented	*/

extern int getPortId (ipos_port_id	 *pPid, int portNum);

extern int	setPortMapping();
				
extern int	showPortMapping();

#endif

extern void oampBmeMarkNotCrashed( int bmeNum );
extern void oampBmeMarkHasCrashed( int bmeNum, int bmeErrorCode );
extern int oampBmeHasItCrashed( int bmeNum );
extern int oampGetBmeErrorCode( int bmeNum );
#endif				
