#ifndef _IPT_MARK_MULTIOPT_H_target
#define _IPT_MARK_MULTIOPT_H_target

enum {
	IPT_MARK_MULTIOPT_SET=	(1<<0),
	IPT_MARK_MULTIOPT_AND=	(1<<1),
	IPT_MARK_MULTIOPT_OR=	(1<<2),
    IPT_RMARK_MULTIOPT_SET=	(1<<3)
};

struct ipt_mark_multiopt_target_info_v1 {
    unsigned long setmark;
	unsigned long andmark;
	unsigned long ormark;
	unsigned long setrmark;
	u_int8_t mode;
};


#endif /*_IPT_MARK_MULTIOPT_H_target*/
