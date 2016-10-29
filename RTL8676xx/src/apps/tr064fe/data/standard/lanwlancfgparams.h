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
//  Filename:       lanwlancfgparams.h
//
******************************************************************************/
#ifdef WIRELESS
#ifndef _LANWLANCFGPARAMS_H
#define _LANWLANCFGPARAMS_H

#define VAR_Enable                                      0
#define VAR_Status                                      1
#define VAR_BSSID                                       2
#define VAR_MaxBitRate                                  3
#define VAR_Channel                                     4
#define VAR_SSID                                        5
#define VAR_BeaconType                                  6
#define VAR_MACAddressControlEnabled                    7
#define VAR_Standard                                    8
#define VAR_WEPKeyIndex                                 9
#define VAR_KeyPassphrase                               10
#define VAR_WEPEncryptionLevel                          11
#define VAR_BasicEncryptionModes                        12
#define VAR_BasicAuthenticationMode                     13
#define VAR_WPAEncryptionModes                          14
#define VAR_WPAAuthenticationMode                       15
#define VAR_IEEE11iEncryptionModes                      16
#define VAR_IEEE11iAuthenticationMode                   17
#define VAR_PossibleChannels                            18
#define VAR_BasicDataTransmitRates                      19
#define VAR_OperationalDataTransmitRates                20
#define VAR_PossibleDataTransmitRates                   21
#define VAR_InsecureOOBAccessEnabled                    22
#define VAR_BeaconAdvertisementEnabled                  23
#define VAR_RadioEnabled                                24
#define VAR_AutoRateFallBackEnabled                     25
#define VAR_LocationDesccription                        26
#define VAR_RegulatoryDomain                            27
#define VAR_TotalIPSKFailures                           28
#define VAR_TotalIntergrityFailures                     29
#define VAR_ChannelsInUse                               30
#define VAR_DeviceOperationMode                         31
#define VAR_DistanceFromRoot                            32
#define VAR_PeerBSSID                                   33
#define VAR_AuthenticationServiceMode                   34
#define VAR_TotalBytesSent                              35
#define VAR_TotalBytesReceived                          36
#define VAR_TotalPacketsSent                            37
#define VAR_TotalPacketsReceived                        38
#define VAR_TotalAssociations                           39
#define VAR_AssociatedDeviceMACAddress                  40
#define VAR_AssociatedDeviceIPAddress                   41
#define VAR_AssociatedDeviceAuthenticationState         42
#define VAR_WEPKey                                      43
#define VAR_PreSharedKey                                44
#define VAR_PreSharedKeyIndex                           45

int GetWlanInfo(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetConfig(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetSSID(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetSSID(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetBSSID(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetChannel(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetChannelInfo(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetBeaconType(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetBeaconType(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetRadioMode(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetRadioMode(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetTotalAssociations(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetDefaultWEPKeyIndex(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetDefaultWEPKeyIndex(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetBasBeaconSecurityProperties(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetBasBeaconSecurityProperties(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetWPABeaconSecurityProperties(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetWPABeaconSecurityProperties(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetGenericAssociatedDeviceInfo(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetSpecificAssociatedDeviceInfo(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetSecurityKeys(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetSecurityKeys(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetWlanStatistics(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetWlanByteStatistics(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetWlanPacketStatistics(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetPreSharedKey(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetPreSharedKey(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
#endif /* LANWLANCFGPARAMS_H */
#endif
