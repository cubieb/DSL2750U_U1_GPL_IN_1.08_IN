/*
 * (C) Copyright 2001-2004
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#include <common.h>
#include <command.h>
#include <net.h>
#include <miiphy.h>
#include <tbs_common.h>

#if (CONFIG_COMMANDS & CFG_CMD_NET) 

#if defined(CONFIG_SOLOSW)
extern int solosw_initialize(void);
#elif defined(CONFIG_ATHEROS7130)
extern int 	ar7100_initialize(void);
#elif defined(CONFIG_AR2317)
extern int 	ar2317_initialize(void);
#elif defined(CONFIG_AMAZON_SE)
extern int 	amazon_se_switch_initialize(void);
#elif defined(CONFIG_VX160)
extern int 	voxEmac_init(void);
#elif defined(CONFIG_VX180)
extern int 	voxEmac_init(void);
#elif defined(CONFIG_RT3052)
extern int 	rt2880_eth_initialize(void);
#elif defined(CONFIG_C1000)
extern int comcerto_gemac_initialize(int index, char *devname);
#elif defined(CONFIG_RTL8672) || defined(CONFIG_RTL8672VE) || defined(CONFIG_RTL8196C) || defined(CONFIG_RTL8676)
extern int rtl_eth_init(void);
#elif defined(CONFIG_VRX288)
extern int board_eth_init(void);
#endif 

static struct eth_device *eth_devices, *eth_current;

struct eth_device *eth_get_dev(void)
{
	return eth_current;
}

char *eth_get_name (void)
{
	return (eth_current ? eth_current->name : "unknown");
}

struct eth_device *eth_get_dev_by_name(char *devname)
{
	struct eth_device *dev, *target_dev;

	if (!eth_devices)
		return NULL;

	dev = eth_devices;
	target_dev = NULL;
	do {
		if (strcmp(devname, dev->name) == 0) {
			target_dev = dev;
			break;
		}
		dev = dev->next;
	} while (dev != eth_devices);

	return target_dev;
}

int eth_get_dev_index (void)
{
	struct eth_device *dev;
	int num = 0;

	if (!eth_devices) {
		return (-1);
	}

	for (dev = eth_devices; dev; dev = dev->next) {
		if (dev == eth_current)
			break;
		++num;
	}

	if (dev) {
		return (num);
	}

	return (0);
}

int eth_register(struct eth_device* dev)
{
	if(!eth_devices)
		{
		eth_current = eth_devices = dev;
		}
	else
		{
		puts("Can't register! One ethernet device has registered.");
		return -1;
		}
	dev->state = ETH_STATE_INIT;
	dev->next  = eth_devices;
	return 0;
}

int eth_initialize(void)
{
	unsigned char *ip;
	int eth_number = 0;

	eth_devices = NULL;
	eth_current = NULL;

	//printf("init ehternet...\n");
#if defined(CONFIG_SOLOSW)
	solosw_initialize();
#elif defined(CONFIG_ATHEROS7130)
	ar7100_initialize();
#elif defined(CONFIG_AR2317)
	ar2317_initialize();
#elif defined(CONFIG_AMAZON_SE)
	amazon_se_switch_initialize();
#elif defined(CONFIG_VX160)
	voxEmac_init();
#elif defined(CONFIG_VX180)
	voxEmac_init();
#elif defined(CONFIG_RT3052)
	rt2880_eth_initialize();
#elif defined(CONFIG_C1000)
	comcerto_gemac_initialize(0, "comcerto_gemac0");
#elif defined(CONFIG_RTL8672) || defined(CONFIG_RTL8672VE) || defined(CONFIG_RTL8196C) || defined(CONFIG_RTL8676)
	rtl_eth_init();
#elif defined(CONFIG_VRX288)
    board_eth_init();
#endif 

	if (!eth_devices)
		{
		puts("No ethernet found.\n");
		} 
	else
		{
		struct eth_device *dev = eth_devices;
		memcpy(dev->enetaddr, (void *)&(gd->bi_enetaddr[0]), 6);
		ip = (unsigned char *)&(gd->bi_ip_addr);
		printf ("IP: %d.%d.%d.%d\t" , ip[0] , ip[1] , ip[2] , ip[3] );		
		printf ("MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",dev->enetaddr[0],dev->enetaddr[1],dev->enetaddr[2],dev->enetaddr[3],dev->enetaddr[4],dev->enetaddr[5]);
		eth_number = 1;
		}

	return eth_number;
}

void eth_set_enetaddr(int num, char *addr) {
	struct eth_device *dev;
	unsigned char enetaddr[6];
	char *end;
	int i;

	debug ("eth_set_enetaddr(num=%d, addr=%s)\n", num, addr);

	if (!eth_devices)
		return;

	for (i=0; i<6; i++) {
		enetaddr[i] = addr ? simple_strtoul(addr, &end, 16) : 0;
		if (addr)
			addr = (*end) ? end+1 : end;
	}

	dev = eth_devices;
	while(num-- > 0) {
		dev = dev->next;

		if (dev == eth_devices)
			return;
	}

	debug ( "Setting new HW address on %s\n"
		"New Address is             %02X:%02X:%02X:%02X:%02X:%02X\n",
		dev->name,
		enetaddr[0], enetaddr[1],
		enetaddr[2], enetaddr[3],
		enetaddr[4], enetaddr[5]);

	memcpy(dev->enetaddr, enetaddr, 6);
}

int eth_init(void)
{
	struct eth_device* old;

	if(!eth_current)
		return 0;

	old = eth_current;
	do {
		debug("Trying %s\n", eth_current->name);
		if(eth_current->init(eth_current) >= 0)
			{
			eth_current->state = ETH_STATE_ACTIVE;
			return 1;
			}
		debug("FAIL\n");
		}
	while(old != eth_current);

	return 0;
}

void eth_halt(void)
{
	if(eth_current)
		{
		eth_current->halt(eth_current);
		eth_current->state = ETH_STATE_PASSIVE;
		}
}

int eth_send(volatile void *packet, int length)
{
	if(!eth_current)
		return -1;

	return eth_current->send(eth_current, packet, length);
}

int eth_rx(void)
{
	if(!eth_current)
		return -1;

	return eth_current->recv(eth_current);
}

int eth_get_link_state(void)
{
	int ret_val = 0;
	
	if((eth_current) && (eth_current->chklk))
		{
		eth_current->chklk(eth_current);
		if(ETH_STATE_UP == eth_current->state)
			{
			ret_val = 1;
			}
		}
	
	return ret_val;
}

#endif
