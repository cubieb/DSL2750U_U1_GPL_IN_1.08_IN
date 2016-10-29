#ifndef __BFTPD_MAIN_H
#define __BFTPD_MAIN_H


#define TRUE 1
#define FALSE 0

#ifdef TBS_FTPUPG
/* Status codes */
#define SL_UNDEF 0
#define SL_SUCCESS 1
#define SL_FAILURE 2

/* Message type */
#define SL_INFO 1
#define SL_COMMAND 2
#define SL_REPLY 3


#define CONTROL_TIMEOUT 300
#define DATA_TIMEOUT 300
#define XFER_BUFSIZE 4096
#define XFER_DELAY 0
#define DEFAULT_PORT 21

#define bftpd_statuslog(...)  do{} while(0)
#define bftpd_log(...)  do{} while(0)
#endif

#include <sys/types.h>

struct bftpd_childpid {
	pid_t pid;
	int sock;
};

extern int global_argc;
extern char **global_argv;
extern struct sockaddr_in name;
extern FILE *passwdfile, *groupfile, *devnull;
extern char *remotehostname;
extern struct sockaddr_in remotename;
extern int control_timeout, data_timeout;
extern int alarm_type;

/* Command line options */
char *configpath;
int daemonmode;

/* scripts to run before and after writing to the file system */
#ifndef TBS_FTPUPG
char *pre_write_script;
char *post_write_script;
#endif

void print_file(int number, char *filename);

#endif
