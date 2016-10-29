
/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: protest.h
 文件描述: 
 修订记录:
        1. 作者: 朱健文
           日期: 
           内容: 创建文件
**********************************************************************/

#ifndef _PROTEST_H_
#define _PROTEST_H_

#include "tbsmsg.h"
#include "tbstype.h"
#include "tbsutil.h"
#include "tbserror.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/

/*********************打印信息定义***************************/
//#define UT_PTI
//#define PTI_DEBUG

#ifdef PTI_DEBUG
#define TRACE1  //打印必要的信息
#define TRACE2  //打印不必要的信息
#define TRACE3  //补充打印单元测试用例的信息
#endif

#ifdef TRACE1
#define PTI_TRACE1(fmt, args...)  printf("%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define PTI_TRACE1(fmt, args...)  do{;}while(0);
#endif
#ifdef TRACE2
#define PTI_TRACE2(fmt, args...)  printf("%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define PTI_TRACE2(fmt, args...)  do{;}while(0);
#endif
#ifdef TRACE3
#define PTI_TRACE3(fmt, args...)  printf("%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#else
#define PTI_TRACE3(fmt, args...)  do{;}while(0);
#endif

/**********************其他宏定义***********************/
#define TW_HIDDEN_PAGE "/usr/www/test_version.html"
#define TW_PRODUCT_MODEL "Gan9."

#define MODULE_NAME "pti_module: "
#define PTI_PRINTF(fmt, args...)  printf(MODULE_NAME "pti_module: ""%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#define TAB_PRINT "	"
#ifdef UT_PTI
#undef  tbsSystem
#define tbsSystem stub_tbsSystem
#endif
#define PTI_System(cmd) tbsSystem(cmd, 0)
#define TelnetPrint(fmt, args...)  printf("~!"fmt"!~\n\r", ##args)
#define HelpPrint(fmt, args...)  printf(fmt, ##args)

#define PTI_PRODUCT PTI_OTHER

#define HELP_MSG_LENGTH 100 
#define FUNC_FLAG_LENGTH 20
#define MAX_NODE_LENGTH 256
#define MSG_BODY_LENGTH   64*1024
#define IP_ADDR_LENGTH  20
#define MAX_LENGTH_PATH 100
#define MAX_LENGTH_CMD 80
#define MAX_LENGTH_BUF 50
#define MAX_NAME_LEN  32
#define MAX_MAC_LEN   12

#ifndef WPA_PSK_LEN
#define WPA_PSK_LEN  65
#endif

#define PATH_PASSWD   "/var/wlanpasswd"
#define PATH_SSID     "/var/ssid"
#define PATH_MAC_ADDR "/proc/llconfig/macaddr"
#define PATH_PRODUCT_MESSAGE "/proc/llconfig/product"
#define PATH_RMMOD "/etc/init.d/rmmod.sh"
#define AP699E7_MSG "AR7130_AP699E7.2U10-4"
#define USB_FLAG_PATH "/var/usbevent"
#define RETURN_SUCCSS   "success"
#define RETURN_FAIL     "false"
#define RETURN_ERROR    "error"


//button
#define MAX_MSGSIZE 1024
#define BLP_CMM_GROUP 0
#define PID_BUF_LEN 32
#define BUTTON_MSG_LEN 128
#define BUTTON_BAN "ban"
#define BUTTON_RESUME "resume"
typedef struct {
	unsigned long	upstreamRate;
	unsigned long	downstreamRate;
} Modem_LinkSpeed;
typedef struct OBCIF_ARG 
{   
	int argsize;
    int arg;
}obcif_arg;
//#define BUTTON_WPS_BAN "wps=ban"
//#define BUTTON_WLAN_BAN "wlan=ban"
//#define BUTTON_RESET_BAN "reset=ban"
//#define BUTTON_WPS_RESEME "wps=resume"
//#define BUTTON_WLAN_RESEME "wlan=resume"
//#define BUTTON_RESET_RESEME "reset=resume"

