#include "pcp.h"

/* timer parameters (sec) */
#define INIT_TIMEOUT 2  /* two seconds */ 
#define MAX_RETRANS_DUR 900 /* 15 minutes */ 
#define NEXT_RETRANSMISSION 1800 /* 30 minutes for pcp request that have not received any response from server */ 

/* a < b */
#define TIMEVAL_LT(a, b) (((a).tv_sec < (b).tv_sec) ||\
			  (((a).tv_sec == (b).tv_sec) && \
			    ((a).tv_usec < (b).tv_usec)))
/* a <= b */
#define TIMEVAL_LEQ(a, b) (((a).tv_sec < (b).tv_sec) ||\
			   (((a).tv_sec == (b).tv_sec) &&\
 			    ((a).tv_usec <= (b).tv_usec)))
/* a == b */
#define TIMEVAL_EQUAL(a, b) ((a).tv_sec == (b).tv_sec &&\
			     (a).tv_usec == (b).tv_usec)


struct pcp_timer {
	struct timeval tm; /* must be first to do type change */ 
	struct pcp_timer *(*expire) (void *);
	struct pcp_oper *expire_data;
	TAILQ_ENTRY(pcp_timer) link;
};

/* store epoch and timer for server rebuilding */
struct epoch_timer {
	uint32_t lastepoch;
	uint32_t lasttime;
};

int is_valid_epoch(struct epoch_timer *last, struct epoch_timer *);

void pcp_timer_init (void);
void pcp_set_timer (struct timeval *, struct pcp_timer *);
void pcp_remove_timer (struct pcp_timer **);
void pcp_reset_timer(struct pcp_oper *oper);
struct pcp_timer *pcp_check_timo(struct pcp_timer *tm);
struct pcp_timer *pcp_pick_timer();
struct timeval * pcp_timer_rest (struct pcp_timer *);

void timeval_sub (struct timeval *, struct timeval *, struct timeval *);
void timeval_add (struct timeval *, struct timeval *, struct timeval *);

