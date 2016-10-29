/*****************************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : ph-client.c
 文件描述 : 花生壳客户端(peanuthullc)进程的实现

 函数列表 :

 修订记录 :
            创建 :  Kevin
            日期 :  2009-11-18
            描述 :  创建

            备注 :  该程序适用于运行环境的网络字节序为大端的情况。
                           如果运行环境的网络字节序为小端，则需要调整发包
                           部分代码的网络字节序。
******************************************************************************/
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include "ph-client.h"
#include "ph_encrypt.h"
#include "ph_socket.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif

/******************************************************************************
                        *                               全局变量                               *
 ******************************************************************************/
 /*花生壳服务器IP地址*/
static char g_szServerIP[MAX_STRING_LEN] = {0};

/*花生壳服务器返回的挑战串*/
static unsigned char g_uszKey[MAX_AUTH_LEN] = {0};

/*花生壳客户端更新信息结构体*/
ST_PH_UDP_SEND_DATE g_stClientInfo;

/*花生壳客户端类型*/
static int g_iClientType;

/*花生壳客户端更新状态*/
static int g_iClientState;
/*****************************************************************************
                        *                                 函数定义                            *
 ******************************************************************************/
/******************************************************************************
功能: 对long型变量进行高低字节转换
参数: lOldValue               input          需要转换的变量
返回: 转换后变量的值
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
static long LongOrderRevers(long lOldValue)
{
    struct stLongStruct
    {
        unsigned long uiZero:8;
        unsigned long uiOne:8;
        unsigned long uiTwo:8;
        unsigned long uiThree:8;
    };

    unsigned char ucTemp = '\0';

    struct stLongStruct *pstNewValue = (struct stLongStruct*)&lOldValue;

    ucTemp = pstNewValue->uiZero;
    pstNewValue->uiZero = pstNewValue->uiThree;
    pstNewValue->uiThree = ucTemp;

    ucTemp = pstNewValue->uiOne;
    pstNewValue->uiOne = pstNewValue->uiTwo;
    pstNewValue->uiTwo = ucTemp;

    return *(long *)pstNewValue;
}
/******************************************************************************
功能: 在字符串的前后去掉指定的字符
参数: psString               input          源字符串
             cTrim                   input         需要去掉的字符
返回: 无
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
static void StrTrim(char *psString, char cTrim)
{
    char *psTemp = psString;
    char *psBegin = NULL;
    int i = 0;
    int iLen = strlen(psString);

    if (psString)
    {
        /*去掉末尾*/
        for (i = iLen-1; i >=0; i--) 
        {
            if (cTrim != psTemp[i])
            {
                psTemp[i+1] = '\0';
                break;
            }
        }

        /*去掉前面*/
        for (i = 0; i < iLen; i++)
        {
            if (cTrim != psTemp[i])
            {
                psBegin = psTemp+i;
                break;
            }
        }

        if (psBegin && (psBegin != psTemp) )
        {
            while(*psBegin)
                *psTemp++ = *psBegin++;
               
                *psTemp = '\0';
        }
       
    }/*end of  if (psString)*/

    return;
}
/******************************************************************************
功能: 在指定的socket描述符中，注册服务器返回的域名
参数: fd                input          socket描述符
             psBuf           input         服务器返回的校验结果
返回: 成功/其他错误
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
static int RegisterDomain(int fd, char *psBuf)
{
    int iLen = 0;
    char *pDomain =  NULL;
    char szCMD[MAX_STRING_LEN] = {0};
    char szBuffer[MAX_STRING_LEN] = {0};
    char szDomain[MAX_BUF_LEN] = {0};

    PH_TRACE_INTO_FUNC;

    /*判断是否认证成功*/
    if (0 == strncmp(psBuf,PH_AUTH_FAIL,PH_RESPOND_LEN))
    {
        PH_TRACE("Authentication failure.\n");
        close(fd);
        return PROC_AUTH_FAIL;
    }

    if (0 == strncmp(psBuf,PH_AUTH_OK,PH_RESPOND_LEN))
    {
        pDomain = (char *)strstr(psBuf, NEWLINE_R_N);
        pDomain += strlen(NEWLINE_R_N);
        pDomain = (char *)strtok(pDomain, NEWLINE_R_N);

        /*注册服务器返回的所有域名*/
        while(pDomain)
        {
            /*向服务器注册域名*/
            sprintf(szCMD, "REGI A %s"NEWLINE_R_N, pDomain);
            strcat(szDomain,pDomain);
            strcat(szDomain,";");
            TCP_SendTo(fd, szCMD);

            /*读取注册结果*/
            iLen = TCP_ReadFromServer(fd, szBuffer, MAX_STRING_LEN);
            if (iLen > 0)
            {
                PH_TRACE("Register domain result :%s\n",szBuffer);
                if ( 0 != strncmp(szBuffer,PH_AUTH_OK,PH_RESPOND_LEN) )
                {
                    PH_TRACE("Register domain failed.\n");
                    close(fd);
                    return PROC_AUTH_FAIL;
                }
            }

            /*继续获取剩下的域名*/
            pDomain = (char *)strtok(NULL, NEWLINE_R_N);

            /*如果没有其他域名，则退出循环*/
	    if ('.' == pDomain[0])	
	        break;
        }/*end of while(pDomain)*/
	
    }/*end of  if (0 == strncmp(psBuf,PH_AUTH_OK,3))*/

    /*输出所注册的域名，方便TBS平台PC进程进行监控*/
    printf("Register Domain Name:%send",szDomain);

    PH_TRACE_OUT_FUNC;

    return PROC_NORMAL;
}
/******************************************************************************
功能:在消息中获取指定字段的值
参数: pcsMsg                input          查询的消息内容
             psKeyword           input          字段的描述
             psBuf                  output        指定字段获取的值
             iBufLen                input          存在字段值的最大长度  
返回: 字段的长度/0
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
static int GetValueFromXML(const char *pcsMsg, const char *pcsKeyword, char *psBuf, int iBufLen)
{
    char *psBegin= NULL;
    char *psEnd = NULL;
    char szTemp[MAX_STRING_LEN] = {0};
    int iLen = 0;

    PH_TRACE_INTO_FUNC;

    /*生成关键字段的前缀*/
    snprintf(szTemp, sizeof(szTemp), "<%s>", pcsKeyword); 
    psBegin = strstr(pcsMsg, szTemp);
    if (psBegin)
    {
       psBegin += strlen(szTemp);

        /*生成关键字的后缀*/
        snprintf(szTemp, sizeof(szTemp), "</%s>", pcsKeyword);
        psEnd = strstr(psBegin, szTemp);
       
        iLen = (psEnd - psBegin);
        if ( (iLen > 0) && (iLen < iBufLen) )
        {
            strncpy(psBuf, psBegin, iLen);
            psBuf[iLen] = '\0';
        }
    }

    PH_TRACE_OUT_FUNC;
   
    return iLen;
}
/******************************************************************************
功能: 构造花生壳客户端(peanuthullc)验证报文
参数: psBuf                   output        所构造的报文内容
             pcsPassport          input          护照
             pcsPassword         input          密码(明文)
             pcusAuthKey         input          服务器返回的 挑战串
返回: 报文的长度/其他错误
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
static int GetAuthInfo(char *psBuf, const char* pcsPassport, const char *pcsPassword, 
                                                                                 const unsigned char *pcusAuthKey)
{
    int iLen = 0;
    int iMoveBit = 0;
    long lEmbedAuthKey = 0;
    long lServerTime = 0;
    long lClientInfo = 0;
    long lReversal = 0;
    unsigned char uszMD5Password[MAX_PASSPORT_LEN] = {0};
    unsigned char szBuf[MAX_STRING_LEN] = {0};
    unsigned char *pCurrent = NULL;

    PH_TRACE_INTO_FUNC;

    /*使用hmac_md5加密密码*/
    hmac_md5((char *)pcusAuthKey, (char *)pcsPassword, strlen(pcsPassword), uszMD5Password);
    PH_TRACE("uszMD5Password=%s,strlen(uszMD5Password)=%d\n",uszMD5Password,strlen(uszMD5Password));

    /*其中，加密嵌入认证码共4个字节，为嵌入认证码与服务器当
    前时间运算得到，服务器时间是挑战串的第六字节后的4个字节，
    算法是嵌入认证码与该时间的取反进行或运算后循环右移一定位数；
    该右移位数是用服务器时间整除30；*/

    /*从服务器返回的挑战串中获取服务器时间时，应该注意网络字节序*/
    lServerTime = *((long*)(pcusAuthKey + 6));
    lServerTime = LongOrderRevers(lServerTime);
    
    lServerTime |= ~(PHCLIENT_KEY);

    iMoveBit = lServerTime % 30; 
    lEmbedAuthKey = (lServerTime << (32 - iMoveBit)) | ((lServerTime >> iMoveBit) & ~(0xffffffff << (32 - iMoveBit)));

    /*客户端信息也是4个字节，前两位为嵌入式的客户号，后两位为
    客户端版本号*/
    lClientInfo = PHCLIENT_INFO;


    /*账号名+一个空格+加密嵌入认证码+客户端信息+加密串*/
    sprintf(szBuf, "%s ", pcsPassport);
    pCurrent = szBuf + strlen(szBuf);

    /*填写嵌入认证码和客户端信息时，需要注意网络字节序*/
    lReversal= LongOrderRevers(lEmbedAuthKey);
    memcpy(pCurrent, &lReversal, 4);
    pCurrent += 4;

    lReversal = LongOrderRevers(lClientInfo);
    memcpy(pCurrent, &lReversal, 4);
    pCurrent += 4;
    
    memcpy(pCurrent, uszMD5Password,strlen(uszMD5Password));
    pCurrent +=strlen(uszMD5Password);
    *pCurrent = '\0';
    iLen = pCurrent - szBuf;

    PH_TRACE("szBuf=%s,strlen(szBuf)=%d\n",szBuf,iLen);

    /*使用Base64进行加密*/
    Base64_Encode(szBuf, iLen, psBuf);
    strcat(psBuf,NEWLINE_R_N);

    PH_TRACE_OUT_FUNC;

    return strlen(psBuf);
}

