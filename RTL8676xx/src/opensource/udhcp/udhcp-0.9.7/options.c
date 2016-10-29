/*
 * options.c -- DHCP server option packet tools
 * Rewrite by Russ Dill <Russ.Dill@asu.edu> July 2001
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "debug.h"
#include "dhcpd.h"
#include "files.h"
#include "options.h"
#include "leases.h"


/* supported options are easily added here */
struct dhcp_option options[] = {
	/* name[10]	flags					code */
	{"subnet",	OPTION_IP | OPTION_REQ,			0x01},
	{"timezone",	OPTION_S32,				0x02},
	{"router",	OPTION_IP | OPTION_LIST | OPTION_REQ,	0x03},
	{"timesvr",	OPTION_IP | OPTION_LIST,		0x04},
	{"namesvr",	OPTION_IP | OPTION_LIST,		0x05},
	{"dns",		OPTION_IP | OPTION_LIST | OPTION_REQ,	0x06},
	{"logsvr",	OPTION_IP | OPTION_LIST,		0x07},
	{"cookiesvr",	OPTION_IP | OPTION_LIST,		0x08},
	{"lprsvr",	OPTION_IP | OPTION_LIST,		0x09},
	{"hostname",	OPTION_STRING | OPTION_REQ,		0x0c},
	{"bootsize",	OPTION_U16,				0x0d},
	{"domain",	OPTION_STRING | OPTION_REQ,		0x0f},
	{"swapsvr",	OPTION_IP,				0x10},
	{"rootpath",	OPTION_STRING,				0x11},
	{"ipttl",	OPTION_U8,				0x17},
	{"mtu",		OPTION_U16,				0x1a},
	{"broadcast",	OPTION_IP | OPTION_REQ,			0x1c},
	{"ntpsrv",	OPTION_IP | OPTION_LIST,		0x2a},
	{"wins",	OPTION_IP | OPTION_LIST,		0x2c},
	{"requestip",	OPTION_IP,				0x32},
	{"lease",	OPTION_U32,				0x33},
	{"dhcptype",	OPTION_U8,				0x35},
	{"serverid",	OPTION_IP,				0x36},
	{"message",	OPTION_STRING,				0x38},
	{"tftp",	OPTION_STRING,				0x42},
	{"bootfile",	OPTION_STRING,				0x43},
	{"lessroute", OPTION_LESSROUTER | OPTION_REQ, 0x79},
	{"240",	OPTION_STRING,				0xf0},
	{"241",	OPTION_STRING,				0xf1},
	{"242",	OPTION_STRING,				0xf2},
	{"243",	OPTION_STRING,				0xf3},
	{"244",	OPTION_STRING,				0xf4},
	{"245",	OPTION_STRING,				0xf5},
	{"",		0x00,				0x00}
};

/* Lengths of the different option types */
int option_lengths[] = {
	[OPTION_IP] =		4,
	[OPTION_IP_PAIR] =	8,
	[OPTION_BOOLEAN] =	1,
	[OPTION_STRING] =	1,
	[OPTION_U8] =		1,
	[OPTION_U16] =		2,
	[OPTION_S16] =		2,
	[OPTION_U32] =		4,
	[OPTION_S32] =		4
};

int  indefinite_optlen = 0;

/* get an option with bounds checking (warning, not aligned). */
int get_option_length(struct dhcpMessage *packet, int code)
{
	int i, length;
	unsigned char *optionptr;
	int over = 0, done = 0, curr = OPTION_FIELD;

	optionptr = packet->options;
	i = 0;
	length = 308;
	while (!done) {
		if (i >= length) {
			LOG(LOG_WARNING, "bogus packet, option fields too long.");
			return NULL;
		}
		if (optionptr[i + OPT_CODE] == code) {
			if (i + 1 + optionptr[i + OPT_LEN] >= length) {
				LOG(LOG_WARNING, "bogus packet, option fields too long.");
				return 0;
			}
			return optionptr[i + OPT_LEN];
		}
		switch (optionptr[i + OPT_CODE]) {
		case DHCP_PADDING:
			i++;
			break;
		case DHCP_OPTION_OVER:
			if (i + 1 + optionptr[i + OPT_LEN] >= length) {
				LOG(LOG_WARNING, "bogus packet, option fields too long.");
				return 0;
			}
			over = optionptr[i + 3];
			i += optionptr[OPT_LEN] + 2;
			break;
		case DHCP_END:
			if (curr == OPTION_FIELD && over & FILE_FIELD) {
				optionptr = packet->file;
				i = 0;
				length = 128;
				curr = FILE_FIELD;
			} else if (curr == FILE_FIELD && over & SNAME_FIELD) {
				optionptr = packet->sname;
				i = 0;
				length = 64;
				curr = SNAME_FIELD;
			} else done = 1;
			break;
		default:
			i += optionptr[OPT_LEN + i] + 2;
		}
	}
	return 0;
}


