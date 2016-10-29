/* Copyright (C) 2006 Ikanos Communications 
 * See IKANOS_PROP_LICENSE.txt for license information. */



/*****************************************************************************
 * File Name     : commandset.h
 * Author        : Arijit
 * Description   : This file defines a structure of type util_cmds with its 
 *               : values assigned as:- the name of the command that would be
 *               : invoked from the shell, and the name of the function that
 *               : would get called when the command is invoked. Appropriate
 *               : macros are used to selectively enable or disable registration
 *               : of the commands. These macros are the same as in the 
 *               : config.in file. All the commands to be registered should be
 *               : placed before the NULL entry at the end.
 *****************************************************************************/
const util_cmds sysutil_cmds[] = {
	{ "createlink", createlink_main },
					/*-- AP UTILITIES --*/
	{ "ethphy", ethphyrw_main },
#if CONFIG_USER_SYSUTIL_MREADWRITE
	{ "mwrite", mrw_main },
	{ "mread", mrw_main },
#endif
#if CONFIG_USER_SYSUTIL_APREADWRITE
	{ "aread", aprw_main },
	{ "awrite", aprw_main },
        { "apfeature", ap_feature_bit_main },
#endif
#if CONFIG_USER_SYSUTIL_APSTATS
	{ "apstats", apstats_main },
        { "apresetstats", ap_reset_stats_main },
#endif
#if CONFIG_USER_SYSUTIL_APROUTETABLE 
	{ "aproutetbl", aproute_table_main },
#endif

#if CONFIG_FUSIV_USER_ACL_FILTER
        { "acll2", acll2_main },
        { "acll3", acll3_main },
#endif
					/*-- ATM/ADSL Utilities --*/
#if CONFIG_USER_SYSUTIL_ADSLUTILS
	{ "adsl", adsl_main },
#endif
#if CONFIG_USER_SYSUTIL_XDSL_LINE_MNGR
	{ "xdsl_line_mgr", xdsl_line_mgr_main },
#endif
#if CONFIG_USER_SYSUTIL_SENDOAMLB
	{ "sendoamlb", sendoamlb_main },
#endif
#if CONFIG_USER_SYSUTIL_SHOWATMCONN
	{ "showatmconn", showatmconn_main },
        { "bmsession",bmsession_main },
#endif
#if CONFIG_USER_SYSUTIL_VERSION
	{ "version", version_main },
#endif
#if CONFIG_USER_SYSUTIL_GETRMON
        { "getrmon", getrmon_main },
#endif
#if CONFIG_USER_SYSUTIL_GETLINK
	{ "getrmon", getrmon_main },
	{ "getlink", getlink_main },
#endif
#if CONFIG_USER_SYSUTIL_SETLINK
	{ "setlink", setlink_main },
#endif
#if CONFIG_USER_SYSUTIL_SRM
	{ "save", save_main },
	{ "erase", erase_main },
	{ "restore", restore_main },
        { "restore_Fromfile",restore_Fromfile_main},
	{ "pppsrm", pppsrm_main },
        { "ppprestore",ppprestore_main },
#endif
#if CONFIG_USER_ADI_PPPOE_RELAY
	{ "pppoe-relay", pppoerelay_main },
#endif
#if CONFIG_USER_SYSUTIL_IGMPSNOOP
        { "igmpsnoop", igmpsnoop_main },
#endif
#if CONFIG_USER_SYSUTIL_POLICY_VLAN
        { "vlanclassify", vlanclassify_main },
        { "vconfigTxAction", vlanclassify_main },
#endif
#if CONFIG_USER_SYSUTIL_SWITCH_CONFIG
        { "swchvlan", switchVlanConfig },
        { "swchqos", switchQosConfig },
        { "switchconf", switchconf_main },
#endif
#if CONFIG_USER_SYSUTIL_APQOS
  { "qos",apqos_config_main },
#endif
#if CONFIG_FUSIV_USER_TFTPCLIENT
  { "tftp",tftp_main},
#endif
#if CONFIG_USER_SYSUTIL_GPON
  { "gpon",gpon_main},
#endif
#if CONFIG_USER_SYSUTIL_COMPDECOMP
  { "compdecomp",comp_decomp_main},
#endif
#if CONFIG_USER_SYSUTIL_SET_ETH_FRAME_READ_CNT
        { "setethframereadcnt", setethframereadcnt_main },
#endif
#if CONFIG_USER_SYSUTIL_SET_ATM_FRAME_READ_CNT
        { "setatmframereadcnt", setatmframereadcnt_main },
#endif
#if CONFIG_USER_SYSUTIL_SET_PERI_FRAME_READ_CNT
        { "setperiframereadcnt", setperiframereadcnt_main },
#endif
#if CONFIG_USER_SYSUTIL_SET_VDSL_FRAME_READ_CNT
        { "setvdslframereadcnt", setvdslframereadcnt_main },
#endif
	{ "", NULL }			/* ===== KEEP THIS NULL ENTRY ==== */
};
