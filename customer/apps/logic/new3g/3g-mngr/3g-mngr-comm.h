#ifndef __3G_MNGR_COMM_H_
#define __3G_MNGR_COMM_H_

/*status string macro*/
#define STATUS_NO_USB    "NO USB CARD"
#define STATUS_SWITCHING  "switching... please wait for several minutes!"
#define STATUS_INIT  "init modem... please wait for several minutes!"

#define REG_INFO_NOT_REG    "NETWORK NOT REGISTERED!"
#define REG_INFO_REG_LOCAL		"NETWORK REGISTERED, HOME NETWORK!"
#define REG_INFO_SEARCH	"SEARCHING NETWORK!"
#define REG_INFO_REG_DENY "NETWORK REGISTRATION DENIED"
#define REG_INFO_UNKOWN  "UNKOWN NETWORK"
#define REG_INFO_REG_ROAM "NETWORK  REGISTERED ROAMING"
#define REG_INFO_NOT_FIND    "NETWORK NOT FIND!"
#define REG_INFO_SEARCH_START	"START SEARCHING NETWORK..."
/*scb+ 2011-12-4 means not need to search network*/
#define REG_INFO_INGNORE	"NETWORK SEARCH IGNORE"

#define PIN_INFO_READY "PIN READY"
#define PIN_INFO_PIN "NEED PIN CODE!"
#define PIN_INFO_INVALID "SIM CARD INVALID OR NO SIM CARD!"
#define PIN_INFO_PUK "NEED PUK CODE!"

#define LOCK_INFO_LOCK "lock enable"
#define LOCK_INFO_UNLOCK "lock disable"

#define PIN_CODE_RET_PIN_ERR "PIN CODE ERROR"
#define PIN_CODE_RET_PUK_ERR "PUK CODE ERROR"

#define AT_CMD_ERR "action not success!"

#define IP_CONN_CHECK_MSG "ipconect"


#define WAN_3G_CONNECTTED "CONNECTTED"
#define WAN_3G_DISCONNECTTED "DISCONNECT"
#define WAN_3G_CONNECTING     "Connecting"
#define WAN_3G_INALID "invalid"
#define WAN_3G_DIALING "dialing..."
#define WAN_3G_DIALWAITING "dial waiting..."
#define WAN_3G_UNDIALING "undialing..."
#define WAN_3G_MANUAL_DIAL "Manual Dialed"
#define WAN_3G_AUTO_DIAL "Auto Dialed"
#define WAN_3G_DIAL_ON_DEMAND "dial on demand"

#define MNGR_MSG_LINKUP	"linkup"
#define MNGR_MSG_LINKDOWN  "linkdown"
#define MNGR_MSG_UNDIALOK  "undialok"
#define MNGR_MSG_DEMANDOK  "demandok"
#define MNGR_MSG_DEMANDDOWN  "demanddown"
#define MNGR_MSG_DEMANDFAIL  "demandfail"
#define MNGR_MSG_DIALFAIL  "dialfail"
#define MNGR_MSG_DIALWAITING  "dialwaiting"
#define MNGR_MSG_DIALAGAIN  "againdial"
#define MNGR_MSG_HOTUNPLUG  "hotunplug"
#define MNGR_MSG_READY  "ready"
#define MNGR_MSG_WEBMSG   "webmsg"
#define MNGR_MSG_NTWKCHANG   "ntwchange"
#define MNGR_MSG_GETCONNS   "getconns"
#define MNGR_MSG_NEEDPIN	"needpincode"
#define MNGR_MSG_NEEDPUK	"needpukcode"
#define MNGR_MSG_INVALID	PIN_INFO_INVALID

#if defined(TBS)
#define MNGR_MSG_LINKUP_FORMAT	"linkup dnsserver=%s %s sessid=%s"
#else
#define MNGR_MSG_LINKUP_FORMAT	"linkup dnsserver=%s,%s"
#endif

#include "lib/3g-lib.h"
#include "module/demand_hook.h"

/*If enable ping check, but no ping address, using this*/
#define PING_CHECK_DEFAULT_ADDR "8.8.8.8"

#define IFNAME_LEN	31
#if defined(TBS) || defined(PC)
#define WANNAME_LEN	255
#else
#define WANNAME_LEN	79
#endif
#define WAN_ALIAS_NAME_LEN 127

#ifdef BRCM
#define IF_3G_NAME "ppp3g0"
#define PPPD_NAME "pppd245"
#else
#define IF_3G_NAME "ppp10"
#define PPPD_NAME "pppd"
#endif

#ifndef EXTERN_USE_3G_MNGR_H

#define OBJ_DEBUF_LOG_CTRL_FILE "/var/obj_debug"

#define DELAY_DO_SWITCH_MAX	20
#define DELAY_DO_HOTPLUG_MAX	20




#define obj_printf(args...)	\
	do {		\
		if (access(OBJ_DEBUF_LOG_CTRL_FILE, F_OK) == 0)		\
			printf_3g("", args);	\
	} while(0)
		
