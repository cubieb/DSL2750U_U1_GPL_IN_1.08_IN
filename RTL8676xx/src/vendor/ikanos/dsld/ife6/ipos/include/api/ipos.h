
#ifndef	IPOSH
#define	IPOSH

/**************************************************************
$Workfile:   ipos.h  $
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

#define	IPOS_USE_RTOS			/* the IPOS API function uses RTOS function	*/

#include "bmeconfig.h"

#include "apitype.h"
#include "apidef.h"
#include "apistruct.h"
#include "apiproto.h"

#include "msgbuf.h"

#ifdef	IPOS_USE_RTOS
#include <osshim/apiosmacro.h>		/* Use the macros for vxWorks OS	*/
#endif

typedef	struct
{
	ipos_port_id	portId;
	U8				uid;
	int				status;
	int				cntRpt;			/* repeat counter	*/
	int				parLen;
    char			par[IPOS_MSG_LEN];		/* parameters	*/
}	oamp_cb_msg;

#endif
	 
	 
	 
	 
	 

