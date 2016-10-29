/*****************************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : ph_encrypt.c
 文件描述 : 实现字符串的加密及解密

 函数列表 :

 修订记录 :
            创建 :  Kevin
            日期 :  2009-11-18
            描述 :

******************************************************************************/

#include "ph_encrypt.h"
#include "string.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif

/******************************************************************************
                        *                               全局变量                               *
 ******************************************************************************/
 /*编码时候的每行的长度*/
static unsigned  int  g_uiLineWidth   =   76;

/*Base64编码表*/
static const   char   BASE64_ENCODE_TABLE[64]   =   {
65,     66,     67,     68,     69,     70,     71,     72,     //   00   -   07
73,     74,     75,     76,     77,     78,     79,     80,     //   08   -   15
81,     82,     83,     84,     85,     86,     87,     88,     //   16   -   23
89,     90,     97,     98,     99,   100,   101,   102,     //   24   -   31
103,   104,   105,   106,   107,   108,   109,   110,     //   32   -   39
111,   112,   113,   114,   115,   116,   117,   118,     //   40   -   47
119,   120,   121,   122,     48,     49,     50,     51,     //   48   -   55
52,     53,     54,     55,     56,     57,     43,     47      //   56   -   63
};

/*Base64解码表*/
static const   unsigned   char BASE64_DECODE_TABLE[256] = {
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
	0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
	0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
	0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
	0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
	0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
	0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
};

/*****************************************************************************
                        *                                    函数定义                           *
 ******************************************************************************/
