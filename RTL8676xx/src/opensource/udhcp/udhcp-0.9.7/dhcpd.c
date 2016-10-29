/* dhcpd.c
 *
 * Moreton Bay DHCP Server
 * Copyright (C) 1999 Matthew Ramsay <matthewr@moreton.com.au>
 *			Chris Trew <ctrew@moreton.com.au>
 *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
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

#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <time.h>
#include <sys/time.h>

#include "debug.h"
#include "dhcpd.h"
#include "arpping.h"
#include "socket.h"
#include "options.h"
#include "files.h"
#include "leases.h"
#include "packet.h"
#include "serverpacket.h"
#include "pidfile.h"
#include "static_leases.h"


/* globals */
struct dhcpOfferedAddr *leases;
struct server_config_t server_config[MAX_INTERFACES];
int no_of_ifaces = 0;

struct vendor_id_config_t vendor_id_config[MAX_VENDOR_IDS];

#define LEASE_ADD		1
#define LEASE_DEL 	2
extern void write_to_delta(u_int8_t *chaddr, u_int32_t yiaddr, u_int8_t *vendorid, u_int8_t *hname,unsigned long leasetime,u_int8_t action);

/* Exit and cleanup */
static void exit_server(int retval, int ifid)
{
	pidfile_delete(server_config[ifid].pidfile);
	CLOSE_LOG();
	exit(retval);
}

typedef char* va_list;
/**************************************************************************
功能: 写log信息到日志文件中
参数: log信息
返回: 无
**************************************************************************/
void dhcp_log(char *szFmt, ...)
{
    char buff[1024];
	char file[64];
    long nLen;
    memset(buff,0, 1024);
    vsnprintf(buff, sizeof(buff), szFmt, (va_list)(&szFmt + 1));
    nLen = strlen(buff);
    if (nLen < 0)
        strcpy(buff, "Overflow");
    else if (nLen > (sizeof(buff) - 4))
        buff[sizeof(buff) - 1] = 0;

	snprintf(file, 64, "/var/udhcp.%d.txt", getpid());
	
    FILE *fp = fopen(file, "a");
    fprintf(fp, "%s", buff);
    fclose(fp);
}

/* SIGTERM handler */
static void udhcpd_killed(int sig, int ifid)
{
	sig = 0;
	LOG(LOG_INFO, "Received SIGTERM");
	exit_server(0, ifid);
}

static int test_vendorid(struct dhcpMessage *packet, char *vendorid, int ifid)
{
	int i = 0;
	char length = 0;
	char opt_vend[MAX_VENDOR_ID_LEN];
	for (i = 0; i < MAX_VENDOR_IDS; i++) {
		if (strlen(vendor_id_config[i].vendorid) == 0) {
			continue;
		}
		memset(opt_vend, 0, MAX_VENDOR_ID_LEN);
		length = *(vendorid - 1);
		memcpy(opt_vend, vendorid, (size_t)length);
		if (strlen(opt_vend) != strlen(vendor_id_config[i].vendorid)) {
			continue;
		}
		if (strncmp(opt_vend, vendor_id_config[i].vendorid,
			strlen(vendor_id_config[i].vendorid)) == 0) {
			sendNAK(packet, ifid);
			DHCPD_TRACE("sendNAK");
			return 1;
		}
	}

	DHCPD_TRACE("return 0");
	return 0;
}

