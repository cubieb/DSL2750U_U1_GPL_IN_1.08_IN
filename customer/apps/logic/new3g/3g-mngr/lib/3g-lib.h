#ifndef __3G_LIB_H_
#define __3G_LIB_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>

#include "3g-lib-cmd-msg.h"

#include "3g-lib-timer.h"

#ifndef EXTERN_USE_3G_MNGR_H
#include <ctype.h>
#endif

#define READ_BUF_SIZE 1024
#define BUFSIZE 1024

#define REG_STR_BUF_SIZE 2048
#define REG_EXPR_BUF_SIZE 2048

#define SWITCH_CFG_LINE_SIZE 1024
#define USB_ID_SIZE 5

/*The bufer length macro*/
#define MNGR_LEN_CMD	CDMG_CMD_LINE_LEN
#define MNGR_LEN_ARG	128
#define MNGR_LEN_REG_BUF_LEN	REG_EXPR_BUF_SIZE


#define WORK_PATH "/var/3gppp/"

#define RESOLV_CONF_PATH "/etc/resolv.conf"
#define MSG_UNIX_SEND_PATH   WORK_PATH"unix_socket_3g_msg_notify"
#define MSG_UNIX_RECEVE_PATH   WORK_PATH"unix_socket_3g_msg_start"
#define MSG_UNIX_DAEMON_PATH   WORK_PATH"unix_socket_3g_msg_daemon"


#define DEV_DIR  "/var/dev"
//#define USB_S_DEV_PATH "/sys/bus/usb-serial/devices"
#define USB_S_DEV_PATH "/sys/class/tty"
#define CDROM_DEV DEV_DIR"/sr0"

#define MSG_LEN CDMG_MSG_LEN

struct modem_3g_info {
	char magic[3];
	int have_modem;	
	int pin_ready;
	int pin_enter_retry_time;
	int puk_enter_retry_time;
	int pin_change_retry_time;
	int lock_change_retry_time;	
	char reg_ready;
	
	int ping_enable;
	int ping_start;
	int ping_for_success;
#ifdef SUPPORT_POP_ENTER_PIN_CODE_WEB	
	int web_state;
#endif
	
};

struct  dec_map {
                char org[10];
                char code[10];
};

 typedef enum {
	WAN_S_DISCONNECTED = 0,
 	WAN_S_CONNECTED,		
	WAN_S_CONNECTING,
	WAN_S_DISCONNECTING,
	WAN_S_DIALING = 100,
	WAN_S_UNDIALING,
	WAN_S_DIALWAITING,
	WAN_S_INVALID,
 } wan_status_t;


int	 __lib3g_script_and_get_ret(char *script, char *buf, int len);
int 	lib3g_init_mobile_msg_recever_socket(const char *path);
int 	lib3g_script_and_get_ret(char *script, char *buf);
int	lib3g_get_modem_info(struct modem_3g_info *info);

void cmd_3g(const char *cmd, char *rbuf, int len);
void lib3g_change_string(struct dec_map *map, int isEncode, 
		char *content);

pid_t* lib3g_find_pid_by_name( char* pidName);

wan_status_t	lib3g_change_wan_status(void *special_status, 
				int solusion);

#define IS_MNGR_DEBUG(f) \
	({	\
		int ret = 0;	\
			\
		if (access ((f), F_OK) == 0)	\
			ret = 1;	\
		ret;		\
	})

#ifndef EXTERN_USE_3G_MNGR_H

#define DAIL_DEBUF_CONTR_FILE "/var/3g_debug"

/* 用自己的函数替换掉库函数*/
time_t my_time(time_t *t);
#define time(a) my_time(a)

/*at different file, this macro can be refine */
#define DIAL_DEBUG_LOCAL_TAG_FILE ""

int is_3g_debug_local(const char *token1, const char *token2, 
		const char *file, const char *func);

#define is_debug() is_3g_debug_local(DAIL_DEBUF_CONTR_FILE, 0, 0, 0)
		
