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
//  Filename:       deviceinfoparams.h
//
******************************************************************************/
#ifndef _DEVICEINFOPARAMS_H
#define _DEVICEINFOPARAMS_H

#define GetInfo                      DefaultAction

#define VAR_Manufacturer                    0
#define VAR_ManufacturerOUI                 1
#define VAR_ModelName                       2
#define VAR_Description                     3
#define VAR_ProductClass                    4
#define VAR_SerialNumber                    5
#define VAR_SoftwareVersion                 6
#define VAR_HardwareVersion                 7
#define VAR_SpecVersion                     8
#define VAR_ProvisioningCode                9
#define VAR_UpTime                          10
#define VAR_DeviceLog                       11

int DeviceInfo_GetVar(struct Service *psvc, int varindex);
int SetProvisioningCode(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
int GetDeviceLog(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
#endif /* DEVICEINFOPARAMS_H */
