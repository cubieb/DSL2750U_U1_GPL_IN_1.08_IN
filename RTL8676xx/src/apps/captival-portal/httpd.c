/**
* scb+ 2011-10-12
*
* file name: httpd.c
*
* if receive the http package, send the captival portal page to the host
*
*/

#include "captival-portal.h"

#define SERVER_NAME "mini_httpd"
#define PROTOCOL "HTTP/1.1"
#define RFC1123FMT "%a, %d %b %Y %H:%M:%S GMT"

struct accept{
	int fd;
	struct accept *next;
};
	
static struct accept *s_clients = 0;
static char s_captival_portal_page[CPLPL_MAXPATHLEN] = {0};
static char s_session_id[12] = {0};

static int cplpl_httpd_accept_process(int fd);

void cplpl_httpd_set(const char *url)
{
	snprintf(s_captival_portal_page, CPLPL_MAXPATHLEN, "%s", url);
}

/*delete the captival portal item*/
void cplpl_httpd_del(const char *url)
{
	memset(s_captival_portal_page, 0, CPLPL_MAXPATHLEN);	
	cplpl_http_close_all_accept();
}

/*init the httpd preccess*/
int cplpl_httpd_init()
{
	int i;
	int listen_fd = 0;
	uint32_t host = 0;		
	struct sockaddr_in sa_in;

	d_printf("int httpd\n");
	/*create the socket*/
	if ((listen_fd = socket( AF_INET, SOCK_STREAM, 0 )) < 0) {
		perror( "socket" );
		return -1;
	}
	(void) fcntl(listen_fd, F_SETFD, FD_CLOEXEC);
	i = 1;
	if ( setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, (char*) &i, 
				sizeof(i) ) < 0)
	{
		perror( "setsockopt" );
		close(listen_fd);
		return -1;
	}

	inet_pton(AF_INET, g_local_host, (void *)&host);
	sa_in.sin_family = AF_INET;
	sa_in.sin_addr.s_addr = host;
	sa_in.sin_port = htons(HTTP_PORT);
	d_printf("bind httpd's socket to %s : %d\n", g_local_host, HTTP_PORT);
	if (bind( listen_fd, (struct sockaddr*)&sa_in, 
		sizeof(struct sockaddr_in) ) < 0 )
	{
		perror( "bind" );
		close(listen_fd);
		return -1;
	}	

	if (listen( listen_fd, 5) < 0 )
	{
		perror( "listen" );
		close(listen_fd);
		return -1;
	}
	d_printf("listen OK\n");
	
	return listen_fd;	
	
}

/*set all the accept fd to the select read fd set*/
int cplpl_httpd_fd_set_accept(fd_set *fds)
{	
	int   max_fd = 0;
	struct accept *p = 0;
		
	for (p = s_clients; p; p = p->next) {
		FD_SET(p->fd, fds);
		if (max_fd < p->fd)
			max_fd = p->fd;
	}

	return max_fd;
}

/*add the accept fd to a list*/
static int cplpl_httpd_add_accept_fd(int fd)
{
	struct accept *ac = 0;

	CDMG_MALLOC(sizeof(struct accept), ac, return -1);

	/*add fd the the accept fd list*/
	CDMG_LIST_ADD(struct accept, s_clients, ac)->fd = fd;
	return 0;	
}

/*accept a cleint request*/
int cplpl_http_process(int httpd_fd)
{
	int conn_fd = 0;
	
	if ((conn_fd = accept(httpd_fd, 0, 0)) < 0) {
		cplpl_printf("accept failed with errno=%d", errno);
		return -1;
	}

	d_printf("accept a httpd:%d\n", conn_fd);
	cplpl_httpd_add_accept_fd(conn_fd);
	
	return 0;
}

/*read the accept socket*/
int cplpl_http_process_accept(fd_set *fds)
{
	struct accept *ac = 0;

	for (ac = s_clients; ac; ac = ac->next) {
		if (FD_ISSET(ac->fd, fds)) {
			cplpl_httpd_accept_process(ac->fd);
			return 0;
		}
	}

	d_printf("Can not find accept fd\n");
	return -1;
}

