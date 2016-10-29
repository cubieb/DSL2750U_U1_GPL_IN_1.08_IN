/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : tbstype.h
 文件描述 : TBS 公共类型定义


 函数列表 :


 修订记录 :
          1 创建 : TBS
            日期 : 2007-8-23
            描述 :
            20080811,heyiming, 增加了WPS进程PID_WPS和模块ID,MID_WPS
**********************************************************************/
#ifndef __TBS_TYPE_H__
#define __TBS_TYPE_H__

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/

/*--------------------常用常量定义----------------------------*/
#ifndef FALSE
    #define FALSE       0
#endif
#ifndef TRUE
    #define TRUE        (!FALSE)
#endif

#ifndef NULL
    #define NULL    ((void *)0)
#endif

#define MAX_IP_LEN    16
#define MAX_BOOL_LEN 12
#define MAX_INT_LEN    12
#define MAX_UINT_LEN 12
#define MAX_NETMASK_LEN    16
#define MAX_ULONG_LEN    12

#define BOOL    unsigned int

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/

/*进程PID----同步s_astPID2PathTbl*/
#define PID_PC      0x01
#define PID_SNMPA   0x02
#define PID_LBT     0x03
#define PID_CCP     0x04
#define PID_WEB     0x05
#define PID_LNB     0x06
#define PID_CLI     0x07
#define PID_TR069FE   0x09
#define PID_TR069_HTTPD   0x0a
#define PID_ELM     0x0b
#define PID_UPG     0x0c
#define PID_VDSLD   0x0d
#define PID_TM      0x0e
#define PID_MON     0x0f
#define PID_UPGCGI  0x10
#define PID_FTPUPG  0x11
#define PID_PTI     0x12
#define PID_MSG4UDP 0x13            /* MSG4UDP通信进程ID, 与wsccmd进程通信 */

#define PID_NULL    0xFF

/*模块MID----同步s_astMID2NameTbl*/
#define FLG_MID_GRP 0xFF

#define MID_CCP         ((PID_CCP << 8) | 0x01)
#define MID_AUTH        ((PID_CCP << 8) | 0x02)     /* 安全认证模块 */
#define MID_IGMP_PROXY  ((PID_CCP << 8) | 0x03)     /* IGMP_Proxy */
#define MID_CMM         ((PID_CCP << 8) | 0x04)     /* CMM */
#define MID_LAN         ((PID_CCP << 8) | 0x05)     /* LAN */
#define MID_IPT         ((PID_CCP << 8) | 0x06)     /* IPT */
#define MID_ETHLAN      ((PID_CCP << 8) | 0x07)     /* LAN端以太网 */
#define MID_ETHWAN      ((PID_CCP << 8) | 0x08)     /* WAN端以太网 */
#define MID_PPPOE       ((PID_CCP << 8) | 0x09)     /* PPPOE */
#define MID_WLAN        ((PID_CCP << 8) | 0x0a)     /* WLAN模块 */
#define MID_TR069BE     ((PID_CCP << 8) | 0x0b)     /* TR069BE */
#define MID_DGN         ((PID_CCP << 8) | 0x0c)     /* Diagnostics */
#define MID_DHCPR       ((PID_CCP << 8) | 0x0d)     /* dhcp relay */
#define MID_DHCPS       ((PID_CCP << 8) | 0x0e)     /* dhcp server */
#define MID_TIMER       ((PID_CCP << 8) | 0x0f)     /* Timer */
#define MID_IPCONN      ((PID_CCP << 8) | 0x10)     /* wan ip connection module */
#define MID_FIREWALL    ((PID_CCP << 8) | 0x11)     /* Fire wall */
#define MID_SNMPC       ((PID_CCP << 8) | 0x12)     /* SNMP配置管理模块 */
#define MID_QOS         ((PID_CCP << 8) | 0x13)     /* QOS模块 */
#define MID_STATIC_ROUTING  ((PID_CCP << 8) | 0x14)     /* static routing */
#define MID_VDSL        ((PID_CCP << 8) | 0x15)     /* VDSL模块 */
#define MID_DNS         ((PID_CCP << 8) | 0x16)     /* DNS模块 */
#define MID_ALG         ((PID_CCP << 8) | 0x17)     /* ALG模块 */
#define MID_WAN         ((PID_CCP << 8) | 0x18)     /* WAN模块 */
#define MID_DYNAMIC_ROUTING  ((PID_CCP << 8) | 0x19)   /* dynamic routing */
#define MID_SNTP        ((PID_CCP << 8) | 0x1a)        /* sntp */
#define MID_VLAN        ((PID_CCP << 8) | 0x1b)        /* vlan */
#define MID_USB_MASS    ((PID_CCP << 8) | 0x1c)    /* USB mass storage module */
#define MID_LOG            ((PID_CCP << 8) | 0x1d)     /* LOGGER 模块 */
#define MID_FTPD        ((PID_CCP << 8) | 0x1e)     /* FTPD module */
#define MID_NATPRIO     ((PID_CCP << 8) | 0x1f)        /* NAT 优先级 */
#define MID_WPS            ((PID_CCP << 8) | 0x20)        /* WPS模块 */
#define MID_ACL         ((PID_CCP << 8) | 0x21)     /* ACL */
#define MID_UPNP        ((PID_CCP << 8) | 0x22)     /* UPNP */
#define MID_LSVLAN       ((PID_CCP << 8) | 0x23)     /* LSVLAN */


