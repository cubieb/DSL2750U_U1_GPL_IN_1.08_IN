#ifndef RAMON_H
#define RAMON_H


/* maximum message size for incoming and outgoing RSs and RAs */

#define MSG_SIZE			4096

#define DFLT_AdvManagedFlag		0
#define DFLT_AdvOtherConfigFlag 0

/* Note: these are applicable to receiving sockopts only */
#if defined IPV6_HOPLIMIT && !defined IPV6_RECVHOPLIMIT
# define IPV6_RECVHOPLIMIT IPV6_HOPLIMIT
#endif

#if defined IPV6_PKTINFO && !defined IPV6_RECVPKTINFO
# define IPV6_RECVPKTINFO IPV6_PKTINFO
#endif

int open_icmpv6_socket(void);
int recv_rs_ra(int sock, unsigned char *msg, struct sockaddr_in6 *addr, struct in6_pktinfo **pkt_info, int *hoplimit);
int check_ramon(unsigned char *msg, unsigned int if_index, int len);
int RAMON_ModuleInit(void);

#endif
