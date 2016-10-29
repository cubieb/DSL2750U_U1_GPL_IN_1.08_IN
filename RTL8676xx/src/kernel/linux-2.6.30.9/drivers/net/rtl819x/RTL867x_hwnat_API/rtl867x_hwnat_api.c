#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <net/rtl/rtl_types.h>
#include <net/rtl/rtl_queue.h>
#include "rtl867x_hwnat_api.h"
#include "../AsicDriver/rtl865x_asicCom.h"
#include "../AsicDriver/rtl865x_asicL2.h"
#include "../l2Driver/rtl865x_fdb.h"
#ifdef CONFIG_RTL_HARDWARE_MULTICAST
/********************************************************         
	RTL8676 API  - Multicast hardware acceleration
***********************************************************/

/* 
 *  Function Name:		rtl8676_set_Mutlicast_acc
 *  Description:			Let ipv4 multicast stream accelerate in 8676 hwnat switch
 *
 *  Input:				group_ip : 			Ipv4 multicast group ip
 *					source_ip:			Ipv4 multicast source ip
 *					source_vid:			the vlan id of multicast packet (if none, pass 0)
 *					source_port:			the source port (in 8676 switch view, start from 0) of multicast packet 
 *					gourp_member:		the members which the multicast packet forward to (in 8676 switch view , bit0 : port0 , bit1 : port1 ....)
 *
 * Return:				SUCCESS:				set 8676hwnat successfully 
 *       				FAILED				set 8676hwnat failed
 */

int rtl8676_set_Mutlicast_acc(unsigned int group_ip, unsigned int source_ip, unsigned short source_vid, unsigned short source_port,unsigned int gourp_member)
{
	return rtl865x_addMulticastEntry(group_ip,source_ip,source_vid,source_port,gourp_member);
}

/* 
 *  Function Name:		rtl8676_reset_Mutlicast_acc
 *  Description:			change accelerated multicast stream's group member in 8676 hwnat switch
 *
 *  Input:				group_ip : 			Ipv4 multicast group ip
 *					gourp_member:		the members which the multicast packet forward to (in 8676 switch view , bit0 : port0 , bit1 : port1 ....)
 *
 * Return:				SUCCESS:				set 8676hwnat successfully 
 *       				FAILED				set 8676hwnat failed 
 */

int rtl8676_reset_Mutlicast_acc(unsigned int group_ip,unsigned int gourp_member)
{
	return rtl865x_resetMulticastEntry(group_ip,gourp_member);
}

/* 
 *  Function Name:		rtl8676_cancel_Mutlicast_acc
 *  Description:			cancle accelerated multicast stream in 8676 hwnat switch
 *
 *  Input:				group_ip : 			Ipv4 multicast group ip
 * 
 * Return:				SUCCESS:				set 8676hwnat successfully 
 *       				FAILED				set 8676hwnat failed 
 */

int rtl8676_cancel_Mutlicast_acc(unsigned int group_ip)
{
	return rtl865x_delMulticastEntry(group_ip);
}
#endif

#ifdef CONFIG_RTL_MULTI_ETH_WAN
/********************************************************         
	RTL8676 API  - Multiwan
***********************************************************/

/* 
 *	Function Name:	rtl8676_register_Multiwan_dev
 *	Description:	set netif to vlanconfig
 *
 *	Input:		ifname		:	network interface name
 *				proto		:	0: pppoe, 1: ipoe, 2: bridge
 *				vid			:	vlan id
 *				napt			:	0: not support napt, 1: support napt
 * 
 *	Return:		SUCCESS	:	set 8676hwnat successfully 
 * 				FAILED		:	set 8676hwnat failed 
 */

int rtl8676_register_Multiwan_dev(char *ifname, int proto, int vid, int napt)
{
	return rtl_set_wanport_vlanconfig(ifname, proto, vid, napt);
}

/* 
 *	Function Name:	rtl8676_update_portmapping_Multiwan_dev
 *	Description:	set group member of netif to vlanconfig
 *
 *	Input:		member 		:	network interface group member
 *				vid			:	vlan id
 *				proto		:	0: pppoe, 1: ipoe, 2: bridge
 *
 *	Return:		SUCCESS	:	set 8676hwnat successfully 
 *				FAILED		:	set 8676hwnat failed 
 */

int rtl8676_update_portmapping_Multiwan_dev(unsigned int member, int vid, int proto)
{
	return rtl_set_wanport_portmapping(member, vid, proto);
}

/* 
 *	Function Name:	rtl8676_unregister_Multiwan_dev
 *	Description:	reset vlanconfig & netif/acl in 8676 hwnat switch
 *
 *	Input:		ifname		:	network interface name
 *				vid			:	vlan id
 *				proto		:	0: pppoe, 1: ipoe, 2: bridge
 * 
 *	Return:		SUCCESS	:	set 8676hwnat successfully 
 *				FAILED		:	set 8676hwnat failed 
 */

int rtl8676_unregister_Multiwan_dev(char *ifname, int vid, int proto)
{
	int retval = FAILED;

	if((retval = rtl_reset_wanport_vlanconfig(vid, proto))==FAILED)
		return retval;
	
	return rtl865x_unregisterDev(ifname);
}

/* 
 *	Function Name:	rtl8676_setNetifMacAddr
 *	Description:	update netif mac address in 8676 hwnat switch
 *
 *	Input:		ifname		:	network interface name
 *				addr			:	mac address
 *
 *	Return:		SUCCESS	:	set 8676hwnat successfully 
 *				FAILED		:	set 8676hwnat failed  
 */

int rtl8676_set_Multiwan_NetifMacAddr(char *ifname, char *addr)
{
	return rtl865x_setNetifMacAddr(ifname, addr);
}
#endif

#if defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L2)
/* 
 *	Function Name:	rtl8676_fdb_delete
 *	Description:	delete hwnat fdb entry
 *
 *	Input:		l2Type		:	entry type in L2 table
 *				addr			:	mac address for the fdb entry
 *
 *	Return:		none  
 */
void rtl8676_fdb_delete(unsigned short l2Type,  const unsigned char *addr)
{
#ifdef CONFIG_RTL865X_LANPORT_RESTRICTION
	rtl865x_delAuthLanFDBEntry(RTL865x_L2_TYPEII, addr);
#else
	rtl865x_delLanFDBEntry(RTL865x_L2_TYPEII, addr);
#endif
}

/* 
 *	Function Name:	rtl8676_fdb_new
 *	Description:	add hwnat fdb entry
 *
 *	Input:		is_static		:	check if the fdb entry in kernel is static
 *				addr			:	mac address for the fdb entry
 *
 *	Return:		none  
 */
void rtl8676_fdb_new(unsigned char is_static, const unsigned char *addr)
{
	if (is_static)
		return;

#ifdef CONFIG_RTL865X_LANPORT_RESTRICTION
	rtl865x_new_AuthFDB(addr);
#else
	rtl865x_addFDBEntry(addr);
#endif
}

/* 
 *	Function Name:	rtl8676_update_L2_check
 *	Description:	delete hwnat fdb entry when the source bridge port is changed for wlan interface
 *
 *	Input:		orig_br_port		:	original bridge port
 *				update_entry		:	updated bridge port
 *				name			:	net device name of new source
 *				addr				:	mac address for the fdb entry
 *
 *	Return:		none  
 */
void rtl8676_update_L2_check(void *orig_br_port, void *updated_br_port, const char *name, const unsigned char *addr)
{
	if (orig_br_port != updated_br_port)
		update_hw_l2table(name, addr);
}

#endif
