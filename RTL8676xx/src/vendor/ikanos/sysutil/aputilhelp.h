/* Copyright (C) 2006 Ikanos Communications 
 * See IKANOS_PROP_LICENSE.txt for license information. */



/*****************************************************************************
 * File Name     : aputilhelp.h
 * Author        : Sreeni
 * Description   : Help macros for file aputil.c
 *****************************************************************************/

#define  MRW_COMMAND_SYNTAX "\n[mread/mwrite] <mem-address> <len> <value>\n \
    mread/mwrite : Memory read or write operation\n \
    mem-address  : 32bit Address in hex range 0x80000000 - 0xc0000000 \n \
    len          : No.of 32bit words in Decimal to read/write \n \
    value        : 32bit Value to be written at \n \
                   mem-address location\n \
    Ex: mread b9110300 10 \n \
        mwrite 801ecfb4 1 11112222 \n"

#define  APRW_APCOMMAND_SYNTAX "\n[aread/awrite] <apId> <address> <len> <value>\n \
    aread/awrite : AP Data area read or write operation\n \
    apId         : AP Id value (1- Eth0 2-Eth1 3-Eth2 4-SEC 5-BM 6-ATM) \n \
    address      : address range from 0x0 - 0x7ff \n \
    len          : No.of bytes in decimal to read/write \n \
    value        : 8bit Value to be written at \n \
                   above address location\n \
    Ex: aread 100 10 \n \
        awrite 100 1 11 \n"

#define  SYSUTIL_APROUTE_TABLE_SYNTAX "\n aproutetbl <apId> \n "
#define  SYSUTIL_AP_RESET_STAT_SYNTAX "\n apresetstats <apId> \n "
#define SYSUTIL_AP_FEATURE_BIT_SYNTAX "\n apfeature <apId> <feature | bit_position> <enable/disable> \n \
Examples:\n apfeature 1 PROMISC enable \n apfeature ALL VLANBRIDGE enable\n\n"
