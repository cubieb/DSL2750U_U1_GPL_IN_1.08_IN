/**************************************************************
***************************************************************
**
** Host Application, Copyright (C) 2002-2004
** Ikanos Communication, Fremont, CA 
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of 
** Ikanos Communication
**
***************************************************************

$Revision: 1.1 $
$Date: 2008/07/18 11:08:36 $
**************************************************************/

#ifndef _NOTCHBND_H_
#define _NOTCHBND_H_

extern int 	notchBandConfig (ipos_port_profile * portProfile);
extern void notchBandConvert (ipos_port_profile * portProfile);
extern void configure_band(int profileNum, int is_cpe);
#endif

