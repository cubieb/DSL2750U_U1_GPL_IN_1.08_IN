/**
* scb+ 2011-10-12 
* implement the captival portal function.
*
* this implement need not modify  the httpd and dns.
*
* this modul need the lib file:
*	3g-lib-cmd-msg.c and 3g-lib-cmd-msg.h
*
*/

#include "captival-portal.h"

static int dns_fd = 0;
static int httpd_fd = 0;
static int admin_fd = 0;

char g_local_host[IP4_ADDR_LEN] = "192.168.1.1";
char g_local_mask[IP4_ADDR_LEN] = "255.255.255.0";
char g_local_intf[INTF_NAME_LEN] = "br0";

int   g_need_exit = 0;
int   g_demand_exit = 0;

static void close_session();
static void admin_process();
static int   cplpl_admin_init();


char *cplpl_sock_addr_to_p(void *data)
{
	char ip_addr[IP4_ADDR_LEN] = {0};
	static char buf[IP4_ADDR_LEN *2] = {0};
	struct sockaddr_in * addr = (struct sockaddr_in * )data;
	
	inet_ntop(AF_INET, (void *)&(addr->sin_addr.s_addr), 
			ip_addr, IP4_ADDR_LEN);
	snprintf(buf, sizeof(buf), "%s:%d", ip_addr, ntohs(addr->sin_port));

	return buf;	
}

/**/
void cplpl_sig_term(int sig)
{
	close_session();
	g_need_exit = 1;
}

/*
* do the select loop.
*/
static int session(void)
{
	int rv;
	int max_fd = 0;
	int rfds_max = 0;
	fd_set rfds;
	struct timeval *timeout;

	d_printf("session start\n");
	cdmg_set_daemon();
	signal(SIGTERM, cplpl_sig_term);
	
	while(1) {			
		FD_ZERO(&rfds);
		
		if (dns_fd) {
			FD_SET(dns_fd, &rfds);
			rfds_max = dns_fd;
		}
		
		if (httpd_fd) {
			FD_SET(httpd_fd, &rfds);
			if (rfds_max < httpd_fd)
				rfds_max = httpd_fd;
		}
		
		if (admin_fd) {
			FD_SET(admin_fd, &rfds);
			if (rfds_max < admin_fd)
				rfds_max = admin_fd;
		}

		/*set the accept fd to select*/
		max_fd = cplpl_httpd_fd_set_accept(&rfds);
		if (max_fd > rfds_max)
			rfds_max = max_fd;
		
		timeout = cplpl_timer_handler();
		/*no task to do, so exit*/
		if (g_need_exit) {
			d_printf("close session\n");
			close_session();			
			return 0;
		}		

		d_printf("timeout:%d\n", timeout? (int)timeout->tv_sec : 0);
		
		/* pend, waiting for one or more fds to become ready */
		rv = select(rfds_max+1, &rfds, NULL, NULL, timeout);
		if (rv < 0)
		{
			d_printf("Interrupt\n");
			if (errno != EINTR) {
				perror("select");
				g_need_exit = 1;				
			}

			/*some singal interrupt the select process, do again*/
			continue;
		}
		else if (rv == 0)
		{
			/* time out */
			d_printf("timeout\n");
			continue;
		}

		/*process the message */
		if (FD_ISSET(dns_fd, &rfds))
			cplpl_dns_process(dns_fd);
		else if (FD_ISSET(admin_fd, &rfds))
			admin_process(admin_fd);
		else if (FD_ISSET(httpd_fd, &rfds))
			cplpl_http_process(httpd_fd);
		else 
			cplpl_http_process_accept(&rfds);
	}
}

/* clear all fd and */
static void close_session()
{
	d_printf("close session\n");
	
	if (dns_fd)
		close(dns_fd);
	if (httpd_fd)
		close(httpd_fd);
	if (admin_fd)
		close(admin_fd);

	cplpl_http_close_all_accept();
	cplpl_dns_rec_delete_all();
	
	dns_fd = 0;
	httpd_fd = 0;
	admin_fd = 0;
}

/*the main*/
int main(int argc, char *argv[])
{
	/*set signal*/
	signal(SIGCHLD, SIG_IGN);

	/*set timer*/
	cdmg_init(cplpl_timer_set, cplpl_timer_del, cplpl_timer_handler, 
		CATIVAL_PORTAL_UN_SOCKET_PATH);

	/*do the shell*/
	cdmg_shell(argc, argv);
	
	return 0;
}

static int  session_init()
{
	/*the sub task */
	unlink(CATIVAL_PORTAL_UN_SOCKET_PATH);

	/*create a receive socket*/
	if ((admin_fd = cplpl_admin_init()) < 0) {
		d_printf("can not init unix socket %s\n", 
				CATIVAL_PORTAL_UN_SOCKET_PATH);
		return -1;
	}

	/*init dns_socekt*/
	if ((	dns_fd = cplpl_dns_init()) < 0) {
		close(admin_fd);
		unlink(CATIVAL_PORTAL_UN_SOCKET_PATH);
		d_printf("Can not init dns socket\n");
		return -1;
	}

	/*init httpd*/
	if ((	httpd_fd = cplpl_httpd_init()) < 0) {
		close(admin_fd);
		unlink(CATIVAL_PORTAL_UN_SOCKET_PATH);
		close(dns_fd);
		d_printf("Can not init httpd socket\n");
		return -1;
	}
	return 0;
}

