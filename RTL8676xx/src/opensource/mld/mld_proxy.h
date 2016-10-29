#ifndef _MLD_PROXY_H_
#define _MLD_PROXY_H_

#include "mld_inc.h"
#include "mld.h"

/******************************************************************************
*                     Global definetions
*******************************************************************************/
#define MLDVERSION  1
#define IS_QUERIER  2
#define UPSTREAM    3
#define DOWNSTREAM  4

#define	MAX_MSGBUFSIZE		2048
#define	MAX_ADDRS	       	50
#define TRUE	         	1
#define FALSE	         	0

#define CLI_MAX_BUF_SZ 128
#define MIN_DEV_LEN    4

#define BUF_CMD	100

#define MAXCTRLSIZE						\
	(sizeof(struct cmsghdr) + sizeof(struct sockaddr_in6) +	\
	sizeof(struct cmsghdr) + sizeof(int) + 32)

#define MLD_MAX_NUM_SRCS          10

/* these definisions are also there br_private.h */
#define SNOOP_IN_ADD		1
#define SNOOP_IN_CLEAR		2
#define SNOOP_EX_ADD		3
#define SNOOP_EX_CLEAR		4

#define	MLD_MINLEN	       	8

#ifdef DEBUG_MLD_PROXY
#define MLD_DEBUG(format, args...) printf("MLD_DEBUG: " format, ##args)
#else
#define MLD_DEBUG(format, args...)
#endif
/******************************************************************************
*                     Structure definetions
*******************************************************************************/
/* MLD interface type */
typedef struct _mld_interface_t 
{
    struct in6_addr            mldi_addr;
    char		               mldi_name[IFNAMSIZ];
    mifi_t 		               mldi_index;
    unsigned short             mldi_version; /*actually, no use*/
    int                        mldi_type;   /*interface type:upstream/downstream*/
    mld_group_t*               mldi_groups;
    sch_query_t*	           sch_group_query;	 	    
    int                        mldi_isquerier;
    int                        mldi_qi;     /* query interval */
    int			               mldi_qri;    /* query response interval */
    int				           mldi_gmi;    /* group membership interval */
    int				           mldi_oqp;    /* other querier present timer */
    int                        mldi_rv;     /* robustness variable */
    int		                   mldi_ti_qi;	/* timer: query interval */
    int                        mldi_socket;	/* mld socket */
    struct _mld_interface_t*   mldi_next;
    int		                   mldi_save_flags;	
    char*			           mldi_buf;
    int				           mldi_bufsize;
    int                        mldi_comp_v1_timer;
} mld_interface_t;

/* proxy membership database */
typedef struct membership_db 
{
  struct 
  {
    struct in6_addr group;
    int fmode;
    int numsources;
    struct in6_addr sources[MLD_MAX_NUM_SRCS];
  } membership;
  struct membership_db *next;
} membership_db;

/* MLD router type */
typedef struct _mld_router_t 
{
  mld_interface_t*        mldrt_interfaces;
  membership_db*          mldrt_membership_db; 
  int 		              mldrt_flag_timer; /*timer_start_flag, 1,started*/
  int 		              mldrt_flag_input; /*no use, just set,no judgement*/
  int 		              mldrt_running;
  pthread_t               mldrt_thr_timer;
  pthread_t               mldrt_thr_input;
  int                     mldrt_up_socket; /*for upstream interface,to tell mld host about reports to send on upstream interface */
  int                     mldrt_socket;    /*downstream, to update mfc and vif table*/
} mld_router_t;

#define ICMPV6_FILTER_WILLPASS(type, filterp) \
	((((filterp)->data[(type) >> 5]) & (1 << ((type) & 31))) == 0)

#define ICMPV6_FILTER_WILLBLOCK(type, filterp) \
	((((filterp)->data[(type) >> 5]) & (1 << ((type) & 31))) != 0)

#define ICMPV6_FILTER_SETPASS(type, filterp) \
	((((filterp)->data[(type) >> 5]) &= ~(1 << ((type) & 31))))

#define ICMPV6_FILTER_SETBLOCK(type, filterp) \
	((((filterp)->data[(type) >> 5]) |=  (1 << ((type) & 31))))

#define ICMPV6_FILTER_SETPASSALL(filterp) \
	memset (filterp, 0, sizeof (struct icmp6_filter));

#define ICMPV6_FILTER_SETBLOCKALL(filterp) \
	memset (filterp, 0xFF, sizeof (struct icmp6_filter));

#endif /* _MLD_PROXY_H_ */
