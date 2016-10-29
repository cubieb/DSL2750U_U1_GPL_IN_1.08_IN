#ifndef	RTL_NIC_H
#define	RTL_NIC_H

#include "rtl865x_netif.h"
#include <linux/interrupt.h>
#include <linux/netdevice.h>
/************************************
*	feature enable/disable
*************************************/
#define RX_TASKLET	1
#define TX_TASKLET	1
#define LINK_TASKLET 1
#define RTL819X_PRIV_IOCTL_ENABLE 1 	/* mark_add */
//#define CONFIG_RTL_PHY_PATCH		1   //Kevin
#define RTK_QUE			1
/*
*#define	CONFIG_RTL_MULTI_LAN_DEV	1
*/

#if defined(CONFIG_POCKET_AP_SUPPORT)
#define	CONFIG_POCKET_ROUTER_SUPPORT
#endif

/************************************
*	const variable defination
*************************************/
#define	RTL_BridgeWANVLANID		7 /* WAN vid (bridged, default no vlan tag)*/
#define	RTL_WANVLANID			8 /* WAN vid (routed,   default no vlan tag)*/
#define	RTL_LANVLANID			9 /* LAN vid  (default no vlan tag) */

#if defined(CONFIG_RTK_VLAN_SUPPORT) || defined (CONFIG_RTL_MULTI_LAN_DEV)
	#if defined(CONFIG_8198_PORT5_GMII)
		#define ETH_INTF_NUM	6
	#else
		#define ETH_INTF_NUM	5 /* nas0m eth0.2~eth0.5 */
	#endif
#else
#define ETH_INTF_NUM	1 /* eth0 */
#endif

#if defined(CONFIG_RTL_ETH_PRIV_SKB)
	#if defined(CONFIG_RTL_819X) || defined(CONFIG_RTL_8676HWNAT)
		#define MAX_ETH_SKB_NUM	(\
			NUM_RX_PKTHDR_DESC \
			+ NUM_RX_PKTHDR_DESC1 \
			+ NUM_RX_PKTHDR_DESC2 \
			+ NUM_RX_PKTHDR_DESC3 \
			+ NUM_RX_PKTHDR_DESC4 \
			+ NUM_RX_PKTHDR_DESC5 \
			+ MAX_PRE_ALLOC_RX_SKB + 512)
	#else
		#define MAX_ETH_SKB_NUM	(\
			NUM_RX_PKTHDR_DESC \
			+ NUM_RX_PKTHDR_DESC1 \
			+ NUM_RX_PKTHDR_DESC2 \
			+ NUM_RX_PKTHDR_DESC3 \
			+ NUM_RX_PKTHDR_DESC4 \
			+ NUM_RX_PKTHDR_DESC5 \
			+ MAX_PRE_ALLOC_RX_SKB + 256)
	#endif
#define ETH_SKB_BUF_SIZE	(SKB_DATA_ALIGN(CROSS_LAN_MBUF_LEN+sizeof(struct skb_shared_info)+160+NET_SKB_PAD))
#define ETH_MAGIC_CODE		"819X"
#define ETH_MAGIC_LEN		4
#endif

struct re865x_priv
{
	u16			ready;
	u16			addIF;
	u16			devnum;
	u32			sec_count;
	u32			sec;
//#if defined(CONFIG_RTK_VLAN_SUPPORT) || defined (CONFIG_RTL_MULTI_LAN_DEV)
	struct	net_device	*dev[ETH_INTF_NUM];
//#else
//	struct	net_device	*dev[NETIF_NUMBER];
//#endif
#ifdef CONFIG_RTL_STP
	struct	net_device	*stp_port[MAX_RE865X_STP_PORT];
#endif
#if defined(CONFIG_RTL_CUSTOM_PASSTHRU)
	struct	net_device	*pdev;
#endif
	spinlock_t		lock;
	void			*regs;
	struct tasklet_struct	rx_tasklet;
	struct timer_list timer;	/* Media monitoring timer. */
	unsigned long		linkchg;	
};

struct dev_priv {
	u32			id;            /* VLAN id, not vlan index */
	u32			portmask;     /* member port mask */
	u32			portnum;     	/* number of member ports */
	u32			netinit;
	struct net_device	*dev;
	//struct net_device   *dev_prev;
	//struct net_device   *dev_next;
#ifdef RX_TASKLET
	struct tasklet_struct   rx_dsr_tasklet;
#endif

#ifdef TX_TASKLET
	struct tasklet_struct   tx_dsr_tasklet;
#endif

