#ifndef _IP6T_MAC_H
#define _IP6T_MAC_H

#include <linux/netfilter/xt_mac.h>
//#define ip6t_mac_info xt_mac_info
struct ip6t_mac_info {
    unsigned char srcaddr[ETH_ALEN];
	int invert;
};

#endif /*_IP6T_MAC_H*/
