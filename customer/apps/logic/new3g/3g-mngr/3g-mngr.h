#ifndef __3G_MNGR_H_
#define __3G_MNGR_H_

#define HTPLUG_MSG_SIZE 80

typedef struct {
	OBJ_MEMBER(mngr, modem_t, mdm, 0, MNGR_START_CREATE_MODEM, 0);
	OBJ_MEMBER(mngr, at_t, at, 1, MNGR_START_CREATE_AT, 0);
	OBJ_MEMBER(mngr, param_t, par, 2, MNGR_START_CREATE_PAR, 0);
	OBJ_MEMBER(mngr, pin_mngr_t, pin, 3, MNGR_START_CREATE_PIN, 0);
	OBJ_MEMBER(mngr, switch_mngr_t, sw, 4, MNGR_START_CREATE_SW, 0);
	OBJ_MEMBER(mngr, dial_mngr_t, dial, 5, MNGR_START_CREATE_DIAL, 0);

#ifdef SUPPORT_IP_CONNECT_CHECK	
	OBJ_MEMBER(mngr, conn_mngr_t, cnn, 6, MNGR_START_CREATE_CNN, SUPPORT_IP_CONNECT_CHECK);
#endif

#ifdef SUPPORT_POP_ENTER_PIN_CODE_WEB
	OBJ_MEMBER(mngr, web_mngr_t, web, 7, MNGR_START_CREATE_WEB, SUPPORT_POP_ENTER_PIN_CODE_WEB);
#endif


#ifdef SUPPORT_3G_WAN_MANAGEMENT	
	OBJ_MEMBER(mngr, wan_mngr_t, wan, 8, MNGR_START_CREATE_WAN, SUPPORT_3G_WAN_MANAGEMENT);
#endif


	unsigned int who;

	int send_hotplug;
	int send_hotunplug;
	int run_on_diald;
	int call_inner;

	int  do_switch_end;/*indicate have done switch*/

	pid_t this_pid;
	pid_t diald_pid;

	pid_t do_hotplug;
	pid_t do_switch;
	pid_t do_get_modem_info;
	pid_t do_get_status;
	pid_t do_dial_wait;
	pid_t do_connect;/*when dial on demand pppd let diald to chat*/
	pid_t do_diald;/*dial need to chat, it take some time, so separate dial to 2 part*/
	
	int do_hotunplug;	

	char htplug_msg[HTPLUG_MSG_SIZE];
	
	DEF_PRI;
} mngr_t;

extern mngr_t *mn;

#include "3g-mngr-bit.h"

int mngr_init(mngr_t *mn);
void  mngr_start(mngr_t **mn, unsigned int who);
mngr_t *mngr_new(unsigned int who);
int mngr_free(mngr_t *mn);
#endif

