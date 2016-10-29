#include "dproxy.h"

#ifdef CONFIG_TELEFONICA_BRAZIL
void dns_construct_reply( dns_request_t *m, int CaptivePortalflag);
#else
void dns_construct_reply( dns_request_t *m );
#endif
void dns_construct_error_reply(dns_request_t *m);
