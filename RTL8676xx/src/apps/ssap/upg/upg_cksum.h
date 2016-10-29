/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : upg_cksum.h
 文件描述 : 校验和的模块的头文件
 函数列表 :
 修订记录 :
          1 创建 : 李浩
            日期 : 2008-04-12
            描述 :

**********************************************************************/
#ifndef __UPGCKSUM_H__
#define __UPGCKSUM_H__

#include <stdio.h>

int tbs_crc_file( FILE *fp , unsigned int offset , unsigned long *checksum );
int tbs_get_sum(FILE *fp, unsigned long *sum);

#endif
