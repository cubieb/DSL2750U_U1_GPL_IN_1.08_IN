/*
 *	 Headler file defines some configure options and basic types
 *
 *	 $Id: 8192cd_cfg.h,v 1.18 2011/11/10 12:37:22 cathy Exp $
 *
 *  Copyright (c) 2009 Realtek Semiconductor Corp.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
 */

#ifndef _8192CD_CFG_H_
#define _8192CD_CFG_H_

#ifdef __MIPSEB__
	#define _BIG_ENDIAN_
#else
	#define _LITTLE_ENDIAN_
#endif

#ifdef __KERNEL__
#include <linux/version.h>

#if LINUX_VERSION_CODE >= 0x020616 // linux 2.6.22
	#define LINUX_2_6_22_
#endif

#if LINUX_VERSION_CODE >= 0x020618 // linux 2.6.24
	#define LINUX_2_6_24_
#endif

#if LINUX_VERSION_CODE >= 0x02061B // linux 2.6.27
	#define LINUX_2_6_27_
#endif

#if LINUX_VERSION_CODE > 0x020600
	#define __LINUX_2_6__
#endif

#ifdef LINUX_2_6_22_
#include <linux/autoconf.h>
#include <linux/jiffies.h>
#include <asm/param.h>
#else
#include <linux/config.h>
#endif
#endif // __KERNEL__

#ifdef CONFIG_RTK_MESH
#include "mesh_ext/mesh_cfg.h"
#endif

#if defined(CONFIG_RTL8196B) || defined(CONFIG_RTL_819X)
	#if defined(CONFIG_RTL8196B_AP_ROOT) || defined(CONFIG_RTL8196B_TR) || defined(CONFIG_RTL8196B_GW) || defined(CONFIG_RTL_8196C_GW) || defined(CONFIG_RTL8196B_KLD) || defined(CONFIG_RTL8196B_TLD) || defined(CONFIG_RTL8196C_AP_ROOT) || defined(CONFIG_RTL8198_AP_ROOT) || defined(CONFIG_RTL8196C_CLIENT_ONLY) || defined(CONFIG_RTL_8198_NFBI_BOARD) || defined(CONFIG_RTL8196C_KLD) || defined(CONFIG_RTL8196C_EC)
		#define USE_RTL8186_SDK
	#endif
#endif

#if defined(_BIG_ENDIAN_) || defined(_LITTLE_ENDIAN_)
#elif defined(_BIG_ENDIAN_) && defined(_LITTLE_ENDIAN_)
	#error "One one ENDIAN should be specified\n"
#else
	#error "Please specify your ENDIAN type!\n"
#endif

#define PCI_CONFIG_COMMAND			(wdev->conf_addr+4)
#define PCI_CONFIG_LATENCY			(wdev->conf_addr+0x0c)
#define PCI_CONFIG_BASE0			(wdev->conf_addr+0x10)
#define PCI_CONFIG_BASE1			(wdev->conf_addr+0x18)


//-------------------------------------------------------------
// Driver version information
//-------------------------------------------------------------
#define DRV_VERSION_H	1
#define DRV_VERSION_L	1
#define DRV_RELDATE		"2010-03-12"
#define DRV_NAME		"RTL8192C/RTL8188C"


//-------------------------------------------------------------
// Will check type for endian issue when access IO and memory
//-------------------------------------------------------------
#define CHECK_SWAP


//-------------------------------------------------------------
// Defined when include proc file system
//-------------------------------------------------------------
#define INCLUDE_PROC_FS
#if defined(CONFIG_PROC_FS) && defined(INCLUDE_PROC_FS)
	#define _INCLUDE_PROC_FS_
#endif


//-------------------------------------------------------------
// Debug function
//-------------------------------------------------------------
//#define _DEBUG_RTL8192CD_		// defined when debug print is used
#define _IOCTL_DEBUG_CMD_		// defined when read/write register/memory command is used in ioctl


//-------------------------------------------------------------
// Defined when internal DRAM is used for sw encryption/decryption
//-------------------------------------------------------------
#ifdef __MIPSEB__
	// disable internal ram for nat speedup
	//#define _USE_DRAM_
#endif


//-------------------------------------------------------------
// Support 8188C/8192C test chip
//-------------------------------------------------------------
#define TESTCHIP_SUPPORT


//-------------------------------------------------------------
// Support Tx Report
//-------------------------------------------------------------
#define TXREPORT
//#define LEAVESTADETECT


//-------------------------------------------------------------
// PCIe power saving function
//-------------------------------------------------------------
#ifdef CONFIG_PCIE_POWER_SAVING
#define PCIE_POWER_SAVING
#endif

#ifdef PCIE_POWER_SAVING
	#define GPIO_WAKEPIN
	#define FIB_96C
//	#define ASPM_ENABLE
	#define PCIE_POWER_SAVING_DEBUG
#ifdef PCIE_POWER_SAVING_DEBUG
	#define PCIE_L2_ENABLE
#endif
#endif


