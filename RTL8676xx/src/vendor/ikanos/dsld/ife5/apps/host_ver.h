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

$Revision: 1.3 $
$Date: 2008/10/20 14:18:27 $
**************************************************************/

#ifndef _HOST_VER_H_
#define _HOST_VER_H_

#include "apposmacro.h"

#define VERSION "2.1.0r18a14"
#ifdef BER_PER_TONE
#define BER_TAG "_BER"
#else
#define BER_TAG
#endif

#if defined(VX180) || defined(RTEMS_VX180) || defined(FUSIV_VX180)
#define DEV_TAG "IKF6850"
#else
#define DEV_TAG "IK105012"
#endif

#define VERSION_STR (VERSION BER_TAG DEV_TAG)

#endif