/******************************************************************************
功能: 构造花生壳客户端(peanuthullc)登录报文
参数: psBuf                   output        所构造的报文内容
             pcsPassport          input          护照
             pcsPassword         input          密码(明文)
返回: 报文的长度/其他错误
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
static int GetUserInfo(char *psBuf, const char* pcsPassport, const char *pcsPassword)
{
    int iLen = 0;
    char *pCurrent = psBuf;
    char szPswCrypt[MAX_MD5PSW_LEN] = {0};

    PH_TRACE_INTO_FUNC;

    /*使用MD5加密护照密码*/
    MD5String((char *)pcsPassword, szPswCrypt);
    PH_TRACE("szPswCrypt=%s\n",szPswCrypt);

    /*构造报文内容*/
    snprintf(pCurrent, MAX_BUF_LEN, "<?xml version=\"1.0\" encoding=\"utf-8\"?>"NEWLINE_R_N);
    iLen = strlen(psBuf);
    pCurrent += iLen;

    snprintf(pCurrent, MAX_BUF_LEN, "<soap:Envelope "	\
		"xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "	\
		"xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\" "	\
		"xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\">"	\
		"<soap:Header>"	\
		"<AuthHeader xmlns=\"http://tempuri.org/\">"	\
		"<Account>%s</Account>"	\
		"<Password>%s</Password>"	\
		"</AuthHeader>"	\
		"</soap:Header>"	\
		"<soap:Body>"	\
		"<GetMiscInfo xmlns=\"http://tempuri.org/\">"	\
		"<clientversion>%s</clientversion>"	\
		"</GetMiscInfo>"	\
		"</soap:Body>"	\
		"</soap:Envelope>", pcsPassport, szPswCrypt, PHCLIENT_VERSION_STR);

    iLen += strlen(pCurrent);

    PH_TRACE_OUT_FUNC;
   
    return iLen;
}

