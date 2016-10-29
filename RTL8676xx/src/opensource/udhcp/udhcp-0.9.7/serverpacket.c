/* serverpacket.c
 *
 * Constuct and send DHCP server packets
 *
 * Russ Dill <Russ.Dill@asu.edu> July 2001
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <time.h>

#include "packet.h"
#include "debug.h"
#include "dhcpd.h"
#include "options.h"
#include "leases.h"
#include "static_leases.h"

static int copy_till(char *inp , char *dest, char sep ,int length)
{
	int i = 0;
  while( (inp[i] != 0x00) && (inp[i] != sep) && (i < length ))
		*(dest + i) = inp[i++];
  *(dest + i) = 0x00;
  return i;
}

/* send a packet to giaddr using the kernel ip stack */
static int send_packet_to_relay(struct dhcpMessage *payload, int ifid)
{
	DEBUG(LOG_INFO, "Forwarding packet to relay");

	return kernel_packet(payload, server_config[ifid].server, SERVER_PORT,
			payload->giaddr, SERVER_PORT);
}


/* send a packet to a specific arp address and ip address by creating our own ip packet */
static int send_packet_to_client(struct dhcpMessage *payload, int force_broadcast, int ifid)
{
	unsigned char *chaddr;
	u_int32_t ciaddr;

	if (force_broadcast) {
		DEBUG(LOG_INFO, "broadcasting packet to client (NAK)");
		ciaddr = INADDR_BROADCAST;
		chaddr = MAC_BCAST_ADDR;
	} else if (payload->ciaddr) {
		DEBUG(LOG_INFO, "unicasting packet to client ciaddr");
		ciaddr = payload->ciaddr;
		chaddr = payload->chaddr;
	} else if (ntohs(payload->flags) & BROADCAST_FLAG) {
		DEBUG(LOG_INFO, "broadcasting packet to client (requested)");
		ciaddr = INADDR_BROADCAST;
		chaddr = MAC_BCAST_ADDR;
	} else {
		DEBUG(LOG_INFO, "unicasting packet to client yiaddr");
		ciaddr = payload->yiaddr;
		chaddr = payload->chaddr;
	}
	return raw_packet(payload, server_config[ifid].server, SERVER_PORT,
			ciaddr, CLIENT_PORT, chaddr, server_config[ifid].ifindex);
}


/* send a dhcp packet, if force broadcast is set, the packet will be broadcast to the client */
static int send_packet(struct dhcpMessage *payload, int force_broadcast, int ifid)
{
	int ret;

	if (payload->giaddr)
		ret = send_packet_to_relay(payload, ifid);
	else ret = send_packet_to_client(payload, force_broadcast, ifid);
	return ret;
}


static void init_packet(struct dhcpMessage *packet, struct dhcpMessage *oldpacket, char type, int ifid)
{
	init_header(packet, type);
	packet->xid = oldpacket->xid;
	memcpy(packet->chaddr, oldpacket->chaddr, 16);
	packet->flags = oldpacket->flags;
	packet->giaddr = oldpacket->giaddr;
	packet->ciaddr = oldpacket->ciaddr;
	add_simple_option(packet->options, DHCP_SERVER_ID, server_config[ifid].server);
}

static int check_valid_request(u_int32_t req, int lease_pool, int ifid)
{
	u_int32_t addr, addrend, ret = 0;
	int i;

	addr = server_config[ifid].start;
	addrend  = server_config[ifid].end;

	DHCPD_TRACE("req = 0x%x, lease_pool = 0x%x, ifid = %d", req, lease_pool, ifid);
	if( lease_pool & 0xFFFFFFFE ) {
		for(i=1;i<=MAX_CLIENT_CLS_COUNT;i++) {
			if((lease_pool >> i) & 0x1) {
				addr = server_config[ifid].CateInfo[i-1].start;
				addrend = server_config[ifid].CateInfo[i-1].end;
				DHCPD_TRACE("cata id = %d, addr = 0x%x, addrend = 0x%x", i - 1, addr , addrend);
                break;
			}
		}
	}
    
	if((ntohl(req) >= ntohl(addr) && ntohl(req) <= ntohl(addrend))) {
		DHCPD_TRACE("addr valid");
		ret = 1;
	}

	return ret;
}


