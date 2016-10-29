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
//  Filename:       ippingparams.h
//
******************************************************************************/
#ifndef _IPPINGPARAMS_H
#define _IPPINGPARAMS_H

#define VAR_DiagnosticsState                            0
#define VAR_Interface                                   1
#define VAR_Host                                        2
#define VAR_NumberOfRepetitions                         3
#define VAR_Timeout                                     4
#define VAR_DataBlockSize                               5
#define VAR_DSCP                                        6
#define VAR_SuccessCount                                7
#define VAR_FailureCount                                8
#define VAR_AverageResponseTime                         9
#define VAR_MinimumResponseTime                         10
#define VAR_MaximumResponseTime                         11

#define GetIPPingInfo DefaultAction
int IPPing_GetVar(struct Service *psvc, int varindex);

int SetDiagnosticsStateIPPing(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetInterfaceIPPing(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetHostIPPing(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int NumberOfRepetitionsIPPing(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetTimeoutIPPing(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetDataBlockSizeIPPing(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetDSCPIPPing(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif /* LANWLANCFGPARAMS_H */

