/* 
 * files.c -- DHCP server file manipulation *
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 */

//-------------------------------------------------------------------------------------
// Copyright 2005, Texas Instruments Incorporated
//
// This program has been modified from its original operation by Texas Instruments
// to do the following:
// 
// 1. NSP Policy Routing Framework
//
// THIS MODIFIED SOFTWARE AND DOCUMENTATION ARE PROVIDED
// "AS IS," AND TEXAS INSTRUMENTS MAKES NO REPRESENTATIONS 
// OR WARRENTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED 
// TO, WARRANTIES OF MERCHANTABILITY OR FITNESS FOR ANY 
// PARTICULAR PURPOSE OR THAT THE USE OF THE SOFTWARE OR 
// DOCUMENTATION WILL NOT INFRINGE ANY THIRD PARTY PATENTS, 
// COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS. 
// See The GNU General Public License for more details.
//
// These changes are covered under version 2 of the GNU General Public License, 
// dated June 1991.
//------------------------------------------------------------------------------------- 

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <netdb.h>
//#include <linux/pr.h>
#include <netinet/ether.h>
#include "static_leases.h"

#include "debug.h"
#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "leases.h"

#define LEASE_ADD		1
#define LEASE_DEL 	2
extern void write_to_delta(u_int8_t *chaddr, u_int32_t yiaddr, u_int8_t *vendorid, u_int8_t *hname,unsigned long leasetime,u_int8_t action);

/* on these functions, make sure you datatype matches */
static int read_ip(char *line, void *arg)
{
	struct in_addr *addr = arg;
	struct hostent *host;
	int retval = 1;
    //unsigned int pr_mark = UDHCP_PR_MARK;

	if (!inet_aton(line, addr)) {

    /* Adding an option which allows to pass a mark value for a particular
     * socket. */
//#if 0
		if ((host = gethostbyname(line))) 
//#else
		//if ((host = ti_gethostbyname(line, pr_mark))) 
//#endif
			addr->s_addr = *((unsigned long *) host->h_addr_list[0]);
		else retval = 0;
	}
	return retval;
}

static int read_mac(const char *line, void *arg)
{
	uint8_t *mac_bytes = arg;
	struct ether_addr *temp_ether_addr;
	int retval = 1;

	temp_ether_addr = ether_aton(line);

	if(temp_ether_addr == NULL)
		retval = 0;
	else
		memcpy(mac_bytes, temp_ether_addr, 6);

	return retval;
}

static int read_str(char *line, void *arg)
{
	char **dest = arg;
	
	if (*dest) free(*dest);
	*dest = strdup(line);
	
	return 1;
}


static int read_u32(char *line, void *arg)
{
	u_int32_t *dest = arg;
	char *endptr;
	*dest = strtoul(line, &endptr, 0);
	return endptr[0] == '\0';
}


static int read_yn(char *line, void *arg)
{
	char *dest = arg;
	int retval = 1;

	if (!strcasecmp("yes", line))
		*dest = 1;
	else if (!strcasecmp("no", line))
		*dest = 0;
	else retval = 0;
	
	return retval;
}


