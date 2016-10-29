#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <netinet/in.h>
#include <sys/queue.h>
#include <sys/time.h>
#include <stdlib.h>
#include <signal.h>
/* #include <arpa/inet.h> */

#include "glbvars.h"
#include "pcp.h"
#include "ext_intf.h"
#include "timer.h"
#include "config.h"

/* overloaded_time, aimed to deal with SERVER_OVERLOADED response code */
struct timeval overld_time;
pcp_t p;
struct pcp_timer *cur_timer = NULL;

static uint32_t sig_flags = 0;
static void main_loop();
static void client6_signal(int sig);
static void get_if2name(const char *str, char *l2name);
static void client_signal(int sig);
static void process_signals();

#define SIGF_TERM 0x1
#define SIGF_HUP 0x2

static void print_usage(void)
{
	printf(
			"Usage: pcp [OPTIONS]\n\n"
			"  -h,      help\n"
			"  -f, 	configuration file\n"
	);
}

static void register_signal()
{
	struct sigaction act;
	act.sa_handler = client_signal;

	if (sigaction(SIGTERM, &act, NULL)) {
		exit(EXIT_FAILURE);
	}

	if (sigaction(SIGHUP, &act, NULL)) {
		exit(EXIT_FAILURE);
	}
}

#ifdef BUILD_STATIC
int pcp_main(int argc, char *argv[])
#else
int main(int argc, char *argv[])
#endif
{
	int ret;
	char cfile[BUF_32] = {'\0'};
	int opt = 0;
	char brcm_ifname[32] = {'\0'};
	int i =0;

	TAILQ_INIT(&oper_hd);

	while (1) {
		opt = getopt(argc, argv, "f:h");
		if (opt == -1) break;

		switch (opt) {
			case 'f':
				strncpy(cfile, optarg, sizeof(cfile)-1);
				break;
			case 'h':
				print_usage();
				return 0;
			case ':':
				printf("options needs a value\n");
				return -1;
		}
	}

	if (read_config(cfile)) {
		exit(1);
	} 

	get_if2name(cfile, brcm_ifname);

	register_signal();

	ret = pcp_init(&p, brcm_ifname);
	if (ret < 0)
		return -1;

	pcp_timer_init();
	ext_intf_init();

	if (gettimeofday(&overld_time, NULL) < 0)
		return -1;

	/*delay sent packet*/
	while((i++) < 5)
	{
	   sleep(1);
          if (sig_flags)
                 process_signals();
	}
	
	main_loop();

	return pcp_close(&p);
}

static void main_loop()
{
	int ret, max_fd;
	fd_set r;
	struct timeval *w;

	while(1) {
		if (sig_flags)
			process_signals();

		if (p.pending_req && p.timer) { /* pending request must have response */
			pcp_check_timo(p.timer);
		} else {
            /* 如果rcl删除了配置，则也会删除定时器 */
            p.pending_req = 0;
			pcp_check_timo(NULL);
		}

		if (!p.pending_req)
			p.timer = pcp_pick_timer();

		FD_ZERO(&r);
		FD_SET(p.s, &r);
		max_fd= p.s;
		ext_intf_fd(&max_fd, &r);

		w = pcp_timer_rest(p.timer);
		ret = select(max_fd + 1, &r, NULL, NULL, w);

		switch (ret) {
		case -1:
			if (errno != EINTR) {
				exit(1);
			}
			break;
		case 0:	/* timeout */
			break;	/* pcp_check_timer() will treat the case */
		default:
			if (FD_ISSET(p.s, &r)) {
				pcp_recv(&p);
			}
			else {
				ext_event(&r);
			}
		}

	}
}

void client_timo(void *arg)
{
	struct pcp_oper *oper = (struct pcp_oper *)arg;

	switch(oper->o_state) {
		case PCP_FAIL:
			if (p.pending_req) {
				p.pending_req = 0;
				oper->error_code = NOT_RESPONSE;
			} else {
				oper->o_state = PCP_INIT;
                pcp_remove_timer(&oper->timer);
            }
			break;
		case PCP_INIT: /* for a random delay */ 
			oper->timeouts = 0;
			pcp_construct_reqdata(&p, oper);
			oper->o_state = PCP_START;
			p.pending_req = 1; /* until we get a response, we will resend it forever */ 
			break;
		case PCP_START:
			oper->timeouts++;
			pcp_send(&p);
			break;
		case PCP_ACTIVE: /* no need to wait for response */ 
			if (oper->renew_nr == 3) { /* expiration */ 
				pcp_reset_oper(oper);
				pcp_remove_timer(&oper->timer);
				return ;
			}
			oper->renew_nr++;
			pcp_construct_reqdata(&p, oper);
			pcp_send(&p);
			break;
	}
	pcp_reset_timer(oper);
}

static void client_signal(int sig)
{
	switch (sig) {
	case SIGTERM:
		sig_flags |= SIGF_TERM;
		break;
	case SIGHUP:
		sig_flags |= SIGF_HUP;
		break;
	}
}

static void process_signals()
{
	if ((sig_flags & SIGF_TERM)) {
		free_resources(&p);
		exit(EXIT_SUCCESS);
	}
	if ((sig_flags & SIGF_HUP)) { /* now to deal with it ? */ 
	}

	sig_flags = 0;
}

static void get_if2name(const char *file, char *l2name)
{
	char * end = NULL;
	char * begin = NULL;
	if (file) {
		end = strstr(file, ".conf");
		begin = strstr(file, "pcp_");
		if (begin && end) {
			if (&begin[4] < end)
				strncpy(l2name, &begin[4], end - &begin[4]);
		}
	}
}

