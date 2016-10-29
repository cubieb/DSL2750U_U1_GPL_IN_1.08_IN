#ifndef __LINUX_BRIDGE_EBT_CLASSIFY_H
#define __LINUX_BRIDGE_EBT_CLASSIFY_H

struct ebt_classify_target_info
{
	u_int32_t priority;
	int target;
};

#define EBT_CLASSIFY_TARGET "classify"

#endif