//-------------------------------------------------------------
// WDS function support
//-------------------------------------------------------------
#if defined(CONFIG_RTL_WDS_SUPPORT)
#define WDS
//	#define LAZY_WDS
#define FILTER_WDS_TXPACKET_TO_BRPORT
#endif


//-------------------------------------------------------------
// Pass EAP packet by event queue
//-------------------------------------------------------------
#define EAP_BY_QUEUE
#undef EAPOLSTART_BY_QUEUE	// jimmylin: don't pass eapol-start up
							// due to XP compatibility issue
//#define USE_CHAR_DEV
#define USE_PID_NOTIFY


//-------------------------------------------------------------
// Client mode function support
//-------------------------------------------------------------
#if defined(CONFIG_RTL_CLIENT_MODE_SUPPORT)
#define CLIENT_MODE
#endif

#ifdef CLIENT_MODE
	#define RTK_BR_EXT		// Enable NAT2.5 and MAC clone support
#endif


//-------------------------------------------------------------
// Defined when WPA2 is used
//-------------------------------------------------------------
#define RTL_WPA2
#define RTL_WPA2_PREAUTH


//-------------------------------------------------------------
// MP test
//-------------------------------------------------------------
#define MP_TEST


//-------------------------------------------------------------
// MIC error test
//-------------------------------------------------------------
//#define MICERR_TEST


//-------------------------------------------------------------
// Log event
//-------------------------------------------------------------
#define EVENT_LOG


//-------------------------------------------------------------
// Tx/Rx data path shortcut
//-------------------------------------------------------------
#define TX_SHORTCUT
#define RX_SHORTCUT
#if defined(CONFIG_RTK_MESH) && defined(RX_SHORTCUT)
#define RX_RL_SHORTCUT
#endif

#ifndef CONFIG_DISABLE_BR_SHORTCUT
#define BR_SHORTCUT
#endif

#if defined(CONFIG_RTK_MESH) && defined(TX_SHORTCUT)
	#define MESH_TX_SHORTCUT
#endif

#define TX_SC_ENTRY_NUM		1


//-------------------------------------------------------------
// back to back test
//-------------------------------------------------------------
//#define B2B_TEST

//-------------------------------------------------------------
// new auto channel
//-------------------------------------------------------------
#define CONFIG_RTL_NEW_AUTOCH

//-------------------------------------------------------------
// enable e-fuse read write
//-------------------------------------------------------------
#ifdef CONFIG_ENABLE_EFUSE
	#define EN_EFUSE
#endif

//-------------------------------------------------------------
// new IQ calibration for 92c / 88c
//-------------------------------------------------------------
#define CONFIG_RTL_NEW_IQK

//-------------------------------------------------------------
// Universal Repeater (support AP + Infra client concurrently)
//-------------------------------------------------------------
#if defined(CONFIG_RTL_REPEATER_MODE_SUPPORT)
	#define UNIVERSAL_REPEATER
#endif


//-------------------------------------------------------------
// Check hangup for Tx queue
//-------------------------------------------------------------
//#define CHECK_HANGUP
#ifdef CHECK_HANGUP
	#define CHECK_BEACON_HANGUP
	#define CHECK_BB_HANGUP
	#define FAST_RECOVERY
#endif


//-------------------------------------------------------------
// DFS
//-------------------------------------------------------------
//#define DFS


//-------------------------------------------------------------
// Driver based WPA PSK feature
//-------------------------------------------------------------
#define INCLUDE_WPA_PSK


//-------------------------------------------------------------
// RF Fine Tune
//-------------------------------------------------------------
//#define RF_FINETUNE


//-------------------------------------------------------------
// Wifi WMM
//-------------------------------------------------------------
#define WIFI_WMM
#ifdef WIFI_WMM
	#define WMM_APSD	// WMM Power Save
#endif


//-------------------------------------------------------------
// IO mapping access
//-------------------------------------------------------------
//#define IO_MAPPING


//-------------------------------------------------------------
// Wifi Simple Config support
//-------------------------------------------------------------
#define WIFI_SIMPLE_CONFIG

/* WPS2DOTX   */
#define WPS2DOTX
#define OUI_LEN					4

#ifdef WPS2DOTX
#define SUPPORT_PROBE_REQ_REASSEM	//for AP mode
#define SUPPORT_PROBE_RSP_REASSEM	// for STA mode
//#define WPS2DOTX_DEBUG
#endif

#ifdef	WPS2DOTX_DEBUG	  //0614 for wps2.0  trace
#define SECU_DEBUG(fmt, args...)  printk("[secu]%s %d:"fmt, __FUNCTION__,__LINE__, ## args)
#define SME_DEBUG(fmt, args...) printk("[sme]%s %d:"fmt,__FUNCTION__ , __LINE__ , ## args)

#else
#define SECU_DEBUG(fmt, args...)
#define SME_DEBUG(fmt, args...) 
#endif
/* WPS2DOTX   */


//-------------------------------------------------------------
// Support Multiple BSSID
//-------------------------------------------------------------
#if defined(CONFIG_RTL_VAP_SUPPORT)
#define MBSSID
#endif
#ifdef MBSSID
#define RTL8192CD_NUM_VWLAN  4
//#define FW_SW_BEACON
#else
#ifdef CONFIG_RTL8671
#define RTL8192CD_NUM_VWLAN  0
#endif
#endif