/* get an option with bounds checking (warning, not aligned). */
unsigned char *get_option(struct dhcpMessage *packet, int code)
{
	int i, length;
	unsigned char *optionptr;
    #ifdef ENABLE_OPT250
    	unsigned char *extend_option = NULL;
    #endif
	int over = 0, done = 0, curr = OPTION_FIELD;

	optionptr = packet->options;
	i = 0;
    #ifdef ENABLE_OPT250
    	length = 1024;  /* enlarge for option250 */
    #else
	length = 308;
    #endif
	while (!done) {
		if (i >= length) {
			LOG(LOG_WARNING, "bogus packet, option fields too long.");
			return NULL;
		}
		if (optionptr[i + OPT_CODE] == code) {
			if (i + 1 + optionptr[i + OPT_LEN] >= length) {
				LOG(LOG_WARNING, "bogus packet, option fields too long.");
				return NULL;
			}

                    #ifndef ENABLE_OPT250
                      indefinite_optlen = optionptr[i+ OPT_LEN];
                      return optionptr + i + 2;
                    #else
                     /* 正常长度的Option */
                     if (optionptr[i + OPT_LEN] < 255) {
                        indefinite_optlen = optionptr[i+ OPT_LEN];
                        return optionptr + i + 2;
                     }
                     else { /* 长度超过255的Option */
                        extend_option = clone_option(optionptr + i);
                        if (NULL == extend_option) {
				LOG(LOG_WARNING, "clone option failed.");
				return NULL;
			   }
                        i += 255 + 2;
                        indefinite_optlen = 255;
                        while (optionptr[i + OPT_CODE] == DHCP_MICROSOFT) {
                            extend_option = cat_option250(&extend_option, optionptr + i);
                            i += optionptr[i + OPT_LEN] + 2;
                        }
                        return extend_option + 2;
                     }
                    #endif
		}
		switch (optionptr[i + OPT_CODE]) {
		case DHCP_PADDING:
			i++;
			break;
		case DHCP_OPTION_OVER:
			if (i + 1 + optionptr[i + OPT_LEN] >= length) {
				LOG(LOG_WARNING, "bogus packet, option fields too long.");
				return NULL;
			}
			over = optionptr[i + 3];
			i += optionptr[OPT_LEN] + 2;
			break;
		case DHCP_END:
			if (curr == OPTION_FIELD && over & FILE_FIELD) {
				optionptr = packet->file;
				i = 0;
				length = 128;
				curr = FILE_FIELD;
			} else if (curr == FILE_FIELD && over & SNAME_FIELD) {
				optionptr = packet->sname;
				i = 0;
				length = 64;
				curr = SNAME_FIELD;
			} else done = 1;
			break;
		default:
			i += optionptr[OPT_LEN + i] + 2;
		}
	}
	return NULL;
}


/* return the position of the 'end' option (no bounds checking) */
int end_option(unsigned char *optionptr)
{
	int i = 0;

	while (optionptr[i] != DHCP_END) {
		if (optionptr[i] == DHCP_PADDING) i++;
		else i += optionptr[i + OPT_LEN] + 2;
	}
	return i;
}


/* add an option string to the options (an option string contains an option code,
 * length, then data) */
int add_option_string(unsigned char *optionptr, unsigned char *string)
{
	int end = end_option(optionptr);

	/* end position + string length + option code/length + end option */
	if (end + string[OPT_LEN] + 2 + 1 >= 308) {
		LOG(LOG_ERR, "Option 0x%02x did not fit into the packet!", string[OPT_CODE]);
		return 0;
	}
	DEBUG(LOG_INFO, "adding option 0x%02x", string[OPT_CODE]);
	memcpy(optionptr + end, string, string[OPT_LEN] + 2);
	optionptr[end + string[OPT_LEN] + 2] = DHCP_END;
	return string[OPT_LEN] + 2;
}


