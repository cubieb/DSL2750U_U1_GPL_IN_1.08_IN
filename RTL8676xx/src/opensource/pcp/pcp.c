#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <net/if.h>
#include <stdlib.h>

#include "pcp.h"
#include "timer.h"
#include "options.h"
#include "pcp_defs.h"
#include "glbvars.h"
#include "ext_intf.h"

/* #define IPV4_MAPPED_IPV6 (soaddr_4, soaddr_6) \ */
	/* memset(&(soaddr_6), 0, sizeof(struct in6_addr)); \ */
	/* (soaddr_6).s6_addr16[5] = 0xFFFF; \ */
	/* (soaddr_6).s6_addr32[3] = (soaddr_4) \ */

void IPV4_MAPPED_IPV6 (struct in_addr soaddr_4, struct in6_addr *soaddr_6)
{
	memset(soaddr_6, 0, sizeof(struct in6_addr));
	soaddr_6->s6_addr16[5] = 0xFFFF;
	soaddr_6->s6_addr32[3] = (soaddr_4.s_addr);
	return;
}

/**
 * @brief 
 *
 * @param p
 * @param oper
 *
 * @return 0 for success, -1 for fail
 */
int pcp_construct_reqdata(pcp_t *p, struct pcp_oper *oper)
{
	struct pcphdr hdr;
	struct opt_head opt_h;
	struct pcp_option *opt;

	memset(&hdr, 0, sizeof(struct pcphdr));
	hdr.version = 1;
	hdr.rbit = 0; /* 0 indicate request */
	hdr.lifetime = oper->lifetime;
	hdr.pcp_pcaddr = p->pcaddr;
	switch(oper->o_type) {
		case O_ANNOUNCE:
			hdr.opcode = 0;
			break;
		case O_MAP:
			hdr.opcode = 1;
			break;
		case O_PEER:
			hdr.opcode = 2;
			break;
		default:
			return -1;
	}

	mempcpy(p->send_buf, &hdr, sizeof(hdr)); /* pcp common header */ 
	p->req_len = sizeof(struct pcphdr);

	switch(oper->o_type) { /* opcode specific  */ 
		case O_ANNOUNCE:
			hdr.opcode = 0;
			break;
		case O_MAP:
		{
			map_t *mt = (map_t *)(oper->data);
			struct map_code m;
			memset(&m, 0, sizeof(m));
			m.protol = mt->protol;
			m.in_port = htons(mt->in_port);
			m.ext_port = htons(mt->ext_port);
			m.addr6 = mt->addr6; /* need to changed byte-order? */ 
			mempcpy(&p->send_buf[p->req_len], &m, sizeof(m)); /* pcp common header */ 
			opt_h = mt->opt_h;
			printf("\tfunc=%s line=%d, oper->lifetime=%d m.protol=%d\n"
					" m.in_port=%d, m.ext_port%d\n", __FUNCTION__,
					__LINE__, oper->lifetime, m.protol, m.in_port, m.ext_port);

			p->req_len += sizeof(m);
			break;
		}
		case O_PEER:
		{
			peer_t *pt = (peer_t *)(oper->data);
			struct peer_code peer;
			memset(&peer, 0, sizeof(peer));
			peer.protol = pt->protol;
			peer.in_port = htons(pt->in_port);
			peer.ext_port = htons(pt->ext_port);
			peer.addr6 = pt->addr6;
			peer.rm_port = htons(pt->rm_port);
			peer.rm_addr6 = pt->rm_addr6;
			mempcpy(&p->send_buf[p->req_len], &peer, sizeof(peer)); /* pcp common header */ 
			opt_h = pt->opt_h;

			p->req_len += sizeof(peer);
			break;
		}
		default:
			return -1;
	}

	SLIST_FOREACH(opt, &opt_h, link) { /* options */ 
		if ((p->req_len + opt->len) > PCP_MAXIMUM)
			return -1;
		printf("\tfunc=%s line=%d, opt->code=%d, opt->len=%d\n", __FUNCTION__, __LINE__, opt->code, opt->len);
		add_option_string(&p->send_buf[p->req_len], opt);
		p->req_len += opt->len + 4;
	}
}

