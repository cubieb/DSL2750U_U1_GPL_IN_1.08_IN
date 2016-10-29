/******************************************************************
 Copyright (c), 1991-2007, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: devinfo.h
 文件描述:
    Nodes below are managed by this module:
   	<DeviceInfo>
		<Manufacturer>TongWei</Manufacturer>
		<ManufacturerOUI>00E0A6</ManufacturerOUI>
		<ModelName>TBS</ModelName>
		<Description>Tongwei Broadband System</Description>
		<ProductClass>IGD</ProductClass>
		<SerialNumber>001EE3010203</SerialNumber>
		<HardwareVersion>Unknown</HardwareVersion>
		<SoftwareVersion>TBS-R2B01</SoftwareVersion>
		<ModemFirmwareVersion>1.0.0</ModemFirmwareVersion>
		<EnabledOptions> </EnabledOptions>
		<AdditionalHardwareVersion> </AdditionalHardwareVersion>
		<AdditionalSoftwareVersion> </AdditionalSoftwareVersion>
		<SpecVersion> </SpecVersion>
		<ProvisioningCode> </ProvisioningCode>
		<UpTime> </UpTime>
		<FirstUseDate> </FirstUseDate>
		<DeviceLog> </DeviceLog>
		<VendorConfigFileNumberOfEntries> </VendorConfigFileNumberOfEntries>
		<VendorConfigFile>
			<i>
				<Name> </Name>
				<Version> </Version>
				<Date> </Date>
				<Description> </Description>
			</i>
		</VendorConfigFile>
	</DeviceInfo>
 修订记录:
        1. 作者: Huangjidong@twsz.com
           日期: 2008年12月02日
           内容: 创建文件

******************************************************************/


#ifndef _DEVINFO_H_
#define _DEVINFO_H_

#include "common_msg_handler.h"
#include "warnlog.h"

/******************************************************************************
*                                 MACRO                                      *
******************************************************************************/


#define BASE_PATH_DEVINFO "^InternetGatewayDevice\\.DeviceInfo\\."
#define PATH_DEVINFO "InternetGatewayDevice.DeviceInfo."



#define MAC_LEN 12


/* 日志打印宏 */
#ifdef DEVINFO_DEBUG
#define DEVINFO_TRACE(fmt, args...)        TBS_TRACE(MID_DEVINFO, fmt, ##args)
#else
#define DEVINFO_TRACE(fmt, args...)
#endif


/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/


int DEVINFO_UpdateSN( void );
int DEVINFO_UpdateUptime( void );
#if defined(CONFIG_TELEFONICA_PERU) || defined(CONFIG_TELEFONICA_CHILE) || defined(CONFIG_TELEFONICA_COLOMBIA) || defined(CONFIG_TELEFONICA_SPAIN) || defined(CONFIG_RTL8673) || defined(CONFIG_TELEFONICA_BRAZIL)
int DEVINFO_UpdateDoubleMemoryStatus(void);
#endif
#ifdef CONFIG_TELEFONICA_PERU
int DEVINFO_UpdateDeviceLog(void);
#endif

int DEVINFO_ModuleInit();
int DEVINFO_ModuleDestroy();

/******************************************************************************
*                                 END                                        *
******************************************************************************/
#endif

