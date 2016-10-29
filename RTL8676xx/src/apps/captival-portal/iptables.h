/**
* scb+ 2011-10-12
*
* file name: iptables.h
*
* create the iptable rule for captival portal function
*
*/
void cplpl_iptable_set(const char *url);
void cplpl_iptable_del(const char *url);
int   cplpl_iptable_dnat(int is_add, uint32_t raw_ip, uint32_t new_ip);
