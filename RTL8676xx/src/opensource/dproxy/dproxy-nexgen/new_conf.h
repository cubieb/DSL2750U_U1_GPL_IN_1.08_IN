/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENZHEN) Co., Ltd.
 文件名称: new_conf.h
 文件描述: 
 修订记录:
        1. 作者: lichuang
           日期: 2009-3-23
           内容: 创建文件
**********************************************************************/

#ifndef __NEW_CONF_H__
#define __NEW_CONF_H__

//#define USE_CONF

/**/
#define INTERFACE_LEN 	16
#define CONF_FILE_LEN 	64

#define IPV4_ENABLE      (1)
#define IPV6_ENABLE      (2)


#ifdef USE_IPV6
#define NUM_OF_PROTO 2
#define IP_LEN 	     64
#else
#define NUM_OF_PROTO 1
#define IP_LEN 	     20
#endif
#define NUM_OF_DNS 	 3

#define PROTO_IPV4 0
#define PROTO_IPV6 1

typedef struct ST_LAN_INFO
{
	char szName[INTERFACE_LEN];
	struct ST_LAN_INFO *pNext;
	char szDnsIp[NUM_OF_PROTO][NUM_OF_DNS][IP_LEN];
	char szCacheFile[CONF_FILE_LEN];
	int ipPro;
}ST_LAN_INFO;

/* 配置文件结构体 */
typedef struct 
{
	ST_LAN_INFO* pLanInfo;
	unsigned char ucDeamon;				/* 是否精灵化进程 */
	int nPurgeTime;						/* 清空的时间 */
	char szConfigFile[CONF_FILE_LEN];	/* 配置文件名 */
	char szHostFile[CONF_FILE_LEN];		/* HOST文件名 */
}ST_DNS_CONFIG;

extern ST_DNS_CONFIG g_stConfig;

/*************************************************************************
  功  能: 读取配置文件
  参  数: 
  		pConfig 配置文件路径
  返回值: 成功返回0, 失败返回-1  
*************************************************************************/
int load_config(const char* pConfig);

#endif /* __NEW_CONF_H__ */

