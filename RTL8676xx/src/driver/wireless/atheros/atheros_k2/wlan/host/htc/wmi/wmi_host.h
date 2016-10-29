#ifndef _WMI_HOST_H_
#define _WMI_HOST_H_

#define WMI_EVT_QUEUE_SIZE 256


typedef void (*wmi_rsp_callback_t)(void *, WMI_COMMAND_ID, a_uint16_t, adf_nbuf_t, a_uint8_t *, a_uint32_t);

struct wmi_t {
    void                     *devt;
    adf_os_handle_t          os_hdl;
    HTC_HANDLE               HtcHandle;
    adf_os_spinlock_t        wmi_lock;
    a_uint32_t                 wmi_flags;
#ifdef MAGPIE_SINGLE_CPU_CASE
    adf_os_spinlock_t        wmi_op_lock;
    a_uint32_t                 wmi_op_flags;
#else
    //adf_os_mutex_t           wmi_op_mutex;
    adf_os_spinlock_t        wmi_op_lock;
#endif
    HTC_ENDPOINT_ID          wmi_endpoint_id;
    wmi_event_callback_t     wmi_cb;
    a_uint16_t               txSeqId;
    a_uint16_t               wmi_stop_flag;

#ifdef MAGPIE_SINGLE_CPU_CASE
#else
    htc_spin_t            spin;
#endif
    a_uint8_t             *cmd_rsp_buf;
    a_uint32_t            cmd_rsp_len;
    WMI_COMMAND_ID           wmi_last_sent_cmd;
};

#define LOCK_WMI(w)     adf_os_spin_lock_irq(&(w)->wmi_lock, &(w)->wmi_flags);
#define UNLOCK_WMI(w)   adf_os_spin_unlock_irq(&(w)->wmi_lock, &(w)->wmi_flags);
#endif /* !_WMI_HOST_H_ */
