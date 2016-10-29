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
#define FOR_EACH_LIST(pos, list)        for ((pos) = list; (pos) != NULL; (pos) = (pos)->pstNext)

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
#define QOS_PATH_PREFIX     "InternetGatewayDevice.QueueManagement."
#define QOS_PATH_IF         QOS_PATH_PREFIX "X_TWSZ-COM_Interface."
#define QOS_PATH_QUEUE      QOS_PATH_PREFIX "Queue."
#define QOS_PATH_CLASS      QOS_PATH_PREFIX "Classification."
#define QOS_PATH_APP        QOS_PATH_PREFIX "App."
#define QOS_PATH_QUEUESTAT  QOS_PATH_PREFIX "QueueStats."

#define QOS_NODE_ENABLE       "Enable"
#define QOS_NODE_QUEUE_ENABLE "QueueEnable"
#define QOS_NODE_QUEUE_STATUS "QueueStatus"
#define QOS_NODE_APP_ENABLE   "AppEnable"
#define QOS_NODE_MAXIF        "MaxInterfaceEntries"
#define QOS_NODE_MAXQUEUE     "MaxQueueEntries"
#define QOS_NODE_MAXCLASS     "MaxClassificationEntries"
#define QOS_NODE_NUMOFIF      "InterfaceNumberOfEntries"
#define QOS_NODE_NUMOFQUEUE   "QueueNumberOfEntries"
#define QOS_NODE_NUMOFCLASS   "ClassificationNumberOfEntries"
#define QOS_NODE_NUMOFUPCLS   "X_TWSZ-COM_UpClsNumOfEntries"
#define QOS_NODE_NUMOFDOWNCLS "X_TWSZ-COM_DownClsNumOfEntries"
#define QOS_NODE_CFG_TRIGGER  "ConfigTrigger"
#define QOS_NODE_SYNSTATUS    "SynchronizedStatus"

#define QOS_NODE_IFPATH      "InterfacePath"
#define QOS_NODE_IFNAME      "InterfaceName"
#define QOS_NODE_COMMRATE    "ShapingRate"
#define QOS_NODE_PEAKRATE    "X_TWSZ-COM_PeakRate"
#define QOS_NODE_BURSTSIZE   "ShapingBurstSize"
#define QOS_NODE_SCHEDTYPE   "SchedulerType"
#define QOS_NODE_SCHED_ALGO  "SchedulerAlgorithm"
#define QOS_NODE_ENDSCPMARK  "X_TWSZ-COM_EnableDSCPMark"
#define QOS_NODE_ENCOSMARK   "X_TWSZ-COM_EnableCOSMark"
#define QOS_NODE_IFSTATE     "InterfaceState"

#define QOS_NODE_QUEUEIF       "QueueInterface"
#define QOS_NODE_QUEUELEN      "QueueBufferLength"
#define QOS_NODE_QUELENTYPE    "X_TWSZ-COM_QueueLengthType"
#define QOS_NODE_QUEWEIGHT     "QueueWeight"
#define QOS_NODE_ENFORCEWEIGHT "X_TWSZ-COM_EnableForceWeight"
#define QOS_NODE_QUEUEPRE      "QueuePrecedence"
#define QOS_NODE_QUEUECAR      "X_TWSZ-COM_QueueCAR"
#define QOS_NODE_QUEDROPALG    "DropAlgorithm"

