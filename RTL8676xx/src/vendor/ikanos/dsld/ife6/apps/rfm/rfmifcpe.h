#ifndef	RFMIFCPEH
#define	RFMIFCPEH
/**************************************************************
$Workfile:   rfmifcpe.h  $
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

#include "rfmdef.h"
#include "rfmstruct.h"

/************************************************************************** 

	this file defines the CPE_PE interface related structure, definitions, 
		and prototype

**************************************************************************/

/************************************************************************
	CPE_PE API prototypes
************************************************************************/

/* initialize the CPE_PE	*/
					
int	iposRFMCPEInit(void);

/*********************************************************************
	CPE command/response primitives
**********************************************************************/

/* transfer abort	*/

/* file transfer abort	*/
typedef	void (* iposFuncAbort) ( ipos_port_id pid, U8 uid, int status, void *ptr);	
														/* the last pointer should point to NULL */

int	iposRFMCPETransferAbort( ipos_port_id pid, U8 uid, iposFuncAbort pComplete);


/* file save  */
typedef	void (* iposFuncSave)( ipos_port_id pid, U8 uid, int status, void *ptr);
														/* the last pointer should point to NULL */

int	iposRFMCPEFileSave(ipos_port_id pid, U8 uid, iposrfm_file_save_info *pFileSave,
					   iposFuncSave pComplete);


/*******************************************************************
	CPE event primitives
*******************************************************************/

/* file transfer error	*/

typedef	int (* iposRFMCPEEvntError)(ipos_port_id pid, U8 uid, int	errCode);

int	iposRFMCPERegError(ipos_port_id pid, U8 uid, iposRFMCPEEvntError pErrorHandler);


/* file version request, callback reg and respond	*/

typedef	void (* iposRFMCPEEvntVersionReq) (ipos_port_id pid, U8	uid);

int	iposRFMCPERegVersionReq (ipos_port_id pid, iposRFMCPEEvntVersionReq pHandler);

/* respond to file version req  */
int iposRFMCPEVersionResp( ipos_port_id	pid, U8 uid, U8 status, iposrfm_ver_info *pRespInfo);
	


/* file transfer req	*/
typedef	void (* iposRFMCPEEvntTransferReq)(ipos_port_id pid, U8 uid, 
 										   iposrfm_file_info	*pNewFileInfo,		/* new file information	*/
										   iposrfm_firmware_ver	*pReplacedVersion);	/* NULL means no recommendation */

int iposRFMCPERegTransferReq(ipos_port_id pid, iposRFMCPEEvntTransferReq pHandler);

/* respond to the file transfer request */
int	iposRFMCPETransferResp(ipos_port_id pid, U8 uid, U8 status, 
						   iposrfm_firmware_ver *pReplacedVersion,	/* replaced version confirmed by the App */
						   U8  *addr								/* the starting address to save the file */
						   );


/* file activation req	*/
typedef	void (* iposRFMCPEEvntActivationReq)(ipos_port_id pid, U8 uid,
										  iposrfm_activation_info *pActivationInfo);

int	iposRFMCPERegActivationReq(ipos_port_id pid, iposRFMCPEEvntActivationReq pHandler);

/* respond to the activation request	*/
int	iposRFMCPEActivationResp(ipos_port_id pid, U8 uid, U8 status);



/* file complete req	*/
typedef	void (* iposRFMCPEEvntTransferCompleteReq)( ipos_port_id pid, U8 uid,
													iposrfm_transfer_info	*pTransferInfo);

int iposRFMCPERegTransferComplete ( ipos_port_id pid,  iposRFMCPEEvntTransferCompleteReq pHandler);


/* respond to the complete req	*/
int	iposRFMCPETransferCompleteResp(ipos_port_id pid, U8 uid, 
								   iposrfm_file_info	*pRcvedFileInfo);





#endif







