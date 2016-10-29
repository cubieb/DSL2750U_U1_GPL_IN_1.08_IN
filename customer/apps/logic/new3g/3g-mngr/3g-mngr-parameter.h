#ifndef __3G_MNGR_PARAMETER_H_
#define __3G_MNGR_PARAMETER_H_

#define	__PAR_MAP_SIZE 35

typedef struct {
	void *mngr;
	char nt[32];	
	DEF_PRI;
} param_t;

param_t *param_new(void *mn); 
int param_init(param_t *par);
int param_free(param_t *par);
int param_get(param_t *par);
int param_get_item(param_t *par, const char *name, char *rbuf);
int param_show_default(param_t *par,  char *rbuf, int rlen);
int param_get_from_user(param_t *par, int argc, char *argv[]);
int param_product_pppd_cfg(param_t *par, int chat_at_pppd);
int param_get_default(param_t *par);
void param_show(param_t *param);
int param_replace_var(param_t *param, char *at_str, int len);
void param_write_par(param_t *par);
void param_read_par(param_t *par);
void param_for_hotplug_notify(int is_hotplug);

#endif

