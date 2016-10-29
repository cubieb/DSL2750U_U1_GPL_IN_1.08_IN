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
//  Filename:       wandslconnmgtparams.h
//
******************************************************************************/
#ifndef _WANDSLCONNMGTPARAMS_H_ 
#define _WANDSLCONNMGTPARAMS_H_ 

#define VAR_WANConnectionDevice                     0
#define VAR_WANConnectionService                    1
#define VAR_DestinationAddress                      2
#define VAR_LinkType                                3
#define VAR_ConnectionType                          4
#define VAR_Name                                    5
#define VAR_WANConnectionServiceNumberOfEntries     6

int GetWANConnectionServiceNumberOfEntries(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetGenericConnectionServiceEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetSpecificConnectionServiceEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int AddConnectionDeviceAndService(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int AddConnectionService(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int DeleteConnectionService(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
#endif 

