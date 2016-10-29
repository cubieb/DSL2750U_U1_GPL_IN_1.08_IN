
/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: pti_product.h
 文件描述: 产测功能配置文件
 修订记录:
        1. 作者: 朱健文
           日期: 
           内容: 创建文件
**********************************************************************/

#ifndef _PTI_PRODUCT_H
#define _PTI_PRODUCT_H

/******************************************************************************
*                                STRUCT                                      *
******************************************************************************/

/* 产测支持的功能 */
struct funcTable funcNode[] = 
{
/***********************************非CMM节点****************************************/
//1. ######产测必须部分#######    
    {"--led",       PTI_LedFunc,        HELP_LED_MSG,       FUNC_ENABLE,    NOT_CMM_NODE},
	{"--button",    PTI_ButtonFunc,     HELP_BUTTON_MSG,    FUNC_ENABLE,    NOT_CMM_NODE},
	{"--mac",       PTI_MacAddrFunc,    HELP_MAC_MSG,       FUNC_ENABLE,    NOT_CMM_NODE},
	{"--wlanpasswd", PTI_WlanPasswdFunc,    HELP_WLAN_PASSWORD_MSG,       FUNC_ENABLE,    NOT_CMM_NODE},
	{"--wlanssid", PTI_WlanSsidFunc,    HELP_WLAN_SSID_MSG,       FUNC_ENABLE,    NOT_CMM_NODE},
#ifdef CONFIG_APPS_CLI_SEPARATE_AUTH
	{"--telnetpasswd", PTI_TelnetPasswordFunc,  HELP_TELNET_PASSWORD_MSG,   FUNC_ENABLE,    NOT_CMM_NODE},
#endif
	{"--usb",       PTI_UsbFunc,        HELP_USB_MSG,       FUNC_ENABLE,    NOT_CMM_NODE},		
	{"--ftp",       PTI_FtpFunc,        HELP_FTP_MSG,       FUNC_ENABLE,    NOT_CMM_NODE},
	{"--restore",   PTI_RestoreFunc,    HELP_RESTORE_MSG,   FUNC_ENABLE,    NOT_CMM_NODE},
	{"--help",      PTI_HelpFunc,       HELP_HELP_MSG,      FUNC_ENABLE,    NOT_CMM_NODE},
//	{"--wan",       PTI_WanLinkFunc,    HELP_WAN_MSG,       FUNC_ENABLE,    NOT_CMM_NODE},	
	{"--TW_version", PTI_GetTwVersion,  HELP_GET_TW_VERSION, FUNC_ENABLE, NOT_CMM_NODE},

	
//2. ######与产品相关部分######
//    {"--adjust",    PTI_AdjustFunc,     HELP_ADJUST_MSG,    FUNC_ENABLE,    NOT_CMM_NODE},

/***********************************CMM节点********************************************/	
//1. ######产测必须部分#######  
    {"--hardware",  NULL,     HELP_HARDV_MSG,     FUNC_ENABLE,    CMM_HARDVERSION_OPTION},
	{"--software",  NULL,     HELP_SOFTV_MSG,     FUNC_ENABLE,    CMM_SOFTVERSION_OPTION},	
	{"--firmware",  NULL,     HELP_FIRMWARE_MSG,  FUNC_ENABLE,    CMM_FIRMWARE_OPTION},
//    {"--lanIP",     NULL,     HELP_LANIP_MSG,     FUNC_ENABLE,    CMM_LANIP_OPTION},	
    
//2. ######与产品相关部分######
    {"--wrate",        NULL,     HELP_WRATE_MSG,        FUNC_ENABLE,    CMM_WRATE_OPTION},		
	{"--ssid",         NULL,     HELP_SSID_MSG,         FUNC_ENABLE,    CMM_SSID_OPTION},
	{"--wcode",        NULL,     HELP_WCODE_MSG,        FUNC_ENABLE,    CMM_WCODE_OPTION},
	{"--channal",      NULL,     HELP_CHANNAL_MSG,      FUNC_ENABLE,    CMM_CHANNAL_OPTION},	
	{"--wmm",          NULL,     HELP_WMM_MSG,          FUNC_ENABLE,    CMM_WMM_OPTION},
//	{"--envlan",       NULL,     HELP_APVLAN_ENABLE_MSG,         FUNC_ENABLE,    CMM_AP_VLAN_ENABLE_OPTION},
	{"--dsluprate",    NULL,     HELP_DSLUPRATE_MSG,    FUNC_ENABLE,    CMM_DSL_UPRATE_OPTION},
	{"--dsldownrate",  NULL,     HELP_DSLDOWNRATE_MSG,  FUNC_ENABLE,    CMM_DSL_DOWNRATE_OPTION},
	{"--pppoeusr",     NULL,     HELP_PPPOEUSR_MSG,     FUNC_ENABLE,    CMM_PPPOE_USRNAME_OPTION},
	{"--pppoepw",      NULL,     HELP_PPPOEPW_MSG,      FUNC_ENABLE,    CMM_PPPOE_PASSWORD_OPTION},
	{"--hssid",        NULL,     HELP_HIDE_SSID_MSG,    FUNC_ENABLE,    CMM_HIDE_SSID_OPTION},
	{"--wireless",        NULL,     HELP_WIRELESS_ENABLED_MSG,    FUNC_ENABLE,    CMM_WIRELESS_ENABLED_OPTION},
  {"--checkreset",   NULL,     HELP_CHECK_RESET_MSG,  FUNC_ENABLE,    CMM_CHECK_RESET_OPTION}
};