#define MID_WEB         ((PID_WEB << 8) | 0x01)     /* WEB模块 */
#define MID_LNB         ((PID_LNB << 8) | 0x01)     /* Load & Backup */
#define MID_CLI         ((PID_CLI << 8) | 0x01)     /* CLI模块 */
#define MID_TR069FE ((PID_TR069FE << 8) | 0x01)     /* TR069FE 模块 */

#define MID_TR069_HTTPD ((PID_TR069_HTTPD << 8) | 0x01)     /*TR069 httpd 模块*/
#define MID_SNMPA    ((PID_SNMPA << 8)  | 0x01)     /* SNMP代理进程 */

#define MID_PC      ((PID_PC << 8)    | 0x01)
#define MID_ELM     ((PID_ELM << 8)   | 0x01)       /* ELM 模块 */
#define MID_UPG     ((PID_UPG << 8)   | 0x01)
#define MID_VDSLD   ((PID_VDSLD << 8) | 0x01)       /* VDSLD 模块 */
#define MID_TM      ((PID_TM << 8)    | 0x01)     /*TM模块*/
#define MID_MON     ((PID_MON << 8)   | 0x01)       /* MON 模块 */
#define MID_LBT     ((PID_LBT << 8)   | 0x01)       /* LBT 模块 */
#define MID_UPGCGI  ((PID_UPGCGI << 8)| 0x01)
#define MID_FTPUPG  ((PID_FTPUPG << 8)| 0x01)       /* FTP 升级模块 */
#define MID_PTI     ((PID_PTI << 8)   | 0X01)
#define MID_MSG4UDP     ((PID_MSG4UDP << 8)   | 0X01)        /* MSG4UDP通信模块 */

#define MID_GRP_CCP ((PID_CCP << 8)   | FLG_MID_GRP)

#define MID_GRP_BASE   0xD0                         /* 多播组基址 */

#define MID_GRP_MACLIST_UPDATE   (((PID_CCP << 8) | MID_GRP_BASE) + 1)
#define MID_GRP_MON_LINK_UPDATE  (((PID_CCP << 8) | MID_GRP_BASE) + 2)
#define MID_GRP_WAN_LINK_UPDATE  (((PID_CCP << 8) | MID_GRP_BASE) + 3)
#define MID_GRP_WAN_CONN_UPDATE  (((PID_CCP << 8) | MID_GRP_BASE) + 4)
#define MID_GRP_DNS_UPDATE       (((PID_CCP << 8) | MID_GRP_BASE) + 5)
#define MID_GRP_WAN_DEL_INST     (((PID_CCP << 8) | MID_GRP_BASE) + 6)
#define MID_GRP_WAN_ADD_INST     (((PID_CCP << 8) | MID_GRP_BASE) + 7)
#define MID_GRP_BUTTON           (((PID_CCP << 8) | MID_GRP_BASE) + 8)
#define MID_GRP_TIME_CHANGED     (((PID_CCP << 8) | MID_GRP_BASE) + 9)
#define MID_GRP_WAN_CONN_ENABLE_UPDATE   (((PID_CCP << 8) | MID_GRP_BASE) + 10)
#define MID_GRP_LAN_DEL_INST     (((PID_CCP << 8) | MID_GRP_BASE) + 11)
#define MID_GRP_LAN_ADD_INST     (((PID_CCP << 8) | MID_GRP_BASE) + 12)

