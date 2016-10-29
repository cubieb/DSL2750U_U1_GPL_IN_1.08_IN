#ifndef	RFMIFCOH
#define	RFMIFCOH

/**************************************************************
$Workfile:   rfmifco.h  $
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
$Date: 2008/07/18 11:08:52 $
***************************************************************/

#include "rfmdef.h"
#include "rfmstruct.h"

/************************************************************************** 

	this file defines the CO_PE interface related structure, definitions, 
		and prototype

**************************************************************************/


/************************************************************************
	CO_PE API prototypes
************************************************************************/

/* initialize the CO_PE	*/
					
int	iposRFMCOInit(void);
	
/************************************************************************
	command primitives 
**************************************************************************/

/* Get remote firmware version 	*/

typedef	void ( * iposFuncRMVersion) ( ipos_port_id pid, U8 uid, int status, iposrfm_firmware_ver_resp *pResponse);

int	iposRFMCOGetRemoteVersion(ipos_port_id pid, U8	uid, iposFuncRMVersion pComplete);	



/* activate the remote firmware version */
typedef	void (* iposFuncActivation) (ipos_port_id pid, U8 uid, int status, iposrfm_activation_resp *ptr);

int iposRFMCOActivateVersion(ipos_port_id pid, U8 uid, iposrfm_activation_info *pActInfo, 
   								iposFuncActivation pComplete);


/* file transfer request	*/
typedef	void (* iposFuncTransfer)(ipos_port_id pid, U8 uid, int status, iposrfm_file_transfer_resp *pResponse);

int	iposRFMCOFileTransfer ( ipos_port_id pid, U8 uid, 
							
							iposrfm_file_info *pFileInfo, U8	*addr,	/* fileinfo and addr to be transfered */

							iposrfm_firmware_ver	*pVerBeReplaced, /* the proposed version to be replaced	*/
							
							iposFuncTransfer pComplete 				/* complete function */
							);



/* file transfer abort	*/

typedef	void (* iposFuncCOAbort) ( ipos_port_id pid, U8 uid, int status, iposrfm_transfer_abort_resp *ptr);
int	iposRFMCOTransferAbort( ipos_port_id pid, U8 uid, iposFuncCOAbort pComplete);


/* used internally by iFFTP    	*/
typedef	void (* iposFuncTrComplete)( ipos_port_id pid, U8 uid, int status, iposrfm_transfer_complete_resp *pPar);
int	iposRFMCOiFftpComplete(ipos_port_id pid, U8 uid, iposrfm_transfer_info  *pInfo, iposFuncTrComplete pComplete);





/************************************************************************** 
					 the event primitives	
***************************************************************************/


/* file save event handler	*/
typedef	void (* iposRFMCOEvntFileSave)(ipos_port_id pid, U8 uid, iposrfm_file_save_req *pFileSaveInfo);

int	iposRFMCORegFileSave(ipos_port_id pid, iposRFMCOEvntFileSave pHandler);

/* respond to file save */
int	iposRFMCOFileSaveResp(ipos_port_id pid, U8 uid, U8 status);



/* file transfer complete	*/
typedef	void (* iposRFMCOEvntTransferComplete)(ipos_port_id pid, U8 uid, iposrfm_transfer_complete_resp *pInfo);

int iposRFMCORegTransferComplete( ipos_port_id pid, U8 uid, iposRFMCOEvntTransferComplete pHandler);

/* respond to the complete req	*/
int	iposRFMCOTransferCompleteResp(ipos_port_id pid, U8 uid);



/* file transfer error		*/
typedef	void (* iposRFMCOEvntError)(ipos_port_id pid, U8 uid, int	errCode);
int	iposRFMCORegError(ipos_port_id pid, U8 uid, iposRFMCOEvntError pErrorHandler);





#endif
