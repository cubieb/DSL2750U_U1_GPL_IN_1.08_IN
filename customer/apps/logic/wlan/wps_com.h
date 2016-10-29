  
/******************************************************************************
  Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
  
  文件名称: wps_com.h
  文件描述: WPS模块的公用头文件，主要定义模块调试宏等公共定义
  函数列表: //列出主要函数
  修订记录:
         1. 作者: Hansel He < heyiming@twsz.com>
            日期: 2008-8-11
            内容: 创建文件
 ******************************************************************************/


#ifndef __WPS_COM_H__
#define __WPS_COM_H__
#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif

/****************************************************************************** 
 *                                 MACRO                                      * 
 ******************************************************************************/

#define ACTION_CALL                     "CALL"
#define ACTION_RETURN                   "RETURN"

#if 0
#define WPS_PRINT(format, args...)		printf("[%s:%d:%s()] ==> "format, __FILE__, __LINE__, __FUNCTION__, ##args)
#else
#define WPS_PRINT(format, args...)		TBS_TRACE(MID_WLAN, format, ##args)
#endif

#ifdef WLAN_WPS_DEBUG
#define WPS_TRACE(format, args...)		WPS_PRINT(format, ##args)
#define WPS_ASSERT(cond)                 do{ \
                                            if(!(cond)){ \
                                               WPS_PRINT("ASSERT failed!\n"); \
                                               exit(-1);} \
                                          }while(0)
#define WPS_FUNC_TRACE(action, arg_fmt, args...)		WPS_TRACE("%s: %s("arg_fmt") ...... \n", action,  \
                                                            __FUNCTION__, ##args)                                          
#define WPS_FUNC_ENTER_TRACE(arg_fmt, args...)	WPS_FUNC_TRACE(ACTION_CALL,arg_fmt, ##args)
#define WPS_FUNC_EXIT_TRACE()			WPS_FUNC_TRACE(ACTION_RETURN,"")
#else
#define WPS_TRACE(format, args...)
#define WPS_ASSERT(cond) 
#define WPS_FUNC_TRACE(action, arg_fmt, args...)
#define WPS_FUNC_ENTER_TRACE(arg_fmt, args...)
#define WPS_FUNC_EXIT_TRACE()
#endif




#define WPS_ERROR(format, args...)      printf("[WPS:ERROR] "format, ##args)
#define WPS_INFO(format, args...)       printf("[WPS:INFO] "format, ##args)

#define SYSTEM_CMD_MAX_LEN              256
#define SYSTEM_CMD(cmd)                 tbsSystem((cmd), 0)

#define WPS_MATCH_CMD(buf, cmd)            (safe_strncmp((buf), (cmd), strlen(cmd)) == 0)

#define RUN_SYS_CMD(format, args...)    do{ \
                                            char cmd[SYSTEM_CMD_MAX_LEN]; \
                                            snprintf(cmd, SYSTEM_CMD_MAX_LEN, format, ##args); \
                                            SYSTEM_CMD(cmd); \
                                        }while(0)
/****************************************************************************** 
 *                                 ENUM                                       * 
 ******************************************************************************/
typedef enum {
    NODE_NOT_SAVE,
    NODE_NEED_SAVE        
}EM_NODE_NEEDSAVE;

typedef enum {
    NODE_UNSTANDARD,
    NODE_STANDARD        
}EM_NODE_STANDARD;

typedef enum {
    NODE_UNWRITEABLE,
    NODE_WRITEABLE        
}EM_NODE_WRITEABLE;
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

#endif /* __WPS_COM_H__ */