//help MESSAGE
#define HELP_START_MSG  "usage: protest --option1 [-option2] [option3]...\n\r"
#define HELP_LED_MSG    "--led -on/off	    led all on/off\n\r"
#define HELP_BUTTON_MSG "--button -ban/resume/r		ban/resume button normal function; read button message!\n\r"
#define HELP_MAC_MSG    "--mac -r/-w mac-addr		read mac addr/set mac addr\n\r"
#define HELP_SSID_MSG   "--ssid -r/-w ssid		read ssid/set ssid\n\r"
#define HELP_WLAN_PASSWORD_MSG   "--wlanpasswd -r/-w -wep(wpa) wlanpasswd		read wlanpasswd/set wlanpasswd\n\r"
#define HELP_WLAN_SSID_MSG   "--wlanssid -r/-w ssid		read ssid/set ssid\n\r"
#define HELP_TELNET_PASSWORD_MSG "--telnetpasswd -r/-w password       read/set telnet password\n\r"
#define HELP_WCODE_MSG   "--wcode -r/-w country-code		read country code/set country code\n\r"
#define HELP_CHANNAL_MSG    "--channal -r/-w channal		read channal/set channal\n\r"
#define HELP_WAN_MSG    "--wan -ip ip_addr -netmask netmask		build static link\n\r"
#define HELP_USB_MSG    "--usb		get message if there are some usb devices\n\r"
#define HELP_FTP_MSG  "--ftp -on/off	    on:run ftpserver; off:stop ftpserver\n\r"
#define HELP_HARDV_MSG    "--hardware -r		get the hardware version\r\n"
#define HELP_SOFTV_MSG    "--software -r		get the software version\r\n"
#define HELP_LANIP_MSG     "--lanIP -r/-w ip_addr  get/set the LAN ip address\n\r"
#define HELP_RESTORE_MSG    "--restore  restore factory setting\n\r"
#define HELP_ADJUST_MSG "--adjust   set enviroment for wireless adjust\n\r"
#define HELP_WRATE_MSG   "--wrate -r/-w rate  get/set the wireless rate\n\r"
#define HELP_HELP_MSG   "--help		protest command help\n\r"
#define HELP_FIRMWARE_MSG "--firmware -r    get the CPE firmware\n\r"
#define HELP_WMM_MSG    "--wmm -w 1/0   shutdown/open the wireless Qos function\n\r"
#define HELP_DSLUPRATE_MSG "--dsluprate -r    get the dsl current upstream rate\n\r"
#define HELP_DSLDOWNRATE_MSG   "--dsldownrate -r    get the dsl current downstream rate\n\r"
#define HELP_PPPOEUSR_MSG   "--pppoeusr -r/-w usrname   get/set the pppoe username\n\r"
#define HELP_PPPOEPW_MSG    "--pppoepw -r/-w passwd     get/set the pppoe password\n\r"
#define HELP_APVLAN_ENABLE_MSG   "--envlan -w 1/0     shutdown/open the vlan function\n\r"
#define HELP_HIDE_SSID_MSG  "--hssid -w 1/0  hide the ssid or show the ssid \n\r"
#define HELP_WIRELESS_ENABLED_MSG  "--wireless -r/-w 1/0  enable or disable the wireless \n\r"
#define HELP_CHECK_RESET_MSG "--checkreset -r is not or is default config\n\r"
#define HELP_GET_TW_VERSION "--TW_version -r    read the internel TW version.\n\r"

//节点路径
#define PATH_SOFTWARE_VERSION "InternetGatewayDevice.DeviceInfo.SoftwareVersion"
#define PATH_HARDWARE_VERSION "InternetGatewayDevice.DeviceInfo.HardwareVersion"
#define PATH_SSID_NODE "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.SSID"
#define PATH_CHANNAL_NODE "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Channel"
#define PATH_LANIP_NODE "InternetGatewayDevice.LANDevice.1.LANHostConfigManagement.IPInterface.1.IPInterfaceIPAddress"
#define PATH_WCODE_NODE "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.RegulatoryDomain"
#define PATH_WRATE_NODE "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.MaxBitRate"
#define PATH_CHECK_RESET_NODE "InternetGatewayDevice.DeviceInfo.X_TWSZ-COM_FactoryMode"

#define PATH_FIRMWARE_VERSION "InternetGatewayDevice.DeviceInfo.ModemFirmwareVersion"
#define PATH_WMM_NODE "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_WMMEnabled"
#define PATH_DSL_UPRATE_NODE "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.UpstreamCurrRate"
#define PATH_DSL_DOWNRATE_NODE "InternetGatewayDevice.WANDevice.1.WANDSLInterfaceConfig.DownstreamCurrRate"
//#define PATH_DSL_UPRATE_NODE "InternetGatewayDevice.X_TWSZ-COM_Vdsl.1.ExtendedStatus.SlowUsPayloadRate"
//#define PATH_DSL_DOWNRATE_NODE "InternetGatewayDevice.X_TWSZ-COM_Vdsl.1.ExtendedStatus.SlowDsPayloadRate"
#define PATH_PPPOE_USRNAME_NODE "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Username"
#define PATH_PPPOE_PASSWORD_NODE "InternetGatewayDevice.WANDevice.1.WANConnectionDevice.1.WANPPPConnection.1.Password"
#define PATH_AP_VLAN_ENABLE_NODE "InternetGatewayDevice.X_TWSZ-COM_VLAN.Enable"
#define PATH_HIDE_SSID_NODE "InternetGatewayDevice.LANDevice.1.WLANConfiguration.%s.X_TWSZ-COM_HideSSID"
#define PATH_MSSID_PATH_NODE "InternetGatewayDevice.LANDevice.1.WLANConfiguration"
#define PATH_WIRELESS_ENABLED_NODE "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.Enable"
#define PATH_BEACONTYPE_NODE  "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.BeaconType"
#define PATH_WEPKEY_NODE  "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.WEPKey.1.WEPKey"
#define PATH_PSK_NODE "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.X_TWSZ-COM_PSKExpression"
#define PATH_KPP_NODE "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.PreSharedKey.1.KeyPassphrase"
#define PATH_PSKEY_NODE "InternetGatewayDevice.LANDevice.1.WLANConfiguration.1.PreSharedKey.1.PreSharedKey"
#define NODE_WLAN_WPA_KEY_PASSPHRASE "KeyPassphrase"
#define NODE_WLAN_WPA_PSK			"PreSharedKey"
#define BEACON_TYPE_NONE			"None"
#define BEACON_TYPE_BASIC			"Basic"
#define BEACON_TYPE_WPA				"WPA"
#define BEACON_TYPE_11i				"11i"
#define BEACON_TYPE_WPA2_MIXED 		"WPAand11i"


