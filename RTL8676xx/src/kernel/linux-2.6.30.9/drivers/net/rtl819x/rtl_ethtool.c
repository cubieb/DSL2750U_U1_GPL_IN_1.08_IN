/*
*	Copyright. All rights is diminished
*
*	$Author: August 2011-10-20
*
*
*	Ethtool for adsl-switch-0412
*/

#include "rtl.h"


#if defined(CONFIG_RTL_MULTI_LAN_DEV) && defined(CONFIG_RTL_MULTI_ETH_WAN)

#if 0
//Currently, the name of dev is eth0.2, eth0.3, ...
#define DEV_TO_PORT(dev, port)		\
	do { 								\
			port = (('e' == dev->name[0]) ? (dev->name[5] - '1') : 0); \
		} while(0)



#define PORT_TO_DEV(portnum, dev)		\
	do {								\
			dev = (_rtl86xx_dev.dev[portnum]);	\
		} while(0)
#endif

/****************************************
 * check whether the port is detected
 *
 *  user output: Link detected: yes/no
****************************************/
static u32 rtl_ethtool_get_port_link(struct net_device *dev)
{
	uint32  portnum;
	uint32	regData;

	//AUG_DBG("dev: %s\n", dev->name);
	//the user space has check that whether dev exist.
	DEV_TO_PORT(dev, portnum);

	regData = READ_MEM32(PSRP0+((portnum)<<2));

	return (regData & PortStatusLinkUp);		
}


/************************************************
 * get the Speed, Link Mode, Duplex, 
 *				N-way(Auto-neg), etc.
 *
 *
************************************************/
static int rtl_ethtool_get_settings(struct net_device *dev, struct ethtool_cmd *ecmd)
{
	uint32	regData;
	uint32	portnum;

	uint32  tmp_spd;

	DEV_TO_PORT(dev, portnum);
	
	regData = READ_MEM32(PSRP0+((portnum)<<2));

	ecmd->supported = (SUPPORTED_10baseT_Half 		
							|	SUPPORTED_10baseT_Full 	
							|	SUPPORTED_100baseT_Half 	
							|	SUPPORTED_100baseT_Full
							|	SUPPORTED_1000baseT_Full
							|	SUPPORTED_Autoneg 		
							|	SUPPORTED_TP 
							|	SUPPORTED_Pause);
		                   
	ecmd->advertising = ADVERTISED_10baseT_Half 
							|	ADVERTISED_10baseT_Full
							|	ADVERTISED_100baseT_Full
							|	ADVERTISED_100baseT_Half
							|	ADVERTISED_Autoneg
							|	ADVERTISED_Pause
							|	ADVERTISED_TP;

	if(0 == portnum)
		ecmd->advertising |= ADVERTISED_1000baseT_Full;

	ecmd->port = PORT_TP;

	ecmd->autoneg = (regData & PortStatusNWayEnable) ? AUTONEG_ENABLE : AUTONEG_DISABLE;

	ecmd->duplex = (regData & PortStatusDuplex) ? DUPLEX_FULL : DUPLEX_HALF;

	ecmd->transceiver = XCVR_INTERNAL;

	ecmd->phy_address = portnum;

	tmp_spd = regData & PortStatusLinkSpeed_MASK;

	//if the port is not linked, we set the speed as "UNKOWN!".
	if(rtl_ethtool_get_port_link(dev))
		ecmd->speed = tmp_spd==PortStatusLinkSpeed100M ? 
								 SPEED_100 : ( tmp_spd==PortStatusLinkSpeed100M? SPEED_1000 : SPEED_10);
	else
		ecmd->speed = 0;

	return 0;	
}


/************************************************
 * get the pauseparam.
 *
 * shell output:
 		# ethtool --show-pause eth0.2
			Pause parameters for eth0.2:
			Autonegotiate:  on
			RX:             off
			TX:             off
 *
************************************************/
static void rtl_ethtool_get_pauseparam(struct net_device *dev, struct ethtool_pauseparam *pause)
{
	uint32	regData;
	uint32	portnum;

	DEV_TO_PORT(dev, portnum);
	
	regData = READ_MEM32(PSRP0+((portnum)<<2));

	pause->autoneg = (regData & PortStatusNWayEnable) ? AUTONEG_ENABLE : AUTONEG_DISABLE;

	pause->rx_pause = (regData & PortStatusRXPAUSE) ? 1 : 0;
	pause->tx_pause = (regData & PortStatusTXPAUSE) ? 1 : 0;
	
}