#define MID_GRP_WLAN_DEL_INST    (((PID_CCP << 8) | MID_GRP_BASE) + 13)
#define MID_GRP_WLAN_ADD_INST    (((PID_CCP << 8) | MID_GRP_BASE) + 14)


#define MID_NULL    ((PID_NULL << 8)    | FLG_MID_GRP)

/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/

/*公共消息类型*/
typedef enum tag_EN_MSG_TYPE
{
    MSG_POLL_EXIT       = 0x0001,

    MSG_PC_START_PROC,
    MSG_PC_SEND_SIGNAL,
    MSG_PC_PROC_STATE,
    MSG_PC_PROC_OUTPUT,
    MSG_PC_EXIT,

    MSG_CMM_GET_VAL,             /* 获取消息 */
    MSG_CMM_SET_VAL,             /* 设置消息 */
    MSG_CMM_ADD_NODE, /* 0009 */ /* 添加消息 */
    MSG_CMM_DEL_NODE,            /* 删除消息 */
    MSG_CMM_GET_NAME,            /* 获取参数名称, 返回本节点的下属内容 */
    MSG_CMM_GET_ATTR,
    MSG_CMM_SET_ATTR,

    MSG_CMM_INFORM_NOTI,        /* 报告有通知可以获取 */
    MSG_CMM_GET_NOTI,           /* 获取通知 */
    MSG_CMM_CLEAR_NOTI,         /* 清除通知 */

    MSG_CMM_COMMIT,  
    MSG_CMM_CANCEL,
    MSG_CMM_UPDATE,  /* 0019 */

    MSG_CMM_SAVE_CFG,
    MSG_CMM_RECOVER_CFG,
    MSG_CMM_UPGRADE,
    MSG_CMM_BACKUP,
    MSG_CMM_REBOOT,


    MSG_TIMER_REGSTER,   
    MSG_TIMER_UNREGSTER,
    MSG_TIMER_TIMEOUT,

    MSG_WAN_INTERFACE_STATE,
    MSG_WAN_CONN_EST,  /* 0029 */
    MSG_WAN_CONN_FIN,
    MSG_WAN_LINK_CHG,


    MSG_LAN_IP_UPDATE,
    MSG_LAN_MACLIST_UPDATE,   
    MSG_LAN_DOMAIN_NAME_UPDATE,

    MSG_GET_DHOST_LIST,
    MSG_DNS_CHANGE,

    MSG_WLAN_ADD_DEVICE,
    MSG_WLAN_DEL_DEVICE,


    MSG_TR069_SET_PARKEY,    /* 0039 */ /* 设置ParameterKey 消息 */
    MSG_TR069_GET_EVENTS,               /* 获取Events 消息 */
    MSG_TR069_CLEAR_EVENTS,  			/* 清空Events 消息 */
    MSG_TR069_DOWNLOAD,                 /* 下载消息 */
    MSG_TR069_UPLOADLOAD,               /* 上传消息 */
    MSG_TR069_REBOOT,                   /* 重启消息 */
    MSG_TR069_FACTORYRESET,             /* 恢复出厂消息 */
    MSG_TR069_SCHEDULEINFORM,           /* 定时通知消息 */
    MSG_TR069_GET_TRANSFERCOMPLETEINFO, /* 获取传输结束消息 */

    /* LBT模块消息类型 */
    MSG_LBT_SET_STATE,   /* WLAN 模块射频开关 */
    MSG_LBT_GET,     /* 0049 */   //wlan模块的wps按钮

    MSG_TR069_HTTPD_CONNECT,
    MSG_TR069_HTTPD_CREATE_UPFILE,

    MSG_ETHNET_LINK_STATE,
    MSG_ETHWAN_INTF_NAME,

    MSG_MON_INTF_REGISTER,
    MSG_MON_INTF_UNREGISTER,
    MSG_MON_INTF_STATUS_QUERY,
    MSG_MON_INTF_STATUS_INFORM,  

    MSG_AUTH,                           /*  鉴权消息 */
    MSG_WAN_DEL_INST,      /* 0059 */              /*WAN实例删除消息*/
    MSG_WAN_ADD_INST,                    /*WAN实例增加消息*/
    MSG_LAN_DEL_INST,                    /*LAN实例删除消息*/
    MSG_LAN_ADD_INST,                    /*LAN实例增加消息*/
    MSG_WLAN_DEL_INST,                    /*WLAN实例删除消息*/
    MSG_WLAN_ADD_INST,                    /*WLAN实例增加消息*/

    MSG_CMD,             /* 扩展命令, 表示在消息体内还有命令定义 */

    MSG_RESP,            /* 普通回应消息 */
    MSG_RESP_FRAGMENT,   /* 回应片段(可能回应消息较大, 必须分片回应) */
    MSG_RESP_ERR,        /* 回应消息--返回错误 */

    MSG_CMM_CLEAR_CFG,   /* 0069 */

    MSG_BUTTON,          /* 按钮消息 */

    MSG_SNTP_UPDATE_TIME,
    MSG_SNTP_TIME_CHANGED,
    MSG_NTPS_CHANGED,

    MSG_TR111_DEVICE_ADD,
    MSG_TR111_DEVICE_DEL,

    MSG_PING_COMPLETE,

    MSG_MSG4UDP_REGISTER,             /* 向MSG4UDP注册 */
    MSG_MSG4UDP_UNREGISTER,             /* 向MSG4UDP取消注册 */
    MSG_MSG4UDP_MESSAGE,   /* 0079 */            /* MSG4UDP正常消息 */

    MSG_WAN_CONN_ENABLE_UPDATE,

    MSG_VLAN_ADD_BRIDGE,                        /* 004f */
    MSG_VLAN_DEL_BRIDGE,
    MSG_VLAN_BIND_PORT,
    MSG_VLAN_UNBIND_PORT,

	MSG_VDSL_BRIEF_STATUS_OF_PORT,
	MSG_VDSL_EXTENDED_STATUS_OF_PORT,     
	MSG_VDSL_STATUS,
	MSG_VDSL_SNR_STATUS,    
	MSG_VDSL_BME_FIRMWARE_VERSION,

    MSG_PUB_END      = 0x7FFF

} EN_MSG_TYPE;




