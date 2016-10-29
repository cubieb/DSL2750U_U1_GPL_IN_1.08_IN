/*
 * Copyright (c) 2002-2005 Atheros Communications, Inc.
 * All rights reserved.
 */

/**
 * @defgroup athcfg Atheros Configuration Tool
 * @{
 */ 

#ifndef _ATHCFG_H
#define _ATHCFG_H
/**
 * @brief Max characters in a command name.
 */ 
#define ATHCFG_MAX_CMD_NAME  32
/**
 * @brief Max characters in a usage string.
 */ 
#define ATHCFG_MAX_CMD_USAGE 128

/**
 * @brief Maximum Number of IOCTL commands supported
 */
#define ATHCFG_MAX_CMDS			70	

typedef enum {
    FALSE,
    TRUE
}a_bool_t;

#if 0
typedef struct dirent {
    int d_fileno;
    int d_reclen;
    int d_type;
    int d_namlen;
#define MAXNAMLEN    255
    char d_name[MAXNAMLEN + 1] ;
}dirent;
#endif
/**
 * @brief Prototype for command handler.
 *
 * @param[in]   nargs       Number of args for this command.
 * @param[in]   args        List of args for this command.
 *
 * @retval Zero indicates success, otherwise failure.
 */ 
typedef int (*athcfg_cmd_hdlr_t)(char *ifname, int nargs, char *args[]);

/**
 * @brief All commands follow this template.
 */ 
typedef struct athcfg_cmd_s {
    char    cmd_name[ATHCFG_MAX_CMD_NAME];   /**< Name of command */
    a_bool_t    handler_present;                 /**< Handled at this level? */
    union {
        struct athcfg_cmd_hdlr_s {
            athcfg_cmd_hdlr_t   fn;          /**< Set if handler_present */
            char    cmd_usage[ATHCFG_MAX_CMD_USAGE];    /**< Usage string */
            int     min_args;                /**< Minimum required args */
            int     max_args;                /**< Maximum possible args */
        } hdlr;
        struct athcfg_cmd_s * next_level;  /**< Set if !handler_present */
    } u;
} athcfg_cmd_t;

#define h_min_args  u.hdlr.min_args
#define h_max_args  u.hdlr.max_args
#define h_cmd_usage u.hdlr.cmd_usage
#define h_fn        u.hdlr.fn


int 
athcfg_ssid_get(char *ifname, int nargs , char *args[]);

int 
athcfg_ssid_set(char *ifname, int nargs, char *args[]);

int
athcfg_rts_get(char *ifname, int nargs, char *args[]);

int
athcfg_rts_set(char *ifname, int nargs , char *args[]);

int
athcfg_fragthres_get(char *ifname, int nargs, char *args[]);

int
athcfg_fragthres_set(char *ifname, int nargs , char *args[]);

int 
athcfg_bssid_get(char *ifname, int nargs, char *args[]);

int 
athcfg_bssid_set(char *ifname, int nargs, char *args[]);

int 
athcfg_nick_get(char *ifname, int nargs, char *args[]);

int 
athcfg_nick_set(char *ifname, int nargs, char *args[]);

int 
athcfg_freq_get(char *ifname, int nargs, char *args[]);

int 
athcfg_freq_set(char *ifname, int nargs, char *args[]);

int 
athcfg_txpower_get(char *ifname, int nargs, char *args[]);

int 
athcfg_txpower_set(char *ifname,int nargs, char *args[]);

int 
athcfg_param_get(char *ifname, int nargs, char *args[]);

int 
athcfg_param_set(char *ifname, int nargs, char *args[]);

int 
athcfg_optie_get(char *ifname, int nargs, char *args[]);

int 
athcfg_optie_set(char *ifname, int nargs, char *args[]);

int 
athcfg_appie_get(char *ifname, int nargs, char *args[]);

int 
athcfg_appie_set(char *ifname, int nargs, char *args[]);

int 
athcfg_key_get(char *ifname, int nargs, char *args[]);

int 
athcfg_key_set(char *ifname, int nargs, char *args[]);

int 
athcfg_scan_get(char *ifname, int nargs, char *args[]);

int 
athcfg_scan_set(char *ifname, int nargs, char *args[]);

