/**************************************************************
$Workfile:   dapi_define.h  $
***************************************************************
**
** FIRMWARE, Copyright (C) 2000-2002
** Ikanos Communications, Fremont, CA
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**
$Revision: 1.1 $
$Date: 2008/07/18 11:09:32 $
***************************************************************/

#ifndef _HPI_H_
#define _HPI_H_
/**** Start of hpi definitions ***/
 
/* Header Section - Opcode */

#define HPI_PROV_PORT           (0x03)
#define HPI_ENABLE_PORT         (0x04)
#define HPI_ACTIVATE_PORT       (0x05)
#define HPI_DEACTIVATE_PORT     (0x06)
#define HPI_RESET_PORT          (0x08)
#define HPI_SET_SYSDB           (0x09)
#define HPI_EOC_CONTROL         (0x0A)
#define HPI_EOC_SET_DB          (0x0B)
#define HPI_EOC_DATA_RECEIVE    (0x0C)
#define HPI_EOC_DATA_REPLY_CMD  (0x0D)
#define HPI_EOC_SEND_DATA       (0x0E)
#define HPI_EOC_GET_DB          (0x0F)
#define HPI_PM_CONFIG           (0x14)
#define HPI_PM_TIMER            (0x15)
#define HPI_PM_TRIGGER_FAILURES (0x16)
#define HPI_HOST_HOT_RESET      (0x19)
#define HPI_BIT_SWAP_REQ        (0x1A)
#define HPI_UPDATE_COUNTER	(0x1B)
#ifdef BER_PER_TONE
#define HPI_DUMMY_ALL_ZERO_REQ  (0x1C)
#endif
#define HPI_PORT_CLASSIFIER_ENABLE        (0x30)
#define HPI_SYS_PROTOCOL2CLASS_MAP        (0x31)
#define HPI_PORT_PROTOCOL_CONFIG          (0x32)
#define HPI_PORT_CLASS2QUEUE_MAP          (0x33)
#define HPI_PORT_QUEUE_CONFIG             (0x34)

#define HPI_PORT_STATUS         (0xC0)
#define HPI_SYSTEM_STATUS       (0xC2)
#define HPI_ALARM_COUNTERS      (0xC3)
#define HPI_SET_STATUS_REQ      (0xC4)
#define HPI_VERSIONS            (0xC6)
#define HPI_SELFTEST_RESULT     (0xC7)
#define HPI_SNR_SETTINGS        (0xC8)
#define HPI_PM_COUNTERS         (0xCA)
#define HPI_GET_QOS_LEVEL       (0xCB)
#define HPI_QOS_COUNTERS        (0xCC)

#define HPI_OP_CMD              (0xD0)

#if defined(RTEMS_VX180) || defined(FUSIV_VX180)
#define HPI_THROTTLE_TRAF_REQ   (0xE0)
#endif

#define HPI_LOOPBACK_STOP       (0x59)
#define HPI_LOOPBACK            (0x60)
#define HPI_TEST_MODE           (0x61)
#define HPI_GET_DELT_RESPONSE   (0x65)


#define HPI_EOC_RELAY_DATA_STREAM_CMD (0x80)
#define HPI_EOC_GET_DATA            (0x80)
#define HPI_EOC_MSG_RECEIVE_CMD     (0x81)  
#define HPI_PM_ALERT             (0x82)
#define HPI_EOC_PROTOCOL_SET      (0x17)
#define HPI_EOC_PROTOCOL_GET      (0x18)
#define HPI_EOC_INTERNAL_TRACE_LOG  (0x83)

#define HPI_HOST_RETRIEVE_BME_CONFIG  (0x85)
#define HPI_HOST_RETRIEVE_PORT_CONFIG (0x86)
/* To be implemented */
#define HPI_UPDATED_REPORTS_ENABLE  (0xC5)
#define HPI_DETECT_SIGNALS          (0x62)
#define HPI_PORT_STATUS_EXTENDED    (0xC1)