#define QOS_NODE_CLSENABLE   "ClassificationEnable"
#define QOS_NODE_CLSTYPE     "ClassificationType"
#define QOS_NODE_DIRECTION   "Direction"
#define QOS_NODE_IPPROTOTYPE "X_TWSZ-COM_IpProtocolType"
#define QOS_NODE_CLASSIF     "ClassInterface"
#define QOS_NODE_CLSEGRESSIF "EgressInterface"
#define QOS_NODE_SRCMAC      "SourceMACAddress"
#define QOS_NODE_SRCMACMASK  "SourceMACMask"
#define QOS_NODE_DSTMAC      "DestMACAddress"
#define QOS_NODE_DSTMACMASK  "DestMACMask"
#define QOS_NODE_ETHTYPE     "Ethertype"
#define QOS_NODE_ETHPRIO     "EthernetPriorityCheck"
#define QOS_NODE_ETHPMARK    "EthernetPriorityMark"
#define QOS_NODE_VLANID      "VLANIDCheck"
#define QOS_NODE_SRCIP       "SourceIP"
#define QOS_NODE_SRCIPMAX    "SourceIPMax"
#define QOS_NODE_SRCMASK     "SourceMask"
#define QOS_NODE_SRCPRELEN   "SourcePrefixLength"
#define QOS_NODE_DSTIP       "DestIP"
#define QOS_NODE_DSTIPMAX    "DestIPMax"
#define QOS_NODE_DSTMASK     "DestMask"
#define QOS_NODE_DSTPRELEN   "DestPrefixLength"
#define QOS_NODE_IPLENMIN    "IPLengthMin"
#define QOS_NODE_IPLENMAX    "IPLengthMax"
#define QOS_NODE_TOS         "TOSCheck"
#define QOS_NODE_FLOWLABEL   "FlowLabelCheck"
#define QOS_NODE_FLOWLABELMARK "FlowLabelMark"
#define QOS_NODE_DSCP        "DSCPCheck"
#define QOS_NODE_DSCPMARK    "DSCPMark"
#define QOS_NODE_TCCHECK     "TrafficClassCheck"
#define QOS_NODE_TCMARK      "TrafficClassMark"

#define QOS_NODE_PROTOCOL   "Protocol"
#define QOS_NODE_SRCPORT    "SourcePort"
#define QOS_NODE_SRCPORTMAX "SourcePortRangeMax"
#define QOS_NODE_DSTPORT    "DestPort"
#define QOS_NODE_DSTPORTMAX "DestPortRangeMax"
#define QOS_NODE_CLASSQUE   "ClassQueue"

#define QOS_NODE_CLSRET_STATE  "X_TWSZ-COM_ClassResultState"
#define QOS_NODE_NFMARK        "X_TWSZ-COM_NfMark"

#define QOS_NODE_APPNAME       "AppName"
#define QOS_NODE_APPQUE        "AppDefaultQueue"
#define QOS_NODE_COSMARK       "X_TWSZ-COM_COSMark"
#define QOS_NODE_APPRET_STATE  "X_TWSZ-COM_AppResultState"
#define QOS_NODE_APPRULE_STATE "X_TWSZ-COM_AppRuleState"

#define QOS_DEVICE_NAME       "X_TWSZ-COM_DeviceName"
#define WAN_CONNED_DEV_NAME   "X_TWSZ-COM_ConnectedDeviceName"
#define WAN_CONN_STATUS       "ConnectionStatus"
#define WAN_CONN_MODE         "ConnectionType"

#define WAN_DEV_PREFIX      "InternetGatewayDevice.WANDevice."
#define KEY_WAN_COMM_IF_CONFIG  "WANCommonInterfaceConfig"
#define KEY_WAN_CONN_DEV    "WANConnectionDevice"
#define KEY_WAN_IP_CONN     "WANIPConnection"
#define KEY_WAN_PPP_CONN    "WANPPPConnection"


