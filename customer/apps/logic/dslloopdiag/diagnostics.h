/**********************************************************************
 Copyright (c), 1991-2012, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: diagnostics.h
 文件描述: tr069诊断模块

 修订记录:
        1. 作者: 韩尚鹏
           日期: 2012-03-14
           内容: 创建文件

**********************************************************************/
#ifndef _TR069DIAG_H_
#define _TR069DIAG_H_

#include "warnlog.h"
#include "tbstype.h"

/*********************************************************************
 *                              DEBUG                                *
 *********************************************************************/
#ifdef TR069DIAG_DEBUG
    #define DIAG_TRACE(fmt, args...) COMMON_TRACE(MID_TR069DIAG, fmt"\n", ##args)
    #define DIAG_ERR(fmt, args...) COMMON_TRACE(MID_TR069DIAG, fmt, ##args)
#else
    #define DIAG_TRACE(fmt, args...) 
    #define DIAG_ERR(fmt, args...) COMMON_LOG_ERROR(MID_TR069DIAG, fmt, ##args)
#endif

#define DIAG_RET_GOTO(iRet, strError, gotoTag)   RET_GOTO(MID_TR069DIAG, iRet, strError, gotoTag)
#define DIAG_TRACE_INTO_FUNC  DIAG_TRACE("##In## %s\n", __func__)

#define DIAG_RET_RETURN(Ret, fmt, args...)  \
    RET_RETURN(MID_TR069DIAG, Ret, fmt, ##args)
    
#define CALL_FUCTION_FAILED     "Call function \"%s\" failed\n"

/*******************************************************************
 *                          节点路径定义                           *
 *******************************************************************/
#define DIAG_LoopDiagnosticsState       "LoopDiagnosticsState"
#define DIAG_ACTPSDds           "ACTPSDds"
#define DIAG_ACTPSDus 			"ACTPSDus"		
#define DIAG_ACTATPds           "ACTATPds"
#define DIAG_ACTATPus 	        "ACTATPus"				
#define DIAG_HLINSCds           "HLINSCds"
#define DIAG_HLINSCus           "HLINSCus"
#define DIAG_HLINGds            "HLINGds" /*vdsl value*/
#define DIAG_HLINGus            "HLINGus" /*vdsl value*/
#define DIAG_HLOGGds            "HLOGGds" /*vdsl value*/
#define DIAG_HLOGGus            "HLOGGus" /*vdsl value*/
#define DIAG_HLOGpsds           "HLOGpsds"
#define DIAG_HLOGpsus           "HLOGpsus"
#define DIAG_HLOGMTds           "HLOGMTds"
#define DIAG_HLOGMTus           "HLOGMTus"
#define DIAG_LATNpbds           "LATNpbds"
#define DIAG_LATNpbus           "LATNpbus"
#define DIAG_SATNds             "SATNds"
#define DIAG_SATNus             "SATNus"
#define DIAG_HLINpsds           "HLINpsds"
#define DIAG_HLINpsus           "HLINpsus"
#define DIAG_QLNGds             "QLNGds"     /*vdsl value*/
#define DIAG_QLNGus 	        "QLNGus"	 /*vdsl value*/				
#define DIAG_QLNpsds            "QLNpsds"
#define DIAG_QLNpsus            "QLNpsus"
#define DIAG_QLNMTds            "QLNMTds"
#define DIAG_QLNMTus            "QLNMTus"
#define DIAG_SNRGds             "SNRGds" /*vdsl value*/
#define DIAG_SNRGus             "SNRGus" /*vdsl value*/
#define DIAG_SNRpsds 	        "SNRpsds"
#define DIAG_SNRpsus 	        "SNRpsus"
#define DIAG_SNRMTds            "SNRMTds"
#define DIAG_SNRMTus 	        "SNRMTus"				
#define DIAG_BITSpsds           "BITSpsds"
#define DIAG_BITSpsus           "BITSpsus"
#define DIAG_GAINSpsds          "GAINSpsds"

#define DIAG_StateNone           "None"
#define DIAG_StateRequested           "Requested"
#define DIAG_StateComplete           "Complete"
#define DIAG_StateErrorInternal           "Error_Internal"
#define DIAG_StateErrorOther           "Error_Other"
#define DIAG_InProgress           "InProgress"


#define RLCM_GET_DIAG_QLN			(RLCM_IOC_MAGIC +87)
#define RLCM_GET_DIAG_HLOG			(RLCM_IOC_MAGIC +88)
#define RLCM_GET_DIAG_SNR			(RLCM_IOC_MAGIC +89)
#define RLCM_START_DIAGNOSTIC		(RLCM_IOC_MAGIC +170)
#define RLCM_DIAGNOSTIC_STATE		(RLCM_IOC_MAGIC +171)
#define RLCM_GET_DIAG_HLINSCALE		(RLCM_IOC_MAGIC +172)
#define RLCM_GET_DIAG_HLINPS	    (RLCM_IOC_MAGIC +173)
#define RLCM_GET_DIAG_ACTATP	    (RLCM_IOC_MAGIC +174)
#define RLCM_GET_DIAG_ACTPSD	    (RLCM_IOC_MAGIC +175)
#define RLCM_GET_DIAG_BITS	        (RLCM_IOC_MAGIC +176)
#define RLCM_GET_DIAG_GAINS		    (RLCM_IOC_MAGIC +177)
#define RLCM_GET_DIAG_LATN          (RLCM_IOC_MAGIC +178)
#define RLCM_GET_DIAG_SATN          (RLCM_IOC_MAGIC +179)

#define DSLDIAG_BASE_PATH "InternetGatewayDevice.WANDevice.{i}.WANDSLDiagnostics."

#define DSLDIAG_BASE_PATH_DSL "InternetGatewayDevice.WANDevice.1.WANDSLDiagnostics."


/*
正则定义
*/
#define DSLDIAG_REGEX_PREFIX                "^InternetGatewayDevice\\.WANDevice\\.1+\\.WANDSLDiagnostics\\."

#define DSLDIAG_REGEX_DIAG(node)           DSLDIAG_REGEX_PREFIX node "$"


#define WAN_DSL_DIAG_TIMEOUT 600
#define WAN_DSL_DIAG_TIMER_INTERNAL 3

#define MAX_DSL_DIAG_RECORD_LEN 64

#define DSL_DIAG_REQUESTED 0 
#define DSL_DIAG_COMPLETE  1 
#define DSL_DIAG_NONE      2



typedef struct DIAGACTPSD {
  int ACTPSDds;  
  int ACTPSDus; 
}diag_actpsd;

typedef struct DIAGACTATP {
  float ACTATPds;
  float ACTATPus;
}diag_actatp;

typedef struct DIAGHLINSCALE {
  unsigned short HLINSCALE_ds;
  unsigned short HLINSCALE_us;
}diag_hlinscale;

typedef struct DIAGHLINPSD {
  float HLINPSD_real;
  float HLINPSD_image;
}diag_hlinpsd;

typedef struct DIAGLATN {
  unsigned short LATNpbds;
  unsigned short LATNpbus;
}diag_latnpb;

typedef struct DIAGSATN {
  unsigned short SATNds;
  unsigned short SATNus;
}diag_satn;

#define TONE_RANGE 512



/******************************************************************************
*                                FUNCTION
******************************************************************************/


/* Init && Destroy */
int DIAG_ModuleInit(void);
int DIAG_ModuleDestroy(void);

#endif

