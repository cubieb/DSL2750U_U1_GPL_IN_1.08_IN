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
//  Filename:       lancfgsecparams.h
//
******************************************************************************/
#ifndef _LANCFGSECPARAMS_H
#define _LANCFGSECPARAMS_H

#define VAR_ConfigPassword                    0

int SetConfigPassword(UFILE *uclient, PService psvc, PAction ac, pvar_entry_t args, int nargs);
#endif /* _LANCFGSECPARAMS_H */