//QueueStats叶子节点名称
//(Enable|Status|Queue|Interface|OutputPackets|OutputBytes|DroppedPackets|DroppedBytes|QueueOccupancyPackets|QueueOccupancyPercentage)
#ifndef QOS_NODE_ENABLE
#define QOS_NODE_ENABLE                         "Enable"
#endif
#ifndef QOS_NODE_STATUS
#define QOS_NODE_STATUS                         "Status"
#endif
#ifndef QOS_NODE_QUEUE
#define QOS_NODE_QUEUE                          "Queue"
#endif
#ifndef QOS_NODE_INTERFACE
#define QOS_NODE_INTERFACE                      "Interface"
#endif
#ifndef QOS_NODE_OUTPUTPACKETS
#define QOS_NODE_OUTPUTPACKETS                  "OutputPackets"
#endif
#ifndef QOS_NODE_OUTPUTBYTES
#define QOS_NODE_OUTPUTBYTES                    "OutputBytes"
#endif
#ifndef QOS_NODE_DROPPEDPACKETS
#define QOS_NODE_DROPPEDPACKETS                 "DroppedPackets"
#endif
#ifndef QOS_NODE_DROPPEDBYTES
#define QOS_NODE_DROPPEDBYTES                   "DroppedBytes"
#endif
#ifndef QOS_NODE_QUEUEOCCUPANCYPACKETS
#define QOS_NODE_QUEUEOCCUPANCYPACKETS          "QueueOccupancyPackets"
#endif
#ifndef QOS_NODE_QUEUEOCCUPANCYPERCENTAGE
#define QOS_NODE_QUEUEOCCUPANCYPERCENTAGE       "QueueOccupancyPercentage"
#endif


#define IP_CONN_INST_PATH     WAN_DEV_PREFIX "{i}." KEY_WAN_CONN_DEV ".{i}." KEY_WAN_IP_CONN ".{i}."
#define PPP_CONN_INST_PATH    WAN_DEV_PREFIX "{i}." KEY_WAN_CONN_DEV ".{i}." KEY_WAN_PPP_CONN ".{i}."

#define ETHLAN_IF_INST_PATH   "InternetGatewayDevice.LANDevice.{i}.LANEthernetInterfaceConfig.{i}."
#define WLAN_IF_INST_PATH     "InternetGatewayDevice.LANDevice.{i}.WLANConfiguration.{i}."

/*****************************************************************
 *                     节点路径匹配正则表达式                    *
 *****************************************************************/
#define QOS_PATH_PREFIX_REGEX "^InternetGatewayDevice\\.QueueManagement\\."
#define QOS_PATH_QOS_ENABLE   QOS_PATH_PREFIX_REGEX "Enable"
#define QOS_PATH_IF_REGEX     QOS_PATH_PREFIX_REGEX "Interface\\.[0-9]+\\."
#define QOS_PATH_HQUE_REGEX   QOS_PATH_PREFIX_REGEX "Queue\\.[0-2]\\."
/* 正则表达式优先级问题导致Classification下实例号超过10匹配出问题 */
#define QOS_PATH_UQUE_REGEX    QOS_PATH_PREFIX_REGEX "Queue\\.([3-9]|[1-9][0-9]+)\\."
#define QOS_PATH_QUE_REGEX    QOS_PATH_PREFIX_REGEX "Queue\\.[0-9]+\\."
#define QOS_PATH_CLASS_REGEX  QOS_PATH_PREFIX_REGEX "Classification\\.[0-9]+\\."
#define QOS_PATH_APP_REGEX    QOS_PATH_PREFIX_REGEX "App\\.[0-9]+\\."
#define QOS_PATH_QUE_STAT_REGEX QOS_PATH_PREFIX_REGEX "QueueStats\\.[0-9]+\\."
#define END "$"

#define QOS_IF_NODE_REGEX(node)  QOS_PATH_IF_REGEX node END
#define QOS_QUE_NODE_REGEX(node) QOS_PATH_QUE_REGEX node END
#define QOS_CLS_NODE_REGEX(node) QOS_PATH_CLASS_REGEX node END
#define QOS_APP_NODE_REGEX(node) QOS_PATH_APP_REGEX node END
#define QOS_QUE_STAT_NODE_REGEX(node) QOS_PATH_QUE_STAT_REGEX node END

