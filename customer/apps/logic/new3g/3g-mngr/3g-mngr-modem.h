#ifndef __3G_MNGR_MODEM_H_
#define __3G_MNGR_MODEM_H_

#define MNGR_MODEM_LEN_TTY_PATH		127
#define MNGR_MODEM_LEN_DEV_PATH		255
#define MNGR_MODEM_LEN_ID_LEN	12

struct csq{
	char rssi[32];
	char ber[32];
} ;

typedef struct { //the dial info come from provider
	char provider_code[80]; //index get from modem by at+cops?
	int idVendor, idProduct; //minor index, one provider will have more apn
	
	char nt[32];
	char provider_name[80];
	char apn[80];
	char number[32];
	char username[80];
	char password[80];	
} provider_t;

typedef enum{
	SEARCH_INVALID = 0,
	SEARCH_FINISHED,
} search_status_t;

typedef struct {
	void *mngr;

	int idProduct;
	int idVendor;	
	search_status_t status;	

	DEF_PRI;
} modem_t;

modem_t	 *modem_new(void *mn);
int modem_init(modem_t *mdm);
int modem_free(modem_t *mdm);

int modem_search(modem_t *m);
int modem_is_exist_tty(modem_t *mdm);


int modem_get_tty_port(modem_t *mdm, char **ports);

int modem_set_ip_port(modem_t *mdm, char *port);
int modem_get_ip_port(modem_t *mdm, char **port);

int modem_set_ctrl_port(modem_t *mdm, char *port);
int modem_get_ctrl_port(modem_t *mdm, char **port);

int modem_get_reg_info(modem_t *mdm, char **info);
int modem_clr_reg_info(modem_t *mdm);
int modem_set_reg_info(modem_t *mdm, const char *reginfo);
int modem_get_pin_info(modem_t *mdm, char **info);
int modem_set_pin_info(modem_t *mdm, const char *pininfo);

int modem_set_pin(modem_t *mdm);
int modem_set_pin_retry_t(modem_t *mdm);

int modem_set_puk(modem_t *mdm);
int modem_set_puk_retry_t(modem_t *mdm);

int modem_set_lock(modem_t *mdm);
int modem_set_lock_retry_t(modem_t *mdm);

int modem_chg_pin(modem_t *mdm);
int modem_chg_pin_retry_t(modem_t *mdm);



int modem_get_serial_id(modem_t *mdm, int *vid, int *pid);
int modem_get_nt(modem_t *mdm, char **nt);
int modem_get_lock_info(modem_t *mdm, char **info);
int modem_get_imsi(modem_t *mdm, char **imsi);
int modem_get_imei(modem_t *mdm, char **imei);
int modem_get_prd(modem_t *mdm, char **prd);
int modem_get_csq(modem_t *mdm, struct csq  **csq);
int modem_get_id(modem_t *mdm, char **identify);
int modem_get_charge(modem_t *mdm, char **charge);
int modem_hangup(modem_t *mdm);
int modem_set_cmee_to_numeric(modem_t *mdm);

int modem_search_provider(modem_t *mdm, provider_t **prd);

int modem_try_chat(modem_t *mdm);
int modem_do_chat(modem_t *mdm, int is_demand_dial);
int modem_is_ready(modem_t *mdm);
int modem_pin_is_ready(modem_t *mdm);
int modem_reg_is_ready(modem_t *mdm);
int modem_wait_search_nt(modem_t *mdm);
int modem_set_network_mode(modem_t *mdm);
int modem_search_init(modem_t *mdm);
int modem_get_lock_retry_time(modem_t *mdm, const char *type, 
							int need_up_date);

void asyn_modem_hangup(modem_t *mdm);
	
void modem_show(modem_t *mdm, char *rbuf, int rlen);
void modem_info(modem_t *mdm);

void modem_show_info(modem_t *mdm, const char *type);
void modem_show_setting_code_remain_time(modem_t *mdm, const char *code_type);
int modem_show_status(modem_t *mdm, char *buf, int rlen);
int modem_get_struct(modem_t *mdm, int action);
int modem_filter_is_allow(const char *idVendor, const char *idProduct);

void modem_read_info(void *par_mngr);
void modem_write_info();
#endif
