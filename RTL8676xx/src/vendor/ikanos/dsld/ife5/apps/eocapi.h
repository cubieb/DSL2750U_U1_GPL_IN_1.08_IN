/**************************************************************
$Workfile:   eocapi.h  $
***************************************************************
**
** Host Application, Copyright (C) 2000-2002
** Ikanos Communication, Fremont, CA 
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of 
** Ikanos Communication
**
***************************************************************

$Revision: 1.1 $
$Date: 2008/07/18 11:08:38 $
**************************************************************/

#ifndef EOCAPIH
#define EOCAPIH

#include <api/apitype.h>
#include <api/apistruct.h>
#include <osshim/apiosmacro.h>
#include <rfm/ipostimer.h>

#include "appconfig.h"
#include "portmap.h"

#define EOC_NO_PROTOCOL		0
#define IPOS_MAX_PROTOCOLS	3

#define	IPOSEOC_STATE_CMD_IDLE		1
#define IPOSEOC_STATE_CMD_INPROGRESS	2

/* MAX_MSG_SIZE is 256 - 6 byte header common to all BME messages, not seen by EOC applications */

#define  MAX_MSG_SIZE  250

#define	IPOS_MSG_COMMAND		0
#define	IPOS_MSG_RESPONSE		1
#define	IPOS_MSG_FILE			2
#define	IPOS_MSG_UNKNOWN		(-1)

#define	IPOS_MSG_DATA_BLOCK			0x80
#define	IPOS_MSG_DATA_ACK			0x81

#define	IPOS_ACK_CMD	0x00
#define	IPOS_ACK_ACK	0x01
#define	IPOS_ACK_NACK	0x02

#define IPOS_MSGHDR_PT          0
#define IPOS_MSGHDR_MSGTYPE     1
#define IPOS_MSGHDR_MSGLEN      2
#define IPOS_MSGHDR_UID         3
#define IPOS_MSGHDR_ACK		4
#define IPOS_MSGHDR_DATA	5

#define IPOS_NO_RESEND		0

/* events that happen when sending a command (IPOS_ACK_CMD) message */

#define	IPOSEOC_EVNT_CMD_SENT		1
#define	IPOSEOC_EVNT_RESP_RCVD		2
#define	IPOSEOC_EVNT_RESP_EXPIRED	3
#define	IPOSEOC_EVNT_RESET		4

#define IPOSEOC_ERROR_BASE		0x4000
#define	IPOSEOC_ERROR_TIMEOUT		(IPOSEOC_ERROR_BASE + 7)	/* LSB compatible with RFM */


typedef	struct
{
	U8	pt;
	U8	msgType;	/* msgtype in standard header	*/
	U8	length;		/* length of the message: 3-250		*/

} ipos_msg_hdr;

typedef	struct
{
	ipos_msg_hdr	genericHdr;	/* generic hdr		*/
    
	U8		uid;		/* user identifier			*/
	U8		ack;		/* 0: initiating, 1: positive ack; >2: error code */

} ipos_ctrl_hdr;

typedef  void (*iposEocCb)( ipos_port_id pid, U8 uid, int status, void *pData, int dataLen );
typedef  void (*iposEocHandler)( ipos_port_id pid, int status, void *pData, int dataLen );

typedef struct {
	ipos_port_id	 pid;
	ipos_sem_id	 semCmd;
	ipos_tmr_info	*ptmrRsnd;			/* resend timer	*/
	iposEocCb	 pCmdCb;			/* callback on command completion / timeout */

	U8		 msgRsn[ MAX_MSG_SIZE ];	/* the message just sent */
	U8		 msgLen;			/* length of message just sent */

	U8		 resendInterval;		/* number of iPOS timer cycles to wait before resend */
	U8		 resendCount;			/* the resend counter	*/

	U8		 stateCmd;			/* command state	*/
	U8		 uid;				/* user ID passed with command message	*/
} iposEocPerPort, *pIposEocPerPort;

/* EOC protocol is declared to be 16 bits to permit "virtual protocols / messages" where
   the protocol or message type is determined from inspecting the entire message */

/* Note if protocol type of 0 (EOC_NO_PROTOCOL) becomes a real protocol,
   a new field will need to be added to distinguish an available entry. */

typedef struct {
	iposEocPerPort	 perPort[ IPOS_MAX_PORTS + 1 ];
	iposEocHandler	 pRemoteCb;			/* handler when message arrives from remote port */
	U16		 eocProtocol;
	U8		 uid;
} iposEocPerProtocol, *pIposEocPerProtocol;

extern pIposEocPerProtocol	iposEocGetPerProtocol( U16 eocProtocol );
extern pIposEocPerPort		iposEocGetPerPort( ipos_port_id pid, U16 eocProtocol );

/* iposEocRegHandler with protocol type, user ID, callback routine */

extern int	iposEocRegHandler( U16 protocol, U8 uid, iposEocHandler pProtocolHandler );

extern void	iposEocCmdSetCtrlHdr( ipos_ctrl_hdr *pHdr, U8 eocProtocol, U8 msgType, U8 ackType, U8 uid);
extern int	iposEocEncodeCtrlHdr( ipos_ctrl_hdr *pHdr, U8 **pPmsg );

extern int	iposEocGetMsgClass( U8 *pEocMsg );
extern int	iposEocDecodeCtrlHdr( U8 *pMsgBuf, ipos_ctrl_hdr *pHdr );

/* iposEocCmdCoreSend with port ID, message address, message length,
		           user ID, resend count, resend interval, callback routine */

extern int	iposEocCmdCoreSend(
	ipos_port_id pid,
	U8 *eocMsg,
	int lenMsg,
	U8 uid,
	int resendCount,
	int resendInterval,
	iposEocCb pCmdCb
);

extern int	iposEocCmdCoreResp( ipos_port_id pid, U16 eocProtocol, U8 *pMsg, int lenMsg );
extern void	iposEocCmdCoreReset( pIposEocPerPort perPort );
extern void	iposEocCmdCoreResend( U32 arg1, U32 arg2 );
extern int	iposEocCmdStateUpdate( pIposEocPerPort perPort, int Event );

#endif /* EOCAPIH */