#define TBS_MSG_ACK_TYPE(type)  (type | 0x8000)



/*公共消息对应的应答类型*/
#define MSG_POLL_EXIT_ACK                         TBS_MSG_ACK_TYPE(MSG_POLL_EXIT)

#define MSG_PC_START_PROC_ACK                     TBS_MSG_ACK_TYPE(MSG_PC_START_PROC)
#define MSG_PC_SEND_SIGNAL_ACK                    TBS_MSG_ACK_TYPE(MSG_PC_SEND_SIGNAL)
#define MSG_PC_PROC_STATE_ACK                     TBS_MSG_ACK_TYPE(MSG_PC_PROC_STATE)
#define MSG_PC_PROC_OUTPUT_ACK                    TBS_MSG_ACK_TYPE(MSG_PC_PROC_OUTPUT)
#define MSG_PC_EXIT_ACK                           TBS_MSG_ACK_TYPE(MSG_PC_EXIT)

#define MSG_CMM_GET_VAL_ACK                       TBS_MSG_ACK_TYPE(MSG_CMM_GET_VAL)
#define MSG_CMM_SET_VAL_ACK                       TBS_MSG_ACK_TYPE(MSG_CMM_SET_VAL)
#define MSG_CMM_ADD_NODE_ACK                      TBS_MSG_ACK_TYPE(MSG_CMM_ADD_NODE)
#define MSG_CMM_DEL_NODE_ACK                      TBS_MSG_ACK_TYPE(MSG_CMM_DEL_NODE)
#define MSG_CMM_GET_NAME_ACK                      TBS_MSG_ACK_TYPE(MSG_CMM_GET_NAME)
#define MSG_CMM_GET_ATTR_ACK                      TBS_MSG_ACK_TYPE(MSG_CMM_GET_ATTR)
#define MSG_CMM_SET_ATTR_ACK                      TBS_MSG_ACK_TYPE(MSG_CMM_SET_ATTR)
#define MSG_CMM_INFORM_NOTI_ACK                   TBS_MSG_ACK_TYPE(MSG_CMM_INFORM_NOTI)
#define MSG_CMM_GET_NOTI_ACK                      TBS_MSG_ACK_TYPE(MSG_CMM_GET_NOTI)
#define MSG_CMM_CLEAR_NOTI_ACK                    TBS_MSG_ACK_TYPE(MSG_CMM_CLEAR_NOTI)
#define MSG_CMM_COMMIT_ACK                        TBS_MSG_ACK_TYPE(MSG_CMM_COMMIT)
#define MSG_CMM_CANCEL_ACK                        TBS_MSG_ACK_TYPE(MSG_CMM_CANCEL)
#define MSG_CMM_UPDATE_ACK                        TBS_MSG_ACK_TYPE(MSG_CMM_UPDATE)
#define MSG_CMM_SAVE_CFG_ACK                      TBS_MSG_ACK_TYPE(MSG_CMM_SAVE_CFG)
#define MSG_CMM_RECOVER_CFG_ACK                   TBS_MSG_ACK_TYPE(MSG_CMM_RECOVER_CFG)
#define MSG_CMM_UPGRADE_ACK                       TBS_MSG_ACK_TYPE(MSG_CMM_UPGRADE)
#define MSG_CMM_BACKUP_ACK                        TBS_MSG_ACK_TYPE(MSG_CMM_BACKUP)
#define MSG_CMM_REBOOT_ACK                        TBS_MSG_ACK_TYPE(MSG_CMM_REBOOT)

