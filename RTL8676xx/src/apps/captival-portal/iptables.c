/**
* scb+ 2011-10-12
*
* file name: iptables.c
*
* create the iptable rule for captival portal function
*
*/

#include "captival-portal.h"

static char s_cmd[CMD_BUF_LEN] = {0};
static int s_rule_is_set = 0;

/*set some rule for redirect the dns and http pakage to the captival portal deamon*/				
void cplpl_iptable_set(const char *url)
{
	/*
	*change the dst port  for dns:
	* 192.168.1.2:1234, x.x.x.x:53--->192.168.1.2:1234, 192.168.1.1:5053
	*/

	if (s_rule_is_set)
		return;
	
	snprintf(s_cmd, CMD_BUF_LEN,
		"iptables -t nat -A PREROUTING -i %s -p udp --dport %d"
		" -j DNAT --to-destination %s:%d",
		g_local_intf,
		RAW_DNS_PORT,
		g_local_host,
		DNS_PORT);	
	d_printf("do:%s\n", s_cmd);
	system(s_cmd);

	/*
	* change the dst port  for http:
	* 	192.168.1.2:1234, x.x.x.x:80--->192.168.1.2:1234, 192.168.1.1:5080
	*/
	snprintf(s_cmd, CMD_BUF_LEN,
		"iptables -t nat -A PREROUTING -i %s  -p tcp -d ! %s/%s --dport %d"
		" -j DNAT --to-destination %s:%d",
		g_local_intf,
		g_local_host,
		g_local_mask,
		RAW_HTTP_PORT,
		g_local_host,
		HTTP_PORT);
	d_printf("do:%s\n", s_cmd);
	system(s_cmd);

	s_rule_is_set = 1;
	
	return;
}

void cplpl_iptable_del(const char *url)
{

	if (!s_rule_is_set) {
		d_printf("No rule to del\n");
		return;
	}
	
	/*
	*delete the dst port  for dns:
	* 192.168.1.2:1234, x.x.x.x:53--->192.168.1.2:1234, 192.168.1.1:5053
	*/
	snprintf(s_cmd, CMD_BUF_LEN,
		"iptables -t nat -D PREROUTING -i %s -p udp --dport %d "
		"-j DNAT --to-destination %s:%d",
		g_local_intf,
		RAW_DNS_PORT,
		g_local_host,
		DNS_PORT);
	d_printf("CMD:%s\n", s_cmd);
	system(s_cmd);

	/*
	* delete the dst port  for http:
	* 	192.168.1.2:1234, x.x.x.x:80--->192.168.1.2:1234, 192.168.1.1:5080
	*/
	snprintf(s_cmd, CMD_BUF_LEN,
		"iptables -t nat -D PREROUTING -i %s  -p tcp -d ! %s/%s --dport %d "
		"-j DNAT --to-destination %s:%d",
		g_local_intf,
		g_local_host,
		g_local_mask,
		RAW_HTTP_PORT,
		g_local_host,
		HTTP_PORT);
	d_printf("CMD:%s\n", s_cmd);
	system(s_cmd);

	s_rule_is_set = 0;
	
	return;
}

int cplpl_iptable_dnat(int is_add, uint32_t raw_ip, uint32_t new_ip)
{
	char act[4] = {0};
	char r_ip[IP4_ADDR_LEN] = {0};
	char n_ip[IP4_ADDR_LEN] = {0};

	if (is_add)
		strcpy(act, "-A");
	else
		strcpy(act, "-D");

	inet_ntop(AF_INET, (void *)&raw_ip, r_ip, IP4_ADDR_LEN);
	inet_ntop(AF_INET, (void *)&new_ip, n_ip, IP4_ADDR_LEN);

	
	/*
	*add the dst dnat: raw_ip-->new_ip
	*/
	snprintf(s_cmd, CMD_BUF_LEN,
		"iptables -t nat %s PREROUTING -i %s -d %s "
		"-j DNAT --to-destination %s", 
		act,
		g_local_intf,
		r_ip,
		n_ip);
	d_printf("CMD:%s\n", s_cmd);
	system(s_cmd);

	return 0;
}