#define QOS_DEL_INST_REGEX        QOS_PATH_PREFIX_REGEX "(Queue|Classification|App)\\.[0-9]+\\.$"
#define QOS_ADD_INST_REGEX        QOS_PATH_PREFIX_REGEX "App\\.$"

#define QOS_UPDATE_QUE_STAT_REGEX(node)    QOS_QUE_STAT_NODE_REGEX(node)
#define QOS_FORMAT_QUE_INST         QOS_PATH_QUEUE "%u."


/***************WAN接口路径匹配表达式****************/
#define WAN_COMM_IF_PATH_REGEX "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANCommonInterfaceConfig$"
#define WAN_CONN_DEV_PATH_REGEX "^InternetGatewayDevice\\.WANDevice\\.[0-9]+\\.WANConnectionDevice\\.[0-9]+"
#define WAN_CONN_PATH_REGEX WAN_CONN_DEV_PATH_REGEX "\\.(WANIPConnection|WANPPPConnection)\\.[0-9]+"
#define WAN_CONN_NODE_REGEX WAN_CONN_PATH_REGEX "\\.(Enable|ConnectionType)"

/* 获取DSL上行速率的节点 */
#define QOS_DSL_RATE "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig."
#define QOS_DSL_UPSTREAM_RATE QOS_DSL_RATE "UpstreamCurrRate"
#define QOS_DSL_DOWNSTREAM_RATE QOS_DSL_RATE "DownstreamCurrRate"

/***************LAN接口路径匹配表达式**************/
#define ETHLAN_IF_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.LANEthernetInterfaceConfig\\.[0-9]+$"
#define WLAN_IF_PATH_REGEX "^InternetGatewayDevice\\.LANDevice\\.[0-9]+\\.WLANConfiguration\\.[0-9]+$"

/*****************************************************************
 *                       打印模式字符串                          *
 *****************************************************************/
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

#define ETHLAN_IF_INST_PATH_FMT       "InternetGatewayDevice.LANDevice.%lu.LANEthernetInterfaceConfig.%lu."
#define WLAN_IF_INST_PATH_FMT         "InternetGatewayDevice.LANDevice.%lu.WLANConfiguration.%lu."
#define WAN_CONN_INST_PATH_FMT_SSCANF WAN_DEV_PREFIX "%lu." KEY_WAN_CONN_DEV ".%lu.%*[^.].%lu"

/*****************************************************************
 *                       数据操作相关的宏                        *
 *****************************************************************/
/*Mbps --> Kbps, ratio=1024*/
#define SPEED_M_TO_K(Speed)  ((unsigned long)(Speed)<<10)
#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define BASIC_NFMARK_UP     0x0UL
#define BASIC_NFMARK_DOWN   0x100000UL

#define GET_L2_NFMARK_UP(ClsIdx) ((ClsIdx) + BASIC_NFMARK_UP + 0x100)
#define GET_L3_NFMARK_UP(ClsIdx) ((ClsIdx) + BASIC_NFMARK_UP + 0x10000)
#define GET_FW_NFMARK_UP(ClsIdx) ((ClsIdx) + BASIC_NFMARK_UP + 0x20000)

#define GET_APP_NFMARK(AppIdx)   ((AppIdx) + BASIC_NFMARK_UP + 0x30000) /*APP也可算做上行*/

#define GET_L2_NFMARK_DOWN(ClsIdx) ((ClsIdx) + BASIC_NFMARK_DOWN + 0x100)
#define GET_L3_NFMARK_DOWN(ClsIdx) ((ClsIdx) + BASIC_NFMARK_DOWN + 0x10000)
#define GET_POST_NFMARK_DOWN(ClsIdx) ((ClsIdx) + BASIC_NFMARK_DOWN + 0x20000)

#define GET_CLASSIFY_VAL(CosMark)   ((CosMark) + 20)

/*****************************************************************
 *                             Others                            *
 *****************************************************************/
#define QOS_SUBCMD_LEN_1    16
#define QOS_SUBCMD_LEN_2    32
#define QOS_SUBCMD_LEN_3    64

