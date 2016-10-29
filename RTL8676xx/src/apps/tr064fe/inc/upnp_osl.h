/*
 * OS Independent Layer
 * 
 * $Copyright Open Broadcom Corporation$   
 * $Id: upnp_osl.h,v 1.8.32.1 2003/10/16 23:12:07 mthawani Exp $
 */

#ifndef _upnp_osl_h_
#define _upnp_osl_h_

#if defined(vxworks)
#include <vx_osl.h>
#elif defined(linux)
#include <linux_osl.h>
#else
#error "Unsupported OSL requested"
#endif
#include "typedefs.h"

/* forward declaration - defined in upnp.h */
struct _if_stats;


typedef enum { OSL_LINK_DOWN = 0, OSL_LINK_UP = 1 } osl_link_t;

extern struct in_addr *osl_ifaddr(const char *, struct in_addr *);
extern int osl_ifstats(char *, struct _if_stats *);
extern void osl_sys_restart(void);
extern void osl_sys_reboot(void);
extern osl_link_t osl_link_status(char *devname);
extern uint osl_max_bitrates(char *devname, ulong *rx, ulong *tx);
extern void osl_sys_restart(void);
extern void osl_sys_reboot(void);
extern void osl_igd_disable(void);
extern void osl_igd_enable(void);
extern bool osl_wan_isup(char *devname);
extern bool osl_lan_isup(char *devname);
extern bool osl_set_macaddr(char *devname, char spoofed[]);

#endif	/* _upnp_osl_h_ */
