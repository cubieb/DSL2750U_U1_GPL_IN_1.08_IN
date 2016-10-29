#ifndef		MSGBUF
#define		MSGBUF

/**************************************************************
$Workfile:   msgbuf.h  $
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

#define		IPOS_MSG_USED		0
#define		IPOS_MSG_NOTUSED	1

#define		IPOS_MSG_CALLBACK_SET		1
#define		IPOS_MSG_CALLBACK_NOTSET	0
								
#include "time.h"

#include "apistruct.h"

typedef struct ipos_msg_buf
{
	void  	*pNextMsg;
	
	int		index;			/* msg buffer index		*/
	int		flag;			/* IPOS_MSG_USED or IPOS_MSG_NOTUSED	*/
	int		len;

	int		cbSet;			/* callback setflag: IPOS_MSG_CALLBACK_SET or IPOS_MSG_CALLBACK_NOTSET */
	int		tick;	
		
    send_ack_info_t send_ack_info;
    
	U32		usrData[6]; 	/* user defined data				*/

    void	*pDataBuf;

	time_t	start_timestamp;

}	ipos_msg_buf;


int	iposMsgBufInit(int	numMsgBuf);

ipos_msg_buf	*iposMsgGet();

int iposMsgFree(ipos_msg_buf *msgBuf);

int iposMsgGetLen(ipos_msg_buf	*msgBuf);

int	iposMsgPutData(ipos_msg_buf *msgBuf, int len,  	void *pSource);

int	iposMsgGetData(ipos_msg_buf *msgBuf, int *pLen, 	void *pSource);

int	iposMsgGetIndex(ipos_msg_buf *pBuf);

ipos_msg_buf  *iposMsgFindMsg(int index);


#endif






