#define MSG_TIMER_REGSTER_ACK                     TBS_MSG_ACK_TYPE(MSG_TIMER_REGSTER)
#define MSG_TIMER_UNREGSTER_ACK                   TBS_MSG_ACK_TYPE(MSG_TIMER_UNREGSTER)
#define MSG_TIMER_TIMEOUT_ACK                     TBS_MSG_ACK_TYPE(MSG_TIMER_TIMEOUT)

#define MSG_WAN_INTERFACE_STATE_ACK               TBS_MSG_ACK_TYPE(MSG_WAN_INTERFACE_STATE)
#define MSG_WAN_CONN_EST_ACK                      TBS_MSG_ACK_TYPE(MSG_WAN_CONN_EST)
#define MSG_WAN_CONN_FIN_ACK                      TBS_MSG_ACK_TYPE(MSG_WAN_CONN_FIN)
#define MSG_WAN_LINK_CHG_ACK                      TBS_MSG_ACK_TYPE(MSG_WAN_LINK_CHG)

#define MSG_LAN_IP_UPDATE_ACK                     TBS_MSG_ACK_TYPE(MSG_LAN_IP_UPDATE)
#define MSG_LAN_MACLIST_UPDATE_ACK                TBS_MSG_ACK_TYPE(MSG_LAN_MACLIST_UPDATE)
#define MSG_LAN_DOMAIN_NAME_UPDATE_ACK            TBS_MSG_ACK_TYPE(MSG_LAN_DOMAIN_NAME_UPDATE)

#define MSG_GET_DHOST_LIST_ACK                    TBS_MSG_ACK_TYPE(MSG_GET_DHOST_LIST)
#define MSG_DNS_CHANGE_ACK                        TBS_MSG_ACK_TYPE(MSG_DNS_CHANGE)
#define MSG_WLAN_ADD_DEVICE_ACK                   TBS_MSG_ACK_TYPE(MSG_WLAN_ADD_DEVICE)
#define MSG_WLAN_DEL_DEVICE_ACK                   TBS_MSG_ACK_TYPE(MSG_WLAN_DEL_DEVICE)

