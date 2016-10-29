#ifndef _LINUX_IF_VNET_H_
#define _LINUX_IF_VNET_H_

#ifdef __KERNEL__

/* externally defined structs */
struct vlan_group;
struct net_device;
struct packet_type;
struct hlist_node;

#include <linux/netdevice.h>
#include <linux/etherdevice.h>

struct vnet_dev_info
{
    struct net_device *real_dev;
    int      mode;
    int      vnet_index;
    struct dev_mc_list *old_mc_list;  /* old multi-cast list for the VLAN interface..
                                       * we save this so we can tell what changes were
                                       * made, in order to feed the right changes down
                                       * to the real hardware...
                                       */
	int old_allmulti;               /* similar to above. */
	int old_promiscuity;            /* similar to above. */
	//struct proc_dir_entry *dent;    /* Holds the proc data */
	struct net_device_stats dev_stats; /* Device stats (rx-bytes, tx-pkts, etc...) */
	int mac_flag;
};

#define VNET_DEV_INFO(x) ((struct vnet_dev_info *)(x->priv))

#define VNET_ERR KERN_ERR
#define VNET_INF KERN_INFO
#define VNET_DBG KERN_ALERT /* change these... to debug, having a hard time
                             * changing the log level at run-time..for some reason.
                             */
#define VNET_NAME "vnet"

#endif /* __KERNEL__ */

enum vnet_ioctl_cmds
{
    ADD_VNET_CMD,
    DEL_VNET_CMD,
};

#define VNET_MODE_ROUTER   1
#define VNET_MODE_BRIDGE   2

struct vnet_ioctl_args {
	int cmd; /* Should be one of the vlan_ioctl_cmds enum above. */
	char device1[24];
	char device2[24];
	int mode;
};

#endif /*_LINUX_IF_VNET_H_*/

