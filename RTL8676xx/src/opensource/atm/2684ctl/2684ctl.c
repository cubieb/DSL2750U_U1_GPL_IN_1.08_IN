/* Written by Marcell GAL <cell@sch.bme.hu> to make use of the */
/* ioctls defined in the br2684... kernel patch */
/* Compile with cc -o br2684ctl br2684ctl.c -latm */

/*
  Modified feb 2001 by Stephen Aaskov (saa@lasat.com)
  - Added daemonization code
  - Added syslog
  
  TODO: Delete interfaces after exit?
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <syslog.h>
#include <fcntl.h>
#include "atm.h"

#define CMD_FILE          "/tmp/2684cmd"
#define DAEMON_PID_FILE    "/var/run/2684d.pid"

/*获得2684d的进程ID*/
int get_process_pid(void)
{
    int process_id = -1;
    FILE *fp = NULL;

    fp = fopen(DAEMON_PID_FILE, "r");
    if (fp == NULL) {
        syslog(LOG_INFO, "Error : Couldn't open file [%s] in read mode!", DAEMON_PID_FILE);
        return (process_id);
    } else {
        if (fscanf(fp, "%d", &process_id) < 0)
            process_id = -1;        
        
        fclose(fp);
    }

    return process_id;
}

int main (int argc, char **argv)
{
	char strcmd[256];
	int cmd_fd = -1;
	int daemon_pid = -1;
	int i;

	/* 将配置命令写到文件中 */
	memset(strcmd,0x00,sizeof(strcmd));
	for(i = 1; i < argc; i++) {
		strcat(strcmd, argv[i]);
		strcat(strcmd, " ");
	}
	strcmd[strlen(strcmd) - 1] = '\0';	
	cmd_fd = open(CMD_FILE,O_WRONLY | O_CREAT);
	if(cmd_fd < 0) {
		perror("br2684ctl ");
		syslog(LOG_ERR,"Could not open cmd file %s in write mode!!!", CMD_FILE);
		return -1;
	}
	write(cmd_fd,strcmd,strlen(strcmd));
	close(cmd_fd);
	
	daemon_pid = get_process_pid();
	if(daemon_pid < 1) {
		syslog(LOG_ERR,"Could not get pid of mpoad!!!");
	} else {
		syslog(LOG_INFO,"br2684_ctl : Sending SIGHUP to %d", daemon_pid);
		kill(daemon_pid,SIGHUP); /*给2684d发送SIGHUP信号*/
	}

	return 0;
}

