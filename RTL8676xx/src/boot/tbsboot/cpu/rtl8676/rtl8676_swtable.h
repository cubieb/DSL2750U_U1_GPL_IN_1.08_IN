/*
* ----------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* $Header: /usr/local/dslrepos/boot0412/tftpnaive/sw/include/rtl8650/swTable.h,v 1.1.1.1 2011/06/10 08:06:31 yachang Exp $
*
* Abstract: Switch core polling mode NIC header file.
*

*
* ---------------------------------------------------------------
*/


#ifndef _SWNIC_TABLE_H
#define _SWNIC_TABLE_H

//#include <rtl_types.h>
//#include <rtl_errno.h>
//#include <rtl8650/asicregs.h>


void tableAccessForeword(u32, u32, void *);
int swTable_addEntry(u32 tableType, u32 eidx, void *entryContent_P);
int swTable_modifyEntry(u32 tableType, u32 eidx, void *entryContent_P);
int swTable_forceAddEntry(u32 tableType, u32 eidx, void *entryContent_P);
int swTable_readEntry(u32 tableType, u32 eidx, void *entryContent_P);




#endif /* _SWNIC_H */
