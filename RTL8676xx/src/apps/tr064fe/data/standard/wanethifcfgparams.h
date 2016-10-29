/*****************************************************************************
//
//  Copyright (c) 2005  Broadcom Corporation
//  All Rights Reserved
//  No portions of this material may be reproduced in any form without the
//  written permission of:
//          Broadcom Corporation
//          16215 Alton Parkway
//          Irvine, California 92619
//  All information contained in this document is Broadcom Corporation
//  company private, proprietary, and trade secret.
//
******************************************************************************
//
//  Filename:       wanethifcfgparams.h
//
******************************************************************************/
#ifdef INCLUDE_WANETHERNETCONFIG
#ifndef _WANETHIFCFGPARAMS_H_ 
#define _WANETHIFCFGPARAMS_H_ 

#define GetInfo DefaultAction

#define VAR_Enable                 0
#define VAR_Status                 1
#define VAR_MACAddress             2
#define VAR_MaxBitRate             3
#define VAR_BytesSent              4
#define VAR_BytesReceived          5
#define VAR_PacketsSent            6
#define VAR_PacketsReceived        7

int WANETHInterfaceConfig_GetVar(struct Service *psvc, int varindex);
int SetETHInterfaceEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetMaxBitRate(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetStatisticsWANETH(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif /* _WANETHIFCCONFIG_H_  */
#endif

