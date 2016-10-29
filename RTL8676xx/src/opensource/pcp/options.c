/**
 * @file options.c
 * @brief 
 * @author wangjiaming
 * @date 2011-06-15
 */
#include <string.h>
#include <stdlib.h>
#include "pcp.h"
#include "options.h"

#define MANY = 0xff;

#define LEN_32 32
static struct option {
	uint8_t  name[LEN_32];
	uint8_t  flags; /* option value type */ 
	uint8_t  code; /* option number */ 
	uint16_t len;
	uint8_t  valid; /* which opcode is valid, or-bitwise */ 
};

static enum {
	OPT_NONE = 0,
	OPT_ADDR6, /* ipv6 address */ 
	OPT_FILTER, /*  */ 
};

#define OPT_LIST 0x80
#define OPT_TYPE_MASK 0xff


/* supported options are easily added here */
static struct option options[] = {
	/* name[24] 		flag,				code, 		len,  valid*/
	{"THIRD_PARTY", 		OPT_ADDR6, 			THIRD_PARTY,	16, 	O_MAP|O_PEER},
	{"PREFER_FAILURE", 	OPT_NONE, 			PREFER_FAILURE, 	0, 	O_MAP},
	{"FILTER", 		 	OPT_FILTER|OPT_LIST, 	FILTER, 	 	20, 	O_MAP}
};

int add_option_string(uint8_t *pcp_str, struct pcp_option *opt)
{
	pcp_str[0] = opt->code;
	pcp_str[1] = 0;
	*((uint16_t *)(&pcp_str[2])) = htons(opt->len);
	memcpy(&pcp_str[4], opt->str, opt->len);
}

/**
 * @brief add pcp_option to the option list
 *
 * @param opt_h
 * @param popt  the memory of it will be freed if it is not added to the list
 *
 * @return NULL is returned if popt is added to the opt_h list, otherwise popt
 * is returned, in order for the callee to call the function free_option
 */
int add_option(int type, struct opt_head *opt_h, struct pcp_option *popt)
{
	int i;
	struct option *opt = NULL;
	struct pcp_option *tmp_opt = NULL;

	if (popt == NULL) /* can't find */
    { 
		return -1;
    }

	for (i=0; i<sizeof(options); i++) {
		if (options[i].code == popt->code) {
			opt = &options[i];
		}
	}
	if (opt == NULL) /* can't find */ 
		return -1;

	if (!(opt->valid & type)) /* this options not allowed */ 
		return -1;

	SLIST_FOREACH(tmp_opt, opt_h, link) {
		if (tmp_opt->code == popt->code) {
			if ((opt->flags & OPT_LIST)) { /* multiple instances */ 
				if (memcmp(tmp_opt->str, popt->str, popt->len) != 0) {
					SLIST_INSERT_HEAD(opt_h, popt, link);
					popt = NULL;
				}
			}
			else {
				if (memcmp(tmp_opt->str, popt->str, popt->len) != 0) {
					memcpy(tmp_opt->str, popt->str, popt->len);
				}
			}
			break;
		}
	}

	if (tmp_opt == NULL) {
		SLIST_INSERT_HEAD(opt_h, popt, link);
		popt = NULL;
	}
	return PCP_OK;
}

void free_option(struct pcp_option *popt)
{
	if (popt == NULL)
		return;
	if (popt->str)
		free(popt->str);
	free(popt);
}

void free_all_options(struct opt_head *opt_h)
{
	struct pcp_option *option = NULL;
	struct pcp_option *tmp_opt = NULL;
	/* singled list, the last entry should be NULL */
	for (option = SLIST_FIRST(opt_h); option != NULL; option = tmp_opt) {
		tmp_opt = SLIST_NEXT(option, link);
		free_option(option);
	}
}

struct pcp_option *malloc_option(int code, int len, void *val)
{
	struct pcp_option *popt = NULL;
    
	uint8_t *opt_val = malloc(len);
	if (opt_val == NULL)
		return NULL;
    if (len > 0)
        memcpy(opt_val, val, len);
	popt = malloc(sizeof(struct pcp_option));
	if (popt) {
		popt->code = code;
		popt->len = len;
		popt->str = opt_val;
	}
	return popt;
}

struct pcp_option *malloc_option_by_name(const char *name)
{
	struct pcp_option *popt = NULL;
	struct option *opt = NULL;
	int i = 0;

	for (i=0; i<sizeof(options); i++) {
		if (!strcmp(options[i].name, name))
			opt = &options[i];
	}

	if (opt == NULL)
		return NULL;

	popt = malloc(sizeof(struct pcp_option));
	if (popt) {
		popt->code = opt->code;
		popt->len = opt->len;
		popt->str = malloc(opt->len);
		if (popt->str)
			memset(popt->str, 0, opt->len);
		else {
			free(popt);
			return NULL;
		}
	}
	return popt;
}