/* add a one to four byte option to a packet */
int add_simple_option(unsigned char *optionptr, unsigned char code, u_int32_t data)
{
	char length = 0;
	int i;
	unsigned char option[2 + 4];
	unsigned char *u8;
	u_int16_t *u16;
	u_int32_t *u32;
	u_int32_t aligned;
	u8 = (unsigned char *) &aligned;
	u16 = (u_int16_t *) &aligned;
	u32 = &aligned;

	for (i = 0; options[i].code; i++)
		if (options[i].code == code) {
			length = option_lengths[options[i].flags & TYPE_MASK];
		}

	if (!length) {
		DEBUG(LOG_ERR, "Could not add option 0x%02x", code);
		return 0;
	}

	option[OPT_CODE] = code;
	option[OPT_LEN] = length;

	switch (length) {
		case 1: *u8 =  data; break;
		case 2: *u16 = data; break;
		case 4: *u32 = data; break;
	}
	memcpy(option + 2, &aligned, length);
	return add_option_string(optionptr, option);
}


/* find option 'code' in opt_list */
struct option_set *find_option(struct option_set *opt_list, char code)
{
	while (opt_list && opt_list->data[OPT_CODE] < code)
		opt_list = opt_list->next;

	if (opt_list && opt_list->data[OPT_CODE] == code) return opt_list;
	else return NULL;
}


/* add an option to the opt_list */
void attach_option(struct option_set **opt_list, struct dhcp_option *option, char *buffer, int length)
{
	struct option_set *existing, *new, **curr;

	/* add it to an existing option */
	if ((existing = find_option(*opt_list, option->code))) {
		DEBUG(LOG_INFO, "Attaching option %s to existing member of list", option->name);
		if (option->flags & OPTION_LIST) {
			if (existing->data[OPT_LEN] + length <= 255) {
				existing->data = realloc(existing->data,
						existing->data[OPT_LEN] + length + 2);
				memcpy(existing->data + existing->data[OPT_LEN] + 2, buffer, length);
				existing->data[OPT_LEN] += length;
			} /* else, ignore the data, we could put this in a second option in the future */
		} /* else, ignore the new data */
	} else {
		DEBUG(LOG_INFO, "Attaching option %s to list", option->name);

		/* make a new option */
		new = malloc(sizeof(struct option_set));
		new->data = malloc(length + 2);
		new->data[OPT_CODE] = option->code;
		new->data[OPT_LEN] = length;
		memcpy(new->data + 2, buffer, length);

		curr = opt_list;
		while (*curr && (*curr)->data[OPT_CODE] < option->code)
			curr = &(*curr)->next;

		new->next = *curr;
		*curr = new;
	}
}

