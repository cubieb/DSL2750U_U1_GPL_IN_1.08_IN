#ifndef	OAMPDEFH
#define	OAMPDEFH
/**************************************************************
$Workfile:   oampdef.h  $
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



typedef	enum
{
	uIDLE	=0x00,						/* BME idle	*/
    uREADY,
	uDOWN	=0x40,				 		/* BME Down	*/
	uRUN	=0x80				   		/* BME RUN	*/
} eoamp_bme_state;


typedef	enum
{

	uENABLE  		=0x01, 	   			/* port provisioned	*/
	uACTIVATING		=0x02,			  	/* port under activation	*/
	uSHOWTIME		=0x03, 			 	/* port in Showtime		*/
    uSTOP           =0x04,              /* stop port */
    uLOOPBACK       =0x05,              /* loopback */
	uDISABLE		=0x0F 			  	/* port disabled	*/

} eoamp_port_state;


typedef	enum
{								   	   
	uCMD_IDLE		=0x00, 				 /* No Command out standing	*/
	uCMD_IN_PROGRESS=0x01,				 /* Non Blocking Command out standing	*/
	uCBLK_CMD_IN_PROGRESS =0x02		/* BME does not allow issue 
											new command while in this state */
} eoamp_cmd_state;


typedef	enum
{
	
	CMD_IN_PROGRESS,					/* command in progress	*/
	CMD_SUCCESS,					   	/* command success		*/
	CMD_FAILED,						  	/* command failed		*/
	
		
	eBME_START,							/* BME start completed	*/
	eBME_RESTART_INPROGRESS,            /* BME restart in progress	*/
	eBME_RESTART_COMPLETE,              /* BME restart completed	*/
	eBME_RESTART_FAILED,				/* BME restart failed	*/
	eSYSTEM_PROVISION,                 	/* BME provision completed	*/
	
	
	ePORT_START,			   			/* port started */ 		
	ePORT_START_COMPLETE,				/* port start completed	*/	
	ePORT_STOP,                         /* port stop completed	*/
	ePORT_INSTANTIATE,		 			/* port instantiate completed	*/
	ePORT_PROVISION,			   		/* port provision completed	*/
	ePORT_DESTROY,				 		/* port destroy completed	*/
	ePORT_RESET,				 		/* port reset completed	*/
	ePORT_PPREPORT_ENABLE,	   			/* periodic report  completed	*/
	eEOC_EVNT,                   		/* BME provision completed	*/
	eBIT_SWAP_EVNT,                   		/* Bit Swap Event	*/
	ePORT_STATUS_BRIEF,		 			/* status brief completed	*/
    ePORT_LB_START_COMPLETE,            /* port in the loopback mode */
    ePORT_CLASSIFIER_ENABLE,            /* port classifer enable */
    eSYS_PROTOCOL2ClASS_MAP,            /* bme protocol to class map */
    ePORT_PROTOCOL_CONFIG,              /* port protocol config */
    ePORT_CLASS2QUEUE_MAP,              /* port class to queue Map */
    ePORT_QUEUE_CONFIG,                 /* port queue config */
    ePORT_GET_QOS_INFO,                 /* Get Qos Counters */
    eSYS_GET_QOS_LEVEL,                 /* Get Qos Level */

    eSTOP_PORT
}eoamp_event;

/*
 * Do Not put any enumation value in between the already defined values
 * New defination must come before LINK_DOWN_UNKNOWN */

typedef enum
{
  LINK_DOWN_LPR        = 0,
  LINK_DOWN_PSTOP      = 1,
  LINK_DOWN_LOS_PM     = 2,
  LINK_DOWN_LOS_NONPM  = 3,       
  LINK_DOWN_LOF_PM     = 4,
  LINK_DOWN_LOF_NONPM  = 5,
  LINK_DOWN_ESE        = 6,
  LINK_DOWN_INI_CRC    = 7,
  LINK_DOWN_CRC        = 8,
  LINK_DOWN_FE_LPR     = 9,
  LINK_DOWN_FE_LOS     = 10,
  LINK_DOWN_FE_LOF     = 11,
  LINK_DOWN_FE_ESE     = 12,
  LINK_DOWN_FE_INI_CRC = 13,
  LINK_DOWN_FE_CRC     = 14,
  LINK_DOWN_UNKNOWN,
}eoamp_link_down;

#endif
