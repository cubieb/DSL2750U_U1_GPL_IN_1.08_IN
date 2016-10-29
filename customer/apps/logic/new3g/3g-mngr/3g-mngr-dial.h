#ifndef __3G_MNGR_DIAL_H_
#define __3G_MNGR_DIAL_H_


typedef struct {	
	void *mngr;

	int demand_dial;
	int chat_at_pppd;

	int delay;
	

	int undial_flag;
	char dial_opts_buf[CDMG_CMD_LINE_LEN+1];
	int dial_try;
	int dial_argc;
	char *dial_argv[CDMG_ARGS_NUM];

	DEF_PRI;
} dial_mngr_t;





dial_mngr_t *dial_mngr_new(void *mn);
int 	dial_mngr_init(dial_mngr_t *dial);
int 	dial_mngr_free(dial_mngr_t *dial);


int 	dial_mngr_is_busy(dial_mngr_t *dial);
int  	dial_mngr_daemon_dial(int argc, char *argv[]);
int  	dial_mngr_daemon_undial(int argc, char *argv[]);


void dial_mngr_del_timers();
#endif