int CreateVClsIDoption(int type, char *VCID, unsigned short EnterpriseCode)
{
/* Customer please define your parameter here */
#define ENTERPRISE_CODE_OFFSET 2
#define ENTERPRISE_CODE 0x0000

#define FLDVENDOR "TW"
#define FLDCATEGORY "CPE"
#define FLDMODEL    "TBSROUTER"
#define FLDVERSION  "1.0"
#define FLDPROTOCOL 0x00010017

	char optionData[VENDOR_CLASS_IDENTIFIER_LEN], *dataPtr;
	int len = 0;
	optionData[0] = 60;
	dataPtr = optionData + ENTERPRISE_CODE_OFFSET;

	memcpy(dataPtr, &EnterpriseCode, 2);
	len += sizeof(unsigned short);
	dataPtr += sizeof(unsigned short);
	//printf("the sizeof short is %d\r\n", sizeof(unsigned short));

	/*for vendor */
	*dataPtr++ = 1;
	len++;

	*dataPtr++ = sizeof(FLDVENDOR)-1;
	len++;

	strncpy(dataPtr, FLDVENDOR, sizeof(FLDVENDOR)-1);
	dataPtr += sizeof(FLDVENDOR)-1;
	len += sizeof(FLDVENDOR)-1;

	/*for Categray*/

	*dataPtr++ = 2;
	len++;

	*dataPtr++ = sizeof(FLDCATEGORY)-1;
	len++;

	strncpy(dataPtr, FLDCATEGORY, sizeof(FLDCATEGORY)-1);
	dataPtr += sizeof(FLDCATEGORY)-1;
	len += sizeof(FLDCATEGORY)-1;


	/*for model*/

	*dataPtr++ = 3;
	len++;

	*dataPtr++ = sizeof(FLDMODEL)-1;
	len++;

	strncpy(dataPtr, FLDMODEL, sizeof(FLDMODEL)-1);
	dataPtr += sizeof(FLDMODEL)-1;
	len += sizeof(FLDMODEL)-1;


	/*for version*/

	*dataPtr++ = 4;
	len++;

	*dataPtr++ = sizeof(FLDVERSION)-1;
	len++;

	strncpy(dataPtr, FLDVERSION, sizeof(FLDVERSION)-1);
	dataPtr += sizeof(FLDVERSION)-1;
	len += sizeof(FLDVERSION)-1;

	/*for protocol*/
	if ( VENDOR_IDENTIFYING_FOR_DEVICE == type ) {
		*dataPtr++ = 5;
		len++;

		*dataPtr++ = 4;
		len++;

		*(uint32_t *)dataPtr = FLDPROTOCOL;
		dataPtr += 4;
		len += sizeof(uint32_t);
	}
	optionData[1] = len;
	memcpy((void*)VCID,(void*)optionData,len+2);
}

int CreatePrivateOption(unsigned char code, int ifid, char *privateopt)
{
	int len = 0;
    FILE *fp = NULL;
    char optionData[VENDOR_PRIVATE_MAX_LEN] = {0};

#if 0 /* only for test: record the result to DEFAULT_OPCH */ 
    char temp[20] = ":::::239.0.5.2:22222";
	if (!(fp = fopen(server_config[ifid].opchfile, "w"))) 
    {
                  server_config[ifid].opchfile);*/
	}     
    fwrite(temp, sizeof(char), 20, fp);
    fclose(fp);
#endif

    optionData[0] = code;
    switch(code)
    {
        case DHCP_PRIVATE_240:
            /* 打开配置文件读取OPCH配置 */
        	if (!(fp = fopen(server_config[ifid].opchfile, "r"))) 
            {
        		dhcp_log("unable to open config file: %s\n", server_config[ifid].opchfile);
        		return -1;
        	}       
            if (fgets(optionData+2, 20+1, fp))
            {
                len = 20;
            }
            fclose(fp);
            break;
            
        default:
            dhcp_log("unable to interprete this private option: %d\r\n", code);
            return -1;
    }

	optionData[1] = len;
	memcpy((void*)privateopt,(void*)optionData,len+2);

    return 0;
}


/* added by wangtao for dhcp option 121: classless static routes. 
   make the option into the format which can be interpreted by shell script */
int parser_lessrouteip(char *sr_option, char *dr_option, 
                              unsigned char *raw_option, const int len) 
{ 
    u_int32_t umask; 
    int masklen; 
    unsigned char *nonius = raw_option; 
    u_int8_t* u; 
    int n; 
    u_int8_t ip_t[4]; 
    int len_t = 0; 

    dhcp_log("parser_lessrouteip\n");

    if ((sr_option == NULL) || (dr_option == NULL) 
        || (raw_option == NULL) || (len <= 0))
    {
        dhcp_log("bad input para\r\n");
        return -1;
    }

    while ((nonius - raw_option) < len) 
    { 
        masklen = *nonius;
        /* parse whether it is a default route */
        if (0 == masklen)
        {
            memcpy(dr_option, ++nonius, 4);
            nonius += 4;
            continue;
        }

        /* parse normal static routes */
        umask = 0xFFFFFFFF; 
        umask = umask << (32 - masklen); //0xFFFFFF00
        u = ((u_int8_t*)&umask); /* big endian */

        bzero(ip_t, 4); 
        for (++nonius, n=0; 
        ((ip_t[n]=(*nonius)&(*u))!=0) && n<4; ++n, ++u, ++nonius); 

        len_t += sprintf(sr_option+len_t, "%s %d.%d.%d.%d/%d ", 
                         "-net", ip_t[0], ip_t[1], ip_t[2], ip_t[3], masklen); 

        for (n=0; n<4; ++n, ++nonius) 
        { 
            ip_t[n] = *nonius; 
        } 
        len_t += sprintf(sr_option+len_t, "%s %d.%d.%d.%d|", 
                         "gw", ip_t[0], ip_t[1], ip_t[2], ip_t[3]); 
    } 

    if (len_t > 0)
    {
        sr_option[--len_t] = '\0'; 
    }

    return len_t; 

} 

