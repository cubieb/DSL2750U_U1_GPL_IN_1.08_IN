/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : om_lib.c
 文件描述 : 用于OM对外提供的库文件


 函数列表 :


 修订记录 :
          1 创建 : 刘泽群
            日期 : 2007-9-29
            描述 :

**********************************************************************/

#include "../include/om.h"

/******************************************************************************
 *                                GLOBAL                                      *
 ******************************************************************************/



/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/


/*=========================================================================*/
/*  函数名称: Web_MsgRequestParse                                          */
/*  函数功能: 来自web模块的请求消息的解析                                  */
/*  输入参数: pstMsg: 接收到的消息                                         */
/*  输出参数: pszName、pszVal, 解析后的name和val的指针                     */
/*  创  建  : 刘泽群 / 2007-9-29                                           */
/*  说  明  : 消息体形如"name=val, 本接口将name和val的指针分别赋予pszName和*/
/*            pszVal， 并将'='改为'\0'                                     */
/*=========================================================================*/
int WEB_MsgRequestParse(ST_MSG *pstMsg, char **pszName, char **pszVal)
{
    if (pstMsg == NULL || pstMsg->stMsgHead.ulBodyLength == 0)
        return TBS_FAILED;

    *pszName = pstMsg->szMsgBody;
    *pszVal  = strchr(pstMsg->szMsgBody, '=');
    if (*pszVal != NULL)
    {
        **pszVal = '\0';
        (*pszVal)++;
    }
    return TBS_SUCCESS;
}


/*=========================================================================*/
/*  函数名称: WEB_MsgRespAddOne                                            */
/*  函数功能: 在发往WEB模块的响应消息中增加一个值                          */
/*  输入参数: nObjNum/nVarNum: 响应消息中对象个数和值的个数                */
/*            strVal         : 响应消息中新加入的值                        */
/*  输出参数: pstMsg         : 增加值后的消息包                            */
/*  返回值  : 成功/失败                                                    */
/*  创  建  : 刘泽群 / 2007-9-29                                           */
/*=========================================================================*/
int WEB_MsgRespAddOne(ST_MSG *pstMsg, unsigned char nObjNum, unsigned char nVarNum, PCSTR strVal)
{
    if (pstMsg == NULL)
        return TBS_FAILED;

    ST_OM_VAL_RESP *pstResp = (ST_OM_VAL_RESP *)pstMsg->szMsgBody;

    // 如果个数不一致， 表示还刚刚开始
    if (pstMsg->stMsgHead.ulBodyLength == 0
     || pstResp->ucObjNum != nObjNum
     || pstResp->ucValNum != nVarNum)
    {
        pstResp->ucObjNum = nObjNum;
        pstResp->ucValNum = nVarNum;
        pstMsg->stMsgHead.ulBodyLength = sizeof(ST_OM_VAL_RESP);
    }

    // 值直接往后面添加
    ST_OM_MSG_VAL *pstVal = (ST_OM_MSG_VAL *)(pstMsg->szMsgBody + pstMsg->stMsgHead.ulBodyLength);
    strcpy(pstVal->ucBuffer, strVal);
    pstVal->usLen = strlen(strVal) + 1;
    pstVal->usType = 0;
    pstMsg->stMsgHead.ulBodyLength += pstVal->usLen + sizeof(ST_OM_MSG_VAL);

    return TBS_SUCCESS;
}

/*=========================================================================*/
/*  函数名称: WEB_MsgRespSetOne                                            */
/*  函数功能: 在发往WEB模块的响应消息中设置一个单值                        */
/*  输入参数: strVal         : 响应消息中设置的值                          */
/*  输出参数: pstMsg         : 增加值后的消息包                            */
/*  返回值  : 成功/失败                                                    */
/*  创  建  : 刘泽群 / 2007-9-29                                           */
/*=========================================================================*/
int WEB_MsgRespSetOne(ST_MSG *pstMsg, PCSTR strVal)
{
    if (pstMsg == NULL)
        return TBS_FAILED;

    pstMsg->stMsgHead.ulBodyLength = 0;

    return WEB_MsgRespAddOne(pstMsg, 1, 1, strVal);
}

/*=========================================================================*/
/*  函数名称: WEB_MsgGetNodeMap                                            */
/*  函数功能: 将消息中的节点串进行分离                                     */
/*  输入参数: szName : 节点串                                              */
/*            aszNode: 节点容器                                            */
/*  返回值  : 分离后的节点个数                                             */
/*  创  建  : liuzequn / 2007-10-9                                         */
/*=========================================================================*/
int WEB_MsgGetNodeMap(char *szName, char aszNode[OM_NODE_MAX][OM_NODE_LEN])
{
    char *pPosDot = NULL;
    char *pPosStart = NULL;
    int  nNodeNum = 0;

    if (aszNode == NULL || szName == NULL)
        return TBS_PARAM_ERR;


    for (pPosDot = szName, pPosStart = szName; pPosStart != NULL && *pPosStart != '\0'; )
    {
        if ((pPosDot = strchr(pPosStart, OM_NODE_TOKEN)) != NULL)
        {
            *pPosDot = '\0';
            strcpy(aszNode[nNodeNum++], pPosStart);
            pPosStart = pPosDot + 1;
        }
        else
        {
            strcpy(aszNode[nNodeNum++], pPosStart);
            break;
        }
    }
    return nNodeNum;

}


/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/


