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
//  Filename:       mgtserverparams.h
//
******************************************************************************/
#ifndef _MGTSERVERPARAMS_H
#define _MGTSERVERPARAMS_H

#define GetInfo 			DefaultAction

 #define VAR_URL                                  0
 #define VAR_Password                             1
 #define VAR_PeriodicInformEnable                 2
 #define VAR_PeriodicInformInterval               3
 #define VAR_PeriodicInformTime                   4
 #define VAR_ParameterKey                         5
 #define VAR_ParameterHash                        6
 #define VAR_ConnectionRequestURL                 7
 #define VAR_ConnectionRequestUsername            8
 #define VAR_ConnectionRequestPassword            9
 #define VAR_UpgradesManaged                      10
 #define VAR_KickURL                              11
 #define VAR_DownloadProgressURL                  12 

int MgtServer_GetVar(struct Service *psvc, int varindex);
int SetManagementServerURL(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetManagementServerPassword(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetUpgradesManagement(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetPeriodicInform(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetConnectionRequestAuthentication(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
#endif /* MGTSERVERPARMS_H */

