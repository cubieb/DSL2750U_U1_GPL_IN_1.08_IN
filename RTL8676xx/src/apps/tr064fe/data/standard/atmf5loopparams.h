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
//  Filename:       atmf5loopparams.h
//
******************************************************************************/
#ifndef _ATMF5LOOPPARAMS_H
#define _ATMF5LOOPPARAMS_H

#define GetInfo                        DefaultAction

#define VAR_DiagnosticsState                0
#define VAR_NumberOfRepetitions             1
#define VAR_Timeout                         2
#define VAR_SuccessCount                    3
#define VAR_FailureCount                    4
#define VAR_AverageResponseTime             5
#define VAR_MinimumResponseTime             6
#define VAR_MaximumResponseTime             7

int WANATMF5LoopbackDiagnostics_GetVar(struct Service *psvc, int varindex);
int SetDiagnosticsStateATMF5(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetNumberOfRepetitionsATMF5(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetTimeoutATMF5(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif
