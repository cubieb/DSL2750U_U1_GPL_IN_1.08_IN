#ifndef _XT_INTERFACES_H
#define _XT_INTERFACES_H
#define WAN_INTERFACES 1
#define LAN_INTERFACES 2
#define WAN_LAN        3
#define SPECIFIC_LAN   4 
#define IN_DIR   1
#define OUT_DIR  2
#define BOTH_DIR 3

struct xt_interfaces_info {
    u_int8_t interfaces;     // 1:wan 2:lan 3:wan/lan 4: specific lan
    u_int8_t direction;      //1:in;  2:out; 3:both
    u_int8_t invert; 
    char     name[30];       // specific lan name
};

#endif /*_XT_INTERFACES_H*/