/* CMM节点属性列表 */
struct cmmTable cmmNode[] = 
{
    {NULL,  PATH_SSID_NODE,           WRITE_ENABLE,    CMM_SSID_OPTION},
    {NULL,  PATH_SOFTWARE_VERSION,    WRITE_DISABLE,   CMM_SOFTVERSION_OPTION},
    {NULL,  PATH_HARDWARE_VERSION,    WRITE_DISABLE,   CMM_HARDVERSION_OPTION},
    {NULL,  PATH_CHANNAL_NODE,        WRITE_ENABLE,    CMM_CHANNAL_OPTION},
    {NULL,  PATH_LANIP_NODE,          WRITE_ENABLE,    CMM_LANIP_OPTION},
    {NULL,  PATH_WCODE_NODE,          WRITE_ENABLE,    CMM_WCODE_OPTION},
    {NULL,  PATH_WRATE_NODE,          WRITE_ENABLE,    CMM_WRATE_OPTION},    
    {NULL,  PATH_FIRMWARE_VERSION,    WRITE_DISABLE,   CMM_FIRMWARE_OPTION},    
    {NULL,  PATH_DSL_UPRATE_NODE,     WRITE_DISABLE,   CMM_DSL_UPRATE_OPTION},
    {NULL,  PATH_DSL_DOWNRATE_NODE,   WRITE_DISABLE,   CMM_DSL_DOWNRATE_OPTION},
    {NULL,  PATH_PPPOE_USRNAME_NODE,  WRITE_ENABLE,    CMM_PPPOE_USRNAME_OPTION},
    {NULL,  PATH_PPPOE_PASSWORD_NODE, WRITE_ENABLE,    CMM_PPPOE_PASSWORD_OPTION},
    {NULL,  PATH_WIRELESS_ENABLED_NODE, WRITE_ENABLE,    CMM_WIRELESS_ENABLED_OPTION},
    {NULL,  PATH_CHECK_RESET_NODE,    WRITE_ENABLE,    CMM_CHECK_RESET_OPTION},
    
    {PTI_CmmNodeBoolCheck,  PATH_AP_VLAN_ENABLE_NODE, WRITE_ENABLE,    CMM_AP_VLAN_ENABLE_OPTION},
    {PTI_CmmNodeBoolCheck,  PATH_HIDE_SSID_NODE,      WRITE_ENABLE,    CMM_HIDE_SSID_OPTION},
    {PTI_CmmNodeBoolCheck,  PATH_WMM_NODE,            WRITE_ENABLE,    CMM_WMM_OPTION},
};

/* 特殊的CMM节点属性列表 */
struct speCmmTable speCmmNode[] =
{
    {PATH_MSSID_PATH_NODE, CMM_HIDE_SSID_OPTION},
};


#endif
