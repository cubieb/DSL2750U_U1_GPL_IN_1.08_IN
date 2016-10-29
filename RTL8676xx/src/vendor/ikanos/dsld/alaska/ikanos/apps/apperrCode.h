#ifndef	APPERRH
#define	APPERRH

/**************************************************************
$Workfile:   apperrCode.h  $
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
$Date: 2008/07/18 11:08:39 $
***************************************************************/



/* OAMP error code	*/

#define	OAMP_ERROR_BASE		0x8000
#define	OAMP_ERROR_BME_NOT_READY		(OAMP_ERROR_BASE+1)
#define	OAMP_ERROR_PORT_NOT_READY		(OAMP_ERROR_BASE+2)
#define OAMP_ERROR_UNKNOWN_PORTSTATE	(OAMP_ERROR_BASE+3)
#define OAMP_ERROR_UNKNOWN_BMESTATE		(OAMP_ERROR_BASE+4)
#define OAMP_ERROR_UNKNOWN_LINE_NUM		(OAMP_ERROR_BASE+5)

#define OAMP_ERROR_BME_RUN            (OAMP_ERROR_BASE+6)
#define OAMP_PORT_ERROR_RUN           (OAMP_ERROR_BASE+7)
/* app error code */
#define	APP_ERROR_BASE		0x9000
#define	APP_ERROR_BME_NUM	(APP_ERROR_BASE+1)		/* wrong bme number	*/
#define APP_ERROR_LINE_NUM  (APP_ERROR_BASE+2)		/* wrong line number */
#define	APP_ERROR_PORT_ID	(APP_ERROR_BASE+3)		/* wrong port id	*/					 
#define	PM_ERROR_NOT_VALID	(APP_ERROR_BASE+4)

#endif
