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
//  Filename:       wandsllinkconfig.h
//
******************************************************************************/
#ifndef _WANDSLLINKCONFIG_H_ 
#define _WANDSLLINKCONFIG_H_ 

#define GetInfo                 DefaultAction
#define GetModulationType       NotImplemented
#define GetDestinationAddress   DefaultAction
#define GetATMEncapsulation     DefaultAction
#define GetFCSPreserved         NotImplemented
#define SetFCSPreserved         NotImplemented

#define GetDSLLinkInfo          DefaultAction
#define GetAutoConfig           DefaultAction

#define VAR_Enable                   0
#define VAR_LinkStatus               1
#define VAR_LinkType                 2
#define VAR_AutoConfig               3
#define VAR_ModulationType           4
#define VAR_DestinationAddress       5
#define VAR_ATMEncapsulation         6
#define VAR_FCSPreserved             7
#define VAR_VCSearchList             8
#define VAR_ATMAAL                   9
#define VAR_ATMTransmittedBlocks     10
#define VAR_ATMReceivedBlocks        11
#define VAR_ATMQoS                   12
#define VAR_ATMPeakCellRate          13
#define VAR_ATMMaximumBurstSize      14
#define VAR_ATMSustainableCellRate   15
#define VAR_AAL5CRCErrors            16
#define VAR_ATMCRCErrors             17
#define VAR_ATMHECErrors             18

struct iftab {
    char *name;
    int  if_id;
};

typedef enum { 
    DSL_LINK_EoA, 
    DSL_LINK_IPoA, 
    DSL_LINK_PPoA,
    DSL_LINK_PPoE,
    DSL_LINK_CIP,
    DSL_LINK_UNCONFIGURED 
} dsl_link_t;

typedef enum { 
    DSL_LINK_UP, 
    DSL_LINK_DOWN, 
    DSL_LINK_INITIALIZING,
    DSL_LINK_UNAVAILABLE 
} dsl_conn_t;


struct WANDSLLinkConfigData {
    dsl_link_t  link_type;
    dsl_conn_t	link_status;
    int		autoconfig;
    timer_t     eventhandle;
};

typedef struct WANDSLLinkConfigData WANDSLLinkConfigData, *PWANDSLLinkConfigData;

int WANDSLLinkConfig_GetVar(struct Service *psvc, int varindex);

int SetATMQoS(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetDestinationAddress(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetDSLEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetATMEncapsulation(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int SetDSLLinkType(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetStatisticsWANDSL(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif /* _WANDSLLINKCONFIG_H_  */