/* add in the bootp options */
static void add_bootp_options(struct dhcpMessage *packet, int ifid)
{
	packet->siaddr = server_config[ifid].siaddr;
	if (server_config[ifid].sname)
		strncpy(packet->sname, server_config[ifid].sname, sizeof(packet->sname) - 1);
	if (server_config[ifid].boot_file)
		strncpy(packet->file, server_config[ifid].boot_file, sizeof(packet->file) - 1);
}


/* send a DHCP OFFER to a DHCP DISCOVER */
int sendOffer(struct dhcpMessage *oldpacket, int ifid)
{
	struct dhcpMessage packet;
	struct dhcpOfferedAddr *lease = NULL;
	u_int32_t req_align, lease_time_align = server_config[ifid].lease,leasetime = 0;
	unsigned char *req, *lease_time;
	struct option_set *curr;
	struct in_addr addr;
	u_int8_t hostname[MAX_HOSTNAME_LEN] = "unknown" , *hname;
	int length = 0;
    unsigned char vciIndex = 0xff;

	unsigned char VClsID[VENDOR_CLASS_IDENTIFIER_LEN];
	unsigned char vendorid[MAX_VENDOR_ID_LEN];
    unsigned char privateopt[VENDOR_PRIVATE_MAX_LEN];
	unsigned char optfieldtype, optfieldlen, *optfieldval;
	unsigned char *opt,optlen = 0;
	unsigned short EnterpriseCode = 0;
	u_int32_t lease_pool = 0;
	int i = 0;
	uint32_t static_lease_ip = 0;
	unsigned char flgFoundMac = 0;

	init_packet(&packet, oldpacket, DHCPOFFER, ifid);

	static_lease_ip = getIpByMac(server_config[ifid].static_leases, oldpacket->chaddr, &flgFoundMac);
	/* untrust static lease ip. MAC is exist but IP is 0.0.0.0, that's mean the MAC is untrusty. */
	if (!static_lease_ip && flgFoundMac){
		LOG(LOG_WARNING, "Untrusty static MAC:%02X:%02X:%02X:%02X:%02X:%02X",
			oldpacket->chaddr[0], oldpacket->chaddr[1], oldpacket->chaddr[2],
			oldpacket->chaddr[3], oldpacket->chaddr[4], oldpacket->chaddr[5]);
		return -1;
	}else{
		addr.s_addr = static_lease_ip;
		LOG(LOG_INFO, "Static MAC:%02X:%02X:%02X:%02X:%02X:%02X will bind to IP:",
			oldpacket->chaddr[0], oldpacket->chaddr[1], oldpacket->chaddr[2],
			oldpacket->chaddr[3], oldpacket->chaddr[4], oldpacket->chaddr[5], inet_ntoa(addr));
		DHCPD_TRACE("static_lease_ip:%s\n", inet_ntoa(addr));
	}


	opt = get_option(oldpacket, DHCP_VENDOR);

	if ( NULL != opt ) {
		optlen = *(opt - 1);
		/* 默认设备为pc */
		//if ( (optlen >= 4) && !(strncmp(opt, "MSFT", 4)) ) {
			//DHCPD_TRACE("COMPUTER");
		//}
		//else {
#if 0
//有电信要求的子属性时用这段代码
            optfieldval = get_opt60_sub(opt, 2);
			if ( optfieldval != NULL ){ /* 包含电信要求的子属性 */
				memcpy(&EnterpriseCode, opt, 2);
				optfieldlen  = *(optfieldval+1);
				if(optfieldlen==0){
					DHCPD_TRACE("COMPUTER");
				}
                            else {
        				for( i = 0; i < MAX_CLIENT_CLS_COUNT; i++) {
        					if(strncmp(server_config[ifid].CateInfo[i].category, optfieldval+2, optfieldlen) == 0) {
        						if(server_config[ifid].CateInfo[i].enable) {
        							lease_pool = (0x1 << (i+1));
        							DHCPD_TRACE("CateInfo = %d", i);
                						break;
        						}
        						else {
        							DHCPD_TRACE("CateInfo %d disable", i);
        							return -1;
        						}
        					}
        				}
                            }
			}
			else {
				DHCPD_TRACE("COMPUTER");
			}
#else
            for( i = 0; i < MAX_CLIENT_CLS_COUNT; i++) 
            {
            	 //DHCPD_TRACE("CateInfo = %s, vendorId = %s, enable = %d", server_config[ifid].CateInfo[i].category,opt,server_config[ifid].CateInfo[i].enable);	
                if(server_config[ifid].CateInfo[i].enable == 1 && server_config[ifid].CateInfo[i].category && strcmp(server_config[ifid].CateInfo[i].category,"-")!=0 
                   && strstr(opt, server_config[ifid].CateInfo[i].category) != NULL) 		//modified by wangjian 20100620. search as substring, no need exact match
                {
                    if(server_config[ifid].CateInfo[i].enable) 
                    {
                        vciIndex = i;
                        lease_pool = (0x1 << (i+1));
                        DHCPD_TRACE("CateInfo = %d", i);
                        break;
                    }
                    else 
                    {
                        DHCPD_TRACE("CateInfo %d disable", i);
                        return -1;
                    }
                }
            }
/*	cancel by wangjian 20100620. When venderid don't match class leasepool, assigned from default leasepool.
            if (!lease_pool)
            {
                return -1;
            }
*/
#endif
		//}
	}

	if ( 0 == lease_pool )
	{
		strncpy(vendorid, DEF_VENDOR_ID, MAX_VENDOR_ID_LEN - 1);
	}
	else
	{
		strncpy(vendorid, server_config[ifid].CateInfo[i].category, MAX_VENDOR_ID_LEN - 1);
	}
	vendorid[MAX_VENDOR_ID_LEN - 1] = '\0';
	DHCPD_TRACE("vendorid = %s", vendorid);

	/* ADDME: if static, short circuit */
	/* the client is in our lease/offered table */
	if (!static_lease_ip && (lease = find_lease_by_chaddr(oldpacket->chaddr, ifid)) && check_valid_request(lease->yiaddr, lease_pool, ifid))
	{
		if (!lease_expired(lease,ifid))
		{
			if(lease->expires == server_config[ifid].inflease_time)
				lease_time_align = server_config[ifid].inflease_time;
			else
				lease_time_align = lease->expires - time(0);
		}
		packet.yiaddr = lease->yiaddr;

	/* Or the client has a requested ip */
	} else if ((req = get_option(oldpacket, DHCP_REQUESTED_IP)) &&

		   /* Don't look here (ugly hackish thing to do) */
		   memcpy(&req_align, req, 4) &&

		   /* and the ip is in the lease range */
		   (check_valid_request(req_align, lease_pool, ifid)) &&
			!static_lease_ip &&  /* Check that its not a static lease. radar */

		   /* and its not already taken/offered */ /* ADDME: check that its not a static lease */
		   ((!(lease = find_lease_by_yiaddr(req_align, ifid)) ||

		   /* or its taken, but expired */ /* ADDME: or maybe in here */
		   lease_expired(lease,ifid)))) {
		   /* check id addr is not taken by a static ip */
		   if(!check_ip(req_align, ifid)) {
				packet.yiaddr = req_align; /* FIXME: oh my, is there a host using this IP? */
			}
		   else {
			   packet.yiaddr = find_address(lease_pool | 0, ifid);

			   /* try for an expired lease */
			   if (!packet.yiaddr) packet.yiaddr = find_address(lease_pool | 1, ifid);
			}



	/* otherwise, find a free IP */ /*ADDME: is it a static lease? */
	}
	else
	{
       	if (static_lease_ip && flgFoundMac){
    	/* It is a static lease... use it */
    		packet.yiaddr = static_lease_ip;
    	}
        else
        {
    		packet.yiaddr = find_address(lease_pool | 0, ifid);
    		/* try for an expired lease */
    		if (!packet.yiaddr) packet.yiaddr = find_address(lease_pool | 1, ifid);
        }
	}

	if(!packet.yiaddr) {
		LOG(LOG_WARNING, "no IP addresses to give -- OFFER abandoned");
		return -1;
	}

    hname = get_option(oldpacket,DHCP_HOST_NAME);
    if(hname)
    {
        add_option_string(packet.options, hname - 2);
        memset(hostname,0x00,MAX_HOSTNAME_LEN);
        length = get_option_length(oldpacket,DHCP_HOST_NAME);
	 length = (length < MAX_HOSTNAME_LEN) ? length : MAX_HOSTNAME_LEN;
        copy_till(hname , hostname, '.',length);
        LOG(LOG_INFO, "SENDING OFFER to %s\n",hostname);
    }

    /*Check for infinite lease */
	if ((lease = find_lease_by_chaddr(oldpacket->chaddr, ifid)))
	{
		if(lease->expires == server_config[ifid].inflease_time)
			leasetime = server_config[ifid].inflease_time;
		else
			leasetime = server_config[ifid].offer_time;
	}
	else
		leasetime = server_config[ifid].offer_time;

	lease = add_lease(packet.chaddr, packet.yiaddr, leasetime /*server_config[ifid].offer_time*/, ifid, vendorid, hostname);
	if (!lease) {
		LOG(LOG_WARNING, "lease pool is full -- OFFER abandoned");
		return -1;
	}

#ifdef ENABLE_OPT125
	add_opt125(&packet, oldpacket, lease);
#endif

	if ((lease_time = get_option(oldpacket, DHCP_LEASE_TIME))) {
		memcpy(&lease_time_align, lease_time, 4);
		lease_time_align = ntohl(lease_time_align);
		if (lease_time_align > server_config[ifid].lease)
			lease_time_align = server_config[ifid].lease;
	}

	/* Make sure we aren't just using the lease time from the previous offer */
	if (lease_time_align < server_config[ifid].min_lease)
		lease_time_align = server_config[ifid].lease;
	/* ADDME: end of short circuit */

  /* For inifinite leases change the lease time */
  if( leasetime == server_config[ifid].inflease_time)
    lease_time_align = leasetime;
	/* ADDME: end of short circuit */
	add_simple_option(packet.options, DHCP_LEASE_TIME, htonl(lease_time_align));

    if (vciIndex != 0xff)
    {
        add_simple_option(packet.options, DHCP_DNS_SERVER, 
                          server_config[ifid].CateInfo[vciIndex].dns);
                              
        curr =  server_config[ifid].CateInfo[vciIndex].options;
        while(curr)
        {   
            add_option_string(packet.options, curr->data);
        	curr = curr->next;
        }
    }

    LOG(LOG_INFO, "SENDOFFER VENDOR INDEX: %d", vciIndex);
	curr = server_config[ifid].options;
	while (curr) {
		unsigned char opt_code;
		opt_code = curr->data[OPT_CODE];
		if(vciIndex == 0xff){
			if(opt_code != DHCP_LEASE_TIME)
				add_option_string(packet.options, curr->data);
		}else{
			if(opt_code == DHCP_ROUTER || opt_code == DHCP_DOMAIN_NAME  || opt_code == DHCP_SUBNET)				
				add_option_string(packet.options, curr->data);
		}
/*		
		if ((curr->data[OPT_CODE] != DHCP_LEASE_TIME)
            && !((vciIndex != 0xff) && (curr->data[OPT_CODE] == DHCP_DNS_SERVER)))
			add_option_string(packet.options, curr->data);*/
		curr = curr->next;
	}

	add_bootp_options(&packet, ifid);

#if 0
	if (CreateVClsIDoption(VENDOR_VCLSID_FOR_GATEWAY, VClsID, EnterpriseCode) != -1)
		add_option_string(packet.options,VClsID);
#endif
    /* telefonica requirements: support Option240 to notify client: <opchAddr:opchPort> */
    if (CreatePrivateOption(DHCP_PRIVATE_240, ifid, privateopt) != -1)
    	add_option_string(packet.options,privateopt);
    
	addr.s_addr = packet.yiaddr;
	LOG(LOG_INFO, "sending OFFER of %s", inet_ntoa(addr));
	return send_packet(&packet, 0, ifid);
}