/******************************************************************************
功能: 完成动态注册的更新和在线状态监测。
参数: 无
返回: 成功/其他错误
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
static int PH_update(void)
{
    int fd = 0;
    int iRet = 0;
    int iMissHeartPacket =  0;
    ST_PH_UDP_SEND_DATE stClientInfoTemp;
    ST_PH_UDP_REC_DATE stServerInfoTemp;
    blf_ctx stCTX;
    struct hostent *pstHost;
    struct sockaddr_in stSocketAddr;

    PH_TRACE_INTO_FUNC;

    /*建立UDP Server连接*/
    fd = UDP_connect(atoi(UDP_PROTOCOL_PORT_6060) );
    if (fd  <= 0 )
    {
        PH_TRACE("Setup UDP connection failed.\n");
        return -1;
    }
 
    stSocketAddr.sin_family = AF_INET;
    pstHost = gethostbyname(g_szServerIP);
    if (0 == pstHost) 
    {
        PH_TRACE("gethostbyname server failed.\n");
        return -1;
    }   
    stSocketAddr.sin_addr = *(struct in_addr *)*pstHost->h_addr_list;
    stSocketAddr.sin_port = htons(atoi(UDP_PROTOCOL_PORT_6060));
   
    while(1)
    {
        /*填写客户端结构体信息*/
        if (PHC_UPDATE == g_iClientState)
            g_stClientInfo.lOptionCode = PHC_UPDATE;
        else
            g_stClientInfo.lOptionCode = PHC_LOGOUT;
           
        g_stClientInfo.lCheckSum = (g_stClientInfo.lOptionCode +g_stClientInfo.lSerialNum)*(-1);
        g_stClientInfo.lAppend = 0;

        PH_TRACE("g_stClientInfo.lSessionID =%ld\n",g_stClientInfo.lSessionID);
        PH_TRACE("g_stClientInfo.lOptionCode =%ld\n",g_stClientInfo.lOptionCode);
        PH_TRACE("g_stClientInfo.lSerialNum =%ld\n",g_stClientInfo.lSerialNum);
        PH_TRACE("g_stClientInfo.lCheckSum =%ld\n",g_stClientInfo.lCheckSum);
        PH_TRACE("g_stClientInfo.lAppend =%ld\n",g_stClientInfo.lAppend);

        /*使用Blow Fish加密*/
        stClientInfoTemp = g_stClientInfo;
        blf_key(&stCTX, g_uszKey, sizeof(g_uszKey));
        blf_enc(&stCTX, (unsigned long *)&stClientInfoTemp.lOptionCode, 4);

        /*网络字节序转换*/
        stClientInfoTemp.lSessionID = LongOrderRevers( stClientInfoTemp.lSessionID);
        stClientInfoTemp.lOptionCode = LongOrderRevers( stClientInfoTemp.lOptionCode);
        stClientInfoTemp.lSerialNum= LongOrderRevers( stClientInfoTemp.lSerialNum);
        stClientInfoTemp.lCheckSum= LongOrderRevers( stClientInfoTemp.lCheckSum);
        stClientInfoTemp.lAppend= LongOrderRevers( stClientInfoTemp.lAppend);

        /*发送更新报文*/
        iRet = UDP_SendTo(fd, (void *)&stClientInfoTemp, sizeof(stClientInfoTemp), &stSocketAddr);
        if (iRet >= 0)
        {
            PH_TRACE("UDP_SendTo iRet=%d\n",iRet);
            iRet = UDP_ReadFromServer(fd, (void *)&stServerInfoTemp, sizeof(stServerInfoTemp), &stSocketAddr);
            if (iRet > 0)
            {
                /*丢失的心跳包数清零*/
                iMissHeartPacket = 0;

                PH_TRACE("UDP_ReadFromServer iRet=%d\n",iRet);
                /*网络字节序转换*/
                stServerInfoTemp.stUDPDate.lSessionID = LongOrderRevers(stServerInfoTemp.stUDPDate.lSessionID);
                stServerInfoTemp.stUDPDate.lOptionCode= LongOrderRevers(stServerInfoTemp.stUDPDate.lOptionCode);
                stServerInfoTemp.stUDPDate.lSerialNum= LongOrderRevers(stServerInfoTemp.stUDPDate.lSerialNum);
                stServerInfoTemp.stUDPDate.lCheckSum= LongOrderRevers(stServerInfoTemp.stUDPDate.lCheckSum);
                stServerInfoTemp.stUDPDate.lAppend= LongOrderRevers(stServerInfoTemp.stUDPDate.lAppend);
                stServerInfoTemp.lIP = LongOrderRevers(stServerInfoTemp.lIP);
              
                if (g_stClientInfo.lSessionID != stServerInfoTemp.stUDPDate.lSessionID)
                {
                    /*应该不会进这里*/
                    PH_TRACE("get back session incorrect.\n");
                    goto UPDATE_FAIL;
                }
                else
                {
                    /*使用Blow Fish解密*/
                    blf_key(&stCTX, g_uszKey, sizeof(g_uszKey));
                    blf_dec(&stCTX, (unsigned long *)&stServerInfoTemp.stUDPDate.lOptionCode, 4);

                    PH_TRACE("stServerInfoTemp.stUDPDate.lSessionID =%ld\n",stServerInfoTemp.stUDPDate.lSessionID);
                    PH_TRACE("stServerInfoTemp.stUDPDate.lOptionCode =%ld\n",stServerInfoTemp.stUDPDate.lOptionCode);
                    PH_TRACE("stServerInfoTemp.stUDPDate.lSerialNum =%ld\n",stServerInfoTemp.stUDPDate.lSerialNum);
                    PH_TRACE("stServerInfoTemp.stUDPDate.lCheckSum =%ld\n",stServerInfoTemp.stUDPDate.lCheckSum);
                    PH_TRACE("stServerInfoTemp.stUDPDate.lAppend =%ld\n",stServerInfoTemp.stUDPDate.lAppend);
                    PH_TRACE("stServerInfoTemp.ip=%x\n",stServerInfoTemp.lIP);

                    /*如果是更新消息*/
                    if (PHC_UPDATE == g_iClientState)
                    {
                        /*更新失败*/
                        if (PH_UPDATE_FAIL == stServerInfoTemp.stUDPDate.lOptionCode)
                        {
                           PH_TRACE("get the incorrect update respond optioncode.\n");
                           goto UPDATE_FAIL;
                        }
                        else
                        {
                            if (g_stClientInfo.lSerialNum == stServerInfoTemp.stUDPDate.lSerialNum)
                                g_stClientInfo.lSerialNum++;
                            else if ((g_stClientInfo.lSerialNum - stServerInfoTemp.stUDPDate.lSerialNum) < 3)
                                g_stClientInfo.lSerialNum = stServerInfoTemp.stUDPDate.lSerialNum;
                            else
                            {
                                PH_TRACE("get the incorrect update respond serial number.\n");
                                goto UPDATE_FAIL;
                            }
                        }/*end of  if (PH_UPDATE_FAIL == stClientInfoTemp.lOptionCode)*/
                    }/*end of if (PHC_UPDATE == g_iClientState)*/
                    else
                    {
                        /*注销成功*/
                        if (PH_LOGOUT_OK == stServerInfoTemp.stUDPDate.lOptionCode)
                        {
                            PH_TRACE("Logout OK\n");
                            close(fd);
                            return PROC_NORMAL;
                        }
                        else
                        {
                            continue;
                        }

                    }/*end of if (PHC_UPDATE == g_iClientState)*/

                    /*根据用户的类型，间隔发更新包*/
                    if (ADVANCED_USER == g_iClientType)
                        sleep(ADVANCED_HEARTBEAT);
                    else
                        sleep(STANDARD_HEARTBEAT);           
                }/*end of  if (g_stClientInfo.lSessionID != stClientInfoTemp.lSessionID)*/
               
            }
            else
            {
                if (PHC_LOGOUT== g_iClientState)
                {
                    PH_TRACE("Logout OK\n");
                    close(fd);
                    return PROC_NORMAL;
                }
                
                /*如果客户机连续5个心跳包发送出去后都没有收到服务器的回应则
                应在最后一个心跳包发送完继续等待20秒后判断网络连接出现问题。
                客户机必须重新执行协议2.2过程重新登录并注册域名。*/
                iMissHeartPacket++;

                if (4 == iMissHeartPacket)
                    sleep(20);

                if (4 < iMissHeartPacket)
                {
                    PH_TRACE("the Internet does not work\n");
                    goto UPDATE_FAIL;
                }
            }/*end of if (iRet >= 0)*/
           
        }/*end of  if (iRet >= 0)*/
        
    }/*end of  while(1)*/
    
    PH_TRACE_OUT_FUNC;