	struct tasklet_struct   link_dsr_tasklet;

#if defined(CONFIG_RTK_VLAN_SUPPORT)
	struct vlan_info	vlan_setting;
#endif

#ifdef CP_VLAN_TAG_USED
	struct vlan_group	*vlgrp;
#endif
	spinlock_t			lock;
	u32			msg_enable;
	u32 			opened;
	u32			irq_owner; //record which dev request IRQ
	struct net_device_stats net_stats;
#if defined(DYNAMIC_ADJUST_TASKLET) || defined(CONFIG_RTL8186_TR) || defined(BR_SHORTCUT) || defined(CONFIG_RTL8196C_REVISION_B) || defined(CONFIG_RTL_8198)
    struct timer_list expire_timer; 
#endif

#ifdef CONFIG_RTL8196C_GREEN_ETHERNET
    struct timer_list expire_timer2; 
#endif
#ifdef CONFIG_RTL_8676HWNAT
	unsigned int port_member;
#endif

};

#ifdef CONFIG_RTL_8676HWNAT
extern int enable_port_mapping;
struct ifvlan
{
	int cmd;
	char enable;
	short vlanIdx;
	short vid;
	char	disable_priority;
	int member;
	int port;
	char txtag;
};
#endif


/*	define return value		*/
#define	RTL_RX_PROCESS_RETURN_SUCCESS		0
#define	RTL_RX_PROCESS_RETURN_CONTINUE		-1
#define	RTL_RX_PROCESS_RETURN_BREAK		-2


#if defined (CONFIG_RTL_UNKOWN_UNICAST_CONTROL)
#define	RTL_MAC_RECORD_NUM		4	/* Must be 2's orders */
#define	RTL_MAC_THRESHOLD		128	/* threshold for block unkown unicast */
#define	RTL_MAC_TIMEOUT			10*HZ	/* unkown unicast restriction time */
#define	RTL_MAC_REFILL_TOKEN		25000	/* per second refill token number */
typedef	struct __rtlMacRecord
{
	uint8		enable;
	uint8		cnt;
	uint8		mac[ETHER_ADDR_LEN];
	struct timer_list	timer;
}	rtlMacRecord;
#endif

#if defined(RTL819X_PRIV_IOCTL_ENABLE)
#define RTL819X_IOCTL_READ_PORT_STATUS			(SIOCDEVPRIVATE + 0x01)	
#define RTL819X_IOCTL_READ_PORT_STATS	              (SIOCDEVPRIVATE + 0x02)	

struct lan_port_status {
    unsigned char link;
    unsigned char speed;
    unsigned char duplex;
    unsigned char nway;    	
}; 

struct port_statistics  {
	unsigned int  rx_bytes;		
 	unsigned int  rx_unipkts;		
       unsigned int  rx_mulpkts;			
	unsigned int  rx_bropkts;		
 	unsigned int  rx_discard;		
       unsigned int  rx_error;			
	unsigned int  tx_bytes;		
 	unsigned int  tx_unipkts;		
       unsigned int  tx_mulpkts;			
	unsigned int  tx_bropkts;		
 	unsigned int  tx_discard;		
       unsigned int  tx_error;			   
};
#endif

#if defined(CONFIG_RTK_VLAN_SUPPORT) || defined (CONFIG_RTL_MULTI_LAN_DEV)
#define	RTL_LANVLANID_1		9
#define	RTL_LANVLANID_2		10
#define	RTL_LANVLANID_3		11
#define	RTL_LANVLANID_4		12
#define RTL_LANVLANID_5		13
#endif
#define	RTL_PPTPL2TP_VLANID	999

//flowing name in protocol stack DO NOT duplicate
#define RTL_PS_BR0_DEV_NAME RTL_BR_NAME
#define RTL_PS_ETH_NAME	"eth"
#define RTL_PS_WLAN_NAME	RTL_WLAN_NAME
#define RTL_PS_PPP_NAME	"ppp"
#define RTL_PS_LAN_P0_DEV_NAME RTL_DEV_NAME_NUM(RTL_PS_ETH_NAME,0)
#define RTL_PS_WAN0_DEV_NAME RTL_DEV_NAME_NUM(RTL_PS_ETH_NAME,1)
#define RTL_PS_WAN1_DEV_NAME RTL_DEV_NAME_NUM(RTL_PS_ETH_NAME,2)
#define RTL_PS_PPP0_DEV_NAME RTL_DEV_NAME_NUM(RTL_PS_PPP_NAME,0)
#define RTL_PS_PPP1_DEV_NAME RTL_DEV_NAME_NUM(RTL_PS_PPP_NAME,1)
#define RTL_PS_WLAN0_DEV_NAME RTL_DEV_NAME_NUM(RTL_PS_WLAN_NAME,0)
#define RTL_PS_WLAN1_DEV_NAME RTL_DEV_NAME_NUM(RTL_PS_WLAN_NAME,1)

