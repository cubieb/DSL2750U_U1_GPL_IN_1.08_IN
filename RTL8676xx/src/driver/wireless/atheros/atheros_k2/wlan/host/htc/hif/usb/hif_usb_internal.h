#ifndef _HIF_USB_INTERNAL_H
#define _HIF_USB_INTERNAL_H

#include <adf_nbuf.h>

#include "athdefs.h"
#include "a_osapi.h"
#include "hif.h"

#include <linux/usb.h>
#define STREAMSUPPORT  -1


#define HIF_USB_PIPE_COMMAND            4 /* 0 */
#define HIF_USB_PIPE_INTERRUPT          3 /* 1 */
#define HIF_USB_PIPE_TX                 1 /* 2 */
#define HIF_USB_PIPE_RX                 2 /* 3 */

#define HIF_USB_MAX_RXPIPES     2
#define HIF_USB_MAX_TXPIPES     4

#if STREAMSUPPORT != -1
    #define ZM_USB_TX_STREAM_MODE   1
    #define ZM_USB_STREAM_MODE      1
#else
    #define ZM_USB_TX_STREAM_MODE   0
    #define ZM_USB_STREAM_MODE      0
#endif

#if STREAMSUPPORT != -1
    #define ZM_MAX_RX_BUFFER_SIZE               8192*2
#else
    #define ZM_MAX_RX_BUFFER_SIZE               2048
#endif

#if ZM_USB_TX_STREAM_MODE == 1
#define ZM_MAX_TX_AGGREGATE_NUM             10
#define ZM_USB_TX_BUF_SIZE                  1600//8096
#define ZM_MAX_TX_URB_NUM                   2
#else
#define ZM_USB_TX_BUF_SIZE                  2048
#define ZM_MAX_TX_URB_NUM                   8
#endif
#define ZM_USB_REG_MAX_BUF_SIZE             512
#define ZM_MAX_RX_URB_NUM                   16
#define ZM_MAX_TX_BUF_NUM                   128

#define MAX_CMD_URB                         4 
              
#define urb_t                       struct urb

/* USB Endpoint definition */
#define USB_WLAN_TX_PIPE                    1
#define USB_WLAN_RX_PIPE                    2
#define USB_REG_IN_PIPE                     3
#define USB_REG_OUT_PIPE                    4
#define USB_WLAN_HP_TX_PIPE                 5
#define USB_WLAN_MP_TX_PIPE                 6

#define FIRMWARE_DOWNLOAD       0x30
#define FIRMWARE_DOWNLOAD_COMP  0x31
#define FIRMWARE_CONFIRM        0x32
#define HOST_COMMAND_RESERVED_1 0x33
#define HOST_COMMAND_RESERVED_2 0x34

#define HOST_FLASH_READ         0x35
#define HOST_FLASH_READ_COMP    0x36

#define ZM_FIRMWARE_WLAN_ADDR		0x501000
#ifdef K2
//#define ZM_FIRMWARE_TEXT_ADDR		0x904000 
#define ZM_FIRMWARE_TEXT_ADDR		0x903000 
#else
#define ZM_FIRMWARE_TEXT_ADDR		0x906000 
#endif

struct _hif_device_usb;

typedef void (*ath_usb_unload)(void);

typedef struct _UsbCmdUrbContext {
    a_uint8_t               use;
    urb_t                   *RegOutUrb;
    adf_nbuf_t              buf;
    struct _hif_device_usb *macp;    
} UsbCmdUrbContext;

typedef struct UsbTxQ
{
    adf_nbuf_t      buf;
} UsbTxQ_t;

typedef struct _UsbTxUrbContext {
    a_uint8_t  index;
    a_uint8_t  inUse;
    struct _hif_device_usb *macp;
    adf_nbuf_t buf;
    urb_t      *WlanTxDataUrb;
    a_uint8_t  txUsbBuf[ZM_USB_TX_BUF_SIZE];
} UsbTxUrbContext;

