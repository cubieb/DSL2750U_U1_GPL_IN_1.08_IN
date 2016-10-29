
/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: dsl_api.h
 文件描述: dsl HAL

 修订记录:
       1 作者 : huangce
         日期 : 2009-1-06
         描述 : 创建

       2 作者 : Xuan Guanglei
         日期 : 2009-07-20
         描述 : 由函数直接调用改为注册
         
**********************************************************************/

#ifndef __DSL_API_H__
#define __DSL_API_H__
#include "dsl.h"
#define DSL_API_DEVICE_NAME "/dev/dsl_cpe_api"

typedef struct
{
    int   (*DSL_Start)(void);
    int   (*DSL_Stop)(void);
    int   (*DSL_StartDevice)(char *szDSLMode);
    int   (*DSL_StopDevice)(void);
    int   (*DSL_SetDSLInfo)(const char *pcName, const char *pcValue);
    int   (*DSL_GetDSLInfo)(const char *pcName, char *pcValue,int len);
    int   (*DSL_GetDslStandard)(char *pcValue, int len , int *iModeMaxCount);
    int   (*DSL_UpdateDslStatus)(void);
    int   (*DSL_GetStatus)(char *pcValue);
}DslApiFuncSet;

extern int DSL_GetStatus(char *pcValue);
extern int DSL_GetUpstreamCurrRate(char *pcValue);
extern int DSL_GetDownstreamCurrRate(char *pcValue);
extern int DSL_GetUpstreamMaxRate(char *pcValue);
extern int DSL_GetDownstreamMaxRate(char *pcValue);
extern int DSL_GetUpstreamNoiseMargin(char *pcValue);
extern int DSL_GetDownstreamNoiseMargin(char *pcValue);
extern int DSL_GetUpstreamAttenuation(char *pcValue);
extern int DSL_GetDownstreamAttenuation(char *pcValue);
extern int DSL_GetPowerManagementState(char *pcValue);
extern int DSL_GetStandardUsed(char *pcValue);
//extern int DSL_GetUpInterleaveDepth(char *pcValue);
extern int DSL_GetDownInterleaveDepth(char *pcValue);
extern int DSL_GetErroredSecs(char *pcValue);
extern int DSL_GetATUCErroredSecs(char *pcValue);
extern int DSL_GetHECErrors(char *pcValue);
extern int DSL_GetATUCHECErrors(char *pcValue);
extern int DSL_GetCRCErrors(char *pcValue);
extern int DSL_GetATUCCRCErrors(char *pcValue);
extern int DSL_GetSeverelyErroredSecs(char *pcValue);
extern int DSL_GetATUCSeverelyErroredSecs(char *pcValue);
extern int DSL_GetFECErrors(char *pcValue);
extern int DSL_GetATUCFECErrors(char *pcValue);
extern int DSL_GetInterleaveDepth(char *pcValue);
extern int DSL_GetDataPath(char *pcValue);
extern int DSL_SetDSLInfo(const char *pcName, const char *pcValue);
extern int DSL_GetDevice();
extern void DSL_ReleaseDevice();
extern int DSL_StartDevice(char *szDSLMode);
extern int DSL_StopDevice();

extern void dsl_register(void);
extern void dsl_register_api(DslApiFuncSet * pstApiFunSet);
extern int DSL_Start_Api(void);
extern int DSL_Stop_Api(void);
extern int DSL_GetDslStandard(char *pcValue, int len , int *iModeMaxCount);
extern int DSL_GetDSLInfo(const char *pcName, char *pcValue,int len);
extern int DSL_UpdateDslStatus(void);

#endif

