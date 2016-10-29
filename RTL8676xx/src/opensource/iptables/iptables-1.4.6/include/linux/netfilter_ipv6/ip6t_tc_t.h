/* ip6tables module for setting the IPv6 traffic class field
 * xuxiaodong <xuxiaodong@twsz.com>
 */

#ifndef _IP6T_TC_H_target
#define _IP6T_TC_H_target

#define IPV6_TC_MASK 0x0FF0

struct ip6t_TC_info {
	unsigned char tc;
};

#endif /*_IP6T_TC_H_target*/
