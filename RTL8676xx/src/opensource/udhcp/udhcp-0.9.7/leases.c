/* 
 * leases.c -- tools to manage DHCP leases 
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 */

#include <time.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "debug.h"
#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "leases.h"
#include "arpping.h"

#include "static_leases.h"

#define LEASE_DELTA_FILE "/var/udhcpd.delta"
#define LEASE_ADD		1
#define LEASE_DEL 	2

unsigned char blank_chaddr[] = {[0 ... 15] = 0};

/* clear every lease out that chaddr OR yiaddr matches and is nonzero */
void clear_lease(u_int8_t *chaddr, u_int32_t yiaddr, int ifid)
{
	unsigned int i, j;
	
	for (j = 0; j < 16 && !chaddr[j]; j++);
	
	for (i = 0; i < server_config[ifid].max_leases; i++)
		if ((j != 16 && !memcmp(server_config[ifid].leases[i].chaddr, chaddr, 16)) ||
		    (yiaddr && server_config[ifid].leases[i].yiaddr == yiaddr)) {
			memset(&(server_config[ifid].leases[i]), 0, sizeof(struct dhcpOfferedAddr));
		}
}

void write_to_delta(u_int8_t *chaddr, u_int32_t yiaddr, u_int8_t *vendorid, u_int8_t *hname,unsigned long leasetime,u_int8_t action)
{
  FILE  *fp;
  char line[80];
  int count = 0 ;
  char *ip;
  //unsigned long leasetime = 86400;
	struct in_addr addr;

	if (!(fp = fopen(LEASE_DELTA_FILE, "w"))) {
		LOG(LOG_ERR, "Unable to open %s for writing", LEASE_DELTA_FILE);
		return;
	}

	memset(line , 0x00 , sizeof(line) );
  addr.s_addr = yiaddr;
  ip = inet_ntoa(addr);

  if(action == LEASE_ADD)
  	count = snprintf(line,sizeof(line),"%s %02x:%02x:%02x:%02x:%02x:%02x %s %u %s %s",
              "ADD",chaddr[0],chaddr[1],chaddr[2],chaddr[3],chaddr[4],chaddr[5],
              ip,leasetime,vendorid,hname);
  else
  	count = snprintf(line,sizeof(line),"%s %02x:%02x:%02x:%02x:%02x:%02x %s %u %s %s",
              "DEL",chaddr[0],chaddr[1],chaddr[2],chaddr[3],chaddr[4],chaddr[5],
              ip,leasetime,vendorid,hname);

	LOG(LOG_INFO, "%s",line);

  fwrite(line, sizeof(char), count, fp);
  fclose(fp);
  return 0;
}

static int zerohwaddr( u_int8_t *chaddr )
{
	int i;
  for(i=0;i<6;i++)
	{
		if( chaddr[i] != 0x00)
			return 0;
	}
	return 1;
}
/* add a lease into the table, clearing out any old ones */
struct dhcpOfferedAddr *add_lease(u_int8_t *chaddr, u_int32_t yiaddr, unsigned long lease, int ifid, u_int8_t *vendorid, u_int8_t *hname)
{
	struct dhcpOfferedAddr *oldest;
	
	/* clean out any old ones */
  if( !zerohwaddr(chaddr ))
		clear_lease(chaddr, yiaddr, ifid);
		
	oldest = oldest_expired_lease(ifid);
	
	if (oldest) {
		memset(oldest, 0, sizeof(struct dhcpOfferedAddr));
		memcpy(oldest->chaddr, chaddr, 16);
		strcpy(oldest->hostname, hname);
		oldest->yiaddr = yiaddr;
    if( lease != server_config[ifid].inflease_time )
			oldest->expires = time(0) + lease;
		else
			oldest->expires = server_config[ifid].inflease_time;
		strncpy(oldest->vendorid, vendorid, MAX_VENDOR_ID_LEN - 1);
    write_to_delta(chaddr,yiaddr,vendorid,hname,lease,LEASE_ADD);
	}
	return oldest;
}


/* true if a lease has expired */
int lease_expired(struct dhcpOfferedAddr *lease, int ifid)
{
	if( lease->expires != server_config[ifid].inflease_time)
		return (lease->expires < (unsigned long) time(0));
	else
		return 0;
}	