#if 0
/*send http head*/
static void send_headers(FILE *conn_fp, int status, char* title, 
		char* extra_header, char* mime_type )
{
    time_t now;
    char timebuf[100];

    (void) fprintf( conn_fp, "%s %d %s\r\n", PROTOCOL, status, title );
    (void) fprintf( conn_fp, "Server: %s\r\n", SERVER_NAME );

    now = time( (time_t*) 0 );
    (void) strftime( timebuf, sizeof(timebuf), RFC1123FMT, gmtime( &now ) );
    (void) fprintf( conn_fp, "Date: %s\r\n", timebuf );
    if ( extra_header != (char*) 0 )
        (void) fprintf( conn_fp, "%s\r\n", extra_header );
    if ( mime_type != (char*) 0 )
        (void) fprintf( conn_fp, "Content-Type: %s\r\n", mime_type );
    (void) fprintf( conn_fp, "Connection: close\r\n" );
    (void) fprintf( conn_fp, "\r\n" );
}
#endif

/*send the redirect http page*/
static void cplpl_http_send_redirect_page(FILE *fp, char *pUrl, 
				char *pInfo)
{
#if 1
	fprintf(fp,
	       "HTTP/1.1 302 Moved Temporarily\r\n"
	       "Content-type: text/html\r\n"
	       "Pragma: no-cache\r\n"
	       "Cache-Control: no-cache\r\n"
	       "Connection: close\r\n");
	if (s_session_id[0])
		fprintf(fp, "set-cookie: sessionid=%s\r\n", s_session_id);
	fprintf(fp, "Location: http://%s/%s\r\n", g_local_host, pUrl);
       fprintf(fp,"\r\n");
#else
	send_headers(fp, 200, "Ok", "Set-Cookie: Name=; path=/", "text/html" );
	fprintf(fp, "<html><head><script language='javascript'>\n");
	if(pInfo)
		fprintf(fp, "alert('%s');\n", pInfo);
	if (strstr(pUrl, "http://"))
		fprintf(fp, "parent.location='%s'\n", pUrl);
	else
		fprintf(fp, "parent.location='http://%s/%s'\n", g_local_host, pUrl);
	fprintf(fp, "</script></head><body></body></html>");
#endif	
	fflush( fp );
}

static void  cplpl_httpd_del_accept_fd(int fd)
{
	struct accept *ac;
	
	d_printf("accept %d close\n", fd);
	for (ac = s_clients; ac; ac = ac->next) {
		if (ac->fd == fd) {
			CDMG_LIST_DEL(struct accept , s_clients, ac);
			close(ac->fd);
			free(ac);			
			return;
		}
	}		
}

void cplpl_http_close_all_accept()
{
	struct accept *p, *q;
	
	for (p = s_clients; p;) {		
		q = p;
		p = p->next;
		close(q->fd);
		free(q);
	}
	s_clients = 0;
}

/*read an accept socket,and send the respond*/
static int cplpl_httpd_accept_process(int fd)
{
	int len = 0;
	FILE *fp =0;
	static char buf[2048] = {0};
	char *p, *q;

	memset(s_session_id, 0, sizeof(s_session_id));
	
	memset(buf, 0, sizeof(buf));
	len = read(fd, buf, 2047);
	d_printf("read %d bytes from %d\n", len, fd);

	if (len == 0) {
		d_printf("accept %d close\n", fd);
		cplpl_httpd_del_accept_fd(fd);
	}
	d_printf(
		"\n-------------------------------------------\n"
		"%s\n"
		"---------------------------------------------\n", 
		buf);
	if ((p = strstr(buf, "sessionid="))) {
		for (q = s_session_id, p += strlen("sessionid="); 
				*p && (*p != ' ' && *p != ';' && *p != '\r') && 
					q - s_session_id < sizeof(s_session_id);
				q++, p++)
			*q = *p;
		d_printf("sessionid=%s\n", s_session_id);				
	}
	
	
	if (len) {
		if ((fp = fdopen(fd, "w")) < 0) {
			perror("fdopen:");
			cplpl_httpd_del_accept_fd(fd);
			return -1;
		}

		cplpl_http_send_redirect_page(fp, s_captival_portal_page, 0);

		fclose(fp);
		cplpl_httpd_del_accept_fd(fd);
	}

	return 0;
}

