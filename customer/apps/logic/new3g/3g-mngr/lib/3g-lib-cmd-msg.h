/**
* scb+ 2011-8-31
* impliment the command and message mechanise
*/
#ifndef __3G_LIB_CMD_MSG_H_
#define __3G_LIB_CMD_MSG_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define CDMG_MSG_LEN	(1024*4-1)
#define CDMG_NAME_LEN   31
#define CDMG_CMD_LINE_LEN	1024
#define CDMG_IPV4_ADDR_LEN 24
#define CDMG_IP_ADDR_LEN 128
#define CDMG_ARGS_NUM 64


#define CDMG_GET_DEF_TIMEOUT 5

/************************************************************
*															*
*            The running environment setting start						*
*															*
*************************************************************/
#define CDMG_DEBUG_FLAG_FILE "/var/cdmg_debug"

#define CDMG_WORK_PATH   "/var/3gppp/"

#define CDMG_DAEMON_ADDR CDMG_WORK_PATH"unix_socket_3g_msg_daemon"

#define CDMG_GET_PATH_PRE CDMG_WORK_PATH"cdmg_get_path_"

#define  CDMG_GET_ACT_TMP_FILE	CDMG_WORK_PATH".cdmg_tmp"

#define  cdmg_printf(fmt ,args...)	\
	printf("[%-6d %-10s:%5d]: " fmt, getpid(), __FILE__, __LINE__, ##args)
	
#define  cdmg_debug(args...) 	\
	do {		\
		if (access(CDMG_DEBUG_FLAG_FILE, F_OK) == 0)	\
			cdmg_printf(args);	\
	}while(0)

#define cdmg_perror(args) \
	do {		\
		if (access(CDMG_DEBUG_FLAG_FILE, F_OK) == 0)	\
			perror(args);	\
	}while(0)

/*check if is debug*/	
#define cdmg_is_debug() \
	({	\
		int ret = 0;	\
					\
		if (access(CDMG_DEBUG_FLAG_FILE, F_OK) == 0)	\
			ret = 1;		\
		ret;					\
	})

/************************************************************
**															*
**				List operation									*
**															*
************************************************************/
#define	CDMG_LIST_ADD(type, head, node)		\
	({										\
		((type *)(node))->next = (type *)(head);	\
		(head) = (node);						\
	})

#define CDMG_LIST_DEL(type, head, node)	\
	({									\
		type *__prev, *__tmp, *__data;			\
						\
		if (!(head))		\
			__data = 0;		\
		else if (!((type *)(head))->next) {	\
			if ((head) == (node)) {		\
				(head) = 0;				\
				__data = (type *)(node);	\
			}							\
			__data = 0;						\
		}								\
		else if ((head) == (node))	{		\
			(head) = ((type *)(head))->next;	\
			__data = (node);				\
		} else {							\
			__data	=	0;					\
			__prev = (type *)(head);			\
			__tmp = ((type *)(head))->next;	\
			while (__tmp)	{				\
				if (__tmp == (type *)(node)) {\
					__data = __tmp;			\
					__prev->next = __tmp->next;	\
					break;				\
				}						\
				__prev = __tmp;				\
				__tmp = __tmp->next;			\
			}							\
		}								\
		__data;							\
	})

#define CDMG_LIST_ADD_IN_ORDER(type, head, node, w, up)	\
	({									\
		type *__prev, *__tmp;					\
		type *__h = (type *)(head);			\
		type *__d = (type *)(node);			\
		int __is_up = (up) ? 1 : 0;			\
										\
		if (!__h)	{						\
			(head) = __d;					\
		} 								\
		else if (!(__h->next))				\
		{								\
			if ((__is_up && __h->w > __d->w) ||	 \
					(!__is_up  && __h->w < __d->w)) { \
				__d->next = __h;				\
				(head) = __d;				\
										\
			} else {						\
				__d->next = 0;				\
				__h->next = __d;						\
			}									\
		}										\
		else		 								\
		{										\
			__prev = (type *)(head);					\
			__tmp = __prev->next;						\
												\
			if ((__is_up && (__d->w < __h->w)) || 		\
					(!__is_up && (__d->w > __h->w)) )	\
			{									\
				__d->next = __h;						\
				(head) = __d;						\
			} 									\
			else 								\
			{									\
				int __add = 0;						\
				while(__tmp)						\
				{								\
					 if ((__is_up && (__prev->w <= __d->w && 			\
						 			__d->w < __tmp->w))	|| 		\
						 		(!__is_up && (__prev->w >= __d->w && \
						 			__d->w > __tmp->w))) 		\
					{										\
						 	__prev->next = __d;					\
						 	__d->next = __tmp;					\
						 	__add = 1;						\
						 	break;							\
					}						\
					__prev = __tmp;				\
					__tmp = __tmp->next;			\
				}							\
				if (!__add) {					\
					__prev->next = __d;			\
					__d->next = 0;				\
				}							\
			}								\
		}									\
		__d;									\
	})

/************************************************************
*															*
*            The running environment setting end						*
*															*
*************************************************************/

#define CDMG_FAIL	-227

/*the command description*/
typedef struct __cdmg_cmd {
	char magic[32];
	char *name;
	char *help;
	/*is_get is set by cdmg_do_get_handler*/
	int (*func)(int argc, char *argv[], char *rbuf, int rlen, int is_get);
	int is_daemon;/*this function must do at daemon*/

	int is_inner; /*this fuction can not be used by user*/
	int is_unshow; /*this function can not show at help function*/
	int is_get_action;
	char *rspath;
	int rlen;/*the return buffer size*/
	int is_asynch;/*indicate the action is run at synch mode*/
	int timeout;
	
	pid_t do_process;
	void *timeout_timer;
	struct __cdmg_cmd *next;
} __cdmg_cmd_t;

/*cdmg pid desc*/
typedef struct __cdmg_pid{
	pid_t  pid;
	int p_fd[2]; /*for asynch between the father and the sun task*/
} cdmg_pid_t;

/*indicate the running env*/
typedef enum{
	CDMG_ENV_USER = 0,
	CDMG_ENV_USER_SENT,
	CDMG_ENV_USER_GET,
	CDMG_ENV_DAEMON,
	CDMG_ENV_DAEMON_GET
} cdmg_env_t;


/*function define*/
int  		cdmg_is_stop(void);
int    	cdmg_is_on_daemon();
int    	cdmg_get_arg(char *argv[],  char *name, char *r_buf, int r_len);
int 		cdmg_send_msg_fd(int fd, const struct sockaddr *dst, int len,
				const char *msg, int size);
int 		cdmg_send_unix_msg(const char *dst_path, int protocol, 
				const char *msg, int size);
int 		cdmg_send_msg_to_daemon(const char *msg, int len);
int 		cdmg_create_unix_socket(const char *path, int protocol);
int  		cdmg_rcvd_msg_fd(int fd, char *rbuf, int len, struct timeval *tv);
int 		cdmg_call_cmd(int argc, char *argv[]);
int  		cdmg_get_act(int argc, char *argv[], int get_len, char **rbuf, 
					struct timeval *time_out);

void 	cdmg_init(void * (*timer_set)(int tick, const char *name, 
						void (*)(void *), void *data),
			void (*timer_del)(void *timer),
			struct timeval * (*timer_handler)(void *data), const char *daemon_addr);
void 	cdmg_msg_process_loop(const char *unix_sok_path, 
			int usleep_v, const char *token, 
			void (*handler)(char *msg), 
			struct timeval * (*timeout_func)());
void  	cdmg_start(void);
void  	cdmg_stop(void);
void  	cdmg_cmd_to_argv(char *msg, int *argc, char **__argv);
void		cdmg_argv_to_cmd(int argc, char **argv, 
			char *rbuf, int rlen);
void  	cdmg_shell(int argc, char *argv[]);
void  	cdmg_daemon(int usleep_v, const char * token, 
			void (*init)(void *date), void *date);


void  *	cdmg_test_timer_set(int tick, const char *name, 
				void (*func)(void *), void *data);
void   	cdmg_test_timer_del(void *timer);
void 	cdmg_start_sub(cdmg_pid_t *pid);
void 	cdmg_process_cmd_msg(char *msg_cmds);
void 	cdmg_set_daemon();

struct timeval *   	cdmg_test_timer_handler(void *date);
cdmg_pid_t *		cdmg_fork_wait( pid_t *sub_pid);


cdmg_env_t  	__cdmg_fork(int argc, char *argv[], int get_len,
						char **rbuf, int *rlen, int is_do_get);

const struct sockaddr  *cdmg_create_unix_addr(const char *path);

extern char    g_cdmg_daemon_addr[128];
extern char    g_cdmg_buf[CDMG_MSG_LEN+1];
extern int      g_cdmg_argc;
extern char * g_cdmg_argv[CDMG_ARGS_NUM];

/*maloc a buffer, if error do 'return_str'*/
#define   CDMG_MALLOC(s, r, return_str) 		\
			do{ 								\
				if (!((r) = malloc(s)))	 {	\
					perror("mallocl error");		\
					(r) = 0;					\
					{return_str;}				\
				}							\
				memset((r), 0, (s));			\
			}while(0)

/*open a file */
#define   CDMG_OPEN(fd, func, flag, path, return_str) \
			do{ 						\
				if (((fd) = func(path, flag)) <= 0) {\
					d_printf(#func " error");	\
					{return_str;}			\
				}	\
			}while(0)

/*likely the strcat*/
#define  cdmg_fstrncat(d, n, args...) \
	({			\
		char *__dst = (d);			\
		int 	  __len = 0;			\
		int     __remain_len = 0;		\
								\
		if (__dst) {					\
			__len = strlen(d);		\
			__remain_len = (n) - __len;	\
			__dst += __len;			\
								\
			snprintf(__dst, __remain_len, args);	\
		}	\
		__dst;\
	})
				
#define  CDMG_GET_ARG_L(n, r, l)  cdmg_get_arg(argv,  (n), (r), (l))
#define  CDMG_GET_ARG(n, r)  CDMG_GET_ARG_L((n), (r), sizeof(r)-1)


#define __CDMG_MAGIC  "magic:_\r\n\n\r_`~*&^;:'<>,?/|$%@!#"

/*
** define two function and define an command decs
**
** @ daemon  this func must done at daemon
** @ inner  this func is called by deamon, not open to the user
** @ is_get  this func is used to get the daemon info by the user 
** @ __rbuf  setting by the deamon
*/
#define CDMG_FUNC(fn, daemon, inner, un_show, is_get, asyn, to, info) 	\
	int   __cdmg_cmd_##fn(int argc, char *argv[], char *__rbuf, \
					int __rlen, int __is_get); 				\
	char __cdmg_name_##fn[] = #fn;						\
	char __cdmg_help_##fn[] = info;						\
	char __cdmg_rpath_buf_##fn[CDMG_CMD_LINE_LEN+1] = "";	\
	__cdmg_cmd_t 	__attribute__((__section__(".cdmg_cmds.text")))	\
					__attribute__ ((aligned (4)))			\
		__cdmg__desc_##fn = 								\
		{												\
			magic		: __CDMG_MAGIC,					\
			name		: __cdmg_name_##fn,			       \
			help			: __cdmg_help_##fn,				\
			func			: __cdmg_cmd_##fn,				\
			is_daemon	: (daemon),						\
			is_inner		:(inner),							\
			is_get_action	: (is_get),						\
			is_asynch	: (asyn),							\
			is_unshow	: (un_show),						\
			rlen			: 0,								\
			timeout		: (to),							\
			do_process	: 0,								\
			rspath		: __cdmg_rpath_buf_##fn,			\
			timeout_timer	: 0,								\
			next		: 0,								\
		};												\
	int __cdmg_cmd_##fn(int argc, char *argv[], char *__rbuf, int __rlen,	 \
						int __is_get)




