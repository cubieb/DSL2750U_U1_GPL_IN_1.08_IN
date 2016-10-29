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

#ifndef _PASSWORD_H_
#define _PASSWORD_H_

#define INVALID_MODE    0
#define SUPERVISOR_MODE 1
#define OPERATOR_MODE   2

extern void changePassword(void);
extern int verifyPassword(void);

#endif
