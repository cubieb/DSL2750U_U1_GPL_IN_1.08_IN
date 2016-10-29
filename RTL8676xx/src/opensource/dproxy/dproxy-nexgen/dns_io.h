#include "dproxy.h"

int dns_read_packet(int sock, dns_request_t *m, int protocol, unsigned char ucFlag);
int dns_write_packet(int sock, unaddr *addr, int port, dns_request_t *m);