/* Operation State */
#define MAINTENANCE   (0x80)
#define WAIT_CONFIG   (0x00)
#define IDLE          (0x01)
#define QUIET         (0x02)
#define ACTIVATE      (0x03)
#define TRAINING      (0x04)
#define ANALYSIS      (0x05)
#define EXCHANGE      (0x06)
#define SHOWTIME      (0x07)
#define FAST_RETRAIN  (0x08)

/* Frame Mode */
#define FULL_ASYNC      (0)
#define FULL_SYNC       (1)
#define REDUCE_SEPARATE (2)
#define REDUCE_MIXED    (3)
#if defined(RTEMS_VX180) || defined(FUSIV_VX180)
/* Traffic Throttling requests */
#define THROTTLE_ON     1
#define THROTTLE_OFF    0
#endif

#if 0
/* Operation Mode Defines */
#define ADSL_AUTO_DETECT  (0x00)
#define ADSL_T1E1         (0x08)
#define ADSL_T1E1_ISSUE_1 (0x08)
#define ADSL_T1E1_ISSUE_2 (0x09)
#define ADSL_DMT          (0x10)
#define ADSL_DMT_ANNEX_A  (0x10)
#define ADSL_DMT_ANNEX_B  (0x11)
#define ADSL_DMT_ANNEX_C  (0x12)
#define ADSL_LITE         (0x20)
#define VDSL_MODE         (0x80)
#define VDSL_AUTO_DETECT  (0x80)
#define VDSL_T1E1         (0x81)
#define VDSL              (VDSL_T1E1)
#define VDSL_ETSI         (0x82)
#endif
/*
 * Bits in Line Type passed to port provision structure
 */
#define ADSL1_ANNEX_A (1<<0)
#define ADSL1_ANNEX_B (1<<1)
#define ADSL1_ANNEX_C (1<<2)
#define ADSL2_ANNEX_A (1<<3)
#define ADSL2_ANNEX_B (1<<4)

#define ADSL2_PLUS_ANNEX_A (1<<5)
#define ADSL2_PLUS_ANNEX_B (1<<6)
#define ADSL2_PLUS_ANNEX_M (1<<7)
#define ADSL2_PLUS_ANNEX_L (1<<8)
#define ADSL_T1E1_ISSUE_2 (1<<11)
#define ADSL_T1E1  ADSL_T1E1_ISSUE_2 

#define ADSL_LITE_MODE     (1<<9)

//
// BITS 9 -15 are reserved
//
#define VDSL1_ANSI         (1<<16)
#define VDSL1_ETSI         (1<<17)
#define VDSL_ITU_993_1     (1<<18)
#define VDSL_IEEE_802_AH   (1<<19)

#define ITU_G993_2_8A (1<<20)
#define ITU_G993_2_8B (1<<21)
#define ITU_G993_2_8C (1<<22)
#define ITU_G993_2_8D (1<<23)
#define ITU_G993_2_12A (1<<24)
#define ITU_G993_2_12B (1<<25)
#define ITU_G993_2_17A (1<<26)
#define ITU_G993_2_30A (1<<27)
/*Reserved Bits 28-31*/

#define ADSL1_TYPE_SUPPORTED (ADSL1_ANNEX_A | ADSL1_ANNEX_B)
#define ADSL2_TYPE_SUPPORTED (ADSL2_ANNEX_A | ADSL2_PLUS_ANNEX_A | ADSL2_PLUS_ANNEX_M | ADSL2_PLUS_ANNEX_B | ADSL2_ANNEX_B)
#define ADSL_TYPE_SUPPORTED (ADSL1_TYPE_SUPPORTED | ADSL2_TYPE_SUPPORTED)
#define VDSL1_TYPE_SUPPORTED (VDSL1_ANSI| VDSL1_ETSI|VDSL_ITU_993_1)
#define VDSL2_TYPE_SUPPORTED ( ITU_G993_2_8A | ITU_G993_2_8B | ITU_G993_2_8C | ITU_G993_2_8D | ITU_G993_2_12A |ITU_G993_2_12B| ITU_G993_2_17A | ITU_G993_2_30A )

#define LINE_TYPE_SUPPORTED ( ADSL1_TYPE_SUPPORTED| VDSL1_TYPE_SUPPORTED| VDSL2_TYPE_SUPPORTED )
 