/* read a dhcp option and add it to opt_list */
static int read_opt(char *line, void *arg)
{
	struct option_set **opt_list = arg;
	char *opt, *val, *endptr;
	struct dhcp_option *option = NULL;
	int retval = 0, length = 0;
	char buffer[255];
	u_int16_t result_u16;
	u_int32_t result_u32;
	int i;

	if (!(opt = strtok(line, " \t="))) return 0;
	
	for (i = 0; options[i].code; i++)
		if (!strcmp(options[i].name, opt))
			option = &(options[i]);
		
	if (!option) return 0;
	
	do {
		val = strtok(NULL, ", \t");
		if (val) {
			length = option_lengths[option->flags & TYPE_MASK];
			retval = 0;
			switch (option->flags & TYPE_MASK) {
			case OPTION_IP:
				retval = read_ip(val, buffer);
				break;
			case OPTION_IP_PAIR:
				retval = read_ip(val, buffer);
				if (!(val = strtok(NULL, ", \t/-"))) retval = 0;
				if (retval) retval = read_ip(val, buffer + 4);
				break;
			case OPTION_STRING:
				length = strlen(val);
				if (length > 0) {
					if (length > 254) length = 254;
					memcpy(buffer, val, length);
					retval = 1;
				}
				break;
			case OPTION_BOOLEAN:
				retval = read_yn(val, buffer);
				break;
			case OPTION_U8:
				buffer[0] = strtoul(val, &endptr, 0);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_U16:
				result_u16 = htons(strtoul(val, &endptr, 0));
				memcpy(buffer, &result_u16, 2);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_S16:
				result_u16 = htons(strtol(val, &endptr, 0));
				memcpy(buffer, &result_u16, 2);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_U32:
				result_u32 = htonl(strtoul(val, &endptr, 0));
				memcpy(buffer, &result_u32, 4);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_S32:
				result_u32 = htonl(strtol(val, &endptr, 0));	
				memcpy(buffer, &result_u32, 4);
				retval = (endptr[0] == '\0');
				break;
			default:
				break;
			}
			if (retval) 
				attach_option(opt_list, option, buffer, length);
		};
	} while (val && retval && option->flags & OPTION_LIST);
	return retval;
}

/* read a dhcp option and add it to opt_list */
static int read_string_opt(char *line, void *arg)
{
	struct option_set **opt_list = arg;
	char *opt, *val, *endptr;
	struct dhcp_option *option = NULL;
	int retval = 0, length = 0;
	char buffer[255];
	u_int16_t result_u16;
	u_int32_t result_u32;
	int i;

	if (!(opt = strtok(line, " \t="))) return 0;


	for (i = 0; options[i].code; i++)
		if (!strcmp(options[i].name, opt))
			option = &(options[i]);

	if (!option) return 0;
	
	do {
		val = strtok(NULL, "\r\n");
		if (val) {
			length = option_lengths[option->flags & TYPE_MASK];
			retval = 0;
			switch (option->flags & TYPE_MASK) {
			case OPTION_IP:
				retval = read_ip(val, buffer);
				break;
			case OPTION_IP_PAIR:
				retval = read_ip(val, buffer);
				if (!(val = strtok(NULL, ", \t/-"))) retval = 0;
				if (retval) retval = read_ip(val, buffer + 4);
				break;
			case OPTION_STRING:
				length = strlen(val);
				if (length > 0) {
					if (length > 254) length = 254;
					memcpy(buffer, val, length);
					retval = 1;
				}
				break;
			case OPTION_BOOLEAN:
				retval = read_yn(val, buffer);
				break;
			case OPTION_U8:
				buffer[0] = strtoul(val, &endptr, 0);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_U16:
				result_u16 = htons(strtoul(val, &endptr, 0));
				memcpy(buffer, &result_u16, 2);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_S16:
				result_u16 = htons(strtol(val, &endptr, 0));
				memcpy(buffer, &result_u16, 2);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_U32:
				result_u32 = htonl(strtoul(val, &endptr, 0));
				memcpy(buffer, &result_u32, 4);
				retval = (endptr[0] == '\0');
				break;
			case OPTION_S32:
				result_u32 = htonl(strtol(val, &endptr, 0));	
				memcpy(buffer, &result_u32, 4);
				retval = (endptr[0] == '\0');
				break;
			default:
				break;
			}
			if (retval) {
				attach_option(opt_list, option, buffer, length);
				}
		};
	} while (val && retval && option->flags & OPTION_LIST);
	return retval;
}

//radar
static int read_staticlease(const char *const_line, void *arg)
{

	char *line;
	char *mac_string;
	char *ip_string;
	uint8_t *mac_bytes;
	uint32_t *ip;


	/* Allocate memory for addresses */
	mac_bytes = malloc(sizeof(unsigned char) * 8);
	ip = malloc(sizeof(uint32_t));

	/* Read mac */
	line = (char *) const_line;
	mac_string = strtok(line, " \t");
	read_mac(mac_string, mac_bytes);

	/* Read ip */
	ip_string = strtok(NULL, " \t");
	read_ip(ip_string, ip);

	addStaticLease(arg, mac_bytes, ip);

#ifdef UDHCP_DEBUG
	printStaticLeases(arg);
#endif

	return 1;

}
//end radar

