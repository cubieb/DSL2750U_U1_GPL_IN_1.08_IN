/*
  *	3g wan management function
  *	scb+ 2011-3-15
  */
#ifdef SUPPORT_3G_WAN_MANAGEMENT

#define WAN_COND_CHECK_MODEM_READY 					1
#define WAN_COND_CHECK_3G_ENBL				(1<<1)
#define WAN_COND_CHECK_PIN_READY				(1<<2)
#define WAN_COND_CHECK_NETWORK				(1<<3)
#define WAN_COND_CHECK_PING_ENBL				(1<<4)
#define WAN_COND_CHECK_3G_CONN_EXIST			(1<<5)
#define WAN_COND_CHECK_3G_CONN_UP			(1<<6)
#define WAN_COND_CHECK_DEF_IS_3G				(1<<7)
#define WAN_COND_CHECK_ALLOW_MANUAL_DIAL	(1<<8)
#define WAN_COND_CHECK_ALLOW_AUTO_DIAL		(1<<9)
#define WAN_COND_CHECK_IS_MANUAL_DIAL		(1<<10)

#define IPV4_ADDR_SIZE   19
#define DNS_SERVER_SIZE   (4*(IPV4_ADDR_SIZE+1) - 1)

struct  wan_connect {
	char ifname[IFNAME_LEN+1];
	char wan_name[WANNAME_LEN+1];
	char wan_alias[WAN_ALIAS_NAME_LEN+1];
	char wan_nexthop[CDMG_IPV4_ADDR_LEN];

#ifdef PC
	/*the gate way for the interface*/
	char wan_gw_addr[IPV4_ADDR_SIZE+1];
#endif

	char dns_server[DNS_SERVER_SIZE+1];
	wan_status_t status;
	wan_status_t status_ipv6;
	int isdemand;
	int is_add;
	int is_del;

	/*
	* When multi dsl wan, this show the priority for 
	* becoming the default gateway.
	* More small, the proirity is more higher.
	* The highest priority is 1, 0 means ignore.
	*/
	int priority;

	struct  wan_connect *next;
} ;

struct  wan_config {
	UBOOL8 enable;
	UBOOL8 autodial;
	UBOOL8 manualdial;/*only allow manual dial*/
	UBOOL8 demand;
	char nettype[32];
	char username[80];
	char passwd[32];
	char authmethod[32];
	char idletime[32];
	char apn[80];
	char dialnumber[80];
	char dialdelay[32];
	
	/*scb+ 2012-3-28. The time during dsl up to 3g down*/
	char undialdelay[32];	
	char defaultwan[12];
	char mru[8];
	char mtu[8];

	UBOOL8 pingenble;
	char pingintf[32];
	char active_pingintf[32];
	char pingaddr[32];
	char pingperiod[8];
	char pingtimeout[32];
	char pingtolerance[8];

#ifdef SUPPORT_POP_ENTER_PIN_CODE_WEB	
	char lanip[32];
	char lanmask[32];
#endif
};

 typedef struct {
	void *mngr;
	
	struct  wan_config wan_config;	
	DEF_PRI;
} wan_mngr_t;


#ifdef SUPPORT_POP_ENTER_PIN_CODE_WEB	

/*the event allowed by web ctl mechaine */
#define WEB_EVENT_STOP "WEB_EVENT_STOP"
#define WEB_EVENT_DIAL_ALLOW "WEB_EVENT_DIAL_ALLOW"
#define WEB_EVENT_READY "WEB_EVENT_READY"
#define WEB_EVENT_MANUAL_DIAL "WEB_EVENT_MANUAL_DIAL"
#define WEB_EVENT_PIN_READY "WEB_EVENT_PIN_READY"
#define WEB_EVENT_PIN_ERROR "WEB_EVENT_PIN_ERROR"
#define WEB_EVENT_NEED_PIN_CODE "WEB_EVENT_NEED_PIN_CODE"
#define WEB_EVENT_DIAL_WAITING "WEB_EVENT_DIAL_WAITING"
#define WEB_EVENT_DIALING "WEB_EVENT_DIALING"
#define WEB_EVENT_DIAL_FAILE "WEB_EVENT_DIAL_FAILE"
#define WEB_EVENT_DIAL_SUCCESS "WEB_EVENT_DIAL_SUCCESS"
#define WEB_EVENT_CONNECTED "WEB_EVENT_CONNECTED"
#define WEB_EVENT_DELAY_TIMEOUT "WEB_EVENT_DELAY_TIMEOUT"
#define WEB_EVENT_CONNECTED_CONFIRM "WEB_EVENT_CONNECTED_CONFIRM"

