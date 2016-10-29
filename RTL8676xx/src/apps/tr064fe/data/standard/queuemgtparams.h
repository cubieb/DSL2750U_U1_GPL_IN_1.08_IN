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
//  Filename:       queuemgtparams.h
//
******************************************************************************/
#ifndef _QUEUEMGTPARAMS_H
#define _QUEUEMGTPARAMS_H


#define VAR_Enable                        0
#define VAR_MaxQueues                     1
#define VAR_MaxClassificationEntries      2
#define VAR_MaxAppEntries                 3
#define VAR_MaxFlowEntries                4
#define VAR_MaxPolicerEntries             5
#define VAR_MaxQueueEntries               6
#define VAR_DefaultForwardingPolicy       7
#define VAR_DefaultPolicer                8
#define VAR_DefaultQueue                  9
#define VAR_DefaultDSCPMark               10
#define VAR_DefaultEthernetPriorityMark   11
#define VAR_AvailableAppList              12
#define VAR_ClassificationNumberOfEntries 13
#define VAR_ClassificationKey             14
#define VAR_ClassificationEnable          15
#define VAR_ClassificationStatus          16
#define VAR_ClassificationOrder           17
#define VAR_ClassInterface                18
#define VAR_DestIP                        19
#define VAR_DestMask                      20
#define VAR_DestIPExclude                 21
#define VAR_SourceIP                      22
#define VAR_SourceMask                    23
#define VAR_SourceIPExclude               24
#define VAR_Protocol                      25
#define VAR_ProtocolExclude               26
#define VAR_DestPort                      27
#define VAR_DestPortRangeMax              28
#define VAR_DestPortExclude               29
#define VAR_SourcePort                    30
#define VAR_SourcePortRangeMax            31
#define VAR_SourcePortExclude             32
#define VAR_SourceMACAddress              33
#define VAR_SourceMACMask                 34
#define VAR_SourceMACExclude              35
#define VAR_DestMACAddress                36
#define VAR_DestMACMask                   37
#define VAR_DestMACExclude                38
#define VAR_Ethertype                     39
#define VAR_EthertypeExclude              40
#define VAR_SSAP                          41
#define VAR_SSAPExclude                   42
#define VAR_DSAP                          43
#define VAR_DSAPExclude                   44
#define VAR_LLCControl                    45
#define VAR_LLCControlExclude             46
#define VAR_SNAPOUI                       47
#define VAR_SNAPOUIExclude                48
#define VAR_SourceVendorClassID           49
#define VAR_SourceVendorClassIDExclude    50
#define VAR_DestVendorClassID             51
#define VAR_DestVendorClassIDExclude      52
#define VAR_SourceClientID                53
#define VAR_SourceClientIDExclude         54
#define VAR_DestClientID                  55
#define VAR_DestClientIDExclude           56
#define VAR_SourceUserClassID             57
#define VAR_SourceUserClassIDExclude      58
#define VAR_DestUserClassID               59
#define VAR_DestUserClassIDExclude        60
#define VAR_TCPACK                        61
#define VAR_TCPACKExclude                 62
#define VAR_IPLengthMin                   63
#define VAR_IPLengthMax                   64
#define VAR_IPLengthExclude               65
#define VAR_DSCPCheck                     66
#define VAR_DSCPExclude                   67
#define VAR_DSCPMark                      68
#define VAR_EthernetPriorityCheck         69
#define VAR_EthernetPriorityExclude       70
#define VAR_EthernetPriorityMark          71
#define VAR_VLANIDCheck                   72
#define VAR_VLANIDExclude                 73
#define VAR_ForwardingPolicy              74
#define VAR_ClassPolicer                  75
#define VAR_ClassQueue                    76
#define VAR_ClassApp                      77
#define VAR_AppNumberOfEntries            78
#define VAR_AppKey                        79
#define VAR_AppEnable                     80
#define VAR_AppStatus                     81
#define VAR_ProtocolIdentifier            82
#define VAR_AppName                       83
#define VAR_AppDefaultForwardingPolicy    84
#define VAR_AppDefaultPolicer             85
#define VAR_AppDefaultQueue               86
#define VAR_AppDefaultDSCPMark            87
#define VAR_AppDefaultEthernetPriorityMark  88
#define VAR_FlowNumberOfEntries             89
#define VAR_FlowKey                         90
#define VAR_FlowEnable                      91
#define VAR_FlowStatus                      92
#define VAR_FlowType                        93
#define VAR_FlowTypeParameters              94
#define VAR_FlowName                        95
#define VAR_AppIdentifier                   96
#define VAR_FlowForwardingPolicy            97
#define VAR_FlowPolicer                     98
#define VAR_FlowQueue                       99
#define VAR_FlowDSCPMark                    100
#define VAR_FlowEthernetPriorityMark        101
#define VAR_PolicerNumberOfEntries          102
#define VAR_PolicerKey                      103
#define VAR_PolicerEnable                   104
#define VAR_PolicerStatus                   105
#define VAR_CommittedRate                   106
#define VAR_CommittedBurstSize              107
#define VAR_ExcessBurstSize                 108
#define VAR_PeakRate                        109
#define VAR_PeakBurstSize                   110
#define VAR_MeterType                       111
#define VAR_PossibleMeterTypes              112
#define VAR_ConformingAction                113
#define VAR_PartialConformingAction         114
#define VAR_NonConformingAction             115
#define VAR_CountedPackets                  116
#define VAR_CountedBytes                    117
#define VAR_QueueNumberOfEntries            118
#define VAR_QueueKey                        119
#define VAR_QueueEnable                     120
#define VAR_QueueStatus                     121
#define VAR_QueueInterface                  122
#define VAR_QueueBufferLength               123
#define VAR_QueueWeight                     124
#define VAR_QueuePrecedence                 125
#define VAR_REDThreshold                    126
#define VAR_REDPercentage                   127
#define VAR_DropAlgorithm                   128
#define VAR_SchedulerAlgorithm              129
#define VAR_ShapingRate                     130
#define VAR_ShapingBurstSize                131

int GetQueueMgt_Info(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetQueueMGTEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);      
int SetDefaultBehavior(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int AddClassificationEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int DeleteClassificationEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetSpecificClassificationEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);       
int GetGenericClassificationEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);       
int SetClassificationEntryEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);       
int SetClassificationEntryOrder(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);       
int AddQueueEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int DeleteQueueEntry( UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetQueueEntryEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);       
int GetSpecificQueueEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);       
int GetGenericQueueEntry(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
#endif /* QUEUEMGTPARAMS_H */
