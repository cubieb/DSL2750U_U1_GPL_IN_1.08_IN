#ifndef _AG7100_PHY_H
#define _AG7100_PHY_H

/* modefine for Atheros8216 */
#define CONFIG_ATHRS26_PHY

#define phy_reg_read        ag7100_mii_read
#define phy_reg_write       ag7100_mii_write

#ifndef CONFIG_ATHRS26_PHY
#define ag7100_phy_ioctl(unit, args)
#endif

#include "ag7100.h"



/* ecos will set the value of CYGNUM_USE_ENET_PHY to one of the following strings
 * based on the cdl. These are defined here in no particuilar way so the
 * #if statements that follow will have something to compare to.
 */

#include "athrs26_phy.h"

#define in_interrupt(x)    0
#define schedule_work(x)
#define INIT_WORK(x,y)



#define ag7100_phy_ioctl(unit, args)    athr_ioctl(unit,args)
#define ag7100_phy_setup(unit)          athrs26_phy_setup (unit)
#define ag7100_phy_is_up(unit)          athrs26_phy_is_up (unit)
#define ag7100_phy_speed(unit)          athrs26_phy_speed (unit)
#define ag7100_phy_is_fdx(unit)         athrs26_phy_is_fdx (unit)
#define ag7100_phy_is_lan_pkt           athr_is_lan_pkt
#define ag7100_phy_set_pkt_port         athr_set_pkt_port
#define ag7100_phy_tag_len              ATHR_VLAN_TAG_SIZE
#define ag7100_phy_get_counters         athrs26_get_counters

static inline unsigned int 
ag7100_get_link_status(int unit, int *link, int *fdx, ag7100_phy_speed_t *speed)
{
  *link=ag7100_phy_is_up(unit);
  *fdx=ag7100_phy_is_fdx(unit);
  *speed=ag7100_phy_speed(unit);
  return 0;
}

static inline int
ag7100_print_link_status(int unit)
{
  return -1;
}



#endif

