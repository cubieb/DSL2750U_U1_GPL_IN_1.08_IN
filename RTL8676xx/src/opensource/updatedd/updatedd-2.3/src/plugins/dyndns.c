/* -- updatedd: dyndns.c --
 *
 * Copyright (C) 2002, 2003, 2004 Philipp Benner
 *
 * This file is part of UpdateDD - http://updatedd.philipp-benner.de.
 *
 * UpdateDD is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * UpdateDD is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UpdateDD; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>

#include <base64encode.h>
#include <get_connection.h>
#include <libexception_handle.h>
#include <ret_codes.h>
#include <version.h>

#include "../updatedd.h"
#include "dyndns.h"

#define MAX_LOGIN 512
char login[MAX_LOGIN];

static void print_usage(char *pname, FILE *fp)
{
	(void)fprintf(fp,
		      "\nUsage: %s [...] %s -- [OPTION]... [USERNAME:PASSWORD] HOSTNAME\n\n",
		      pname, COLORED("dyndns"));
	(void)fprintf(fp,
		      "For security reasons use the environment variable LOGIN instead of\n"
		      "passing the login information directly.\n\n"
		      "Options:\n"
		      "   -4    --ipv4 <address>        ip address version 4\n"
		      "   -b    --with-backmx           enable backmx\n"
		      "   -m    --mx <host>             hostname of your mail exchange\n"
		      "   -o    --offline               set host to offline mode\n"
		      "   -s    --system <system>       specify the system\n"
		      "                                 (dyndns [default], statdns, custom)\n"
		      "   -w    --wildcard <on|off>     switch wildcard on or off\n"
		      "         --help                  print help and exit\n"
		      "         --version               display version information and exit\n\n"
		      "Report bugs to <"EMAIL">.\n\n");
        
	return;
}

static void print_version(FILE *fp)
{
        
	(void)fprintf(fp,
		      "\n" PNAME " plugin for dyndns.org version " VERSION ",\n"
		      "Copyright (C) 2004 Philipp Benner.\n"
		      HOMEPAGE "\n\n"

		      "This is free software, and you are welcome to redistribute it\n"
		      "under certain conditions; see the source for copying conditions.\n"
		      "There is NO warranty; not even for MERCHANTABILITY or FITNESS\n"
		      "FOR A PARTICULAR PURPOSE.\n\n");

	return;
        
}

static void ret_msg(int mode, const char *fmt, ...)
{
        
	va_list az;
        
	va_start(az, fmt);
	(void)vs_warn(ret_msg_buf, BUFSIZE, mode, fmt, az);
	va_end(az);
        
	return;
        
}

int dyndns(int argc, char *argv[])
{
        
	struct arguments args;
	int s, ret = RET_WRONG_USAGE;
	const char *ptr;

	printf("dyndns plugin started ...\n"); 
#ifdef NSP
	memset((void *)&args, 0xFF, sizeof(struct arguments));
#else
	(void)memset(&args, 0, sizeof(struct arguments));
#endif

	if(get_flags(&args, argc, argv) != RET_OK) {
		return RET_WRONG_USAGE;
	}
        
	s = get_connection(DYNDNSHOST, args.iface, PORT, &ptr);
	if(s == -1) {
		/*ret_msg(HERR, "%s: %s", ptr, DYNDNSHOST);*/
		printf("%s: %s\n", ptr, DYNDNSHOST);
		/*ret = RET_WARNING;*/
		exit(EXIT_FAILURE);
	} else {
		ret = update_dyndns(s, &args);
		if(ret == RET_OK) {
			ret = check_server_msg(s, args.hostname);
		}
		(void)close(s);
	}


	return ret;

}

static void stolower(char *str, char *buf, size_t size)
{

	int n;
        
	for(n = 0; n < size && str[n] != '\0'; n++) {
		buf[n] = tolower(str[n]);
	}
	buf[n] = '\0';

	return;
        
}

