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
$Date: 2008/07/18 11:08:38 $
**************************************************************/

#ifndef _UI_UTIL_H_
#define _UI_UTIL_H_
#include <api/apitype.h>

extern int getBandPlan (int bandPlan);
extern int getBmeBandPlan (int bandPlanSelect);
extern unsigned char getPsdMaskLevel(unsigned char psdMaskLevel);
extern void	getCO_PSD_const_array();
extern void getMaxPsd (void);
extern void copyMaxPsdArray (U8 *maxPsdArray);

#endif