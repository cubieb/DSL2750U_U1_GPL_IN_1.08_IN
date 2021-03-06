/*
 *	Device handling code
 *	Linux ethernet bridge
 *
 *	Authors:
 *	Lennert Buytenhek		<buytenh@gnu.org>
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/ethtool.h>

#include <asm/uaccess.h>
#include "br_private.h"

#if defined (CONFIG_RTL_IGMP_SNOOPING)
/*2008-01-15,for porting igmp snooping to linux kernel 2.6*/
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/igmp.h>
#include <net/checksum.h>
#include <net/rtl/rtl865x_igmpsnooping_glue.h>
#include <net/rtl/rtl865x_igmpsnooping.h>
extern int igmpsnoopenabled;
#if defined (CONFIG_RTL_MLD_SNOOPING)
#include <linux/ipv6.h>
#include <linux/in6.h>
extern int mldSnoopEnabled;
#endif
#if defined (CONFIG_RTL_HARDWARE_MULTICAST)
#include <net/rtl/rtl865x_multicast.h>
#include <net/rtl/rtl865x_netif.h>
#include <net/rtl/rtl_nic.h>
#endif
extern unsigned int br0SwFwdPortMask;
extern unsigned int brIgmpModuleIndex;
extern unsigned int nicIgmpModuleIndex;

mac_addr upnp_addr = {{0x01, 0x00, 0x5e, 0x7f, 0xff, 0xfa}};
mac_addr sys1_addr = {{0x01, 0x00, 0x5e, 0x00, 0x00, 0x01}};
mac_addr sys2_addr = {{0x01, 0x00, 0x5e, 0x00, 0x00, 0x02}};
mac_addr ospf1_addr = {{0x01, 0x00, 0x5e, 0x00, 0x00, 0x05}};
mac_addr ospf2_addr = {{0x01, 0x00, 0x5e, 0x00, 0x00, 0x06}};
mac_addr ripv2_addr = {{0x01, 0x00, 0x5e, 0x00, 0x00, 0x09}};
mac_addr sys_addr = {{0xff, 0xff, 0xff, 0xff, 0xff, 0xff}};

/* Define ipv6 multicast mac address to let them pass through control filtering.
 * All ipv6 multicast mac addresses start with 0x33 0x33. So control_filter
 * only need to compare the first 2 bytes of the address.
 */
mac_addr ipv6_mc_addr = {{0x33, 0x33, 0x00, 0x00, 0x00, 0x00}}; /* only the left two bytes are significant */

static int control_filter(unsigned char *dest)
{
    if ((!memcmp(dest, &upnp_addr, ETH_ALEN)) ||
            (!memcmp(dest, &sys1_addr, ETH_ALEN)) ||
            (!memcmp(dest, &sys2_addr, ETH_ALEN)) ||
            (!memcmp(dest, &ospf1_addr, ETH_ALEN)) ||
            (!memcmp(dest, &ospf2_addr, ETH_ALEN)) ||
            (!memcmp(dest, &sys_addr, ETH_ALEN)) ||
            (!memcmp(dest, &ripv2_addr, ETH_ALEN)) ||
            (!memcmp(dest, &ipv6_mc_addr, 2)))
        return 0;
    else
        return 1;
}
#endif

#if defined (CONFIG_RTL_HARDWARE_MULTICAST)
int rtl865x_ipMulticastHardwareAccelerate(struct net_bridge *br, unsigned int brFwdPortMask,
												unsigned int srcPort,unsigned int srcVlanId, 
												unsigned int srcIpAddr, unsigned int destIpAddr);
#endif
#if defined (CONFIG_RTL_MLD_SNOOPING)
extern int re865x_getIpv6TransportProtocol(struct ipv6hdr* ipv6h);
#endif
/* net device transmit always called with no BH (preempt_disabled) */
int br_dev_xmit(struct sk_buff *skb, struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);
	const unsigned char *dest = skb->data;
	struct net_bridge_fdb_entry *dst;

#if defined (CONFIG_RTL_IGMP_SNOOPING)	
	struct iphdr *iph=NULL;
	unsigned char proto=0;
	unsigned char reserved=0;
#if defined (CONFIG_RTL_MLD_SNOOPING) 	
	struct ipv6hdr *ipv6h=NULL;
#endif
	struct rtl_multicastDataInfo multicastDataInfo;
	struct rtl_multicastFwdInfo multicastFwdInfo;
	int ret=FAILED;
