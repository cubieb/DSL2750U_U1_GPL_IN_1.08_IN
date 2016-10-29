#ifndef	IPOSAPIDEFH
#define IPOSAPIDEFH

/**************************************************************
$Workfile:   apidef.h  $
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

#include <dapi/dapi_define.h>

#define	IPOS_SUCCESS	0
#define	IPOS_ERROR		1
#define IPOS_RFM_SPACE_ERR 2



/* Notch Band defines	*/
#define STOP_BAN_640K_3BAND   148
#define STOP_BAN_1100K_3BAND  255
#define STOP_BAN_640K_4BAND   74
#define STOP_BAN_1100K_4BAND  128

/* for ipos_bme_provision	*/

/* Bit Wise Option Mask */
#define NTR_DISABLE       (0x01)
#define IMUX_ENABLE       (0x02)
#define SUPPORT_INI       (0x04)

#ifdef	BME_CPE_20

/* IPOS memory configuration defs	*/

#define	IPOS_BME_MEM_DEFAULT	0

#define	IPOS_BME_MEM_SRAM		0
#define	IPOS_BME_MEM_SDRAM16	1
#define	IPOS_BME_MEM_SDRAM32	2

#define	IPOS_BME_MEM_CLKHALF	1	/* half core clock rate	*/
#define	IPOS_BME_MEM_CLKFULL	2	/* full core clock rate	*/

#endif
/**************************************************************
**
**		error code
**
**************************************************************/
				
#define	IPOS_ERROR_BASE				0x1000

#define	IPOS_ERROR_NO_MSGBUF		(IPOS_ERROR_BASE+1)

#define	IPOS_ERROR_SEM_ALLOCATION 	(IPOS_ERROR_BASE+2)				

#define	IPOS_ERROR_BME_START		(IPOS_ERROR_BASE+3)

#define	IPOS_ERROR_EXPIRED			(IPOS_ERROR_BASE+4)

#define	IPOS_ERROR_RECEIVE_FAILED	(IPOS_ERROR_BASE+5)

#define	IPOS_ERROR_MSG_SOURCE		(IPOS_ERROR_BASE+6)

#define	IPOS_ERROR_CALLBACK_NOT_SET	(IPOS_ERROR_BASE+7)	   	/* callback is not set in the msgbuf */ 		

#define	IPOS_ERROR_DAPI_FAILED		(IPOS_ERROR_BASE+8)		/* direct API call failed	*/	   	

#define	IPOS_ERROR_TX_TIMEOUT		(IPOS_ERROR_BASE+9)		/* pollTxMsgRdy time out	*/

#define	IPOS_ERROR_BME_IDLE			(IPOS_ERROR_BASE+10)				   

#define	IPOS_ERROR_NOT_IMPLEMENTED	(IPOS_ERROR_BASE+11)
#endif				    											   
