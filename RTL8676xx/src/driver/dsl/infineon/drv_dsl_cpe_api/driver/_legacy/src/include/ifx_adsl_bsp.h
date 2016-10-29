#ifndef _IFX_ADSL_BSP_H
#define _IFX_ADSL_BSP_H

/******************************************************************************

                               Copyright (c) 2006
                            Infineon Technologies AG
                     Am Campeon 1-12; 81726 Munich, Germany

  Any use of this Software is subject to the conclusion of a respective
  License Agreement. Without such a License Agreement no rights to the
  Software are granted.

******************************************************************************/

/** \file
   This file defines the shared interface definitions from MEI BSP Driver
*/

#define IFX_ADSL_BSP_IOC_IS_MODEM_READY   ( 0 + IFX_ADSL_IOC_BSP_BASE)
#define IFX_ADSL_BSP_IOC_GET_BSP_VERSION  ( 1 + IFX_ADSL_IOC_BSP_BASE)
#define IFX_ADSL_BSP_IOC_FREE_RESOURCE	  ( 2 + IFX_ADSL_IOC_BSP_BASE)

#define IFX_ADSL_BSP_IOC_RESET            IFX_ADSL_IOC_RESET
#define IFX_ADSL_BSP_IOC_REBOOT           IFX_ADSL_IOC_REBOOT
#define IFX_ADSL_BSP_IOC_HALT             IFX_ADSL_IOC_HALT
#define IFX_ADSL_BSP_IOC_RUN              IFX_ADSL_IOC_RUN
#define IFX_ADSL_BSP_IOC_BOOTDOWNLOAD     IFX_ADSL_IOC_DOWNLOAD
#define IFX_ADSL_BSP_IOC_JTAG_ENABLE      IFX_ADSL_IOC_JTAG_ENABLE
#define IFX_ADSL_BSP_IOC_REMOTE           IFX_ADSL_IOC_REMOTE
#define IFX_ADSL_BSP_IOC_DSLSTART         IFX_ADSL_IOC_START
#define IFX_ADSL_BSP_IOC_DEBUG_READ       IFX_ADSL_IOC_READDEBUG
#define IFX_ADSL_BSP_IOC_DEBUG_WRITE      IFX_ADSL_IOC_WRITEDEBUG
#define IFX_ADSL_BSP_IOC_CMV_WINHOST      IFX_ADSL_IOC_CMV_WINHOST
#define IFX_ADSL_BSP_IOC_GET_BASE_ADDRESS IFX_ADSL_IOC_GET_BASE_ADDRESS
#define IFX_ADSL_BSP_IOC_CMV_READ         IFX_ADSL_IOC_CMV_READ
#define IFX_ADSL_BSP_IOC_CMV_WRITE        IFX_ADSL_IOC_CMV_WRITE

typedef enum {
   IFX_ADSL_BSP_EVENT_DYING_GASP = 0,
   IFX_ADSL_BSP_EVENT_CEOC_IRQ,
} ifx_adsl_event_id_t;

typedef union ifx_adsl_cbparam
{
   unsigned long irq_message;
}ifx_adsl_cbparam_t;

typedef struct ifx_adsl_cb_event
{
   unsigned long ID;
   struct ifx_adsl_device *pDev;
   ifx_adsl_cbparam_t *param;
}ifx_adsl_cb_event_t;

typedef enum {
   IFX_ADSL_LED_LINK_ID=0,
   IFX_ADSL_LED_DATA_ID,
} ifx_adsl_led_id_t;

typedef enum {
   IFX_ADSL_LED_LINK_TYPE=0,
   IFX_ADSL_LED_DATA_TYPE,
} ifx_adsl_led_type_t;

typedef enum {
   IFX_ADSL_LED_ON=0,
   IFX_ADSL_LED_OFF,
   IFX_ADSL_LED_FLASH,
} ifx_adsl_led_mode_t;

typedef enum {
   IFX_ADSL_CPU_HALT=0,
   IFX_ADSL_CPU_RUN,
   IFX_ADSL_CPU_RESET,
} ifx_adsl_cpu_mode_t;

typedef enum {
   IFX_ADSL_MEMORY_READ=0,
   IFX_ADSL_MEMORY_WRITE,
} ifx_adsl_memory_access_type_t;

typedef enum {
   IFX_ADSL_LED_HD_CPU=0,
   IFX_ADSL_LED_HD_FW,
} ifx_adsl_led_handler_t;

typedef struct ifx_adsl_device
{
   int nInUse; /*  modem state, update by bsp driver, */
   void *priv;
   unsigned long base_address; /*  mei base address */
   int nIrq;   /*  irq number */
   meidebug lop_debugwr;            /* dying gasp */
}ifx_adsl_device_t;

