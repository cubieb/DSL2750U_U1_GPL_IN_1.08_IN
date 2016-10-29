#ifndef __3G_MNGR_AT_
#define __3G_MNGR_AT_

#define AT_CMD_LEN 256
#define AT_CFG_LINE_LEN 1024
#define AT_CONTENT_LEN 1024
#define AT_TOEKN_LEN 32

typedef enum{
   AT_NONE = -1,
   AT_INIT = 0,
   AT_CONFIG,
   AT_DIAL,
   AT_HANGUP,
   AT_END,

   /*point out the tty name for ip traffic*/	
   AT_IP_TTY,

   /*
   * scb+ 2012-1-12 
   * point out the tty name for control traffic
   * some dongle must send the pin manage AT command to the control 
   * tty.
   */	
   AT_CTL_TTY,   

   /*the delay between two at command*/
   AT_CMD_DELAY,

   /*the following funcs help to control the modem*/
   AT_GET_PRVDR,  //get the privider.
   AT_GET_PRVDR_PATTN, //the result pattern of the 

   AT_SEARCH_INIT,/*scb+ 2011-12-3 done befor search network*/

   AT_PHONE_INIT,/*scb+ 2012-2-17 do some at command before do any other at command*/

   AT_SET_NETWORK_MODE,/*scb+ 2012-2-18 set the dongle to work at 3g mode*/

   AT_GET_REG,
   AT_GET_REG_PATTN,

   AT_GET_CSQ,
   AT_GET_CSQ_RSSI_PATTN,
   AT_GET_CSQ_BER_PATTN,


   AT_GET_ID,
   AT_GET_ID_PATTN,

   AT_GET_IMEI,
   AT_GET_IMEI_PATTN,
   
   AT_GET_CHARGE,
   AT_GET_CHARGE_PATTN,

   /*pin management */
   AT_GET_PIN,  //need pin, ready or puk
   AT_GET_PIN_PATTN,
   
   AT_SET_PIN,
   AT_SET_PIN_RETRY_T,
   AT_SET_PIN_RETRY_T_PATTN,

   AT_SET_CMEE_TO_NUM_MODE,

   AT_CHG_PIN,
   AT_CHG_RETRY_T,
   AT_CHG_RETRY_T_PATTN,
   
   AT_SET_PUK,
   AT_SET_PUK_RETRY_T,
   AT_SET_PUK_RETRY_T_PATTN,

   AT_SET_LOCK,
   AT_SET_LOCK_RETRY_T,
   AT_SET_LOCK_RETRY_T_PATTN,   
   
   AT_GET_LOCK, //lock or unlock   
   AT_GET_LOCK_PATTN,

   AT_GET_IMSI,
   AT_GET_IMSI_PATTN,

   AT_GET_LAC,
   AT_GET_LAC_PATTN,

   AT_GET_CELLID,
   AT_GET_CELLID_PATTN,   
   
   AT_CMD_END, //must at the end.
} at_cmd_t;


#define AT_NAME_LEN 80
#define NET_TYPE_NAME_LEN 32

typedef struct {//the at function relative with the modem.
   char name[AT_NAME_LEN];	//description info, index

   char *ids; //format:(vid0,pid0),(vid1,pid1),...
   char nt[NET_TYPE_NAME_LEN]; //index, when idVendor,idProduct not mach see this to get a default.

   char *atcmd[AT_CMD_END];
} AT_func_t;

typedef struct {
	void *mngr;
	DEF_PRI;
} at_t;

#if 0
/**except 3g-mngr-at.c, do not call the AT macros*/

int __at_cmd(at_t *at, int fd, const char *at_str, char *rbuf, int rlen);
int __at_update(at_t *at);
int __at_load_config(at_t *at);