int sendNAK(struct dhcpMessage *oldpacket, int ifid)
{
	struct dhcpMessage packet;

	init_packet(&packet, oldpacket, DHCPNAK, ifid);

	DEBUG(LOG_INFO, "sending NAK");
	return send_packet(&packet, 1, ifid);
}


int sendACK(struct dhcpMessage *oldpacket, u_int32_t yiaddr, int ifid)
{
	struct dhcpMessage packet;
	struct option_set *curr;
	unsigned char *lease_time;
	u_int32_t lease_time_align = server_config[ifid].lease;
	struct in_addr addr;
       u_int8_t hostname[MAX_HOSTNAME_LEN]="" , *hname;
  int length = 0;
    unsigned char vciIndex = 0xff;
	struct dhcpOfferedAddr *lease = NULL;

	unsigned char vendorid[MAX_VENDOR_ID_LEN];
    unsigned char privateopt[VENDOR_PRIVATE_MAX_LEN];
	u_int32_t lease_pool = 0;
	unsigned char *opt,optlen = 0;
	unsigned char optfieldtype, optfieldlen, *optfieldval = NULL;
	u_int32_t optindex;
	int i = 0;
	uint32_t static_lease_ip = 0;
	unsigned char flgFoundMac = 0;

	static_lease_ip = getIpByMac(server_config[ifid].static_leases, oldpacket->chaddr, &flgFoundMac);

#define CONFIG_VERSION "TW"
	struct VENDORSPECINFO {
		char code, length, fieldtype, fieldlength;
		char fldvalue[4];
	} vendorspecinfo = {DHCP_VENDOR_SPEC_INFO, 6, 1, 4, CONFIG_VERSION};

	init_packet(&packet, oldpacket, DHCPACK, ifid);
	packet.yiaddr = yiaddr;

	opt = get_option(oldpacket, DHCP_VENDOR);

	/* Make a judge if it can from Linux PC */
	if ( NULL != opt ) {
		optlen = *(opt - 1);
		/* Make a judgement if  it can from Windows PC */
		//if ( (optlen >= 4) && !(strncmp(opt, "MSFT", 4)) ) {
		//	DHCPD_TRACE("COMPUTER");
		//} else {
		#if 0
			optfieldval = get_opt60_sub(opt, 2);
			if ( optfieldval != NULL ) {
				optfieldlen  = *(optfieldval+1);
				if(optfieldlen==0){
					DHCPD_TRACE("COMPUTER");
				}
                            else {
                			for( i = 0; i < MAX_CLIENT_CLS_COUNT; i++) {
                				if(strncmp(server_config[ifid].CateInfo[i].category, optfieldval+2, optfieldlen) == 0) {
                					if(server_config[ifid].CateInfo[i].enable) {
                						lease_pool = (0x1 << (i+1));
                						DHCPD_TRACE("CateInfo = %d", i);
                        					break;
                					}
                					else {
                						DHCPD_TRACE("CateInfo %d disable", i);
                						return -1;
                					}
                				}
                			}
                            }
			}
			else {
				DHCPD_TRACE("COMPUTER");
			}
        #endif    
            for( i = 0; i < MAX_CLIENT_CLS_COUNT; i++) 
            {
            	//DHCPD_TRACE("CateInfo = %s, vendorId = %s, enable = %d", server_config[ifid].CateInfo[i].category,opt,server_config[ifid].CateInfo[i].enable);	
                if(server_config[ifid].CateInfo[i].enable == 1 && server_config[ifid].CateInfo[i].category && strcmp(server_config[ifid].CateInfo[i].category,"-")!=0 &&
                   strstr(opt, server_config[ifid].CateInfo[i].category) != NULL)
                {
                    if(server_config[ifid].CateInfo[i].enable) 
                    {
                        vciIndex = i;
                        lease_pool = (0x1 << (i+1));
                        DHCPD_TRACE("CateInfo = %d", i);
                        break;
                    }
                    else 
                    {
                        DHCPD_TRACE("CateInfo %d disable", i);
                        return -1;
                    }
                }
            }         
         
		//}
	}

	if ( 0 == lease_pool )
	{
		strncpy(vendorid, DEF_VENDOR_ID, MAX_VENDOR_ID_LEN - 1);
	}
	else
	{
		strncpy(vendorid, server_config[ifid].CateInfo[i].category, MAX_VENDOR_ID_LEN - 1);
	}
	vendorid[MAX_VENDOR_ID_LEN - 1] = '\0';
	DHCPD_TRACE("vendorid = %s, i = %d", vendorid, i);

	if ( (!static_lease_ip && flgFoundMac) && ! check_valid_request(yiaddr, lease_pool, ifid) )
	{
		DHCPD_TRACE("Call check_valid_request fail");
		sendNAK(oldpacket, ifid);
		return -1;
	}

	if ((lease_time = get_option(oldpacket, DHCP_LEASE_TIME))) {
		memcpy(&lease_time_align, lease_time, 4);
		lease_time_align = ntohl(lease_time_align);

		if (lease_time_align > server_config[ifid].lease)
			lease_time_align = server_config[ifid].lease;
		else if (lease_time_align < server_config[ifid].min_lease)
			lease_time_align = server_config[ifid].lease;
	}

  /* If the existing lease entry has infinite entry give it infinite time */
	if ( (lease = find_lease_by_chaddr(oldpacket->chaddr, ifid)) )
	{
		if(lease->expires == server_config[ifid].inflease_time)
			lease_time_align = server_config[ifid].inflease_time;
	}

	add_simple_option(packet.options, DHCP_LEASE_TIME, htonl(lease_time_align));

  hname = get_option(oldpacket,DHCP_HOST_NAME);
  if(hname)
  {
    add_option_string(packet.options, hname - 2 );
    memset(hostname,0x00,MAX_HOSTNAME_LEN);
    length = get_option_length(oldpacket,DHCP_HOST_NAME);
    length = (length < MAX_HOSTNAME_LEN) ? length : MAX_HOSTNAME_LEN;
    copy_till(hname , hostname, '.',length);
		LOG(LOG_INFO, "SENDING ACK to %s\n",hostname);
  }

    if (vciIndex != 0xff)
    {
        add_simple_option(packet.options, DHCP_DNS_SERVER, 
                          server_config[ifid].CateInfo[vciIndex].dns);

        curr =  server_config[ifid].CateInfo[vciIndex].options;
        while(curr)
        {   
        	add_option_string(packet.options, curr->data);
        	curr = curr->next;
    	}
    }
	curr = server_config[ifid].options;
	while (curr) {
		unsigned char opt_code;
		opt_code = curr->data[OPT_CODE];
		if(vciIndex == 0xff){
			if(opt_code != DHCP_LEASE_TIME)
				add_option_string(packet.options, curr->data);
		}else{
			if(opt_code == DHCP_ROUTER || opt_code == DHCP_DOMAIN_NAME  || opt_code == DHCP_SUBNET)				
				add_option_string(packet.options, curr->data);
		}
		/*
		if ((curr->data[OPT_CODE] != DHCP_LEASE_TIME)
            && !((vciIndex != 0xff) && (curr->data[OPT_CODE] == DHCP_DNS_SERVER)))
			add_option_string(packet.options, curr->data);*/
		curr = curr->next;
	}

	add_bootp_options(&packet, ifid);

	add_option_string(packet.options,(char *)&vendorspecinfo);

    /* telefonica requirements: Option240 notify client: <opchAddr:opchPort> */
    if (CreatePrivateOption(DHCP_PRIVATE_240, ifid, privateopt) != -1)
        add_option_string(packet.options,privateopt);

	addr.s_addr = packet.yiaddr;
	LOG(LOG_INFO, "sending ACK to %s", inet_ntoa(addr));

	lease = add_lease(packet.chaddr, packet.yiaddr, lease_time_align, ifid, vendorid, hostname);

#ifdef ENABLE_OPT125
	add_opt125(&packet, oldpacket, lease);
#endif

	if (send_packet(&packet, 0, ifid) < 0)
		return -1;

	return 0;
}


