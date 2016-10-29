/******************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: logger.h
 文件描述:
           LOGGER 模块
 修订记录:
        1. 作者: WeiChongxu@twsz.com
           日期:
           内容: 创建

        2. 作者: HuangJidong@twsz.com
           日期: 20081219
           内容: 重构

******************************************************************/

#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>
#include "common.h"
#include "tbsmsg.h"
#include "tbserror.h"
#include "cfg_api.h"
#include "pc_api.h"
#include "tbsutil.h"
#include "warnlog.h"
#include "tbstype.h"
#include "tbsflash.h"
#include "new_msg.h"
#include "common_func.h"
#include "flash_layout.h"
#include "common_msg_handler.h"

/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/


/* 日志函数定义 */
#ifdef LOGGER_DEBUG
    #define LOGGER_TRACE_PRINTF(fmt, args...)       printf("[LOGGER]:"fmt,##args)
    #define LOGGER_TRACE(fmt, args...)              TBS_TRACE(MID_LOG, fmt, ##args)
#else
    #define LOGGER_TRACE_PRINTF(fmt, args...)       {do { ; } while(0);}
    #define LOGGER_TRACE(fmt, args...)              {do { ; } while(0);}
#endif

#define LEVEL_1_SPACE " "
#define LEVEL_5_SPACE "     "


/* 设备信息节点路径定义 */
#define LOGGER_PATH_DEVICEINFO              "InternetGatewayDevice.DeviceInfo."

#define LOGGER_DEV_INFO_KEY_MANUFACTURER    "Manufacturer"
#define LOGGER_DEV_INFO_KEY_PRODUCTCLASS    "ProductClass"
#define LOGGER_DEV_INFO_KEY_SERIALNUMBER    "SerialNumber"
#define LOGGER_DEV_INFO_KEY_IP              "IP"
#define LOGGER_DEV_INFO_KEY_HWVER           "HWVer"
#define LOGGER_DEV_INFO_KEY_SWVER           "SWVer"

#define LOGGER_PATH_MANUFACTURER        LOGGER_PATH_DEVICEINFO LOGGER_DEV_INFO_KEY_MANUFACTURER
#define LOGGER_PATH_PRODUCTCLASS        LOGGER_PATH_DEVICEINFO LOGGER_DEV_INFO_KEY_PRODUCTCLASS
#define LOGGER_PATH_SERIALNUMBER        LOGGER_PATH_DEVICEINFO LOGGER_DEV_INFO_KEY_SERIALNUMBER
#define LOGGER_PATH_DEFAULTCONNECTIONSERVICE    "InternetGatewayDevice.X_TWSZ-COM_DEFAULT_GW"
#define LOGGER_NODE_EXTERNALIP          "ExternalIPAddress"
#define LOGGER_PATH_HARDWAREVERSION     LOGGER_PATH_DEVICEINFO "HardwareVersion"
#define LOGGER_PATH_SOFTWAREVERSION     LOGGER_PATH_DEVICEINFO "SoftwareVersion"
#define LOGGER_PATH_IP                         "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress"

/* 节点路径定义 */
#define LOGGER_PATH         "InternetGatewayDevice.X_TWSZ-COM_Logger."
#define LOGGER_PATH_LEVEL   LOGGER_PATH "LogLevel."
#define BASE_LOGGER_PATH    "^InternetGatewayDevice\\.X_TWSZ-COM_Logger\\."

/* 节点类型定义 */
#define LOGGER_INST_BASE    0
#define LOGGER_INST_LEVEL   1

#define LOGGER_NODE_ENABLED         "LoggerEnabled"
#define LOGGER_NODE_TFTPSERVER      "LogTFTPServer"
#define LOGGER_NODE_UPLOADTRG       "LogUploadTrigger"
#define LOGGER_NODE_CLEARTRG        "LogClearTrigger"

#define LOGGER_NODE_DEBUGENABLED    "DebugLogEnabled"
#define LOGGER_NODE_DEBUGLOGSIZE    "DebugLogFileSize"
#define LOGGER_NODE_RMTENABLED      "RemoteLogEnabled"
#define LOGGER_NODE_RMTLOGGER       "RemoteLogger"
#define LOGGER_NODE_RMTPORT         "RemotePort"