/******************************************************************************
功能: 使用BASE64加密字符串
参数: psSrc          input          源串
             iSize           input          源串长度
             psDest        output        加密串(密文)
返回: 加密串的实际长度
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
int  Base64_Encode(char *psSrc, unsigned int uiSize, char *psDest)
{
    unsigned int uiByte = 0;
    unsigned int uiInMax3 =0 ;
    unsigned int uiLen = 0;
    char *psInPtr = NULL;
    char *psInLimitPtr = NULL;
    char *psOutPtr = NULL  ;

    if ((psSrc   ==   NULL)   ||   (uiSize   <=   0))   
        return   0;

    psInPtr = psSrc;
    uiInMax3 = uiSize/3*3;
    psOutPtr = psDest;
    psInLimitPtr = psInPtr + uiInMax3;

    while (psInPtr != psInLimitPtr)
    {
        uiLen = 0;
        while ((uiLen < g_uiLineWidth)  &&  (psInPtr != psInLimitPtr))
        {
            uiByte = (unsigned char) *psInPtr++;
            uiByte = uiByte << 8;

            uiByte = uiByte |(unsigned   char) *psInPtr++;
            uiByte = uiByte << 8;
            uiByte = uiByte | (unsigned   char)   *psInPtr++;

            /*以4   byte倒序写入输出缓冲*/
            psOutPtr[3] = BASE64_ENCODE_TABLE[uiByte & 0x3F];
            uiByte = uiByte   >>   6;
            psOutPtr[2] = BASE64_ENCODE_TABLE[uiByte & 0x3F];
            uiByte = uiByte>>   6;
            psOutPtr[1] = BASE64_ENCODE_TABLE[uiByte & 0x3F];
            uiByte = uiByte   >>   6;
            psOutPtr[0] = BASE64_ENCODE_TABLE[uiByte];
            psOutPtr += 4;
            uiLen += 4;
        }/*end of while ((uiLen < g_uiLineWidth) && (psInPtr != psInLimitPtr))*/
       
        if (uiLen >= g_uiLineWidth)
        {
            *psOutPtr++ =  '\r';   //加上回车换行符
            *psOutPtr++ =  '\n';
        }
    }/*end of   while   (psInPtr   !=   psInLimitPtr)*/
    
    /*设置尾部*/
    switch (uiSize - uiInMax3)
    {
        case 1:
            uiByte = (unsigned char) *psInPtr;
            uiByte = uiByte << 4;
            psOutPtr[1] = BASE64_ENCODE_TABLE[uiByte & 0x3F];
            uiByte = uiByte >> 6;
            psOutPtr[0]  =  BASE64_ENCODE_TABLE[uiByte];
            psOutPtr[2]  =  '=';   //用'='也就是64码填充剩余部分
            psOutPtr[3]  =  '=';
            psOutPtr += 4;
            break;
        case   2:
            uiByte = (unsigned  char) *psInPtr++;
            uiByte = uiByte << 8;
            uiByte = uiByte |(unsigned char) *psInPtr;
            uiByte = uiByte << 2;
            psOutPtr[2]  =  BASE64_ENCODE_TABLE[uiByte & 0x3F];
            uiByte = uiByte >> 6;
            psOutPtr[1] = BASE64_ENCODE_TABLE[uiByte & 0x3F];
            uiByte = uiByte >> 6;
            psOutPtr[0] = BASE64_ENCODE_TABLE[uiByte];
            psOutPtr[3] =  '=';
            psOutPtr +=4;
            break;
    }
    
    return  (unsigned int) (psOutPtr  - psDest);
}
/******************************************************************************
功能: 使用BASE64解密字符串
参数: psSrc          input          加密串
             iSize           input          加密串长度
             psDest        output        解密串(明文)
返回: 解密串的实际长度
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
int   Base64_Decode(char *psSrc, int iSize, char *psDest)
{
    unsigned int uiByteBuffer = 0;
    unsigned int uiByteBufferSpace = 0;
    unsigned int uiTemp = 0;
    char *psInPtr = NULL;
    char *psInLimitPtr = NULL;
    char  *psOutPtr = NULL;
    int   iRealLen = 0;

    if ((psSrc == NULL) ||(psDest == NULL) ||(iSize   <=   0)) 
        return 0;

    uiByteBuffer = 0;
    uiByteBufferSpace = 4;

    psInPtr = psSrc;
    psInLimitPtr = psInPtr + iSize;
    psOutPtr = psDest;

    while (psInPtr != psInLimitPtr)
    {
        uiTemp   =   BASE64_DECODE_TABLE[*psInPtr];
        psInPtr++;
        if (uiTemp ==   0xFF)   
            continue;   //读到255非法字符
        uiByteBuffer   =   uiByteBuffer   <<   6   ;
        uiByteBuffer   =   uiByteBuffer   |   uiTemp;
        uiByteBufferSpace--;

        if (uiByteBufferSpace   !=   0)   
            continue;   //一次读入4个字节
           
        /*倒序写入3个字节到缓冲*/
        psOutPtr[2]   =   uiByteBuffer;
        uiByteBuffer   =   uiByteBuffer   >>   8;
        psOutPtr[1]   =   uiByteBuffer;
        uiByteBuffer   =   uiByteBuffer   >>   8;
        psOutPtr[0]   =   uiByteBuffer;

        /*准备写入后3位*/
        psOutPtr+=   3;   
        uiByteBuffer   =   0;   
        uiByteBufferSpace   =   4;
    }/*end of  while (psInPtr != psInLimitPtr)*/
   
    iRealLen   =   (unsigned   int)psOutPtr   -   (unsigned   int)psDest;
    /*处理尾部   返回实际长度*/
    switch   (uiByteBufferSpace)
    {
    case   1:
        uiByteBuffer   =   uiByteBuffer   >>   2;
        psOutPtr[1]   =   uiByteBuffer;
        uiByteBuffer   =   uiByteBuffer   >>   8;
        psOutPtr[0]   =   uiByteBuffer;
        return   iRealLen   +   2;
    case   2:
        uiByteBuffer   =   uiByteBuffer   >>   4;
        psOutPtr[0]   =   uiByteBuffer;
        return   iRealLen   +   1;
    default:
        return   iRealLen;
    }
}
/******************************************************************************
功能: 使用MD5加密字符串
参数: pszSrcString          input           原文
             pszDesString         output          密文
返回: 无
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
void MD5String(char *pszSrcString, char *pszDesString)
{
    MD5_CTX mdContext;
    int i = 0;
    unsigned char uszHash[16] = {0};
    char temp[2] = {0};
   
    MD5Init(&mdContext);
    MD5Update(&mdContext,pszSrcString,strlen(pszSrcString));
    MD5Final(uszHash,&mdContext);

    for(i=0;i<16;i++)  
    {
        sprintf(temp,"%02x", uszHash[i]);
        strcat(pszDesString,temp);
    }

    return ;
}


#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif
