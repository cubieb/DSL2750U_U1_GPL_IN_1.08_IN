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
//  Filename:       wandslifcfgparams.h
//
******************************************************************************/
#ifndef _WANDSLIFCFGPARAMS_H_ 
#define _WANDSLIFCFGPARAMS_H_ 

#define GetInfo DefaultAction

#define VAR_Enable                 0
#define VAR_Status                 1
#define VAR_UpstreamCurrRate       2
#define VAR_DownstreamCurrRate     3
#define VAR_UpstreamMaxRate        4
#define VAR_DownstreamMaxRate      5
#define VAR_UpstreamNoiseMargin    6
#define VAR_DownstreamNoiseMargin  7
#define VAR_UpstreamAttenuation    8
#define VAR_DownstreamAttenuation  9
#define VAR_UpstreamPower          10
#define VAR_DownstreamPower        11
#define VAR_ATURVendor             12
#define VAR_ATURCountry            13
#define VAR_ATURANSIStd            14
#define VAR_ATURANSIRev            15
#define VAR_ATUCVendor             16
#define VAR_ATUCCountry            17
#define VAR_ATUCANSIStd            18
#define VAR_ATUCANSIRev            19
#define VAR_TotalStart             20
#define VAR_ShowTimeStart          21
#define VAR_LastShowTimeStart      22
#define VAR_CurrentDayStart        23
#define VAR_QuarterHourStart       24

#define VAR_ReceiveBlocks          25
#define VAR_TransmitBlocks         26
#define VAR_CellDelin              27
#define VAR_LinkRetrain            28
#define VAR_InitErrors             29
#define VAR_InitTimeouts           30
#define VAR_LossOfFraming          31
#define VAR_ErroredSecs            32
#define VAR_SeverelyErroredSecs    33
#define VAR_FECErrors              34
#define VAR_ATUCFECErrors          35
#define VAR_HECErrors              36
#define VAR_ATUCHECErrors          37
#define VAR_CRCErrors              38
#define VAR_ATUCCRCErrors          39

int WANDSLInterfaceConfig_GetVar(struct Service *psvc, int varindex);
int SetDSLInterfaceEnable(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetStatisticsTotal(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetStatisticsShowTime(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);

#endif /* _WANDSLLINKCONFIG_H_  */