/* LogLevel 节点 */
#define LOGGER_NODE_DEFAULTLEVEL            "DefaultLogLevel"
#define LOGGER_NODE_CCPLEVEL                "CcpLogLevel"
#define LOGGER_NODE_AUTHLEVEL               "AuthLogLevel"
#define LOGGER_NODE_IGMPLEVEL               "Igmp_proxyLogLevel"
#define LOGGER_NODE_CMMLEVEL                "CmmLogLevel"
#define LOGGER_NODE_LANLEVEL                "LanLogLevel"
#define LOGGER_NODE_IPTLEVEL                "IptLogLevel"
#define LOGGER_NODE_ETHLANLEVEL             "EthlanLogLevel"
#define LOGGER_NODE_ETHWANLEVEL             "EthwanLogLevel"
#define LOGGER_NODE_PPPOELEVEL              "PppoeLogLevel"
#define LOGGER_NODE_WLANLEVEL               "WlanLogLevel"
#define LOGGER_NODE_TR069BELEVEL            "Tr069beLogLevel"
#define LOGGER_NODE_DGNLEVEL                "DgnLogLevel"
#define LOGGER_NODE_DHCPRLEVEL              "DhcprLogLevel"
#define LOGGER_NODE_DHCPSLEVEL              "DhcpsLogLevel"
#define LOGGER_NODE_TIMERLEVEL              "TimerLogLevel"
#define LOGGER_NODE_IPCONNLEVEL             "IpconnLogLevel"
#define LOGGER_NODE_FIREWALLLEVEL           "FirewallLogLevel"
#define LOGGER_NODE_SNMPCLEVEL              "SnmpcLogLevel"
#define LOGGER_NODE_QOSLEVEL                "QosLogLevel"
#define LOGGER_NODE_STATIC_ROUTINGLEVEL     "Static_routingLogLevel"
#define LOGGER_NODE_VDSLLEVEL               "VdslLogLevel"
#define LOGGER_NODE_DNSLEVEL                "DnsLogLevel"
#define LOGGER_NODE_ALGLEVEL                "AlgLogLevel"
#define LOGGER_NODE_WANLEVEL                "WanLogLevel"
#define LOGGER_NODE_DROUTELEVEL    "Dynamic_routingLogLevel"
#define LOGGER_NODE_SNTPLEVEL               "SntpLogLevel"
#define LOGGER_NODE_VLANLEVEL               "VlanLogLevel"
#define LOGGER_NODE_USB_MASSLEVEL           "Usb_massLogLevel"
#define LOGGER_NODE_LOGLEVEL                "LogLogLevel"
#define LOGGER_NODE_FTPDLEVEL               "FtpdLogLevel"      /* FTPD module */
#define LOGGER_NODE_NATPRIOLEVEL            "NatPrioLogLevel"   /* NAT 优先级 */
#define LOGGER_NODE_WPSLEVEL                "WpsLogLevel"       /* WPS module */
#define LOGGER_NODE_ACLLEVEL                "AclLogLevel"       /* ACL module */
#define LOGGER_NODE_UPNPLEVEL               "UpnpLogLevel"
#define LOGGER_NODE_LSVLANLEVEL             "LsvlanLogLevel"
#define LOGGER_NODE_PORTOFFLEVEL            "PortoffLogLevel"
#define LOGGER_NODE_URLFILTERLEVEL          "UrlfilterLogLevel"
#define LOGGER_NODE_TFTPUPGLEVEL            "TftpupgLogLevel"
#define LOGGER_NODE_DEVINFOLEVEL            "DevinfoLogLevel"

#define LOGGER_NODE_WEBLEVEL                "WebLogLevel"
#define LOGGER_NODE_LNBLEVEL                "LnbLogLevel"
#define LOGGER_NODE_CLILEVEL                "CliLogLevel"
#define LOGGER_NODE_TR069FELEVEL            "Tr069feLogLevel"

#define LOGGER_NODE_TR069_HTTPDLEVEL        "Tr069_httpdLogLevel"
#define LOGGER_NODE_SNMPALEVEL              "SnmpaLogLevel"

#define LOGGER_NODE_PCLEVEL                 "PcLogLevel"
#define LOGGER_NODE_ELMLEVEL                "ElmLogLevel"
#define LOGGER_NODE_UPGLEVEL                "UpgLogLevel"
#define LOGGER_NODE_VDSLDLEVEL              "VdsldLogLevel"
#define LOGGER_NODE_TMLEVEL                 "TmLogLevel"
#define LOGGER_NODE_MONLEVEL                "MonLogLevel"
#define LOGGER_NODE_LBTLEVEL                "LbtLogLevel"
#define LOGGER_NODE_UPGCGILEVEL             "UpgcgiLogLevel"
#define LOGGER_NODE_FTPUPGLEVEL             "FtpupgLogLevel"     /* FTP 升级模块 */
#define LOGGER_NODE_PTILEVEL                "PtiLogLevel"
#define LOGGER_NODE_MSG4UDPLEVEL            "Msg4udpLogLevel"   /* MSG4UDP通信模块 */

