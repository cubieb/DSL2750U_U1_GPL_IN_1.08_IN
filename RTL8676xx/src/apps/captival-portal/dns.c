/**
* scb+ 2011-10-12
*
* file name: dns.c
*
* product the phony dns for captival portal function
*
*/
#include "captival-portal.h"

#define NAME_SIZE 256

/*****************************************************************************/
struct dns_rr_no_name{
  uint16_t type;
  uint16_t class;
  uint32_t ttl;
  uint16_t rdatalen;
  char data[NAME_SIZE];
};

/*****************************************************************************/
/*
union header_flags {
  uint16_t flags;
  
  struct {
    unsigned short int rcode:4;
    unsigned short int unused:3;
    unsigned short int recursion_avail:1;
    unsigned short int want_recursion:1;
    unsigned short int truncated:1;
    unsigned short int authorative:1;
    unsigned short int opcode:4;
    unsigned short int question:1;
  } f ;
} __attribute__((packed));
*/

/*****************************************************************************/
struct dns_header_s{
	uint16_t id;
	uint16_t flags;
	//union header_flags flags;
	uint16_t qdcount;
	uint16_t ancount;
	uint16_t nscount;
	uint16_t arcount;
} __attribute__((packed));
/*****************************************************************************/


static char s_captival_portal_page[CPLPL_MAXPATHLEN] = {0};

struct dns_rec {
	char req_url[CPLPL_MAXPATHLEN];
	uint32_t  phony_ip;
	uint32_t  real_ip;
	int 		resolve_time;
	struct timer_cplpl *to_timer;
	struct timer_cplpl *do_again_timer;
	struct dns_rec *next;
}  *s_dns_rec;

static int cplpl_dns_rcvd(int fd, char *buf, int buflen,
			struct sockaddr *from, int *fromlen);
static int cplpl_dns_respond(int socke, uint32_t phony_ip, 
			char *rcvdpkg, int rcvdlen, struct sockaddr *dst_addr, 
			socklen_t addrlen);

static struct dns_rec * dns_create_and_add_rec(char *pkg, int pkglen);

/*malloc a phony ip addr*/
static uint32_t dns_alloc_phony_ip()
{
	int can_use = 0;	
	struct dns_rec *rec = 0;
	uint32_t ip_start, ip_end, ip_tmp, start, end, tmp;

	inet_pton(AF_INET, DNS_ADDR_START, (void *)&ip_start);
	inet_pton(AF_INET, DNS_ADDR_END, (void *)&ip_end);
	start = ntohl(ip_start);
	end  = ntohl(ip_end);

	for (tmp = start; tmp <= end;  tmp++) {
		can_use = 1;

		ip_tmp = htonl(tmp);

		/*check the addr if is used*/
		for(rec = s_dns_rec; rec; rec = rec->next) {			
			if (rec->phony_ip == ip_tmp) {
				can_use = 0;
				break;
			}
		}
		
		if (can_use)
			return ip_tmp;
	}

	cplpl_printf("no phony ip addr to malloc\n");
	return 0;	
}

/*dns_rec time out timer, the timer will delete the rec*/
static void  dns_rec_timeout(void *data)
{	
	struct dns_rec *rec = (struct dns_rec *)data;

	d_printf("Delete dns rec:%s\n", rec->req_url);

	CDMG_LIST_DEL(struct dns_rec, s_dns_rec, rec);

	/*No node*/
	if (!s_dns_rec && g_demand_exit)
			g_need_exit = 1;

	/*delete the dnat*/
	if (rec->real_ip)
		cplpl_iptable_dnat(0, rec->phony_ip, rec->real_ip);
	if (rec->do_again_timer)
		cplpl_timer_del((void *)(rec->do_again_timer));
	
	free(rec);	
}

/*delete all rec node*/
void  cplpl_dns_rec_delete_all()
{
	struct dns_rec *rec, *rec1;

	for (rec = s_dns_rec; rec;) {
		rec1 = rec;
		rec = rec->next;
		
		/*delete the dnat*/
		if (rec1->real_ip)
			cplpl_iptable_dnat(0, rec1->phony_ip, rec1->real_ip);		

		if (rec1->do_again_timer)
			cplpl_timer_del((void *)rec1->do_again_timer);
		if (rec1->to_timer)
			cplpl_timer_del((void *)rec1->to_timer);
		free(rec1);
	}
	
	s_dns_rec = 0;
}

