#ifndef _HIF_H_
#define _HIF_H_

/**
 * @file hif.h
 * HostInterFace layer.
 * Abstracts interface-dependent transmit/receive functionality.
 */ 


#define HIF_MAX_DEVICES             1

typedef void * HIF_HANDLE;

/**
 * @brief List of registration callbacks - filled in by HTC.
 */
struct htc_drvreg_callbacks {
    A_STATUS (* deviceInsertedHandler)(HIF_HANDLE hHIF);
    A_STATUS (* deviceRemovedHandler)(void *instance, a_uint8_t surpriseRemoved);
};

/**
 * @brief List of callbacks - filled in by HTC.
 */ 
struct htc_callbacks {
    void *Context;  /**< context meaningful to HTC */
    A_STATUS (* txCompletionHandler)(void *Context, adf_nbuf_t netbuf);
    A_STATUS (* rxCompletionHandler)(void *Context, adf_nbuf_t netbuf, a_uint8_t pipeID);
#ifdef MAGPIE_SINGLE_CPU_CASE
    void (*wlanTxCompletionHandler)(void *Context, a_uint8_t epnum);
#endif
};


typedef struct htc_drvreg_callbacks HTC_DRVREG_CALLBACKS;
typedef struct htc_callbacks HTC_CALLBACKS;




A_STATUS HIF_register(HTC_DRVREG_CALLBACKS *callbacks);
A_STATUS HIF_deregister(void);

/** 
 * @brief: This API is used by the HTC layer to initialize the HIF layer and to
 * register different callback routines. Support for following events has
 * been captured - DSR, Read/Write completion, Device insertion/removal,
 * Device suspension/resumption/wakeup. In addition to this, the API is
 * also used to register the name and the revision of the chip. The latter
 * can be used to verify the revision of the chip read from the device
 * before reporting it to HTC.
 * @param[in]: callbacks - List of HTC callbacks
 * @param[out]:
 * @return: an opaque HIF handle
 */
//void *HIFInit(void *hHTC, HTC_CALLBACKS *callbacks);

void HIFPostInit(void *HIFHandle, void *hHTC, HTC_CALLBACKS *callbacks);

void HIFStart(HIF_HANDLE hHIF);

/**
 * @brief: Send a buffer to HIF for transmission to the target.
 * @param[in]: dev - HIF handle
 * @param[in]: pipeID - pipe to use
 * @param[in]: netbuf - buffer to send
 * @param[out]:
 * @return: Status of the send operation.
 */ 
A_STATUS HIFSend(void *dev, a_uint8_t PipeID, adf_nbuf_t hdr_buf, adf_nbuf_t buf);

/**
 * @brief: Shutdown the HIF layer.
 * @param[in]: HIFHandle - opaque HIF handle.
 * @param[out]:
 * @return:
 */ 
void HIFShutDown(void *HIFHandle);

#ifdef MAGPIE_SINGLE_CPU_CASE
a_uint32_t HIFQueryQueueDepth(HIF_HANDLE hHIF, a_uint8_t epnum);
#endif

void HIFGetDefaultPipe(void *HIFHandle, a_uint8_t *ULPipe, a_uint8_t *DLPipe);

a_uint8_t HIFGetULPipeNum(void);
a_uint8_t HIFGetDLPipeNum(void);

/**
 * For boot /Firmware downloader
 */
a_status_t  hif_boot_start(HIF_HANDLE   hdl);
void        hif_boot_done(HIF_HANDLE     hdl);

#endif  /* !_HIF_H_ */
