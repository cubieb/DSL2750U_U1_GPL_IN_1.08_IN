#ifndef	RFMLOCALH
#define	RFMLOCALH
/**************************************************************
$Workfile:   rfmlocal.h  $
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
$Date: 2008/07/18 11:08:51 $
***************************************************************/

#include	"ipostimer.h"															  
typedef	void (* iposRFMCmdCb)(ipos_port_id pid, U8 uid, int status, void *pPar);


/* common structure for both CO and CPE	*/

typedef	struct
{

	ipos_port_id	pid;					/* the port id	*/
	
	ipos_sem_id	semCmd;
 	
	U8		stateCmd;				/* command state	*/
	
	U8		cntRsnd;						/* the resnd counter	*/
#if 0
	U32		tmrRsnd;	   			 		/* Handle to resend timer	*/
#endif
	
	ipos_tmr_info	*ptmrRsnd;				/* resend timer	*/

	U8		resendTick;						/* number of ticks to resend	*/
	U8		msgLen;
	U8		msgRsn[MAX_MSG_SIZE];		   	/* the message to be resent	*/
	
	U8		uidResp;
	U8 		respStruct[MAX_MSG_SIZE];		/* the place for parsing  responding structure */ 	
	
	U8		uPar;							/* UID for the response	*/
	iposRFMCmdCb	pCbCmd;					/* the callback for command response	*/

}	ipos_rfm_cmd_handler;


/**************************************************************
	The structure for CO_PE
***************************************************************/	
typedef	struct
{

	ipos_port_id	pid;	 			/* the port id for the EOC channel */
		
	
	/* members related to the file transfer	*/
	
	U8		sid;						/* session id	*/
	
	U16		currBlkIdx;					/* the current block index	*/
	U16		lastBlkIdx;

	iposrfm_data_block	currBlk;		/* the current data block	*/

	U8		stateiFftp;					  	/* the iFFTP state	*/
    S8		flgAck[MAX_NUM_DATA_BLOCKS];	/* the flg array for received ack 
												-1: the ack received.>0 the times of resend */
    U8	   	*firmwareAddr;				/* the starting address of the firmware	*/
	iposrfm_file_info	fileInfo;

	U32		cntAck;			   				/* the acknoledgement received	*/
	U32		cntDataBlock;					/* the datablock sent	*/
	ipos_tmr_info	*ptmrExp;				/* file transfer expiration timer	*/

	/* members related to the command/response primitive	*/
	
	ipos_rfm_cmd_handler	cmdHandler;

	/* members related to the event callback function	*/

	U8	uidEvntError;
	iposRFMCOEvntError 	pEvntError;

	U8	uidEvntFileSave;
	iposRFMCOEvntFileSave	pEvntFileSave;

	U8	uidEvntTransferComplete;
	iposRFMCOEvntTransferComplete pEvntTransferComplete;

	int	displayLevel;

} ipos_rfm_co;



/**************************************************************
	The structure for CPE_PE
***************************************************************/	
typedef	struct
{

	ipos_port_id	pid;	 			/* the port id for the EOC channel */
	U8		*pEocMsg; 					/* the pointer to the current EOC msg */

	/* members related to the file transfer	*/
	
	U8		sid;
	U8		stateiFftp;					/* the iFFTP state	*/
    
	
	
	U8		*addrFirmware;			/* the starting address of the firmware	*/
	iposrfm_file_info	fileInfo;
	iposrfm_firmware_ver	firmwareReplaced;
	
	U32		cntDataBlk;				/* the datablock sent	*/
	ipos_tmr_info	*ptmrExp;				/* file transfer expiration timer	*/

	/* members related to the command/response primitive	*/

	ipos_rfm_cmd_handler	cmdHandler;

	/* members related to the event callback function	*/
	U8		uidEvntError;
	iposRFMCPEEvntError	pEvntError;

	iposRFMCPEEvntVersionReq	pEvntVersionReq;

	iposRFMCPEEvntTransferReq	pEvntTransferReq;

	iposRFMCPEEvntActivationReq	pEvntActivationReq;

	iposRFMCPEEvntTransferCompleteReq	pEvntTransferCompleteReq;

	int	displayLevel;

}	ipos_rfm_cpe;

/* RFM utilities function for the command/response function	*/


/* CO/CPE state machine	*/
int		iposRFMCmdStateUpdate(ipos_rfm_cmd_handler *pCmd, int	Event);
int		iposRFMCmdGetState(ipos_rfm_cmd_handler *pCmd);


/* Message manipulation	*/

int		MsgCheckClass(U8 *pEocMsg);
int		msgMatchCmdResp(U8	*pResp, U8	*pCmd);


/* get the object from the PID	*/
ipos_rfm_cpe	*iposRFMGetCpePe(ipos_port_id	pid);
ipos_rfm_co		*iposRFMGetCoPe(ipos_port_id	pid);

/* get the object from the index	*/
ipos_rfm_co		*iposRFMIndxGetCo( int index);
ipos_rfm_cpe	*iposRFMIndxGetCpe( int index);

/******************************************* 
	the message parser function prototype	
********************************************/

/************************************************
		from msg to structure
		
		input: pMsgBuf,
		Output:	pStructBuf, msgType
		
		return 	ipos_success or errorcode
					
*********************************************/
int	iposRFMMsg2Struct(U8 *pMsgBuf,	void	* pStuctBuf, U8 *msgType);


/************************************************
		from structure to msg
		
		input:	pStructBuf, msgType
		output:	pMsgBuf
		
		return 	ipos_success or errorcode
					
*********************************************/

int	iposRFMStruct2Msg(void *pStructBuf, U8 msgType, U8 *pMsgBuf, U8 *msgLen);



/* ifftp routines for CO	*/
int	iposRFMCOiFftpInit(ipos_rfm_co		*pCoPe);

int	iposRFMCOiFftpStart(ipos_port_id	pid, U8 sid);
int	iposRFMCOiFFtpStop(ipos_port_id pid);
int	iposRFMCOiFftpHandler(ipos_rfm_co	*pCoPe, U8	*pEocData);


/* ifftp routines for CPE	*/
int	iposRFMCPEiFftpInit(ipos_rfm_cpe	*pCpePe);

int	iposRFMCPEiFftpStart(ipos_port_id pid, U8 sid, U8 *addrFirmware);
int	iposRFMCPEiFftpComplete(ipos_port_id pid);
int	iposRFMCPEiFftpHandler(ipos_rfm_cpe	*pCpePe, U8	*pEocMsg);

#endif				  
