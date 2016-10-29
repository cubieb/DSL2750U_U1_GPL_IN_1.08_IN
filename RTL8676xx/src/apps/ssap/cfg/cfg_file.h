/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : cfgapi.h
 文件描述 : cfg实现文件读写配置保存恢复等功能的头文件


 修订记录 :
          1 创建 : 陈跃东
            日期 : 2007-11-23
            描述 :

**********************************************************************/

#ifndef __CFGFILE_H__
#define __CFGFILE_H__

#include "cfg_api.h"

#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif



/******************************************************************************
 *                                 MACRO                                      *
 ******************************************************************************/


#define CFG_DEF_FILE  "/etc/config.xml"     /* 缺省配置文件 */
#define CFG_FULL_FILE "/etc/config_full.xml"

#define CFG_WRITE_MOD    0666               /* 读写配置文件的标志 */

#define CFG_FILE_NAME_LEN 32
#define CFG_FILE_KEY_LEN  8

#define MAGICNUM 0x434F4E46UL               /* 配置文件前的魔数标志"CONF" */



#define CFG_COMM_MTD_LEN   (1024*20)    /* MTD配置文件预计长度 */

#define CFG_MAX_MTD_LEN   0xffff    /* MTD配置文件最大长度 */



/******************************************************************************
 *                                STRUCT                                      *
 ******************************************************************************/



/* 配置文件时间格式 */
typedef struct
{
    unsigned long year : 6;
    unsigned long mon  : 4;
    unsigned long date : 5;
    unsigned long hour : 5;
    unsigned long minu : 6;
    unsigned long sec  : 6;

} ST_CFG_TIME;


/* 配置文件存放格式 */
typedef struct
{
    unsigned long ulMagicNum;
    unsigned long ulVer;
    ST_CFG_TIME   stTime;
    char          acKey[CFG_FILE_KEY_LEN];
    unsigned long ulUncompLen;
    unsigned long ulCompLen;
    unsigned long ulCrc;
    char          acData[0];

} ST_CFG_FILE;


/******************************************************************************
 *                               GLOBAL                                       *
 ******************************************************************************/
/*
  还原出厂默认配置之后，不再允许写flash
  只能重启后再配置 add by pengyao 20121015
*/
extern int g_bResetedFlag;



/******************************************************************************
 *                               FUNCTION                                     *
 ******************************************************************************/
 #ifdef CONFIG_TBS_BACKUP_ENCRYPTION
CFG_RET CFG_ReadCfgfile_EX(const char *pcFile, char **ppcBuf, unsigned long ulPrevSpace,
                     unsigned long ulPostSpace, unsigned long *pulLen);
#endif
CFG_RET CFG_ReadFile(const char *pcFile, char **ppcBuf,
                     unsigned long ulPrevSpace, unsigned long ulPostSpace,
                     unsigned long *puLen);

CFG_RET CFG_WriteFile(const char *pcFile, const char *pcBuf, unsigned long ulLen);


/* 读取当前配置 */
CFG_RET CFG_ReadCurCfg(const char *pcItem, char **ppcBuf, unsigned long ulPrevSpace,
                       unsigned long ulPostSpace, unsigned long *pulLen);

/* 读取缺省配置 */
CFG_RET CFG_ReadDefaultCfg(char **ppcBuf, unsigned long ulPrevSpace,
                           unsigned long ulPostSpace, unsigned long *pulLen);
CFG_RET CFG_ReadFullCfg(char **ppcBuf, unsigned long ulPrevSpace,
                           unsigned long ulPostSpace, unsigned long *pulLen);


/* 恢复出厂配置 */
CFG_RET CFG_RecoverFactCfg(const char *pcAccessor);


/* 清除当前配置 */
CFG_RET CFG_ClearCurCfg(void);

#ifdef CONFIG_APPS_LOGIC_WANSELECT
CFG_RET CFG_ClearAllTheCfg(void);
#endif

typedef CFG_RET (*FUNC_CFG_SPEC_RECOVER)(void);


/******************************************************************************
 *                                 END                                        *
 ******************************************************************************/

int app_item_get( void *data , char *item_name ,unsigned short *len );
int app_item_save( void *data , char *item_name ,unsigned short len );


#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif


#endif /* __CFGFILE_H__ */