#define MSG_TR069_SET_PARKEY_ACK                  TBS_MSG_ACK_TYPE(MSG_TR069_SET_PARKEY)
#define MSG_TR069_GET_EVENTS_ACK                  TBS_MSG_ACK_TYPE(MSG_TR069_GET_EVENTS)
#define MSG_TR069_CLEAR_EVENTS_ACK                TBS_MSG_ACK_TYPE(MSG_TR069_CLEAR_EVENTS)
#define MSG_TR069_DOWNLOAD_ACK                    TBS_MSG_ACK_TYPE(MSG_TR069_DOWNLOAD)
#define MSG_TR069_UPLOADLOAD_ACK                  TBS_MSG_ACK_TYPE(MSG_TR069_UPLOADLOAD)
#define MSG_TR069_REBOOT_ACK                      TBS_MSG_ACK_TYPE(MSG_TR069_REBOOT)
#define MSG_TR069_FACTORYRESET_ACK                TBS_MSG_ACK_TYPE(MSG_TR069_FACTORYRESET)
#define MSG_TR069_SCHEDULEINFORM_ACK              TBS_MSG_ACK_TYPE(MSG_TR069_SCHEDULEINFORM)
#define MSG_TR069_GET_TRANSFERCOMPLETEINFO_ACK    TBS_MSG_ACK_TYPE(MSG_TR069_GET_TRANSFERCOMPLETEINFO)

#define MSG_LBT_SET_STATE_ACK                     TBS_MSG_ACK_TYPE(MSG_LBT_SET_STATE)
#define MSG_LBT_GET_ACK                           TBS_MSG_ACK_TYPE(MSG_LBT_GET)

#define MSG_TR069_HTTPD_CONNECT_ACK               TBS_MSG_ACK_TYPE(MSG_TR069_HTTPD_CONNECT)
#define MSG_TR069_HTTPD_CREATE_UPFILE_ACK         TBS_MSG_ACK_TYPE(MSG_TR069_HTTPD_CREATE_UPFILE)

#define MSG_ETHNET_LINK_STATE_ACK                 TBS_MSG_ACK_TYPE(MSG_ETHNET_LINK_STATE)
#define MSG_ETHWAN_INTF_NAME_ACK                  TBS_MSG_ACK_TYPE(MSG_ETHWAN_INTF_NAME)

#define MSG_MON_INTF_REGISTER_ACK                 TBS_MSG_ACK_TYPE(MSG_MON_INTF_REGISTER)
#define MSG_MON_INTF_UNREGISTER_ACK               TBS_MSG_ACK_TYPE(MSG_MON_INTF_UNREGISTER)
#define MSG_MON_INTF_STATUS_QUERY_ACK             TBS_MSG_ACK_TYPE(MSG_MON_INTF_STATUS_QUERY)
#define MSG_MON_INTF_STATUS_INFORM_ACK            TBS_MSG_ACK_TYPE(MSG_MON_INTF_STATUS_INFORM)

#define MSG_AUTH_ACK                              TBS_MSG_ACK_TYPE(MSG_AUTH)

#define MSG_WAN_DEL_INST_ACK                      TBS_MSG_ACK_TYPE(MSG_WAN_DEL_INST)
#define MSG_WAN_ADD_INST_ACK                      TBS_MSG_ACK_TYPE(MSG_WAN_ADD_INST)
#define MSG_LAN_DEL_INST_ACK                      TBS_MSG_ACK_TYPE(MSG_LAN_DEL_INST)
#define MSG_LAN_ADD_INST_ACK                      TBS_MSG_ACK_TYPE(MSG_LAN_ADD_INST)
#define MSG_WLAN_DEL_INST_ACK                     TBS_MSG_ACK_TYPE(MSG_WLAN_DEL_INST)
#define MSG_WLAN_ADD_INST_ACK                     TBS_MSG_ACK_TYPE(MSG_WLAN_ADD_INST)

