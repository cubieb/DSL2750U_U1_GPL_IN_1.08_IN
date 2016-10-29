/* Copyright (C) 2006 Ikanos Communications
 * See IKANOS_PROP_LICENSE.txt for license information. */

/* Buffer size used in SRM */
# define SRM_BUFFER_SIZE (2500)

/*
Note: the following structure is also declared in 
fusiv_src/kernel/driver/pppoer/ppprelaylib.h file, if any modification done here
please do the same in that structure also..
Redundancy because to make pppoerelay kernel code independent of user mode files...
*/

typedef struct srmDataBuff_t
{
    int  iLength;
    unsigned char aucBuff[5000];
}srmDataBuff_t;


# define CONFIG_IFCONFIG            1
# define CONFIG_ROUTE               2
# define GET_BRIDGE_SAVE_INFO      34
# define SET_BRIDGE_RESTORE_INFO   34
# define UDHCPD_ID                 50
 
# define CLI_IFCONFIG	 20
# define CLI_ROUTE 	 21
# define CLI_EMAC 	 22
# define CLI_BRIDGE 	 23
# define CLI_ZEBRA 	 24
# define CLI_RIPD 	 25
# define CLI_DNS	 26
# define CLI_DNSMASQ 	 27
# define CLI_UDHCPD 	 28
# define CLI_SNMPD 	 29
# define CLI_DHCPC 	 30
# define CLI_IGMP 	 31
# define CLI_USER 	 32
# define CLI_VLAN 	 33
# define CLI_WLAN 	 36
# define CLI_DHCPRELAY   35
# define CLI_DDNS        37
# define CLI_PPPOE_RELAY 14
# define CLI_SNTP	 38
# define CLI_SETLLID     39
# define CLI_LINKPARAMS  40
# define CLI_PPP         41
# define CLI_IPQOS       42
# define CLI_IPTABLES    43
# define CLI_BR2684	 44
# define CLI_IPOA	 45
# define CLI_ALG	 46
# define CLI_ATHEROS 	 47
# define CLI_ATHEROS_SEC 48
#define  CLI_ATTACK  49
# define CLI_RALINK 50
