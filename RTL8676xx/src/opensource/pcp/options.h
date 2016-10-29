#ifndef _OPTIONS_H_
#define _OPTIONS_H_

/* 
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |  Option Code  |  Reserved     |       Option-Length           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * :                       (optional) data                         :
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *                   Figure 4: Options Header
 *  */
#define THIRD_PARTY 1
#define PREFER_FAILURE 2
#define FILTER 3

struct pcp_option {
	uint8_t code;
	uint16_t len;
	uint8_t *str;
	SLIST_ENTRY(pcp_option) link;
};
SLIST_HEAD(opt_head, pcp_option);

int add_option_string(uint8_t *pcp_ptr, struct pcp_option *opt);
int add_option(int type, struct opt_head *opt_h, struct pcp_option *popt);
void free_option(struct pcp_option *popt);
void free_all_options(struct opt_head *opt_h);
struct pcp_option *malloc_option(int code, int len, void *val);
struct pcp_option *malloc_option_by_name(const char *name);
#endif
