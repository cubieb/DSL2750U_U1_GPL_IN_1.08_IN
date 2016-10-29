#ifndef  __3G_LIB_TIMER_H__
#define __3G_LIB_TIMER_H__

#define TIMER_TOKEN_LEN	128
struct timer_3g {
	time_t expire;
	int tick;

	void *data;
	void (*handler)(void *data);
	
	char token[TIMER_TOKEN_LEN];
	
	struct timer_3g *next;
	
};

char  	*timer_3g_show(struct timer_3g *tr, char *buf, int len);

void 	timer_del_all();
void 	timer_add(struct timer_3g *tm);
void 	timer_del (struct timer_3g *tm);
void 	timer_del_by_token(const char *token);
void 	timer_token_set(int tick, const char *token);

struct timeval 	*timer_handler();
struct timer_3g  	*timer_set(int tick, const char *token, 
					void (*func)(void *), void *data);
struct timer_3g	*timer_get(const char *token);
#endif