/*resolve the dns if delete the captival portal*/
static void  dns_rec_resolve(void * data)
{
	char   *ptr;
	char   str[32];
	struct hostent *hptr;	
	struct dns_rec * rec = (struct dns_rec *)data;
	
	ptr = rec->req_url;

	d_printf("===Start to resolve:%s\n", ptr);

	if (rec->resolve_time >= DNS_RESOLVE_TIME_MAX) {
		d_printf("Resolve [%s] fail too times, destroy me\n",
			rec->req_url);

		rec->do_again_timer = 0;
		return;
	}

	/*resove*/
	if(!(hptr = gethostbyname(ptr)))
	{
		d_printf(" gethostbyname error for host:%s\n", ptr);

		rec->resolve_time++;
		
		rec->do_again_timer = 
			cplpl_timer_set(DNS_RESOVLE_PERIOD, 
				"dns_rec_resolve", 
				dns_rec_resolve, 
				(void *)rec);	
		return;
	}

	/*delete do again timer*/
	if (rec->do_again_timer) {
		d_printf("delete dns_rec_resolve timer \n");
		cplpl_timer_del(rec->do_again_timer);
		rec->do_again_timer = 0;
	}
	
      d_printf("===End, official hostname: %s, first address: %s\n", 
	  	hptr->h_name,
             inet_ntop(hptr->h_addrtype, hptr->h_addr, str, sizeof(str)));	

	rec->real_ip = *(uint32_t*)(hptr->h_addr);

	/*add iptable rule*/
	cplpl_iptable_dnat(1, rec->phony_ip, rec->real_ip);

	rec->do_again_timer = 0;
     	return;	
}

/*setup the captival portal item*/
void cplpl_dns_set(const char *url)
{
	memset(s_captival_portal_page, 0, CPLPL_MAXPATHLEN);
	strncpy(s_captival_portal_page, url, CPLPL_MAXPATHLEN-1);
}

/*delete the captival portal item*/
void cplpl_dns_del(const char *url)
{
	struct dns_rec  *tmp;

	d_printf("cancel the captival portal page:%s\n", url);

	if (!s_dns_rec) {
		d_printf("No dns to resolve, exit\n");
		g_need_exit = 1;
		return;
	}

	for (tmp = s_dns_rec; tmp; tmp = tmp->next) {
		if (tmp->real_ip == 0 &&  tmp->phony_ip != 0 ) {
			dns_rec_resolve(tmp);
		}
	}
}

/*init dns*/
int cplpl_dns_init()
{	
	int dns_sock =0;
	uint32_t host = 0;
	struct sockaddr_in sa;

	d_printf("enter\n");
	/* Clear it out */
	memset((void *)&sa, 0, sizeof(sa));

	dns_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	/* Error */
	if( dns_sock < 0 ) {
		perror("Could not create socket");
		return -1;
	} 

	inet_pton(AF_INET, g_local_host, (void *)&host);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = host;
	sa.sin_port = htons(DNS_PORT);
	
  	d_printf("bind dns socket to %s : %d\n", g_local_host, DNS_PORT);
	/* bind() the socket to the interface */
	if (bind(dns_sock, (struct sockaddr *)&sa, 
				sizeof(struct sockaddr_in)) < 0){
		cplpl_printf("dns_init: bind: Could not bind to port %d\n", DNS_PORT);
		perror("bind");
		close(dns_sock);
		return -1;
	}

	return dns_sock;
}


/*process the dns request come from the browse of the PC*/
void cplpl_dns_process(int fd)
{
	struct sockaddr_in from;	
	struct dns_rec * rec;
	int r_len = 0, len = 0;
	int fromlen = sizeof(from);
	char buf[MAX_DNS_PACKET_SIZE] = {0};	
		
	r_len = cplpl_dns_rcvd(fd, buf, MAX_DNS_PACKET_SIZE, 
				(struct sockaddr *)&from, 
				&fromlen);
	if (r_len > 0 && (rec = dns_create_and_add_rec(buf, r_len)) > 0) {
		len = cplpl_dns_respond(fd, rec->phony_ip, buf, r_len, 
				(struct sockaddr *)&from, fromlen);
		d_printf("have send %d bytes\n", len);
	}
}

