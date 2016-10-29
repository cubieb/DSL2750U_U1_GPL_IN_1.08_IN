#include <linux/kernel.h>
#include <linux/init.h>
#include "rtk_voip.h"

#if defined (CONFIG_AUDIOCODES_VOIP) && defined (CONFIG_RTK_VOIP_DRIVERS_IP_PHONE) || defined (CONFIG_RTK_VOIP_DECT_UART_SUPPORT)
int rtk_dbg_level = RTK_DBG_ERROR;
#else
int rtk_dbg_level = RTK_DBG_INFO;
#endif

static int __init rtk_voip_init(void)
{
	PRINT_MSG("rtk_voip_init\n");
	return 0;
}

static void __exit rtk_voip_exit(void)
{
	PRINT_MSG("rtk_voip_exit\n");
}

#ifdef CONFIG_RTK_VOIP_DRIVERS_11N_MP
int PCM_Check_Any_ChanEnabled(void)
{
	return 0;
}
#endif

voip_initcall(rtk_voip_init);
voip_exitcall(rtk_voip_exit);