#define DEV_CONFIG_KW_DEF_VALUE(ifnum, cate_index, start_addr, end_addr) \
	{"enable" #cate_index,     read_u32, &(server_config[ifnum].CateInfo[cate_index].enable), "1"}, \
	{"class" #cate_index,    read_str, &(server_config[ifnum].CateInfo[cate_index].category),    "-"}, \
	{"start" #cate_index,  read_ip,  &(server_config[ifnum].CateInfo[cate_index].start),  "192.168." #ifnum "." #start_addr}, \
	{"end" #cate_index,    read_ip,  &(server_config[ifnum].CateInfo[cate_index].end),    "192.168." #ifnum "." #end_addr},\
	{"dns" #cate_index,    read_ip,  &(server_config[ifnum].CateInfo[cate_index].dns),    ""},\
	{"option" #cate_index,    read_string_opt,  &(server_config[ifnum].CateInfo[cate_index].options),    ""},\
    
#define CONFIG_KW_DEF_VALUE(ifnum) \
{ \
{ \
	/* keyword[14]	handler   variable address		default[20] */ \
	{"start",	read_ip,  &(server_config[ifnum].start),	"192.168." #ifnum ".2"}, \
	{"end",		read_ip,  &(server_config[ifnum].end),		"192.168." #ifnum ".50"}, \
	{"interface",	read_str, &(server_config[ifnum].interface),	"eth" #ifnum}, \
	{"inflease_time",read_u32,&(server_config[ifnum].inflease_time),"604800"}, \
	{"option",	read_string_opt, &(server_config[ifnum].options),	""}, \
	{"opt",		read_opt, &(server_config[ifnum].options),	""}, \
	{"max_leases",	read_u32, &(server_config[ifnum].max_leases),	"254"}, \
	{"remaining",	read_yn,  &(server_config[ifnum].remaining),	"yes"}, \
	{"auto_time",	read_u32, &(server_config[ifnum].auto_time),	"1"/*"7200"*/}, \
	{"decline_time",read_u32, &(server_config[ifnum].decline_time),"3600"}, \
	{"conflict_time",read_u32,&(server_config[ifnum].conflict_time),"3600"}, \
	{"offer_time",	read_u32, &(server_config[ifnum].offer_time),	"60"}, \
	{"min_lease",	read_u32, &(server_config[ifnum].min_lease),	"60"}, \
	{"lease_file",	read_str, &(server_config[ifnum].lease_file),	"/var/lib/misc/udhcpd" #ifnum ".leases"}, \
	{"pidfile",	read_str, &(server_config[ifnum].pidfile),	"/var/run/udhcpd.pid"}, \
	{"opchfile",	read_str, &(server_config[ifnum].opchfile),	"/var/opch.conf"},\
	{"notify_file", read_str, &(server_config[ifnum].notify_file),	""}, \
	{"siaddr",	read_ip,  &(server_config[ifnum].siaddr),	"0.0.0.0"}, \
	{"sname",	read_str, &(server_config[ifnum].sname),	""}, \
	{"boot_file",	read_str, &(server_config[ifnum].boot_file),	""}, \
	{"static_lease",	read_staticlease, &(server_config[ifnum].static_leases),	""},	\
	/*ADDME: static lease. radar */	\
 \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 0, 60, 69) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 1, 70, 79) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 2, 80, 89) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 3, 90, 99) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 4, 100, 109) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 5, 110, 119) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 6, 120, 129) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 7, 130, 139) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 8, 140, 149) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 9, 150, 159) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 10, 160, 169) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 11, 170, 179) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 12, 180, 189) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 13, 190, 199) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 14, 200, 209) \
	DEV_CONFIG_KW_DEF_VALUE(ifnum, 15, 210, 219) \
} \
}

static struct config_kw_arr k_arr[MAX_INTERFACES] = {
	CONFIG_KW_DEF_VALUE(0),
	CONFIG_KW_DEF_VALUE(1),
	CONFIG_KW_DEF_VALUE(2),
	CONFIG_KW_DEF_VALUE(3),
	CONFIG_KW_DEF_VALUE(4),
	CONFIG_KW_DEF_VALUE(5),
};


