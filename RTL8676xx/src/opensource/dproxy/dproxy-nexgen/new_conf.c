/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENZHEN) Co., Ltd.
 文件名称: new_conf.c
 文件描述: 读取dproxy配置文件 
 修订记录:
        1. 作者: lichuang
           日期: 2009-3-23
           内容: 创建文件
**********************************************************************/

#include "new_conf.h"
#include "dproxy.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define BUFFER_LEN 200

ST_DNS_CONFIG g_stConfig;

static void init_config();


static char **environ1 = NULL;

static int systemCmdEx (char *command, int printFlag) 
{
   int errno;
   int pid = 0, status = 0;
      char *argv[4];

   if ( command == 0 )
      return 1;

   pid = fork();
   if ( pid == -1 )
      return -1;

   if ( pid == 0 ) {
      argv[0] = "sh";
      argv[1] = "-c";
      argv[2] = command;
      argv[3] = 0;

      if (printFlag)
         printf("app: %s\r\n", command);

      execve("/bin/sh", argv, environ1);
      exit(127);
   }

   /* wait for child process return */
   do {
      if ( waitpid(pid, &status, 0) == -1 ) {
         if ( errno != EINTR )
            return -1;
      } else
         return status;
   } while ( 1 );

   return status;
}

/*************************************************************************
  功  能: 读取配置文件
  参  数: 
  		pConfig 配置文件路径
  返回值: 成功返回0, 失败返回-1  
*************************************************************************/
int load_config(const char* pConfig)
{
	FILE *pFile;
	char szBuffer[BUFFER_LEN];
	char *pLan, *pIp;
	ST_LAN_INFO *pLanInfo;
	int i;
	int protocol = 0;

    /*rm dns cache*/
    systemCmdEx("rm -fr /var/cache/* 2>/dev/null", 0);
    
	init_config();

	if (!pConfig || (NULL == (pFile = fopen(pConfig, "r"))))
	{
        g_stConfig.pLanInfo = NULL;
		return 0;
	}

	strcpy(g_stConfig.szConfigFile, pConfig);

	while (fgets(szBuffer, BUFFER_LEN, pFile))
	{
		szBuffer[strlen(szBuffer) - 1] = '\0';
		pLan = strtok(szBuffer, " ");
		pIp = strtok(NULL, " ");

		if (!pLan || !pIp)
		{
			continue;
		}

		for (pLanInfo = g_stConfig.pLanInfo; pLanInfo; pLanInfo = pLanInfo->pNext)
		{
			if (!strcmp(pLanInfo->szName, pLan))
			{
				break;
			}
		}
		if (!pLanInfo)
		{
			pLanInfo = (ST_LAN_INFO*)malloc(sizeof(ST_LAN_INFO));
			if (!pLanInfo)
			{
				return -1;
			}
			memset(pLanInfo, 0, sizeof(ST_LAN_INFO));
			/*加入链表*/
			pLanInfo->pNext = g_stConfig.pLanInfo;
			g_stConfig.pLanInfo = pLanInfo;
                     pLanInfo->ipPro = 0;
			strcpy(pLanInfo->szName, pLan);
			snprintf(pLanInfo->szCacheFile, sizeof(pLanInfo->szCacheFile)
					,"/var/cache/%s.cache", pLan);
		}

		if (strchr(pIp, '.'))
		{
            protocol = PROTO_IPV4;
		   pLanInfo->ipPro |= IPV4_ENABLE ;
		}
		else
		{
            protocol = PROTO_IPV6;
		   pLanInfo->ipPro |= IPV6_ENABLE ;
		}
		
		for (i = 0; i < NUM_OF_DNS; ++i)
		{
			if (!strlen(pLanInfo->szDnsIp[protocol][i]))
			{
				strcpy(pLanInfo->szDnsIp[protocol][i], pIp);
				break;
			}
		}
	}

	fclose(pFile);
#if 1
	for (pLanInfo = g_stConfig.pLanInfo; pLanInfo; pLanInfo = pLanInfo->pNext)
	{
		debug("~~~~~~LanInfoList: Name: %s,  dns: %s, %s, %s, %s, %s, %s, cache: %s\n"
				, pLanInfo->szName
				, pLanInfo->szDnsIp[0][0]
				, pLanInfo->szDnsIp[0][1]
				, pLanInfo->szDnsIp[0][2]
				, pLanInfo->szDnsIp[1][0]
				, pLanInfo->szDnsIp[1][1]
				, pLanInfo->szDnsIp[1][2]
				, pLanInfo->szCacheFile);
	}
#endif

	return 0;
}

void init_config()
{
    if (g_stConfig.pLanInfo)
    {
        ST_LAN_INFO *pNext;
        for (pNext = g_stConfig.pLanInfo; pNext; pNext = g_stConfig.pLanInfo)
        {
            g_stConfig.pLanInfo = pNext->pNext;
            free(pNext);
        }
    }
	g_stConfig.pLanInfo = NULL;
	g_stConfig.ucDeamon = 0;
	g_stConfig.nPurgeTime = 5 * 60;
	strcpy(g_stConfig.szHostFile, "/etc/hosts");
	//strcpy(g_stConfig.szCacheFile, "/var/cache/dproxy.cache");
}

