/**************************************************************
$Workfile:   rmmapp.h  $
***************************************************************
**
** Host Application, Copyright (C) 2000-2002
** Ikanos Communication, Fremont, CA 
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of 
** Ikanos Communication
**
***************************************************************

$Revision: 1.1 $
$Date: 2008/07/18 11:08:39 $
**************************************************************/

#ifndef RMMAPPH
#define RMMAPPH

#include "apitype.h"
#include "apistruct.h"
#include "eocapi.h"
#include "eocapp.h"

typedef enum eDeviceChoice {
	eHOST_VERISON,
	eBME_FW_VERSION,
	eBME_HW_VERSION,
	eAFE_VERSION,
	eIFE_VERSION,
	eNO_DEVICE
} eDeviceChoice;

typedef enum eModemInfo {
	eBME_ID,
	eIFE_ID,
	eAFE_ID,
	eNetwork_IF,
	eVendor_ID,
	eNo_Info
} eModemInfo;
extern const char	*getDeviceVersion( int bmeNum, eDeviceChoice eDesired );
extern void	 	 storeDeviceVersions(
		ipos_port_id portId,
		U8 UID,
		U32 cntRpt,
		int status,
		ipos_bme_ver *par
);

extern int autoEnetRptEnable;

#define IPOSRMM_GET			      1
#define IPOSRMM_SET			      2
#define IPOSRMM_ILLEGAL_REG   3
#define NOT_A_REGISTER        255

typedef enum
{
	FIRMWARE_VERSION_INFO     =	2,
	ETHERNET_STATUS_INFO	    = 3,
	ERRORED_SECONDS_INFO	    =	4,
  LIST_RMM_IE_TYPES		      = 5,

  RMM_IE_TYPE_SERIES		    = 5,
	REBOOT_FIRMWARE_INFO	    =	6,
	REBOOT_STATUS_INFO		    =	7,
	MDIO_REGISTER_LIST		    =	12,
	GPIO_PIN_LIST				      = 13,
	MDIO_REGISTER_VALUES	    =	14,
	MDIO_FPGA_REGISTER_VALUES =	15,
	SET_MDIOFPGAREG_STATUS    = 16,
	SET_PHY_LOOPBACK_LEVEL		= 18,
	SET_PHY_LOOPBACK_STATUS		= 20,
	SET_MDIOREG_STATUS        = 21,
	MODEM_ID_INFO				      = 22,
	SET_ENET_FLOW_CTRL			  = 23,
	SET_ENET_FLOW_CTRL_STATUS = 24,
	CLEAR_REMOTE_COUNTERS		  = 25,
	CLEAR_REMOTE_CNT_STATUS		= 26,

  SET_PHY_PARAMS            = 27,
  SET_PHY_PARAMS_STATUS     = 28,
  GET_ENET_CFG              = 29,
  REMOTE_EXT_STATUS         = 30,
  SET_AUTOENET_STATUS_RPT   = 31,
  GET_AUTOENET_RPT_FLAG     = 32,

  ENET_ADDRESS_FILTERING    = 40

} eIeTypeCode;

typedef enum
{
  HOST_VERSION_INFO		= 2,
  BME_VERSION_INFO		= 3,
  AFE_VERSION_INFO		= 4,
  IFE_VERSION_INFO		= 5,
  BME_HW_VERSION_INFO	=	6

} eIeSubTypeVersionInfo; //FIRMWARE_VERSION_INFO = 2

typedef enum
{
  LINK_STATUS			    = 2,
  DUPLEX_STATUS			  = 3,
  TRANSMISSION_SPEED	=	4,
  PAUSE_FRAME_STATUS	=	5,
  COUNT_PAUSE_FRAMES	=	6

} eIeSubTypeEnetStatus;  //ETHERNET_STATUS_INFO = 3