#ifdef COMBINED_BINARY
int udhcpd(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	fd_set rfds;
	struct timeval tv;
	int server_socket[MAX_INTERFACES];
	int bytes, retval;
	struct dhcpMessage packet;
	unsigned char *state;
	unsigned char *server_id, *requested;
	u_int32_t server_id_align, requested_align;
	unsigned long timeout_end[MAX_INTERFACES];
	struct option_set *option;
	struct dhcpOfferedAddr *lease;
	struct dhcpOfferedAddr static_lease;
	int pid_fd;
	int i;
  //int totalleases;
	uint32_t static_lease_ip;
  	unsigned char flgFoundMac = 0;

	unsigned char *vendorid = NULL;
	int vendor_id_cfg;

	OPEN_LOG("udhcpd");
	LOG(LOG_INFO, "udhcp server (v%s) started", VERSION);

	for (i = 0; i < MAX_INTERFACES; i++)
		memset(&server_config[i], 0, sizeof(struct server_config_t));

	if (argc < 2)
		read_config(DHCPD_CONF_FILE);
	else read_config(argv[1]);

	if (no_of_ifaces == 0)
		exit(0);

    /*
	for (i = 0; i < no_of_ifaces; i++)
    totalleases += server_config[i].max_leases;
	leases = malloc(sizeof(struct dhcpOfferedAddr) * totalleases);
	memset(leases, 0, sizeof(struct dhcpOfferedAddr) * totalleases);  */


	for (i = 0; i < no_of_ifaces; i++)
	{
		pid_fd = pidfile_acquire(server_config[i].pidfile);
		pidfile_write_release(pid_fd);

		if ((option = find_option(server_config[i].options, DHCP_LEASE_TIME))) {
			memcpy(&server_config[i].lease, option->data + 2, 4);
			server_config[i].lease = ntohl(server_config[i].lease);
		}
		else server_config[i].lease = LEASE_TIME;

		//leases = malloc(sizeof(struct dhcpOfferedAddr) * server_config[i].max_leases);
		//memset(leases, 0, sizeof(struct dhcpOfferedAddr) * server_config[i].max_leases);
    server_config[i].leases = malloc(sizeof(struct dhcpOfferedAddr) * server_config[i].max_leases);
		memset(server_config[i].leases, 0, sizeof(struct dhcpOfferedAddr) * server_config[i].max_leases);
		read_leases(server_config[i].lease_file, i);

		if (read_interface(server_config[i].interface, &server_config[i].ifindex,
			   &server_config[i].server, server_config[i].arp) < 0)
			server_config[i].active = FALSE;
		else
			server_config[i].active = TRUE;

#ifndef DEBUGGING
		pid_fd = pidfile_acquire(server_config[i].pidfile); /* hold lock during fork. */
		/* cfgmr req: do not fork */
		/*
		if (daemon(0, 0) == -1) {
			perror("fork");
			exit_server(1, i);
		}
		*/

		pidfile_write_release(pid_fd);
#endif

		signal(SIGUSR1, write_leases);
		signal(SIGTERM, udhcpd_killed);
	}

	for (i = 0; i < no_of_ifaces; i++) {
		timeout_end[i] = time(0) + server_config[i].auto_time;
		server_socket[i] = -1;
		LOG(LOG_INFO, "interface: %s, start : %x end : %x\n", server_config[i].interface, server_config[i].start, server_config[i].end);
	}

#if 0
        if (read_vendor_id_config(DHCPD_VENDORID_CONF_FILE) == 1) {
		DHCPD_TRACE("Call read_vendor_id_config succ");
		vendor_id_cfg = 1;
        }
#else
        vendor_id_cfg = 0;		// modify 1 to 0 by wangjian20100620
#endif

#ifdef ENABLE_OPT125
	if ( init_opt125() < 0 )
	{
		exit(0);
	}
#endif

	while(1) { /* loop until universe collapses */
		for (i = 0; i < no_of_ifaces; i++)
		{
			if (server_config[i].active == FALSE)
					continue;

			if (server_socket[i] < 0)
				if ((server_socket[i] = listen_socket(INADDR_ANY, SERVER_PORT, server_config[i].interface)) < 0) {
					LOG(LOG_ERR, "FATAL: couldn't create server socket, %s", strerror(errno)/*sys_errlist[errno]*/);
					exit_server(0, i);
				}

			FD_ZERO(&rfds);
			FD_SET(server_socket[i], &rfds);

			if (server_config[i].auto_time) {
				tv.tv_sec = timeout_end[i] - time(0);
				if (tv.tv_sec <= 0) {
					tv.tv_sec = server_config[i].auto_time;
					timeout_end[i] = time(0) + server_config[i].auto_time;
					write_leases(i);
				}
				tv.tv_usec = 0;
			}
			retval = select(server_socket[i] + 1, &rfds, NULL, NULL, server_config[i].auto_time ? &tv : NULL);

			if (retval == 0) {
				write_leases(i);
				timeout_end[i] = time(0) + server_config[i].auto_time;
				continue;
			} else if (retval < 0) {
				DEBUG(LOG_INFO, "error on select");
				continue;
			}

			if ((bytes = get_packet(&packet, server_socket[i])) < 0) { /* this waits for a packet - idle */
				if (bytes == -1 && errno != EINTR) {
					DEBUG(LOG_INFO, "error on read, %s, reopening socket", strerror(errno)/*sys_errlist[errno]*/);
					close(server_socket[i]);
					server_socket[i] = -1;
				}
				continue;
			}

			if ((state = get_option(&packet, DHCP_MESSAGE_TYPE)) == NULL) {
				DEBUG(LOG_ERR, "couldn't get option from packet, ignoring");
				continue;
			}

			/* ADDME: look for a static lease */
		static_lease_ip = getIpByMac(server_config[i].static_leases, &packet.chaddr, &flgFoundMac);

		if(static_lease_ip)
		{
			printf("Found static lease: %x\n", static_lease_ip);

			memcpy(&static_lease.chaddr, &packet.chaddr, 16);
			static_lease.yiaddr = static_lease_ip;
			static_lease.expires = 0;

			lease = &static_lease;

		}
		else
		{
			lease = find_lease_by_chaddr(packet.chaddr, i);
		}

			switch (state[0]) {
				case DHCPDISCOVER:
					DEBUG(LOG_INFO,"received DISCOVER");

					/* Check the vendor ID with the configured vendor ID */
					if (vendor_id_cfg) {
						vendorid = get_option(&packet, DHCP_VENDOR);
						if ( test_vendorid(&packet, vendorid, i) )
							break;
					}

					if (sendOffer(&packet, i) < 0)
						LOG(LOG_ERR, "send OFFER failed");
					break;

				case DHCPREQUEST:
					DEBUG(LOG_INFO, "received REQUEST");
					requested = get_option(&packet, DHCP_REQUESTED_IP);
					server_id = get_option(&packet, DHCP_SERVER_ID);
					if (requested) memcpy(&requested_align, requested, 4);
					if (server_id) memcpy(&server_id_align, server_id, 4);

					/* Check the vendor ID with the configured vendor ID */
					if (vendor_id_cfg) {
						vendorid = get_option(&packet, DHCP_VENDOR);
						if ( test_vendorid(&packet, vendorid, i) )
							break;
					}

					if (lease)
          { /*ADDME: or static lease */
						if (server_id)
            {
							/* SELECTING State */
							DEBUG(LOG_INFO, "server_id = %08x", ntohl(server_id_align));
							if (server_id_align == server_config[i].server && requested &&
					    		requested_align == lease->yiaddr)
								sendACK(&packet, lease->yiaddr, i);
						}
            else
            {
							if (requested)
              {
								/* INIT-REBOOT State */
								if (lease->yiaddr == requested_align)
									sendACK(&packet, lease->yiaddr, i);
								else
                  sendNAK(&packet, i);
							}
              else
              {
								/* RENEWING or REBINDING State */
								if (lease->yiaddr == packet.ciaddr)
								{
								     /*kevin    2009-08-28   BUG:0021018*/
								     /*这个MAC是否绑定了IP?*/
								     static_lease_ip = getIpByMac(server_config[i].static_leases, &packet.chaddr, &flgFoundMac);
								     if(static_lease_ip)
								     {
									  sendACK(&packet, lease->yiaddr, i);
								     }
								     else
								     {/*这个MAC并没有绑定IP*/
								          /*这个IP是否被别的MAC绑定?*/
									    if(reservedIp(server_config[i].static_leases, htonl(lease->yiaddr)))
                                                             {
                                                                /*是，重新获取IP*/
									        lease->yiaddr = 0;
									        sendNAK(&packet, i);
									    }
									    else
									    {
									         sendACK(&packet, lease->yiaddr, i);
									    }
									}
								}
								else
									/* don't know what to do!!!! */
									sendNAK(&packet, i);
							}
						}
            /* what to do if we have no record of the client */
					}
          else if (server_id)
					  /* SELECTING State */
						sendNAK(&packet,i);
          else if (requested)
          {
					  /* INIT-REBOOT State */
					  if ((lease = find_lease_by_yiaddr(requested_align,i)))
            {
					    if (lease_expired(lease,i))
					      /* probably best if we drop this lease */
						    memset(lease->chaddr, 0, 16);
						  /* make some contention for this address */
              else
							  sendNAK(&packet,i);
			      }
            else if (requested_align < server_config[i].start ||
					                   requested_align > server_config[i].end)
					    sendNAK(&packet,i);
            else
						  /* else remain silent */
							sendNAK(&packet,i);
		      }
          else
            /* RENEWING or REBINDING State */
						sendNAK(&packet,i);
          break;
				case DHCPDECLINE:
					DEBUG(LOG_INFO,"received DECLINE");
					if (lease) {
						memset(lease->chaddr, 0, 16);
						lease->expires = time(0) + server_config[i].decline_time;
					}
					break;

				case DHCPRELEASE:
					DEBUG(LOG_INFO,"received RELEASE");
					if (lease)
					{
						if( lease->expires != server_config[i].inflease_time)
						{
							lease->expires = time(0);
							write_to_delta(lease->chaddr, lease->yiaddr, lease->vendorid, lease->hostname, 0,LEASE_DEL);

#ifdef ENABLE_OPT125
							del_opt125(lease);
#endif
						}
					}
					break;

				case DHCPINFORM:
					DEBUG(LOG_INFO,"received INFORM");
					send_inform(&packet, i);
					break;

				default:
					LOG(LOG_WARNING, "unsupported DHCP message (%02x) -- ignoring", state[0]);
			}
		}
	}
	return 0;
}



