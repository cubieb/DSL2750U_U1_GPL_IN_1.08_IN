/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : app_btn_cfg.c
 文件描述 : 本文件保存应用层按钮配置信息，此文件将会被连接到 build/app/ssap/btn 目录下编译
 函数列表 : 

 
 修订记录 :
          1 创建 : 张喻
            日期 : 2008-11-07
            描述 : 创建文档

**********************************************************************/

#include <btn.h>
#include <btn_app.h>

#define wlan_wps_in_wlan


struct APP_BTN_CFG AppBtnCfg[] =
{
    {btn_wps,CFG_APP_BTN_MODE_RELEASE_LESS,1,0,APP_BTN_EVEN_WPS},
    {btn_wps,CFG_APP_BTN_MODE_TIMER,1,0,APP_BTN_EVEN_WPS},
#ifdef wlan_wps_in_wlan
    {btn_wlan,CFG_APP_BTN_MODE_RELEASE_LESS,1,0,APP_BTN_EVEN_WLAN},
    {btn_wlan,CFG_APP_BTN_MODE_INTERVAL,2,6,APP_BTN_EVEN_WPS},
     {btn_wlan,CFG_APP_BTN_MODE_RELEASE_GREATER,7,0,APP_BTN_EVEN_WPS_SWITCH},
#else
    {btn_wlan,CFG_APP_BTN_MODE_RELEASE_LESS,1,0,APP_BTN_EVEN_WLAN},
    {btn_wlan,CFG_APP_BTN_MODE_TIMER,1,0,APP_BTN_EVEN_WLAN},
#endif
    {btn_reset,CFG_APP_BTN_MODE_RELEASE_LESS,3,0,APP_BTN_EVEN_REBOOT},  /* RESET按钮:按下后松开，小于3秒触发APP_BTN_EVEN_REBOOT事件 */
    {btn_reset,CFG_APP_BTN_MODE_RELEASE_GREATER,5,0,APP_BTN_EVEN_RESET},/* RESET按钮:按下后松开，大于5秒触发APP_BTN_EVEN_RESET事件 */
    {btn_reset,CFG_APP_BTN_MODE_TIMER,1,0,APP_BTN_EVEN_RESET},          /* RESET按钮:按下不动8秒触发APP_BTN_EVEN_RESET事件 */
    {btn_end,0,0,0},                                                  /* btn_end:数组结束标志 */
};

