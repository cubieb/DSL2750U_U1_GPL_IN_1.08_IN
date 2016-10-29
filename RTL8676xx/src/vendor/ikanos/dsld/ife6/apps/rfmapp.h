#ifndef		RFMAPPH
#define		RFMAPPH

/**************************************************************
$Workfile:   rfmapp.h  $
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


#include	"rfmifco.h"
#include	"rfmifcpe.h"
#include	"rfmlocal.h"						
						
//#ifdef	VTU_R		

typedef	struct
{

	ipos_port_id	pid;
	/* RFM object			*/
	ipos_rfm_cpe	*pCpePe;



	iposrfm_ver_info	verInfo; 				/* current firmware info	*/
	
	/* current boot info		*/
	int		currImageIndx;		  				/* current image index	*/  
	int		currImageFlag;						/* current image flag	*/

	/* BME connection info		*/
	int		currConnectionState;
	int		cntConnectionAttempt;
	ipos_tmr_info	*pTmrCnt;					/* connection time out timer	*/
	ipos_tmr_info	*pTmrReboot;				/* reboot timer	*/	
	
	/* file info in progress	*/
	iposrfm_file_info	fileInfo;
	U8		*fileAddr;							/* file address	*/

	/* file activation info	*/
	iposrfm_activation_info	activationInfo;

	/* display option		*/
	int	displayLevel;							/* 0: minimum display, 1: maximum display	*/

}	app_cpe_rfm;

//#endif


//#ifdef	VTU_O

typedef	struct
{

	ipos_port_id	pid;

	int				fsState;	/* remote download state	*/

/* RFM object			*/
	ipos_rfm_co		*pCoPe;

/*  RM: firmware info	*/
	iposrfm_ver_info	rmVerInfo;

/* 	local file info		*/
	
	U8		*fileAddr;	
	iposrfm_file_info	localFileInfo;

/* 	remote file info	*/
	iposrfm_file_info	rmFileInfo;


/* activation info 		*/
	iposrfm_activation_info	actInfo;
	U8	activationStatus;

	int	displayLevel;

}	app_co_rfm;


extern U8 feDldStatus[MAX_NUM_OF_PORTS];
//#endif

void	uiSetDisplay(int newLevel);

//#ifdef	VTU_R
int	cpeRFMAppInit(void);
int	cpeRFMBootDone(void);
//#endif


//#ifdef	VTU_O
int		coRFMAppInit(void);
int		coRFMAppFileTransfer(ipos_port_id pid, iposrfm_file_info	*pFileInfo, U8 *fileAddr);
int		coRFMAppActivate(ipos_port_id	pid, U8	uid, iposrfm_activation_info *pActivationInfo);
void	uiActivateImage(ipos_port_id	pid, U8	actOption);
void	uiGetRMVersion(ipos_port_id	pid);
//#endif




#endif