char  *get_opt60_sub(char *opt, char fld)
{
	char total_len, fld_len;
	char *optionptr, *pfld;
	pfld = NULL;
	total_len = *(opt-1);
	if(total_len <= 2) {
		return NULL;
	}
	optionptr = opt + 2;
	total_len -= 2;
	while(total_len > 0){
		if(*optionptr > 0 && *optionptr < 64){ /* 包含电信要求的子属性 */
			if(*optionptr == fld){
				pfld = optionptr;
				break;
			} else {
				fld_len = *(optionptr+1);
				optionptr += (fld_len + 2);
				total_len -= (fld_len + 2);
			}
		} else {
			return NULL;
		}
	}
	return pfld;
}

#ifdef ENABLE_OPT250

void TW_printf(const void *buf, int len)
{
    const int LINE_WIDTH = 16;
    int i = 0, j = 0;
    int tail = 0;
    const unsigned char *pucBuf = NULL;

    if (len == 0 || NULL == buf)
    {
        return;
    }

    pucBuf = buf;

    for(; i<len; i++)
    {
        if(i>0 && i%LINE_WIDTH == 0)
        {
            j = i - LINE_WIDTH;
            printf("; ");
            for(; j<i; j++)
            {
                if(pucBuf[j] < 16)
                    printf("%c", '.');
                else
                    printf("%c", pucBuf[j]);
            }

            printf("\x0a\x0d");
        }

        printf("%02X ", pucBuf[i]);
    }

    tail = len%LINE_WIDTH == 0 ? len-LINE_WIDTH:(len/LINE_WIDTH)*LINE_WIDTH;
    if(tail != len-LINE_WIDTH)
    {
        for(i=0; i<48-(len-tail)*3; i++)
        {
            printf("%c", ' ');
        }
    }

    printf("; ");

    for(i=tail; i<len; i++)
    {
        if(pucBuf[i] < 16)
            printf("%c", '.');
        else
            printf("%c", pucBuf[i]);
    }

    printf("\x0a\x0d");
}

/* 克隆一个Option */
unsigned char *clone_option(const unsigned char *src)
{
    unsigned char *dest = NULL;
    dest = (unsigned char *)malloc(src[OPT_LEN] + 2);
    if (!dest) {
        LOG(LOG_INFO, "clone_option###memery alloc failed");
        return NULL;
    }

    memset(dest, 0, src[OPT_LEN] + 2);

    dest[OPT_CODE] = src[OPT_CODE];
    dest[OPT_LEN] = src[OPT_LEN];
    memcpy(dest + 2, src + 2, src[OPT_LEN]);

    return dest;
}

/* 将第二个Option拼接到第一个Option */
unsigned char *cat_option250(unsigned char **dest, const unsigned char *src)
{
    unsigned char *org_option = *dest;
    unsigned char *new_option = NULL;
    int length = indefinite_optlen + src[OPT_LEN];

    new_option = (unsigned char *)realloc(org_option, length + 2);
    if (!new_option)
        return NULL;

    memcpy(new_option + org_option[OPT_LEN] + 2, src + 2, src[OPT_LEN]);
    *dest = new_option;

    indefinite_optlen += src[OPT_LEN];

    return *dest;
}
#endif



#ifdef ENABLE_OPT125

#define DSL_FORUM_ENTERPRISE_NUM 3561

#define SUBOPT_DEVICE_OUI_CODE 1
#define SUBOPT_DEVICE_SN_CODE 2
#define SUBOPT_DEVICE_PC_CODE 3

#define SUBOPT_GATEWAY_OUI_CODE 4
#define SUBOPT_GATEWAY_SN_CODE 5
#define SUBOPT_GATEWAY_PC_CODE 6

#define MAX_OUI_SUBOPT_LEN 6
#define MAX_SN_SUBOPT_LEN 64
#define MAX_PC_SUBOPT_LEN 64

