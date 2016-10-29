#ifndef		ELMLOCALH
#define		ELMLOCALH

/**************************************************************
$Workfile:   elmlocal.h  $
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
$Date: 2008/07/18 11:08:50 $ 
***************************************************************/

/* put definitions related to Event Log Management
   used only within of the implementation here */

#include "appconfig.h"
#include "ipostimer.h"
#include "eocapp.h"
#include "elmdef.h"

/* maximum number of the data blocks: assuming filesize < 200K	*/	

#define		MAX_NUM_DATA_BLOCKS	1000

/* The maximum msg size: limited by the HIC interface */			 

#define		MAX_MSG_SIZE	250

#define		MAX_STRUCT_SIZE	300

#define	IPOS_DATA_MSG_LEN	250		/* msg Len for Data	*/
#define	IPOS_DATA_BLK_SIZE	242		/* file block size: 244+6 byte hdr=250 byte */

/* Definition of the protocol type for RFM		*/

#define	IPOSELM_PT			0x21

/* Definition of the message class	*/

#define	IPOSELM_MSG_COMMAND			0x00
#define	IPOSELM_MSG_RESPONSE			0x01
#define	IPOSELM_MSG_FILE			0x02						   

/* definition of the msgs header byte position  */

#define IPOSELM_MSGHDR_PT                               0
#define IPOSELM_MSGHDR_MSGTYPE                  1
#define IPOSELM_MSGHDR_MSGLEN                   2
#define IPOSELM_MSGHDR_UID                              3
#define IPOSELM_MSGHDR_ACK                              4

/* definition for iposrfm_data_block	*/
#define	IPOSELM_FILE_SID				3
#define	IPOSELM_FILE_SEQ				4
#define	IPOSELM_FILE_DATA				6

#define	IPOSELM_FILE_ACK1				4
#define	IPOSELM_FILE_ACK2				6

/**************************************************************
	command/response states
**************************************************************/
#define		IPOSELM_STATE_CMD_IDLE		1
#define		IPOSELM_STATE_CMD_INPROGRESS	2

#define	IPOSELM_MSG_CMDRESP_START		0x01
#define	IPOSELM_MSG_GET_EVENT_LOG		0x01
#define	IPOSELM_MSG_SEND_EVENT_LOG		0x02
#define	IPOSELM_MSG_FILE_TRANSFER		0x03
#define	IPOSELM_MSG_FILE_TR_COMPLETE		0x04
#define	IPOSELM_MSG_BME_WAS_RESET		0x05
#define	IPOSELM_MSG_CMDRESP_END			0x05

#define	IPOSELM_MSG_DATA_BLOCK			0x80
#define	IPOSELM_MSG_ACK				0x81

#define	IPOSELM_ACK_CMD				0x00
#define	IPOSELM_ACK_ACK				0x01
#define	IPOSELM_ACK_NACK			0x02

#define NO_INFO				0
#define EVENT_LOG_SELECTION_INFO	1
#define EVENT_LOG_INFO			2
#define SID_INFO			3
#define XFER_COMPLETE_CMD_INFO		4
#define XFER_COMPLETE_RESP_INFO		5
#define BME_ERROR_CODE_INFO		6

#define	IPOSELM_DATABLK_RESEND	6		/* exceed 6 times? error declared	*/

/**************************************************
	Time to declare file transfer expired
***************************************************/

#define	IPOSELM_FILE_EXPIRATION_TICK	160	/* ca. 30 seconds	*/

/**************************************************************
	command/response events
**************************************************************/

#define		ELM_EVNT_CMD_SENT		1
#define		ELM_EVNT_RESP_RCVD		2
#define		ELM_EVNT_RESP_EXPIRED		3


/**************************************************************
	iFftp states

        For ELM CO, a third state represents the CO
        requested the event log from the CPE but
        has received no data packets
**************************************************************/
#define		IPOSELM_IFFTP_CO_ACTIVE			1
#define		IPOSELM_IFFTP_CO_IDLE			2
#define		IPOSELM_IFFTP_CO_REQSTD			3
				  
#define		IPOSELM_IFFTP_CPE_ACTIVE   		1
#define		IPOSELM_IFFTP_CPE_IDLE			2

/**************************************************************
        event log transfer state

        these are specific to ELM
**************************************************************/
#define  IPOSELM_CO_XFER_IDLE		1
#define  IPOSELM_CO_REQ_XFER_START	2
#define  IPOSELM_CO_XFER_IN_PROGRESS	3