/*
	macro 'OBJ_MEMBER' is used by 3g-mngr.sh to generate init code
	'profile' means the condition compile macro, 0 is no condition compile 
	'bit', 'bit_name' are used to notify if need to create the sub object when the father object is created.
*/		
#define OBJ_MEMBER(obj, m_type, m_name, bit, bit_name, profile)  \
			m_type *m_name

#define PRI_ADDR(type,  addr)    \
			(void *)((unsigned int)addr + \
			(unsigned int)sizeof(type) + \
			(unsigned int)4)

#define GET_PRI(addr,  priv_type) \
			((priv_type *)((addr)->data))

#define SIZE_3G(type,  priv_type)  \
			(sizeof(type) + sizeof(priv_type) + 4)

#define INIT_3G(addr,  content,  type,  priv_type) \
        do{     \
                memset((addr), (content), SIZE_3G(type, priv_type)); \
                (((type *)(addr))->data) = (void *)PRI_ADDR(type, (addr));  \
                obj_printf(#addr"(0x%x)'s priv data is 0x%x\n", \
                	(unsigned int)(addr), \
                	(unsigned int)(((type *)(addr))->data));	\
        } while(0)

#define ALLOC_3G(type, priv_type)                  \
	({ 	type * r;	\
		r = (type*)malloc(SIZE_3G(type, priv_type));	\
			\
		if (r)	\
			INIT_3G(r, 0, type, priv_type);	\
			\
		obj_printf("alloc "#type" at 0x%x\n", (unsigned int)r);	\
		r;	\
	})
	
#define DEF_PRI         void *data

/*get the mngr member of the struct*/
#define MN(addr) 	\
	({ \
		mngr_t *r = 0; 	\
						\
		if (!(addr))	\
			obj_printf(#addr" is 0!!!!!!!!!!!!!!!!!!!!!!!!\n");	\
		else	{	\
			r = (mngr_t *)addr->mngr; \
			obj_printf("the mngr of "#addr"(0x%x) is 0x%x\n", \
				(unsigned int)addr, (unsigned int)r);\
		}	\
				\
		r;	\
	})

		
#define MN_SET(addr, __mngr)  \
	do{	\
		(addr)->mngr = (void *)(__mngr); \
		obj_printf("set "#addr"(0x%x)'s mngr to "#__mngr"(0x%x)\n", \
			(unsigned int)addr, (unsigned int)__mngr);\
	}while(0)


#define SUB_INIT(sub, sub_type, sub_priv_type)	\
	do{	\
		if (!(sub))	{	\
			obj_printf(#sub" is 0 !!!!!!!!!!!!!!!!!!!!!!!\n");	\
		} else {\
			mngr_t *mn = 0;	\
					\
			mn = MN(sub);	\
			if (!mn) 	\
				obj_printf(#sub"'s father is 0\n");	\
					\
			INIT_3G	(sub, 0, sub_type, sub_priv_type);	\
					\
			MN_SET(sub, mn);	\
		}	\
	}while(0)
	
#define CHAT_SCRIPT_PATH  WORK_PATH"chat.script"
#define PPP_CONFIG WORK_PATH"pppd.cfg"
#define CHAP_SECRET "/var/chap-secrets"
#define PPPD_CMD PPPD_NAME" file "PPP_CONFIG
#define DNS_FILE WORK_PATH"dns"
#define MSG_UNIX_CONNECT_PATH  WORK_PATH"unix_socket_3g_msg_connect"
#define MSG_UNIX_TIMER_PATH  WORK_PATH"unix_socket_3g_msg_timer"

#define USB_SWITCH_CMD  "usb_modeswitch"
#define USB_SWITCH_CFG_TMP "/var/3gppp/.usb_switch"

#define USB_INFO_BUS_DEVICES_PROC_FILE "/proc/bus/usb/devices"


/*************************************************************************/

#define ARGS_NUM	CDMG_ARGS_NUM
#define ARGS_BUF_SIZE	MNGR_LEN_CMD

#define FUNC(func, s) 	CDMG_FUNC(func, 0, 0, 0, 0, 0, 0, s)

#define CALL(func) CDMG_N_TO_F(func)


#define DIALD_RUN_CHECK()	\
		do {	\
			if (mn && mn->this_pid && mn->diald_pid &&	\
					mn->this_pid == mn->diald_pid)	\
				printf_3g("", "*****NOTE: RUNNING  AT DIALD,"\
							"THIS ACTION TAKE UP MORE TIME, SHALL NOT BEEN DONE"	\
						  	"THIS IS A BUG, PLS FIX IT!!*****\n");	\
		}	while(0)



/**********************************************************************/
extern char g_3g_ifname[IFNAME_LEN+1];
extern char g_3g_wan_name[WANNAME_LEN+1];			
extern int stop_msg_process;

void 	signal_init();
void 	wait_sub_process();
void 	signal_handler_set(int sig, void (*handler)());

pid_t 	fork_3g();
#endif
#endif