int pcp_init(pcp_t *p, const char *ifname)
{
	int err;
	uint8_t *sock_addr;
	struct addrinfo hints, *result, *rp;
	if (!p)
		return PCP_INVALIDARGS;

	memset(p, 0, sizeof(pcp_t));

	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	if (getaddrinfo(g_servername, NULL, &hints, &result)) {
		return PCP_INVALIDARGS;
	}
	for (rp=result; rp != NULL; rp = rp->ai_next) {
		p->s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
		if (p->s == -1)
			continue;

		/* allow multiple pcp to work */
		struct ifreq interface;
            strncpy(interface.ifr_ifrn.ifrn_name, ifname, IFNAMSIZ);
            if (setsockopt(p->s, SOL_SOCKET, SO_BINDTODEVICE,(char *)&interface, sizeof(interface)) < 0) {
                   exit(1);
            }

		sock_addr = (uint8_t*)rp->ai_addr;
		*(uint16_t *)(sock_addr + 2) = PCP_PORT;
		if (connect(p->s, rp->ai_addr, rp->ai_addrlen) != -1) {
			struct sockaddr_storage addr;
			socklen_t addrlen = sizeof(addr);
			if (getsockname(p->s, (struct sockaddr*)(&addr), &addrlen)) {
				close(p->s);
				return PCP_CONNECTERR;
			}

			switch (addr.ss_family) {
				case AF_INET:
					IPV4_MAPPED_IPV6( ((struct sockaddr_in *)(&addr))->sin_addr, &p->pcaddr);
					break;
				case AF_INET6:
					p->pcaddr = ((struct sockaddr_in6 *)(&addr))->sin6_addr;
					break;
				default:
					return PCP_INVALIDARGS;
			}
			break;
		}
	}

	if (rp == NULL)
		return PCP_CONNECTERR;

	if (p->s) {
		int flags;
		if ((flags = fcntl(p->s, F_GETFL, 0)) < 0) {
			close(p->s);
			return PCP_FCNTLERROR;
		}
		if (fcntl(p->s, F_SETFL, flags|O_NONBLOCK) < 0) {
			close(p->s);
			return PCP_FCNTLERROR;
		}
	}

	return PCP_OK;
}

int pcp_close(pcp_t *p)
{
	if (!p)
		return PCP_INVALIDARGS;
	if (p->s)
		close(p->s);

	return 0;
}

int pcp_recv(pcp_t *p)
{
	static struct epoch_timer epoch;
	struct epoch_timer epoch_n;
	struct pcp_oper *oper = NULL;
	struct map_code *msend, *mrecv;
	struct peer_code *psend, *precv;
	map_t *mt;
	peer_t *pt;
	char servername[INET6_ADDRSTRLEN] = {'\0'};

	ssize_t                 cnt;
	struct sockaddr_storage ss;
	socklen_t               sslen = sizeof(ss);
	int i = 0;

	cnt = recvfrom(p->s, p->recv_buf, PCP_MAXIMUM, 0, (struct sockaddr*)&ss, &sslen);
	if (cnt == -1) {
		/* perror("\tpcp,recvfrom error"); */
		return  -1;
	}

	/* verify that the source IP address belong to the PCP server of a previously-sent PCP request. */
	switch (ss.ss_family) {
		case AF_INET:
			inet_ntop(AF_INET, &((struct sockaddr_in *)(&ss))->sin_addr, servername, sizeof(servername));
			break;
		case AF_INET6:
			inet_ntop(AF_INET6, &((struct sockaddr_in6 *)(&ss))->sin6_addr, servername, sizeof(servername));
			break;
		default:
			return PCP_INVALIDARGS;
	}
	if (strncmp(g_servername, servername, sizeof(g_servername)))
		return PCP_DROP;

	if (cnt < 0) {
		/* switch(errno) {
		 * [> case EAGAIN: <]
		 * case EWOULDBLOCK:
		 *       return -1;
		 * case ECONNREFUSED:
		 *       r = ERR_NOGATEWAYSUPPORT;
		 *       break;
		 * default:
		 *       r = ERR_RECVFROM; */
		return -1;

	} else { /* refer to 7.3. General PCP Client: Processing a Response */
		if (cnt < 4) {
			return -1;
		}

		struct pcphdr* s_pcphdr = (struct pcphdr *)(p->send_buf);
		struct pcphdr* r_pcphdr = (struct pcphdr *)(p->recv_buf);

		if (r_pcphdr->rbit == 0)
			return -1;

		if ((cnt < 24) || (cnt > PCP_MAXIMUM) || (cnt % 4 != 0)) {  /* not a multiple of 4 octets */
			return PCP_MALFORMATRESPONSE;
		}
		/* check if PCP Client's IP address matchs */
		if (memcmp(&s_pcphdr->pcp_pcaddr, &r_pcphdr->pcp_pcaddr,
					sizeof(struct in6_addr))) {
			return PCP_DROP;
		}

		switch(r_pcphdr->opcode) {
		case OPCODE_ANNOUNCE:
			break;
		case OPCODE_MAP:
			msend = (struct map_code *)&p->send_buf[sizeof(struct pcphdr)];
			mrecv = (struct map_code *)&p->recv_buf[sizeof(struct pcphdr)];
			if ((s_pcphdr->opcode == r_pcphdr->opcode) &&
				(msend->protol == mrecv->protol) &&
					(msend->in_port == mrecv->in_port)) {
				p->pending_req = 0; /* until we get a response, we will resend it forever */ 
				oper = p->oper;
			} else { /* if is not the current pending request */ 
				TAILQ_FOREACH(oper, &oper_hd, link) {
					if(oper->o_type == O_MAP) {
						mt = (map_t *)oper->data;
						if ((mt->protol == mrecv->protol) &&
								(mt->in_port == ntohs(mrecv->in_port))) {
							break;
						}
					}
				}
			}
			break;

		case OPCODE_PEER:
			psend = &p->send_buf[sizeof(struct pcphdr)];
			precv = &p->recv_buf[sizeof(struct pcphdr)];
			if ((s_pcphdr->opcode == r_pcphdr->opcode) &&
					(psend->protol == precv->protol) &&
					(psend->in_port == precv->in_port) &&
					(psend->rm_port == precv->rm_port) &&
					in6_addr_equal(&psend->rm_addr6, &precv->rm_addr6)){
				oper = p->oper;
				p->pending_req = 0; /* until we get a response, we will resend it forever */ 
			} else {
				TAILQ_FOREACH(oper, &oper_hd, link) {
					if(oper->o_type == O_PEER) {
						pt = (peer_t *)oper->data;
						if ((pt->protol == mrecv->protol) &&
								(pt->in_port == ntohs(precv->in_port)) &&
								(pt->rm_port == ntohs(precv->rm_port)) &&
								in6_addr_equal(&pt->rm_addr6, &precv->rm_addr6)){
							break;
						}
					}
				}
			}

			break;
		default:
			return -1;
		}

		if (oper == NULL) /* not found matched specific opcode */
			return PCP_DROP;

		epoch_n.lastepoch = ntohl(r_pcphdr->pcp_epoch);
		epoch_n.lasttime = time(NULL);

		if (!is_valid_epoch(&epoch, &epoch_n)) {
			TAILQ_FOREACH(oper, &oper_hd, link) {
				if(oper->o_state == PCP_ACTIVE)
					oper->o_state = PCP_INIT;
				ext_onotify(oper);
			}
			epoch.lastepoch  = epoch_n.lastepoch;
			epoch.lasttime  = epoch_n.lasttime;
			return PCP_OK;
		}

		oper->lifetime = r_pcphdr->lifetime;
		gettimeofday(&oper->tm, NULL);
		oper->error_code = r_pcphdr->pcp_rescode;
		if (oper->error_code == SUCCESS) {
			oper->o_state = PCP_ACTIVE;
			switch(r_pcphdr->opcode) {
			case OPCODE_ANNOUNCE: /* announce not need to do anything */ 
				return;
			case OPCODE_MAP:
				mt = (map_t *)oper->data;
				mt->ext_port = ntohs(mrecv->ext_port);
				mt->addr6 = mrecv->addr6;
				break;

			case OPCODE_PEER:
				pt = (peer_t *)oper->data;
				pt->ext_port = ntohs(precv->ext_port);
				pt->addr6 = precv->addr6;
				break;
			default:
				return PCP_DROP;
			}
		} else { /* dealing with error code */ 
			oper->o_state = PCP_FAIL;
			p->pending_req = 0;
		}
		pcp_reset_timer(p->oper);

		ext_onotify(oper);
	}

	return PCP_OK;
}