#define WAN_CONN_IP_RT_MODE     "IP_Routed"
#define WAN_CONN_IP_BR_MODE     "IP_Bridged"
#define WAN_CONN_PPP_BR_MODE    "PPPoE_Bridged"
#define WAN_CONN_PPP_BR_RT_MODE "Bridged_Routed"
#define WAN_CONN_PPPOU_MODE     "PPPoU"

#define QOS_SCHED_ALGO_DWRR      "WRR"
#define QOS_SCHED_ALGO_CAR       "CAR"
#define QOS_SCHED_ALGO_SP        "SP"


#define LINK_SPEED_10    10
#define LINK_SPEED_20    20
#define LINK_SPPED_100   100
#define LINK_SPEED_1000  1000
#define SPEED_RATIO      1024
#define ETHER_MTU        1500

/*上下行默认速率 */
#define QOS_DFLT_DSL_UPLINK_RATE 1
#define QOS_DFLT_DSL_DOWNLINK_RATE 20

#define QOS_DFLT_OTHER_UPLINK_RATE 100
#define QOS_DFLT_OTHER_DOWNLINK_RATE 100

/*SP,DWRR,CAR调度方式下的队列数目*/
#define QOS_PRIO_QUE_NUM   4
#define QOS_WEIGHT_QUE_NUM 4
#define QOS_CAR_QUE_NUM    6
#define QOS_QUEUE_NUM      6 /*上行或下行的队列数目*/

/*上行和下行分类实例的最大数量*/
#define MAX_UP_CLS_ENTRIES    8
#define MAX_DOWN_CLS_ENTRIES  8

#define MAX_WLAN_INST_ENTRIES 8

/*初始实例数量*/
#define INITIAL_CLS_ENTRIES   0
#define INITIAL_APP_ENTRIES   0
#define INITIAL_QUE_ENTRIES   4

/*Classification match MASK*/
#define QOS_CLS_L2_RULE_MASK      0x00000001
#define QOS_CLS_L3_RULE_MASK      0x00000010
#define QOS_CLS_FORWARD_RULE_MASK 0x00000100
#define QOS_CLS_POST_RULE_MASK    0x00001000

/*Classification result MASK*/
#define QOS_CLS_BIND_QUE_MASK  0x00000001
#define QOS_CLS_DSCP_MARK_MASK 0x00000010
#define QOS_CLS_COS_MARK_MASK  0x00000100

/*App result MASK*/
#define QOS_APP_BIND_QUE_MASK   0x00000001
#define QOS_APP_DSCP_MARK_MASK  0x00000010
#define QOS_APP_COS_MARK_MASK   0x00000100

/*IPTOS value*/
#define	IPTOS_LOWDELAY		0x10
#define	IPTOS_THROUGHPUT	0x08
#define	IPTOS_RELIABILITY	0x04
#define	IPTOS_MINCOST		0x02
#define IPTOS_NORMALSVC     0

/* ebtables规则链 */
#define QOS_EBT_PROUTE         "QOS_PROUTE"          /*下行*/
#define QOS_EBT_BROUTE         "QOS_BROUTE"          /*上行*/
#define QOS_EBT_FORWARD        "QOS_FORWARD_MATCH"   /*上行*/
#define QOS_EBT_POST_MATCH     "QOS_POST_MATCH"      /*下行*/
#define QOS_EBT_POST_MARK      "QOS_POST_MARK"       /*下行/上行*/

/* ip6tables规则链 */
#define QOS_IP6T_PRE_MATCH     "QOS_PRE_MATCH"
#define QOS_IP6T_PRE_MATCH_UP  "QOS_PRE_MATCH_UP"
#define QOS_IP6T_FORWARD_MATCH "QOS_FORWARD_MATCH"
#define QOS_IP6T_FORWARD_IMQ   "QOS_FORWARD_IMQ"
#define QOS_IP6T_OUT_MATCH     "QOS_OUTPUT_MATCH"
#define QOS_IP6T_POST_MARK     "QOS_POSTROUTING_MARK"

