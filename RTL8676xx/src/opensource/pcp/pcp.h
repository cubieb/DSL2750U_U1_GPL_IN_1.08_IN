#ifndef _PCP_H_
#define _PCP_H_
/**
 * @file pcp.h
 * @brief core pcp 
 * @author wangjiaming 
 * @date 2012-02-01
 */
#include <netinet/in.h> /* INET6_ADDRSTRLEN */ 
#include <netdb.h>
#include <sys/queue.h>
#include "options.h"
#include "pcp_defs.h"

enum PCP_TYPE {
	O_MAP = 0x01,
	O_PEER = 0x02,
	O_ANNOUNCE = 0x04,
};
enum PCP_ACTION {
	PCP_ADD,
	PCP_DEL,
};

/* return value */
#define PCP_OK (0)
/* -1 reserved for unknown error */
#define PCP_DROP (-2)
#define PCP_CONNECTERR (-3)
#define PCP_FCNTLERROR (-4)
#define PCP_INVALIDARGS (-5)
#define PCP_MALFORMATRESPONSE (-6)
#define PCP_SENDERR (-7)
/**
 * @brief we 
 */
enum PCP_STATE {
	PCP_INIT = 0, /* OPERATION that have never been sent to server */ 
	PCP_START, /* OPERATION that has been sent to server and waiting for response */
	PCP_ACTIVE, /* OPERATION has received a SUCCESS response */ 
	PCP_FAIL, /* OPERATION has received an error response code from server */ 
};

/* from userland point */ 
typedef struct { /* to map code */ 
	uint8_t protol;	/* conform to IANA protocol registry */ 
	uint16_t in_port;
	uint16_t ext_port;
	struct in6_addr addr6;
	struct opt_head opt_h;
} map_t;


/* from userland point */ 
typedef struct { /* to map code */ 
	uint8_t protol;	/* conform to IANA protocol registry */ 
	uint16_t in_port;
	uint16_t ext_port;
	int rm_port;
	struct in6_addr addr6;
	struct in6_addr rm_addr6;
	struct opt_head opt_h;
} peer_t;

typedef struct {
	int s;
	struct in6_addr pcaddr;
	int pending_req;
	int req_len;
	struct timeval retry_time;
	uint8_t send_buf[PCP_MAXIMUM];
	uint8_t recv_buf[PCP_MAXIMUM + 8];
	struct pcp_timer *timer;
	struct pcp_oper *oper;
} pcp_t;

/**
 * @brief we call a pcp request (map/peer/announce) a OPERATION term, 
 * to distinguish with request term which can be easily confused.
 */
struct pcp_oper
{
	int oid; /* reserved for editing a OPERATION, for now an OPERATION is not allowd to be edited, so this field is not used for now */ 
	enum PCP_TYPE o_type;
      enum PCP_STATE o_state;
	struct timeval tm; /* receipt time for SUCCESS response, otherwise the time until which the PCP_FAIL OPERATION can be restarted */ 
	uint32_t lifetime;
	int timeouts; /* resend number for pending pcp request */ 
	int interval; /* interval between each resend number*/ 
	int renew_nr; /* renew number, record how many times the operation has renewed */
	int error_code; /* the error code for the PCP_FAIL OPERATION */ 
	TAILQ_ENTRY(pcp_oper) link;
	struct pcp_timer *timer;
	void *data; /* pointer to specific request, such as map_t or pcp_t */ 
};


int pcp_construct_reqdata(pcp_t *p, struct pcp_oper *oper);
int pcp_init(pcp_t *p, const char *ifname);
int pcp_close(pcp_t *p);
int pcp_recv(pcp_t *p);
void pcp_reset_oper(struct pcp_oper *oper);
int pcp_send(pcp_t *p);
void client_timo(void *arg);
int in6_addr_equal(struct in6_addr *, struct in6_addr *);
int in6_addr_assign(struct in6_addr *dst, struct in6_addr *src);
void IPV4_MAPPED_IPV6 (struct in_addr soaddr_4, struct in6_addr *soaddr_6);
void free_resources(pcp_t *p);
#endif