static int get_flags(struct arguments *args, int argc, char *argv[])
{
        
	int c, n;
	char buf[BUFSIZE];

	args->backmx = yn.no;
	args->wildcard = "NOCHG";
	args->offline = yn.no;
	args->system = dd_system[0];

        if(argc == 1)
	{
		print_usage(argv[ARGV_PNAME], stdout);
		printf("Try `updatedd --help' for more information\n");
		exit(EXIT_FAILURE);
	}
        
	for(;;) {
                
		int option_index = 0;
		static struct option long_options[] = {
			{ "ipv4",		1, 0, '4' },
			{ "help",		0, 0, 'h' },
			{ "mx",			1, 0, 'm' },
			{ "offline",		0, 0, 'o' },
			{ "system",		1, 0, 's' },
			{ "wildcard",		1, 0, 'w' },
			{ "with-backmx",	0, 0, 'b' },
			{ "version",		0, 0, 'v' },
			{ "interface",		1, 0, 'i' },
			{ "user",		1, 0, 'u' },
			{ "pass",		1, 0, 'p' },
			{ NULL,			0, 0, 0   }			
		};
                
		c = getopt_long(argc, argv, "4:bm:os:w:i:u:p:",
				long_options, &option_index);
                
		if(c == -1) break;
                
		switch(c) {
		case '4':
			args->ipv4 = optarg;
			break;
		case 'b':
			args->backmx = yn.yes;
			break;
		case 'h':
			print_usage(argv[ARGV_PNAME], stdout);
			exit(EXIT_SUCCESS);
		case 'm':
			args->mx = optarg;
			break;
		case 'o':
			args->offline = yn.yes;
			break;
		case 's':
			stolower(optarg, buf, BUFSIZE);
			for(n = 0;; n++) {
				if(dd_system[n] == NULL) {
					/*ret_msg(NONE,"invalid argument for `-s': %s",optarg);*/
					printf("invalid argument for `-s': %s\n",optarg);
					return 1;
				}
				if(strcmp(buf, dd_system[n]) == 0) {
					args->system = dd_system[n];
					break;
				}
			}
			break;
		case 'v':
			print_version(stdout);
			exit(EXIT_SUCCESS);
		case 'w':
			stolower(optarg, buf, BUFSIZE);
			if(strcmp(buf, "on") == 0) {
				args->wildcard = "ON";
			} else if(strcmp(buf, "off") == 0) {
				args->wildcard = "OFF";
			} else {
				/*ret_msg(NONE, "invalid argument for `-w': %s", optarg);*/
				printf("invalid argument for `-w': %s\n", optarg);
				return 1;
			}
			break;
		case 'i':
			args->iface = optarg;
			break;
		case 'u':
			strcpy(login,optarg);
			strcat(login,":");
			break;
		case 'p':
			strcat(login,optarg);
		}
	}

	/*
	switch(argc-optind) {
        default:
		*//*ret_msg(NONE, "wrong usage");*//*
		printf("wrong usage\n");
		return RET_WRONG_USAGE;
                
        case 2:
		args->login = getenv("LOGIN");
		if(args->login == NULL) {
			*//*ret_msg(NONE, "environment variable LOGIN is empty");*//*
			printf("environment variable LOGIN is empty\n");
			return RET_WRONG_USAGE;
		}
		break;
        case 3:
		args->login = argv[ARGV_LOGIN];
	}
	*/

	if(strcmp(login,"") == 0)
	{
		args->login = getenv("LOGIN");
		if(args->login == NULL) {
			/*ret_msg(NONE, "environment variable LOGIN is empty");*/
			printf("environment variable LOGIN is empty\n");
			return RET_WRONG_USAGE;
		}
	}
	else
	{
		args->login = login;
	}
	
	args->hostname = argv[ARGV_HOSTNAME];
        
	return RET_OK;
        
}

