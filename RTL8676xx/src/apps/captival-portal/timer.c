/**
* scb+ 2011-10-12 
* file name: timer.c
*
* implement a timer
*
*/
#include "captival-portal.h"

static struct timeval timeout;
static struct timer_cplpl	*timer_list;
static char  *cplpl_timer_show(struct timer_cplpl *tr, char *buf, int len);
	
#define timer_printf(tr, args...)	\
	do{	\
		char __buf1__[80] = {0};	\
		char __buf2__[80] = {0};	\
				\
		snprintf(__buf1__, sizeof(__buf1__), 	##args);	\
		cplpl_timer_show((tr), __buf2__, sizeof(__buf2__));	\
		d_printf("%s%s\n", __buf1__, __buf2__);	\
	}while(0);

/*do the timeout timer's function and get the next timeout timer*/	
struct timeval *  cplpl_timer_handler()
{	
	time_t curr;
	struct timer_cplpl  *tr;

	time(&curr);
	
	while (timer_list) {
		timer_printf(timer_list, "Checkint timer,for timer:\n");

		if (timer_list->expire > curr)
			break;

		tr = timer_list;
		timer_list = tr->next;

		timer_printf(tr,"Time Out:");
		
		if (tr->handler) {			
			d_printf("do the handler 0x%x\n", 
				(unsigned int)tr->handler);
			tr->handler(tr->data);
		}

		free(tr);		
	}

	if (timer_list) {
		timer_printf(timer_list, "The next expired timer:\n");
		timeout.tv_sec = timer_list->expire - curr;
		timeout.tv_usec = 0;
		return &timeout;
	} else
		return 0;

}

/*delete a timer*/
void cplpl_timer_del(void *data)
{
	struct timer_cplpl *tm = (struct timer_cplpl *)data;

	d_printf("enter\n");
	if (!tm)
		return;
	
	d_printf("Try to Delate timer:[%s]\n", tm->token);
	CDMG_LIST_DEL(struct timer_cplpl, timer_list, tm);
	timer_printf(tm, "DELETE  timer:\n");
	free(tm);	
}

void cplpl_timer_del_all()
{
	struct timer_cplpl  *tr;

	while (timer_list) {
		tr = timer_list;
		timer_list = timer_list->next;
		timer_printf(tr, "DELETE  timer:\n");
		free(tr);
	}
}

void  * cplpl_timer_set(int tick, const char *token, 
						void (*func)(void *), void *data)
{
	time_t curr = 0;
	struct timer_cplpl  *tr = 0;

	d_printf("enter tick=%d token=%s func=0x%x\n",
		tick, token, (unsigned int )func);

	if (tick == 0)
		return 0;
	
	CDMG_MALLOC(sizeof(struct timer_cplpl), tr, return 0);

	snprintf(tr->token, sizeof(tr->token), "%s", token);
	
	if (data == 0)
		tr->data = (void *)tr;
	else
		tr->data = data;

	tr->handler = func;	
	tr->tick = tick;
	
	time(&curr);
	tr->expire = curr + tr->tick;
	timer_printf(tr, "ADD timer:\n");
	CDMG_LIST_ADD_IN_ORDER(struct timer_cplpl, timer_list, tr, expire, 1);

	return tr;
}

/*show the timer info*/
static char  *cplpl_timer_show(struct timer_cplpl *tr, char *buf, int len)
{
	time_t curr;

	if (!buf || len <= 0)
		return 0;

	time(&curr);
	
	memset(buf, 0, len);
	snprintf(buf, len, 
		"[TIMER:0x%x Tn:<%s> Tk:%d Ep:%d Ct:%d Rmt:%d Fn:0x%x]",
		(unsigned int)tr,
		tr->token, 
		tr->tick, 
		(unsigned int)(tr->expire), 
		(unsigned int)curr,
		(unsigned int)(tr->expire - curr),
		(unsigned int)tr->handler);
	
	return buf;
}

