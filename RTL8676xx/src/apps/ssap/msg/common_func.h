/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : common_func.h
 文件描述 : 针对模块处理的公共函数封装

 修订记录 :
          1 创建 : 李伟
            日期 : 2008-9-1
            描述 : 具体用法请看lan.c
**********************************************************************/

#ifndef _COMMON_FUNC_
#define _COMMON_FUNC_

#include <autoconf.h>
#include <tbsmsg.h>
#include <sys/time.h>
#include <unistd.h>

typedef enum
{
    FROM_CACHE,
    FROM_CFG,
    CACHE_CFG,
}ValuePostionType;

/*节点值条目*/
typedef struct {
    char *pszNodeName;
    char *pszNodeValue;
} ST_SET_NODE_VAL_ENTRY;

/******************************************************************************
                                                        common macro
******************************************************************************/
#define NONE          "\033[m"
#define LIGHT_RED "\033[0;31m"
#define GREEN         "\033[0;32m"

/* 终端中使用的颜色常量定义 */
#define ECHO_NONE           "\033[0m"

#define ECHO_RED            "\033[0;31m"
#define ECHO_GREEN          "\033[0;32m"
#define ECHO_YELLOW         "\033[0;33m"
#define ECHO_BLUE           "\033[0;34m"
#define ECHO_PURPLE         "\033[0;35m"

/* 闪烁 */
#define ECHO_FLASH_RED      "\033[5;31m"
#define ECHO_FLASH_GREEN    "\033[5;32m"
#define ECHO_FLASH_YELLOW   "\033[5;33m"
#define ECHO_FLASH_BLUE     "\033[5;34m"
#define ECHO_FLASH_PURPLE   "\033[5;35m"

#define ATTR_STAT            "Stat"
#define ATTR_CACHE           "Cache"
#define FM_PATH				 "InternetGatewayDevice.DeviceInfo.X_TWSZ-COM_FactoryMode"

//typedef  int BOOL;

#define RET_GOTO(module,Ret,strError,gotoTag)         \
      {\
        if ( RET_FAILED(Ret) )    \
        {   \
            COMMON_TRACE(module,strError); \
            goto gotoTag; \
        }\
      }
