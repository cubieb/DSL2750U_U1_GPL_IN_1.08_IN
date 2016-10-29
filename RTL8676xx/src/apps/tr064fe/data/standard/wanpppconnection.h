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
//  Filename:       wanpppconnection.h
//
******************************************************************************/
#ifndef _WANPPPCONNECTION_H_
#define _WANPPPCONNECTION_H_

#define VAR_Enable                         0
#define VAR_ConnectionType                 1
#define VAR_PossibleConnectionTypes        2
#define VAR_ConnectionStatus               3
#define VAR_Name                           4
#define VAR_IdleDisconnectTime             5
#define VAR_NATEnabled                     6
#define VAR_Username                       7
#define VAR_Password                       8
#define VAR_ExternalIPAddress              9
#define VAR_RemoteIPAddress                10
#define VAR_DNSServers                     11
#define VAR_PPPoEServiceName               12
#define VAR_ConnectionTrigger              13

#define VAR_PortMappingNumberOfEntries     14
#define VAR_PortMappingEnabled             15
#define VAR_ExternalPort                   16
#define VAR_InternalPort                   17
#define VAR_PortMappingProtocol            18
#define VAR_InternalClient                 19
#define VAR_PortMappingDescription         20

#define VAR_BytesSent                      21
#define VAR_BytesReceived                  22
#define VAR_PacketsSent                    23
#define VAR_PacketsReceived                24

#define GetInfo                     DefaultAction
#define GetConnectionTypeInfo       DefaultAction
#define GetStatusInfo               DefaultAction
#define GetUsername                 DefaultAction
#define GetStatisticsWANPPP         DefaultAction
#define GetIdleDisconnectTime       DefaultAction

int SetConnectionType(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetWANPPPConnEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetUsername(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetPassword(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetPPPoEService(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetConnectionTrigger(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int ForceTermination(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int RequestTermination(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int RequestConnection(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int WANPPPConnection_GetVar(struct Service *psvc, int varindex);
int SetIdleDisconnectTime(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int ppp_GetGenericPortMappingEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int ppp_GetSpecificPortMappingEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int ppp_AddPortMappingEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int ppp_GetPortMappingNumberOfEntries(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int ppp_DeletePortMappingEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif /* _WANPPPCONNECTION_H */

