#ifndef _PKTLOG_H_
#define _PKTLOG_H_

#include "adf_nbuf.h"

struct ath_softc;

#ifdef DEBUG_PKTLOG
#define CUR_PKTLOG_VER          10008  /* Packet log version */
#define PKTLOG_MAGIC_NUM        7735225


struct ath_pktlog_funcs {
	a_status_t (* pktlog_ioctl)(adf_drv_handle_t drv_hdl, adf_net_wcmd_type_t cmd, 
                            adf_net_wcmd_data_t *data);
	void (* pktlog_rx)(void *context, adf_nbuf_t skb);
};


#define ath_pktlog_mod_inserted (g_pktlog_funcs)

#define ath_pktlog_ioctl(hdl, cmd, data) \
        	g_pktlog_funcs->pktlog_ioctl(hdl, cmd, data)

#define ath_pktlog_rx(ctx, skb) \
        	g_pktlog_funcs->pktlog_rx(ctx, skb)


#define ath_pktlog_ioctl_get_data()\
    do{\
	ubytes = sizeof(adf_net_wcmd_pktlog_read_t);\
	uptr = data.d_pktlog_read;\
    }while(0)

#define ath_pktlog_connect_service(fn)\
    do{\
	connect.ServiceID = WMI_PKTLOG_SVC;\
	connect.EpCallbacks.EpRecv = fn; \
	connect.EpCallbacks.pContext = sc;\
        status = HTCConnectService(sc->host_htc_handle, &connect, &sc->pktlog_ep);\
	if (A_FAILED(status)) {\
	    adf_os_print("Failed to connect services!");\
	}\
	else \
	    adf_os_print("\nPKTLOG conected");\
    }while(0)



#else
#define ath_pktlog_mod_inserted (0)

#define ath_pktlog_ioctl(hdl, cmd, data)  -A_STATUS_EINVAL

#define ath_pktlog_ioctl_get_data() (ubytes = 0)

/*Just to keep the compiler happy */
#define ath_pktlog_connect_service(fn) \
    do{\
	connect.EpCallbacks.EpRecv = fn;\
    }while(0)

	#define ath_pktlog_rx(ctx, skb) 
#endif

#endif                          /* _PKTLOG_H_ */