int 
athcfg_bmode_get(char *ifname, int nargs, char *args[]);

int 
athcfg_bmode_set(char *ifname, int nargs, char *args[]);

int 
athcfg_chanlist_get(char *ifname, int nargs, char *args[]);

int 
athcfg_chanlist_set(char *ifname, int nargs, char *args[]);

int 
athcfg_wmmparam_get(char *ifname, int nargs, char *args[]);

int 
athcfg_wmmparam_set(char *ifname, int nargs, char *args[]);

int 
athcfg_name_get(char *ifname, int nargs, char *args[]);

int
athcfg_name_set(char *ifname, int nargs, char *args[]);

int 
athcfg_iccaps_get(char *ifname, int nargs, char *args[]);

int
athcfg_iccaps_set(char *ifname, int nargs, char *args[]);

int 
athcfg_retries_get(char *ifname, int nargs, char *args[]);

int 
athcfg_retries_set(char *ifname, int nargs, char *args[]);

int 
athcfg_waplist_get(char *ifname, int nargs, char *args[]);

int 
athcfg_waplist_set(char *ifname, int nargs, char *args[]);

int 
athcfg_addbastat_get(char *ifname, int nargs, char *args[]);

int
athcfg_addbastat_set(char *ifname, int nargs, char *args[]);

int 
athcfg_chaninfo_get(char *ifname, int nargs, char *args[]);

int
athcfg_chaninfo_set(char *ifname, int nargs, char *args[]);

int 
athcfg_wpaie_get(char *ifname, int nargs, char *args[]);

int
athcfg_wpaie_set(char *ifname, int nargs, char *args[]);

int 
athcfg_wscie_get(char *ifname, int nargs, char *args[]);

int
athcfg_wscie_set(char *ifname, int nargs, char *args[]);

int 
athcfg_txpowlimit_set(char *ifname, int nargs, char *args[]);

int
athcfg_txpowlimit_get(char *ifname, int nargs, char *args[]);

int 
athcfg_athcap_set(char *ifname, int nargs, char *args[]);

int
athcfg_athcap_get(char *ifname, int nargs, char *args[]);

int 
athcfg_turbo_set(char *ifname, int nargs, char *args[]);

int
athcfg_turbo_get(char *ifname, int nargs , char *args[]);

int
athcfg_filter_set(char *ifname, int nargs, char *args[]);

int 
athcfg_filter_get(char *ifname, int nargs, char *args[]);

int 
athcfg_ADDBAresp_set(char *ifname, int nargs, char *args[]);

int
athcfg_ADDBAresp_get(char *ifname, int nargs, char *args[]);

int 
athcfg_mlme_set(char *ifname, int nargs, char *args[]);

int
athcfg_mlme_get(char *ifname, int nargs, char *args[]);

int 
athcfg_sendADDBA_set(char *ifname, int nargs, char *args[]);

int
athcfg_sendADDBA_get(char *ifname, int nargs, char *args[]);

int 
athcfg_sendDELBA_set(char *ifname, int nargs, char *args[]);

int
athcfg_sendDELBA_get(char *ifname, int nargs, char *args[]);

int 
athcfg_delkey_set(char *ifname, int nargs, char *args[]);

int
athcfg_delkey_get(char *ifname, int nargs, char *args[]);

int 
athcfg_delmac_set(char *ifname, int nargs, char *args[]);

int
athcfg_delmac_get(char *ifname, int nargs, char *args[]);

int 
athcfg_addmac_set(char *ifname, int nargs, char *args[]);

int
athcfg_addmac_get(char *ifname, int nargs, char *args[]);

int 
athcfg_opmode_get(char *ifname, int nargs, char *args[]);

int
athcfg_opmode_set(char *ifname, int nargs, char *args[]);

int
athcfg_vapdelete_set(char *ifname, int nargs, char *args[]);

int 
athcfg_sopmode_set(char *ifname, int nargs, char *args[]);

int
athcfg_sopmode_get(char *ifname, int nargs, char *args[]);

int 
athcfg_athstats_get(char *ifname, int nargs, char *args[]);

int
athcfg_athstats_set(char *ifname, int nargs, char *args[]);

int 
athcfg_athdiag_get(char *ifname, int nargs, char *args[]);