//-------------------------------------------------------------
// Support Tx Descriptor Reservation for each interface
//-------------------------------------------------------------
#if defined(CONFIG_RTL_TX_RESERVE_DESC)
#define RESERVE_TXDESC_FOR_EACH_IF
#endif

//-------------------------------------------------------------
// Group BandWidth Control
//-------------------------------------------------------------
//#define GBWC


//-------------------------------------------------------------
// Support 802.11 SNMP MIB
//-------------------------------------------------------------
//#define SUPPORT_SNMP_MIB


//-------------------------------------------------------------
// Driver-MAC loopback
//-------------------------------------------------------------
//#define DRVMAC_LB


//-------------------------------------------------------------
// Use perfomance profiling
//-------------------------------------------------------------
//#define PERF_DUMP


//-------------------------------------------------------------
// WPAI performance issue
//-------------------------------------------------------------
#ifdef CONFIG_RTL_WAPI_SUPPORT
	//#define IRAM_FOR_WIRELESS_AND_WAPI_PERFORMANCE
	#if defined(CONFIG_RTL8192CD)
	// if CONFIG_RTL_HW_WAPI_SUPPORT defined, { SWCRYPTO=1: sw wapi; SWCRYPTO=0: hw wapi. }
	// if CONFIG_RTL_HW_WAPI_SUPPORT not defined, { SWCRYPTO=1: sw wapi;  SWCRYPTO=0: should not be used! }
	#define	CONFIG_RTL_HW_WAPI_SUPPORT		1
	#endif
#endif


//-------------------------------------------------------------
// Software High Queue
//-------------------------------------------------------------
#define SW_MCAST


//-------------------------------------------------------------
// Use local ring for pre-alloc Rx buffer.
// If no defined, will use kernel skb que
//-------------------------------------------------------------
#define RTK_QUE


//-------------------------------------------------------------
//Support IP multicast->unicast
//-------------------------------------------------------------
#define SUPPORT_TX_MCAST2UNI
#define SUPPORT_RX_UNI2MCAST

/* for cameo feature*/
#ifdef CONFIG_RTL865X_CMO
	#define IGMP_FILTER_CMO
#endif

// Support  IPV6 multicast->unicast
#ifdef	SUPPORT_TX_MCAST2UNI
	#define	TX_SUPPORT_IPV6_MCAST2UNI
#endif


//-------------------------------------------------------------
// Support  USB tx rate adaptive
//-------------------------------------------------------------
// define it always for object code release
#ifdef CONFIG_USB
	#define USB_PKT_RATE_CTRL_SUPPORT
#endif


//-------------------------------------------------------------
// Support Tx AMSDU
//-------------------------------------------------------------
//#define SUPPORT_TX_AMSDU


//-------------------------------------------------------------
// Mesh Network
//-------------------------------------------------------------
#ifdef CONFIG_RTK_MESH
#define _MESH_ACL_ENABLE_

/*need check Tx AMSDU dependency ; 8196B no support now */
#ifdef	SUPPORT_TX_AMSDU
#define MESH_AMSDU
#endif
//#define MESH_ESTABLISH_RSSI_THRESHOLD
//#define MESH_BOOTSEQ_AUTH
#endif // CONFIG_RTK_MESH


//-------------------------------------------------------------
// Support buffered Tx ampdu. Not define in default
//-------------------------------------------------------------
//#define BUFFER_TX_AMPDU


//-------------------------------------------------------------
// Realtek proprietary wake up on wlan mode
//-------------------------------------------------------------
//#define RTK_WOW


//-------------------------------------------------------------
// Support 802.11d
//-------------------------------------------------------------
//#define DOT11D


//-------------------------------------------------------------
// Use static buffer for STA private buffer
//-------------------------------------------------------------
//#define PRIV_STA_BUF


//-------------------------------------------------------------
// Do not drop packet immediately when rx buffer empty
//-------------------------------------------------------------
#ifdef CONFIG_RTL8190_PRIV_SKB
	#define DELAY_REFILL_RX_BUF
#endif


//-------------------------------------------------------------
// WiFi 11n 20/40 coexistence
//-------------------------------------------------------------
#define WIFI_11N_2040_COEXIST


//-------------------------------------------------------------
// Add TX power by command
//-------------------------------------------------------------
#define ADD_TX_POWER_BY_CMD


//-------------------------------------------------------------
// Merge firmware and phy parameter files into binary
//-------------------------------------------------------------
#define MERGE_FW


//-------------------------------------------------------------
// Do Rx process in tasklet
//-------------------------------------------------------------
//#define RX_TASKLET


//-------------------------------------------------------------
// Support external high power PA
//-------------------------------------------------------------
#ifdef CONFIG_HIGH_POWER_EXT_PA
#define HIGH_POWER_EXT_PA
#endif



//-------------------------------------------------------------
// Cache station info for bridge
//-------------------------------------------------------------
//#define RTL_CACHED_BR_STA


