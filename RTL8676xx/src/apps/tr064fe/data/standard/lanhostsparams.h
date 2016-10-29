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
//  Filename:     lanhostsparams.h
//
******************************************************************************/
#ifndef _LANHOSTSPARAMS_H
#define _LANHOSTSPARAMS_H

#define VAR_HostNumberOfEntries             0
#define VAR_IPAddress                       1
#define VAR_AddressSource                   2
#define VAR_LeaseTimeRemaining              3
#define VAR_MACAddress                      4
#define VAR_HostName                        5
#define VAR_InterfaceType                   6
#define VAR_Active                          7

int GetHostNumberOfEntries(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetSpecificHostEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetGenericHostEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif /* LANHOSTSPARAMS_H */
