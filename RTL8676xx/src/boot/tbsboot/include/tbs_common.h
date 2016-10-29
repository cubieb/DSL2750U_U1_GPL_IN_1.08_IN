/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : tbs_common.h
 文件描述 : 函数声明

 修订记录 :
          1 创建 : 轩光磊
            日期 : 2008-9-17
            描述 : 创建文档


**********************************************************************/

#ifndef __TBS_COMMON_H__
#define __TBS_COMMON_H__

int config_is_ok(void *data);
int ConvertEndian4(int a);
int ConvertEndian2(short a);
int check_addr_null( unsigned int *addr, unsigned int len );

#endif
