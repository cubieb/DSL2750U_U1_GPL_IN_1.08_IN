/* leases.h */
#ifndef _LEASES_H
#define _LEASES_H

//#define INF_LEASETIME  	604800

struct dhcpOfferedAddr {
	u_int8_t chaddr[16];
	u_int32_t yiaddr;	/* network order */
	u_int32_t expires;	/* host order */
	u_int8_t vendorid[64];
	u_int8_t hostname[50];

	u_int8_t opt125;	/* 客户端获取地址时是否使用了选项125 */
};

extern unsigned char blank_chaddr[];

void clear_lease(u_int8_t *chaddr, u_int32_t yiaddr, int ifid);
struct dhcpOfferedAddr *add_lease(u_int8_t *chaddr, u_int32_t yiaddr, unsigned long lease, int ifid, u_int8_t *vendorid, u_int8_t *hname);
int lease_expired(struct dhcpOfferedAddr *lease,int ifid);
struct dhcpOfferedAddr *oldest_expired_lease(int ifid);
struct dhcpOfferedAddr *find_lease_by_chaddr(u_int8_t *chaddr, int ifid);
struct dhcpOfferedAddr *find_lease_by_yiaddr(u_int32_t yiaddr, int ifid);
u_int32_t find_address(int check_expired, int ifid);
int check_ip(u_int32_t addr, int ifid);


#endif