#define MAX_SUBOPT_LEN MAX_PC_SUBOPT_LEN

#define MAX_LINE_LEN 100

#define DEF_OPT125_SUBOPT_VALUE "-"


struct opt125_t opt125_content = { DHCP_VI_VENDOR_SPEC };

/* 从文件中读取字段值初始化选项125 的内容*/
int init_opt125()
{
  	FILE *file = NULL;
	char line[MAX_LINE_LEN] = { 0 };
	char subopt[MAX_SUBOPT_LEN + 1] = { 0 };
	char *ret = NULL;
	char *pos = NULL;
	unsigned char len = 0;
	int count = 0;

	file = fopen("/var/vendor", "r");
	if ( NULL == file )
	{
		return -1;
	}

	/* 企业码*/
	*((unsigned int *)(opt125_content.enterprise_num)) = ntohl(DSL_FORUM_ENTERPRISE_NUM);

	pos = opt125_content.data;

	/* 填充子选项内容*/
	ret = fgets(line, MAX_LINE_LEN, file);
	count = sscanf(line, "GatewayManufacturerOUI %s", subopt);
	if ( NULL == ret || 1 != count )
	{
		return -1;
	}

	len = strlen(subopt);
	if ( len > MAX_OUI_SUBOPT_LEN )
	{
		len = MAX_OUI_SUBOPT_LEN;
	}

	*pos++ = SUBOPT_GATEWAY_OUI_CODE;
	*pos++ = len;
	memcpy(pos, subopt, len);
	pos += len;

	/* 填充子选项内容*/
	ret = fgets(line, MAX_LINE_LEN, file);
	count = sscanf(line, "GatewaySerialNumber %s", subopt);
	if ( NULL == ret || 1 != count )
	{
		return -1;
	}

	len = strlen(subopt);
	if ( len > MAX_SN_SUBOPT_LEN )
	{
		len = MAX_SN_SUBOPT_LEN;
	}

	*pos++ = SUBOPT_GATEWAY_SN_CODE;
	*pos++ = len;
	memcpy(pos, subopt, len);
	pos += len;

	/* 填充子选项内容*/
	ret = fgets(line, MAX_LINE_LEN, file);
	count = sscanf(line, "GatewayProductClass %s", subopt);
	if ( NULL == ret || 1 != count )
	{
		return -1;
	}

	len = strlen(subopt);
	if ( len > MAX_PC_SUBOPT_LEN )
	{
		len = MAX_PC_SUBOPT_LEN;
	}

	*pos++ = SUBOPT_GATEWAY_PC_CODE;
	*pos++ = len;
	memcpy(pos, subopt, len);
	pos += len;

	fclose(file);

	/* 数据长度 */
	opt125_content.data_len = pos - opt125_content.data;

	/* 选项总长度 */
	opt125_content.len = opt125_content.data_len + 5;

	return 0;
}