/**
* The macro is used to print the output info at 'get info ' function.
*
* @ __rbuf come from the args list, see macro 'CDMG_FUNC_SART'
* @ __rlen come from the args list, see macro 'CDMG_FUNC_SART'
*/
#define cdmg_get_printf(args...)	\
		do{	\
			if (cdmg_is_on_daemon())		\
				snprintf(__rbuf, __rlen, args);	\
		}while(0)
		
#define cdmg_get_strncat(args...) \
		do{	\
			if (cdmg_is_on_daemon())		\
				cdmg_fstrncat(__rbuf, __rlen, args);	\
		}while(0)
		
#define cdmg_get_return()	 do{return strlen(__rbuf)+1;}while(0)
			
/*
* @__is_get  is set at cdmg_do_get_handler(), come 
*		from the args list at macro 'CDMG_FUNC_SART'
*		
* return:
*	CDMG_ENV_DAEMON  run at daemon by cdmg_process_cmd_msg()
*  	CDMG_ENV_DAEMON_GET      run at deamon by cdmg_do_get_handler()
*	CDMG_ENV_USER_SENT run at user's shell, have send the command 
*			to the daemon, then the cdmg_process_cmd_msg() or cdmg_do_get_handler()
*			will call this function again later at daemon, so the user caller need do nothing.
*	CDMG_ENV_USER   run at user's shell, caller need to do the realy action
*     CDMG_ENV_USER_GET  send the get command to daemon and return, then
*                   need to show the result.
*/	
#define CDMG_FORK(get_len,  rbuf,  rlen) 	\
	({									\
		__cdmg_fork(argc, argv, (get_len), \
				(rbuf), (rlen), __is_get);	\
	})

