/* options.h */
#ifndef _OPTIONS_H
#define _OPTIONS_H

#include "packet.h"

#define TYPE_MASK	0x0F

enum {
	OPTION_IP=1,
	OPTION_IP_PAIR,
	OPTION_STRING,
	OPTION_BOOLEAN,
	OPTION_U8,
	OPTION_U16,
	OPTION_S16,
	OPTION_U32,
	OPTION_S32,
	OPTION_LESSROUTER
};

#define OPTION_REQ	0x10 /* have the client request this option */
#define OPTION_LIST	0x20 /* There can be a list of 1 or more of these */

struct dhcp_option {
	char name[10];
	char flags;
	unsigned char code;
};

extern struct dhcp_option options[];
extern int option_lengths[];

/* 具体Option的长度，必须在调用get_option后使用*/
extern int  indefinite_optlen;

int get_option_length(struct dhcpMessage *packet, int code);
unsigned char *get_option(struct dhcpMessage *packet, int code);
int end_option(unsigned char *optionptr);
int add_option_string(unsigned char *optionptr, unsigned char *string);
int add_simple_option(unsigned char *optionptr, unsigned char code, u_int32_t data);
struct option_set *find_option(struct option_set *opt_list, char code);
void attach_option(struct option_set **opt_list, struct dhcp_option *option, char *buffer, int length);

int CreateVClsIDoption(int type, char *VCID, unsigned short EnterpriseCode);
int CreatePrivateOption(unsigned char code, int ifid, char *privateopt);

int parser_lessrouteip(char *sr_option, char *dr_option, 
                              unsigned char *raw_option, const int len); 

#ifdef ENABLE_OPT250
unsigned char *clone_option(const unsigned char *src);

unsigned char *cat_option250(unsigned char **dest, const unsigned char *src);

void TW_printf(const void *buf, int len);

#define free_option(option) (indefinite_optlen >= 255) ? free(option) : free(NULL)
#endif

/* option header: 2 bytes + subcode headers (6) + option data (64*2+6);
   these are TR111 option data.    Option can be longer.  */
#define VENDOR_BRCM_ENTERPRISE_NUMBER     4413
#define VENDOR_ENTERPRISE_LEN             4    /* 4 bytes */
#define VENDOR_IDENTIFYING_INFO_LEN       142
#define VENDOR_CLASS_IDENTIFIER_LEN       142
#define VENDOR_IDENTIFYING_OPTION_CODE    125
#define VENDOR_OPTION_CODE_OFFSET         0
#define VENDOR_OPTION_LEN_OFFSET          1
#define VENDOR_OPTION_ENTERPRISE_OFFSET   2
#define VENDOR_OPTION_DATA_OFFSET         6
#define VENDOR_SUBCODE_AND_LEN_BYTES      2
#define VENDOR_DEVICE_OUI_SUBCODE            1
#define VENDOR_DEVICE_SERIAL_NUMBER_SUBCODE  2
#define VENDOR_DEVICE_PRODUCT_CLASS_SUBCODE  3
#define VENDOR_GATEWAY_OUI_SUBCODE           4
#define VENDOR_GATEWAY_SERIAL_NUMBER_SUBCODE 5
#define VENDOR_GATEWAY_PRODUCT_CLASS_SUBCODE 6
#define VENDOR_IDENTIFYING_FOR_GATEWAY       1
#define VENDOR_IDENTIFYING_FOR_DEVICE        2
#define VENDOR_VCLSID_FOR_GATEWAY            1
#define VENDOR_VCLSID_FOR_DEVICE             2
#define VENDOR_GATEWAY_OUI_MAX_LEN           6
#define VENDOR_GATEWAY_SERIAL_NUMBER_MAX_LEN 64
#define VENDOR_GATEWAY_PRODUCT_CLASS_MAX_LEN 64
#define VENDOR_PRIVATE_MAX_LEN              256



#ifdef ENABLE_OPT125

/* 字段都定义为char 型，防止出现内存对齐问题*/
struct opt125_t {
	char code, len;
	char enterprise_num[4];
	char data_len;
	char data[250];
};

int init_opt125(void);
void add_opt125(struct dhcpMessage *packet, struct dhcpMessage *oldpacket, struct dhcpOfferedAddr *lease);
void del_opt125(struct dhcpOfferedAddr *lease);

#endif


#endif
