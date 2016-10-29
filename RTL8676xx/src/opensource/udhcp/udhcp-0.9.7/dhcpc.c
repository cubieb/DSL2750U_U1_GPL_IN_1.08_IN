/* dhcpd.c
 *
 * udhcp DHCP client
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

#include <stdio.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/file.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <errno.h>

#include "dhcpd.h"
#include "dhcpc.h"
#include "options.h"
#include "clientpacket.h"
#include "packet.h"
#include "script.h"
#include "socket.h"
#include "debug.h"
#include "pidfile.h"

//save the destination mac,When % 50 time, send unicast package need the MAC address 
unsigned char server_mac_addr_tmp[6],server_mac_addr[6];
static int state;
static unsigned long requested_ip; /* = 0 */
static unsigned long server_addr;
static unsigned long timeout;
static int packet_num; /* = 0 */
static int fd;
static int backup = 0;
static int failurecnt = 0;
static unsigned char backupip[16] = "0.0.0.0";

#define LISTEN_NONE 0
#define LISTEN_KERNEL 1
#define LISTEN_RAW 2
static int listen_mode;

#define DEFAULT_SCRIPT	"/usr/share/udhcpc/default.script"
#define DEFAULT_OPCH    "/var/opch.conf"

struct client_config_t client_config = {
	/* Default options. */
	abort_if_no_lease: 0,
	foreground: 0,
	quit_after_lease: 0,
	interface: "eth0",
	pidfile: NULL,
	opchfile: DEFAULT_OPCH,
	script: DEFAULT_SCRIPT,
	clientid: NULL,
	hostname: NULL,
	ifindex: 0,
	arp: "\0\0\0\0\0\0",		/* appease gcc-3.0 */
  defroute: 1 ,
#ifdef ENABLE_OPT42
  ntpsvr_req: 0,
#endif
};

static void print_usage(void)
{
	printf(
"Usage: udhcpcd [OPTIONS]\n\n"
"  -b, --backupip=IP               Use backup IP on failure\n"
"  -c, --clientid=CLIENTID         Client identifier\n"
"  -H, --hostname=HOSTNAME         Client hostname\n"
"  -f, --foreground                Do not fork after getting lease\n"
"  -i, --interface=INTERFACE       Interface to use (default: eth0)\n"
"  -n, --now                       Exit with failure if lease cannot be\n"
"                                  immediately negotiated.\n"
#ifdef ENABLE_OPT42
"  -N, --ntpsvr                       Request NTP Servers\n"
#endif
"  -p, --pidfile=file              Store process ID of daemon in file\n"
"  -q, --quit                      Quit after obtaining lease\n"
"  -r, --request=IP                IP address to request (default: none)\n"
"  -s, --script=file               Run file at dhcp events (default:\n"
"                                  " DEFAULT_SCRIPT ")\n"
"  -v, --version                   Display version\n"
"  -x, --noroute                   Do not install default route\n"
"  -S, --search                    for PVC search\n"
	);
}


/* just a little helper */
static void change_mode(int new_mode)
{
	DEBUG(LOG_INFO, "entering %s listen mode",
		new_mode ? (new_mode == 1 ? "kernel" : "raw") : "none");
	close(fd);
	fd = -1;
	listen_mode = new_mode;
}


/* SIGUSR1 handler (renew) */
static void renew_requested(int sig)
{
	sig = 0;

	if (state == BOUND || state == RENEWING || state == REBINDING)
  {
	  change_mode(LISTEN_KERNEL);
		packet_num = 0;
		state = RENEW_REQUESTED;
	}
	else if (state == RELEASED) {
    packet_num = 0;
		change_mode(LISTEN_RAW);
		state = INIT_SELECTING;
	}
	LOG(LOG_INFO, "Received SIGUSR1");

	/* Kill any timeouts because the user wants this to hurry along */
	timeout = 0;
}