/* Bit Wise Option Mask */
#define NTR_DISABLE       (0x01)
#define IMUX_ENABLE       (0x02)
#define SUPPORT_INI       (0x04)
#define ADSL_SAFE_MODE      (0x40)

/* Rate Adaptive Mode */
#define FIXED       (0)
#define AT_STARTUP  (1)
#define AT_RUNTIME  (2)

/* ATM option defines */
#define ATM_LEVEL_2     (0x00)
#define ATM_LEVEL_3     (0x01)
#define ATM_EXTEND_MODE (0x02)
#define ATM_REMOVE_HEC  (0x04)

/* Number of ports */
#define ONE_PORT        (1)
#define TWO_PORTS       (2)
#define FOUR_PORTS      (3)
#define EIGHT_PORTS     (4)

/* Activation flag defines */
#define ACTIVATE_AFTER_PROV     (0x03)
#define QUIET_AFTER_PROV        (0x01)
#define IDLE_AFTER_PROV             (0x00)
#define ACTIVATE_AFTER_RESET    (ACTIVATE_AFTER_PROV)
#define QUIET_AFTER_RESET           (QUIET_AFTER_PROV)

/* VDSL band setting defines */
#define BP1_998_3       (0)
#define BP2_998_3       (1)
#define BP998_3B_8_5M   BP2_998_3
#define BP3_998_4       (2)
#define BP998_4B_12M    BP3_998_4
#define BP4_997_3       (3)
#define BP997_3B_7_1M   BP4_997_3
#define BP5_997_3       (4)
#define BP6_997_4       (5)
#define BP997_4B_7_1M   BP6_997_4
#define BP7_MXU_3       (6)
#define FLEX_3B_8_5M    BP7_MXU_3
#define BP8_MXU_2       (7)
#define BP9_998_2       (8)
#define BP10_998_2      (9)
#define BP998_2B_3_8M   BP10_998_2
#define BP11_998_2      (10)
#define BP12_998_2      (11)
#define BP13_MXU_3      (12)
#define BP14_MXU_3      (13)
#define BP15_MXU_3      (14)
#define BP16_997_4B_4P  (15)
#define BP17_998_138_4400  (16)
#define BP18_997_138_4400   (17)
#define BP19_997_32_4400    (18)
/* do not use 19 and 20 */
#define BP20_998_138_4400_opBand    (21)
#define BP21_997_138_4400_opBand    (22)
#define BP22_998_138_4400_opBand    BP20_998_138_4400_opBand
#define BP23_998_138_16000  (23)
#define BP24_998_3B_8KHZ    (24)
#define BP25_998_138_17600  (25)
#define BP26_CH1_3  (26)
#define BP27_CH1_4  (27)

/* Enumeration Definitions */

/* Data direction types */
typedef enum eDataDir 
    {
        HOST_TO_BME,
        BME_TO_HOST
    }eDataDir;

/* Tx and Rx maintenance */
#define HPI_XMT_QUIET            (1)
#define HPI_XMT_TONES            (2)
#define HPI_XMT_TONES_REQ        (3)
#define HPI_XMT_TONE_SWEEP       (4)
#define HPI_XMT_TRAINING_SYMBOL  (5)
#define HPI_XMT_SYNCHRO_SYMBOL   (6)
#define HPI_XMT_MEDLEY_SYMBOL    (7)
#define HPI_XMT_GARABIT          (8)
#define HPI_AFE_LOOPBACK         (10)

#define HPI_RX_POWER             (20)
#define HPI_CAP_AFIFO            (21)
#define HPI_CAP_FFT              (22)

/* Loopback Modes */
#define HPI_RELEASE_LOOP        (1)
#define HPI_NETWORK_NETWORK     (2)
#define HPI_NETWORK_LINE        (3)
#define HPI_LINE_NETWORK        (4) 
#define HPI_LINE_LINE           (5)

/* Micro cut flag(state) from the bme */
#define BME_MICROCUT_ON			0x01
#define BME_MICROCUT_OFF		0x00
#define BME_MICROCUT_PORT_DROP	0xFF

/**** End of hpi definitions ***/

