#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/queue.h>

#include <netinet/in.h>

#include <unistd.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include "pcp.h"
#include "glbvars.h"
#include "timer.h"

#define MILLION 1000000

TAILQ_HEAD(, pcp_timer) timer_head;
static struct timeval tm_max = {0x7fffffff, 0x7fffffff};

void
pcp_timer_init()
{
	TAILQ_INIT(&timer_head);
}

/**
 * @brief insertion sort to add a new timer
 *
 * @param timeout
 * @param timeodata
 *
 * @return NULL if failed and pointer to new timer if success
 */
struct pcp_timer *
pcp_add_timer(timeout, timeodata)
	struct pcp_timer *(*timeout) (void *);
	struct pcp_oper *timeodata;
{
	struct pcp_timer *newtimer;
	struct timeval now;
	gettimeofday(&now, NULL);

	if ((newtimer = malloc(sizeof(*newtimer))) == NULL) {
		return (NULL);
	}

	memset(newtimer, 0, sizeof(*newtimer));

	if (timeout == NULL) {
		exit(1);
	}
	newtimer->expire = timeout;
	newtimer->expire_data = timeodata;
	newtimer->tm = now;

	TAILQ_INSERT_TAIL(&timer_head, newtimer, link);

	return (newtimer);
}

void
pcp_remove_timer(timer)
	struct pcp_timer **timer;
{
	TAILQ_REMOVE(&timer_head, *timer, link);
	free(*timer);
	*timer = NULL;
}

void
pcp_set_timer(tm, timer)
	struct timeval *tm;
	struct pcp_timer *timer;
{
	struct timeval now;

	/* reset the timer */
	gettimeofday(&now, NULL);

	timeval_add(&now, tm, &timer->tm);

	return;
}

/*
 * Check expiration for each timer. If a timer is expired,
 * call the expire function for the timer and update the timer.
 * Return the next interval for select() call.
 */
struct pcp_timer *pcp_check_timo(struct pcp_timer *tm)
{
	struct timeval now, *tm_sentinel = NULL;
	struct pcp_timer *timer = NULL;

	gettimeofday(&now, NULL);

	if (tm) {
		if (TIMEVAL_LEQ(tm->tm, now))
			(tm->expire)(tm->expire_data);
		return tm;
	}

	/**
	 * @brief only do a expiration function
	 * @author wangjiaming
	 * @date 2012-02-15
	 */

	TAILQ_FOREACH(timer, &timer_head, link) {
		if (TIMEVAL_LEQ(timer->tm, now))
			(*timer->expire)(timer->expire_data);
	}
}

struct pcp_timer *pcp_pick_timer()
{
	struct timeval now, *tm_sentinel = NULL;
	struct pcp_timer *timer = NULL;

       gettimeofday(&now, NULL);
   
	/**
	 * @brief only do a expiration function
	 * @author wangjiaming
	 * @date 2012-02-15
	 */
	TAILQ_FOREACH(timer, &timer_head, link) {
		if (tm_sentinel == NULL)
			tm_sentinel = &timer->tm;
		else if (TIMEVAL_LT(timer->tm, *tm_sentinel))
			tm_sentinel = &timer->tm;
	}

	/* select the next timer, first PCP_INIT */
	if ((tm_sentinel == NULL) || (TIMEVAL_LT(now, *tm_sentinel))) { /* timer has been sorted from small to big */
		struct pcp_oper *oper;
		TAILQ_FOREACH(oper, &oper_hd, link) {
			if (oper->o_state == PCP_INIT) {
				timer = pcp_add_timer(client_timo, oper);
				oper->timer = timer;
				break;
			}
		}
	}

	if ((!timer) && tm_sentinel)
		timer =  (struct pcp_timer *)tm_sentinel;

	/* [> select the next timer, second PCP_FAIL <]
	 * if (timer == NULL) {
	 *       struct pcp_oper *oper;
	 *       TAILQ_FOREACH(oper, oper_hd, link) {
	 *             if (oper->o_state == PCP_FAIL) {
	 *                   timer = pcp_add_timer(client_timo, oper);
	 *             }
	 *       } */

	return timer;
}

struct timeval * pcp_timer_rest(timer)
	struct pcp_timer *timer;
{
	struct timeval now;
	static struct timeval returnval; /* XXX */

