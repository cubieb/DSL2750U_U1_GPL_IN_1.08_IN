/**************************************************************
***************************************************************
**
** C-SIMULATION, Copyright (C) 2002-2004
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
$Date: 2008/07/18 11:09:33 $
**************************************************************/

#ifndef _CONSOLEUTIL_H_
#define _CONSOLEUTIL_H_

extern void consoleEchoDisable(void);
extern void consoleEchoEnable(void);
extern int consoleKbHit (unsigned int timeout /* timeout time in seconds */);

#endif

