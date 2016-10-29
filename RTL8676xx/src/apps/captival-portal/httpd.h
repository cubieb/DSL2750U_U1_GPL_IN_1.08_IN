/**
* scb+ 2011-10-12
*
* file name: httpd.h
*
* if receive the http package, send the captival portal page to the host
*
*/

void cplpl_httpd_set(const char *url);
void cplpl_httpd_del(const char *url);
void cplpl_http_close_all_accept();
int   cplpl_httpd_init();
int   cplpl_httpd_fd_set_accept(fd_set *fds);
int   cplpl_http_process(int httpd_fd);
int   cplpl_http_process_accept(fd_set *fds);