static void get_local_args(int argc, char *argv[])
{
	char intf[INTF_NAME_LEN] = {0};
	char host[IP4_ADDR_LEN] = {0};
	char mask[IP4_ADDR_LEN] = {0};
	
	CDMG_GET_ARG("intf", intf);
	CDMG_GET_ARG("host", host);
	CDMG_GET_ARG("mask", mask);

	if (intf[0])
		strncpy(g_local_intf, intf, INTF_NAME_LEN);
	if (host[0])
		strncpy(g_local_host, host, IP4_ADDR_LEN);
	if (mask[0])
		strncpy(g_local_mask, mask, IP4_ADDR_LEN);	
}

			
/*
* set the captival portal page
*/
CDMG_FUNC(set, 0, 0, 0, 0, 0, 0, "Set the captival portal.\n"
				"exmp: cplpl set  --url=xxx --intf=xxx"" --host=x.x.x.x"
				" --mask=x.x.x.x\n")
{
	char url[CPLPL_MAXPATHLEN] = {0};

	
	CDMG_GET_ARG("url", url);

	if (access(CATIVAL_PORTAL_UN_SOCKET_PATH, F_OK) == 0) {
		d_printf("the old process is exist, send the msg to it to let it"
			" do the things\n");
		if (url[0])
			CDMG_SEND_MSG(captival_portal, " --act=set --url=%s", url);
		return 0;
	}

	if (fork() != 0)
		return 0;

	get_local_args(argc, argv);
	if (url[0]) {
		d_printf("Set url\n");
		cplpl_dns_set(url);
		cplpl_iptable_set(url);
		cplpl_httpd_set(url);
	}
	if (session_init() == 0) {
		d_printf("OK! Start session\n");
		session();
		d_printf("Session Exit!\n");
	}
	
	unlink(CATIVAL_PORTAL_UN_SOCKET_PATH);
	return 0;
}

/*
* delete the captival portal page
*/
CDMG_FUNC(del, 0, 0, 0, 0, 0, 0, "Delete the captival portal.\n"
				"exmp: cplpl del --force\n")
{	
	if (access(CATIVAL_PORTAL_UN_SOCKET_PATH, F_OK) == 0) {
		d_printf("the old process is exist, send the msg to it to let it"
			" do the things\n");
		if (cdmg_get_arg(argv, "force", 0, 0))
			CDMG_SEND_MSG(captival_portal, "%s", "--act=del  --force");
		else
			CDMG_SEND_MSG(captival_portal, "%s", "--act=del ");
	}

	return 0;
}

static void admin_process(admin_fd)
{
	int i;
	static char buf[CDMG_MSG_LEN+1] = {0};

	memset(buf, 0, CDMG_MSG_LEN+1);	
	if ((i = recv(admin_fd, buf, CDMG_MSG_LEN, 0)) > 0) 
	{			
		d_printf("<MSG-CPLPL RCVD %d byte(s)>:[%s]\n", i, buf);

		/*handle the message*/
		cdmg_process_cmd_msg(buf);
	} 
	else
		perror("UNIX socket error:");	
}

static int   cplpl_admin_init()
{
	int s = 0;
	
	/*create a receive socket*/
	if ((s = cdmg_create_unix_socket(CATIVAL_PORTAL_UN_SOCKET_PATH, SOCK_DGRAM)) 
			< 0) {
		d_printf("can not init unix socket %s\n", 
				CATIVAL_PORTAL_UN_SOCKET_PATH);
		return -1;
	}
	d_printf("OK create %d to rcvd admin msg\n", s);
	return s;	
}

/*
* the deamon process
*/
int captival_portal(int argc, char *argv[])
{
	char act[16] = {0};
	char url[CPLPL_MAXPATHLEN] = {0};

	d_printf("enter\n");
	CDMG_GET_ARG("url", url);
	CDMG_GET_ARG("act", act);

	get_local_args(argc, argv);
	
	if (strstr(act, "set") ) {
		g_demand_exit = 0;
		
		cplpl_dns_set(url);
		cplpl_iptable_set(url);
		cplpl_httpd_set(url);
	}
	else {
		g_demand_exit = 1;
		
		cplpl_dns_del(url);
		cplpl_iptable_del(url);
		cplpl_httpd_del(url);
		if (cdmg_get_arg(argv, "force", 0, 0)) {
			cplpl_printf("Force to exit\n");
			cplpl_dns_rec_delete_all();
			g_need_exit = 1;
		}			
	}

	return 0;
}
__CDMG_SETUP_MSG(captival_portal, 0, 1, captival_portal, 
			"the deamon process");