#define printf_3g(token, fmt,args...) \
		do {\
			printf("[%d %s %s:%d %s()]:" fmt, \
				getpid(), token, __FILE__, __LINE__, __FUNCTION__, ##args);\
		}while(0)
	
#define debug_3g(tag_file, token, args...) \
	do{	\
		if (is_3g_debug_local(tag_file, DAIL_DEBUF_CONTR_FILE, \
				__FILE__, __FUNCTION__)) \
			printf_3g(token, args);\
	}while(0)

#define ERR(args...) printf_3g("", ##args)
#define d_printf(args...) debug_3g("", "", args)


#define d_perror(fmt,s...) d_printf(fmt ":%s\n", ##s, strerror(errno))
#define perror(fmt,s...) printf_3g("", fmt ":%s\n", ##s, strerror(errno))

/*generate by Makefile, to define ID_CONFIG_FILE and MODULE_FILE*/
#include "../modem_def.h"

#endif	



#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))

void lib3g_get_kernel_ver(int *version, int *patchlevel, int *sublevel, 
			char *total_ver, int total_ver_len);

#define SCRIPT_CMD_SIZE MNGR_LEN_CMD
extern char lib3g_script_cmd[SCRIPT_CMD_SIZE+1];

#define lib3g_fmt_script_r(rbuf, len, fmt, args...)	\
	( {	\
		snprintf(lib3g_script_cmd, SCRIPT_CMD_SIZE, fmt, ##args);	\
		d_printf("CMD:%s\n", lib3g_script_cmd);					\
		__lib3g_script_and_get_ret(lib3g_script_cmd, (rbuf), (len));	 \
	})
	
#define lib3g_fmt_script(fmt, args...) \
	( {	\
		snprintf(lib3g_script_cmd, SCRIPT_CMD_SIZE, fmt, ##args);	\
		d_printf("CMD:%s\n", lib3g_script_cmd);					\
		__lib3g_script_and_get_ret(lib3g_script_cmd,0, 0);	 \
	})
	
#define lib3g_fstrncat(d , n, args...)	cdmg_fstrncat(d, n, args)
	
#define	MNGR_LEN(r)			(sizeof(r)-1)
#define   MNGR_GET_ARG_LEN(n, r, l)	cdmg_get_arg(argv, n, r, l)
#define   MNGR_GET_ARG(n, r)		MNGR_GET_ARG_LEN(n, r, MNGR_LEN(r))

int 	__lib3g_serial_send_recv(int fd, int not_change_attr, 
		char *send, int send_len, 
		char *recv_buf, int recv_len, 
		int timeout_s, int timeout_us, 
		int wait_s, int wait_us,
		int exit_func(char *r_buf, int r_len, void *, char *o_buf, int *io_buf), void *exit_data);

int 	lib3g_serial_send_recv(const char *dev, 
		char *send, int send_len, 
		char *recv_buf, int recv_len, 
		int timeout_s, int timeout_us, 
		int wait_s, int wait_us,
		int exit_func(char *r_buf, int r_len, void *, char *o_buf, int *io_buf), void *exit_data);

int 	lib3g_is_support_at_cmd(char *tty_dev);
int 	lib3g_at_is_end(const char *buf, int byte);
int 	lib3g_at_is_success(const char *buf, int byte);
int 	lib3g_eject(const char *device, int do_flags);
int 	lib3g_system (char *command, int is_wait);
int 	lib3g_reg_exec(char *str, char *regstr, char *rbuf, int r_len);
int 	lib3g_read_file(const char *file, char *buf, int buf_l);
int 	lib3g_write_file0(const char *file, char *buf, int len);
int 	lib3g_write_file(const char *file, char *buf);
int 	lib3g_file_copy(const char *f1, const char *f2);
int 	lib3g_run_script(const char *pragram, int infd, int outfd, int errfd);
int 	lib3g_set_dns(const char * dns1, const char *dns2, FILE *fp);
#ifdef FIND_TTY_PORT_BY_INT_EP
int 	lib3g_have_int_ep(const char *tty);
#endif
int 	lib3g_send_mobile_msg(const char *path, const char *msg);
int 	lib3g_send_mobile_chage_msg(const char *msg);
int 	lib3g_chat_file(const char *file, const char *tty, const char *log_file);
int 	lib3g_scsi_sg_set_cmd(int fd, int num,  uint8_t *cmd);
int 	lib3g_is_def_gw(const char *ifname);


void 	lib3g_msg_process_loop(const char *unix_sok_path, 
			int usleep_v, 	const char *token, 
			void (*handler)(char *msg),
			struct timeval * (*timeout_func)(void));

void 	lib3g_mkttynod(const char * name, char *dev_path,
			int *maj, int *min);
void		lib3g_strl2u(char *s);
void 	lib3g_strdelblank(char *s);
void 	lib3g_strdelCR(char *s);
void 	lib3g_at_ret_print(char *src);
void 	lib3g_install_modules();
void 	lib3g_serial_attr_change_for_dongle(int fd, int is_restore);

char *	lib3g_strncpy(char *dest, int dlen, const char *src, int slen);
char *	lib3g_at_strerror(const char *err_raw_info, int *r_errorn);
#endif