int read_config(char *file)
{
	FILE *in;
	char buffer[256], orig[256], *token, *line;
	int i, j, index;

	for (index = 0; index < MAX_INTERFACES; index++)
		for (j = 0; j < MAX_KEYWORDS; j++)
			if (strlen(k_arr[index].keywords[j].def))
				k_arr[index].keywords[j].handler(k_arr[index].keywords[j].def, k_arr[index].keywords[j].var);

	
	if (!(in = fopen(file, "r"))) {
		LOG(LOG_ERR, "unable to open config file: %s", file);
		return 0;
	}

	index = -1;	
	while (fgets(buffer, 256, in)) {
		if (strchr(buffer, '\n')) *(strchr(buffer, '\n')) = '\0';
		strncpy(orig, buffer, 256);
		if (strchr(buffer, '#')) *(strchr(buffer, '#')) = '\0';
		token = buffer + strspn(buffer, " \t");
		if (*token == '\0') continue;
		line = token + strcspn(token, " \t=");
		if (*line == '\0') continue;
		*line = '\0';
		line++;
		
		/* eat leading whitespace */
		line = line + strspn(line, " \t=");
		/* eat trailing whitespace */
		for (i = strlen(line) ; i > 0 && isspace(line[i-1]); i--);
		line[i] = '\0';
	
		for (j = 0; j < MAX_KEYWORDS; j++)
		{
			if (!strcasecmp(token, "start")) {
				index++;
			}		
			if (!strcasecmp(token, k_arr[index].keywords[j].keyword)) {
				if (!k_arr[index].keywords[j].handler(line, k_arr[index].keywords[j].var)) {
					LOG(LOG_ERR, "unable to parse '%s'", orig);
					/* reset back to the default value */
					k_arr[index].keywords[j].handler(k_arr[index].keywords[j].def, k_arr[index].keywords[j].var);
				}
				break;
			}
		}
	}
	no_of_ifaces = index+1;
	fclose(in);
	return 1;
}


/* the dummy var is here so this can be a signal handler */
void write_leases(int ifid)
{
	FILE *fp;
	unsigned int i;
	char buf[255];
	time_t curr = time(0);
	unsigned int lease_time;
  int j;
  unsigned char line[100];
  struct in_addr in;
	
	if (!(fp = fopen(server_config[ifid].lease_file, "w"))) {
		LOG(LOG_ERR, "Unable to open %s for writing", server_config[ifid].lease_file);
		return;
	}
	
	for (i = 0; i < server_config[ifid].max_leases; i++) {
		if (server_config[ifid].leases[i].yiaddr != 0) {
			if (server_config[ifid].remaining) {
				if (lease_expired(&(server_config[ifid].leases[i]),ifid))
				{
					lease_time = 0;
					if( server_config[ifid].leases[i].expires != 0)
						write_to_delta(server_config[ifid].leases[i].chaddr,
                           server_config[ifid].leases[i].yiaddr,
                           server_config[ifid].leases[i].vendorid,server_config[ifid].leases[i].hostname,0,LEASE_DEL);
					server_config[ifid].leases[i].expires = 0;
				}
				else
				{
					if( server_config[ifid].leases[i].expires != server_config[ifid].inflease_time)
        		lease_time = server_config[ifid].leases[i].expires - curr;
					else
						lease_time = server_config[ifid].inflease_time;
				}
			}
			else
				lease_time = server_config[ifid].leases[i].expires;

      sprintf(line,"%02x:%02x:%02x:%02x:%02x:%02x ",server_config[ifid].leases[i].chaddr[0],
																								    server_config[ifid].leases[i].chaddr[1],
																								    server_config[ifid].leases[i].chaddr[2],
																								    server_config[ifid].leases[i].chaddr[3],
																								    server_config[ifid].leases[i].chaddr[4],
																								    server_config[ifid].leases[i].chaddr[5]);

      in.s_addr = server_config[ifid].leases[i].yiaddr;
      j = snprintf(&line[18],sizeof(line)-18,"%s %ld %s %s\n",inet_ntoa(in),lease_time,server_config[ifid].leases[i].vendorid,server_config[ifid].leases[i].hostname);
      fwrite( line, 18+j, 1, fp);
		}
	}
	fclose(fp);
	
	if (server_config[ifid].notify_file) {
		sprintf(buf, "%s %s", server_config[ifid].notify_file, server_config[ifid].lease_file);
		system(buf);
	}
}


