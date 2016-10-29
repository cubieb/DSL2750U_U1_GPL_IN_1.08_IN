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
//  Filename:       layer2bridgeparams.h
//
******************************************************************************/

#ifndef _LAYER2BRIDGEPARAMS_H
#define _LAYER2BRIDGEPARAMS_H

#define VAR_MaxBridgeEntries                          0
#define VAR_MaxFilterEntries                          1
#define VAR_MaxMarkingEntries                         2
#define VAR_BridgeNumberOfEntries                     3
#define VAR_BridgeKey                                 4
#define VAR_BridgeEnable                              5
#define VAR_BridgeStatus                              6
#define VAR_BridgeName                                7
#define VAR_VLANID                                    8
#define VAR_FilterNumberOfEntries                     9
#define VAR_FilterKey                                 10
#define VAR_FilterEnable                              11
#define VAR_FilterStatus                              12
#define VAR_FilterBridgeReference                     13
#define VAR_ExclusivityOrder                          14
#define VAR_FilterInterface                           15
#define VAR_VLANIDFilter                              16
#define VAR_AdmitOnlyVLANTagged                       17
#define VAR_EthertypeFilterList                       18
#define VAR_EthertypeFilterExclude                    19
#define VAR_SourceMACAddressFilterList                20
#define VAR_SourceMACAddressFilterExclude             21
#define VAR_DestMACAddressFilterList                  22
#define VAR_DestMACAddressFilterExclude               23
#define VAR_SourceMACFromVendorClassIDFilter          24
#define VAR_SourceMACFromVendorClassIDFilterExclude   25
#define VAR_DestMACFromVendorClassIDFilter            26
#define VAR_DestMACFromVendorClassIDFIlterExclude     27
#define VAR_SourceMACFromClientIDFilter               28
#define VAR_SourceMACFromClientIDFilterExclude        29
#define VAR_DestMACFromClientIDFilter                 30
#define VAR_DestMACFromClientIDFilterExclude          31
#define VAR_SourceMACFromUserClassIDFilter            32
#define VAR_SourceMACFromUserClassIDFilterExclude     33
#define VAR_DestMACFromUserClassIDFilter              34
#define VAR_DestMACFromUserClassIDFilterExclude       35
#define VAR_MarkingNumberOfEntries                    36
#define VAR_MarkingKey                                37
#define VAR_MarkingEnable                             38
#define VAR_MarkingStatus                             39
#define VAR_MarkingBridgeReference                    40
#define VAR_MarkingInterface                          41
#define VAR_VLANIDUntag                               42
#define VAR_VLANIDMark                                43
#define VAR_EnternetPriorityMark                      44
#define VAR_EthernetPriorityOverride                  45
#define VAR_AvailableInterfaceNumberOfEntries         46
#define VAR_AvailableInterfaceKey                     47
#define VAR_InterfaceType                             48
#define VAR_InterfaceReference                        49

int GetLayer2Bridge_Info(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int AddBridgeEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int DeleteBridgeEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetSpecificBridgeEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);       
int GetGenericBridgeEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);       
int SetBridgeEntryEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);       
int GetSpecificAvailableInterfaceEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);       
int GetGenericAvailableInterfaceEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);       
#endif /* LAYER2BRIDGEPARAMS_H */