void pcp_reset_oper(struct pcp_oper *oper)
{
	oper->o_state = PCP_INIT;
	oper->tm.tv_sec = 0;
	oper->tm.tv_usec = 0;
	oper->lifetime = 0;
	oper->timeouts = 0;
	oper->interval = 0;
	oper->renew_nr = 0;
}

int pcp_send(pcp_t *p)
{
	int r = PCP_OK;
	if (!p)
		return PCP_INVALIDARGS;

	r = (int)send(p->s, p->send_buf, p->req_len, 0); /* only try 1 time */

	/* ntow = p->req_len;
	 * for (n=0; n<3; n++)
	 *       nw = (int)send(p->s, p->send_buf, ntow, 0); */
	if (r < p->req_len) /* nonblock, need to iterate it? */
		return PCP_SENDERR;

	return r;
}

int in6_addr_equal(struct in6_addr *in1, struct in6_addr *in2)
{
	return memcmp(in1->s6_addr, in2->s6_addr, 16) == 0;
}

int in6_addr_assign(struct in6_addr *dst, struct in6_addr *src)
{
	memcpy(dst->s6_addr, src->s6_addr, 16);
	return 0;
}

void free_resources(pcp_t *p)
{
	struct pcp_oper *oper = NULL;
	struct pcp_oper *tmp_oper = NULL;

	for (oper = TAILQ_FIRST(&oper_hd); oper != NULL; oper = tmp_oper) {
		tmp_oper = TAILQ_NEXT(oper, link);

		/* need only to send once to notify server */
		if (oper->o_state == PCP_ACTIVE) {
			oper->lifetime = 0;
			pcp_construct_reqdata(&p, oper);
			pcp_send(&p);
		}

		/* TAILQ_REMOVE(&oper_hd, oper, link); */
		switch (oper->o_type) {
			case O_MAP:
				free_all_options(&((map_t *)oper->data)->opt_h);
				break;
			case O_PEER:
				free_all_options(&((peer_t *)oper->data)->opt_h);
				break;
			default:
				break;
		}
		free(oper->data);
		free(oper);
	}
}