//-------------------------------------------------------------
// Use default keys of WEP (instead of keymapping keys)
//-------------------------------------------------------------
#define USE_WEP_DEFAULT_KEY

//-------------------------------------------------------------
// Concurrent support up to 64 clients
//-------------------------------------------------------------
//#define STA_EXT

//-------------------------------------------------------------
// Rx buffer gather feature
//-------------------------------------------------------------
#ifdef CONFIG_ENABLE_RX_BUFFER_GATHER
#define RX_BUFFER_GATHER
#endif




/*********************************************************************/
/* some definitions in 8192cd driver, we set them as NULL definition */
/*********************************************************************/
#ifdef USE_RTL8186_SDK
#ifdef CONFIG_WIRELESS_LAN_MODULE
#define __DRAM_IN_865X
#define __IRAM_IN_865X
#else
#define __DRAM_IN_865X		__attribute__ ((section(".dram-rtkwlan")))
#define __IRAM_IN_865X		__attribute__ ((section(".iram-rtkwlan")))
#endif

#define RTL8190_DIRECT_RX						/* For packet RX : directly receive the packet instead of queuing it */
#define RTL8190_ISR_RX							/* process RXed packet in interrupt service routine: It become useful only when RTL8190_DIRECT_RX is defined */

#ifndef CONFIG_WIRELESS_LAN_MODULE
#define RTL8192CD_VARIABLE_USED_DMEM			/* Use DMEM for some critical variables */
#endif

#else // not USE_RTL8186_SDK

#define __DRAM_IN_865X
#define __IRAM_IN_865X
#endif


#undef __MIPS16
//#define __MIPS16
#define __MIPS16			__attribute__ ((mips16))

#ifdef IRAM_FOR_WIRELESS_AND_WAPI_PERFORMANCE
#define __IRAM_WLAN_HI		__attribute__ ((section(".iram-wapi")))
#define __DRAM_WLAN_HI		__attribute__ ((section(".dram-wapi")))
#endif


//-------------------------------------------------------------
// Kernel 2.6 specific config
//-------------------------------------------------------------
#ifdef __LINUX_2_6__

#define USE_RLX_BSP

#define RTL8192CD_VARIABLE_USED_DMEM

#ifndef RX_TASKLET
	#define	RX_TASKLET
#endif

#endif


#if 0
//-------------------------------------------------------------
// TR define flag
//-------------------------------------------------------------
#if defined(CONFIG_RTL8196B_TR) || defined(CONFIG_RTL8196C_EC)

#ifndef INCLUDE_WPA_PSK
	#define INCLUDE_WPA_PSK
#endif

#ifdef UNIVERSAL_REPEATER
	#undef UNIVERSAL_REPEATER
#endif

#ifdef CLIENT_MODE
	#undef CLIENT_MODE
	#undef RTK_BR_EXT
#endif

#ifndef WIFI_SIMPLE_CONFIG
	#define WIFI_SIMPLE_CONFIG
#endif

#ifdef GBWC
	#undef GBWC
#endif

#ifdef SUPPORT_SNMP_MIB
	#undef SUPPORT_SNMP_MIB
#endif

#endif // CONFIG_RTL8196B_TR


//-------------------------------------------------------------
// AC define flag
//-------------------------------------------------------------
#ifdef CONFIG_RTL865X_AC

#ifndef INCLUDE_WPA_PSK
	#define INCLUDE_WPA_PSK
#endif

#ifdef UNIVERSAL_REPEATER
	#undef UNIVERSAL_REPEATER
#endif

#ifdef CLIENT_MODE
	#undef CLIENT_MODE
	#undef RTK_BR_EXT
#endif

#ifndef WIFI_SIMPLE_CONFIG
	#define WIFI_SIMPLE_CONFIG
#endif

#ifdef GBWC
	#undef GBWC
#endif

#ifdef SUPPORT_SNMP_MIB
	#undef SUPPORT_SNMP_MIB
#endif

#endif // CONFIG_RTL865X_AC


//-------------------------------------------------------------
// X86 define flag
//-------------------------------------------------------------
#ifdef CONFIG_X86

#ifndef CONFIG_NET_PCI
	#define CONFIG_NET_PCI
#endif

#ifdef __MIPSEB__
	#undef __MIPSEB__
#endif

#ifdef _BIG_ENDIAN_
	#undef _BIG_ENDIAN_
#endif

#ifndef _LITTLE_ENDIAN_
	#define _LITTLE_ENDIAN_
#endif

#ifdef _USE_DRAM_
	#undef _USE_DRAM_
#endif

#ifdef CHECK_SWAP
	#undef CHECK_SWAP
#endif

#ifdef EVENT_LOG
	#undef EVENT_LOG
#endif

#ifdef BR_SHORTCUT
	#undef BR_SHORTCUT
#endif

#ifdef RTK_BR_EXT
	#undef RTK_BR_EXT
#endif

#ifdef UNIVERSAL_REPEATER
	#undef UNIVERSAL_REPEATER
#endif

#ifdef GBWC
	#undef GBWC
#endif