UPDATE_FAIL:
    PH_TRACE("Update failed.\n");
    if (fd)
        close(fd);
        
   return PROC_UPDATE_FAIL;
}
/******************************************************************************
功能: 客户机必须通过账号验证才能进行域名申请、修改、注册等操作。
参数: pcsPassport         input     护照名
             pcsPassword        input    密码(明文)
返回: 成功/其他错误
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
static int PH_auth(const char *pcsPassport, const char *pcsPassword)
{
    int fd = 0;
    int iRet = 0;
    int iLen = 0;
    char szBuffer[MAX_BUF_LEN] = {0};
    unsigned char uszBase64Key[MAX_STRING_LEN] = {0};

    PH_TRACE_INTO_FUNC;

    /*建立TCP 连接*/
    fd = TCP_connect( g_szServerIP, TCP_PROTOCOL_PORT_6060);
    if ( fd < 0 )
    {
        PH_TRACE("Setup TCP connection failed.\n");
        return -1;
    }

    /*读取服务器的回应*/
    iLen = TCP_ReadFromServer(fd,szBuffer,MAX_BUF_LEN);
    if (iLen > 0)
    {
        /*连接不成功，直接退出*/
        if (0 != strncmp(szBuffer,PH_TCP_OK,PH_RESPOND_LEN))
        {
            PH_TRACE("Connect TCP 6060 failed.\n");
            iRet = -1;
            goto OUT;
        }
    }

    /*发送AUTH ROUTER*/
    snprintf(szBuffer, MAX_BUF_LEN, PHC_AUTH_ROUTE NEWLINE_R_N);
    TCP_SendTo(fd, szBuffer);

    /*获取服务器返回的挑战串*/
    iLen = TCP_ReadFromServer(fd,szBuffer,MAX_BUF_LEN);
    if (iLen > 0)
    {
        /*连接不成功，直接退出*/
        if (0 != strncmp(szBuffer,PH_AUTH_ROUTE_OK,PH_RESPOND_LEN))
        {
            PH_TRACE("Get AUTH ROUTER respond failed.\n");
            iRet = -1;
            goto OUT;
        }
        else
        {
            /*获取挑战串*/
            sscanf(szBuffer, "%*d %s", uszBase64Key);
            PH_TRACE("server szBase64Key =%s\n",uszBase64Key);

            /*解密挑战串*/
            iLen = Base64_Decode(uszBase64Key,strlen(uszBase64Key),g_uszKey);
            PH_TRACE("server szKey=%s,strlen(uszKey)=%d\n",g_uszKey,strlen(g_uszKey));        
        }
    }/*end of  if (iLen > 0)*/

    /*构造校验报文*/
    iLen = GetAuthInfo(szBuffer,pcsPassport,pcsPassword,g_uszKey);
    TCP_SendTo(fd, szBuffer);

    /*读取校验结果*/
    iLen = TCP_ReadFromServer(fd,szBuffer,MAX_BUF_LEN);
    if (iLen <= 0)
    {
        PH_TRACE("Get auth result failed.\n");
        iRet = -1;
        goto OUT;
    }
    else
    {
        /*注册域名*/
        iRet = RegisterDomain(fd,szBuffer);
        if (PROC_NORMAL != iRet)
        goto OUT;
    }

    /*域名确认*/
    sprintf(szBuffer, PHC_CNFM NEWLINE_R_N);
    TCP_SendTo(fd,szBuffer);

    /*获取会话编号和初始序号*/
    iLen = TCP_ReadFromServer(fd,szBuffer,MAX_BUF_LEN);
    if (iLen > 0)
    {
        /*域名注册成功，直接退出*/
        if (0 == strncmp(szBuffer,PH_REGISTER_FAIL, PH_RESPOND_LEN))
        {
            PH_TRACE("No name registered.\n");
            iRet = -1;
            goto OUT;
        }
        else if (0 == strncmp(szBuffer,PH_AUTH_OK, PH_RESPOND_LEN))
        {
            /*域名确认成功，获取会话编号和序列号*/
            sscanf(szBuffer,"250 %ld %ld", &g_stClientInfo.lSessionID, &g_stClientInfo.lSerialNum);
        }
        else
        {
            PH_TRACE("Register failed Return Code:[%d].\n", atoi(szBuffer));
            iRet = -1;
            goto OUT;
        }
    }/*end of  if (iLen > 0)*/

    /*结束本次会话*/
    sprintf(szBuffer,PHC_QUIT NEWLINE_R_N);
    TCP_SendTo(fd,szBuffer);

    /*读取服务器返回结果*/
    iLen = TCP_ReadFromServer(fd,szBuffer,MAX_BUF_LEN);
    if (iLen > 0)
    {
        /*结果已经不重要了*/
        PH_TRACE("Quit result %s\n",szBuffer);
    }

    iRet = PROC_NORMAL;

