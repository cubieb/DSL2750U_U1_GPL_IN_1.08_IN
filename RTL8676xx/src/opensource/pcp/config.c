#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "config.h"
#include "glbvars.h"
#include "pcp.h"
#include "options.h"

int g_flg_resend_forever = 0;
static struct pcp_option *opt = NULL;
static struct pcp_oper *oper = NULL;
map_t *mt = NULL;
peer_t *pt = NULL;
struct opt_head *opt_h = NULL;
static struct in_addr addr_4;

/* static int config_item(char *block, char *item, char *value); */

static int config_item(char *block, char *item, const char *value)
{
	if (block[0] == '\0') {
		if (strncmp(item, PCP_SERVER, sizeof(PCP_SERVER)) == 0) {
			printf("\tfunc=%s line=%d, item=%s, value=%s\n",
					__FUNCTION__, __LINE__, item, value );
			strncpy(g_servername, value, sizeof(g_servername));
			g_servername[sizeof(g_servername) - 1] = '\0';
		}
		else {
			return -1;
		}
	} else {
		if (!strncmp(block, PCP_MAP, sizeof(PCP_MAP))) {
			if (mt) {
				if (!strncmp(item, PROTOL, sizeof(PROTOL)))
					mt->protol = atoi(value);
				else if (!strncmp(item, INT_PORT, sizeof(INT_PORT)))
					mt->in_port = atoi(value);
				else if (!strncmp(item, EXT_PORT, sizeof(EXT_PORT)))
					mt->ext_port = atoi(value);
				else if (!strncmp(item, LIFETIME, sizeof(LIFETIME)))
					oper->lifetime = atoi(value);
				else if (!strncmp(item, EXT_ADDR, sizeof(EXT_ADDR))) {
					/* not a valid ipv6 address */
					if (!inet_pton(AF_INET6, value, &mt->addr6)) {
						if (inet_pton(AF_INET, value, &addr_4))
							IPV4_MAPPED_IPV6(addr_4, &mt->addr6);
					}

				}
				else if (!strncmp(item, STATE, sizeof(STATE))) {
					/* FIXME: for now just ignore the state value in
					 * the config file, and assume that o_state is
					 * PCP_INIT, because usually when the wan
					 * connection is up and start the pcp process, all
					 * pcp request need to renew because the wan
					 * address is changed*/
					oper->o_state = PCP_INIT;
				}
			}
		} else if (!strncmp(block, PCP_PEER, sizeof(PCP_PEER))) {
			if (pt) {
				if (!strncmp(item, PROTOL, sizeof(PROTOL)))
					pt->protol = atoi(value);
				else if (!strncmp(item, INT_PORT, sizeof(INT_PORT)))
					pt->in_port = atoi(value);
				else if (!strncmp(item, EXT_PORT, sizeof(EXT_PORT)))
					pt->ext_port = atoi(value);
				else if (!strncmp(item, RM_PORT, sizeof(RM_PORT)))
					pt->rm_port = atoi(value);
				else if (!strncmp(item, LIFETIME, sizeof(LIFETIME)))
					oper->lifetime = atoi(value);
				else if (!strncmp(item, EXT_ADDR, sizeof(EXT_ADDR))) {
					if (!inet_pton(AF_INET6, value, &pt->addr6)) {
						if (inet_pton(AF_INET, value, &addr_4))
							IPV4_MAPPED_IPV6(addr_4, &pt->addr6);
					}
				}
				else if (!strncmp(item, RM_ADDR6, sizeof(RM_ADDR6))) {
					if (!inet_pton(AF_INET6, value, &pt->rm_addr6)) {
						if (inet_pton(AF_INET, value, &addr_4))
							IPV4_MAPPED_IPV6(addr_4, &pt->rm_addr6);
					}
				}
				else if (!strncmp(item, STATE, sizeof(STATE))) {
					/* FIXME: for now just ignore the state value in
					 * the config file, and assume that o_state is
					 * PCP_INIT, because usually when the wan
					 * connection is up and start the pcp process, all
					 * pcp request need to renew because the wan
					 * address is changed*/
					oper->o_state = PCP_INIT;
				}
			}
		} else if (!strncmp(block, THIRD_PARTY_STR, sizeof(THIRD_PARTY_STR))) {
			if (!strncmp(item, INT_ADDR, sizeof(INT_ADDR))) {
				if (!inet_pton(AF_INET6, value, (struct in6_addr *)opt->str)) {
                    if (inet_pton(AF_INET, value, &addr_4))
                        IPV4_MAPPED_IPV6(addr_4, (struct in6_addr *)opt->str);
                }
            }
		}
	}
	return 0;
}

