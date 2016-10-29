/**********************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : cmm.h
 文件描述 : cmm模块对外的头文件


 修订记录 :
          1 创建 : 陈跃东
            日期 : 2008-11-23
            描述 :

**********************************************************************/


#ifndef __CMM_H__
#define __CMM_H__


#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif


#define CMM_OK              0
#define CMM_ERR_BASE_ADDR   ((MID_CMM << 16) | 0x00000001)


#define CMM_MAX_MID_NUM          32
#define CMM_MSG_PART_STEP        16
#define CMM_MAX_PATH_LEN         256


typedef  long  CMM_RET;


typedef struct
{
    long lStatus;
    unsigned long ulIndex;
    char acErrPth[CMM_MAX_PATH_LEN];

} ST_CMM_OPT_RESULT;



typedef long (*FUNC_CMM_PROC)(const ST_MSG *pstMsg, ST_CMM_OPT_RESULT *pstOptResult);



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif

#endif /* __CMM_H__ */


