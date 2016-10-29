#ifndef		RFMERRH
#define		RFMERRH

/**************************************************************
$Workfile:   rfmerr.h  $
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

/*************************************************************

	protocol type and msg types 

*************************************************************/	


#define	IPOSRFM_ERROR_BASE	0x5000

#define	IPOSRFM_ERROR						(IPOSRFM_ERROR_BASE)
		
#define	IPOSRFM_ERROR_NOSEM					(IPOSRFM_ERROR_BASE +1)

#define	IPOSRFM_ERROR_MSGCODING				(IPOSRFM_ERROR_BASE +2)

#define IPOSRFM_ERROR_SENDMSG				(IPOSRFM_ERROR_BASE +3)

#define	IPOSRFM_ERROR_TMR					(IPOSRFM_ERROR_BASE +4)

#define	IPOSRFM_ERROR_UNKNOWN_MSG			(IPOSRFM_ERROR_BASE +5)

#define	IPOSRFM_ERROR_MSGDECOING			(IPOSRFM_ERROR_BASE +6)

#define	IPOSRFM_ERROR_TIMEOUT				(IPOSRFM_ERROR_BASE	+7)

#define	IPOSRFM_ERROR_PID					(IPOSRFM_ERROR_BASE	+8)
					
#define	IPOSRFM_ERROR_MAX_RESEND			(IPOSRFM_ERROR_BASE	+9)

#define	IPOSRFM_ERROR_FILE_SIZE				(IPOSRFM_ERROR_BASE+10)

#define	IPOSRFM_ERROR_SEQ					(IPOSRFM_ERROR_BASE+11)

#define	IPOSRFM_ERROR_BOOTLOAD				(IPOSRFM_ERROR_BASE+12)								

#define	IPOSRFM_ERROR_ACTIVE_IMAGE	   		(IPOSRFM_ERROR_BASE+13)	

#define	IPOSRFM_ERROR_IMAGE_TARGET	   		(IPOSRFM_ERROR_BASE+14)

#define	IPOSRFM_ERROR_IMAGE_CRC		   		(IPOSRFM_ERROR_BASE+15)

#define	IPOSRFM_ERROR_IMAGE_NUMBER	   		(IPOSRFM_ERROR_BASE+16)
#endif
