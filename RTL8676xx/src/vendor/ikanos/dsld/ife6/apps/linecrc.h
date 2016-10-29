/**************************************************************
***************************************************************
**
** Host Application, Copyright (C) 2003-2005
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

#ifndef _LINECRC_H_
#define _LINECRC_H_

#include <api/apitype.h>
#include <api/apistruct.h>

extern void setLineCrcThreshold (ipos_port_id pid, U32 dataRateF, U32 dataRateS);
extern void reCalLineCrcThreshold (ipos_port_id pid);
extern int evalLineCrc (ipos_port_id pid, U16 currentCntF, U16 currentCntS);
extern void dumpLineCrcStatus (ipos_port_id pid);
extern void iniPrevLineCrc (ipos_port_id pid, U16 currentCntF, U16 currentCntS);
extern int evalSixPtLineHold(U8 lineHold6ptOk);
#endif