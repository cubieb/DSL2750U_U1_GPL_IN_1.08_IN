#ifndef _CONFIG_H_
#define _CONFIG_H_

#define PCP_SERVER "pcp_server"
#define PCP_MAP "pcp_map"
#define PCP_PEER "pcp_peer"
#define PROTOL "protol"
#define INT_PORT "int_port"
#define EXT_PORT "ext_port"
#define EXT_ADDR "ext_addr"
#define RM_PORT "rm_port"
#define LIFETIME "lifetime"
#define RM_ADDR6 "rm_addr6"
#define STATE "state"
#define INT_ADDR "int_addr"
#define THIRD_PARTY_STR "THIRD_PARTY"
#define PREFER_FAILURE_STR "PREFER_FAILURE"
#define FILTER_STR "FILTER"
#define PF_LEN "pf_len"

#define PCP_DRAFT_VERSION "draft-ietf-pcp-base-22"

#define DELT_ITEM '='
#define DELT_BLOCK_ST '('
#define DELT_BLOCK_END ')'

int read_config(char *cfile);

#endif