/* LOG 级别定义 */
#define LOGGER_LEVEL_EMERGENCY     "Emergency"
#define LOGGER_LEVEL_ALERT         "Alert"
#define LOGGER_LEVEL_CRITICAL      "Critical"
#define LOGGER_LEVEL_ERROR         "Error"
#define LOGGER_LEVEL_WARNING       "Warning"
#define LOGGER_LEVEL_NOTICE        "Notice"
#define LOGGER_LEVEL_INFORMATIONAL "Informational"
#define LOGGER_LEVEL_DEBUGGING     "Debugging"

#define LOGGER_DEF_LEVEL           LOG_NOTICE

#define LOGGER_PC_LIST_NULL        0xff

#define MAX_MSG_LEN                1024

/* 与文件操作相关的定义 */
#define LOGGER_EVENTS_REC_PROG     "/usr/bin/logmonitor"
#define LOGGER_SYSLOGD_PROG        "/sbin/syslogd"
#define LOGGER_KLOGD_PROG        "/sbin/klogd"

#define LOGGER_LOG_PATH            "/var/log/"
#define LOGGER_SYSEVENT_NAME       "sysevent.txt"
#define LOGGER_DEVICE_INFO         "device_info"
#define LOGGER_NORMAL_NAME         "messages"

#define LOGGER_PATH_DEBUG_FLASH    "/var/log/original_data.flash"
#define LOGGER_PATH_DEBUG_WRITE    "/var/log/original_data.write"
#define LOGGER_PATH_DEBUG_READ     "/var/log/original_data.read"


#define LOGGER_CONF_MTD_PATH       "/var/log/original_data"     //UML使用的文件

#define LOGGER_ORIGINAL_DATA       "original_data"

/* "/var/log/sysevent.txt" */
#define LOGGER_SYSEVENT_FILE       LOGGER_LOG_PATH LOGGER_SYSEVENT_NAME
#define LOGGER_DEV_INFO_FILE       LOGGER_LOG_PATH LOGGER_DEVICE_INFO

/* "/var/log/messages" */
#define LOGGER_NORMAL_FILE         LOGGER_LOG_PATH LOGGER_NORMAL_NAME

#define LOGGER_DEVINFO_BUF_LEN     1024
#define LOGGER_MTD_FILE_NAME_LEN   32

#define LOGGER_MTD_DESC            "/proc/mtd"         /* MTD 设备分区描述文件 */
#define LOGGER_MTD_KEY             "llconfig"          /* 标识日志文件保存区的关键字 */
#define LOGGER_MTD_FILE_FMT        "/dev/mtdblock%ld"  /* 日志文件存取的格式 */
#define LOGGER_MTD_NO_INDEX        3                   /* mtd0 中 0 的下标 */
#define LOGGER_MTD_NO_BASE         10                  /* mtd0 中 0 的进制 */

#define LOGGER_FILE_KEY_LEN        8

#define LOGGER_MIN_LOG_SIZE        100                 /* 调试日志文件 /var/log/messages 的最小长度 */
#define LOGGER_MAX_LOG_SIZE        512                /* 调试日志文件 /var/log/messages 的最大长度 */
#define LOGGER_MAX_SYSEVENT_SIZE   80                  /* /var/log/sysevent.txt的最大长度 */

#define LOGGER_MAGICNUM            0x4C4F4747          /* 日志文件前的魔数标志 "LOGG" */

#define LOGGER_MAX_MTD_LEN         0xA000              /* 存放日志文件的 MTD 设备分区大小, 40 KBytes */

/* 压缩的日志数据可用最大空间, 可用 MTD 空间大小 - 结构占用空间 - CRC32 长度 */
#define LOGGER_COMP_MAX_LEN \
        (LOGGER_MAX_MTD_LEN - (sizeof(ST_MTD_FILE) + sizeof(unsigned long)))

/* 未压缩的日志文件最大长度，按照此长度截断后的文件应能达到 1000 行日志的需求 */
#define LOGGER_MAX_UNCOMP_FILE_LEN (3 * LOGGER_COMP_MAX_LEN)     /* 3 倍于可用压缩空间 */

#define LOGGER_CMD_LEN             64

#define LOGGER_GETVAL(node)        ((char *)COMM_GetNodeValue(LOGGER_PATH node, CACHE_CFG))

#define LOGGER_NODE_PATH(node)     (BASE_LOGGER_PATH node "$")


/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/