int
athcfg_athdiag_set(char *ifname, int nargs, char *args[]);

int 
athcfg_phyerr_get(char *ifname, int nargs, char *args[]);

int
athcfg_phyerr_set(char *ifname, int nargs, char *args[]);

int 
athcfg_athcwm_get(char *ifname, int nargs, char *args[]);

int
athcfg_athcwm_set(char *ifname, int nargs, char *args[]);

int 
athcfg_ethtool_get(char *ifname, int nargs, char *args[]);

int
athcfg_ethtool_set(char *ifname, int nargs, char *args[]);

int 
athcfg_vapcreate_set(char *ifname, int nargs, char *args[]);

int
athcfg_vapcreate_get(char *ifname, int nargs, char *args[]);

int 
athcfg_stats_get(char *ifname, int nargs, char *args[]);

int
athcfg_stats_set(char *ifname, int nargs, char *args[]);

int 
athcfg_athmac_get(char *ifname, int nargs, char *args[]);

int
athcfg_athmac_set(char *ifname, int nargs, char *args[]);

int 
athcfg_mtu_set(char *ifname, int nargs, char *args[]);

int
athcfg_mtu_get(char *ifname, int nargs, char *args[]);

int 
athcfg_sysctl_get(char *ifname, int nargs, char *args[]);

int 
athcfg_txtimeout_set(char *ifname, int nargs, char *args[]);

int 
athcfg_txtimeout_get(char *ifname, int nargs, char *args[]);

int 
athcfg_modeinit_set(char *ifname, int nargs, char *args[]);

int 
athcfg_modeinit_get(char *ifname, int nargs, char *args[]);

int 
athcfg_statsclear_get(char *ifname, int nargs , char *args[]);

int 
athcfg_statsclear_set(char *ifname, int nargs, char *args[]);

int 
athcfg_getrange_get(char *ifname, int nargs, char *args[]);

int 
athcfg_getrange_set(char *ifname, int nargs, char *args[]);

int
athcfg_power_get(char *ifname, int nargs, char *args[]);

int
athcfg_power_set(char *ifname, int nargs, char *args[]);

int
athcfg_mode_get(char *ifname, int nargs, char *args[]);

int
athcfg_mode_set(char *ifname, int nargs, char *args[]);

int
athcfg_statsinfo_get(char *ifname, int nargs, char *args[]);

int
athcfg_stainfo_get(char *ifname, int nargs, char *args[]);

int
athcfg_stastats_get(char *ifname, int nargs, char *args[]);

int
athcfg_statsinfo_set(char *ifname, int nargs, char *args[]);

int
athcfg_stainfo_set(char *ifname, int nargs, char *args[]);

int
athcfg_stastats_set(char *ifname, int nargs, char *args[]);

int
athcfg_eifsmask_get(char *ifname, int nargs, char *args[]);

int
athcfg_eifsmask_set(char *ifname, int nargs, char *args[]);

int
athcfg_eifsdur_get(char *ifname, int nargs, char *args[]);

int
athcfg_eifsdur_set(char *ifname, int nargs, char *args[]);

int
athcfg_slottime_get(char *ifname, int nargs, char *args[]);

int
athcfg_slottime_set(char *ifname, int nargs, char *args[]);

int
athcfg_acktimeout_get(char *ifname, int nargs, char *args[]);

int
athcfg_acktimeout_set(char *ifname, int nargs, char *args[]);

int
athcfg_ctstimeout_get(char *ifname, int nargs, char *args[]);

int
athcfg_ctstimeout_set(char *ifname, int nargs, char *args[]);

int
athcfg_softled_get(char *ifname, int nargs, char *args[]);

int
athcfg_softled_set(char *ifname, int nargs, char *args[]);

int
athcfg_ledpin_get(char *ifname, int nargs, char *args[]);

int
athcfg_ledpin_set(char *ifname, int nargs, char *args[]);

int
athcfg_debug_get(char *ifname, int nargs, char *args[]);

int
athcfg_debug_set(char *ifname, int nargs, char *args[]);

int
athcfg_tx_antenna_get(char *ifname, int nargs, char *args[]);

int
athcfg_tx_antenna_set(char *ifname, int nargs, char *args[]);

