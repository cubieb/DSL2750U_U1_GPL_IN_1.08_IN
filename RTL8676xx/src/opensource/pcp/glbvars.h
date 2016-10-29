#ifndef _GLBVARS_H_
#define _GLBVARS_H_
#include <netinet/in.h> /* INET6_ADDRSTRLEN */ 
#include <sys/queue.h>

#define BUF_1024 1024
#define BUF_32 32
#define BUF_128 128

TAILQ_HEAD(oper_head, pcp_oper);
struct oper_head oper_hd; /* global opertions list */ 

/* for a pending request(that a pcp request having not receiving any response),
 * resend the request forever or give up it and start another request*/
extern int g_flg_resend_forever;

char g_servername[INET6_ADDRSTRLEN];
#endif