#define BUFLEN		4096
#define BUFFREE(name)	BUFLEN - strlen(name)

static int update_dyndns(const int s, struct arguments *args)
{

	char *b64user;
	char message[BUFLEN];

	if(strlen(args->login) > 128) {
		/*ret_msg(NONE, "username is too long");*/
		printf("username is too long\n");
		return RET_ERROR;
	}
	b64user = (char *)malloc((2 * strlen(args->login) + 1));
	if(b64user == NULL) {
		/*ret_msg(PERR, "malloc() failed");*/
		printf("malloc() failed\n");
		return RET_WARNING;
	}
	(void)memset((void *)b64user, 0, 2 * strlen(args->login) + 1);
        
	base64encode(args->login, b64user);
	(void)snprintf(message, BUFLEN,
                       "GET /nic/update?system=%s&hostname=%s&wildcard=%s"
                       "&backmx=%s&offline=%s",
                       args->system, args->hostname,
                       args->wildcard,	args->backmx, args->offline);
        
	if(args->ipv4) {
		(void)strncat(message, "&myip=", BUFFREE(message));
		(void)strncat(message, args->ipv4, BUFFREE(message));
	}
        
	if(args->mx) {
		(void)strncat(message, "&mx=", BUFFREE(message));
		(void)strncat(message, args->mx, BUFFREE(message));
	}
        
	{
		char buffer[1024];
                
		(void)snprintf(buffer, 1024,
                               " HTTP/1.1\r\n"
                               "Host: %s\r\n"
                               "Authorization: Basic %s\r\n"
                               "User-Agent: %s %s - %s\r\n"
                               "Connection: close\r\n"
                               "Pragma: no-cache\r\n\r\n",
                               DYNDNSHOST, b64user, PNAME, VERSION, HOMEPAGE);
		(void)strncat(message, buffer, BUFLEN - 1 - strlen(message));
	}
	print_debug("\n\nMessage:"
		    "\n--------------------------------------\n"
		    "%s--------------------------------------\n\n",
                    message);
	
	if(write(s, message, strlen(message)) == -1) {
		/*ret_msg(PERR, "write() failed");*/
		printf("write() failed\n");
		return RET_WARNING;
	}
        
	free(b64user);
	return RET_OK;
        
}

static int check_server_msg(int s, const char *hostname)
{
        
	int n;
	char server_msg[BUFSIZE], *ptr;
	
	printf("Inside check_server_msg\n");
        
	/* get server_msg */
	(void)memset((void *)server_msg, 0, sizeof(server_msg));
	if(read(s, server_msg, sizeof(server_msg) - 1) < 0) {
		/*ret_msg(PERR, "read() failed");*/
		printf("read() failed\n");
		return RET_WARNING;
	}
        
	print_debug("\n\nServer message:"
		    "\n--------------------------------------\n"
		    "%s--------------------------------------\n\n",
		    server_msg);
        
	if(strstr(server_msg, "HTTP/1.1 200 OK") ||
	   strstr(server_msg, "HTTP/1.0 200 OK")) {
                
		(void)strtok(server_msg, "\n");
		while((ptr = strtok(NULL, "\n")) != NULL) {
			for(n=0; return_codes[n].code != NULL; n++) {
				if(strstr(ptr, return_codes[n].code)) {
					/*ret_msg(NONE, "%s: %s",hostname, return_codes[n].message);*/
					printf("%s\n",return_codes[n].message);
					if(return_codes[n].error == 1) {
						return RET_ERROR;
					} else {
						return RET_OK;
					}
				}
			}
		}
	} else if(strstr(server_msg, "401 Authorization Required")) {
		/*ret_msg(NONE, "dyndns.org: wrong username or password");*/
		printf("Wrong username or password.error\n");
	} else {
		/*ret_msg(NONE, "dyndns.org: Internal Server Error");*/
		printf("Dynamic DNS internal server fault.error\n");
	}
        
	return RET_ERROR;
}