int send_inform(struct dhcpMessage *oldpacket, int ifid)
{
	struct dhcpMessage packet;
	struct option_set *curr;
	u_int32_t ciaddr;
	int i, vciIndex=0xFF;

	init_packet(&packet, oldpacket, DHCPACK, ifid);

	ciaddr = oldpacket->ciaddr;

	if(ciaddr >=server_config[ifid].start && ciaddr <= server_config[ifid].end){
		vciIndex=0xFF;
	}else{
		for( i = 0; i < MAX_CLIENT_CLS_COUNT; i++) 
	            {
	                if(ciaddr>=server_config[ifid].CateInfo[i].start&&
	                   ciaddr<=server_config[ifid].CateInfo[i].end)
	                {
	                    if(server_config[ifid].CateInfo[i].enable) 
	                    {
	                        vciIndex = i;
	                        DHCPD_TRACE("CateInfo = %d", i);
	                       
	                    }
	                   break;
	                }
	            }   
	}

    if (vciIndex != 0xff)
    {
        unsigned char len;
        unsigned char paramlen = 0;
        u_int8_t *paramreq;

        add_simple_option(packet.options, DHCP_DNS_SERVER, 
                          server_config[ifid].CateInfo[vciIndex].dns);
              
        paramreq = get_option(oldpacket, DHCP_PARAM_REQ);
        if(paramreq)
        {
            paramlen = *(paramreq - 1);

            curr =  server_config[ifid].CateInfo[vciIndex].options;

            while(curr)
            {   
                for(len = 0; len < paramlen; len++)
                {
                    if(*(paramreq+len) == *(curr->data))
                    {
                        add_option_string(packet.options, curr->data);
                        break;
                    }
                }        	    

                curr = curr->next;
            }
        }
    }

	    LOG(LOG_INFO, "SENDOFFER VENDOR INDEX: %d", vciIndex);
		curr = server_config[ifid].options;
		while (curr) {
			unsigned char opt_code;
			opt_code = curr->data[OPT_CODE];
			if(vciIndex == 0xff){
				if(opt_code != DHCP_DNS_SERVER && opt_code != DHCP_LEASE_TIME)
					add_option_string(packet.options, curr->data);
			}else{//DHCP_DNS_SERVER			
				if(opt_code == DHCP_ROUTER || opt_code == DHCP_DOMAIN_NAME  || opt_code == DHCP_SUBNET)				
					add_option_string(packet.options, curr->data);
			}

			curr = curr->next;
		}
/*		
	curr = server_config[ifid].options;
	while (curr) {
		if (curr->data[OPT_CODE] != DHCP_LEASE_TIME)
			add_option_string(packet.options, curr->data);
		curr = curr->next;
	}

	add_bootp_options(&packet, ifid);
*/
#ifdef ENABLE_OPT125
	add_opt125(&packet, oldpacket, NULL);
#endif

	return send_packet(&packet, 0, ifid);
}



