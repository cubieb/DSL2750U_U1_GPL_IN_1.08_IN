#ifndef ATHCONF_H
#define ATHCONF_H

/*
Permissions for creating proc entries
The procfs entries to configure, enable ath_athconf
are in /proc/sys/ath_systats
The results are written to /proc/ath_systats/report
*/
#define ATHCONF_PROC_PERM        0444
#define ATHCONF_PROCSYS_DIR_PERM 0555
#define ATHCONF_PROCSYS_PERM     0644
#define ATHCONF_PROC_DIR	"athconf"
#define CTL_AUTO    -2  /* cannot be CTL_ANY or CTL_NONE */

#define ATHCONF_SYSCTL_DECL(f, ctl, write, filp, buffer, lenp, ppos) \
    f(ctl_table *ctl, int write, struct file *filp, void *buffer,\
        size_t *lenp, loff_t *ppos)

/* Deal with the sysctl handler api changing */
#include <linux/version.h>

#if LINUX_VERSION_CODE < KERNEL_VERSION(2, 6, 8)
#define ATHCONF_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer, lenp, ppos) \
    proc_dointvec(ctl, write, filp, buffer, lenp)
#else
#define ATHCONF_SYSCTL_PROC_DOINTVEC(ctl, write, filp, buffer, lenp, ppos) \
    proc_dointvec(ctl, write, filp, buffer, lenp, ppos)
#endif

#define ATHCONF_MAXDEV 10
#define WLANDEV_BASENAME "wifi"
#define MAX_WLANDEV 10
#define ATHCONF_DEVNAME_SIZE 10
#define ATHCONF_DIRNAME_SIZE 200
#define ATHCONF_MAXSYSCTLS 46

#define TEMP_POWER_CAP 1


#endif

