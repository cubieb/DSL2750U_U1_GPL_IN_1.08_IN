/*
	A tool for setting user's account & rights
	Copyright 2008 by TBS T&W.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <crypt.h>
#ifdef	DEPEND_ON_CONFIG
#include "mystring.h"
#endif
#include "options.h"
#include "mypaths.h"
#include "base64.h"

#ifdef	DEPEND_ON_CONFIG
/*
Returns NULL on error. May return
emtpy string "" for empty or
commented lines.
*/
static char *config_read_line(FILE *configfile)
{
	static char str[MAX_STRING_LENGTH];
	char *s = str;
	if (!fgets(str, sizeof(str), configfile))
		return NULL;
	while ((strchr(s, '#') > strchr(s, '"')) && strchr(s, '"')) {
		s = strchr(strchr(s, '"') + 1, '"');
		if (!s) { // This means there is only one " in the string, which is a syntax error.
			str[0] = 0; // So we empty the string in order not to confuse the parser.
			return str;
		}
	}
	if (strchr(s, '#'))
		*strchr(s, '#') = 0;
	s = str;
	while ((s[0] == ' ') || (s[0] == '\t'))
		s++;
	return s;
}

static char *get_confoption(char *configfilepath, char *keyword)
{
	FILE *fpconfig = NULL;
	char *pconfline = NULL;
	char *poffset = NULL;
	char *pret = NULL;

	if (!configfilepath || !keyword)
		return NULL;

	fpconfig = fopen(configfilepath, "r");
	if (!fpconfig){
		perror("open");
		return NULL;
	}
	while( (pconfline = config_read_line(fpconfig)) ) {
		if (0 == strlen(pconfline))
			continue;
		poffset = strstr(pconfline, keyword);
		if ( poffset ) {
			poffset = strchr(poffset, '"');	/* first " */
			if (poffset)
				poffset++;

			if (strchr(poffset, '"'))	/* second " */
				*strchr(poffset, '"') = 0;

			pret = strdup(poffset);
			break;
		}
	}
	fclose(fpconfig);
	return pret;
}
#else
#define	get_confoption(a...)	(NULL)
#endif

/*
	-c, ftpd.conf path;
	-u, username;
	-p, password;
	-r, disallow commands key word;
*/
int main(int argc, char **argv)
{
	int retval;
	char *username = NULL;
	char *username_base64 = NULL;
	char *password = NULL;
	char *ftpcmds = NULL;
	char *configpath = NULL;
	char *vuserdbpath = PATH_VIRTUAL_USER_DB;
	char *vrightdbpath = PATH_VIRTUAL_RIGHT_DB;
	int len_base64 = 0;
	while ((retval = getopt(argc, argv, "c:u:p:r:h")) > -1) {
		switch (retval) {
			case 'h':
				printf(
					"Usage: %s [-h] [-c <filename>] [-u <username>] [-p <password>][-r <STOR|APPE|LIST|MKD|...>]\n"
					"-h print this help\n"
#ifdef	DEPEND_ON_CONFIG
					"-c read the config file named \"filename\" instead of " PATH_BFTPD_CONF "\n"
#else
					"-c unused. config file always be "PATH_BFTPD_CONF"\n"
#endif
					"-u username\n"
					"-p password\n"
					"-r ftp commands disallowed\n", argv[0]);
				return 0;
#ifdef	DEPEND_ON_CONFIG
			case 'c': configpath = strdup(optarg); break;
#endif
			case 'u': username = strdup(optarg); break;
			case 'p': password = strdup(optarg); break;
			case 'r': ftpcmds = strdup(optarg); printf("cmds:%s\n", ftpcmds); break;
		}
	}

	if (!username){
		printf("Missing username\n");
		return -1;
	}

	if (NULL == configpath)
		configpath = PATH_BFTPD_CONF;
	printf("configpath:%s\n", configpath);

	vuserdbpath = get_confoption(configpath, "FILE_AUTH");
	if (NULL == vuserdbpath)
		vuserdbpath = PATH_VIRTUAL_USER_DB;
	printf("virtual user path:%s\n", vuserdbpath);

	printf("user:%s\npassword:%s\nftp commands:%s\n", username, password, ftpcmds);
	username_base64 = base64_encode(username, strlen(username));
	len_base64 = strlen(username_base64);
	printf("%s %s ftp /mnt\n", username_base64, (password ? crypt(password, FTPD_CRYPT_SALT) : "*") );

#define	SAFE_FREE(p)	do{if(p) free(p);}while(0)
	SAFE_FREE(username);
	SAFE_FREE(password);
	SAFE_FREE(ftpcmds);
	SAFE_FREE(username_base64);
	return 0;
}
