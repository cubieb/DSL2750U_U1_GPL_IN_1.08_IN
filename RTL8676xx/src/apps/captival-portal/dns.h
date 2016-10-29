/**
* scb+ 2011-10-12
*
* file name: dns.h
*
* product the phony dns for captival portal function
*
*/

#define DNS_PERIOD 1800
#define DNS_RESOVLE_PERIOD 10
#define MAX_DNS_PACKET_SIZE 1600
#define DNS_RESOLVE_TIME_MAX 6

void cplpl_dns_del(const char *url);
void cplpl_dns_set(const char *url);
void cplpl_dns_process(int fd);
void cplpl_dns_rec_delete_all();
int   cplpl_dns_init();
