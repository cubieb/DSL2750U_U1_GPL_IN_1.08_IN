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
//  Filename:       timeparams.h
//
******************************************************************************/
#ifndef _TIMEPARAMS_H
#define _TIMEPARAMS_H

#define GetInfo                   DefaultAction

#define VAR_NTPServer1                  0
#define VAR_NTPServer2                  1
#define VAR_NTPServer3                  2
#define VAR_NTPServer4                  3
#define VAR_NTPServer5                  4

#define VAR_CurrentLocalTime            5
#define VAR_LocalTimeZone               6
#define VAR_LocalTimeZoneName           7
#define VAR_DaylightSavingsUsed         8
#define VAR_DaylightSavingsStart        9
#define VAR_DaylightSavingsEnd          10

int TimeServer_GetVar(struct Service *psvc, int varindex);

int SetNTPServers(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetLocalTimeZone(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif /* TIMEPARAMS_H */