/* LogLevel 节点名称与 MID 关联 */
typedef struct tab_ST_LOGGER_Node_MID_Pair {
    char * pszNodeName;               /* 节点名 */
    unsigned short usMID;
} ST_LOGGER_NodeMidPair;

/* LOG 级别名称与级别关联 */
typedef struct tab_ST_LOGGER_String_Level_Pair {
    char * pszLevelDesc;               /* 描述日志级别的字符串 */
    unsigned char ucLevel;
} ST_LOGGER_StrLevelPair;

/* 日志文件存放时间格式 */
typedef struct
{
    unsigned long year : 6;
    unsigned long mon  : 4;
    unsigned long date : 5;
    unsigned long hour : 5;
    unsigned long minu : 6;
    unsigned long sec  : 6;
} ST_LOGGER_TIME;

/* MTD 分区日志文件存放格式 */
typedef struct
{
    unsigned long ulMagicNum;                   /* 魔字，用于表示压缩后的日志文件 */
    unsigned long ulVer;                        /* 版本，暂不使用 */
    ST_LOGGER_TIME   stTime;                       /* 日志文件保存时间，暂不使用 */
    char          acKey[LOGGER_FILE_KEY_LEN];      /* 内部关键字，暂不使用 */
    unsigned long ulUncompLen;                  /* 未压缩的数据长度 */
    unsigned long ulCompLen;                    /* 压缩后的数据长度 */
    char          acData[0];                    /* 压缩后的数据 */
} ST_MTD_FILE;


/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/

int LOGGER_UploadCommitHandle(char *pszPath,char *pszValue);
int LOGGER_ClearCommitHandle(char *pszPath,char *pszValue);
int LOGGER_LogLevelCommitHandle(char *pszPath,char *pszValue);
int LOGGER_DeamonCommitHandle(char *pszPath,char *pszValue);

int LOGGER_ModuleInit(void);
int LOGGER_ModuleDestroy(void);
int LOGGER_MsgStartProcAckHandler(ST_MSG *pstMsg);
int LOGGER_MsgSignalAckHandler(ST_MSG *pstMsg);
int LOGGER_CheckTftpServ(const char *pszValue);
int LOGGER_CheckSingleAddr(const char *pszString);
unsigned short LOGGER_ConvertToMID(const char *pszModName);
unsigned char LOGGER_ConvertToLevel(const char *pszLevel);
int LOGGER_CheckLogLevel(const char *pszString);
int LOGGER_RestoreHistory(void);
int LOGGER_WriteToFileLocked(const char *pszPath, const char *pszBuf, unsigned long ulLen, int iFlag);
int LOGGER_ConfigDaemon(void);
int LOGGER_ConfEvtRecorder(int iAction);
int LOGGER_SendProcSignal(unsigned char ulProcID, unsigned long ulMsgID, unsigned char ucSignal);
int LOGGER_StopDaemonProc(void);
int LOGGER_StopRecorderProc(void);
int LOGGER_KlogdProcCtrl(const char *pcName, unsigned long ulInstNo, unsigned char ucOpt);
int LOGGER_StartProc(const char *pszName, unsigned long ulMsgID, ...);
int LOGGER_StartDaemonPorc(int iSysLogEn, const char *pszRmtAddr);
int LOGGER_StartRecorderProc(void);
int LOGGER_CheckFileSize(const char *pszValue);
int LOGGER_DebugLevelInit(void);
int LOGGER_RegisterTimer(void);
int LOGGER_UnregisterTimer(void);
int LOGGER_FileLockTruncate(const char *pszFile, long lSize);
ssize_t LOGGER_SafeWrite(int iFD, const void *pszBuf, size_t ulCount);
ssize_t LOGGER_FullWrite(int iFD, const void *pszBuf, size_t ulLen);
int LOGGER_MsgTimeoutHandler(ST_MSG *pstMsg);
int LOGGER_MsgProcStateHandler(ST_MSG *pstMsg);
int LOGGER_MsgLanIpUpdateHandler(ST_MSG *pstMsg);
int LOGGER_FileMonitor(void);
int LOGGER_TftpUploadFile(void);
int LOGGER_ZeroMtdLogFile(void);
#ifdef LOGGER_DIRECT_READ_FLASH
int LOGGER_ReadFile(const char *pszPath, char *pszBuf, unsigned long *pulLen);
#endif
unsigned long LOGGER_SkipDevInfo(const char *pszData, unsigned long ulDataLen);
int LOGGER_MakeInfoPathDevIP(char *pszBuf, unsigned long ulBufLen);
int LOGGER_MakeDevInfo(void);

/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/
#endif