/*
* receive the dns package
*/
static int cplpl_dns_rcvd(int fd, char *buf, int buflen,
			struct sockaddr *from, int *fromlen)
{ 
	/* Read in the actual packet */
	socklen_t r_len = 0;
	
	d_printf("received dns pkg\n");
	
	memset(buf, 0, buflen);

	while (1) {
		if ((r_len = recvfrom(fd, buf, buflen, 0, from, (socklen_t *)fromlen)) < 0) {
			if (errno == EINTR)
				continue;

			d_printf("No content\n");
			break;
		}
		break;
	}	

	if (is_debug())
		cplpl_printf("RCVD from:%s\n", cplpl_sock_addr_to_p(from));

	if (r_len < 0) 
	{
		perror("dns_read_packet: recvfrom\n");
		return  -1;
	}
  	/* TODO: check source addr against list of allowed hosts */

	/* check that the message is long enough */
	if(r_len < sizeof (struct dns_header_s) )
	{
		cplpl_printf("dns_read_packet not enough long\n");
		return -1;
	}

	return r_len;	
}

/*****************************************************************************/
/* Queries are encoded such that there is and integer specifying how long 
 * the next block of text will be before the actuall text. For eaxmple:
 *             www.linux.com => \03www\05linux\03com\0
 * This function assumes that buf points to an encoded name.
 * The name is decoded into name. Name should be at least 255 bytes long.
 */
void dns_decode_name(char *name, char **buf)
{
	int i, k, len, j;

	i = k = 0;
	
	while( **buf ) {
		len = *(*buf)++;
		for( j = 0; j<len ; j++)
			name[k++] = *(*buf)++;
		name[k++] = '.';
	}
	
	name[k-1] = *(*buf)++;
}

/*parse the dns req then create the dns rec and add to the list*/
static struct dns_rec * dns_create_and_add_rec(char *pkg, int pkglen)
{
	char *q_name;
	char url[MAX_DNS_PACKET_SIZE] = {0};
	char ip_addr[IP4_ADDR_LEN] = {0};
	uint32_t phony_ip;
	struct dns_header_s *h;	
	struct dns_rec *rec = 0;
	

	if (pkglen < sizeof(struct dns_header_s)) {
		cplpl_printf("dns pkg error\n");
		return 0;
	}

	h = (struct dns_header_s *)pkg;

	d_printf("Dns head:\n");
	if (is_debug()) {
		printf("id=0x%04x, flags=0x%04x,qdcount=0x%04x,"
			"ancount=0x%04x,nscount=0x%04x, arcount=0x%04x\n",
			ntohs(h->id), ntohs(h->flags), ntohs(h->qdcount),
			ntohs(h->ancount), ntohs(h->nscount), ntohs(h->arcount));
		
	}

	if (ntohs(h->flags) & 0x8000) {/*check QR bit*/
		cplpl_printf("dns pkg is not req\n");
		return 0;
	}

	if (ntohs(h->qdcount) == 0) {
		cplpl_printf("dns pkg no question\n");
		return 0;		
	}

	q_name = (pkg + sizeof(struct dns_header_s));
	dns_decode_name(url, &q_name);
	d_printf("req url:[%s]\n", url);

	/*check if is exist the rec*/
	for (rec = s_dns_rec; rec; rec = rec->next) {
		if (strcmp(rec->req_url, url) == 0) {
			d_printf("The %s exist\n", url);
			if (rec->to_timer)
				cplpl_timer_del((void*)rec->to_timer);
			rec->to_timer = cplpl_timer_set(DNS_PERIOD, 
					"timeout",  
					dns_rec_timeout, 
					(void *)rec);
			return rec;
		}
	}
	

	if ((phony_ip = dns_alloc_phony_ip()) == 0) {
		cplpl_printf("No phony ip to malloc\n");
		return 0;
	}

