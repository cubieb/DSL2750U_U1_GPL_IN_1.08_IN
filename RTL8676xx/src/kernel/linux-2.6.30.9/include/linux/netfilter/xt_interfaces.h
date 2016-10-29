#ifndef _XT_INTERFACES_H
#define _XT_INTERFACES_H

#include <linux/types.h>

#define WAN_INTERFACES 1
#define LAN_INTERFACES 2
#define WAN_LAN        3
#define SPECIFIC_LAN   4 
#define IN_DIR   1
#define OUT_DIR  2
#define BOTH_DIR 3

struct xt_interfaces_info {
    __u8 interfaces; // 1:wan 2:lan
    __u8 direction;//1:in;  2:out; 3:both
    __u8 invert;
    char name[30]; //lan name eth0/eth1/eth2/eth3/wl
};

#endif /*_XT_INTERFACES_H*/

