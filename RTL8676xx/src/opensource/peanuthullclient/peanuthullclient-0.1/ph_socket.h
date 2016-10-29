/*****************************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : ph_socket.h
 文件描述 : Socket操作入口文件的信息定义

 函数列表 :

 修订记录 :
            创建 :  Kevin
            日期 :  2009-11-18
            描述 :

******************************************************************************/

#ifndef __PH_SOCKET_H
#define __PH_SOCKET_H

#include "stdio.h"


/******************************************************************************
                        *                                 DEBUG函数                                    *
 ******************************************************************************/
 #define PH_DEBUG
 
#ifdef PH_DEBUG
    #define PH_TRACE(fmt,args...) printf("%s:%4d %20s: " fmt, __FILE__, __LINE__, __FUNCTION__, ##args)
    #define PH_TRACE_INTO_FUNC PH_TRACE("##In## %s\n", __FUNCTION__)
    #define PH_TRACE_OUT_FUNC PH_TRACE("##Out##%s\n",__FUNCTION__)
#else
    #define PH_TRACE(fmt, args...) 
    #define PH_TRACE_INTO_FUNC 
    #define PH_TRACE_OUT_FUNC 
#endif


/******************************************************************************
                        *                                 宏定义                                   *
 ******************************************************************************/
 #define TCP_PROTOCOL_PORT             "tcp"
#define	SOCK_READ_WAIT_SEC		4    /*second*/
#define	SOCK_READ_NOWAIT		       0

 /*****************************************************************************
                        *                               函数申明                       *
 ******************************************************************************/
int TCP_ReadFrom(int fd, char *psBuf, int iBufLen, int iTimeout);
int TCP_SendTo( int fd, void *pvBuf );
int TCP_connect(const char *pcsHost,const char *pcsPort);
int TCP_ReadFromServer(int fd, char *psBuf, int iBufLen);

#endif /* __PH_SOCKET_H */

