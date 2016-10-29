/**************************************************************
***************************************************************
**
** Host Application, Copyright (C) 2000-2002
** Ikanos Communication, Fremont, CA 
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of 
** Ikanos Communication
**
***************************************************************

$Revision: 1.7.2.3 $
$Date: 2008/11/27 12:47:37 $
**************************************************************/

#ifndef _HOST_VER_H_
#define _HOST_VER_H_

#include "apposmacro.h"

#define VERSION "2.2.0r7a04"

#if defined(VX180) || defined(USE_VX180)
#define DEV_TAG "IKF6850"
#else
#define DEV_TAG "IK105012"
#endif

#define VERSION_STR (VERSION DEV_TAG)

#endif