#ifndef __LINUX_2_6__
	#define del_timer_sync del_timer
#endif

#ifdef USE_IO_OPS
	#undef USE_IO_OPS
#endif

#ifdef IO_MAPPING
	#undef IO_MAPPING
#endif

#ifdef MBSSID
	#undef MBSSID
#endif

#ifdef RTK_QUE
	#undef RTK_QUE
#endif

#endif // CONFIG_X86
#endif


//-------------------------------------------------------------
// Define flag of EC system
//-------------------------------------------------------------
#ifdef CONFIG_RTL8196C_EC

#ifndef USE_WEP_DEFAULT_KEY
	#define USE_WEP_DEFAULT_KEY
#endif

#ifdef TESTCHIP_SUPPORT
	#undef TESTCHIP_SUPPORT
#endif

#endif 


//-------------------------------------------------------------
// Define flag of 8672 system
//-------------------------------------------------------------
#ifdef CONFIG_RTL8676

#undef DRV_RELDATE
#define DRV_RELDATE		"2010-03-31/2012-04-09"

#ifndef RX_TASKLET
	#define RX_TASKLET
#endif

#ifdef RTL8190_DIRECT_RX
	#undef RTL8190_DIRECT_RX
#endif

#ifdef RTL8190_ISR_RX
	#undef RTL8190_ISR_RX
#endif

#if defined(USE_RLX_BSP) && !defined(LINUX_2_6_22_)
	#undef USE_RLX_BSP
#endif

#undef TX_SC_ENTRY_NUM
#define TX_SC_ENTRY_NUM		2

#ifdef __DRAM_IN_865X
	#undef __DRAM_IN_865X
#endif
#define __DRAM_IN_865X		__attribute__ ((section(".dram-rtkwlan")))

#ifdef __IRAM_IN_865X
	#undef __IRAM_IN_865X
#endif
#define __IRAM_IN_865X		__attribute__ ((section(".iram-rtkwlan")))

#ifdef __IRAM_IN_865X_HI
	#undef __IRAM_IN_865X_HI
#endif
#define __IRAM_IN_865X_HI	__attribute__ ((section(".iram-tx")))


#ifdef PCIE_POWER_SAVING
	#define GPIO_ABCD_IRQ 16
	// enable it if you want to implement system wide power saving.
	//#define PCIE_POWER_SAVING_NOTIFY_SYSTEM
#endif

#define USE_TXQUEUE
#ifdef USE_TXQUEUE
	#define TXQUEUE_SIZE	512
#endif

#ifdef PCIE_POWER_SAVING
	#define RTL8676_WAKE_GPIO
#endif

#ifdef __MIPS16
	#undef __MIPS16
	#define __MIPS16
#endif
#endif // CONFIG_RTL8676


#if 0
//-------------------------------------------------------------
// TLD define flag
//-------------------------------------------------------------
#ifdef CONFIG_RTL8196B_TLD

#ifndef STA_EXT
	#define STA_EXT
#endif

#ifdef GBWC
	#undef GBWC
#endif

#ifdef SUPPORT_SNMP_MIB
	#undef SUPPORT_SNMP_MIB
#endif

#ifdef DRVMAC_LB
	#undef DRVMAC_LB
#endif

#ifdef HIGH_POWER_EXT_PA
	#undef HIGH_POWER_EXT_PA
#endif

#ifdef ADD_TX_POWER_BY_CMD
	#undef ADD_TX_POWER_BY_CMD
#endif

#endif // CONFIG_RTL8196B_TLD
#endif



//-------------------------------------------------------------
// KLD define flag
//-------------------------------------------------------------
#if defined(CONFIG_RTL8196C_KLD)

#ifndef INCLUDE_WPA_PSK
	#define INCLUDE_WPA_PSK
#endif

#ifdef UNIVERSAL_REPEATER
	#undef UNIVERSAL_REPEATER
#endif

#ifndef WIFI_SIMPLE_CONFIG
	#define WIFI_SIMPLE_CONFIG
#endif

#ifdef GBWC
	#undef GBWC
#endif

#ifdef SUPPORT_SNMP_MIB
	#undef SUPPORT_SNMP_MIB
#endif

#ifdef DRVMAC_LB
	#undef DRVMAC_LB
#endif

#ifdef MBSSID
	#undef RTL8192CD_NUM_VWLAN
	#define RTL8192CD_NUM_VWLAN  1
#endif

//#ifdef HIGH_POWER_EXT_PA
//	#undef HIGH_POWER_EXT_PA
//#endif

//#ifdef ADD_TX_POWER_BY_CMD
//	#undef ADD_TX_POWER_BY_CMD
//#endif

#endif // CONFIG_RTL8196C_KLD


//-------------------------------------------------------------
// Dependence check of define flag
//-------------------------------------------------------------
#if defined(B2B_TEST) && !defined(MP_TEST)
	#error "Define flag error, MP_TEST is not defined!\n"
#endif


#if defined(UNIVERSAL_REPEATER) && !defined(CLIENT_MODE)
	#error "Define flag error, CLIENT_MODE is not defined!\n"
