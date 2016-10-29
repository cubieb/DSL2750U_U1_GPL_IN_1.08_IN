#ifndef		RFMDEFH
#define		RFMDEFH

/**************************************************************
$Workfile:   rfmdef.h  $
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

/**************************************************************

	global definitions
	
**************************************************************/


#define		MAX_VER_STRING	32			/* 32 char in the version string */

#define		MAX_CPE_VERS	4			/* the maximum versions stored in the CPE */ 
					 
#define		MAX_NUM_DATA_BLOCKS	10000	/* maximum number of the data blocks: assuming filesize
											2M	*/	
#define		MAX_MSG_SIZE	250			/* The maximum msg size: limited by the HIC interface */			 
#define		MAX_STRUCT_SIZE	300			/* rhw maximem msg structure size after parsing	*/			    	 


/**************************************************
	Maximum File Block Re-tx times
	
***************************************************/

#define	IPOSRFM_DATABLK_RESEND	6		/* exceed 6 times? error declared	*/	
					   

/**************************************************
	Time to declare file transfer expired
	
***************************************************/
                                                 
#define	IPOSRFM_FILE_EXPIRATION_TICK	160	/* ca. 30 seconds	*/ 					   
					   
													  
/**************************************************************
	command/response events
**************************************************************/

						   
#define		RFM_EVNT_CMD_SENT		1						   
#define		RFM_EVNT_RESP_RCVD		2
#define		RFM_EVNT_RESP_EXPIRED	3
			    	 



/**************************************************************
	command/response states
**************************************************************/
#define		IPOSRFM_STATE_CMD_IDLE			1
#define		IPOSRFM_STATE_CMD_INPROGRESS	2

				  
/**************************************************************
	iFftp states
**************************************************************/
#define		IPOSRFM_IFFTP_CO_ACTIVE			1
#define		IPOSRFM_IFFTP_CO_IDLE			2
				  
#define		IPOSRFM_IFFTP_CPE_ACTIVE   		1
#define		IPOSRFM_IFFTP_CPE_IDLE			2
				  
				  
				    			   
/**************************************************************
	Message definition
**************************************************************/

#define	IPOS_DATA_MSG_LEN	250			/* msg Len for Data	*/
#define	IPOS_DATA_BLK_SIZE	242			/* file block size: 244+6 byte hdr=250 byte */
								 
/* Definition of the protocol type for RFM		*/
#define	IPOSRFM_PT			0x20

/* Definition of the message class	*/
#define	IPOSRFM_MSG_COMMAND				0x00
#define	IPOSRFM_MSG_RESPONSE			0x01
#define	IPOSRFM_MSG_FILE				0x02						   
						   
/* message type definitions	*/
#define	IPOSRFM_MSG_CMDRESP_START			0x01

#define	IPOSRFM_MSG_FIRMWARE_VER		0x01
#define	IPOSRFM_MSG_FIRMWARE_ACTIVATE	0x02
#define	IPOSRFM_MSG_FILE_TRANSFER		0x03
#define	IPOSRFM_MSG_FILE_TR_COMPLETE	0x04
#define	IPOSRFM_MSG_FILE_TR_ABORT		0x05
#define	IPOSRFM_MSG_FILE_SAVE			0x06	

#define	IPOSRFM_MSG_CMDRESP_END			0x06
    
#define	IPOSRFM_MSG_DATA_BLOCK			0x80
#define	IPOSRFM_MSG_ACK					0x81

/* definitions for ACK/NACK	*/
#define	IPOSRFM_ACK_CMD					0x00
#define	IPOSRFM_ACK_ACK					0x01
#define	IPOSRFM_ACK_NACK				0x02
											
											
											
/* definition of the msgs header byte position	*/

#define	IPOSRFM_MSGHDR_PT				0
#define	IPOSRFM_MSGHDR_MSGTYPE			1
#define	IPOSRFM_MSGHDR_MSGLEN			2
#define	IPOSRFM_MSGHDR_UID				3
#define	IPOSRFM_MSGHDR_ACK				4

/* definition for iposrfm_data_block	*/
#define	IPOSRFM_FILE_SID				3
#define	IPOSRFM_FILE_SEQ				4
#define	IPOSRFM_FILE_DATA				6


#define	IPOSRFM_FILE_ACK1				4
#define	IPOSRFM_FILE_ACK2				6
#endif