#define MSG_CMD_ACK                               TBS_MSG_ACK_TYPE(MSG_CMD)
#define MSG_RESP_ACK                              TBS_MSG_ACK_TYPE(MSG_RESP)
#define MSG_RESP_FRAGMENT_ACK                     TBS_MSG_ACK_TYPE(MSG_RESP_FRAGMENT)
#define MSG_RESP_ERR_ACK                          TBS_MSG_ACK_TYPE(MSG_RESP_ERR)

#define MSG_CMM_CLEAR_CFG_ACK                     TBS_MSG_ACK_TYPE(MSG_CMM_CLEAR_CFG)

#define MSG_BUTTON_ACK                            TBS_MSG_ACK_TYPE(MSG_BUTTON)

#define MSG_SNTP_UPDATE_TIME_ACK                  TBS_MSG_ACK_TYPE(MSG_SNTP_UPDATE_TIME)
#define MSG_SNTP_TIME_CHANGED_ACK                 TBS_MSG_ACK_TYPE(MSG_SNTP_TIME_CHANGED)
#define MSG_NTPS_CHANGED_ACK                      TBS_MSG_ACK_TYPE(MSG_NTPS_CHANGED)

#define MSG_TR111_DEVICE_ADD_ACK                  TBS_MSG_ACK_TYPE(MSG_TR111_DEVICE_ADD)
#define MSG_TR111_DEVICE_DEL_ACK                  TBS_MSG_ACK_TYPE(MSG_TR111_DEVICE_DEL)

#define MSG_PING_COMPLETE_ACK                     TBS_MSG_ACK_TYPE(MSG_PING_COMPLETE)

#define MSG_MSG4UDP_REGISTER_ACK                  TBS_MSG_ACK_TYPE(MSG_MSG4UDP_REGISTER)
#define MSG_MSG4UDP_UNREGISTER_ACK                TBS_MSG_ACK_TYPE(MSG_MSG4UDP_UNREGISTER)
#define MSG_MSG4UDP_MESSAGE_ACK                   TBS_MSG_ACK_TYPE(MSG_MSG4UDP_MESSAGE)

#define MSG_WAN_CONN_ENABLE_UPDATE_ACK            TBS_MSG_ACK_TYPE(MSG_WAN_CONN_ENABLE_UPDATE)

#define MSG_VLAN_ADD_BRIDGE_ACK                   TBS_MSG_ACK_TYPE(MSG_VLAN_ADD_BRIDGE)
#define MSG_VLAN_DEL_BRIDGE_ACK                   TBS_MSG_ACK_TYPE(MSG_VLAN_DEL_BRIDGE)
#define MSG_VLAN_BIND_PORT_ACK                    TBS_MSG_ACK_TYPE(MSG_VLAN_BIND_PORT)
#define MSG_VLAN_UNBIND_PORT_ACK                  TBS_MSG_ACK_TYPE(MSG_VLAN_UNBIND_PORT)


#define MSG_VDSL_BRIEF_STATUS_OF_PORT_ACK		  TBS_MSG_ACK_TYPE(MSG_VDSL_BRIEF_STATUS_OF_PORT)
#define MSG_VDSL_EXTENDED_STATUS_OF_PORT_ACK	  TBS_MSG_ACK_TYPE(MSG_VDSL_EXTENDED_STATUS_OF_PORT)
#define MSG_VDSL_STATUS_ACK						  TBS_MSG_ACK_TYPE(MSG_VDSL_STATUS) 
#define MSG_VDSL_SNR_STATUS_ACK					  TBS_MSG_ACK_TYPE(MSG_VDSL_SNR_STATUS)
#define MSG_VDSL_BME_FIRMWARE_VERSION_ACK		  TBS_MSG_ACK_TYPE(MSG_VDSL_BME_FIRMWARE_VERSION)



/******************************************************************************
 *                                 TYPEDEF                                    *
 ******************************************************************************/


/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/


/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/

/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/

#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif

#endif /*__TBS_TYPE_H__*/