#endif


/*=============================================================*/
/*------ Compiler Portability Macros --------------------------*/
/*=============================================================*/
#ifdef EVENT_LOG
	extern int scrlog_printk(const char * fmt, ...);
#ifdef CONFIG_RTK_MESH
/*
 *	NOTE: dot1180211sInfo.log_enabled content from webpage MIB_LOG_ENABLED (bitmap) (in AP/goahead-2.1.1/LINUX/fmmgmt.c  formSysLog)
 */
	#define _LOG_MSG(fmt, args...)	if (1 & GET_MIB(priv)->dot1180211sInfo.log_enabled) scrlog_printk(fmt, ## args)
	#define LOG_MESH_MSG(fmt, args...)	if (16 & GET_MIB(priv)->dot1180211sInfo.log_enabled) _LOG_MSG("%s: " fmt, priv->mesh_dev->name, ## args)
#else
	#define _LOG_MSG(fmt, args...)	scrlog_printk(fmt, ## args)
#endif
#if defined(CONFIG_RTL8196B_TR) || defined(CONFIG_RTL8196C_EC)
	#define _NOTICE	"NOTICElog_num:13;msg:"
	#define _DROPT	"DROPlog_num:13;msg:"
	#define _SYSACT "SYSACTlog_num:13;msg:"

	#define LOG_MSG_NOTICE(fmt, args...) _LOG_MSG("%s" fmt, _NOTICE, ## args)
	#define LOG_MSG_DROP(fmt, args...) _LOG_MSG("%s" fmt, _DROPT, ## args)
	#define LOG_MSG_SYSACT(fmt, args...) _LOG_MSG("%s" fmt, _SYSACT, ## args)
	#define LOG_MSG(fmt, args...)	{}

	#define LOG_START_MSG() { \
			char tmpbuf[10]; \
			LOG_MSG_NOTICE("Access Point: %s started at channel %d;\n", \
				priv->pmib->dot11StationConfigEntry.dot11DesiredSSID, \
				priv->pmib->dot11RFEntry.dot11channel); \
			if (priv->pmib->dot11StationConfigEntry.autoRate) \
				strcpy(tmpbuf, "best"); \
			else \
				sprintf(tmpbuf, "%d", get_rate_from_bit_value(priv->pmib->dot11StationConfigEntry.fixedTxRate)/2); \
			LOG_MSG_SYSACT("AP 2.4GHz mode Ready. Channel : %d TxRate : %s SSID : %s;\n", \
				priv->pmib->dot11RFEntry.dot11channel, \
				tmpbuf, priv->pmib->dot11StationConfigEntry.dot11DesiredSSID); \
	}

#elif defined(CONFIG_RTL865X_AC) || defined(CONFIG_RTL865X_KLD) || defined(CONFIG_RTL8196B_KLD) || defined(CONFIG_RTL8196C_KLD)
	#define _NOTICE	"NOTICElog_num:13;msg:"
	#define _DROPT	"DROPlog_num:13;msg:"
	#define _SYSACT "SYSACTlog_num:13;msg:"

	#define LOG_MSG_NOTICE(fmt, args...) _LOG_MSG("%s" fmt, _NOTICE, ## args)
	#define LOG_MSG_DROP(fmt, args...) _LOG_MSG("%s" fmt, _DROPT, ## args)
	#define LOG_MSG_SYSACT(fmt, args...) _LOG_MSG("%s" fmt, _SYSACT, ## args)
	#define LOG_MSG(fmt, args...)	{}

	#define LOG_START_MSG() { \
			char tmpbuf[10]; \
			LOG_MSG_NOTICE("Access Point: %s started at channel %d;\n", \
				priv->pmib->dot11StationConfigEntry.dot11DesiredSSID, \
				priv->pmib->dot11RFEntry.dot11channel); \
			if (priv->pmib->dot11StationConfigEntry.autoRate) \
				strcpy(tmpbuf, "best"); \
			else \
				sprintf(tmpbuf, "%d", get_rate_from_bit_value(priv->pmib->dot11StationConfigEntry.fixedTxRate)/2); \
			LOG_MSG_SYSACT("AP 2.4GHz mode Ready. Channel : %d TxRate : %s SSID : %s;\n", \
				priv->pmib->dot11RFEntry.dot11channel, \
				tmpbuf, priv->pmib->dot11StationConfigEntry.dot11DesiredSSID); \
	}
#elif defined(CONFIG_RTL8196B_TLD)
	#define LOG_MSG_DEL(fmt, args...)	_LOG_MSG(fmt, ## args)
	#define LOG_MSG(fmt, args...)	{}
#else
	#define LOG_MSG(fmt, args...)	_LOG_MSG("%s: "fmt, priv->dev->name, ## args)
#endif
#else
	#if defined(__GNUC__) || defined(GREEN_HILL)
		#define LOG_MSG(fmt, args...)	{}
	#else
		#define LOG_MSG
	#endif
#endif // EVENT_LOG

#ifdef _USE_DRAM_
	#define DRAM_START_ADDR		0x81000000	// start address of internal data ram
#endif

