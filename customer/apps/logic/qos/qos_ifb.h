/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: qos.h
 文件描述: qos模块头文件

 修订记录:
        1. 作者: XuXiaodong
           日期: 2008-12-23
           内容: 创建文件
**********************************************************************/

#ifndef __QOS_H__
#define __QOS_H__

#include "warnlog.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef QOS_DEBUG
    #define QOS_TRACE(fmt, args...) COMMON_TRACE(MID_QOS, ECHO_GREEN fmt ECHO_NONE, ##args)
    #define QOS_LOG_ERR(fmt, args...) COMMON_TRACE(MID_QOS, ECHO_RED fmt ECHO_NONE, ##args)
    #define CMD_PRINT 1 /* 调用tbsSystem时打印命令 */
#else
    #define QOS_TRACE(fmt, args...)
    #define QOS_LOG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_QOS, fmt, ##args)
    #define CMD_PRINT 0 
#endif

#define QOS_TRACE_INTO_FUNC QOS_TRACE("##In## %s\n", __func__)

#define QOS_ASSERT(expr) \
    do{ \
        if(!(expr))\
        {\
            QOS_TRACE("Assert \"%s\" failed\n", #expr); \
            exit(-1); \
        } \
    }while(0)
/********************************************************************
 *                   对函数返回值错误处理的简化定义                 *
 ********************************************************************/
#define QOS_RET_GOTO(Ret, gotoTag, fmt, args...) \
        if ( RET_FAILED(Ret) )  \
        {\
            QOS_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define QOS_RET_RETURN(Ret, fmt, args...)  \
        if ( RET_FAILED(Ret) ) \
        { \
            QOS_TRACE(fmt, ##args); \
            return Ret; \
        }

#define QOS_PTR_GOTO(Pointer,gotoTag, fmt, args...)  \
        if (!Pointer)    \
        { \
            QOS_TRACE(fmt, ##args); \
            goto gotoTag; \
        }

#define QOS_PTR_RETURN(Pointer, Ret, fmt, args...) \
        if (!Pointer)    \
        { \
            QOS_TRACE(fmt, ##args); \
            return Ret; \
        }

#define QOS_CHECK_NULL_PTR(ptr)   QOS_PTR_RETURN(ptr, TBS_NULL_PTR, "TBS_NULL_PTR\n")

/*******************************************************************
 *                       设置节点值的简化宏定义                    *
 *******************************************************************/
#define SET_NODE_VAL(szPath, szValue)  \
        if (CFG_RET_FAILED(CFG_SetNodeVal(szPath, szValue, NULL))) \
        {\
            QOS_TRACE("Set cfg value fail: %s=%s\n", szPath, szValue);  \
            return TBS_FAILED;  \
        }

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define QOS_PATH_PREFIX     "InternetGatewayDevice.X_TWSZ-COM_QoS."
#define QOS_PATH_IPFLOW       QOS_PATH_PREFIX "IPFlow."       //dlink IP flow


#define QOS_NODE_ENABLE     "Enable" 
#define QOS_NODE_WANBAND    "WanBand"     //dlink wan band

//dlink IP flow
#define QOS_NODE_ENAVERAGE     "EnAverage"
#define QOS_NODE_IDLEBAND 	  "OtherIntfsBand"
#define QOS_NODE_SRCSTARTIP   "SrcStartIP"
#define QOS_NODE_SRCENDIP     "SrcEndIP"
#define QOS_NODE_INTERFACE	  "LanIntf"
#define QOS_NODE_MAXRATE	  "RateMax"
#define QOS_NODE_MINRATE	  "RateMin"
//time mode
#define QOS_NODE_SCHTIMEMODE   "ScheduleName"   //allways ,never
#define QOS_NODE_ENALLDAY       "EnableAllDays"     //1,0
#define QOS_NODE_DAYS           "Days"
#define QOS_NODE_ENALLTIME      "EnableAllTimes"
#define QOS_NODE_STARTTIME      "StartTime"
#define QOS_NODE_ENDTIME        "EndTime"

/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/
#define QOS_PATH_PREFIX_REGEX "^InternetGatewayDevice\\.X_TWSZ-COM_QoS\\."
#define QOS_PATH_QOS_ENABLE   QOS_PATH_PREFIX_REGEX "Enable"
#define QOS_PATH_ENAVERAGE    QOS_PATH_PREFIX_REGEX "EnAverage"
#define QOS_PATH_WANBAND      QOS_PATH_PREFIX_REGEX "WanBand"
#define QOS_PATH_IDLEWANBAND  QOS_PATH_PREFIX_REGEX "OtherIntfsBand"
#define QOS_PATH_IPFLOW_REGEX QOS_PATH_PREFIX_REGEX "IPFlow\\.[0-9]+\\."
#define END "$"

#define QOS_IPFLOW_NODE_REGEX(node) QOS_PATH_IPFLOW_REGEX node END

#define QOS_DEL_IPFLOW_INST_REGEX  QOS_PATH_IPFLOW_REGEX END

#define QOS_APPLY_CHAINS_REGEX QOS_PATH_PREFIX_REGEX \
                                     "("QOS_NODE_ENABLE"|" \
                                     QOS_NODE_ENAVERAGE"|" \
                                     QOS_NODE_IDLEBAND"|" \
                                     QOS_NODE_WANBAND")$"
/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define MAX_INDEX_COUNT     1
#define QOS_MAX_IFNAME_LEN  16
#define QOS_SUBCMD_LEN_1    16
#define QOS_SUBCMD_LEN_2    32
#define QOS_SUBCMD_LEN_3    64
#define MAX_HOURS           24
#define MAX_Minutes         60

/* QOS规则链 */
#define QOS_CHAIN           "QOS_SUBCHAIN"
#define QOS_POST_CHAIN      "QOS_POST_SUBCHAIN"

/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/

/* 生效操作方式 */
typedef enum {
    QOS_APPLY_NON,
    QOS_APPLY_ADD,
    QOS_APPLY_DEL
} EN_QOS_APPLY;



/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/
#endif /* __QOS_H__ */

