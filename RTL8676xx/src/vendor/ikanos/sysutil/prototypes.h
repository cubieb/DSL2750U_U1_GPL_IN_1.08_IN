/* Copyright (C) 2006 Ikanos Communications 
 * See IKANOS_PROP_LICENSE.txt for license information. */


/*****************************************************************************
 * File Name     : prototypes.h
 * Author        : Arijit
 * Description   : This file contains function prototype declarations for the 
 *               : function names registered with the sysutil_cmds structure 
 *               : in the file commandset.h. It is essential that any function 
 *               : name registered with sysutil_cmds structure be declared here.
 *****************************************************************************/

extern int createlink_main(int argc, char* argv[]);

		/* FUNCTIONS DEFINED IN FILE aputil.c */

extern int mrw_main(int argc, char *argv[]);
extern int aprw_main(int argc, char *argv[]);
extern int apstats_main(int argc, char *argv[]);
extern int ap_reset_stats_main(int argc, char *argv[]);
extern int aproute_table_main(int argc, char *argv[]);
extern int ap_feature_bit_main(int argc, char *argv[]);
		/* Functions for ACL filtering, defined in acll2Filter.c and acll3Filter.c */
extern int acll2_main(int argc, char *argv[]);
extern int acll3_main(int argc, char *argv[]);
		/* FUNCTIONS DEFINED IN FILE adslatmutil.c */

extern int adsl_main(int argc, char *argv[]);
extern int bmsession_main(int argc, char *argv[]);

		/* FUNCTIONS DEFINED IN FILE sendoamlb.c */

extern int xdsl_line_mgr_main(int argc, char* argv[]);
extern int sendoamlb_main(int argc, char* argv[]);

		/* FUNCTIONS DEFINED IN FILE version.c */

extern int version_main(int argc, char* argv[]);
#if CONFIG_USER_SYSUTIL_GETRMON
extern int getrmon_main(int argc, char* argv[]);
#endif

		/* FUNCTIONS DEFINED IN FILE linkstate.c */

extern int getrmon_main(int argc, char* argv[]);
extern int getlink_main(int argc, char* argv[]);
extern int setlink_main(int argc, char* argv[]);

		/* FUNCTIONS DEFINED IN FILE showatmconn.c */

extern int showatmconn_main(int argc, char* argv[]);

		/* FUNCTIONS DEFINED for SAVE & RESTORE */

extern int erase_main(int argc, char* argv[]);
extern int save_main(int argc, char* argv[]);
extern int restore_main(int argc, char* argv[]);
extern int restore_Fromfile_main(int argc, char* argv[]);
		/* FUNCTIONS DEFINED IN FILE ppprelaycli.c */

extern int pppoerelay_main(int argc, char* argv[]);

		/* FUNCTIONS DEFINED IN FILE pppsrm.c */

extern int pppsrm_main(int argc, char* argv[]);
extern int igmpsnoop_main(int argc, char *argv[]);
extern int switchVlanConfig(int argc, char *argv[]);
extern int switchQosConfig(int argc, char *argv[]);
extern int ppprestore_main(int argc,char* argv[]);
extern int vlanclassify_main(int argc, char *argv[]);
/* FUNCTIONS DEFINED IN FILE qosutil.c */
extern int  apqos_config_main(int argc, char **argv);

/* FUNCTIONS DEFINED IN tftpcli.c */
int tftp_main(int argc, char *argv[]);

/* FUNCTIONS DEFINED IN gponcli.c */
int gpon_main(int argc, char *argv[]);

/* FUNCTIONS DEFINED IN compdecomp.c */
int comp_decomp_main(int argc, char *argv[]);

/* FUNCTIONS DEFINED IN setethframereadcnt.c */
extern int setethframereadcnt_main(int argc, char* argv[]);

/* FUNCTIONS DEFINED IN setatmframereadcnt.c */
extern int setatmframereadcnt_main(int argc, char* argv[]);

/* FUNCTIONS DEFINED IN setperiframereadcnt.c */
extern int setperiframereadcnt_main(int argc, char* argv[]);

/* FUNCTIONS DEFINED IN setvdslframereadcnt.c */
extern int setvdslframereadcnt_main(int argc, char* argv[]);

/* FUNCTIONS DEFINED IN switchconf.c */
extern int switchconf_main(int argc, char **argv);
/*  FUNCTIONS DEFINED IN ethphyrw.c */
extern int ethphyrw_main(int argc, char **argv);