	/*malloc a rec node*/
	inet_ntop(AF_INET, (void *)&phony_ip, ip_addr, sizeof(ip_addr));
	d_printf("malloc a rec node, the phony ip is :%s\n", ip_addr);
	CDMG_MALLOC(sizeof(struct dns_rec), rec, return 0);

	strncpy(rec->req_url, url, CPLPL_MAXPATHLEN);
	rec->phony_ip = phony_ip;

	/*add the rec to list*/
	CDMG_LIST_ADD(struct dns_rec, s_dns_rec, rec)->to_timer = 
		cplpl_timer_set(DNS_PERIOD, 
			"timeout", 
			dns_rec_timeout, 
			(void *)rec);
	d_printf("set timeout timer\n");
	return rec;
}

/*
* create a dns respond used the ip addr for the request package 'q_pkg'
* return the len of the created package's length
*/
static int dns_create_respond_package(uint32_t ip, char *rcvdpkg, 
				int rcvdlen, char *res, int reslen)
{
	int sendlen = 0;
	char *buf, *an_buf = 0;
	char *sendbuf  = res;
	struct dns_header_s *qn_h, *an_h;	
	struct dns_rr_no_name *an_rr_no_name;	

	qn_h = (struct dns_header_s *)rcvdpkg;
	an_h = (struct dns_header_s *)sendbuf;

	an_buf = sendbuf;

	/*copy the head*/
	memcpy(an_h, qn_h, sizeof(struct dns_header_s));

	/*set the flags*/
	an_h->flags |= htons(0x8000);  
	
	an_h->ancount = htons(1); /*have one anser*/
	an_h->arcount = 0;
	an_h->nscount = 0;

	/*copy the QName*/
	buf = rcvdpkg + sizeof(struct dns_header_s);
	an_buf += sizeof(struct dns_header_s);
	
	for (; *buf && buf - rcvdpkg < rcvdlen; buf++, an_buf++)
		*an_buf = *buf;
	
	if (*buf)
		return -1;

	/*QName end*/			
	*an_buf = *buf;

	/*copy QType 2 bytes*/
	*(++an_buf) = *(++buf);
	*(++an_buf) = *(++buf);

	/*copy QClass 2 bytes*/
	*(++an_buf) = *(++buf);
	*(++an_buf) = *(++buf);

	/*set anser*/	
	*(++an_buf) = 0xc0;/*the name is a point*/
	*(++an_buf) = 0x0c;/*the location is after the dns head*/

	an_rr_no_name = (struct dns_rr_no_name *)(++an_buf);

	an_rr_no_name->class = htons(1); /*the rr class is 'A', that is ip4 address*/
	an_rr_no_name->ttl = htonl(5); /*5s*/
	an_rr_no_name->type = htons(1); /*IN_ADDR*/
	an_rr_no_name->rdatalen = htons(4); /*4 byte*/	
	*((uint32_t *)&(an_rr_no_name->data)) = ip;

	/*caculate the respond package's len*/
	sendlen = an_buf - sendbuf + sizeof(struct dns_rr_no_name) + 1;

	return sendlen;
}

/*
* send a respond for the received package 
*/
static int cplpl_dns_respond(int socke, uint32_t phony_ip, char *rcvdpkg, 
				int rcvdpkglen, struct sockaddr *dst_addr, socklen_t addrlen)
{
	int sendlen = 0;
	int ret = 0;
	char sendbuf[MAX_DNS_PACKET_SIZE] = {0};

	d_printf("Start to send respond pkg\n");
	if (!phony_ip) {
		cplpl_printf("No ip addr\n");
		return -1;
	}

	memset(sendbuf, 0, MAX_DNS_PACKET_SIZE);
	sendlen = dns_create_respond_package(phony_ip, rcvdpkg, rcvdpkglen, 
				sendbuf, MAX_DNS_PACKET_SIZE);
	d_printf("Try to send %d bytes\n", sendlen);

	/*send the respond pachage*/
	if (sendlen > 0) {
		if (is_debug())
			cplpl_printf("SEND to:%s\n", cplpl_sock_addr_to_p(dst_addr));
		ret = sendto(socke, sendbuf, sendlen, 0, dst_addr, addrlen);
		if (ret < 0)
			perror("sendto");
	} 
	return ret;
}