#if defined (CONFIG_RTL_HARDWARE_MULTICAST)
	unsigned int srcPort=skb->srcPort;
	unsigned int srcVlanId=skb->srcVlanId;
#endif
#endif	
	dev->stats.tx_packets++;
	dev->stats.tx_bytes += skb->len;

	skb_reset_mac_header(skb);
	skb_pull(skb, ETH_HLEN);


	if (dest[0] & 1)
	{
#if defined (CONFIG_RTL_IGMP_SNOOPING)	
		if(igmpsnoopenabled) 
		{	
			if(MULTICAST_MAC(dest))
			{
			
				iph=(struct iphdr *)skb_network_header(skb);
				proto =  iph->protocol;
				#if 0
				if(( iph->daddr&0xFFFFFF00)==0xE0000000)
				{
				        reserved=1;
				}
				#endif

				if( iph->daddr == 0xEFFFFFFA)
				{
					/*for microsoft upnp*/
					reserved=1;
				}
				
				if(((proto ==IPPROTO_UDP) ||(proto ==IPPROTO_TCP))  && (reserved ==0))
				{
					multicastDataInfo.ipVersion=4;
					multicastDataInfo.sourceIp[0]=  (uint32)(iph->saddr);
					multicastDataInfo.groupAddr[0]=  (uint32)(iph->daddr);
					ret= rtl_getMulticastDataFwdInfo(brIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
					if(ret==SUCCESS)
					{
					
						br_multicast_deliver(br, multicastFwdInfo.fwdPortMask, skb, 0);

						#if defined (CONFIG_RTL_HARDWARE_MULTICAST)
						if((srcVlanId!=0) && (srcPort!=0xFFFF))
						{
							#if defined(CONFIG_RTK_VLAN_SUPPORT)
							if(rtk_vlan_support_enable == 0)
							{
								rtl865x_ipMulticastHardwareAccelerate(br, multicastFwdInfo.fwdPortMask,srcPort,srcVlanId, multicastDataInfo.sourceIp[0], multicastDataInfo.groupAddr[0]);
							}
							#else
							rtl865x_ipMulticastHardwareAccelerate(br, multicastFwdInfo.fwdPortMask,srcPort,srcVlanId, multicastDataInfo.sourceIp[0], multicastDataInfo.groupAddr[0]);
							#endif
						}
						#endif		
					}
					else
					{
                        if(control_filter(dest))
                            kfree_skb(skb);
                        else
    						br_flood_deliver(br, skb);
					}
				}
				else
				{
					br_flood_deliver(br, skb);
				}

				
			}
#if defined(CONFIG_RTL_MLD_SNOOPING)	
			else if(mldSnoopEnabled && IPV6_MULTICAST_MAC(dest))
			{
				ipv6h=(struct ipv6hdr *)skb_network_header(skb);
				proto=re865x_getIpv6TransportProtocol(ipv6h);
				if ((proto ==IPPROTO_UDP) ||(proto ==IPPROTO_TCP))
				{
					multicastDataInfo.ipVersion=6;
					memcpy(&multicastDataInfo.sourceIp, &ipv6h->saddr, sizeof(struct in6_addr));
					memcpy(&multicastDataInfo.groupAddr, &ipv6h->daddr, sizeof(struct in6_addr));
					ret= rtl_getMulticastDataFwdInfo(brIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
					if(ret==SUCCESS)
					{
						br_multicast_deliver(br, multicastFwdInfo.fwdPortMask, skb, 0);
					}
					else
					{
						br_flood_deliver(br, skb);
					}
				}
				else
				{
					br_flood_deliver(br, skb);
				}
			}
#endif		
			else
			{
				br_flood_deliver(br, skb);
			}
		
		}
		else
		{ 
			br_flood_deliver(br, skb);
		}	
#else
/*
 * TBS_TAG: by luolei 2008--
 * Desc:Add IGMP Snooping support
 */
#if defined(CONFIG_IGMP_SNOOPING)
		if (!mc_forward(br, skb, (unsigned char *) dest, 0, 0))
#endif
{
		br_flood_deliver(br, skb);
}
#endif
	}
	else if ((dst = __br_fdb_get(br, dest)) != NULL)
		br_deliver(dst->dst, skb);
	else
		br_flood_deliver(br, skb);

	return 0;
}

static int br_dev_open(struct net_device *dev)
{
	struct net_bridge *br = netdev_priv(dev);

	br_features_recompute(br);
	netif_start_queue(dev);
	br_stp_enable_bridge(br);

	return 0;
}

static void br_dev_set_multicast_list(struct net_device *dev)
{
}

static int br_dev_stop(struct net_device *dev)
{
	br_stp_disable_bridge(netdev_priv(dev));

	netif_stop_queue(dev);

	return 0;
}

static int br_change_mtu(struct net_device *dev, int new_mtu)
{
	struct net_bridge *br = netdev_priv(dev);
	if (new_mtu < 68 || new_mtu > br_min_mtu(br))
		return -EINVAL;

	dev->mtu = new_mtu;

#ifdef CONFIG_BRIDGE_NETFILTER
	/* remember the MTU in the rtable for PMTU */
	br->fake_rtable.u.dst.metrics[RTAX_MTU - 1] = new_mtu;
#endif

	return 0;
}

/* Allow setting mac address to any valid ethernet address. */
static int br_set_mac_address(struct net_device *dev, void *p)
{
	struct net_bridge *br = netdev_priv(dev);
	struct sockaddr *addr = p;

	if (!is_valid_ether_addr(addr->sa_data))
		return -EINVAL;

	spin_lock_bh(&br->lock);
	memcpy(dev->dev_addr, addr->sa_data, ETH_ALEN);
	br_stp_change_bridge_id(br, addr->sa_data);
	br->flags |= BR_SET_MAC_ADDR;
	spin_unlock_bh(&br->lock);

	return 0;
}

static void br_getinfo(struct net_device *dev, struct ethtool_drvinfo *info)
{
	strcpy(info->driver, "bridge");
	strcpy(info->version, BR_VERSION);
	strcpy(info->fw_version, "N/A");
	strcpy(info->bus_info, "N/A");
}

static int br_set_sg(struct net_device *dev, u32 data)
{
	struct net_bridge *br = netdev_priv(dev);

	if (data)
		br->feature_mask |= NETIF_F_SG;
	else
		br->feature_mask &= ~NETIF_F_SG;

	br_features_recompute(br);
	return 0;
}

static int br_set_tso(struct net_device *dev, u32 data)
{
	struct net_bridge *br = netdev_priv(dev);

	if (data)
		br->feature_mask |= NETIF_F_TSO;
	else
		br->feature_mask &= ~NETIF_F_TSO;

	br_features_recompute(br);
	return 0;
}

static int br_set_tx_csum(struct net_device *dev, u32 data)
{
	struct net_bridge *br = netdev_priv(dev);

	if (data)
		br->feature_mask |= NETIF_F_NO_CSUM;
	else
		br->feature_mask &= ~NETIF_F_ALL_CSUM;

	br_features_recompute(br);
	return 0;
}

static const struct ethtool_ops br_ethtool_ops = {
	.get_drvinfo    = br_getinfo,
	.get_link	= ethtool_op_get_link,
	.get_tx_csum	= ethtool_op_get_tx_csum,
	.set_tx_csum 	= br_set_tx_csum,
	.get_sg		= ethtool_op_get_sg,
	.set_sg		= br_set_sg,
	.get_tso	= ethtool_op_get_tso,
	.set_tso	= br_set_tso,
	.get_ufo	= ethtool_op_get_ufo,
	.get_flags	= ethtool_op_get_flags,
};

static const struct net_device_ops br_netdev_ops = {
	.ndo_open		 = br_dev_open,
	.ndo_stop		 = br_dev_stop,
	.ndo_start_xmit		 = br_dev_xmit,
	.ndo_set_mac_address	 = br_set_mac_address,
	.ndo_set_multicast_list	 = br_dev_set_multicast_list,
	.ndo_change_mtu		 = br_change_mtu,
	.ndo_do_ioctl		 = br_dev_ioctl,
};

void br_dev_setup(struct net_device *dev)
{
	random_ether_addr(dev->dev_addr);
	ether_setup(dev);

	dev->netdev_ops = &br_netdev_ops;
	dev->destructor = free_netdev;
	SET_ETHTOOL_OPS(dev, &br_ethtool_ops);
	dev->tx_queue_len = 0;
	dev->priv_flags = IFF_EBRIDGE;
#if defined(CONFIG_HTTP_FILE_SERVER_SUPPORT)
	dev->features = NETIF_F_SG | NETIF_F_FRAGLIST | NETIF_F_HIGHDMA |
			NETIF_F_GSO_MASK | NETIF_F_NO_CSUM | NETIF_F_LLTX |
			NETIF_F_NETNS_LOCAL | NETIF_F_GSO|NETIF_F_GRO|NETIF_F_LRO;
#elif defined(CONFIG_RTL_USB_IP_HOST_SPEEDUP)
        dev->features = NETIF_F_SG | NETIF_F_FRAGLIST | NETIF_F_HIGHDMA |
                        NETIF_F_GSO_MASK | NETIF_F_NO_CSUM | NETIF_F_LLTX |
                        NETIF_F_NETNS_LOCAL | NETIF_F_GSO|NETIF_F_GRO;
#else
	dev->features = NETIF_F_SG | NETIF_F_FRAGLIST | NETIF_F_HIGHDMA |
			NETIF_F_GSO_MASK | NETIF_F_NO_CSUM | NETIF_F_LLTX |
			NETIF_F_NETNS_LOCAL | NETIF_F_GSO;
#endif
}

#if defined (CONFIG_RTL_HARDWARE_MULTICAST)

int rtl865x_ipMulticastHardwareAccelerate(struct net_bridge *br, unsigned int brFwdPortMask,
												unsigned int srcPort,unsigned int srcVlanId, 
												unsigned int srcIpAddr, unsigned int destIpAddr)
{
	int ret;
	//int fwdDescCnt;
	//unsigned short port_bitmask=0;

	struct rtl_multicastDataInfo multicastDataInfo;
	struct rtl_multicastFwdInfo  multicastFwdInfo;
	
//	rtl865x_tblDrv_mCast_t * existMulticastEntry;
//	rtl865x_mcast_fwd_descriptor_t  fwdDescriptor;


	#if 0
	printk("%s:%d,srcPort is %d,srcVlanId is %d,srcIpAddr is 0x%x,destIpAddr is 0x%x\n",__FUNCTION__,__LINE__,srcPort,srcVlanId,srcIpAddr,destIpAddr);
	#endif
	if(strcmp(br->dev->name,RTL_PS_BR0_DEV_NAME)!=0)
	{
		return -1;
	}
	if(brFwdPortMask & br0SwFwdPortMask)
	{
		return -1;
	}
	//printk("%s:%d,destIpAddr is 0x%x, srcIpAddr is 0x%x, srcVlanId is %d, srcPort is %d\n",__FUNCTION__,__LINE__,destIpAddr, srcIpAddr, srcVlanId, srcPort);
	#if 0

	existMulticastEntry=rtl865x_findMCastEntry(destIpAddr, srcIpAddr, (unsigned short)srcVlanId, (unsigned short)srcPort);
	if(existMulticastEntry!=NULL)
	{
		/*it's already in cache */
		return 0;

	}   
	if(brFwdPortMask==0)
	{
	    printk("%s %d \n",__FUNCTION__,__LINE__);
		rtl865x_blockMulticastFlow(srcVlanId, srcPort, srcIpAddr, destIpAddr);
		return 0;
	}
	#endif
	
	multicastDataInfo.ipVersion=4;
	multicastDataInfo.sourceIp[0]=  srcIpAddr;
	multicastDataInfo.groupAddr[0]=  destIpAddr;

	#if 0
	/*add hardware multicast entry*/

	memset(&fwdDescriptor, 0, sizeof(rtl865x_mcast_fwd_descriptor_t ));
	strcpy(fwdDescriptor.netifName,"eth*");
	fwdDescriptor.fwdPortMask=0xFFFFFFFF;
	#endif
	
	ret= rtl_getMulticastDataFwdInfo(nicIgmpModuleIndex, &multicastDataInfo, &multicastFwdInfo);
	if(ret != SUCCESS)
	{
		return -1;
	}
	else
	{
		if(multicastFwdInfo.cpuFlag)
		{
			#if 0
			fwdDescriptor.toCpu=1;
			#endif
			return -1;
		}
		//fwdDescriptor.fwdPortMask=multicastFwdInfo.fwdPortMask & (~(1<<srcPort));
	}

	#if 0
	rtl865x_addMulticastEntry(destIpAddr, srcIpAddr, (unsigned short)srcVlanId, (unsigned short)srcPort, 
						&fwdDescriptor, 1, 0, 0, 0);
	#endif
	rtl865x_addMulticastEntry(destIpAddr, srcIpAddr, (unsigned short)srcVlanId, (unsigned short)srcPort, 
						multicastFwdInfo.fwdPortMask& (~(1<<srcPort)));
	
	return 0;
}

#endif