#define MAX_BME_HEADER      6
#define MAX_BME_MESSAGE     256
#define MAX_BME_DATA        (MAX_BME_MESSAGE - MAX_BME_HEADER)

/* BME message structure based on HPI definition */

#define _BmeMsgSRC_     0
#define _BmeMsgDEST_    1
#define _BmeMsgOPCODE_  2
#define _BmeMsgACK_     3
#define _BmeMsgCTAG_    4
#define _BmeDataLENGTH_ 5
#define _BmeDataStart_  6

/* Endian Types */
#define LITTLE_ENDIAN_FORMAT        1
#define BIG_ENDIAN_FORMAT           2

#define TOTAL_PORTPARMS 82          // Used to indicate number of  
/* Host API Opcodes */

typedef enum eCommands
    {
    /* Control Commands */
    BME_START               =0x01, /*host only*/
    BME_RESET               =0x02, /*host only*/
    PORT_PROVISION          =HPI_PROV_PORT,
    PORT_INSTANTIATE        =HPI_ENABLE_PORT,
    PORT_START              =HPI_ACTIVATE_PORT,
    PORT_STOP               =HPI_DEACTIVATE_PORT,
    PORT_DESTROY            =0x07, /*@ host lvl only, @bme =HPI_DEACTIVATE_PORT*/
    PORT_RESET              =HPI_RESET_PORT,
    SYSTEM_PROVISION        =HPI_SET_SYSDB,
    EOC_CONTROL_SEND        =HPI_EOC_CONTROL,
    EOC_SET_DATA            =HPI_EOC_SET_DB,
    BIT_SWAP_REQ            =HPI_BIT_SWAP_REQ,
#ifdef BER_PER_TONE    
	DUMMY_ALL_ZERO_REQ      =HPI_DUMMY_ALL_ZERO_REQ,
#endif	
    EOC_DATA_RECEIVE        =HPI_EOC_DATA_RECEIVE,
    EOC_DATA_REPLY          =HPI_EOC_DATA_REPLY_CMD,
    EOC_SEND_DATA_STREAM    =HPI_EOC_SEND_DATA,
    EOC_GET_DATA            =HPI_EOC_GET_DB,
    EOC_PROTOCOL_SET        =HPI_EOC_PROTOCOL_SET,
        EOC_PROTOCOL_GET        =HPI_EOC_PROTOCOL_GET,
    CLEAR_ALL_COUNTERS      =0x11, /*host only*/
    EMI_CONFIG              =0x12, /*host only*/
    UTOPIA_LB               =0x13,
    PM_CONFIG_PARAMS        = HPI_PM_CONFIG,
    PM_SEND_TIMER           = HPI_PM_TIMER,
    PM_TRIGGER_FAILURES     = HPI_PM_TRIGGER_FAILURES,
        HOST_HOT_RESET          =HPI_HOST_HOT_RESET,
	UPDATE_COUNTER          =HPI_UPDATE_COUNTER,
    PORT_CLASSIFIER_ENABLE  = HPI_PORT_CLASSIFIER_ENABLE,
    SYS_PROTOCOL2CLASS_MAP  = HPI_SYS_PROTOCOL2CLASS_MAP,
    PORT_PROTOCOL_CONFIG    = HPI_PORT_PROTOCOL_CONFIG,
    PORT_CLASS2QUEUE_MAP    = HPI_PORT_CLASS2QUEUE_MAP,
    PORT_QUEUE_CONFIG       = HPI_PORT_QUEUE_CONFIG,

    
    LAST_CTRL_COMMAND       =0x5F, /*host only*/ 
    /* Test APIs */
    SET_LOOPBACK            =HPI_LOOPBACK,
    STOP_LOOPBACK           =HPI_LOOPBACK_STOP,
    SEND_SIGNALS            =HPI_TEST_MODE,
    DETECT_SIGNALS          =HPI_DETECT_SIGNALS,
    GET_DELT_RESPONSE       =HPI_GET_DELT_RESPONSE,
    DEBUG_MON               =0x7E,
    LAST_TEST_COMMAND       =0x7F,
    /*Autonomous APIs   */
    EOC_RELAY_DATA_STREAM   =HPI_EOC_GET_DATA,
    EOC_MSG_RECEIVE         =HPI_EOC_MSG_RECEIVE_CMD,
    GET_PM_ALERT              = HPI_PM_ALERT,
    EOC_INTERNAL_TRACE_LOG  =HPI_EOC_INTERNAL_TRACE_LOG,
    HOST_RETRIEVE_BME_CONFIG =HPI_HOST_RETRIEVE_BME_CONFIG,
    HOST_RETRIEVE_PORT_CONFIG = HPI_HOST_RETRIEVE_PORT_CONFIG,
    LAST_AUTO_COMMAND       =0xBF,
    /*Status Request Commands*/
    PORT_STATUS_BRIEF       =HPI_PORT_STATUS,
    PORT_STATUS_EXTENDED    =HPI_PORT_STATUS_EXTENDED,
    SYSTEM_STATUS           =HPI_SYSTEM_STATUS,
    GET_COUNTERS            =HPI_ALARM_COUNTERS,
    PERIODIC_REPORTS_ENABLE =HPI_SET_STATUS_REQ,
    UPDATED_REPORTS_ENABLE  =HPI_UPDATED_REPORTS_ENABLE,
    DEVICE_VERSIONS         =HPI_VERSIONS,
    GET_QOS_LEVEL           =HPI_GET_QOS_LEVEL,
    GET_SELFTEST_RESULTS    =HPI_SELFTEST_RESULT,
    GET_NE_SNR_ATTN         =HPI_SNR_SETTINGS,
    GET_PM_COUNTERS         =HPI_PM_COUNTERS,
    GET_OPCMD_STATUS        =HPI_OP_CMD,
    GET_QOS_COUNTERS        =HPI_QOS_COUNTERS,
    LAST_STATUS_COMMAND     =0xCF,
    /* config commands */
    GET_HOST_ENDIAN_TYPE    =0x100, 
    SET_HOST_ENDIAN_TYPE,
    EVENT_LOG_STATUS,
    EVENT_LOG_ENABLE,
    EVENT_LOG_CLEAR,
    EVENT_LOG_PRINT,
    FIFO_DISPLAY_STATUS,
    FIFO_DISPLAY_ENABLE,
    MSG_TRACING_STATUS,
    MSG_TRACING_ENABLE,
    MSG_FILTER_VALUE,
    MSG_FILTER,
    /* additional commands supported for UI */
    GET_PORT_PROFILE,
    SET_PORT_PROFILE_PARAMETER,
    PERFORMANCE_MONITORING,
    THRESHOLD_REPORTS,
    PORT_BRINGUP,
    PORT_TEARDOWN,
    USER_LOG_CLEAR,
    USER_LOG_PRINT,
    GET_EOC_PROTOCOL_VER,
    REMOTE_REBOOT,
    GET_REMOTE_VERSION,
    GET_DEVICE_VERSIONS,
    SHOW_DEVICE_VERSIONS,
    GET_REMOTE_ERRORED_SECONDS,
    GET_REMOTE_ENET_STATUS,
    GET_REMENET_RPT_FLAG,
    GET_REMOTE_MDIO_REGS,
	GET_REMOTE_FPGA_MDIO_REGS,
    GET_REMOTE_ENET_CFG,
    GET_REMOTE_MODEM_ID,
    GET_REMOTE_GPIO_PINS,
    GET_REMOTE_TXPWR,
  GET_REMOTE_LEARNED_ENET_ADDR,
    SHOW_ERRORED_SECONDS,
    SHOW_ETHERNET_STATUS,
    SEND_VIRTUAL_MESSAGE_1,
    SET_REMOTE_ENET_LB,
    SET_ENET_FLOW_CONTROL,
    SET_REMOTE_MDIO_REG,
	SET_REMOTE_FPGA_MDIO_REG,
    SET_REMOTE_ENET_CFG,
    SET_REMENET_STATUS_RPT,
    CLEAR_REMOTE_CNTRS,
    CANCEL_CMD
    } eCommands;

#define DELT_RESPONSE_SIZE 1
#define DELT_BUFFER_SIZE  (2584+2*258)
#endif
       
