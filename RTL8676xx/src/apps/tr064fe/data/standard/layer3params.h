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
//  Filename:       layer3params.h
//
******************************************************************************/

#ifndef _LAYER3PARAMS_H
#define _LAYER3PARAMS_H

#define GetDefaultConnectionService         DefaultAction

#define VAR_DefaultConnectionService        0
#define VAR_ForwardNumberOfEntries          1
#define VAR_Enable                          2
#define VAR_Status                          3
#define VAR_Type                            4
#define VAR_DestIPAddress                   5
#define VAR_DestSubnetMask                  6
#define VAR_SourceIPAddress                 7
#define VAR_SourceSubnetMask                8
#define VAR_GatewayIPAddress                9
#define VAR_Interface                       10
#define VAR_ForwardingMetric                11
#define VAR_MTU                             12


int Layer3Forwarding_Init(PService, service_state_t);
int Layer3_GetVar(struct Service *psvc, int varindex);
int SetDefaultConnectionService(UFILE *, PService psvc, PAction ac, pvar_entry_t args, int nargs);

int AddForwardingEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int DeleteForwardingEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

int GetSpecificForwardingEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetGenericForwardingEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetForwardNumberOfEntries(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetForwardingEntryEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif /* LAYER3PARAMS_H */
