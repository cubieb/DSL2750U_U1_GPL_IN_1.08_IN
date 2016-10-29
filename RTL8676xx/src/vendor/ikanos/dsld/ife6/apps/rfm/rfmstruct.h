#ifndef	RFMSTRUCTH
#define	RFMSTRUCTH
/**************************************************************
$Workfile:   rfmstruct.h  $
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

	This header file defines the common structures used for the remote
	firmware management module
	
**************************************************************/	
	
typedef	struct
{

	U16		verId;			/* version ID	*/
	U8		lenStr;			/* version string lenth	*/
 	U8		verStr[MAX_VER_STRING];

}	iposrfm_firmware_ver;



/* CPE firmware version information 	*/

typedef	struct
{
	U8	numFirmware;				/* number of firmware versions in the CPE	*/
	U8	activeVer;		  			/* the indentifier for the active version	*/
	iposrfm_firmware_ver version[MAX_CPE_VERS]; /* all versions in CPE	*/

} 	iposrfm_ver_info;


/* activation information	*/

typedef	struct
{
    U8	activationOption;				/* activation: immediate or delayed	*/
    iposrfm_firmware_ver	version;	/* the version to be activated	*/
}	iposrfm_activation_info;



/* file transfer request information */

typedef struct
{
    
	iposrfm_firmware_ver	verTransfer;	/* the version to be transfered		*/

	U32	fileLength;							/* the length of the file to be transfered 
												in bytes 	*/		
	U32	checkSum;							/* the checsum of the file	*/

}iposrfm_file_info;



/* file transfer complete information */

typedef	struct
{
	U32		dataBlockSent;			  	/* number of dataBlock sent	*/
	U32		byteSent;				  	/* number of bytes sent	*/
}	iposrfm_transfer_info;


/* file save information 	*/

typedef	struct
{
	U8	fileSaveStatus;						/* file save status: failed or success	*/
	iposrfm_firmware_ver	firmwareVerSaved;	/* The firmware version saved	*/
	iposrfm_firmware_ver	replacedFirmwareVer;/* the firmware version replaced	*/

}iposrfm_file_save_info;


/************************************************************** 

			message structures
			
			The message strutures defined here are the element
			ddefinitions for each message.
			
			Coding(parsing) or decoding procedures are necessary to convert 
			between the msg structures and msg buffers	
			
***************************************************************/

typedef	struct
{
	
	U8	pt;					/* protocol type. 0x20 for the header	*/
	U8	msgType;			/* msgtype for the rfm					*/
	U8	length;				/* length of the message: 3-250			*/

} iposrfm_msg_hdr;


typedef	struct
{
	iposrfm_msg_hdr	genericHdr;	/* generic hdr		*/
    
	U8	uid;					/* user identifier			*/
	U8	ack;					/* 0: initiating, 1: positive ack; >2: error cdoe	*/

}	iposrfm_ctrl_hdr;


/* structure for the file transfer	*/

typedef	struct
{
	iposrfm_msg_hdr	genericHdr;
	U8	sid;		 			/* session id	*/
	U16	seq;					/* sequence number	*/
	U8	*pData;			   		 /* data block	*/
} 	iposrfm_data_block;


typedef	struct
{
    iposrfm_msg_hdr	genericHdr;
	U8	sid;			 		/* session id	*/ 
	U16	ack1;				  	/* ack for the starting data_block	*/
	U16	ack2;			   		/* ack for the last data_block		*/
}	iposrfm_data_ack;



/************************************************************** 

	structures for the control messages	
	
***********************************************************/


/* remote firmware version request and response	*/
typedef	struct
{
	iposrfm_ctrl_hdr ctrlHdr;

}	iposrfm_firmware_ver_req;



typedef	struct
{
	iposrfm_ctrl_hdr	ctrlHdr;
	iposrfm_ver_info	verInfo;		/* message field for the version information */											   
} iposrfm_firmware_ver_resp;
	  

/* remote file activation	*/

typedef	struct
{
	iposrfm_ctrl_hdr ctrlHdr;
	iposrfm_activation_info	activationInfo;	  /* information for the activation msg */

}	iposrfm_activation_req;


typedef	struct
{
	iposrfm_ctrl_hdr ctrlHdr;
	U8	activationStatus;				/* activation status: accept or reject	*/
}	iposrfm_activation_resp;


/*  file transfer request	*/
typedef	struct
{
	iposrfm_ctrl_hdr ctrlHdr;
    iposrfm_file_info newFileInfo;			/* file information	in the msg	*/
	iposrfm_firmware_ver	firmwareReplaced;	/* firmware to be replaced proposal */

}	iposrfm_file_transfer_req;


typedef	struct
{
	
	iposrfm_ctrl_hdr 		ctrlHdr;
    iposrfm_firmware_ver	replacedFirmwareVer;	/* the firmware version replaced	*/

}iposrfm_file_transfer_resp;



/* file transfer complete	*/
typedef struct
{
	iposrfm_ctrl_hdr	ctrlHdr;
	iposrfm_transfer_info transferInfo;		/* file transfer completion info from CO  */

}	iposrfm_transfer_complete_req;



typedef	struct
{
	
	iposrfm_ctrl_hdr	ctrlHdr;
	iposrfm_transfer_info transferInfo; 		/* file transfer completion info from CPE */
	iposrfm_file_info	rcvedFile;	/* the file just received: Checksum	*/

}	iposrfm_transfer_complete_resp;


/*  file save successful	*/

typedef	struct
{
	iposrfm_ctrl_hdr	ctrlHdr;
	iposrfm_file_save_info	fileSave;  		/* the file save infomation from CPE	*/
    
}	iposrfm_file_save_req;


typedef	struct
{                          
	iposrfm_ctrl_hdr	ctrlHdr;
}	iposrfm_file_save_resp;



/* 	file transfer abort	*/
typedef	struct
{
	iposrfm_ctrl_hdr	ctrlHdr;
	U8	abortCause;	 				/* the cause for hte about	*/
}	iposrfm_transfer_abort_req;

typedef	struct
{
	iposrfm_ctrl_hdr	ctrlHdr;
}	iposrfm_transfer_abort_resp;


#endif				                                                                