typedef enum
{
  SECONDS_WITH_CRC	=	2,
  SECONDS_WITH_LOS	=	3,
  SECONDS_WITH_SEF	=	4,
  SECONDS_WITH_LOF	=	5

} eIeSubTypeErroredSecInfo;  //ERRORED_SECONDS_INFO = 4

typedef enum
{
  REBOOT_OK			          = 0,
  INVALID_FIRMWARE		    = 2,
  WATCHDOG_DISABLED		    = 3,
  INCOMPATIBLE_BOOTLOADER	= 12	/* for compatibility with RFM (rfm/rfmerr.h) */

} eIeSubTypeRebootStatus;  //REBOOT_STATUS_INFO = 7

typedef enum
{
  GPIO_PIN_NO        = 2,
  GPIO_PIN_MASK      = 3,
  GPIO_PIN_VALUE     = 4

} eIeSubTypeGpioPin;  //GPIO_PIN_LIST	 = 13,

typedef enum
{
  BME_ID_INFO             = 7,
  IFE_ID_INFO             = 8,
  AFE_ID_INFO             = 9,
  NETWORK_INTERFACE_INFO  = 10, 
  VENDOR_ID_INFO          = 11,
  MANUFATURING_ID_INFO    = 12,
  MODEL_NUMBER_INFO       = 13,
  SERIAL_NUMBER_INFO      = 14,
  BOOTLOADER_VERSION_INFO = 15

} eIeSubTypeModemId;  //	MODEM_ID_INFO				      = 22,

typedef enum
{
  AUTO_NEGOTIATION      =   2,
  ETHERNET_SPEED        =   3,
  ETHERNET_LINK         =   4,
  ETHERNET_AGC          =   5,
  ETHERNET_FLOW_CTRL    =   6,
  ETHERNET_LPBACK       =   7

} eIeSubTypeEnetPhyParam;  //  SET_PHY_PARAMS            = 27,

typedef enum
{
  CONFIG_FLAGS            = 2,
  STATIC_ADDRESS          = 3,
  LEARNED_ADDRESS         = 4

} eIeSubTypeEnetAddrFilter;  //ENET_ADDRESS_FILTERING    = 40,

#define   UNKNOWN_STATUS		      0

#define   LINK_IS_DOWN			      1
#define   LINK_IS_UP			        2

#define   HALF_DUPLEX			        1
#define   FULL_DUPLEX			        2

#define   TEN_BASE_T			        1
#define   HUNDRED_BASE_T	        2

#define   FLOW_CONTROL_DISABLED		1
#define   FLOW_CONTROL_ENABLED		2

#define   RMM_RESEND_INTERVAL  		7
#define   RMM_RESEND_INTERVAL1  	15 	

#define   TX_POWER                 2

#define RMM_ERROR_BASE			0xb000
#define RMM_INCOMPATIBLE_BOOTLOADER	(RMM_ERROR_BASE + INCOMPATIBLE_BOOTLOADER)


/*These are the address of the Remote FPGA Direct Address and Data
 * Register */
#define FPGA_DIRECT_ADDRESS_REGISTER   19
#define FPGA_DIRECT_DATA_REGISTER      20 

typedef struct {
        ipos_ctrl_hdr   ctrlHdr;
	U8		ieTypeList[ 32 ];
} rmm_get_req;

typedef struct {
        ipos_ctrl_hdr   ctrlHdr;
        U8              firmwareChoice;
} rmm_reboot_firmware;

typedef struct {
        ipos_ctrl_hdr   ctrlHdr;
        U8              status;
} rmm_reboot_status;

extern void	uiShowDeviceVersions( int bmeNum );
extern void	uiGetRemoteVersion( ipos_port_id pid );
extern void	uiGetRemoteErroredSeconds( ipos_port_id pid );
extern void	uiRemoteReboot( ipos_port_id pid, U8 firmwareChoice );
extern void uiGetRemoteMDIORegs( ipos_port_id pid );

#endif /* RMMAPPH */
