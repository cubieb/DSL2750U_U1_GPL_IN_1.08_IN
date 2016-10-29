#ifndef _IP6T_MARK_MULTIOPT_H_target
#define _IP6T_MARK_MULTIOPT_H_target
#if 0
struct ip6t_mark_target_info {
#ifdef KERNEL_64_USERSPACE_32
	unsigned long long mark;
#else
	unsigned long mark;
#endif
};
#endif

enum {
	IPT_MARK_MULTIOPT_SET=    (1<<0),
	IPT_MARK_MULTIOPT_AND=    (1<<1),
	IPT_MARK_MULTIOPT_OR=     (1<<2),
	IPT_RMARK_MULTIOPT_SET=   (1<<3)
};

struct ip6t_mark_target_info {
#ifdef KERNEL_64_USERSPACE_32
	unsigned long long setmark;
    unsigned long long andmark;
    unsigned long long ormark;
    unsigned long long setrmark;
#else
	unsigned long setmark;
    unsigned long andmark;
    unsigned long ormark;
    unsigned long setrmark;

#endif
	u_int8_t mode;
};

#endif /*_IP6T_MARK_MULTIOPT_H_target*/
