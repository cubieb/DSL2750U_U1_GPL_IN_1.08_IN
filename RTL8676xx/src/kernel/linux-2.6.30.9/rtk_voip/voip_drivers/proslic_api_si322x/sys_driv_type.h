/*
** $Id: sys_driv_type.h,v 1.1 2011/03/15 08:13:30 yachang Exp $
** 
** This file is system specific and should be edited for your hardware platform
**
** This file is used by proslic_timer_intf.h and proslic_spiGci.h
*/
#ifndef DRIV_TYPE_H
#define DRIV_TYPE_H
#include "proslic_datatypes.h"

/*
** SPI/GCI structure
*/
typedef struct{
	uInt16 portID;
} ctrl_S;

/*
** System timer interface structure 
*/
typedef struct{
	//_int64 ticksPerSecond;
	unsigned long ticksPerSecond;
} systemTimer_S;

/*
** System time stamp
*/
typedef struct{
	//_int64 time;
	unsigned long time;
} timeStamp;

#endif
/*
** $Log: sys_driv_type.h,v $
** Revision 1.1  2011/03/15 08:13:30  yachang
** VOIP driver - 1st release
**
** Revision 1.2  2007/02/15 23:33:25  lajordan
** no message
**
** Revision 1.1.1.1  2006/07/13 20:26:08  lajordan
** no message
**
*/
