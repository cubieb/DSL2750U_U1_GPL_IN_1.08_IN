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
//  Filename:       wanethlinkconfig.h
//
******************************************************************************/
#ifdef INCLUDE_WANETHERNETLINKCONFIG
#ifndef _WANETHLINKCONFIG_H_ 
#define _WANETHLINKCONFIG_H_ 

#define GetEthernetLinkStatus DefaultAction

#define VAR_EthernetLinkStatus             0

int WANETHLinkConfig_GetVar(struct Service *psvc, int varindex);
#endif /* _WANETHLINKCONFIG_H_  */
#endif