#if defined(CONFIG_RTL_PUBLIC_SSID)
#define RTL_LAN_DEVICE_NAME "br0"
#define RTL_GW_WAN_DEVICE_NAME "br1"
#define RTL_WISP_WAN_DEVICE_NAME "wlan0"
#define RTL_BR_WAN_DEVICE_NAME "br0"
#endif

//Used by fastpath mac-based qos under IMPROVE_QOS 
#define QOS_LAN_DEV_NAME RTL_PS_BR0_DEV_NAME

#ifdef CONFIG_RTL_LAYERED_DRIVER
struct rtl865x_vlanConfig {
	uint8 			ifname[IFNAMSIZ];
	uint8			isWan;
	uint16			if_type;
	uint16			vid;
	uint16			fid;
	uint32			memPort;
	uint32			untagSet;
	uint32			mtu;
	ether_addr_t		mac;
	uint8			is_slave;
};
#define RTL865X_CONFIG_END					{ "", 0, 0, 0, 0, 0, 0, 0, {{0}}, 0 }

#define GATEWAY_MODE				0
#define BRIDGE_MODE				1
#define WISP_MODE					2
//#define MULTIPLE_VLAN_BRIDGE_MODE 3
//#define MULTIPLE_VLAN_WISP_MODE 4
#define CONFIG_CHECK(expr) do {\
	if(((int32)expr)!=SUCCESS){\
		rtlglue_printf("Error >>> %s:%d failed !\n", __FUNCTION__,__LINE__);\
			return FAILED;\
	}\
}while(0)

#define INIT_CHECK(expr) do {\
	if(((int32)expr)!=SUCCESS){\
		rtlglue_printf("Error >>> %s:%d failed !\n", __FUNCTION__,__LINE__);\
			return FAILED;\
	}\
}while(0)
#endif

typedef struct _ps_drv_netif_mapping_s
{
	uint32 valid:1, //entry enable?
		flags;	//reserverd
	struct net_device *ps_netif; //linux ps network interface
	char drvName[IFNAMSIZ];//netif name in driver
	
}ps_drv_netif_mapping_t;

int32 rtl865x_changeOpMode(int mode,int allLan);
struct net_device* re865x_get_netdev_by_name(const char* name);
#ifdef CONFIG_RTL_MULTI_ETH_WAN
int32 rtl865x_updateNetifForPortmapping(void);
#endif
#ifdef CONFIG_RTL_8676HWNAT
int rtl_set_wanport_vlanconfig(char *ifname, int proto, int vid, int napt /*, unsigned char *addr*/);
int rtl_set_wanport_portmapping(unsigned int member, int vid, int proto);
int rtl_reset_wanport_vlanconfig(int vid, int proto);
int rtl865x_unregisterDev(char *ifname);
int rtl865x_setNetifMacAddr(char *ifname, unsigned char *addr);
#endif

#if defined(CONFIG_RTL_ETH_PRIV_SKB)
int is_rtl865x_eth_priv_buf(unsigned char *head);
void free_rtl865x_eth_priv_buf(unsigned char *head);
#endif
#if defined(CONFIG_RTL_CUSTOM_PASSTHRU)
extern int __init rtl8651_customPassthru_init(void);
int32 rtl8651_initStormCtrl(void);
#endif

ps_drv_netif_mapping_t* rtl_get_ps_drv_netif_mapping_by_psdev(struct net_device *dev);

#if defined(CONFIG_RTK_VLAN_SUPPORT) && defined(CONFIG_RTK_VLAN_FOR_CABLE_MODEM)
extern struct net_device* get_dev_by_vid(int vid);
#endif
#if defined(BR_SHORTCUT)
extern struct net_device *get_shortcut_dev(unsigned char *da);
#endif
//#define CONFIG_RTL_NIC_HWSTATS

#endif