typedef struct ifx_adsl_bsp_version
{
   unsigned long major;
   unsigned long minor;
   unsigned long revision;
}ifx_adsl_bsp_version_t;


extern MEI_ERROR IFX_ADSL_BSP_SendCMV
(
   struct ifx_adsl_device *dev,u16 * request,
   int reply, u16 *response
);

extern MEI_ERROR IFX_ADSL_BSP_FWDownload
(
   struct ifx_adsl_device *dev,
   const char * buf,
   unsigned long size,
   long * loff,
   long * current_off
);

extern MEI_ERROR IFX_ADSL_BSP_Showtime
(
   struct ifx_adsl_device *dev,
   u32 rate_fast,
   u32 rate_intl
);

extern int IFX_ADSL_BSP_KernelIoctls
(
   ifx_adsl_device_t *pDev,
   unsigned int command,
   unsigned long lon
);

extern ifx_adsl_device_t *IFX_ADSL_BSP_DriverHandleGet
(
   int maj,
   int num
);

extern int IFX_ADSL_BSP_DriverHandleDelete
(
   ifx_adsl_device_t *nHandle
);

extern MEI_ERROR IFX_ADSL_BSP_AdslLedInit
(
   struct ifx_adsl_device *dev,
   ifx_adsl_led_id_t led_number,
   ifx_adsl_led_type_t type,
   ifx_adsl_led_handler_t handler
);

extern int IFX_ADSL_BSP_ATMLedCBRegister
(
   int (*ifx_adsl_ledcallback)(void)
);

extern int IFX_ADSL_BSP_ATMLedCBUnregister
(
   int (*ifx_adsl_ledcallback)(void)
);

extern MEI_ERROR IFX_ADSL_BSP_MemoryDebugAccess
(
   struct ifx_adsl_device *dev,
   ifx_adsl_memory_access_type_t type,
   u32 srcaddr,
   u32 *databuff,
   u32 databuffsize
);

extern volatile ifx_adsl_device_t *adsl_dev;

/**
 *  Dummy structure by now to show mechanism of extended data that will be
 *  provided within event callback itself.
 */
typedef struct
{
   /**
 Dummy value */
   unsigned long nDummy1;
} DSL_BSP_CB_Event1DataDummy_t;

/**
 *    Dummy structure by now to show mechanism of extended data that will be
 *    provided within event callback itself.
 */
typedef struct
{
   /**
 *    Dummy value */
   unsigned long nDummy2;
} DSL_BSP_CB_Event2DataDummy_t;

/**
 *    encapsulate all data structures that are necessary for status event
 *       callbacks.
 */
typedef union
{
   DSL_BSP_CB_Event1DataDummy_t;
   DSL_BSP_CB_Event2DataDummy_t;
} DSL_BSP_CB_DATA_Union_t;

typedef enum
{
        /**
	*    Informs the upper layer driver (DSL CPE API) about a reboot request from the
	*    firmware.
	*    \note This event does NOT include any additional data.
	*          More detailed information upon reboot reason has to be requested from
	*          upper layer software via CMV (INFO 109) if necessary. 
	*/
        DSL_BSP_CB_FIRMWARE_REBOOT_REQUEST = 0,
        DSL_BSP_CB_CLEAREOC_INTERRUPT,
	DSL_BSP_CB_DYING_GASP,
        /**
	 *    Delimiter only */
        DSL_BSP_CB_LAST
} DSL_BSP_CB_Type_t;

/**
 *    Specifies the common event type that has to be used for registering and
 *    signalling of interrupts/autonomous status events from MEI BSP Driver.
 *
 *    \param pDev
 *    Context pointer from MEI BSP Driver.
 *
 *    \param IFX_ADSL_BSP_CallbackType_t
 *    Specifies the event callback type (reason of callback). Regrading to the
 *    setting of this value the data which is included in the following union
 *    might have different meanings.
 *    Please refer to the description of the union to get information about the
 *    meaning of the included data.
 *
 *    \param pData
 *    Data according to \ref DSL_BSP_CB_DATA_Union_t.
 *    If this pointer is NULL there is no additional data available.
 *    \return depending on event
 */
typedef int (*DSL_BSP_EventCallback_t)
(
   struct ifx_adsl_device *pDev,
   DSL_BSP_CB_Type_t nCallbackType,
   DSL_BSP_CB_DATA_Union_t *pData
);

typedef struct {
        DSL_BSP_EventCallback_t function;
        DSL_BSP_CB_Type_t       event;
        DSL_BSP_CB_DATA_Union_t *pData;
} DSL_BSP_EventCallBack_t;

extern int IFX_ADSL_BSP_RegisterEventCallBack(DSL_BSP_EventCallBack_t *);
extern int IFX_ADSL_BSP_UnRegisterEventCallBack(DSL_BSP_EventCallBack_t *);

#endif /* _IFX_ADSL_BSP_H */
