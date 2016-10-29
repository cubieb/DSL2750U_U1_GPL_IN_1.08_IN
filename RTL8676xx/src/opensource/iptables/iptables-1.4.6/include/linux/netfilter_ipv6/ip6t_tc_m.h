/* ip6tables module for matching the traffic class value
 * xuxiaodong <xuxiaodong@twsz.com>
 */

#ifndef _IP6T_TC_H
#define _IP6T_TC_H

#define IPV6_TC_MASK 0x0FF0

struct ip6t_tc_info {
	unsigned char tc;
	u_int8_t invert;
};

#endif /*_IP6T_TC_H*/