#define QOS_ENABLED  "Enabled"
#define QOS_DISABLED "Disabled"

#define QOS_STATUS_Disabled     "Disabled" 
#define QOS_STATUS_Enabled      "Enabled"
#define QOS_STATUS_Error        "Error"
#define MAX_DEC_LENGTH          16

#define QOS_QUEUE_STATUS_Disabled   QOS_STATUS_Disabled
#define QOS_QUEUE_STATUS_Enabled    QOS_STATUS_Enabled
#define QOS_QUEUE_STATUS_Error      QOS_STATUS_Error
#define QOS_UPSTREAM_IF             "imq1"
#define QOS_DOWNSTREAM_IF           "imq0"
#define QOS_QDISC_ROOT_STR          " 1:1 "


/* 3G接口名字目前固定为ppp10, 如果变动则需要修改
    add by zhoumingming 20120418*/
#define SPECIAL_INTERFACE_FOR_3G "ppp10"
/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/
/* 实例类型定义 */
enum {
    QOS_IF = 1,
    QOS_QUEUE,
    QOS_CLASS,
    QOS_APP
};

enum {
    QOS_SCH_PRIO = 1,
    QOS_SCH_WEIGHT,
    QOS_SCH_CAR
};

/* 生效操作方式 */
typedef enum {
    QOS_APPLY_NONE,
    QOS_APPLY_ADD,
    QOS_APPLY_DEL
} EN_QOS_APPLY;

/*分类配置方式*/
enum {
    QOS_CLS_BIND_QUEUE = 1, //分类匹配后绑定队列
    QOS_CLS_DSCP_MARK,      //分类匹配后进行DSCP标记
    QOS_CLS_COS_MARK,       //分类匹配后进行COS(802.1P)标记
    QOS_CLS_WMM_MARK,       //分类匹配后进行WMM标记(DSCP标记)

    QOS_APP_BIND_QUEUE,     //将内部业务流绑定到队列
    QOS_APP_DSCP_MARK,      //对内部业务流进行DSCP标记
    QOS_APP_COS_MARK        //对内部业务流进行COS标记
};

/*业务类型*/
enum {
    QOS_APP_NONE,
    QOS_APP_TR069,
    QOS_APP_VOIP
};

enum {
  QOS_CLS_UPSTREAM = 1,
  QOS_CLS_DOWNSTREAM
};

typedef enum {
    UNKNOWN_CONN_TYPE,
    WAN_IP_CONN,
    WAN_PPP_CONN
} EN_WAN_CONN_TYPE;

typedef enum {
    UNKNOWN_CONN_MODE,
    IP_ROUTED,
    IP_BRIDGED,
    PPPOE_BRIDGED,
    BRIDGED_ROUTED
} EN_WAN_CONN_MODE;


/******************************************************************************
 *                                  STRUCT                                    *
 ******************************************************************************/
typedef struct tag_ST_QOS_WANCONN_INST {
    struct tag_ST_QOS_WANCONN_INST *pstNext;
    char szIfName[MAX_IF_NAME_LEN];  /*路由桥混合模式时存储桥接口名*/
    BOOL bWanConnState;
    EN_WAN_CONN_TYPE enWanConnType;
    EN_WAN_CONN_MODE enWanConnMode;
    struct {
        unsigned long ulWanDevIdx;
        unsigned long ulWanConnDevIdx;
        unsigned long ulWanConnIdx;
    }stIndex;
    char szData[0];  /*对于PPP连接会多分配16字节的空间用于路由桥混合模式时存储路由PPP接口名*/
}ST_QOS_WANCONN_INST;

/******************************************************************************
 *                                      END                                   *
 ******************************************************************************/
#endif /* __QOS_H__ */

