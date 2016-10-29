/*
 * INTEL CONFIDENTIAL
 * Copyright (c) 2002 - 2006 Intel Corporation.  All rights reserved.
 * 
 * The source code contained or described herein and all documents
 * related to the source code ("Material") are owned by Intel
 * Corporation or its suppliers or licensors.  Title to the
 * Material remains with Intel Corporation or its suppliers and
 * licensors.  The Material contains trade secrets and proprietary
 * and confidential information of Intel or its suppliers and
 * licensors. The Material is protected by worldwide copyright and
 * trade secret laws and treaty provisions.  No part of the Material
 * may be used, copied, reproduced, modified, published, uploaded,
 * posted, transmitted, distributed, or disclosed in any way without
 * Intel's prior express written permission.
 
 * No license under any patent, copyright, trade secret or other
 * intellectual property right is granted to or conferred upon you
 * by disclosure or delivery of the Materials, either expressly, by
 * implication, inducement, estoppel or otherwise. Any license
 * under such intellectual property rights must be express and
 * approved by Intel in writing.
 * 
 * $Workfile: UTF8Utils.c
 *
 *
 *
 */
/* For Test */
/* #define ILibString_Copy(x,y) strdup(x) */
#if defined(WIN32) || defined(_WIN32_WCE)
#include <time.h>
#include <windows.h>
#endif

#undef __WCHAR_TYPE__
#define __WCHAR_TYPE__ unsigned short 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "UTF8Utils.h"
#include "ILibParsers.h"

 /*
 * Implements additional string functionality.
 */
#if 1
#define DEBUG_LOG(...) 
#else
#define DEBUG_LOG printf
#endif
int char2uni(const unsigned char *rawstring, int boundlen, wchar_t *uni);
int uni2char(const wchar_t uni, unsigned char *out, int boundlen);
int utf8_wcstombs(unsigned char *s, const wchar_t *pwcs, int maxlen);
int utf8_mbstowcs(wchar_t *pwcs, const unsigned char *s, int n);

char* EncodeToUTF8(const char* mbcsStr) 
{ 
#if defined(WIN32)
        wchar_t*  wideStr; 
        char*   utf8Str; 
        int   charLen; 

        charLen = MultiByteToWideChar(CP_ACP, 0, mbcsStr, -1, NULL, 0); 
        wideStr = (wchar_t*) malloc(sizeof(wchar_t)*charLen); 
        MultiByteToWideChar(CP_ACP, 0, mbcsStr, -1, wideStr, charLen); 

        charLen = WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, NULL, 0, NULL, NULL); 

		utf8Str = (char*) malloc(charLen);

        WideCharToMultiByte(CP_UTF8, 0, wideStr, -1, utf8Str, charLen, NULL, NULL);

        free(wideStr); 
        return utf8Str;
#elif defined(TBS)
    /* GB18030 转换到  UTF8 长度不会超出4倍 */
    int iInputLen=0,iInputPos=0;
    int n=0;
    int iOutputLen=0,iOutputPos=0;
    wchar_t *pwszBuffer=NULL;
    char *pszOutputBuffer=NULL;
    char *result=NULL;
    DEBUG_LOG("mbcsStr(%p) is %s:%d\n",mbcsStr,mbcsStr,strlen(mbcsStr));
    
    iInputLen=strlen(mbcsStr);
    iOutputLen=iInputLen*4;
    pwszBuffer=(wchar_t *)malloc(iOutputLen);
    if (!pwszBuffer)
    {
        DEBUG_LOG("no memory 1\n");
        goto errout;
    }
    pszOutputBuffer=(char *)malloc(iOutputLen);
    if (!pszOutputBuffer)
    {
        DEBUG_LOG("no memory 2\n");
        free(pwszBuffer);
        goto errout;
    }

       
    while (iInputLen>0)
    {
        n=char2uni(mbcsStr+iInputPos,iInputLen,&pwszBuffer[iOutputPos]);
        if (n>0)
        {
            iInputLen -= n;
            iInputPos += n;
            iOutputPos ++;
        }
        else
        {
            break;
        }
    }
    pwszBuffer[iOutputPos]=0;
   
    iOutputPos=utf8_wcstombs(pszOutputBuffer,pwszBuffer,iOutputLen);
    pszOutputBuffer[iOutputPos]='\0';

    free(pwszBuffer);

    result=ILibString_Copy(pszOutputBuffer,iOutputPos);
    free(pszOutputBuffer);

    DEBUG_LOG("result(%p) is %s:%d iOutputPos=%d\n",result,result,strlen(result),iOutputPos);
    return result;     
errout:
	return(ILibString_Copy(mbcsStr, (int)strlen(mbcsStr)));

#else
	return(ILibString_Copy(mbcsStr, (int)strlen(mbcsStr)));
#endif
} 

char* DecodeFromUTF8(const char* utf8Str) 
{ 
#if defined(WIN32)
        wchar_t*  wideStr; 
        char*   mbcsStr; 
        int   charLen; 

        charLen = MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, NULL, 0); 
        wideStr = (wchar_t*) malloc(sizeof(wchar_t)*charLen); 
        MultiByteToWideChar(CP_UTF8, 0, utf8Str, -1, wideStr, charLen); 

        charLen = WideCharToMultiByte(CP_ACP, 0, wideStr, -1, NULL, 0, NULL, NULL); 

		mbcsStr = (char*) malloc(charLen);

        WideCharToMultiByte(CP_ACP, 0, wideStr, -1, mbcsStr, charLen, NULL, NULL);

        free(wideStr); 
        return mbcsStr;
#elif defined(TBS)
    int iInputLen=0,iwsInputPos=0;
    int n=0;
    int iwsInputLen=0,iOutputPos=0;
    wchar_t *pwszBuffer=NULL;
    char *pszOutputBuffer=NULL;
    char *result;
    iInputLen=strlen(utf8Str);
    pwszBuffer=(wchar_t *)malloc(iInputLen*4);
    if (!pwszBuffer)
    {
        DEBUG_LOG("no memory 1\n");
        goto errout;
    }
    pszOutputBuffer=(char *)malloc(iInputLen*4);
    if (!pszOutputBuffer)
    {
        DEBUG_LOG("no memory 2\n");
        free(pwszBuffer);
        goto errout;
    }
    DEBUG_LOG("utf8Str(%p) is %s\n",utf8Str,utf8Str);
    iwsInputLen=utf8_mbstowcs(pwszBuffer,utf8Str,iInputLen);
    

    while (iwsInputLen>0)
    {
        n=uni2char(pwszBuffer[iwsInputPos],&pszOutputBuffer[iOutputPos],iInputLen);
        if (n>0)
        {
            iwsInputLen --;
            iwsInputPos ++;
            iOutputPos += n;
        }
        else
        {
            break;
        }
    }
    pszOutputBuffer[iOutputPos]=0;

    free(pwszBuffer);

    result=ILibString_Copy(pszOutputBuffer,iOutputPos);
    DEBUG_LOG("result(%p) is %s\n",result,result);
    free(pszOutputBuffer);
    return result;     
errout:
	return(ILibString_Copy(utf8Str, (int)strlen(utf8Str)));
#else
	return(ILibString_Copy(utf8Str, (int)strlen(utf8Str)));
#endif
} 