#define  IPOSELM_CPE_XFER_IDLE		1
#define  IPOSELM_CPE_GET_EVENT_LOG	2
#define  IPOSELM_CPE_XFER_IN_PROGRESS	3


typedef	struct
{
	U8	pt;				/* protocol type. 0x21 for the header	*/
	U8	msgType;			/* msgtype for elm			*/
	U8	length;				/* length of the message: 3-250		*/

} iposelm_msg_hdr;

typedef	struct
{
	iposelm_msg_hdr	genericHdr;	/* generic hdr		*/
    
	U8	uid;			/* user identifier			*/
	U8	ack;			/* 0: initiating, 1: positive ack; >2: error code */

} iposelm_ctrl_hdr;

typedef struct
{
	U8	eventLogOption;
} iposelm_event_log_choice;

typedef	struct
{
	iposelm_ctrl_hdr ctrlHdr;
	iposelm_event_log_choice eventLogChoice;

} iposelm_select_event_log_req;

/* event log transfer response information */

typedef struct
{
	U32	size;			/* the size of the event log in bytes */
	U32	checkSum;		/* the checksum for the event log */
} iposelm_event_log_info;

typedef	struct
{
	iposelm_ctrl_hdr ctrlHdr;
	iposelm_event_log_info eventLogInfo;

} iposelm_select_event_log_resp;

/* file transfer complete information */

typedef	struct
{
	U32		dataBlockSent;			/* number of dataBlock sent	*/
	U32		byteSent;			/* number of bytes sent	*/
} iposelm_transfer_info;

typedef	struct
{
	iposelm_ctrl_hdr	ctrlHdr;
	iposelm_transfer_info transferInfo; 	/* file transfer completion info from CPE */
} iposelm_xfer_complete_req; 

typedef	struct
{
	iposelm_ctrl_hdr	ctrlHdr;
	iposelm_transfer_info transferInfo; 	/* file transfer completion info from CPE */
	iposelm_event_log_info	rcvdEventLog;	/* the file just received: Checksum	*/
} iposelm_xfer_complete_resp;

/* structure for the file transfer	*/

typedef	struct
{
	iposelm_msg_hdr	genericHdr;
	U8	sid;		 			/* session id	*/
	U16	seq;					/* sequence number	*/
	U8	*pData;			   		/* data block	*/
} iposelm_data_block;

typedef	struct
{
	iposelm_ctrl_hdr ctrlHdr;
	U8		 sid;			/* define the session ID */
} iposelm_xfer_event_log_req;

typedef struct
{
	U32	bmeErrorCode;
} iposelm_bme_crash_report;

typedef	struct
{
	iposelm_ctrl_hdr		ctrlHdr;
	iposelm_bme_crash_report	crashReport;
} iposelm_bme_crash_req;

/*******************************************************************************/

typedef	struct
{
	ipos_port_id	pid;				/* the port id	*/
	
	ipos_sem_id	semCmd;
 	
	U8		stateCmd;			/* command state	*/
	
	U8		cntRsnd;			/* the resnd counter	*/
	
	ipos_tmr_info	*ptmrRsnd;			/* resend timer	*/

	U8		resendTick;			/* number of ticks to resend	*/
	U8		msgLen;
	U8		msgRsn[MAX_MSG_SIZE];		/* the message to be resent	*/
	
	U8		uidResp;
	U8 		respStruct[MAX_MSG_SIZE];	/* the place for parsing  responding structure */ 	
	
	U8		uPar;				/* UID for the response	*/
	iposELMCmdCb	pCbCmd;				/* the callback for command response	*/
} ipos_elm_cmd_handler;

/**************************************************************
	The structure for CO_PE
***************************************************************/	
typedef	struct						/* ipos_elm_co */
{
	ipos_port_id	pid;	 			/* the port id for the EOC channel */
		
	/* members related to the file transfer	*/
	
	U8		sid;				/* session id	*/
	
	U16		currBlkIdx;			/* the current block index	*/
	U16		lastBlkIdx;

	U8		stateiFftp;			/* the iFFTP state	*/
	U8	   	*eventLogAddr;		/* the starting address of the event log       */
	iposelm_event_log_info	fileInfo;

	U32		cntAck;			   	/* the acknoledgement received	*/
	U32		cntDataBlock;			/* the datablock sent	*/
	ipos_tmr_info	*ptmrExp;			/* file transfer expiration timer	*/

	int	displayLevel;

	/* members related to the command/response primitive	*/
	
	ipos_elm_cmd_handler	cmdHandler;

	iposELMCmdCb	pCbEventLogComplete;		/* callback when event log received */
	iposELMCmdCb	pCbRemoteBMEReset;	 /* callback when remote BME reported reset */

	U8	stateXfer;
} ipos_elm_co;

