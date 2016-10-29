/*****************************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : ph_encrypt.h
 文件描述 : 处理字符串的加密及解密

 函数列表 :

 修订记录 :
            创建 :  Kevin
            日期 :  2009-11-18
            描述 :

******************************************************************************/

#ifndef __PH_ENCRYPT_H
#define __PH_ENCRYPT_H

#include "md5.h"
#include "hmac_md5.h"
#include "blowfish.h"


/******************************************************************************
                        *                               函数申明                                   *
 ******************************************************************************/
void MD5String(char *pszSrcString, char *pszDesString);
int   Base64_Decode(char   *psSrc,  int   iSize,   char   *psDest);
int  Base64_Encode(char *psSrc, unsigned int uiSize, char *psDest);

#endif /* __PH_ENCRYPT_H */

