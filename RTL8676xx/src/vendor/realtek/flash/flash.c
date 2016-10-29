/******************************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: flash.c
 文件描述:  产测工具
 修订记录:
         1 作者 : 陈赞辉
           日期 : 2009-08-07
           内容: 创建文件

         2 by  夏超仁
           日期 : 2012-04-09
           内容: 修改写入长度        
******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <flash_layout.h>

/*****************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*****************************************************************************
*                                 ENUM                                       *
******************************************************************************/

/*****************************************************************************
*                                STRUCT                                      *
******************************************************************************/

/*****************************************************************************
*                               GLOBAL VAR                                   *
******************************************************************************/


/*****************************************************************************
*                               FUNCTION                                     *
******************************************************************************/

/*****************************************************************************
功能: 产测工具main函数
参数: 
返回: 无
备注:   
******************************************************************************/
int main(int argc, char** argv)
{
	char mib[128];
	char buffer[512];
	unsigned short len = 0;
	int ret_val = -1;
	
	memset(mib, 0x00, sizeof(mib));
	memset(buffer, 0x00, sizeof(buffer));
	switch(argc)
		{
		case 3:
		if(0 == strcmp(argv[1], "get"))
			{
			sscanf(argv[2], "%s", mib);
			len = sizeof(buffer);
			app_item_get(buffer, mib, &len);
			printf("%s=%s\n", mib, buffer);
			ret_val = 0;
			}
		break;

		case 4:
		if(0 == strcmp(argv[1], "set"))
			{
			sscanf(argv[2], "%s", mib);
			sscanf(argv[3], "%s", buffer);
			len = strlen(buffer);
			printf("set %s=%s, len=%d\n",mib, buffer, len);
			app_item_save(buffer, mib, len);		/* 保存新条目*/
			ret_val =0;
			}
		break;
		
		default:
		break;
		}
	if(ret_val < 0)
		{
		printf("Usage: flash cmd\n");
		printf("cmd:\n");
		printf("  get MIB-NAME \t\t\tget a specific mib from flash memory.\n");
		printf("  set MIB-NAME MIB-VALUE \tset a specific mib into flash memory.\n");
		printf("support mib_name:\n");
		printf("  HW_TX_POWER_CCK \n");
		printf("  HW_TX_POWER_OFDM_1S \n");
		printf("  HW_TX_POWER_OFDM_2S \n");
		printf("  HW_11N_LOFDMPWDA \n");
		printf("example:\n");
		printf("  HW_TX_POWER_CCK \"xx(14byte)\"\n");
		printf("  HW_TX_POWER_OFDM_1S \"xx(28byte)\"\n");
		printf("  HW_TX_POWER_OFDM_2S \"xx(28byte)\"\n");
		printf("  HW_11N_LOFDMPWDA \"xx xx\"\n");
		}
	
	return ret_val;
}

/*****************************************************************************
*                                 END                                        *
******************************************************************************/