/**************************************************************
	The structures for CPE_PE
***************************************************************/	

typedef void (*iposELMGetEventLogReq ) (ipos_port_id pid, U8 uid, iposelm_event_log_choice *pEventLogChoice );

typedef	struct						/* ipos_elm_cpe */
{

	ipos_port_id	pid;	 			/* the port id for the EOC channel */
	U8		*pEocMsg; 			/* the pointer to the current EOC msg */

	/* members related to the file transfer	*/
	
	U8		sid;
	U16		currBlkIdx;			/* the current block index	*/
	U16		lastBlkIdx;

	iposelm_data_block	currBlk;		/* the current data block	*/

	U8		stateiFftp;			/* the iFFTP state	*/
	
	S8		flgAck[MAX_NUM_DATA_BLOCKS];	/* the flg array for received ack */
						/* -1: the ack received.>0 the times of resend */
	U8		*addrEventLog;		/* the starting address of the event log */
	iposelm_event_log_info	fileInfo;
	
	U32		cntAck;			   	/* the acknoledgement received	*/
	U32		cntDataBlk;			/* the datablock sent	*/
	ipos_tmr_info	*ptmrExp;			/* file transfer expiration timer	*/

	/* members related to the command/response primitive	*/

	ipos_elm_cmd_handler	cmdHandler;

	/* members related to the event callback function	*/

	iposELMGetEventLogReq	pEvntGetEventLogReq;

	int	displayLevel;
	U8	stateXfer;

} ipos_elm_cpe;


/* from RFM PE programming */

extern int	msgMatchCmdResp(U8 *pResp, U8 *pCmd);
extern U32	iposRFMGetCheckSum( U8 *eventLogAddr, U32 thisEventLogSize );

/* ELM PE programming */

extern int
iposELMCmdCoreReq(
	ipos_elm_cmd_handler *pCmd,
	int cmdType,
	void *pMsgStruct,
	int resendTimes,
	U8 uid,
	void *pCb
);

extern int	iposELMCmdCoreResp( ipos_elm_cmd_handler *pCmd, U8 *pMsgRcvd );
extern int	iposELMCmdInit(ipos_elm_cmd_handler *pCmd, ipos_port_id pid);
extern int	iposELMCmdSetCtrlHdr(iposelm_ctrl_hdr *pHdr, U8	msgType, U8 ackType, U8 uid);
extern int	iposELMCmdStateUpdate(ipos_elm_cmd_handler *pCmd, int Event);
extern int	iposELMMsg2Struct( U8 *pMsgBuf, void *pStructBuf, U8 *pMsgType);
extern int	iposELMSendMsg(ipos_port_id pid, U8 *pMsg, U8 msgLen );
extern int	iposELMStruct2Msg(void *pStructBuf, U8 msgType, U8 *pBuf );

extern int	msgCheckElmClass(U8 *pEocMsg);

/* ELM PE programming for CPE */

#ifdef VTU_R
extern ipos_elm_cpe	*iposELMGetCpePe(ipos_port_id	pid);
extern ipos_elm_cpe	*iposELMIndxGetCpe( int index );

extern int iposELMCPEGetEventLogResp(
	ipos_port_id pid,
	U8 uid,
	U8 status,
	U32 theEventLogSize,
	U32 theEventLogCheckSum
);

/* components of the iFftp engine required by deferred processing */

extern int cpeStart( ipos_elm_cpe *pCpePe );
extern int cpeGetDataBlk(ipos_elm_cpe *pCpePe);
extern int cpeSendDataPkt(ipos_elm_cpe *pCpePe, iposelm_data_block *pData, U16 BlkIndex);
extern int cpeErrorHandler(ipos_elm_cpe *pCpePe, int status);
#endif  /* VTU_R */

/* ELM PE programming for CO */

#ifdef VTU_O
extern int	iposELMCOGetEventLogResult( ipos_elm_cmd_handler *pCmd );
extern ipos_elm_co	*iposELMGetCoPe(ipos_port_id	pid);
extern ipos_elm_co	*iposELMIndxGetCo( int index );
#endif  /* VTU_R */

#endif   /* ELMLOCALH */
