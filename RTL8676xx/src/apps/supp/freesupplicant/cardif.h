#ifndef _CARDIF_H
#define _CARDIF_H
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <net/if.h>
#include <features.h>    /* for the glibc version number */
#if __GLIBC__ >= 2 && __GLIBC_MINOR >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>     /* the L2 protocols */
 #else
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>   /* The L2 protocols */
#endif

#ifndef ETH_P_EAPOL
#define ETH_P_EAPOL 0x888e
#endif

#define MAX_MTU_LEN 1520
struct cardif_data{
	char ifname[16];
	int ifindex;
	struct sockaddr_ll sll;
	int sock;
	char *recv_frame;
	int recv_size;
	char *send_frame;
	int send_size;
	int min_mac_frame_size;
	char src_mac[6];
	char dst_mac[6];
	
};
extern unsigned int if_nametoindex(const char *);
extern struct cardif_data cardif;
#endif