static int malloc_block(char *block)
{
	if (!strncmp(block, PCP_MAP, sizeof(PCP_MAP))) {
		if (mt)
			free(mt);


		if ((mt = (map_t *)malloc(sizeof(map_t))) == NULL) {
			return -1;
		}

		memset(mt, 0, sizeof(map_t));
		opt_h = &(mt->opt_h);
		SLIST_INIT(opt_h);

		oper = (struct pcp_oper *)malloc(sizeof(struct pcp_oper));
		if (oper == NULL)
			return -1;

		memset(oper, 0, sizeof(struct pcp_oper));
		oper->o_type = O_MAP;
		oper->data = mt;

	} else if (!strncmp(block, PCP_PEER, sizeof(PCP_PEER))) {
		if (pt)
			free(pt);

		if ((pt = (peer_t *)malloc(sizeof(peer_t))) == NULL) {
			return -1;
		}

		memset(pt, 0, sizeof(peer_t));
		opt_h = &pt->opt_h;
		SLIST_INIT(opt_h);

		oper = (struct pcp_oper *)malloc(sizeof(struct pcp_oper));
		if (oper == NULL)
			return -1;

		memset(oper, 0, sizeof(struct pcp_oper));
		oper->o_type = O_PEER;
		oper->data = pt;
	} else if (!strncmp(block, THIRD_PARTY_STR, sizeof(THIRD_PARTY_STR))) {
		opt = malloc_option_by_name(THIRD_PARTY_STR);
	} else if (!strncmp(block, PREFER_FAILURE_STR, sizeof(PREFER_FAILURE_STR))) {
		opt = malloc_option_by_name(PREFER_FAILURE_STR);
	} else if (!strncmp(block, FILTER_STR, sizeof(FILTER_STR))) {
		opt = malloc_option_by_name(FILTER_STR);
	}
	return 0;
}

static int null_block(char *block)
{
	if (block[0] != '\0') {
		if (!strncmp(block, THIRD_PARTY_STR, sizeof(THIRD_PARTY_STR)) ||
				!strncmp(block, PREFER_FAILURE_STR, sizeof(PREFER_FAILURE_STR)) ||
				!strncmp(block, FILTER_STR, sizeof(FILTER_STR))) {
			add_option(O_MAP, opt_h, opt);
			opt = NULL;
		}
	}
	return 0;
}

static int finalize()
{
	if (oper) {
		TAILQ_INSERT_TAIL(&oper_hd, oper, link);
		oper = NULL;
	}

	mt = NULL;
	pt = NULL;
	return 0;
}

int read_config(char *cfile)
{
	char buf[BUF_1024];
	char *p = NULL;
	FILE *f = fopen(cfile, "r");


	while (fgets(buf, sizeof(buf), f)) {
		char token[BUF_128] = {'\0'};
		char tmp[BUF_128] = {'\0'};

		char block[BUF_32] = {'\0'}; /* such as pcp_map( */ 

		char item[BUF_128] = {'\0'};
		char val[BUF_128] = {'\0'};

		int flg_delt = 0;
		int flg_block = 0;

		int it = 0;

		for (p=buf; *p; p++) {
			if (isspace(*p)) {
				token[it] = '\0';
				if (token[0] != '\0') {
					strncpy(tmp, token, sizeof(tmp));
				}

				if (flg_delt && (tmp[0] != '\0')) {
					strncpy(val, tmp, sizeof(val));
					tmp[0] = '\0';
					config_item(block, item, val);
					flg_delt = 0;
				}

				it = 0;
				token[it] = '\0';

				if (*p == '\n') {
					finalize();
				}
			}
		      else if (*p == DELT_ITEM) {
				token[it] = '\0';
				if (token[0] != '\0') {
					strncpy(tmp, token, sizeof(tmp));
				}

				if (tmp[0] != '\0') {
					strncpy(item, tmp, sizeof(item));
					flg_delt = 1;
					tmp[0] = '\0';
				}

				it = 0;
				token[it] = '\0';
			}
		      else if (*p == DELT_BLOCK_ST) {
                token[it] = '\0';
				if (token[0] != '\0')
					strncpy(tmp, token, sizeof(item));

				if (tmp[0] != '\0')
					strncpy(block, tmp, sizeof(block));

				malloc_block(block);
				flg_block = 1;

				it = 0;
				token[it] = '\0';
			}
		      else if (*p == DELT_BLOCK_END) {
                token[it] = '\0';
				if (token[0] != '\0')
					strncpy(tmp, token, sizeof(item));

				if (flg_delt && (tmp[0] != '\0')) {
					strncpy(val, tmp, sizeof(val));
					tmp[0] = '\0';
					config_item(block, item, val);
					flg_delt = 0;
				}
				flg_block = 0;
				null_block(block);
				block[0] = '\0';

				it = 0;
				token[it] = '\0';
			}
			else {
				token[it] = *p;
				if (it < (sizeof(token)-1))
					it++;
				else
					return -1;
			}
		}
	}
	return 0;
}
