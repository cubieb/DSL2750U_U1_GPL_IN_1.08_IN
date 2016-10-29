#ifndef __3G_MNGR_SWITCH_H_
#define __3G_MNGR_SWITCH_H_
typedef struct{
	modem_t *mdm;
	void *mngr;
	DEF_PRI;
} switch_mngr_t;

switch_mngr_t *switch_mngr_new(void *mn);
int switch_mngr_init(switch_mngr_t *sw);
int switch_mngr_free(switch_mngr_t *sw);

int switch_mngr_switch_for_modem(switch_mngr_t *sw, int agrc, char *argv[]);
int switch_mngr_switch(switch_mngr_t *sw, int agrc, char *argv[]);
void switch_mngr_switch_fixup(switch_mngr_t *sw);
#endif