/* 当请求报文中包含选项125 时，为响应报文添加选项125 ，并打印添加了ManageableDevice */
void add_opt125(struct dhcpMessage *packet, struct dhcpMessage *oldpacket, struct dhcpOfferedAddr *lease)
{
	unsigned int en_num = 0;
	char subopt_code = 0;
	short total_len = 0;
	short len = 0;
    char choseflag = 0;
	char *opt = NULL;
	char *ip = NULL;
	char oui[MAX_OUI_SUBOPT_LEN + 1] = { 0, };
	char sn[MAX_SN_SUBOPT_LEN + 1] = { 0, };
	char pc[MAX_PC_SUBOPT_LEN + 1] = { 0, };
	char line[256] = { 0 };
	struct in_addr addr;

	if ( NULL == packet || NULL == oldpacket )
	{
		return;
	}

	opt = get_option(oldpacket, DHCP_VI_VENDOR_SPEC);
	if ( NULL == opt )
	{
		return;
	}

	total_len = *(opt - 1);

	/* find the part whose enterprise num is DSL_FORUM_ENTERPRISE_NUM */
	while ( total_len >= 4 )
	{
		/* parse enterprise num */
		en_num = ntohl(*((unsigned int *)opt));
		opt += 4;
		total_len -= 4;

		if ( DSL_FORUM_ENTERPRISE_NUM == en_num )
		{
		    choseflag = 1;
			break;
		}

		/* move the option data */
		len = *opt + 1;
        if (len > total_len)
        {
            dhcp_log("bogus packet, option fields is illegal.");
            LOG(LOG_WARNING, "bogus packet, option fields is illegal.");
            return;
        }
		opt += len;
		total_len -= len;
	}
    
    if (!choseflag)
    { 
        /* available enterprise number dosenot exist */
        dhcp_log("there is no available enterprise num, return\n");
        return;
    }

	/* 数据字段长度*/
	len = *opt++;

	/* 数据字段长度和总长度不匹配*/
	if ( total_len < len + 1 )
	{
	    dhcp_log("bogus packet, date fields is illegal.");
	    LOG(LOG_WARNING, "bogus packet, date fields is illegal.");
		return;
	}

	/* 从数据字段中解析子选项1、2 和3 的内容*/
	total_len = len;
	while ( total_len >= 2 )
	{
		/* 获取子选项code 和长度*/
		subopt_code = *opt++;
		len = *opt++;
        if (len > total_len)
        {
            dhcp_log("bogus packet, date fields is illegal.");
    	    LOG(LOG_WARNING, "bogus packet, suboption len fields is illegal.");
    		return;
        }
		total_len -= 2;

		switch ( subopt_code )
		{
			case SUBOPT_DEVICE_OUI_CODE:
				memcpy(oui, opt, len > MAX_OUI_SUBOPT_LEN ? MAX_OUI_SUBOPT_LEN : len);
				oui[len] = '\0';
				break;

			case SUBOPT_DEVICE_SN_CODE:
				memcpy(sn, opt, len > MAX_SN_SUBOPT_LEN ? MAX_SN_SUBOPT_LEN : len);
				sn[len] = '\0';
				break;

			case SUBOPT_DEVICE_PC_CODE:
				memcpy(pc, opt, len > MAX_PC_SUBOPT_LEN ? MAX_PC_SUBOPT_LEN : len);
				pc[len] = '\0';
				break;

			default:
				break;
		}

		/* 下一个子选项*/
		opt += len;
		total_len -= len;
	}

	/* 为子选项提供缺省值是为了方便适配模块进行输出匹配*/
	if ( 0 == strlen(oui) )
	{
		strcpy(oui, DEF_OPT125_SUBOPT_VALUE);
	}

	if ( 0 == strlen(sn) )
	{
		strcpy(sn, DEF_OPT125_SUBOPT_VALUE);
	}

	if ( 0 == strlen(pc) )
	{
		strcpy(pc, DEF_OPT125_SUBOPT_VALUE);
	}

	/* 打印添加信息，供适配模块使用*/
	if ( lease )
	{
		/* 记录客户端获取地址时使用了选项125 */
		lease->opt125 = 1;

		/* 通知适配模块添加ManageableDevice */
		addr.s_addr = lease->yiaddr;
		ip = inet_ntoa(addr);
		sprintf(line, "add ManageableDevice %s %s %s %s", ip, oui, sn, pc);
		LOG(LOG_INFO, "%s", line);
	}
	else
	{
	    /* 记录客户端获取地址时使用了选项125 */
		//lease->opt125 = 1;

		/* 通知适配模块添加ManageableDevice */
		addr.s_addr = oldpacket->ciaddr;
		ip = inet_ntoa(addr);
		sprintf(line, "add ManageableDevice %s %s %s %s", ip, oui, sn, pc);
		LOG(LOG_INFO, "%s", line);
	}

	/* 为响应报文添加选项125 */
	add_option_string(packet->options, (char *)&opt125_content);

	return;
}

/* 当获取地址时使用了选项125 的客户端释放时，打印删除信息*/
void del_opt125(struct dhcpOfferedAddr *lease)
{
	char line[80] = { 0 };
	char *ip = NULL;
	struct in_addr addr;

	if ( NULL == lease )
	{
		return;
	}

	/* 判断客户端获取地址时是否使用了选项125 */
	if ( lease->opt125 )
	{
		/* 通知适配模块删除ManageableDevice */
		addr.s_addr = lease->yiaddr;
		ip = inet_ntoa(addr);
		sprintf(line, "del ManageableDevice %s", ip);
		LOG(LOG_INFO, "%s", line);
	}

	return;
}

#endif