typedef struct _hif_usb_tx_pipe {
    a_uint8_t               TxPipeNum;
    //a_uint8_t               txUsbBuf[ZM_MAX_TX_URB_NUM][ZM_USB_TX_BUF_SIZE];
    //urb_t                   *WlanTxDataUrb[ZM_MAX_TX_URB_NUM];
    UsbTxQ_t                UsbTxBufQ[ZM_MAX_TX_BUF_NUM];
    a_uint16_t              TxBufHead;
    a_uint16_t              TxBufTail;
    a_uint16_t              TxBufCnt;
    a_uint16_t              TxUrbHead;
    a_uint16_t              TxUrbTail;
    a_uint16_t              TxUrbCnt;
    UsbTxUrbContext         TxUrbCtx[ZM_MAX_TX_URB_NUM];
    usb_complete_t          TxUrbCompleteCb;
#if STREAMSUPPORT != -1
#if ZM_USB_TX_STREAM_MODE == 1
    unsigned char           txbuf0[1024*16];
    unsigned char           txbuf1[1024*16];
#endif
#endif

} HIFUSBTxPipe;

typedef struct _hif_device_usb {
    void *htc_handle;
    HTC_CALLBACKS htcCallbacks;
    spinlock_t      cs_lock;
	
    struct usb_device       *udev;
    struct usb_interface    *interface;
    adf_nbuf_t              regUsbReadBuf;
//    adf_nbuf_t              regUsbWriteBuf;
    urb_t                   *WlanRxDataUrb[ZM_MAX_RX_URB_NUM];  
    //urb_t                   *RegOutUrb;
    urb_t                   *RegInUrb;
    adf_nbuf_t              UsbRxBufQ[ZM_MAX_RX_URB_NUM];
    a_uint16_t              RxBufHead;
    a_uint16_t              RxBufTail;
    a_uint16_t              RxBufCnt;
    HIFUSBTxPipe            TxPipe;
    HIFUSBTxPipe            HPTxPipe;
    HIFUSBTxPipe            MPTxPipe;
    
    UsbCmdUrbContext        CmdUrbCtxs[MAX_CMD_URB];
    spinlock_t              CmdUrbLock;
#if STREAMSUPPORT != -1
    adf_nbuf_t remain_buf ;
    struct timer_list       tm_txagg;
#endif

    a_uint8_t               surpriseRemoved;
} HIF_DEVICE_USB;


extern int zfLnxAllocAllUrbs(HIF_DEVICE_USB *macp);
extern void zfLnxUnlinkAllUrbs(HIF_DEVICE_USB *macp);
extern void zfLnxFreeAllUrbs(HIF_DEVICE_USB *macp);
extern a_uint32_t zfLnxSubmitRegInUrb(HIF_DEVICE_USB *macp);
extern a_uint32_t zfLnxUsbSubmitBulkUrb(urb_t *urb, struct usb_device *usb, a_uint16_t epnum, a_uint16_t direction,
                  a_uint8_t *transfer_buffer, int buffer_length, usb_complete_t complete, void *context);
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0))
extern void zfLnxUsbRegOut_callback(urb_t *urb);
#else
extern void zfLnxUsbRegOut_callback(urb_t *urb, struct pt_regs *regs);
#endif

extern void zfLnxInitUsbTxQ(HIF_DEVICE_USB *macp);
extern void zfLnxInitUsbRxQ(HIF_DEVICE_USB *macp);
extern a_uint32_t zfLnxPutUsbRxBuffer(HIF_DEVICE_USB *macp, adf_nbuf_t buf);

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,5,0))
extern void zfLnxUsbDataIn_callback(urb_t *urb);
#else
extern void zfLnxUsbDataIn_callback(urb_t *urb, struct pt_regs *regs);
#endif

extern a_uint32_t zfLnxUsbIn(HIF_DEVICE_USB *macp, urb_t *urb, adf_nbuf_t buf);
extern a_uint32_t zfLnxUsbOut(HIF_DEVICE_USB *macp, adf_nbuf_t buf, HIFUSBTxPipe *pipe);

extern a_uint32_t zfLnxUsbSubmitControl(HIF_DEVICE_USB *macp, a_uint8_t req, a_uint16_t value, a_uint16_t index, void *data, a_uint32_t size);

extern UsbCmdUrbContext* zfLnxAllocCmdUrbCtx(HIF_DEVICE_USB *macp);
extern a_uint32_t zfLnxUsbSubmitIntUrb(urb_t *urb, struct usb_device *usb, a_uint16_t epnum, a_uint16_t direction,
        void* buf, int buffer_length, usb_complete_t complete, void *context,
        a_uint32_t interval);

#endif
