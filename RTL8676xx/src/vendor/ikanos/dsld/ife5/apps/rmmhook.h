/**************************************************************
$Workfile$
***************************************************************
**
** Host Application, Copyright (C) 2007
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

#ifndef RMMHOOKH
#define RMMHOOKH

#include <api/apitype.h>
#include <api/apistruct.h>

extern void cpeRMMSetEnetFlowCtrlEventHandler (ipos_port_id pid, U8 *pRespBase,
                                               U8 **pPResp, U8 ctrlOption);
extern void cpeRMMSetPhyParamsHandler (ipos_port_id pid, U8 *pRespBase,
                                       U8 **pPResp, U8 *pEocMsg, U8 ieType,
                                       U8 ieLen);
extern void cpeRMMSetAutoEnetStatusRptHandler (ipos_port_id pid, U8 *pRespBase,
                                               U8 **pPResp, U8 reportEn);
extern int cpeRMMGetEnetCfg (int lineNum, U8 *pRespBase, U8 **pPResp,
                             U8 *pEocMsg, U8 ieLen);
extern int cpeRMMGetEthernetInfo (int lineNum, U8 *pRespBase, U8 **pPResp,
                                  U8 *pEocMsg, U8 ieLen);

#endif /* RMMHOOKH */
