/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : web_msg.h
 文件描述 : WEB 模块消息处理头文件

 函数列表 :


 修订记录 :
          1 创建 : 刘泽群
            日期 : 2007-8-21
            描述 :

**********************************************************************/
#ifndef __WEB_MSG_H__
#define __WEB_MSG_H__

#include "web_pub.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/
#ifdef WEBPROC_DEBUG
#define WEB_WAIT_MSG_TIME      60       // 等待消息的时间 3s
#else
#define WEB_WAIT_MSG_TIME      60       // 等待消息的时间 5s
#endif

/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/




/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/



/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/

int WEB_SendCmd(char *szDstMid, unsigned short usType, PCSTR szName, PCSTR szValue, BOOL bSerialRand);
int WEB_SendPostMsg(ST_MSG* pstMsg);
int WEB_SendAgtPostMsg(ST_MSG* pstMsg);
int WEB_MsgObjNameGet(char *szName, char  szIndexMap[WEB_MAX_OBJ_NUM][12]);
//int WEB_MsgGetLeafVal(char *szPathName, char *szValue); wei changed it ====>
int WEB_MsgGetLeafVal(char *szPathName, char **p_szValue, unsigned short usDestModID);
int WEB_MsgGetObjVal(char *szPathName,  // 路径名称
                     char *szIndex,     // 索引
                     char szValMap[WEB_MAX_VAL_PER_OBJ][WEB_MAX_OBJ_LEN],   // 待获取的值名称
                     int nValNum,
                     char szValRet[WEB_MAX_VAL_PER_OBJ][WEB_MAX_OBJ_LEN]   // 待获取的值存放
                     );

int WEB_GetResponse();
/*  函数功能: 获取一个消息变量, 并返回终止位置                             */
ST_WEB_VALUE* WEB_MsgGetVal(PCSTR *pszBuffer, unsigned short usLen, int nCount);
/*  函数功能: 将消息体解析成消息值列表                                     */
ST_WEB_VALUE* WEB_MsgParseVal(PCSTR pBuffer,  unsigned short usLen);
/*  函数功能: 获取最近一个消息的类型                                       */
//unsigned short WEB_MsgGetRecvType(); wei changed it ====>
unsigned short WEB_MsgGetRecvType(unsigned short usOrigType);
/*  函数功能: 获取最近一个消息的消息体                                     */
PSTR WEB_MsgGetRecvBody();
/*  函数功能: 获取最近一个消息的消息体长度                                 */
unsigned short WEB_MsgGetRecvLen();
/*  函数功能: 销毁单个消息值                                               */
void WEB_MsgFreeVal(ST_WEB_VALUE *pstVal);
/*  函数功能: 销毁消息值链表                                               */
void WEB_MsgFreeVals(ST_WEB_VALUE *pstVal);



/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/







/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/


#endif /* __WEB_MSG_H__ */