OUT:
   if (fd)
       close(fd);

   return iRet;
}

/******************************************************************************
功能: 在登陆花生壳服务之前取得用户应该使用的花生壳服务器
             的地址，以及该用户的类型。
参数: pcsPassport         input     护照名
             pcsPassword        input    密码(明文)
返回: PROC_NORMAL/其他错误
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
static int PH_login(const char *pcsPassport, const char *pcsPassword)
{
    int iRet =  0;
    int iLen = 0;
    int fd = 0;
    char szHeadBuf[MAX_BUF_LEN] = {0};
    char szBodyBuf[MAX_BUF_LEN] = {0};
    char szTemp[MAX_STRING_LEN] = {0};
  
    PH_TRACE_INTO_FUNC;

    /*建立TCP链接*/
    fd = TCP_connect( PH_SERVER_URL, HTTP_PROTOCOL_PORT);
    if ( fd < 0 )
    {
        PH_TRACE("Setup TCP connection failed.\n");
        return -1;
    }

    /*构造登录报文*/
    iLen = GetUserInfo(szBodyBuf,pcsPassport,pcsPassword);
    PH_TRACE("szBuf=%s\n",szBodyBuf);

    /*构造报文头部*/
    snprintf(szHeadBuf, MAX_BUF_LEN, "POST /userinfo.asmx HTTP/1.1"NEWLINE_R_N	\
                       "Host: %s"NEWLINE_R_N	\
                       "Content-Type: text/xml; charset=utf-8"NEWLINE_R_N	\
                       "Content-Length: %d"NEWLINE_R_N	\
                       "SOAPAction: \"http://tempuri.org/GetMiscInfo\""NEWLINE_R_N NEWLINE_R_N,
                       PH_SERVER_URL, iLen);

    /*发送登录报文*/
    TCP_SendTo(fd,szHeadBuf);
    TCP_SendTo(fd,szBodyBuf);

    /*读取服务器的回应*/
    iLen = TCP_ReadFromServer(fd,szBodyBuf,MAX_BUF_LEN);
    if (iLen > 0)
    {
        /*获取服务器IP*/
        iRet = GetValueFromXML(szBodyBuf,PH_SERVER_IP,g_szServerIP,MAX_STRING_LEN);
        if(iRet > 0)
        {
            StrTrim(g_szServerIP,CHAR_BLANK);
            PH_TRACE("Get server ip:%s\n",g_szServerIP);
        }
        else
        {
            PH_TRACE("Get server ip failed.\n");
            iRet = -1;
            goto OUT;
        }

        /*获取用户类型*/
        iRet = GetValueFromXML(szBodyBuf,PH_USER_TYPE,szTemp,MAX_STRING_LEN);
        if(iRet > 0)
        {
            StrTrim(szTemp,CHAR_BLANK);
            PH_TRACE("Get user type:%s\n",szTemp);

            if (1 == atoi(szTemp))
                g_iClientType = ADVANCED_USER;
            else
                g_iClientType = STANDARD_USER;
        }
        else
        {
            PH_TRACE("Get user type failed.\n");
            iRet = -1;
            goto OUT;
        }

        iRet =  PROC_NORMAL;
    }/*end of if (iLen > 0)*/

