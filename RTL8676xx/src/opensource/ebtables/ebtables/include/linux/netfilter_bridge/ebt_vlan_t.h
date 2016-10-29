#ifndef __LINUX_BRIDGE_EBT_VALN_T_H
#define __LINUX_BRIDGE_EBT_VLAN_T_H

struct ebt_vlan_t_info
{
	unsigned char cmd;  /*0x01 is set-vlan 0x02 is set-header 0x03 is untag*/
	unsigned char priority;
	unsigned short vlan;
	/* EBT_ACCEPT, EBT_DROP, EBT_CONTINUE or EBT_RETURN */
	int target;
};
#define EBT_VLAN_TARGET "vlan"

#endif