#ifdef __GNUC__
#define __WLAN_ATTRIB_PACK__		__attribute__ ((packed))
#define __PACK
#endif

#ifdef __arm
#define __WLAN_ATTRIB_PACK__
#define __PACK	__packed
#endif

#ifdef GREEN_HILL
#define __WLAN_ATTRIB_PACK__
#define __PACK
#endif


/*=============================================================*/
/*-----------_ Driver module flags ----------------------------*/
/*=============================================================*/
#ifdef CONFIG_WIRELESS_LAN_MODULE
	#define	MODULE_NAME		"Realtek WirelessLan Driver"
	#define	MODULE_VERSION	"v1.00"

	#define MDL_DEVINIT
	#define MDL_DEVEXIT
	#define MDL_INIT
	#define MDL_EXIT
	#define MDL_DEVINITDATA
#else
	#define MDL_DEVINIT		__devinit
	#define MDL_DEVEXIT		__devexit
	#define MDL_INIT		__init
	#define MDL_EXIT		__exit
	#define MDL_DEVINITDATA	__devinitdata
#endif


/*=============================================================*/
/*----------- System configuration ----------------------------*/
/*=============================================================*/
#if defined(CONFIG_RTL8196B_GW_8M)
#define NUM_TX_DESC		200
#else
#ifdef CONFIG_RTL8198
#define NUM_TX_DESC		512
#else
#define NUM_TX_DESC		256		// kmalloc max size issue
#endif
#endif

#ifdef CONFIG_RTL8198
#define NUM_RX_DESC		128
#else

#ifdef RX_BUFFER_GATHER
#define NUM_RX_DESC		64
#else
#define NUM_RX_DESC		32
#endif
#endif

#ifdef DELAY_REFILL_RX_BUF
#define REFILL_THRESHOLD	NUM_RX_DESC
#endif

#define FW_NUM_STAT 32

#if (defined(CONFIG_RTL8196B_KLD) || defined(CONFIG_RTL8196C_KLD)) && defined(MBSSID)
#define NUM_CMD_DESC	2
#else
#define NUM_CMD_DESC	16
#endif

#ifdef STA_EXT
#define NUM_STAT		64
#else
#define NUM_STAT		32
#endif
#define MAX_GUEST_NUM   NUM_STAT

#define NUM_TXPKT_QUEUE			64
#define NUM_APSD_TXPKT_QUEUE	32

#define PRE_ALLOCATED_HDR		NUM_TX_DESC
#ifdef DRVMAC_LB
#define PRE_ALLOCATED_MMPDU		32
#define PRE_ALLOCATED_BUFSIZE	(2048/4)		// 600 bytes long should be enough for mgt! Declare as unsigned int
#else
#define PRE_ALLOCATED_MMPDU		64
#define PRE_ALLOCATED_BUFSIZE	((600+128)/4)		// 600 bytes long should be enough for mgt! Declare as unsigned int
#endif

#define MAX_BSS_NUM		64


#define MAX_NUM_WLANIF		4
#define WLAN_MISC_MAJOR		13

#define MAX_FRAG_COUNT		10

#define NUM_MP_SKB		32

// unit of time out: 10 msec, Use HZ to replace
#define AUTH_TO			(5*HZ) //500
#define ASSOC_TO		(5*HZ) //500
#define FRAG_TO			(20*HZ) //2000
#define SS_TO			(msecs_to_jiffies(50))//5
#define SS_PSSV_TO		(msecs_to_jiffies(120))//12			// passive scan for 120 ms

#ifdef CONFIG_RTL_NEW_AUTOCH
#define SS_AUTO_CHNL_TO		(msecs_to_jiffies(200))	//20
#endif

#ifdef CONFIG_RTK_MESH
//GANTOE for automatic site survey 2008/12/10
#define SS_RAND_DEFER		300
#endif
#ifdef LINUX_2_6_22_
#define EXPIRE_TO		HZ
#else
#define EXPIRE_TO		HZ//100	//this is time
#endif
#define REAUTH_TO		(5*HZ)//500	//this is time
#define REASSOC_TO		(5*HZ)//500	//this is time
#define REAUTH_LIMIT	6
#define REASSOC_LIMIT	6

#define DEFAULT_OLBC_EXPIRE		60	//this is time (seconds)

#define GBWC_TO			(msecs_to_jiffies(250)) //25

#ifdef __DRAYTEK_OS__
#define SS_COUNT		2
#else
#define SS_COUNT		3
#endif

#define TUPLE_WINDOW	64

#define RC_TIMER_NUM	64
#define RC_ENTRY_NUM	128
#define AMSDU_TIMER_NUM	64
#define AMPDU_TIMER_NUM	64

#define ROAMING_DECISION_PERIOD_INFRA	5
#define ROAMING_DECISION_PERIOD_ADHOC	10
#define ROAMING_DECISION_PERIOD_ARRAY (ROAMING_DECISION_PERIOD_ADHOC+1)
#define ROAMING_THRESHOLD		1	// roaming will be triggered when rx
									// beacon percentage is less than the value
#define FAST_ROAMING_THRESHOLD	40