int
athcfg_rx_antenna_get(char *ifname, int nargs, char *args[]);

int
athcfg_rx_antenna_set(char *ifname, int nargs, char *args[]);

int
athcfg_diversity_get(char *ifname, int nargs, char *args[]);

int
athcfg_diversity_set(char *ifname, int nargs, char *args[]);

int 
athcfg_tx_intrperiod_get(char *ifname, int nargs, char *args[]);

int 
athcfg_tx_intrperiod_set(char *ifname, int nargs, char *args[]);

int
athcfg_txq_min_get(char *ifname, int nargs, char *args[]);

int
athcfg_txq_min_set(char *ifname, int nargs, char *args[]);

int
athcfg_tkipmic_get(char *ifname, int nargs, char *args[]);

int
athcfg_tkipmic_set(char *ifname, int nargs, char *args[]);

int
athcfg_glob_txtimeout_get(char *ifname, int nargs, char *args[]);

int
athcfg_glob_txtimeout_set(char *ifname, int nargs, char *args[]);

int
athcfg_sw_wscbutton_get(char *ifname, int nargs, char *args[]);

int
athcfg_sw_wscbutton_set(char *ifname, int nargs, char *args[]);

int
athcfg_country_code_get(char *ifname, int nargs, char *args[]);

int
athcfg_reg_domain_get(char *ifname, int nargs, char *args[]);

int
athcfg_scandelay_get(char *ifrn_name, int nargs, char *args[]);

int
athcfg_scandelay_set(char *ifrn_name, int nargs, char *args[]);
    
/**
 * Device Statistics
 */
int athcfg_hst_stats_get(char *ifname, int nargs, char *args[]);
int athcfg_hst_11n_stats_get(char *ifname, int nargs, char *args[]);
int athcfg_tgt_stats_get(char  ifname, int nargs, char *args[]);
int athcfg_tgt_11n_stats_get(char  *ifname, int nargs, char *args[]);


/**
 * Pktlog handlers
 */
int athcfg_pktlog_enable(char *ifrn_name, int nargs, char *args[]);
int athcfg_pktlog_disable(char *ifrn_name, int nargs, char *args[]);
int athcfg_pktlog_set_cmd(char *ifrn_name, int nargs, char *args[]);
int athcfg_pktlog_get_data(char *ifrn_name, int nargs, char *args[]);

/**
 * @brief User specified debug routines
 */ 
int     athcfg_dbg_info_get(char *ifname, int nargs, char *args[]);
int     athcfg_dbg_info_set(char *ifname, int nargs, char *args[]);

int     athcfg_reg_get(char *ifname, int nargs, char *args[]);
int     athcfg_reg_set(char *ifname, int nargs, char *args[]);

int athcfg_nominal_nf_get(char *ifname, int nargs, char *args[]);
int athcfg_nominal_nf_set(char *ifname, int nargs, char *args[]);
int athcfg_minimum_nf_get(char *ifname, int nargs, char *args[]);
int athcfg_minimum_nf_set(char *ifname, int nargs, char *args[]);
int athcfg_maximum_nf_get(char *ifname, int nargs, char *args[]);
int athcfg_maximum_nf_set(char *ifname, int nargs, char *args[]);
int athcfg_nf_delta_get(char *ifname, int nargs, char *args[]);
int athcfg_nf_delta_set(char *ifname, int nargs, char *args[]);
int athcfg_nf_weight_get(char *ifname, int nargs, char *args[]);
int athcfg_nf_weight_set(char *ifname, int nargs, char *args[]);
int athcfg_nf_switch_get(char *ifname, int nargs, char *args[]);
int athcfg_nf_switch_set(char *ifname, int nargs, char *args[]);
/* Some sanity check values for noisefloor */
#define MAX_NOISE_FLOOR_VALUE (-20)
#define MIN_NOISE_FLOOR_VALUE (-128)

/**
 * Ether-Dongle-MAC handlers
 */
int athcfg_ether_dongle_mac_set_cmd(char *ifrn_name, int nargs, char *args[]);
int athcfg_ether_dongle_mac_get_cmd(char *ifrn_name, int nargs, char *args[]);

#endif /* _ATHCFG_H */

/**
 * @}
 */ 
