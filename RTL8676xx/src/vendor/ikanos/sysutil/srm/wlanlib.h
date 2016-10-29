/* Copyright (C) 2006 Ikanos Communications
 * See IKANOS_PROP_LICENSE.txt for license information. */

# include "../../../../adiwlan/inc/wlan_osif_nmapi.h"

# define WLAN_MAX_FLT_LST     23
# define MAX_IFACE             8    // max interfaces
# define WLAN_MAX_BSSID        8    // defined in adiwlan/drv/wlan_dev.h

struct wlan_data
{
    int      index[WLAN_MAX_BSSID];
    int      cnt_bss;
    int      bss_index;
    apcfg_ls apcfg;
    apwep_ls apwep;
    apwpa_ls apwpa;
    int      cnt_flt; // number of entries in filter table
    apflt_ls apflt[WLAN_MAX_FLT_LST];
};

