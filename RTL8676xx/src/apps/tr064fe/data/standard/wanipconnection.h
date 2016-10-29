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
//  Filename:       wanipconnection.h
//
******************************************************************************/
#ifndef _WANIPCONNECTION_H_
#define _WANIPCONNECTION_H_


#define VAR_Enable                       0
#define VAR_ConnectionStatus              3
#define VAR_PossibleConnectionTypes       2
#define VAR_ConnectionType                1
#define VAR_Name                          4
#define VAR_Uptime                        5
#define VAR_NATEnabled                    6
#define VAR_AddressingType                7
#define VAR_ExternalIPAddress             8
#define VAR_SubnetMask                    9
#define VAR_DefaultGateway               10
#define VAR_DNSEnabled                   11
#define VAR_DNSServers                   12
#define VAR_ConnectionTrigger            13

#define VAR_PortMappingNumberOfEntries     14
#define VAR_PortMappingEnabled             15
#define VAR_ExternalPort                   16
#define VAR_InternalPort                   17
#define VAR_PortMappingProtocol            18
#define VAR_InternalClient                 19
#define VAR_PortMappingDescription         20

#define VAR_BytesSent                    21
#define VAR_BytesReceived                22
#define VAR_PacketsSent                  23
#define VAR_PacketsReceived              24

#define GetInfo                     DefaultAction
#define GetStatisticsWANIP          DefaultAction
#define GetConnectionTypeInfo       DefaultAction
#define GetStatusInfo               DefaultAction
#define GetExternalIPAddress        DefaultAction

int WANIPConnection_GetVar(struct Service *psvc, int varindex);
int SetWANIPConnEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetIPConnectionType(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetIPInterfaceInfo(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetIPConnectionTrigger(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int ForceTerminationTR64(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int RequestTerminationTR64(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int RequestConnectionTR64(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

int GetGenericPortMappingEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetSpecificPortMappingEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int AddPortMappingEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetPortMappingNumberOfEntries(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int DeletePortMappingEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif /* _WANIPCONNECTION_H */
