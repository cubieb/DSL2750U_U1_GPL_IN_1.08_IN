/**
* scb+ 2011-10-12 
*
* file name: timer.h
*
* implement a timer
*
*/

#define TIMER_TOKEN_LEN 80

struct timer_cplpl {
	time_t expire;
	int tick;

	void *data;
	void (*handler)(void *data);
	
	char token[TIMER_TOKEN_LEN];
	
	struct timer_cplpl *next;	
};

void  cplpl_timer_del_all();

void  * cplpl_timer_set(int tick, const char *token, void (*func)(void *), 
		void *data);
void     cplpl_timer_del(void *data);

struct timeval *  cplpl_timer_handler();