typedef enum{
	WEB_STATE_IDLE = 0,
	WEB_STATE_READY,/*3g gongle ready,not need pin code*/
	WEB_STATE_MANUAL_READY,
	WEB_STATE_NEED_PIN_CODE, /*3g dongle is pluged, but need pin code*/
	WEB_STATE_PINREADY,
	WEB_STATE_DIAL_REQ, /*send dial req to ssk, let ssk to send dial cmd to the dial_mngr*/
	WEB_STATE_DIALING,/*start to dial, received the connnecting msg from ssk */
	WEB_STATE_DIAL_FAIL,
	WEB_STATE_DIAL_WAITING,/*when dial faile, system wait some, then start to dial again*/
	WEB_STATE_DIAL_SUCCESS,
	WEB_STATE_CONNECTED,
}	web_state_t;

/*pin manage*/
typedef enum {
	PIN_RULE_EVENT_NONE = 0,
	PIN_RULE_EVENT_NEEDPINCODE,
	PIN_RULE_EVENT_READY,
	PIN_RULE_EVENT_PINREADY,
	PIN_RULE_EVENT_DSL_UP,
	PIN_RULE_EVENT_DSL_DOWN,
	PIN_RULE_ENVENT_PING_FAILSE,
	PIN_RULE_ENVENT_PING_SUCCESS,
	PIN_RULE_EVENT_TRY_UP_3G,
	PIN_RULE_EVENT_USB_UNPLUG,
	PIN_RULE_EVENT_DIAL_REQ,
} web_rule_event_t;
#endif

typedef enum{
	WAN_DIAL_ACT_INIT = 0,
	WAN_DIAL_ACT_DIAL,
	WAN_DIAL_ACT_UNDIAL,
	WAN_DIAL_ACT_SWITCH_DIAL,
	WAN_DIAL_ACT_SWITCH_UNDIAL,
	WAN_DIAL_ACT_WAIT,
	WAN_DIAL_ACT_SET_DIAL_DELAY,
} wan_dial_act_t;

int wan_mngr_init(wan_mngr_t *wan);
wan_mngr_t *wan_mngr_new(void *mn);
int wan_mngr_free(wan_mngr_t *wan);
int wan_mngr_cond_check(unsigned int type);

void  wan_msg_init();
#ifdef SUPPORT_POP_ENTER_PIN_CODE_WEB	
void wan_mngr_check_web_rule(web_rule_event_t event);
#endif
void wan_init();
void wan_mngr_ping_status_change(int is_success);
void wan_mngr_web_rule_for_hotplug_notify(int is_hotplug);
void wan_mngr_dial_for_hotplug_notify(int is_hotplug);
void wan_mngr_conn_for_hotplug_notify(int is_hotplug);
void wan_mngr_dial_decide();
void wan_mngr_dial_set(wan_dial_act_t act, 
				int switch_event_is_up, 
				char *switch_wan_name,
				int is_manual_dial, int dial_delay);

int wan_show_status(modem_t *mdm, char *rbuf, int rlen);
void wan_mngr_set_3g_link_info(wan_status_t status);

void wan_mngr_ping_for_hotplug_notify(int is_hotplug);

#ifdef SUPPORT_POP_ENTER_PIN_CODE_WEB	
void wan_mngr_set_web_ctl_event(const char *event);
#endif

#ifdef	SUPPORT_ALLWAYS_CONNECTED
void wan_mngr_linkdown_dial();
#endif

#endif
