/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.         
 文件名称:ippmngr.h                                                   
 文件描述:                                           
 修订记录:                                                             
         1 创建:雷达                                                   
           日期:2009.04.28                                                  
           描述:                                                      
                                                                       
**********************************************************************/

#ifndef	__IPPMNGR_H__
#define	__IPPMNGR_H__

#define	IPPD_CONFIG_FILE	"/var/printers.ini"
#define	PRINTER_DEV "/dev/usb/lp/lp0"
#define	PRINTER_MAKE "HP"

//#define	IPPD_DEBUG

#ifdef TBS_TRACE
#undef TBS_TRACE
#endif
#define TBS_TRACE(mid, fmt, args...) \
    printf("%04x %s:%25s:%4d "fmt, \
            mid, __FILE__, __func__, __LINE__, ##args);

#ifdef IPPD_DEBUG
#define IPPD_TRACE(fmt, args...)        TBS_TRACE(MID_IPPD, fmt "\n", ##args)
#else
#define IPPD_TRACE(fmt, args...)
#endif
#define IPPD_TRACE_SUCCESS(fmt, args...) IPPD_TRACE(GREEN INFO_SUCCESS NONE fmt, ##args)
#define IPPD_TRACE_ERROR(fmt, args...) IPPD_TRACE(LIGHT_RED INFO_ERROR NONE fmt, ##args)

#define	CUST_NODE_NAME_PREFIX "X_TWSZ-COM_"
#define	IPPD_PATH	"InternetGatewayDevice."CUST_NODE_NAME_PREFIX"IPPD."
#define	NODE_IPPD_ENABLE	"IppdEnable"
#define	NODE_IPPD_PRNTNAME	"PrinterName"
#define	NODE_IPPD_MAKEMODEL	"MakeModel"
#define	NODE_IPPD_MANUFACTURER	"Manufacturer"
#define	NODE_IPPD_MODEL	"Model"
#define	NODE_IPPD_CMD		"CMD"
#define	NODE_IPPD_FWVER	"FWVer"
#define	NODE_IPPD_UNKNOWN_OPTIONS	"UnknownOptions"

#define IPPD_NODE_LENGTH	256

#define IPPD_MODULE "[IPPD: ]"

#define IPPD_PRINTF(fmt, args...)  printf(IPPD_MODULE "[ERROR]" "%s: %d: " fmt, __FUNCTION__, __LINE__, ##args)
#define IPPD_GOTO(iRet,strError,gotoTag)         \
      {\
        if ( RET_FAILED(iRet) )    \
        {   \
            IPPD_PRINTF("%s, %d %s \n\r", __func__, __LINE__, strError); \
            goto gotoTag; \
        }\
      }

#endif
