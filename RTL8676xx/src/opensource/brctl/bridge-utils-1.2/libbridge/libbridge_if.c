/*
 * Copyright (C) 2000 Lennert Buytenhek
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>

#include "libbridge.h"
#include "libbridge_private.h"


int br_add_bridge(const char *brname)
{
	int ret;

#ifdef SIOCBRADDBR
	ret = ioctl(br_socket_fd, SIOCBRADDBR, brname);
	if (ret < 0)
#endif
	{
		char _br[IFNAMSIZ];
		unsigned long arg[3] 
			= { BRCTL_ADD_BRIDGE, (unsigned long) _br };

		strncpy(_br, brname, IFNAMSIZ);
		ret = ioctl(br_socket_fd, SIOCSIFBR, arg);
	} 

	return ret < 0 ? errno : 0;
}

int br_del_bridge(const char *brname)
{
	int ret;

#ifdef SIOCBRDELBR	
	ret = ioctl(br_socket_fd, SIOCBRDELBR, brname);
	if (ret < 0)
#endif
	{
		char _br[IFNAMSIZ];
		unsigned long arg[3] 
			= { BRCTL_DEL_BRIDGE, (unsigned long) _br };

		strncpy(_br, brname, IFNAMSIZ);
		ret = ioctl(br_socket_fd, SIOCSIFBR, arg);
	} 
	return  ret < 0 ? errno : 0;
}

int br_add_interface(const char *bridge, const char *dev)
{
	struct ifreq ifr;
	int err;
	int ifindex = if_nametoindex(dev);

	if (ifindex == 0) 
		return ENODEV;
	
	strncpy(ifr.ifr_name, bridge, IFNAMSIZ);
#ifdef SIOCBRADDIF
	ifr.ifr_ifindex = ifindex;
	err = ioctl(br_socket_fd, SIOCBRADDIF, &ifr);
	if (err < 0)
#endif
	{
		unsigned long args[4] = { BRCTL_ADD_IF, ifindex, 0, 0 };
					  
		ifr.ifr_data = (char *) args;
		err = ioctl(br_socket_fd, SIOCDEVPRIVATE, &ifr);
	}

	return err < 0 ? errno : 0;
}

int br_del_interface(const char *bridge, const char *dev)
{
	struct ifreq ifr;
	int err;
	int ifindex = if_nametoindex(dev);

	if (ifindex == 0) 
		return ENODEV;
	
	strncpy(ifr.ifr_name, bridge, IFNAMSIZ);
#ifdef SIOCBRDELIF
	ifr.ifr_ifindex = ifindex;
	err = ioctl(br_socket_fd, SIOCBRDELIF, &ifr);
	if (err < 0)
#endif		
	{
		unsigned long args[4] = { BRCTL_DEL_IF, ifindex, 0, 0 };
					  
		ifr.ifr_data = (char *) args;
		err = ioctl(br_socket_fd, SIOCDEVPRIVATE, &ifr);
	}

	return err < 0 ? errno : 0;
}

#ifdef CONFIG_VLAN_MODE
static int show_vlan_list(VLAN_MODE_T *vlan)
{
	int i,j;
	switch(vlan->mode)
	{
		case VLAN_TRANSPARENT:
		{
			printf("\tvlan mode: transparent\n");
			return 0;
		}
		case VLAN_TAG:
		{
			printf("\tvlan mode: tag\n");
			printf("\tdefault vid: %d\n",vlan->pvlan);
			return 0;
		}
		case VLAN_TRUNK:
		{
			printf("\tvlan mode: trunk\n");
			printf("\tdefault vid: %d\n",vlan->pvlan);
			printf("\tvid group: %d\n",vlan->num);
			printf("\tvid: ");
			for(i=0;i<vlan->num;i++)
				printf("%d ",vlan->u.trunk[i]);
			printf("\n");
			return 0;
		}
		case VLAN_TRANSLATE:
		{
			printf("\tvlan mode: translate \n");
			printf("\tdefault vid: %d\n",vlan->pvlan);
			printf("\tvid group: %d\n",vlan->num);
			printf("\tvid: ");
			
			for(i = 0;i < vlan->num;i++)
			{
				printf("%d %d\t",vlan->u.translate[i].old_vlan,vlan->u.translate[i].new_vlan);
			}
			
			printf("\n");
			return 0;
		}
		case VLAN_AGGR:
		{
			printf("\tvlan mode: aggr\n");
			printf("\tdefault vid: %d\n",vlan->pvlan);
			printf("\tvid group: %d\n",vlan->num);
			for(i=0;i<vlan->num;i++)
			{
				printf("\t%d:",i+1);
				printf("vlan_l: %d ",vlan->u.aggr[i].vlan_1);
				printf("vlan_num: %d",vlan->u.aggr[i].num);				
				printf(" vlan_n: ");
				for(j=0;j<vlan->u.aggr[i].num;j++)
					printf(" %d ",vlan->u.aggr[i].vlan_n[j]);
				printf("\n");
			}
			printf("\n");
			return 0;
		}
		case VLAN_HYBIRD:
		{
			printf("\tvlan mode: hybird\n");
			printf("\tdefault vid: %d\n",vlan->pvlan);
			printf("\ttrans vid group: %d\n",vlan->u.hybird.trans_num);
			printf("\tvid: ");
			for(i=0;i<vlan->u.hybird.trans_num;i++)
				printf("%d ",vlan->u.hybird.trans_vlan[i]);
			printf("\n");
			printf("\tuntag vid group: %d\n",vlan->u.hybird.untag_num);
			printf("\tvid: ");
			for(i=0;i<vlan->u.hybird.untag_num;i++)
				printf("%d ",vlan->u.hybird.untag_vlan[i]);
			printf("\n");
			return 0;
		}
		default:
			return -1;
	}
}
int br_vlan_set(const char *port,VLAN_MODE_T *vlan)
{
	int err=0;
	struct ifreq ifr;
	int ifindex = if_nametoindex(port);
	if (ifindex == 0) 
	{
		printf("interface not exist!\n");
		return ENODEV;
	}
	unsigned long args[4] = { BRCTL_VLAN_SET, ifindex, (unsigned long)vlan, 0 };
	strncpy(ifr.ifr_name, "br0", IFNAMSIZ);
	ifr.ifr_data = (char *) &args;
	err= ioctl(br_socket_fd,SIOCDEVPRIVATE,&ifr);
	if(err)
	{
//		printf("something wrong,vlan set fail!\n");
		return -1;
	}
	else
		return err;
}

int br_vlan_show(const char *port)
{
	int err;
	struct ifreq ifr;
	VLAN_MODE_T vlan;
	int ifindex = if_nametoindex(port);
	if (ifindex == 0) 
	{
		printf("interface not exist!\n");
		return ENODEV;
	}
	unsigned long args[4] = { BRCTL_VLAN_SHOW, ifindex, &vlan, 0 };
	strncpy(ifr.ifr_name, "br0", IFNAMSIZ);
	ifr.ifr_data = (char *) &args;
	err=ioctl(br_socket_fd,SIOCDEVPRIVATE,&ifr);
	if(err)
	{
		printf("no vlan set or something wrong!\n");
		return -1;
	}
	err=show_vlan_list(&vlan);
	return err < 0 ? errno : 0;
}
#endif