#define GET_AT(at, func, r)	\
	do { \
		__at_load_config((at));	\
		__at_update(at);	\
		memset((r), 0, AT_CMD_LEN);	\
		if (GET_PRI((at), __at_t)->at_special.atcmd[func])	\
			strcpy((r), GET_PRI((at), __at_t)->at_special.atcmd[func]);	\
		else if (GET_PRI((at), __at_t)->at_nt_defalt.atcmd[func])	\
			strcpy((r), GET_PRI((at), __at_t)->at_nt_defalt.atcmd[func]);	\
		else	if ( GET_PRI((at), __at_t)->at_normal_default.atcmd[func]) 	\
			strcpy((r), GET_PRI((at), __at_t)->at_normal_default.atcmd[func]);	\
	} while(0)
	
#define CALL_AT(at, func) \
	({	\
		char atbuf[AT_CMD_LEN] = {0};	\
		GET_AT((at), func, GET_PRI((at), __at_t)->tmp_atbuf);	\
		__at_cmd((at), GET_PRI((at), __at_t)->fd, GET_PRI((at), __at_t)->tmp_atbuf, 0, 0);	\
	})

#define CALL_AT1(at, func, rbuf, rlen) \
	({	\
		GET_AT((at), func, GET_PRI((at), __at_t)->tmp_atbuf);	\
		__at_cmd((at), GET_PRI((at), __at_t)->fd , GET_PRI((at), __at_t)->tmp_atbuf, rbuf, rlen);	\
	} )




/*the following macro or functions can be call at other file except 3g-mngr-at.c*/
#define  at_do_cmd(at, cmd, retry, cmd_pattn, rb)	 \
({	\
	int i = 0;	\
	int ret = 0;	\
	DEB_EN();	\
	for (i = 0; i <= retry; i++) 	\
		if ((ret = CALL_AT1(cmd, GET_PRI((at), __at_t)->tmp_rbuf, 128)) == 0)	\
			break;	\
	if (i == 	retry + 1) {	\
		d_printf("at cmd failure:%d\n", cmd);	\
		ret = -1;	\
	}		\
	if (cmd_pattn != AT_NONE) {			\
		GET_AT(cmd_pattn, GET_PRI((at), __at_t)->tmp_reg);		\
		lib3g_reg_exec(GET_PRI((at), __at_t)->tmp_rbuf, GET_PRI((at), __at_t)->tmp_reg, rb);		\
		if (debug)	{\
			printf_3g("", "pattn:\n");	\
			at_ret_print(GET_PRI((at), __at_t)->tmp_reg[0]?GET_PRI((at), __at_t)->tmp_reg:"none");	\
			printf("reg result:");	\
			at_ret_print((rb));	\
		}\
			\
	}		\
	DEB_DIS();	\
	ret;	\
})

#define at_parse(at, pattn, str, rbuf) \
	do {		\
		GET_AT((at), pattn, GET_PRI((at), __at_t)->tmp_reg);		\
		d_printf("get a reg:\n[%s]\n", GET_PRI((at), __at_t)->tmp_reg);	\
		lib3g_reg_exec(str, GET_PRI((at), __at_t)->tmp_reg, rbuf);	\
	}while(0)

#endif

at_t *at_new(void *mn);
int at_init(at_t *at);
int at_free(at_t *at);
int at_get_item(at_t *at, at_cmd_t item, char *rbuf, int rlen);
int at_do_cmd(at_t *at, at_cmd_t cmd, int retry,  at_cmd_t cmd_pattn, char *rb, int rlen);
int at_parse(at_t *at, at_cmd_t pattn, char *str, char *rbuf, int rlen);
int at_set_tty(at_t *at, const char *port);
int at_cmd_free(at_t *at);
int at_config_free(at_t *at);
int at_config_load(at_t *at);
int at_cmd_load(at_t *at);
int at_product_chat_script(at_t *at, int argc, char *argv[]);
int at_item_get_and_check_exist(at_t *at, at_cmd_t item, char *rbuf, int rlen);
void  at_set_delay(at_t *at);

char *at_cmd_to_str(at_cmd_t item);

#endif

