/* ----------------------------------------------------------------------------
 * Copyright (C), 1988-1999, Shenzheng T&W Tech. Co., Ltd. 
 * File Name: s3gmngr.h
 * Author: Zhao Yanming  (zhaoyanming@twsz.com)     
 * Version: V1.0     
 * Date: 07/29/2009
 * --------------------------------------------------------------------------*/

#ifndef _3GMNGR_H_
#define _3GMNGR_H_

#if defined(__cplusplus)
extern "C" {
#endif

//#define S3G_DEBUG
#ifdef S3G_DEBUG
#define s3gdbg(format, args...) printf("[%d:%s()]==>"format, __LINE__, __FUNCTION__, ##args)
#else
#define s3gdbg(format, args...)
#endif

#define CARD_CHECK_MS  1000
#define CARD_LOCK_FILE  "/var/pppd3g/pin_lock"

/* define variable type */
typedef unsigned char     _u_8;
typedef unsigned short    _u_16;
typedef unsigned int      _u_32;

/* 3g device */
typedef struct s3g_device_t_
{
    char *ps_vdr_name;
    char *ps_pdt_name;
    char *ps_data_itf;
    char *ps_ctl_itf;
    _u_16 ui_vid;
    _u_16 ui_pid;
    int i_3g_proto;
    _u_32 ui_feature;
    _u_32 ui_flag;
}s3g_device_t;

/* The data class of 3g management */
typedef struct s3g_entry_t_
{
    s3g_device_t *p_s3g_device;
    char *ps_product;
    char *ps_mft;
    char *ps_sn;
    char *ps_scpt_inner;
    int i_need_init;
    int i_signal;
    int i_speed;
    int i_net_mode;
    int i_sim_status;
    int i_net_status;
    int i_wan_status;

    int ( *s3g_init )( struct s3g_entry_t_ *p_ent );
    int ( *s3g_check_dev )( struct s3g_entry_t_ *p_ent );
    int ( *s3g_made_script )( struct s3g_entry_t_ *p_ent );
    int ( *s3g_get_signal )( struct s3g_entry_t_ *p_ent );
    int ( *s3g_get_sim_status )( struct s3g_entry_t_ *p_ent );
    int ( *s3g_set_mode )( struct s3g_entry_t_ *p_ent );
    int ( *s3g_set_speed )( struct s3g_entry_t_ *p_ent );
    int ( *s3g_check_net )( struct s3g_entry_t_ *p_ent );
	int ( *s3g_config_card )( struct s3g_entry_t_ *p_ent );
	int ( *s3g_reset_card )( struct s3g_entry_t_ *p_ent );
    int ( *s3g_complete )( struct s3g_entry_t_ *p_ent );
}s3g_entry_t;

/* event and time for monitor */
typedef struct et_motr_t_
{
    int i_event;
    time_t cur_tm;
    time_t sig_tm;
    time_t sim_tm;
    time_t net_tm;
    time_t wan_tm;
}et_motr_t;

/* struct with AT command for device control */
typedef struct s3g_atcc_t_ 
{
    char *ps_at;
    char *p_key;
    int i_res_idx;
}s3g_atcc_t;

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

/* cdma2000 net type */
#define S3G_MODE_CDMA1X                           0
#define S3G_MODE_EVDO                             1
#define S3G_MODE_AUTO                             2

/* td-scdma*/
#define S3G_TDSCDMA_MODE_AUTO 							  0
#define S3G_TDSCDMA_MODE_EDGE                             1
#define S3G_TDSCDMA_MODE_3G                               2


/* sim card status */
#define S3G_SIM_STATUS_EN                         1
#define S3G_SIM_STATUS_LOCK                         2
#define S3G_SIM_STATUS_240                        240
#define S3G_SIM_STATUS_255                        255

/* message for cfm and s3gmngr process */
#define S3G_MSG_RESET_CARD                        0x0001

/* signal file for cfm or other */
#define S3G_SIGNAL_VALUE_FILE                     "/var/pppd3g/s3g_signal"
#define S3G_NETWORK_MODE_FILE                     "/var/pppd3g/s3g_network"
#define S3G_SIM_STATUS_FILE                       "/var/pppd3g/s3g_sim"
#define S3G_PROCESS_MSG_FILE                      "/var/s3gmsg"
#define PATH_CROND                                "/var/spool/cron/crontabs/cpe"
#define S3G_MODEN_INFO_FILE                       "/var/pppd3g/s3g_modeninfo"

//evdo net
enum
{
	EVDO_SYSMODE_NOSERVICE=0,
	EVDO_SYSMODE_AMPS,
	EVDO_SYSMODE_CDMA,
	EVDO_SYSMODE_GSMORGPRS,
	EVDO_SYSMODE_HDR,
	EVDO_SYSMODE_WCDMA,
	EVDO_SYSMODE_GPS,
	EVDO_SYSMODE_GMSORWCDMA,
	EVDO_SYSMODE_CDMAORHDRHYBRID
};
//td net
enum
{
	TD_SYSMODE_NOSERVICE=0,
	TD_SYSMODE_RESERVATION,
	TD_SYSMODE_RESERVATION_1,
	TD_SYSMODE_GSMORGPRS,
	TD_SYSMODE_WCDMA=5,
	TD_SYSMODE_TDSCDMA=15,
};

enum
{
    DATA_CARD_AT_GET_EVDO_SIG = 0,
    DATA_CARD_AT_GET_CDMA1X_SIG,
    DATA_CARD_AT_SET_PREF_NET,
    DATA_CARD_AT_SET_TD_PREF_NET,
    DATA_CARD_AT_GET_WCDMA_SIG,
    DATA_CARD_AT_GET_TDSCDMA_SIG,
    DATA_CARD_AT_SET_TDSCDMA_OPERATOR_MODE,
    DATA_CARD_AT_SET_TDSCDMA_RATE,
    DATA_CARD_AT_GET_NET_MODE,
    DATA_CARD_AT_GET_SYSINFO_SIMST,
    DATA_CARD_AT_GET_CONFIG,
    DATA_CARD_AT_DIAL_DOWN,
    DATA_CARD_AT_RESET,
    DATA_CARD_AT_ESN,//ESN
    DATA_CARD_AT_FRVER,
    DATA_CARD_AT_HDVER ,
    DATA_CARD_AT_IMSI,
    DATA_CARD_AT_VOLT,
    DATA_CARD_AT_CGMI,
    DATA_CARD_AT_CGMM
};

enum
{
    S3G_CARD_INDEX_SIGNAL = 0,
    S3G_CARD_INDEX_MODE,
    S3G_CARD_INDEX_OPERATOR_MODE,
    S3G_CARD_INDEX_RATE,
    S3G_CARD_INDEX_NET_MODE_GET,
    S3G_CARD_INDEX_TEMP,
    S3G_CARD_INDEX_CONFIG,
    S3G_CARD_INDEX_DIAL_DOWN,
    S3G_CARD_INDEX_RESET,
    S3G_CARD_INDEX_3GINFO
};

enum
{	
	RT3G_MODE_CDMA1X=0,
	RT3G_MODE_EVDO,
	RT3G_MODE_AUTO
};

enum
{	
	RT3G_TD_RATE_AUTO=0,
	RT3G_TD_RATE_2M,
	RT3G_TD_RATE_1M,
	RT3G_TD_RATE_384K
};

 enum
{	
	RT3G_TD_UP_RATE_AUTO=0,
	RT3G_TD_UP_RATE_64K,
	RT3G_TD_UP_RATE_128K,
	RT3G_TD_UP_RATE_384K
};

enum
{	
	RT3G_TD_DOWN_RATE_AUTO=0,
	RT3G_TD_DOWN_RATE_64K,
	RT3G_TD_DOWN_RATE_128K,
	RT3G_TD_DOWN_RATE_384K,
	RT3G_TD_DOWN_RATE_1048K,
	RT3G_TD_DOWN_RATE_2048K,
	RT3G_TD_DOWN_RATE_2200K,
	RT3G_TD_DOWN_RATE_2800K
};
enum
{
    DATA_CARD_LINE_ESN = 1,//ESN
    DATA_CARD_LINE_FRVER,
    DATA_CARD_LINE_HDVER ,
    DATA_CARD_LINE_IMSI,
    DATA_CARD_LINE_VOLT,
    DATA_CARD_LINE_CGMI,
    DATA_CARD_ALINE_CGMM
};
int s3gmngr_init();
/*
int s3gmngr_ex_dial_on_manual( void );
int s3gmngr_ex_dial_off_manual( void );
int s3gmngr_ex_dial_on_time( void );
*/
int s3gmngr_ex_get_signal( void );
int s3gmngr_ex_get_network( void );
int s3gmngr_ex_get_sim_status( void );
int s3gmngr_ex_get_sys_mode( void );
int s3gmngr_ex_reset_card( void );
void s3gmngr_ex_get_reset_status ( void );
void card_unlock(void);
int card_lock(void);
int parse_3ginfo_filetostr(int num,char *str);
#if defined(__cplusplus)
}
#endif

#endif