static void rtl_ethtool_get_drvinfo(struct net_device *dev, struct ethtool_drvinfo *drvinfo)
{
	printk("\033[9m %s \033[m\n", "RTL Rights Reserved!");
}



static const struct ethtool_ops rtl_ethtool_ops = {
	.get_link 			= ethtool_op_get_link,//rtl_ethtool_get_port_link,
	.get_drvinfo		= rtl_ethtool_get_drvinfo,
	.get_settings 		= rtl_ethtool_get_settings,
	.get_pauseparam		= rtl_ethtool_get_pauseparam,

};

void rtl_set_ethtool_ops(struct net_device *netdev)
{
	SET_ETHTOOL_OPS(netdev, &rtl_ethtool_ops);
}

#endif



//make wrappers of netlink funcs and make a protocol of kernel-2-user communication

struct sock *global_rtlmsg_sock;

#ifdef CONFIG_RTL_NLMSG_PROTOCOL
void rtl_nlmsg_handler(struct sk_buff *skb)
{
	printk("Nothing to do!!!\n");
	return;
}


int rtl_gbl_rtlmsg_sock_init(void)
{
	global_rtlmsg_sock = netlink_kernel_create(&init_net, NETLINK_RTL_NLMSG, 0, rtl_nlmsg_handler, NULL, THIS_MODULE);
    if(!global_rtlmsg_sock)
    {
        printk("Fail to create netlink socket.\n");
        return -1;
    }
	return 0;
}


struct sk_buff* rtl_nlmsg_alloc_skb(const RTL_ST_NLMSG* p_nlmsg)
{
	int len;
	struct sk_buff  *skb;

	len = NLMSG_SPACE(RTL_NLMSG_HEAD_SZ + p_nlmsg->length);

	skb = alloc_skb(len, GFP_ATOMIC);
	
	return skb;
}

void rtl_nlmsg_set_skb(struct sk_buff *skb, const RTL_ST_NLMSG* p_nlmsg, u32 pid, u32 seq)
{
	struct nlmsghdr *nlhdr;
	int len;

	RTL_ST_NLMSG* tmp_nlmsg;
	
	len = NLMSG_SPACE(RTL_NLMSG_HEAD_SZ + p_nlmsg->length);

	nlhdr = __nlmsg_put(skb, pid, seq, 0, (len - sizeof(struct nlmsghdr)), 0);

	tmp_nlmsg = (RTL_ST_NLMSG *)(NLMSG_DATA(nlhdr));

	memcpy(tmp_nlmsg, p_nlmsg, RTL_NLMSG_HEAD_SZ + p_nlmsg->length);

	return;
}

int rtl_nlmsg_broadcast(struct sock *ssk, struct sk_buff *skb, RTL_NL_GRPS dest_grp, gfp_t allocation)
{
	//august : the param 'dest_group' is uesd strangly, but there it is.
	//august : I 've checked linux version from 2.6.30 to 3.1.0, the usage and codes do not change! 
	//august : this will be sended to linux-dev-mailist.
	return  netlink_broadcast(ssk, skb, 0, (1 << (dest_grp - 1)), GFP_KERNEL);
}

/***@@@@@@@@@@@%%%%%%%%%#########
	
	SERIOUS NOTE: 	This func is called in the bottom half func.

@@@@@@@@@@@%%%%%%%%%#########***/

int rtl_nl_send_lkchg_msg(const unsigned int old_ptmks, const unsigned int new_ptmks)
{
	struct sk_buff  *skb;

	RTL_NLMSG_LINKCHG link;

	link.rtl_magic 	= 	RTL_NL_SRC_KENERL | RTL_NL_MAGIC_NUM;

	link.type		=	RTL_NL_TYPE_LINKCHANGE;
	link.length		=	EXTSIZE_RTL_NLMSG_LINKCHG;

	link.extended.new_ptmks = new_ptmks;
	link.extended.old_ptmks = old_ptmks;

	skb = rtl_nlmsg_alloc_skb((RTL_ST_NLMSG*)&link);
	if(!skb)
	{
		printk("alloc skb failed!\n");
		return -55;
	}

	rtl_nlmsg_set_skb(skb, (RTL_ST_NLMSG*)&link, 0, 0);

	return rtl_nlmsg_broadcast(global_rtlmsg_sock, skb, RTL_BASIC_GRP, GFP_KERNEL);

}

#endif




