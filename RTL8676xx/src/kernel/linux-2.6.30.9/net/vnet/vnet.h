
#ifndef __VNET_H__
#define  __VNET_H__

//#define VNET_DEBUG
#ifdef VNET_DEBUG
	#define VNET_PRINTK(fmt, args...) \
		printk(VNET_DBG "#%s, %d, %s#, " fmt, __FILE__, __LINE__, __func__, ##args)
#else
	#define VNET_PRINTK(fmt, args...)
#endif /*VNET_DEBUG*/

#define MAX_VNET_NUM  16

#define REAL_DEV_HASH_SHIFT	5
#define REAL_DEV_HASH_SIZE	(1 << REAL_DEV_HASH_SHIFT)
#define REAL_DEV_HASH_MASK	(REAL_DEV_HASH_SIZE - 1)

struct vnet_real_dev {
	struct hlist_node	hlist;	/* linked list */
	struct rcu_head		rcu;
	struct net_device   *vnet_dev[MAX_VNET_NUM];
	int  vnet_cnt;
	int  real_dev_ifindex; /* The ifindex of the ethernet(like)
							  device the vlan is attached to. */
};

#define VNET_HASH_BITS 8
#define VNET_HASH_SIZE 128

#define VNET_AGEING_TIMER    180
#define get_timeout() (jiffies+VNET_AGEING_TIMER*HZ)

struct vnet_fdb
{
	spinlock_t			hash_lock;
	struct hlist_head	hash[VNET_HASH_SIZE];
	struct timer_list	ageing_timer;
	int		addr_num;
	int		addr_limited;
};

struct vnet_fdb_entry
{
	struct hlist_node	hlist;
	struct rcu_head		rcu;
	struct net_device	*dst;
	struct net_device	*real_dev;
	atomic_t			use_count;
	unsigned long		ageing_timer;
	unsigned char		addr[6];
	int  				is_local;
	int     			is_static;
};

#define print_mac(mac)   \
	printk(KERN_ALERT "%02x:%02x:%02x:%02x:%02x:%02x \n", \
		    mac[0],mac[1],mac[2],mac[3],mac[4],mac[5])

struct net_device_stats *vnet_dev_get_stats(struct net_device *dev);

int vnet_dev_hard_start_xmit(struct sk_buff *skb, struct net_device *dev);

int vnet_dev_change_mtu(struct net_device *dev, int new_mtu);

int vnet_dev_set_mac_address(struct net_device *dev, void *addr_struct_p);

int vnet_dev_open(struct net_device *dev);

int vnet_dev_stop(struct net_device *dev);

int vnet_dev_ioctl(struct net_device *dev, struct ifreq *ifr, int cmd);

/** Taken from Gleb + Lennert's VLAN code, and modified... */
void vnet_dev_set_multicast_list(struct net_device *vlan_dev);

void vnet_fdb_update(struct net_device *source, const unsigned char *addr);

int vnet_fdb_insert(struct net_device *source, const unsigned char *addr);

void vnet_fdb_delete_by_port(struct net_device *p);
void vnet_fdb_del_by_real_dev(struct net_device *real_dev, int local_stat);

void vnet_fdb_visit(void (*func)(void *,struct vnet_fdb_entry*),void *ptr);

struct net_device *vnet_get_device(const unsigned char *addr, struct net_device *real_dev);

void vnet_fdb_changeaddr(struct net_device *p, const unsigned char *newaddr);

void vnet_fdb_init(void);
void vnet_fdb_fini(void);

int get_vnet_fdb_entry_num(void);

int get_vnet_fdb_entry_limited(void);

#ifdef CONFIG_PROC_FS
	int vnet_proc_init(void);
	void vnet_proc_cleanup (void);
#else /* No CONFIG_PROC_FS */
	#define vnet_proc_init()	(0)
	#define vnet_proc_cleanup()	do {} while(0)
#endif /*CONFIG_PROC_FS*/

#endif  /*   __VNET_H__ */

