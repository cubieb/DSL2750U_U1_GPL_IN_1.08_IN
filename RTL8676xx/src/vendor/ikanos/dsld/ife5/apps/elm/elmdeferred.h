#ifndef		ELMDFRDH
#define		ELMDFRH

/**************************************************************
$Workfile:   elmdeferred.h  $
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
$Date: 2008/07/18 11:08:49 $ 
***************************************************************/

/*  Interface to routines to set up deferred processing
 *
 *  Each routine defined here is called from a callback rountine,
 *  in the context of the BME handler task.  Each is responsible
 *  for setting up processing that must be deferred; that is,
 *  processing that cannot be completed in the context of the BME
 *  handler.
 *
 *  In the Reference Host Application, deferred processing is
 *  accomplished by sending an OAMP task a message whose contents
 *  represent the deferred processing.  The message includes the
 *  address of the routine that completes the deferred processing.
 *  These routines are declared static, to limit their visibility
 *  to the file where they are instantiated.
 *
 */

#include "appconfig.h"
#include <rfm/ipostimer.h>
#include "eocapp.h"
#include "elmdef.h"

extern int submitJobCpeAckHandler( ipos_port_id pid, int status );

extern int submitJobNewBmeEventLog( int bmeNum, U8 uid );

#endif	/*ELMDFRH*/
