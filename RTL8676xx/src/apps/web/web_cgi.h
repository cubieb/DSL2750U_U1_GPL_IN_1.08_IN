/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : web_cgi.h
 文件描述 : WEB服务器部分的Cgi处理文件


 函数列表 :


 修订记录 :
          1 创建 : 刘泽群
            日期 : 2007-8-21
            描述 :

**********************************************************************/


#ifndef __WEB_CGI_H__
#define __WEB_CGI_H__

#include "web_pub.h"
#include "web_cmd.h"
#include "autoconf.h"


/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/

#define WEB_ESC_CHAR            '%'             /* 请求内容转义符*/
#define WEB_SPLIT_CHAR_SET      "&;"            /* 请求内容分隔符集合*/
#define WEB_SSID_TOKEN          "sessionid"
#define WEB_NEWOBJID_TOKEN      "newobjindex"   /* 最后一次AddObj成功后的节点序号 */
#define WEB_LANG_TOKEN          "language"      /* 页面语言 */
#define WEB_NOREDIRECT_TOKEN    "noredirect"    /* POST处理之后不进行重定向操作 */

#define WEB_USERNAME_TOKEN      "sys_UserName"      /* 用户登录名 */
#define WEB_USERLEVEL_TOKEN     "sys_UserLevel"     /* 用户等级 */
#define WEB_USERINDEX_TOKEN     "sys_UserIndex"     /* 用户Id */

#define WEB_ACTION_TOKEN        "obj-action"
#define WEB_ACTION_SET          "set"
#define WEB_ACTION_ADD          "add"
#define WEB_ACTION_DEL          "del"
#define WEB_ACTION_AUTH         "auth"
#define WEB_ACTION_SAVE         "save"
#define WEB_ACTION_RECOVER      "recover"
#define WEB_ACTION_REBOOT       "reboot"
#define WEB_ADD_OBJ_NAME        "add-obj"
#define WEB_DEL_OBJ_NAME        "del-obj"
#define WEB_ADD_SET_NAME        "add-set"
#define WEB_ADD_SET_OBJ_NAME    "add-obj"

#define WEB_ACTION_DEL_ADD_SET  "del-add-set"

#define WEB_SSID_NAME           WEBCMD_VAL_TOKEN WEB_SSID_TOKEN
#define WEB_NEWOBJID_NAME       WEBCMD_VAL_TOKEN WEB_NEWOBJID_TOKEN
#define WEB_LANG_NAME           WEBCMD_VAL_TOKEN WEB_LANG_TOKEN
#define WEB_NOREDIRECT_NAME     WEBCMD_VAL_TOKEN WEB_NOREDIRECT_TOKEN

#define WEB_USERNAME_NAME       WEBCMD_VAL_TOKEN WEB_USERNAME_TOKEN
#define WEB_USERLEVEL_NAME      WEBCMD_VAL_TOKEN WEB_USERLEVEL_TOKEN
#define WEB_USERINDEX_NAME      WEBCMD_VAL_TOKEN WEB_USERINDEX_TOKEN

/**/
#define WEB_MID_TAG             "mid"

/* 模块列表文件*/
#define WEB_PATH_MODLIST        "/etc/modlist.ini"
/*页面权限列表*/
#define WEB_AUTH_LIST           "/usr/www/webAuth.xml"
/* 模块宏前缀*/
#define LOGIC_CONFIG_TOKEN      "CONFIG_APPS_LOGIC_"


/******************************************************************************
 *                                 ENUM                                       *
 ******************************************************************************/

/* CGI通用类型和返回值*/
enum en_CGI_GEN_RET
{
    CGI_NONE = 0,       /* 无*/
    CGI_GET,            /* GET请求*/
    CGI_POST,           /* POST请求*/
    CGI_ERROR           /* 错误*/
};

/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/

/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/

/* 将Url中的转义符取消转义 */
void WEB_UrlUnEsc(char *szUrl);

/*  产生一个随机的SesionID  */
unsigned long WEB_CreateSSID();

/* 以name/val的方式获取CGI变量 */
int  WEB_GetCgiVars(ST_NAME_VAL **ppstCgiVal);

/* 分类获取各类CGI变量，加入全局链表 */
int  WEB_GetEnv(void);

/* 添加系统支持的模块到g_pstWebVars中*/
void WEB_AddSupportMods();

/* 添加系统支持的语言到 g_pstWebVars */
void WEB_AddSupportLangs();

/*=========================================================================*/
/*  函数功能: OBJCET的POST的处理函数                                       */
/*=========================================================================*/
int WEB_PostObjSet();   // 设置
int WEB_PostObjAdd();   // 添加
int WEB_PostObjDel();   // 删除
int CheckAuth(char *pszSessionID);
int WEB_PostObjAddSet();
int WEB_PostObjDelAddSet();

int WEB_PostSimpCmd(unsigned short usMsgType);


/* POST的处理函数 */
int  WEB_Post();
/*页面认证处理函数*/
int WEB_CheckPageLevel(char *pageName,int pageLevel);

#endif /* __WEB_CGI_H__ */


