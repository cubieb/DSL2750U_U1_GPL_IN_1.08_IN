#ifndef RTL8676_API_H
#define RTL8676_API_H

#ifdef CONFIG_RTL_HARDWARE_MULTICAST
/********************************************************         
	RTL8676 API  - Multicast hardware acceleration
***********************************************************/
#include <net/rtl/rtl865x_multicast.h>
#include <net/rtl/rtl_nic.h>

int rtl8676_set_Mutlicast_acc(unsigned int group_ip, unsigned int source_ip, unsigned short source_vid, unsigned short source_port,unsigned int gourp_member);
int rtl8676_reset_Mutlicast_acc(unsigned int group_ip,unsigned int gourp_member);
int rtl8676_cancel_Mutlicast_acc(unsigned int group_ip);
#endif

#ifdef CONFIG_RTL_MULTI_ETH_WAN
/********************************************************         
	RTL8676 API  - Multiwan
***********************************************************/

int rtl8676_register_Multiwan_dev(char *ifname, int proto, int vid, int napt);
int rtl8676_update_portmapping_Multiwan_dev(unsigned int member, int vid, int proto);
int rtl8676_unregister_Multiwan_dev(char *ifname, int vid, int proto);
int rtl8676_set_Multiwan_NetifMacAddr(char *ifname, char *addr);
#endif

#if defined(CONFIG_RTL_LAYERED_DRIVER) && defined(CONFIG_RTL_LAYERED_DRIVER_L2)
/********************************************************         
	RTL8676 API  - fdb
***********************************************************/
#include <net/rtl/rtl865x_fdb_api.h>

void rtl8676_fdb_delete(unsigned short l2Type,  const unsigned char *addr);
void rtl8676_fdb_new(unsigned char is_static, const unsigned char *addr);
void rtl8676_update_L2_check(void *orig_br_port, void *updated_br_port, const char *name, const unsigned char *addr);
#endif

#endif