/* Find the oldest expired lease, NULL if there are no expired leases */
struct dhcpOfferedAddr *oldest_expired_lease(int ifid)
{
	struct dhcpOfferedAddr *oldest = NULL;
	unsigned long oldest_lease = time(0);
	unsigned int i;

	
	for (i = 0; i < server_config[ifid].max_leases; i++)
	{
		if( server_config[ifid].leases[i].expires == server_config[ifid].inflease_time)
			continue;
		if (oldest_lease > server_config[ifid].leases[i].expires)
		{
			oldest_lease = server_config[ifid].leases[i].expires;
			oldest = &(server_config[ifid].leases[i]);
		}
	}
	return oldest;
		
}


/* Find the first lease that matches chaddr, NULL if no match */
struct dhcpOfferedAddr *find_lease_by_chaddr(u_int8_t *chaddr, int ifid)
{
	unsigned int i;

	for (i = 0; i < server_config[ifid].max_leases; i++)
		if (!memcmp(server_config[ifid].leases[i].chaddr, chaddr, 16)) return &(server_config[ifid].leases[i]);
	
	return NULL;
}


/* Find the first lease that matches yiaddr, NULL is no match */
struct dhcpOfferedAddr *find_lease_by_yiaddr(u_int32_t yiaddr, int ifid)
{
	unsigned int i;

	for (i = 0; i < server_config[ifid].max_leases; i++)
		if (server_config[ifid].leases[i].yiaddr == yiaddr) return &(server_config[ifid].leases[i]);
	
	return NULL;
}


/* find an assignable address, it check_expired is true, we check all the expired leases as well.
 * Maybe this should try expired leases by age... */
u_int32_t find_address(int check_expired, int ifid) 
{
	u_int32_t addr, ret, addrend;
	struct dhcpOfferedAddr *lease = NULL;		
	int i,index = 0;

	addr = ntohl(server_config[ifid].start); /* addr is in host order here */
	addrend  = ntohl(server_config[ifid].end);

	DHCPD_TRACE("check_expired = 0x%x, ifid = %d", check_expired, ifid);
	if(check_expired & 0xFFFFFFFE) {
		for(i=1;i<=MAX_CLIENT_CLS_COUNT;i++) {
			if(check_expired & (1<<i)) {
				if(server_config[ifid].CateInfo[i-1].enable == 0) {
					DHCPD_TRACE("CateInfo disable");
					return 0;
				}

				addr = ntohl(server_config[ifid].CateInfo[i-1].start);
				addrend = ntohl(server_config[ifid].CateInfo[i-1].end);
				index = i-1;   /* Keep record only. */
				DHCPD_TRACE("index = %d, addr = %x, addrend = %x", index, addr, addrend);
				break;
			}
		}
	}
	else {
		DHCPD_TRACE("index = COMPUTER");
		index = MAX_CLIENT_CLS_COUNT;  /* Keep record only. */
	}

	for(;addr <= addrend ;addr = addr + 1) {
		/* ie, 192.168.55.0 */
		if (!(addr & 0xFF)) continue;

		/* ie, 192.168.55.255 */
		if ((addr & 0xFF) == 0xFF) continue;

	//radar
		/* Only do if it isn't an assigned as a static lease */
		if(!reservedIp(server_config[ifid].static_leases, htonl(addr))) {
			/* lease is not taken */
			ret = htonl(addr);
			if ((!(lease = find_lease_by_yiaddr(ret, ifid)) ||

			     /* or it expired and we are checking for expired leases */
			     ((check_expired & 0x1) && lease_expired(lease,ifid))) &&

			     /* and it isn't on the network */
		    	     !check_ip(ret, ifid)) {
				return ret;
				break;
			}
		}	// if(!reservedIp..) radar
	}
	return 0;
}


/* check is an IP is taken, if it is, add it to the lease table */
int check_ip(u_int32_t addr, int ifid)
{
	struct in_addr temp;
  unsigned char hwaddr[6];

	if (arpping(addr, server_config[ifid].server, server_config[ifid].arp, server_config[ifid].interface,hwaddr) == 0) {
		temp.s_addr = addr;
	 	LOG(LOG_INFO, "%s belongs to someone, reserving it for %ld seconds", 
	 		inet_ntoa(temp), server_config[ifid].conflict_time);
		//add_lease(blank_chaddr, addr, server_config[ifid].conflict_time, ifid, DEF_VENDOR_ID, "unknown");
      printf("%x:%x:%x:%x:%x:%x\n",hwaddr[0],hwaddr[1],hwaddr[2],hwaddr[3],hwaddr[4],hwaddr[5]);
        //add_lease(hwaddr, addr, server_config[ifid].conflict_time, ifid, DEF_VENDOR_ID, "unknown");
		return 1;
	} else 
	{
		return 0;
	}
}