void read_leases(char *file, int ifid)
{
	FILE *fp;
	unsigned int n;
	struct dhcpOfferedAddr lease;

/* ---------------- beg add by zdl -------------- */
    unsigned int mac[16];
    int i = 0;
/* ---------------- end add by zdl -------------- */
	
  char ipaddress[17],macaddr[18];
  char hname[50];
  char vendorid[MAX_VENDOR_ID_LEN];
  unsigned int leasetime;
  struct in_addr ipaddr;

/*  mac ipaddress leasetime hostname */
#define	readentry(fp) \
	fscanf((fp), "%s %s %u %s %s\n", \
		 macaddr, ipaddress, &leasetime, vendorid, hname)

	if (!(fp = fopen(file, "r"))) {
		LOG(LOG_ERR, "Unable to open %s for reading", file);
		return;
	}

	for (;;)
  {
		if ((n = readentry(fp)) == EOF)
			break;
		if (!inet_aton(ipaddress, &ipaddr))
			continue;
    lease.yiaddr = ipaddr.s_addr;
    memset(lease.chaddr,0x00,16);

/* ---------------- beg del by zdl -----------------
    sscanf(macaddr,"%x:%x:%x:%x:%x:%x", &lease.chaddr[0],
																										&lease.chaddr[1],
																										&lease.chaddr[2],
																										&lease.chaddr[3],
																										&lease.chaddr[4],
																										&lease.chaddr[5]);
 * ---------------- end del by zdl -------------- */

/* ---------------- beg add by zdl -------------- */

    /* 避免字节序问题，因为在sscanf 中，每个%s 都会写四个字节，
      * 如果采用上面的方法，会导致每次取出的mac 都为0 */
    sscanf(macaddr, "%x:%x:%x:%x:%x:%x", mac, mac + 1, mac + 2, mac + 3, mac + 4, mac + 5);
    for ( i = 0; i < 6; i++ )
    {
        lease.chaddr[i] = mac[i];
    }
/* ---------------- end add by zdl -------------- */

    lease.expires = leasetime;
    strcpy(lease.hostname,hname);

    strncpy(lease.vendorid,vendorid,MAX_VENDOR_ID_LEN);

		if (lease.yiaddr >= server_config[ifid].start && lease.yiaddr <= server_config[ifid].end)
    {
			if (!server_config[ifid].remaining) lease.expires -= time(0);
			if (!(add_lease(lease.chaddr, lease.yiaddr, lease.expires, ifid, lease.vendorid, lease.hostname)))
      {
				LOG(LOG_WARNING, "Too many leases while loading %s\n", file);
				break;
			}
    }				
	}
	fclose(fp);
}
		
int read_vendor_id_config(char *file)
{
	FILE *in;
	char buffer[MAX_VENDOR_ID_LEN];
	int i = 0;

	if (!(in = fopen(file, "r"))) {
		DHCPD_TRACE("unable to open config file: %s", file);
		return 0;
	}

	memset(buffer, 0, MAX_VENDOR_ID_LEN);
	for (i = 0; i < MAX_VENDOR_IDS; i++) {
		memset(vendor_id_config[i].vendorid, 0, MAX_VENDOR_ID_LEN);
	}

	i = 0;
	while (fgets(buffer, MAX_VENDOR_ID_LEN, in)) {
		if (i < MAX_VENDOR_IDS) {
			DHCPD_TRACE("vendor_id_config[i] = %s", vendor_id_config[i]);
			memset(vendor_id_config[i].vendorid, 0, MAX_VENDOR_ID_LEN);
			if (buffer[strlen(buffer)-1] == '\n') {
				buffer[strlen(buffer)-1] = '\0';
			}
			memcpy(vendor_id_config[i].vendorid, buffer, strlen(buffer));
			i++;
			memset(buffer, 0, MAX_VENDOR_ID_LEN);
		} else {
			DHCPD_TRACE("vendor id too more");
			break;
		}
	}
	fclose(in);
	//unlink(file);
	return 1;
}

		