#define POINTER_GOTO(module, Pointer, gotoTag, strError, args...)         \
    {\
        if ( !Pointer)    \
        {   \
            COMMON_TRACE(module, strError, ##args); \
            goto gotoTag; \
        }\
     }



#define RET_FALSE(module,Ret,strError,args...)         \
    {\
        if ( RET_FAILED(Ret) )    \
        {   \
            COMMON_TRACE(module, strError, ##args); \
            return false; \
        }\
     }

#define RET_RETURN(module,Ret,strError,args...)         \
    {\
        if ( RET_FAILED(Ret) )    \
        {   \
            COMMON_TRACE(module, strError, ##args); \
            return Ret; \
        }\
    }
#define RET_LOG(module,Ret,strError,args...)         \
    {\
        if ( RET_FAILED(Ret) )    \
        {   \
            COMMON_TRACE(module, strError, ##args); \
        }\
    }


#define POINTER_RETURN(module,Pointer,strError,args...)         \
    {\
        if ( !Pointer)    \
        {   \
            COMMON_TRACE(module, strError, ##args); \
            return TBS_NULL_PTR; \
        }\
     }

#define POINTER_FALSE(module,Pointer,strError,args...)         \
    {\
        if ( !Pointer)    \
        {   \
            COMMON_TRACE(module, strError, ##args); \
            return FALSE; \
        }\
    }
#define POINTER_LOG(module,Pointer,strError,args...)         \
    {\
        if ( !Pointer)    \
        {   \
            COMMON_TRACE(module, strError, ##args); \
        }\
    }

#define RET_VAL_IF_FAIL(module, con, ret)   \
    {\
        if ( !(con) ) \
        {\
            COMMON_TRACE(module, "%s failed\n", #con); \
            return ret; \
        }\
    }

#define RET_IF_FAIL(module, con)   \
    {\
        if ( !(con) ) \
        {\
            COMMON_TRACE(module, "%s failed\n", #con); \
            return; \
        }\
    }

#define LOG_IF_FAIL(module, con)   \
    {\
        if ( !(con) ) \
        {\
            COMMON_TRACE(module, "%s failed\n", #con); \
        }\
    }



/******************************************************************************
                                                        common function
******************************************************************************/
int COMM_NullFunc(char *pszPath);


/*
去掉路径最后的'.'
注意:此函数将修改传入的指针内容
*/
void COMM_RemovePathLastDot(char *pszPath);


/*
复制路径,复制时去掉路径最后的'.'
*/
void COMM_CopyPathNoLastDot(char *pszDestPath, const char *pszSrcPath);


/*************************************************************************
Function:    char *  COMM_GetPathNode(char *pszCurPath,int iPos)
Description:   获取路径中的第n 个节点,
Input:         char *pszCurPath ,                       当前处理的路径
                   int iPos,                                        获取第几个节点
Output:        无
Return:        NULL ,       失败;
                    其它,     成功，值的指针
Others:         为了简化接口，这里使用了全局变量存储返回值，
                    所以不支持多线程的并发调用
*************************************************************************/
const char * COMM_GetPathNode(char *pszCurPath,int iPos);

/*************************************************************************
Function:    int  COMM_GetPathIndex(char *pszCurPath,int *iIndex)
Description:   获取路径中的index
Calls:         无
Data Accessed:
Data Updated:
Input:         char *pszCurPath ,                       当前处理的路径
Output:        int *iIndex
Return:        当前index的数量
Others:         为了简化接口，这里使用了全局变量存储返回值，
                    所以不支持多线程的并发调用
*************************************************************************/
int  COMM_GetPathIndex(char *pszCurPath,int *iIndex);

/*************************************************************************
Function:      int GetPathFromCurrentPath(char *pszCurPath,int iChangeLevel,char *pszExtraPath,char *pszDestPath,unsigned long *pulPathLen)
Description:   通过现有的路径拼装新路径
Input:         char *pszCurPath ,                       当前处理的路径
                   int iChangeLevel,                         改变的路径层数(0或负值，表示从路径右边去掉的节点数)
                   char *pszExtraPath                     附加 的路径，为NULL则表示不添加
Output:        无
Return:        NULL ,       失败;
                    其它,     成功，值的指针
Others:
*************************************************************************/
int COMM_GetPathEx(const char *pszCurPath,int iChangeLevel,char *pszExtraPath,char *pszDestPath,unsigned long *pulPathLen);



/*************************************************************************
Function:      char * COMM_GetNodeValue(char *pszPath,ValuePostionType *eGetMode)
Description:   统一的获取值的函数，包括从缓存获取，从配置树获取和先从缓存后从配置树三种方式
Input:         char *pszPath ,                       当前处理的路径
                   ValuePostionType eGetMode  获取值得方式
                                                                   (    FROM_CACHE 从缓存, FROM_CFG 从配置树,  CACHE_CFG 先缓存后配置树)
Output:        ValuePostionType eGetMode 从什么地方获取到的值
Return:        NULL ,       失败;
                    其它,     成功，值的指针
Others:
*************************************************************************/
const char * COMM_GetNodeValue(char *pszPath,ValuePostionType eGetMode);
ValuePostionType COMM_GetLastPos();/*返回最近一次COMM_GetNodeValue函数获取值的位置*/


/*************************************************************************
Function:    char * COMM_GetNodeValueEx(char *pszCurPath,int iChangeLevel,char *pszExtraPath,ValuePostionType eGetMode)
Description:  获取值的封装接口，支持:对路径可以通过从现有路径拼装获得
                    1.对想获取值的路径可以通过从现有路径拼装获得
                    2.支持从缓存，从配置树，先缓存后配置树等几种方式获得节点值
Input:         char *pszCurPath ,                       当前处理的路径
                   int iChangeLevel,                         改变的路径层数(0或负值，表示从路径右边去掉的节点数)
                   char *pszExtraPath                     附加 的路径，为NULL则表示不添加
                   ValuePostionType eGetMode        获取值的方式
                                                                         (    FROM_CACHE 从缓存, FROM_CFG 从配置树,  CACHE_CFG 先缓存后配置树)
Output:        无
Return:        NULL ,       失败;
                    其它,     成功，值的指针
Others:
*************************************************************************/
const char * COMM_GetNodeValueEx(const char *pszCurPath,int iChangeLevel,char *pszExtraPath,ValuePostionType eGetMode);


/*************************************************************************
Function:    COMM_GetMultiNodeValueEx
Description:  获取多个节点值的封装接口，支持:对路径可以通过从现有路径拼装获得
                    1.对想获取值的路径可以通过从现有路径拼装获得
                    2.支持从缓存，从配置树，先缓存后配置树等几种方式获得节点值
Input:         char *pszCurPath ,               当前处理的路径
               int iChangeLevel,                改变的路径层数(0或负值，表示从路径右边去掉的节点数)
               ValuePostionType eGetMode        获取值的方式  (    FROM_CACHE 从缓存, FROM_CFG 从配置树,  CACHE_CFG 先缓存后配置树)
               unsigned short usNodeCount       节点个数
               ...                              指针变参,用来接收获取的节点值,
                                                如: char *pcNodeName, char **ppcNodeValue
Output:        无
Return:        成功:TBS_SUCCESS
               失败:错误码
Others:
*************************************************************************/
int COMM_GetMultiNodeValueEx(char *pszCurPath, int iChangeLevel,
    ValuePostionType eGetMode, unsigned short usNodeCount, ...);



/*************************************************************************
Function:    char * COMM_GetAndSynValueEx(char *pszCurPath,int iChangeLevel,char *pszExtraPath,ValuePostionType eGetMode)
Description:  获取值并设置同步标记，同步标记表示会从缓存中去掉，供commit函数等调用
Input:         char *pszCurPath ,                       当前处理的路径
                   int iChangeLevel,                         改变的路径层数(0或负值，表示从路径右边去掉的节点数)
                   char *pszExtraPath                     附加 的路径，为NULL则表示不添加
                   ValuePostionType eGetMode        获取值的方式
                                                                        (    FROM_CACHE 从缓存, FROM_CFG 从配置树,  CACHE_CFG 先缓存后配置树)
Output:        无
Return:        NULL ,       失败;
                    其它,     成功，值的指针
Others:
*************************************************************************/
const char * COMM_GetAndSynValueEx(char *pszCurPath,int iChangeLevel,char *pszExtraPath,ValuePostionType eGetMode);

/*************************************************************************
Function:    int  COMM_SetValueEx(char *pszCurPath,int iChangeLevel,char *pszExtraPath,char *pszValue)
Description:  设置值的封装接口，支持:对路径可以通过从现有路径拼装获得
Input:         char *pszCurPath ,                       当前处理的路径
                   int iChangeLevel,                         改变的路径层数(0或负值，表示从路径右边去掉的节点数)
                   char *pszExtraPath                     附加 的路径，为NULL则表示不添加
                   char *pszValue                            设置的值
Output:        无
Return:        TBS_SUCCESS ,      成功;
                    其它,     失败
Others:
*************************************************************************/
int COMM_SetValueEx(const char *pszCurPath,int iChangeLevel,char *pszExtraPath, const char *pszValue);


/*************************************************************************
Function:       COMM_SetMultiNodeValueEx
Description:    设置多个节点值的封装接口，支持:对想设置值的路径可以通过从现有路径拼装获得
Calls:          无
Data Accessed:
Data Updated:
Input:         char *pszCurPath ,               当前处理的路径
               int iChangeLevel,                改变的路径层数(0或负值，表示从路径右边去掉的节点数)
               unsigned short usNodeCount       设置节点的个数
               ...                              指针变参,用来接收要设置的节点名称和节点值,
                                                如: char *pcNodeName, char *pcNodeValue
Output:        无
Return:        TBS_SUCCESS ,      成功;
                    其它,     失败
Others:
*************************************************************************/
int COMM_SetMultiNodeValueEx(const char *pszCurPath, int iChangeLevel, unsigned short usNodeCount, ...);

/*************************************************************************
Function:       COMM_SetMultiCacheValueEx
Description:    设置多个节点缓存的封装接口

Input:         usMID                    节点值所属的MID
               char *pszCurPath ,       当前处理的路径
               int iChangeLevel,        改变的路径层数(0或负值，表示从路径右边去掉的节点数)
               usNodeCount              设置节点的个数,为0时表示使用的变参为ST_SET_NODE_VAL_ENTRY
                                        数组。大于等于1时表示使用的变参为节点名称和值的指针。
               ...                      (usNodeCount等于0时)指向ST_SET_NODE_VAL_ENTRY数组的指针,
                                        该数组中最后的元素必须为NULL指针以表示数组的结尾。
                                        (usNodeCount>0时)指针变参,形式
                                        如: char *pcNodeName, char *pcNodeValue
Return:        TBS_SUCCESS ,      成功;
               其它,              失败
Others:
*************************************************************************/
int COMM_SetMultiCacheValueEx(unsigned short usMID, const char *pszCurPath, 
                              int iChangeLevel, unsigned short usNodeCount, ...);

/*************************************************************************
Function:       COMM_GetMultiCacheValueEx
Description:    获取多个节点缓存值的封装接口
                1.对想获取缓存值的路径可以通过从现有路径拼装获得
                2.支持从缓存,从配置树,先缓存后配置树等几种方式获得节点值

Input:   usMID                       节点所属的MID      
         char *pszCurPath ,          当前处理的路径
         int iChangeLevel,           改变的路径层数(0或负值，表示从路径右边去掉的节点数)
         unsigned short usNodeCount  设置节点的个数
         ...                         指针变参,用来接收要设置的节点名称和节点值,
                                     如: char *pcNodeName, char **ppcNodeValue
Return:        TBS_SUCCESS , 成功;
               其它,         失败
Others:
*************************************************************************/
int COMM_GetMultiCacheValueEx(unsigned short usMID, const char *pszCurPath, 
                              int iChangeLevel, unsigned short usNodeCount, ...);                              

/*************************************************************************
Function:       COMM_DelMultiCacheNode
Description:    删除多个节点缓存值的封装接口
                1.对想删除缓存值的路径可以通过从现有路径拼装获得

Input:   usMID                       节点所属的MID      
         char *pszCurPath ,          当前处理的路径
         int iChangeLevel,           改变的路径层数(0或负值，表示从路径右边去掉的节点数)
         unsigned short usNodeCount  设置节点的个数
         ...                         指针变参, 指向要删除缓存的节点名称
Return:        TBS_SUCCESS , 成功;
               其它,         失败
Others:
*************************************************************************/
int COMM_DelMultiCacheNode(unsigned short usMID, const char *pszCurPath,
                           int iChangeLevel, unsigned short usNodeCount, ...);
                           
/*************************************************************************
Function:    int COMM_SetCacheValueEx(unsigned short usMID,char *pszCurPath,int iChangeLevel,char *pszExtraPath,char *pszValue)
Description:  设置缓存值的封装接口，支持:对想设置值的路径可以通过从现有路径拼装获得
Calls:         无
Data Accessed:
Data Updated:
Input:     unsigned short            调用模块的MID
               char *pszCurPath,       当前处理的路径
               int iChangeLevel,        改变的路径层数(0或负值，表示从路径右边去掉的节点数)
               char *pszExtraPath    附加的路径，为NULL则表示不添加
               char *pszValue           设置的值
Output:        无
Return:        TBS_SUCCESS ,      成功;
               其它,     失败
Others:
*************************************************************************/
int COMM_SetCacheValueEx(unsigned short usMID,const char *pszCurPath,int iChangeLevel, 
                         char *pszExtraPath, char *pszValue);

/*************************************************************************
Function:  
     const char *COMM_GetCacheValueEx(unsigned short usMID, const char *pszCurPath, 
                                      int iChangeLevel, char *pszExtraPath)
                                
Description:  获取缓存值的封装接口，支持:对想设置值的路径可以通过从现有路径拼装获得

Input:     usMID,          调用模块MID
           pszCurPath,     当前处理的路径
           iChangeLevel,   改变的路径层数(0或负值，表示从路径右边去掉的节点数)
           pszExtraPath    附加的路径，为NULL则表示不添加
           
Return:    缓存值指针,   成功
           NULL,         失败
Others:
*************************************************************************/
const char *COMM_GetCacheValueEx(unsigned short usMID, const char *pszCurPath, 
                                 int iChangeLevel, char *pszExtraPath);

/*************************************************************************
Function: void COMM_SynCache2CFG(unsigned short usMID)

Description:  将模块缓存值保存到配置树中

Input:     usMID,   模块MID

Return:   无

Others:
*************************************************************************/
void COMM_SynCache2CFG(unsigned short usMID);

/*************************************************************************
Function:    int COMM_AddObjectEx(char *pszCurPath,int iChangeLevel,char *pszExtraPath)
Description:  添加object的封装接口，支持:对想设置值的路径可以通过从现有路径拼装获得
Input:         char *pszCurPath ,                       当前处理的路径
                   int iChangeLevel,                         改变的路径层数(0或负值，表示从路径右边去掉的节点数)
                   char *pszExtraPath                     附加 的路径，为NULL则表示不添加
Output:        无
Return:        TBS_SUCCESS ,      成功;
                    其它,     失败
Others:
*************************************************************************/
int COMM_AddObjectEx(char *pszCurPath,int iChangeLevel,char *pszExtraPath);

/*************************************************************************
Function:    int COMM_DeleteObjectEx(char *pszCurPath,int iChangeLevel,char *pszExtraPath)
Description:  删除object的封装接口，支持:对想设置值的路径可以通过从现有路径拼装获得
Input:         char *pszCurPath ,                       当前处理的路径
                   int iChangeLevel,                         改变的路径层数(0或负值，表示从路径右边去掉的节点数)
                   char *pszExtraPath                     附加 的路径，为NULL则表示不添加
Output:        无
Return:        TBS_SUCCESS ,      成功;
                    其它,     失败
Others:
*************************************************************************/
int COMM_DeleteObjectEx(char *pszCurPath,int iChangeLevel,char *pszExtraPath);


/*************************************************************************
Function:    int CFG_CheckForEach(char *pszCurPath,char *pszCurVal,int iChangeLevel,CheckEachFunc pfnCheck)
Description:  对某层路径进行遍历
Input:         char *pszCurPath ,                       当前处理的路径 (a.b.1.c)
                   int iChangeLevel,                         遍历的路径的层次，表示从右边去掉的节点数
                   CheckEachFunc pfnCheck,           检查函数
                   void *pData,                                 传入的值，供模块调用
                   BOOL bRet,                                    表示遇到什么状态返回，
                                                                        true表示遇到第一个TBS_SUCCESS返回，false遇到第一个非TBS_SUCCESS返回
Output:        无
Return:       TBS_SUCCESS ,      成功;
                    其它,     失败
Others:
*************************************************************************/
typedef int (*CheckEachFunc)(char *pszNewNodePath, void *pData, char *pszEachNodePath);
int CFG_CheckForEach(char *pszCurPath, int iChangeLevel, CheckEachFunc pfnCheck, void *pData, int bRet);

/*************************************************************************
Function:    int CFG_ForEachEx(char *pszCurPath, ForEachFunc pfnForEach, void *pData)
Description:  对某层路径进行遍历
Calls:         无
Data Accessed:
Data Updated:
Input:         char *pszCurPath ,                       当前处理的路径
                   ForEachFunc pfnForEach            遍历使用的函数
                   void *pData,                                传入的私有数据
Output:        无
Return:       true ,      成功;
                   false,     失败
Others:
*************************************************************************/
typedef int (*ForEachFunc)(char *pszNewNodePath, void *pData);
int CFG_ForEachEx(char *pszCurPath, ForEachFunc pfnForEach, void *pData,int bRet);

/*************************************************************************
Function:    int COMM_SynNode2Cache(char *pszPath)
Description:  把值同步到缓存中
Input:         char *pszPath ,                       当前处理的路径
Output:        无
Return:       TBS_SUCCESS ,      成功;
                    其它,     失败
Others:
*************************************************************************/
int COMM_SynNode2Cache(unsigned short usMID,char *pszPath);

/*************************************************************************
Function:    int COMM_ApplyAllNodes(unsigned short usMID)
Description:  生效该模块所有的节点
Input:         unsigned short usMID ,                       当前的MID
Output:        无
Return:       TBS_SUCCESS ,      成功;
                    其它,     失败
Others:
*************************************************************************/
int COMM_ApplyAllNodes(unsigned short usMID);

/*************************************************************************
Function:    int COMM_ApplyAllNodesEx(unsigned short usMID,const char * pszPath)
Description:  生效该模块这个路径下所有的节点
Input:         unsigned short usMID ,                       当前的MID
                   const char * pszPath                          需要生效的路径，必须以 .结束
Output:        无
Return:       TBS_SUCCESS ,      成功;
                    其它,     失败
Others:
*************************************************************************/
int COMM_ApplyAllNodesEx(unsigned short usMID,const char * pszPath);

/*************************************************************************
Function:    int COMM_ParseCustomMsg(ST_MSG *pstMsg,char *pszFormat, ...)
Description:  解析自定义消息体
Calls:         无
Data Accessed:
Data Updated:
Input:         ST_MSG *pstMsg       消息体
                  char *pszFormat       需要的解析name串，以空格隔开。如:"name1 name2"
Output:        变参，会把解析的值传入到变参中
Return:       TBS_SUCCESS ,      成功;
                    其它,     失败
Others:        解析的消息必须是 name1=value1...类型的
*************************************************************************/
int COMM_ParseCustomMsg(ST_MSG *pstMsg,
                                                        char *pszFormat,
                                                        ...);

/*************************************************************************
Function:    int COMM_MakeCustomMsg(ST_MSG *pstMsg,unsigned short usMsgType,unsigned short usSrcMID,unsigned short usDstMID,unsigned short usNodeNum, ...);
Description:  构造自定义消息体
Calls:         无
Data Accessed:
Data Updated:
Input:         ST_MSG *pstMsg                  消息体
                  unsigned short usMsgType  消息类型
                  unsigned short usSrcMID     源MID
                  unsigned short usDstMID     目的MID
                  unsigned short usNodeNum 发送消息的节点数
                  变参                                   构造用的字符串
Output:
Return:       TBS_SUCCESS ,      成功;
                    其它,     失败
Others:       构造的消息必须是 name1=value1...类型的
*************************************************************************/
int COMM_MakeCustomMsg(ST_MSG **ppstMsg,unsigned short usMsgType,unsigned short usSrcMID,unsigned short usDstMID,unsigned short usNodeNum, ...);

/*************************************************************************
Function:    int COMM_MakeAndSendCustomMsg(unsigned short usMsgType,unsigned short usSrcMID,unsigned short usDstMID,unsigned short usNodeNum, ...);
Description:  构造并发送自定义消息体
Calls:         无
Data Accessed:
Data Updated:
Input:        unsigned short usMsgType  消息类型
                  unsigned short usSrcMID     源MID
                  unsigned short usDstMID     目的MID
                  unsigned short usNodeNum 发送消息的节点数
                  变参                                  构造用的字符串
Output:
Return:       TBS_SUCCESS ,      成功;
                    其它,     失败
Others:       构造的消息必须是 name1=value1...类型的
*************************************************************************/
int COMM_MakeAndSendCustomMsg(unsigned short usMsgType,unsigned short usSrcMID,unsigned short usDstMID,unsigned short usNodeNum, ...);

/*************************************************************************
Function:    int COMM_ResponseCustomMsg(ST_MSG *pstMsg,unsigned short usMsgType,unsigned short usNodeNum, ...)
Description:  构造并发送自定义消息体的回复消息
Calls:         无
Data Accessed:
Data Updated:
Input:        ST_MSG *pstMsg                   接收到的消息体
                  unsigned short usMsgType  回复的消息类型
                  unsigned short usNodeNum 发送消息的节点数
                  变参                                  构造用的字符串
Output:
Return:       TBS_SUCCESS ,      成功;
                    其它,     失败
Others:       构造的消息必须是 name1=value1...类型的
*************************************************************************/
int COMM_ResponseCustomMsg(ST_MSG *pstMsg,unsigned short usMsgType,unsigned short usNodeNum, ...);

/*************************************************************************
Function:       COMM_MakeAndSendBinMsg
Description:    构造并发送自定义消息体,二进制格式
Calls:          无
Data Accessed:
Data Updated:
Input:          unsigned short usMsgType    消息类型
                unsigned short usSrcMID     源MID
                unsigned short usDstMID     目的MID
                unsigned short usMsgLen     消息体长度
                char *pcMsgBody             消息体数据
Output:
Return:         TBS_SUCCESS ,      成功;
                    其它,     失败
Others:         构造的消息可以是任意类型的二进制格式
*************************************************************************/
int COMM_MakeAndSendBinMsg(unsigned short usMsgType,unsigned short usSrcMID,unsigned short usDstMID,unsigned short usMsgLen, char *pcMsgBody);


/*************************************************************************
Function:    COMM_SetCacheValue(char *pszPath,char *pszValue);
Description:  设置缓存值
Calls:         无
Data Accessed:
Data Updated:
Input:        unsigned short usMID  MID
                  char *pszPath              路径
                  char *pszValue            设置的值
Output:
Return:       TBS_SUCCESS ,      成功;
                    其它,     失败
Others:
*************************************************************************/
int COMM_SetCacheValue(unsigned short usMID,char *pszPath,char *pszValue);

/*************************************************************************
Function:    const char *COMM_GetCacheValue(unsigned short usMID, char *pszPath)
Description:  获取节点cache值

Input:      usMID      模块的MID
            pszPath    节点路径

Return:       节点缓存值指针 ,      成功;
              NULL,                 失败
Others:      
*************************************************************************/
const char *COMM_GetCacheValue(unsigned short usMID, char *pszPath);

/*************************************************************************
Function:    int COMM_DelCacheNode(unsigned short usMID, char *pszPath)
Description: 删除参数pszPath指向的路径节点的缓存

Input:      usMID      模块的MID
            pszPath    节点路径

Return:       TBS_SUCCESS,      成功;
              其它,             失败
Others:      
*************************************************************************/
int COMM_DelCacheNode(unsigned short usMID, char *pszPath);

/*************************************************************************
Function:    int COMM_CleanCache(unsigned short usMID)
Description:  清除cache
Calls:         无
Data Accessed:
Data Updated:
Input:        unsigned short usMID      模块的MID
Output:
Return:       TBS_SUCCESS ,      成功;
                    其它,     失败
Others:
*************************************************************************/
int COMM_CleanCache(unsigned short usMID);

/**********************************************************************
         设置set ack消息中错误路径，由模块在冲突检查时调用
**********************************************************************/
void COMM_SetLastErrPath(char *pszPath);


/************************************************************
Function:      int COMM_SystemEx(const char *pszCmd,char * pszResult,int iCount)
Description:   获取shell输出的表信息中某些行的某些字段，用法类似于strtok
Input:         const char *pszCmd,              要执行的命令
                   char * pszResult,                    执行的输出结果
                   int iCount                                当前buffer的长度
Output:
Return:        false ,       失败;
                    true,     成功;
Others:
************************************************************/
int COMM_SystemEx(const char *pszCmd,char * pszResult,int iCount);

/************************************************************
Function:      int COMM_StrtokEx(char *szBuf,int iMinLine,int iMaxLine,int iParaNum,...)
Description:   获取shell输出的表信息中某些行的某些字段，用法类似于strtok
Input:         char *szBuf,                             输出的字符buf
                   char *szDelim,                         分割符字符串，如果分割每列用的字符集合
                   int iMinLine, int iMaxLine,        需要获取信息的行数，>=iMinLine,<=iMaxLine,从0开始编号，
                                                                    当iMinLine为0时表示从最上面一行开始，iMaxLine为-1表示上限没有限制
                   int iParaNum,                           获取的列数，即变参有几对
                   ...                                              变参，需要获取的列和对应值的指针，如:2,pCol2,4,pCol4 (注意col数必须以升序排列)
Output:        ...                                              第偶数个变参会把前一个列值的指针输出
Return:        false,       失败;
                    true,     成功
Others:       用法类似于strtok，第一个参数为NULL,表示继续循环，非空表示开始循环遍历
************************************************************/
int COMM_StrtokEx(char *szBuf,char *szDelim,int iMinLine,int iMaxLine,int iParaNum,...);

BOOL COMM_IsPathInCache(unsigned short usMID, char *pszPath);

int COMM_ParserPairValue(char *szBuf,char *szDelim,char *szName,char **pszValue);

void COMM_PrintCurTime();

#endif
