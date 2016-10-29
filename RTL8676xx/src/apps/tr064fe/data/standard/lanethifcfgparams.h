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
//  Filename:       lanethifcfgparams.h
//
******************************************************************************/
#ifndef _LANETHIFCFGPARAMS_H
#define _LANETHIFCFGPARAMS_H

#define GetInfo                       DefaultAction
#define GetStatistics                 DefaultAction


#define VAR_Enable                          0
#define VAR_Status                          1
#define VAR_MACAddress                      2
#define VAR_MACAddressControlEnabled        3
#define VAR_MaxBitRate                      4
#define VAR_DuplexMode                      5
#define VAR_BytesSent                       6
#define VAR_BytesReceived                   7
#define VAR_PacketsSent                     8
#define VAR_PacketsReceived                 9

int LANEthIFCfg_GetVar(struct Service *psvc, int varindex);
int SetEnableLANETH(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetMaxBitRateLANETH(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif /* LANETHIFCFGPARAMS_H */