#define NOT_CMM_NODE -1

/******************************************************************************
*                                 ENUM                                       *
******************************************************************************/
enum
{
    PTI_WIRELESS_AP = 0,
    PTI_IKANOS,
    PTI_OTHER,
};

enum
{
	CMM_SSID_OPTION = 0,
	CMM_WCODE_OPTION,
	CMM_CHANNAL_OPTION,
    CMM_HARDVERSION_OPTION,
	CMM_SOFTVERSION_OPTION,
	CMM_LANIP_OPTION,
	CMM_WRATE_OPTION,
	
	CMM_FIRMWARE_OPTION,
	CMM_WMM_OPTION,
	CMM_DSL_UPRATE_OPTION,
	CMM_DSL_DOWNRATE_OPTION,
	CMM_PPPOE_USRNAME_OPTION,
	CMM_PPPOE_PASSWORD_OPTION,
	CMM_AP_VLAN_ENABLE_OPTION,
	CMM_HIDE_SSID_OPTION,
	CMM_WIRELESS_ENABLED_OPTION,
	CMM_CHECK_RESET_OPTION,
};

enum
{
    FUNC_ENABLE = 0,
    FUNC_DISABLE,
};

enum
{
    WRITE_ENABLE = 0,
    WRITE_DISABLE,
};

enum ret_result
{
	PTI_RET_OK = 0,
	PTI_RET_FALSE,
};

enum
{
	PTI_LED_ON = 0,
	PTI_LED_OFF,
};

enum
{
	PTI_BUTTON_BAN = 0,
	PTI_BUTTON_RESUME,
};

enum
{
	PTI_WRITE_OPERATE = 0,
	PTI_READ_OPERATE,
};

enum BUTTON_RECEIVE_STATE
{
    BTN_RECEIVE_MSG = 0,
    BTN_NOT_RECEIVE_MSG,
    BTN_RECEIVE_ERROR,
};

/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/

/* 读写缓存数据结构 */
struct para_buf
{
	char readBuf[MAX_NODE_LENGTH];
	char writeBuf[MAX_NODE_LENGTH];
};

/* 功能列表 */
struct funcTable
{
    char  function[FUNC_FLAG_LENGTH];        //功能标识，如--led，--mac
    void (*func)(int arg, char **argc);      //功能函数，对某个标识调用相应的处理函数
	char  *helpMsg;                          //help打印信息
	int  enable;                             //使能开关，对某个产品可以设置此标识，用来开启或者关闭此功能点
    int  CmmPosition;                        //如果不是cmm节点操作，那么设置为－1，如果是，用来标识其在cmm数组中所在位置
};

/* CMM节点属性 */
struct cmmTable
{
    int (*node_check)(char *nodeValue);  //节点检测函数
//    void (*func)(int arg, char **argc, int CmmPosition);
    char nodePath[MAX_NODE_LENGTH];                    //节点所在路径
	int  enWrite;                                      //节点是否可写
	int  nodeNum;                                      //节点标识，区别不同的节点
};

/* 特殊的CMM属性 */
struct speCmmTable
{
    char nodePath[MAX_NODE_LENGTH];
    int nodeNum;   
};

/* 建立WAN连接数据结构 */
struct wan_para
{
	char wanName[MAX_NAME_LEN];
	char ip_addr[IP_ADDR_LENGTH];
	char netmask[IP_ADDR_LENGTH];
};

/******************************************************************************
*                        FUNCTION DECLARE                                     *
******************************************************************************/

void PTI_LedFunc(int argc, char **argv);
void PTI_ButtonFunc(int argc, char **argv);
void PTI_MacAddrFunc(int argc, char **argv);
void PTI_WlanPasswdFunc(int argc, char **argv);
void PTI_WlanSsidFunc(int argc, char **argv);
void PTI_TelnetPasswordFunc(int argc, char **argv);
void PTI_WanLinkFunc(int argc, char **argv);
void PTI_UsbFunc(int argc, char **argv);
void PTI_RestoreFunc(int argc, char **argv);
void PTI_AdjustFunc(int argc, char **argv);
void PTI_HelpFunc(int argc, char **argv);
int PTI_CmmNodeBoolCheck(char *nodeValue);
void PTI_FtpFunc(int argc, char **argv);
void PTI_GetTwVersion(int argc, char **argv);

/******************************************************************************
*                               UNIT TEST                                     *
******************************************************************************/
#ifdef UT_PTI
FILE *stub_fopen(const char *path, const char *mode);

#define fopen stub_fopen
#define fgets stub_fgets
#define fputs PTI_utfputs
#define MSG_AllModStartOK() do{;}while(0);
#define fclose stub_fclose

#endif

#endif