OUT:
    if (fd)
        close (fd);
   
    PH_TRACE_OUT_FUNC;

    return iRet;
}
/******************************************************************************
功能: 花生壳客户端(peanuthullc)进程初始化函数
参数: 无   
返回: PROC_NORMAL/其他错误
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
static int PH_init(void)
{
    PH_TRACE_INTO_FUNC;

    PH_TRACE_OUT_FUNC;

    return PROC_NORMAL;
}
 /******************************************************************************
功能: 花生壳客户端(peanuthullc)使用说明
参数: pszProName         input      进程名   
返回: 无
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
static void Usage(char *psProName)
{
    PH_TRACE_INTO_FUNC;

    fprintf(stderr, "Usage:\n\t"
		        "%s passport password\n"
		        "\nNotes:\n\tThe max length of passport or password is 16 Bytes.\n\n"
		        "",psProName);

    PH_TRACE_OUT_FUNC;

    return ;
}

/******************************************************************************
功能: 信号处理函数
参数: 无     
返回: 无
创建: Kevin 
时间: 2010-02-23
*******************************************************************************/
static void SigalHandler(int iSigNum)
{
    PH_TRACE_INTO_FUNC;

    PH_TRACE("The signal is %d\n",iSigNum);

    g_iClientState = PHC_LOGOUT;

    PH_TRACE_OUT_FUNC;

    return ;
}
/******************************************************************************
功能: 监控信号的函数钩子表
参数: 无     
返回: 无
创建: Kevin 
时间: 2010-02-23
*******************************************************************************/
static void SignalInit(void)
{
    signal(SIGHUP, SigalHandler);
    signal(SIGINT, SigalHandler);
    signal(SIGQUIT, SigalHandler);
    signal(SIGKILL, SigalHandler);
    signal(SIGTERM, SigalHandler);

    return;
}
/******************************************************************************
功能: 花生壳客户端(peanuthullc)进程入口函数
参数: argc         input      
             argv[]      input      
返回: 进程的运行状态
创建: Kevin 
时间: 2009-11-18
*******************************************************************************/
int main(int argc, char *argv[])
{
    int iProcStatus =  PROC_NORMAL;
    char szPassport[MAX_PASSPORT_LEN] = {0};
    char szPassword[MAX_PASSPORT_LEN] = {0};

    PH_TRACE_INTO_FUNC;

    /*判断输入参数是否正确*/
    if (argc != 3)
    {
        Usage(argv[0]);
        return -1;
    }

    strcpy(szPassport,argv[1]);
    strcpy(szPassword,argv[2]);

    /*注册信号处理函数*/
    SignalInit();

    /*进程初始化*/
    iProcStatus = PH_init();
    if (PROC_NORMAL != iProcStatus)
    {
        PH_TRACE("Init process failed.\n");
        return iProcStatus;
    }

    do
    {
        /*登录花生壳服务器*/
        iProcStatus = PH_login(szPassport,szPassword);
        if (PROC_NORMAL != iProcStatus)
        {
            PH_TRACE("Login PH failed.\n");
            return PROC_LOGIN_FAIL;
        }

        /*验证并注册域名*/
        iProcStatus = PH_auth(szPassport,szPassword);
        if (PROC_NORMAL != iProcStatus)
        {
            PH_TRACE("Auth PH failed.\n");
            return PROC_AUTH_FAIL;
        }

        g_iClientState = PHC_UPDATE;
        iProcStatus = PH_update();
    }while(PROC_UPDATE_FAIL == iProcStatus);
    
    return iProcStatus;
}

#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif
