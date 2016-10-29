/* ----------------------------------------------------------------------------
 * Copyright (C), 1988-2009, Shenzheng T&W Tech. Co., Ltd. 
 * File Name: s3g_dev_def.h
 * Author: Zhao Yanming (zhaoyanming@twsz.com)     
 * Version: V1.0     
 * Date: 08/13/2009
 * --------------------------------------------------------------------------*/

#ifndef _S3G_DEV_DEF_H_
#define _S3G_DEV_DEF_H_

/* device feature */
#define S3G_FEATURE_SING_CH              0x00000000
#define S3G_FEATURE_DUAL_CH              0x00000001

/* device flag */
#define S3G_FLAG_UNDEF                   0x00000000

/* ppp dial type */
#define S3G_DIAL_AUTO                             0
#define S3G_DIAL_MANUAL                           1
#define S3G_DIAL_PPPOEPROXY                       2
#define S3G_DIAL_HTTP                             3
#define S3G_DIAL_ONDEMAND                         4
#define S3G_DIAL_ONTIME                           5

/* signal status */
#define S3G_SIGNAL_STEP0                          0
#define S3G_SIGNAL_STEP1                          1
#define S3G_SIGNAL_STEP2                          2
#define S3G_SIGNAL_STEP3                          3
#define S3G_SIGNAL_STEP4                          4
#define S3G_SIGNAL_STEP5                          5

/* 3g net protocol */
#define S3G_PROTO_UNDEFINE                        0
#define S3G_PROTO_TDSCDMA                         1
#define S3G_PROTO_WCDMA                           2
#define S3G_PROTO_CDMA2K                          3

#if 0
/* cdma2000 net type */
#define S3G_MODE_CDMA1X                           0
#define S3G_MODE_EVDO                             1
#define S3G_MODE_AUTO                             2

/* td-scdma*/
#define TS3G_MODE_AUTO 							  0
#define TS3G_MODE_EDGE                             1
#define TS3G_MODE_3G                               2
#endif

/* sim card status */
#define S3G_SIM_STATUS_EN                         1
#define S3G_SIM_STATUS_240                        240
#define S3G_SIM_STATUS_255                        255

/* message for cfm and s3gmngr process */
#define S3G_MSG_RESET_CARD                        0x0001

enum{	
RT3G_DIAL_AUTO=0,	
RT3G_DIAL_MANUAL,	
RT3G_DIAL_PPPOEPROXY,	
RT3G_DIAL_HTTP,	
RT3G_DIAL_ONDEMAND,	
RT3G_DIAL_ONTIME
};

enum{		
RT3G_MODE_CDMA1X=0,	
RT3G_MODE_EVDO,	
RT3G_MODE_AUTO
};

enum{		
RT3G_TD_RATE_AUTO=0,	
RT3G_TD_RATE_2M,	
RT3G_TD_RATE_1M,	
RT3G_TD_RATE_384K
};

enum{		
RT3G_NETORK_MODE_CDMA1X=0,	
RT3G_NETORK_MODE_EVDO
};

enum{	
RT3G_SIG_STEP0=0,	
RT3G_SIG_STEP1=1,	
RT3G_SIG_STEP2=2,	
RT3G_SIG_STEP3=3,	
RT3G_SIG_STEP4=4,	
RT3G_SIG_STEP5=5
};

/* 3g device */
typedef struct
{
    char *ps_vdr_name;
    char *ps_pdt_name;
    char *ps_data_itf;
    char *ps_ctl_itf;
    unsigned short ui_vid;
    unsigned short ui_pid;
    int i_3g_proto;
    unsigned int ui_feature;
    unsigned int ui_flag;
}s3g_device_t;

#endif /* _S3G_DEV_DEF_H_ */