/* below is for security.h  */
#define MAXDATALEN		1560
#define MAXQUEUESIZE	8	//WPS2DOTX
#define MAXRSNIELEN		128
#define E_DOT11_2LARGE	-1
#define E_DOT11_QFULL	-2
#define E_DOT11_QEMPTY	-3
#ifdef WIFI_SIMPLE_CONFIG
#define PROBEIELEN		260
#endif

// for SW LED
#define LED_MAX_PACKET_CNT_B	400
#define LED_MAX_PACKET_CNT_AG	1200
#define LED_MAX_SCALE			HZ    //100
#define LED_NOBLINK_TIME		(msecs_to_jiffies(1100))	//110	// time more than watchdog interval
#define LED_INTERVAL_TIME		(msecs_to_jiffies(500))	//50	// 500ms
#define LED_ON_TIME				(msecs_to_jiffies(40))	//4	// 40ms
#define LED_ON					0
#define LED_OFF					1
#define LED_0 					0
#define LED_1 					1
#define LED_2					2

// for counting association number
#define INCREASE		1
#define DECREASE		0

// DFS
#define CH_AVAIL_CHK_TO			(62*HZ)//6200	 // 62 seconds


#ifdef RX_BUFFER_GATHER
#ifdef __LINUX_2_6__
#define MAX_SKB_BUF	2260//2256
#else
#define MAX_SKB_BUF	2048
#endif
#define MAX_RX_BUF_LEN	(MAX_SKB_BUF -sizeof(struct skb_shared_info) - 128)
#define MIN_RX_BUF_LEN MAX_RX_BUF_LEN	

#else
#define MAX_RX_BUF_LEN	8400
//#define MIN_RX_BUF_LEN	4300
#define MIN_RX_BUF_LEN	4400
#endif

/* for RTL865x suspend mode */
#define TP_HIGH_WATER_MARK 50 //80 /* unit: Mbps */
#define TP_LOW_WATER_MARK 30 //40 /* unit: Mbps */

#define FW_BOOT_SIZE	400
#define FW_MAIN_SIZE	52000
#define FW_DATA_SIZE	850
#define AGC_TAB_SIZE	1600

#define MAC_REG_SIZE	1024
#define	PHY_REG_SIZE	2048
#define PHY_REG_PG_SIZE 256
#define PHY_REG_1T2R	256
#define PHY_REG_1T1R	256
#define FW_IMEM_SIZE	40*(1024)
#define FW_EMEM_SIZE	50*(1024)
#define FW_DMEM_SIZE	48

// for PCIe power saving
#define POWER_DOWN_T0	(10*HZ)
#define PKT_PAGE_SZ 	128
#define TX_DESC_SZ 		32


#ifdef SUPPORT_TX_MCAST2UNI
#define MAX_IP_MC_ENTRY		8
#endif

#define IQK_ADDA_REG_NUM	16
#define MAX_TOLERANCE		5
#define	IQK_DELAY_TIME		1		//ms
#define IQK_MAC_REG_NUM		4

//Analog Pre-distortion calibration
#define		APK_BB_REG_NUM	5
#define		APK_AFE_REG_NUM	16
#define		APK_CURVE_REG_NUM 4
#define		PATH_NUM		2

// for dynamic mechanism of reserving tx desc
#if defined(RESERVE_TXDESC_FOR_EACH_IF) && (defined(UNIVERSAL_REPEATER) || defined(MBSSID))
#define IF_TXDESC_UPPER_LIMIT	70	// percentage
#ifdef USE_TXQUEUE
#define IF_TXQ_UPPER_LIMIT		85	// percentage
#endif
#endif

//-------------------------------------------------------------
// Define flag for 8M gateway configuration
//-------------------------------------------------------------
#if defined(CONFIG_RTL8196B_GW_8M)

//#ifdef MERGE_FW
//	#undef MERGE_FW
//#endif
//#define DW_FW_BY_MALLOC_BUF

#ifdef MBSSID
	#undef RTL8192CD_NUM_VWLAN
	#define RTL8192CD_NUM_VWLAN  1
#endif

#undef NUM_STAT
#define NUM_STAT		16

#endif // CONFIG_RTL8196B_GW_8M


#if defined(CONFIG_RTL8196C_AP_ROOT) || defined(CONFIG_RTL8198_AP_ROOT)

#ifdef DOT11D
	#undef DOT11D
#endif


#ifdef MBSSID
	#undef RTL8192CD_NUM_VWLAN
	#define RTL8192CD_NUM_VWLAN  1
#endif

#undef NUM_STAT
#define NUM_STAT		16


#endif //defined(CONFIG_RTL8196C_AP_ROOT)



#if defined(CONFIG_RTL8196C_CLIENT_ONLY)

#ifdef DOT11D
	#undef DOT11D
#endif

#endif

#if defined(CONFIG_RTL8676) && defined(LINUX_2_6_22_) && defined(CONFIG_MEM_LIMITATION)
	#undef NUM_STAT
	#define NUM_STAT	16
#endif

#endif // _8192CD_CFG_H_

