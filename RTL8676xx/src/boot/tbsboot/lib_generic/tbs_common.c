/*=========================================================================
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : tbs_common.c
 文件描述 : 自己实现在函数

 修订记录 :
          1 创建 : 轩光磊
            日期 : 2008-9-27
            描述 :

=========================================================================*/
#include <common.h>
#include <tbs_common.h>
#include <malloc.h>

/*=========================================================================
 Function Description:		转换长度为4字节的字节序
 Calls:			无
 Data Accessed:
 Data Updated:
 Input:			a:		要转换的数据 				 				
 Output:			无
 Return:			b:		转换的结果			
 Others:			
=========================================================================*/

int ConvertEndian4(int a)
{
	unsigned char * byte;
	int b = 0;
	byte = ( unsigned char * )&a;
	b = (byte[3]<<3)|(byte[2]<<2)|(byte[1]<<1)|byte[0];
	return b;
}

/*=========================================================================
 Function Description:		转换长度为2字节的字节序
 Calls:			无
 Data Accessed:
 Data Updated:
 Input:			a:		要转换的数据 				 				
 Output:			无
 Return:			b:		转换的结果			
 Others:			
=========================================================================*/

int ConvertEndian2(short a)
{
	unsigned char * byte;
	short b = 0;
	byte = ( unsigned char * )&a;
	b = (byte[1]<<1)|byte[0];
	return b;
}

int	config_is_ok(void *data)
{
	if(memcmp(data, CONFIG_MARK, CONFIG_MARK_LEN) == 0)
		return 0;
	else
		return -1;
}