/* SIGUSR2 handler (release) */
static void release_requested(int sig)
{
	sig = 0;
	/* send release packet */
	if (state == BOUND || state == RENEWING || state == REBINDING) {
		send_release(server_addr, requested_ip); /* unicast */
		run_script(NULL, "deconfig");
	}
	LOG(LOG_INFO, "Received SIGUSR2");
	change_mode(LISTEN_NONE);
	state = RELEASED;
	timeout = 0x7fffffff;
}


/* Exit and cleanup */
static void exit_client(int retval)
{
	pidfile_delete(client_config.pidfile);
	CLOSE_LOG();
	exit(retval);
}


/* SIGTERM handler */
static void terminate(int sig)
{
	sig = 0;
 LOG(LOG_INFO, "Received SIGTERM");
	exit_client(0);

}


static void background(void)
{
	int pid_fd;
	if (client_config.quit_after_lease) {
		exit_client(0);
	} else if (!client_config.foreground) {
		pid_fd = pidfile_acquire(client_config.pidfile); /* hold lock during fork. */
		if (daemon(0, 0) == -1) {
			perror("fork");
			exit_client(1);
		}
		client_config.foreground = 1; /* Do not fork again. */
		pidfile_write_release(pid_fd);
	}
}


#ifdef COMBINED_BINARY
int udhcpc(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	unsigned char *temp, *temp_option121, *message;
	unsigned long t1 = 0, t2 = 0, xid = 0;
	unsigned long start = 0, lease;
	unsigned long searchPVC = 0;
	fd_set rfds;
	int i, retval;
	struct timeval tv;
	int c, len;
	struct dhcpMessage packet;
	struct in_addr temp_addr,temp_inet;
	int pid_fd;
	time_t now;
	char calline[50];
	char * strbuf;
	char * bufpos;
	FILE *fp;
	
	static struct option arg_options[] = {
		{"backupip",	required_argument,		0, 'b'},
		{"clientid",	required_argument,	0, 'c'},
		{"foreground",	no_argument,		0, 'f'},
		{"hostname",	required_argument,	0, 'H'},
		{"help",	no_argument,		0, 'h'},
		{"interface",	required_argument,	0, 'i'},
		{"now", 	no_argument,		0, 'n'},
		#ifdef ENABLE_OPT42
		/* ����-N����������ʾNTP Serversʹ��*/
		{"ntpsrv", 	no_argument,		0, 'N'},
		#endif
		{"pidfile",	required_argument,	0, 'p'},
		{"quit",	no_argument,		0, 'q'},
		{"request",	required_argument,	0, 'r'},
		{"script",	required_argument,	0, 's'},
		{"version",	no_argument,		0, 'v'},
		{"noroute",	no_argument,		0, 'x'},
		{"search",	no_argument,		0, 'S'},
		{0, 0, 0, 0}
	};
	
	/* get options */
	while (1) {
		int option_index = 0;
		#ifdef ENABLE_OPT42
		c = getopt_long(argc, argv, "b:c:fH:hi:nNp:qr:s:vxS", arg_options, &option_index);
		#else
		c = getopt_long(argc, argv, "b:c:fH:hi:np:qr:s:vxS", arg_options, &option_index);
		#endif
		if (c == -1) break;
	
		switch (c) {
			case 'b':
				strcpy(backupip,optarg);
				backup = 1;
				break;
			case 'c':
				len = strlen(optarg) > 255 ? 255 : strlen(optarg);
				if (client_config.clientid) free(client_config.clientid);
				client_config.clientid = malloc(len + 2);
				client_config.clientid[OPT_CODE] = DHCP_CLIENT_ID;
				client_config.clientid[OPT_LEN] = len;
				client_config.clientid[OPT_DATA] = '\0';
				strncpy(client_config.clientid + 3, optarg, len - 1);
				break;
			case 'f':
				client_config.foreground = 1;
				break;
			case 'H':
				len = strlen(optarg) > 255 ? 255 : strlen(optarg);
				if (client_config.hostname) free(client_config.hostname);
				client_config.hostname = malloc(len + 2);
				client_config.hostname[OPT_CODE] = DHCP_HOST_NAME;
				client_config.hostname[OPT_LEN] = len;
				strncpy(client_config.hostname + 2, optarg, len);
				break;
			case 'h':
				print_usage();
				return 0;
				case 'i':
				client_config.interface =  optarg;
				break;
			case 'n':
				client_config.abort_if_no_lease = 1;
				break;
			#ifdef ENABLE_OPT42
			case 'N':
				/* ����NTP ServersΪRequest Option */
				for (i = 0; options[i].code != 0; i++) {
					if (options[i].code == DHCP_NTP_SERVER) {
					options[i].flags |= OPTION_REQ;
					client_config.ntpsvr_req = 1;
					break;
					}
				}
				break;
			#endif
			
			case 'p':
				client_config.pidfile = optarg;
				break;
			case 'q':
				client_config.quit_after_lease = 1;
				break;
			case 'r':
				requested_ip = inet_addr(optarg);
				break;
			case 's':
				client_config.script = optarg;
				break;
			case 'v':
				printf("udhcpcd, version %s\n\n", VERSION);
				exit_client(0);
			case 'x':
				client_config.defroute = 0;
				break;
			case 'S':
				searchPVC = 1;
				break;
		}
	}
	
	OPEN_LOG("udhcpc");
	LOG(LOG_INFO, "udhcp client (v%s) started", VERSION);
	
	pid_fd = pidfile_acquire(client_config.pidfile);
	pidfile_write_release(pid_fd);
	
	if (read_interface(client_config.interface, &client_config.ifindex,
	NULL, client_config.arp) < 0)
		exit_client(1);
	
	if (!client_config.clientid) {
		client_config.clientid = malloc(6 + 3);
		client_config.clientid[OPT_CODE] = DHCP_CLIENT_ID;
		client_config.clientid[OPT_LEN] = 7;
		client_config.clientid[OPT_DATA] = 1;
		memcpy(client_config.clientid + 3, client_config.arp, 6);
	}
	
	/* setup signal handlers */
	signal(SIGUSR1, renew_requested);
	signal(SIGUSR2, release_requested);
	signal(SIGTERM, terminate);
	
	state = INIT_SELECTING;
	run_script(NULL, "deconfig");
	change_mode(LISTEN_RAW);
	
	for (;;) {
		tv.tv_sec = timeout - time(0);
		tv.tv_usec = 0;
		FD_ZERO(&rfds);
	
		if (listen_mode != LISTEN_NONE && fd < 0) {
			if (listen_mode == LISTEN_KERNEL)
				fd = listen_socket(INADDR_ANY, CLIENT_PORT, client_config.interface);
			else
				fd = raw_socket(client_config.ifindex);
			
			if (fd < 0) {
				LOG(LOG_ERR, "FATAL: couldn't listen on socket, %s", strerror(errno)/*sys_errlist[errno]*/);
				exit_client(0);
			}
		}
		if (fd >= 0) FD_SET(fd, &rfds);
		
		if (tv.tv_sec > 0) {
			DEBUG(LOG_INFO, "Waiting on select...\n");
			retval = select(fd + 1, &rfds, NULL, NULL, &tv);
		} else 
			retval = 0; /* If we already timed out, fall through */

		now = time(0);
		
		if (retval == 0) {
			/* timeout dropped to zero */
			switch (state) {
				case INIT_SELECTING:
					if (packet_num < 3) {
						if (packet_num == 0)
							xid = random_xid();
				
						/* send discover packet */
						send_discover(xid, requested_ip); /* broadcast */
						
						timeout = now + ((packet_num == 2) ? 10 : 2);
						packet_num++;
					}
					else{
						if((backup == 1) && (failurecnt >= 2)){
							sprintf(calline,"ifconfig %s %s",client_config.interface,backupip);
							system(calline);
							LOG(LOG_INFO, "Static-Lease of %s",backupip);
							exit_client(1);
						}
						else if (client_config.abort_if_no_lease){
							LOG(LOG_INFO, "No lease, failing.");
							exit_client(1);
						}
						/* wait to try again */
						
						failurecnt++;
						packet_num = 0;
						timeout = now + 5;
					}
					break;
				case RENEW_REQUESTED:
				case REQUESTING:
					if (packet_num < 3)
					{
						/* send request packet */
						if (state == RENEW_REQUESTED){
							send_renew(xid, server_addr, requested_ip); /* unicast */
						
						}else 
							send_selecting(xid, server_addr, requested_ip); /* broadcast */
						
						timeout = now + ((packet_num == 2) ? 10 : 2);
						packet_num++;
					}
					else
					{
						/* timed out, go back to init state */
						state = INIT_SELECTING;
						timeout = now;
						packet_num = 0;
						change_mode(LISTEN_RAW);
					}
					break;
				case BOUND:
					/* Lease is starting to run out, time to enter renewing state */
					state = RENEWING;
					//LISTEN_KERNEL mode can't receive ACK package
					change_mode(LISTEN_RAW);
					DEBUG(LOG_INFO, "Entering renew state");
					break;
					/* fall right through */
				case RENEWING:
					/* Either set a new T1, or enter REBINDING state */
					if ((t2 - t1) <= (lease / 14400 + 1)) {
						/* timed out, enter rebinding state */
						state = REBINDING;
						timeout = now + (t2 - t1);
						DEBUG(LOG_INFO, "Entering rebinding state");
					} else {
						/* send a request packet */
						send_renew(xid, server_addr, requested_ip); /* unicast */
						
					t1 = (t2 - t1) / 2 + t1;
					timeout = t1 + start;
					}
					break;
				case REBINDING:
					/* Either set a new T2, or enter INIT state */
					if ((lease - t2) <= (lease / 14400 + 1)) {
						/* timed out, enter init state */
						state = INIT_SELECTING;
						run_script(NULL, "deconfig");
						LOG(LOG_INFO, "Lease lost, entering init state");
						timeout = now;
						packet_num = 0;
						change_mode(LISTEN_RAW);
					} else {
						/* send a request packet */
						send_renew(xid, 0, requested_ip); /* broadcast */
						
						t2 = (lease - t2) / 2 + t2;
						timeout = t2 + start;
					}
					break;
				case RELEASED:
					/* yah, I know, *you* say it would never happen */
					timeout = 0x7fffffff;
					break;
			}
		} else if (retval > 0 && listen_mode != LISTEN_NONE && FD_ISSET(fd, &rfds)) {
			/* a packet is ready, read it */
			if (listen_mode == LISTEN_KERNEL){
				len = get_packet(&packet, fd);
			}
			else {
				len = get_raw_packet(&packet, fd);
			}
			if (len == -1 && errno != EINTR) {
				DEBUG(LOG_INFO, "error on read, %s, reopening socket", strerror(errno)/*sys_errlist[errno]*/);
				change_mode(listen_mode); /* just close and reopen */
			}
			if (len < 0) continue;
			if (packet.xid != xid) {
				DEBUG(LOG_INFO, "Ignoring XID %lx (our xid is %lx)",
				(unsigned long) packet.xid, xid);
				continue;
			}
			
			if ((message = get_option(&packet, DHCP_MESSAGE_TYPE)) == NULL) {
				DEBUG(LOG_ERR, "couldnt get option from packet -- ignoring");
				continue;
			}
		
			switch (state) {
				case INIT_SELECTING:
					/* Must be a DHCPOFFER to one of our xid's */
					if (*message == DHCPOFFER) {
						//copy the destination mac to server_mac_addr from server_mac_addr_tmp
						if ((temp = get_option(&packet, DHCP_SERVER_ID))) {
							memcpy(&server_addr, temp, 4);
							xid = packet.xid;
							requested_ip = packet.yiaddr;
							
							/* enter requesting state */
							state = REQUESTING;
							timeout = now;
							packet_num = 0;
						} else {
							DEBUG(LOG_ERR, "No server ID in message");
						}
					}
					break;
				case RENEW_REQUESTED:
				case REQUESTING:
				case RENEWING:
				case REBINDING:
					if (*message == DHCPACK) {
						if (!(temp = get_option(&packet, DHCP_LEASE_TIME))) {
							LOG(LOG_ERR, "No lease time with ACK, using 1 hour lease");
							lease = 60 * 60;
						} else {
							memcpy(&lease, temp, 4);
							lease = ntohl(lease);
						}
						
						/* enter bound state */
						t1 = lease / 2;
						
						/* little fixed point for n * .875 */
						t2 = (lease * 0x7) >> 3;
						temp_addr.s_addr = packet.yiaddr;
						
						LOG(LOG_INFO, "Lease of %s obtained, lease time %ld",
						inet_ntoa(temp_addr),lease);

						if (searchPVC == 1){
                            FILE *fps;
                            if ((fps=fopen("/var/TRY_DHCP_SEARCH", "w")) != NULL)
                            {
                                LOG(LOG_INFO, "create TRY_PVC_SEARCH successed! \n");
                                fprintf(fps, "1\n");
                                fclose(fps);
                            }
                            return;
                        }
						
						if (!(temp = get_option(&packet, DHCP_SUBNET)))
							memset(&temp_inet.s_addr, 0x00, 4);
						else
							memcpy(&temp_inet.s_addr, temp, 4);
						
						LOG(LOG_INFO, "Subnet mask %s obtained", inet_ntoa(temp_inet));
				
						/* Added by wangtao for dhcp option 121:֧��Dhcp Option3/Option121��
						Լ������: �����Opion121: �����̬·�ɣ������Option3: Ĭ��·��ѡ�� */
						if(!(temp_option121 = get_option(&packet,DHCP_Classless_StaticRoute))
						&& (temp = get_option(&packet,DHCP_ROUTER))){
							memcpy(&temp_inet.s_addr,temp,4);
							LOG(LOG_INFO, "Router %s obtained", inet_ntoa(temp_inet));
						}
						else if(temp_option121){
							strbuf = (char *)malloc(indefinite_optlen + 1);
							if (strbuf) {
								int iRet = 0;
								memset(strbuf, 0, indefinite_optlen + 1);
								temp_inet.s_addr = 0;
								iRet = parser_lessrouteip(strbuf, &temp_inet.s_addr, 
								temp_option121, temp_option121[OPT_LEN-2]); 
								
								if (-1 == iRet){
									dhcp_log("parser_lessrouteip return err\r\n");
								}
						
								/* output the default route which will be captured by pc */  
								if (temp_inet.s_addr != 0){
									LOG(LOG_INFO, "Router %s obtained", inet_ntoa(temp_inet));
								}
								/* output static routes which will be captured by pc */  
								if (iRet > 0){
									LOG(LOG_INFO, "Static routes %s", strbuf);
								} 
								
								free(strbuf);
							}
							
							#ifdef ENABLE_OPT250
							free_option(temp);
							#endif
						}
				
						/* ������δ��룬ƥ��Dns ������*/
						if(temp = get_option(&packet, DHCP_DNS_SERVER)){
							strbuf = (char *)malloc(indefinite_optlen + 1);
							if (strbuf) {
								memset(strbuf, 0, indefinite_optlen + 1);
								i = 0;
								bufpos = strbuf;
								while (i < indefinite_optlen){
									sprintf(bufpos, "%d.%d.%d.%d%s", temp[i], temp[i+1], temp[i+2],
									temp[i+3], ((i+4)==temp[OPT_LEN-2])?"":",");
									i += 4;
									bufpos += strlen(bufpos);
								}
								bufpos = '\0';
								LOG(LOG_INFO, "Dns server %s obtained", strbuf);
								free(strbuf);
							}
							#ifdef ENABLE_OPT250
							free_option(temp);
							#endif
						}                    
						
						#ifdef ENABLE_OPT15
							/* ������δ��룬ƥ��Domain Name */
							if (temp = get_option(&packet, DHCP_DOMAIN_NAME)) {
								strbuf = (char *)malloc(indefinite_optlen + 1);
								if (strbuf) {
									memset(strbuf, 0, indefinite_optlen + 1);
									memcpy(strbuf, temp, indefinite_optlen);
									strbuf[indefinite_optlen + 1] = '\0';
									LOG(LOG_INFO, "Domain Name:%s", strbuf);
									free(strbuf);
								}
								
								#ifdef ENABLE_OPT250
								free_option(temp);
								#endif
							}
						#endif
						
						#ifdef ENABLE_OPT42
						/* ������δ��룬ƥ��NTPServers */
							if (client_config.ntpsvr_req) {
								if (temp = get_option(&packet, DHCP_NTP_SERVER)) {
									strbuf = (char *)malloc(indefinite_optlen + 1);
									
									if (strbuf) {
										memset(strbuf, 0, indefinite_optlen + 1);
										i = 0;
										bufpos = strbuf;
										
										while (i < indefinite_optlen) {
											sprintf(bufpos, "%d.%d.%d.%d%s", temp[i], temp[i+1], temp[i+2],
											temp[i+3], ((i+4)==temp[OPT_LEN-2])?"":",");
											i += 4;
											bufpos += strlen(bufpos);
										}
										
										bufpos = '\0';
										LOG(LOG_INFO, "NTP Server:%s", strbuf);
										free(strbuf);
									}
								}
								#ifdef ENABLE_OPT250
								free_option(temp);
								#endif
							}
						#endif
						
						/* telefonica requirements: record the <opchAddr:opchPort> */
						if(temp = get_option(&packet, DHCP_PRIVATE_240)){
							/* use for log */
							strbuf = (char *)malloc(indefinite_optlen + 1);
							
							if (strbuf){
								memset(strbuf, 0, indefinite_optlen + 1);
								memcpy(strbuf, temp, indefinite_optlen);
								strbuf[indefinite_optlen + 1] = '\0';
								LOG(LOG_INFO, "OPCH CFG: %s", strbuf);
								free(strbuf);
							}
							/* record the result to DEFAULT_OPCH */
							if (!(fp = fopen(client_config.opchfile, "w"))){
								LOG(LOG_ERR, "Unable to create file %s to record the opch cfg ", 
								client_config.opchfile);
							} 
							
							fwrite(temp, sizeof(char), indefinite_optlen, fp);
							fclose(fp);
						}                    
						
						start = now;
						timeout = t1 + start;
						requested_ip = packet.yiaddr;
						
						if(client_config.defroute == 1)
							run_script(&packet,
								((state == RENEWING || state == REBINDING) ? "renew" : "bound"));
						else
							run_script(&packet, "altbound");
						
						LOG(LOG_INFO, "Lease of %s obtained, lease time %ld",
						inet_ntoa(temp_addr),lease);
						
						/* ����ȷ��ƥ�����*/
					LOG(LOG_INFO, "Lease obtained, entering bound state");
						state = BOUND;
						change_mode(LISTEN_NONE);
						
						//background();
				
					} else if (*message == DHCPNAK) {
						/* return to init state */
						run_script(&packet, "nak");
						if (state != REQUESTING)
						run_script(NULL, "deconfig");
						LOG(LOG_INFO, "Received DHCP NAK");
						state = INIT_SELECTING;
						timeout = now;
						requested_ip = 0;
						packet_num = 0;
						change_mode(LISTEN_RAW);
						sleep(3); /* avoid excessive network traffic */
					}
				break;
				/* case BOUND, RELEASED: - ignore all packets */
			}
		} else if (retval == -1 && errno == EINTR) {
			/* a signal was caught */
		} else {
			/* An error occured */
			DEBUG(LOG_ERR, "Error on select");
		}
	}
	
	return 0;
}
