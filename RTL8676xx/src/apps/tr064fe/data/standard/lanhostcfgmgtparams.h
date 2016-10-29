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
//  Filename:       lanparams.h
//
******************************************************************************/
#ifndef _LANPARAMS_H
#define _LANPARAMS_H

#define VAR_DefaultConnectionService	0
#define GetDefaultConnectionService		DefaultAction

#define GetInfo 			DefaultAction
#define GetDHCPServerConfigurable	DefaultAction
#define GetDHCPRelay			DefaultAction
#define GetSubnetMask			DefaultAction
#define GetIPRoutersList		DefaultAction
#define GetDomainName			DefaultAction
#define GetReservedAddresses		DefaultAction
#define GetDNSServers			DefaultAction

#define VAR_DHCPServerConfigurable          0
#define VAR_DHCPServerEnable                1
#define VAR_DHCPRelay                       2
#define VAR_MinAddress                      3
#define VAR_MaxAddress                      4
#define VAR_ReservedAddresses               5
#define VAR_SubnetMask                      6
#define VAR_DNSServers                      7
#define VAR_DomainName                      8
#define VAR_IPRouters                       9
#define VAR_DHCPLeaseTime                   10
#define VAR_IPInterfaceNumberOfEntries      11
#define VAR_Enable                          12
#define VAR_IPInterfaceIPAddress            13
#define VAR_IPInterfaceSubnetMask           14
#define VAR_IPInterfaceAddressingType       15

int LANHostConfigManagement_Init(PService, service_state_t);
int LANHostConfigManagement_GetVar(struct Service *psvc, int varindex);
void LANHostConfigManagement_UpdateStats(timer_t t, PService psvc);

int SetDHCPLeaseTime(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetDHCPServerConfigurable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetDHCPServerEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetIPRouter(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetSubnetMask(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetDomainName(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetAddressRange(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetDNSServer(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int DeleteDNSServer(UFILE *uclient, PService psvc, PAction ac,pvar_entry_t args, int nargs);
int GetAddressRange(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetIPInterfaceNumberOfEntries(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetIPInterfaceGenericEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetIPInterface(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
#endif /* LANPARAMS_H */
