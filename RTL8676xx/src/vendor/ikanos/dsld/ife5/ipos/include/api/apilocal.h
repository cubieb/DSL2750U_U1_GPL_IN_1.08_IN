#ifndef		APILOCAL
#define		APILOCAL

/**************************************************************
$Workfile:   apilocal.h  $
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
$Date: 2008/07/18 11:09:31 $
***************************************************************/

					 
/* events for usr API BME state	*/

typedef	enum
{
	IPOS_BME_STARTED,
	IPOS_BME_RESTART

}	eipos_bmeapi_evnt;

typedef	enum
{

	IPOS_BME_RUN,
	IPOS_BME_IDLE

}	eipos_bmeapi_state;

													  

/* port user level API structure	*/

typedef	struct
{

	int portNum;						   			 /* the port idex; PortNum=index+1 */

/* autonomous message callback	*/
	U8					uidEocDataRelay;	
	iposCbEocDataRelay	pCbEocDataRelay;			/* callback function for data relay	*/
	
	U8					uidEocMsgRcv;
	iposCbEocMsgRcv		pCbEocMsgRcv;				/* callback function for EOC msg rcv	*/

	U8					uidEocEventLog;
	iposCbEocEventLog	pCbEocEventLog;				/* callback function for EOC Trace Messages	*/
    	
	U8					uidCntReport;
	iposCbCntReport		pCbCntReport;					/* periodic report	*/
  
  U8          uidPMReport;
  iposCbPmCounters   pCbPmCounters;         /* PM Counters  */
  
  U8          uidPmAlert;
  iposCbPmAlert   pCbPmAlert;         /* PM Alert  */

	U8					uidError;
	iposCbError		pCbError;				/* generic app error handler */
	U8          uidRetrievePortConfig;
  iposCbRetrievePort pCbPortConfig;   /* Retrieve Port Configuration. */
	U8          uidUpdateReport;
	iposCbUReport		pCbUpdateReport;   /* update report callback. */
#if defined(RTEMS_VX180) || defined(FUSIV_VX180)
    U8                  uidThrottleTraffic;
    iposCbCutTraffic    pCbThrottleTraffic;
#endif
} ipos_api_port;

						
						
																		
/* BME user level structure	*/																		

typedef	struct
{
	
	int	bmeNum;								/* the BME number: index+1	*/
	
	int	MaxPort;							/* the number of ports */
	
	tBmeStruct			*pBme;

#ifdef	IPOS_USE_RTOS
	ipos_sem_id			semBme;				/* semaphore for BME access protection */
#endif	

	eipos_bmeapi_state	bmeState;
	
/* BME global callback functions	*/

	U8					uidStatus;
	iposCbStatusReport 	pCbStatus;					/* Update report	*/
	U8          uidRetrieveSysConfig;
  iposCbRetrieveSys pCbSystemConfig;   /* Retrieve System Configuration. */
	ipos_api_port	 	port[IPOS_MAX_PORT_PER_BME + 1];



}ipos_api_bme;

										 
										 
/* API internalfunctions, not visible by user */

extern int	iposSetCallback(ipos_msg_buf   *pBuf, iposCbCtl pCb, ipos_port_id pid, U8 UID, U32 uPar);

extern iposCbCtl iposGetCallback(ipos_msg_buf *pBuf,ipos_port_id *pid, U8 *UID, U32 *uPar);

extern void	iposApiEnter(ipos_api_bme *pBme);

extern void	iposApiLeave(ipos_api_bme *pBme);

					
/* API for usr API level BME statemachine	*/

extern void	iposStateUpdate(ipos_api_bme *pApiBme, eipos_bmeapi_evnt evnt);

extern eipos_bmeapi_state	iposGetState(ipos_api_bme *pApiBme);

#endif					
