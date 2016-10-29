/* Our own header, to be included before all standard system headers */

#ifndef	_COMMON_H
#define	_COMMON_H


#include <sys/types.h>		/* some systems still require this */
#include <sys/stat.h>
#include <stdio.h>		/* for convenience */
#include <stdlib.h>		/* for convenience */
#include <stddef.h>		/* for offsetof */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */

#include <sys/types.h>
#include <sys/socket.h>

/*
 * Default file access permissions for new files.
 */
#define	FILE_MODE	(S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)

/*
 * Default permissions for new directories.
 */
#define	DIR_MODE	(FILE_MODE | S_IXUSR | S_IXGRP | S_IXOTH)

typedef	void	Sigfunc(int);	/* for signal handlers */

#if	defined(SIG_IGN) && !defined(SIG_ERR)
#define	SIG_ERR	((Sigfunc *)-1)
#endif

#define	min(a,b)	((a) < (b) ? (a) : (b))
#define	max(a,b)	((a) > (b) ? (a) : (b))


#define SERV_UNIX_SOCK_PATH				"/var"
#define SERV_UNIX_SOCK_FNAME			"myunix.socket"
#define SERV_UNIX_SOCK_FULLNAME		SERV_UNIX_SOCK_PATH "/" SERV_UNIX_SOCK_FNAME
#define SERV_UNIX_SOCK_TYPE    		SOCK_STREAM

extern int quit_signal;

/*
 * Prototypes for our own functions.
 */
int		 serv_listen(const char *, int sock_type);		/* {Progs servlisten_streams servlisten_sockets} */
int		 serv_accept(int, uid_t *);		/* {Progs servaccept_streams servaccept_sockets} */
int cli_conn(const char *serv_name, int sock_type, const char *cli_name);

int send_msg(int fd, char*msg, int len);
int receive_msg(int fd, char* buf, int buf_len);
int cli_echo_msg(int fd, char*msg);
int serv_echo_msg(int fd);

#endif	/* _COMMON_H */