	if (timer == NULL)
		return NULL;

	gettimeofday(&now, NULL);
	if (TIMEVAL_LEQ(timer->tm, now)) {
		returnval.tv_sec = returnval.tv_usec = 0;
	} else
		timeval_sub(&timer->tm, &now, &returnval);

	return (&returnval);
}

void pcp_reset_timer(struct pcp_oper *oper)
{
	struct timeval tm = {0,0};
	switch (oper->o_state) {
		/* case PCP_INIT: [> resend <]  */
		case PCP_START:
			/* printf("\tfunc=%s line=%d, oper=%p, oper->timeouts=%d\n",
			 *             __FUNCTION__, __LINE__, oper, oper->timeouts); */
			if (oper->timeouts == 0) {/* need to do random delay? */ 
				oper->interval = 0;
			} else if (oper->timeouts == 1) {
				oper->interval = INIT_TIMEOUT;
			} else if (oper->timeouts > 1) {
				oper->interval *= 2;
			}

			if (oper->interval > MAX_RETRANS_DUR) {
				oper->interval = MAX_RETRANS_DUR;
			}
			if (oper->interval == MAX_RETRANS_DUR) {
				if (!g_flg_resend_forever) {
					oper->o_state = PCP_FAIL;
					return ;
				} 
			}
			tm.tv_sec = oper->interval;
			break;

		case PCP_ACTIVE: /* renew */ 
			{
				int x = 0, y = 0;
				struct timeval now;
				srand((unsigned)time(NULL));
				if (oper->renew_nr == 0) {
					x = oper->lifetime >> 1;
					y = oper->lifetime * 5 >> 3;
				} else if (oper->renew_nr == 1) {
					x = oper->lifetime * 3 >> 2;
					y = x + (oper->lifetime >> 4);
				} else if (oper->renew_nr == 2) {
					x = oper->lifetime * 7 >> 3;
					y = x + (oper->lifetime >> 5);
				}
				gettimeofday(&now, NULL);
				y = rand() % (y-x+1) + x;
				tm.tv_sec = oper->tm.tv_sec + y;
				tm.tv_usec = oper->tm.tv_usec;
				timeval_sub(&tm, &now, &tm);

				/* renewal requests MUST NOT be sent less than four seconds apart */
				if (tm.tv_sec < 4) {
					tm.tv_sec = 4;
					tm.tv_usec = 0;
				}
				break;
			}

		case PCP_FAIL: /* fail */ 
			if (oper->error_code == NOT_RESPONSE) {
				tm.tv_sec = NEXT_RETRANSMISSION;
			}
			else
				tm.tv_sec = oper->lifetime;
			tm.tv_usec = 0;
			break;
		default:
			return ;
	}

	timeval_add(&oper->timer->tm, &tm, &oper->timer->tm);
}


/* result = a + b */
void
timeval_add(a, b, result)
	struct timeval *a, *b, *result;
{
	long l;

	if ((l = a->tv_usec + b->tv_usec) < MILLION) {
		result->tv_usec = l;
		result->tv_sec = a->tv_sec + b->tv_sec;
	}
	else {
		result->tv_usec = l - MILLION;
		result->tv_sec = a->tv_sec + b->tv_sec + 1;
	}
}

/*
 * result = a - b
 * XXX: this function assumes that a >= b.
 */
void
timeval_sub(a, b, result)
	struct timeval *a, *b, *result;
{
	long l;

	if ((l = a->tv_usec - b->tv_usec) >= 0) {
		result->tv_usec = l;
		result->tv_sec = a->tv_sec - b->tv_sec;
	}
	else {
		result->tv_usec = MILLION + l;
		result->tv_sec = a->tv_sec - b->tv_sec - 1;
	}
}

int is_valid_epoch(struct epoch_timer *last, struct epoch_timer *new)
{
	int c_delta = 0;
	int s_delta = 0;
	if (last->lastepoch == 0 && last->lasttime == 0)
		return 1;

	if (new->lastepoch < last->lastepoch)
		return 0;

	c_delta = new->lasttime - last->lasttime;
	s_delta = new->lastepoch - last->lastepoch;

	if (((c_delta + 2) < (s_delta - (s_delta>>4))) ||
			((s_delta + 2) < (c_delta - (c_delta>>4))))
		return 0;
	else
		return 1;
}