/**
* judge if is let done at deamon by the args.
*/
#define CDMG_HAVE_DN_ARGS() 									\
	({															\
		int ret = 0;												\
																\
		if ( CDMG_GET_ARG("s", 0) || CDMG_GET_ARG("asynch",0) ||	\
				CDMG_GET_ARG("deamon",0))						\
			ret = 1;												\
		ret;														\
	})

#define CDMG_SEND_MSG(fn, fmt, args...)		\
	do {																	\
 		snprintf(g_cdmg_buf, sizeof(g_cdmg_buf), " %s  " fmt, #fn, ##args);	\
		cdmg_send_msg_to_daemon( g_cdmg_buf, strlen(g_cdmg_buf));		\
	}while(0)

/*
** Get info from daemon, then return the result.
**
**  @to_usec  timeout
**  @rbuf	the return buf, it MUST be free. the type should be 'char *'
**  
**  return: the length of the rbuf. -1 error
*/
#define CDMG_SEND_AND_GET(rbuf, to_usec, name, fmt, args...)		\
	({									\
		struct timeval tv;					\
										\
 		snprintf(g_cdmg_buf, sizeof(g_cdmg_buf), " %s  " fmt, #name, ##args);\
 		cdmg_cmd_to_argv(g_cdmg_buf, &g_cdmg_argc, g_cdmg_argv);		\
 		tv.tv_sec = (to_usec)/1000000;		\
 		tv.tv_usec = (to_usec) % 1000000;	\
 		cdmg_get_act(g_cdmg_argc, g_cdmg_argv, 0, (char **)(&(rbuf)), &tv);	\
	})
	
/*send the cmd to the deamon to do it at daemon*/
#define CDMG_DO_AT_DN(name, fmt, args...)		\
	do{										\
		if (!cdmg_is_on_daemon())			\
			CDMG_SEND_MSG(name, fmt, args);	\
		}									\
	}while(0)

/*get the function by the name*/
#define CDMG_N_TO_F(fn)		\
	({											\
		extern __cdmg_cmd_t __cdmg__desc_##fn;	\
		__cdmg_cmd_t *cmd = &__cdmg__desc_##fn;	\
												\
		cmd->func;								\
	})
	
/*call the function*/
#define CDMG_DO_FUNC(fn, fmt, args...)		\
	({	\
 		snprintf(g_cdmg_buf, sizeof(g_cdmg_buf), " %s  " fmt, #fn, ##args);	\
			\
		cdmg_debug("do cmd:[%s]\n", g_cdmg_buf);	\
		cdmg_cmd_to_argv(g_cdmg_buf, &g_cdmg_argc, \
					g_cdmg_argv);	\
		CDMG_N_TO_F(fn)(g_cdmg_argc, g_cdmg_argv, 0, 0, 0);	\
	})
	

/*this macro for user to define the command function*/
#define __CDMG_SETUP_CMD(name, unshow, handler, info)	 \
	CDMG_FUNC(name, 0, 0, unshow, 0, 0, 0, info)		\
	{								\
		if (CDMG_FORK(0, 0, 0) == CDMG_ENV_USER_SENT) {	\
			return 0;							\
		}									\
		return handler(argc, argv);			\
	}

/*
** this macro for user to define the get function
**
** the get_func must be define as:
** int get_func(int argc, char *argv[], char *rbuf, int rlen)
**
*/
#define __CDMG_SETUP_GET(name, un_show, get_func, asyn, to, info)		\
	CDMG_FUNC(name, 0, 0, un_show, 1, asyn, to, info)					\
	{								\
		char *buf = 0;				\
		int ret = CDMG_FORK(CDMG_MSG_LEN+1, &buf, 0);	\
												\
		cdmg_debug("FORK return:%d\n", ret);			\
		if (ret == CDMG_ENV_DAEMON_GET) {	\
			if (buf)	\
				free(buf);	\
			return get_func(argc, argv, __rbuf, __rlen);	\
		}									\
		if (ret == CDMG_ENV_USER_GET) {	\
			if (buf) {					\
				printf("%s", buf);		\
				free(buf);				\
			}								\
			return 0;						\
		}									\
		if (buf)			\
			free(buf);	\
		cdmg_printf("Get error\n");		\
		return -1;							\
	}

/*
** The command allways done at the deamon
**
** the get_fn is:
**   int handler(int argc, char *argv[])
*/
#define __CDMG_SETUP_MSG(name, inner, unshow, handler, info)	\
	CDMG_FUNC(name, 1, inner, unshow, 0, 0, 0, info)		\
	{								\
		int ret = CDMG_FORK(0, 0, 0);	\
									\
		if (ret == CDMG_ENV_DAEMON) {		\
			return handler(argc, argv);		\
		}									\
		return 0;								\
	}		

/*
** the fn is:
**   int fn(int argc, char *argv[])
*/
#define CDMG_SETUP_CMD(n, fn, i) __CDMG_SETUP_CMD(n, 0, fn, i)

/*
** the get_fn is:
**   int get_fn(int argc, char *argv[], char *rbuf, int rlen)
*/
#define CDMG_SETUP_GET(n, get_fn, asyn, to, i) \
			__CDMG_SETUP_GET(n, 0, get_fn, asyn, to, i)

/*
** the get_fn is:
**   int fn(int argc, char *argv[])
*/
#define CDMG_SETUP_MSG(n, fn, i) \
			__CDMG_SETUP_MSG(n, 1, 1, fn, i)
		
#endif
