/* ip6tables module for matching the flow label value
 * xuxiaodong <xuxiaodong@twsz.com>
 */

#ifndef _IP6T_FL_H
#define _IP6T_FL_H

#define IPV6_FLOW_LABLE_MASK 0x000FFFFF

struct ip6t_fl_info {
	unsigned int flowlabel;
	u_int8_t invert;
};

#endif /*_IP6T_FL_H*/

