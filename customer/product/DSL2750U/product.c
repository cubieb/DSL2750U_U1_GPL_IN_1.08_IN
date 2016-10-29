/*
 * 说明:本文件是内核与bootloader共用的部分硬件相关数据结构
 *      包含LED,BUTTON,FLASH
 * 作者:夏超仁
 */

#ifndef __LINUX_RTL8676_PRODUCT_H_INCLUDED__
#define __LINUX_RTL8676_PRODUCT_H_INCLUDED__
#include <autoconf.h>
#include <led.h>
#include <btn.h>
#include <spi_flash.h>
#include "rtl8676_gpio.h"

const struct spi_flash_params flash_table[]= {
	{"AT26DF161"   ,0xEF4600, 2 , 64 , 32 , 256},
	{"AT26DF161A"  ,0xEF4601, 2 , 64 , 32 , 256},
	{"AT26DF161A"  ,0xEF4602, 2 , 64 , 32 , 256},
	{"A25L016"     ,0x373015, 2 , 64 , 32 , 256},
	{"EN25P16"     ,0x1C3015, 2 , 64 , 32 , 256},//ok
	{"EN25F16"     ,0x1C3115, 2 , 64 , 32 , 256},//ok
	//{"F25L016A"    ,0x8C2015, 2 , 64 , 32 , 256},//ESMT
	//{"F25L016A"    ,0x8C2115, 2 , 64 , 32 , 256},
	{"MX25L1605D"  ,0xc22015, 2 , 64 , 32 , 256},//ok
	{"MX25L1635D"  ,0xc22415, 2 , 64 , 32 , 256},//ok
	{"M25P16"      ,0x202015, 2 , 64 , 32 , 256},//numonyx
	{"W25X16"      ,0xEF3015, 2 , 64 , 32 , 256},//ok
	{"W25Q16"      ,0xEF4015, 2 , 64 , 32 , 256},//ok
	//{"QH25FL016"   ,0x898911, 2 , 64 , 32 , 256},//intel
	{"S25FL016A"   ,0x010214, 2 , 64 , 32 , 256},//ok
	//{"SST25VF016"  ,0xBF2541, 2 , 64 , 32 , 256},
	{"GD25Q16"     ,0xC84015, 2 , 64 , 32 , 256},
	{"AT25DF321"   ,0x1F4700, 4 , 64 , 64 , 256},
	{"AT25DF321"   ,0x1F4701, 4 , 64 , 64 , 256},
	{"A25L032"     ,0x373016, 4 , 64 , 64 , 256},
	{"EN25P32"     ,0x1C2016, 4 , 64 , 64 , 256},//ok
	{"EN25F32"     ,0x1C3116, 4 , 64 , 64 , 256},//ok
	{"EN25Q32"     ,0x1C3316, 4 , 64 , 64 , 256},
	{"EN25Q32A"    ,0x1C3016, 4 , 64 , 64 , 256},//ok
	{"M25P32"      ,0x202016, 4 , 64 , 64 , 256},
	{"MX25L3205D"  ,0xc22016, 4 , 64 , 64 , 256},//ok
	{"MX25L3235D"  ,0xc22416, 4 , 64 , 64 , 256},//ok
	{"N25S32"      ,0xD53016, 4 , 64 , 64 , 256},//?
	{"W25X32"      ,0xEF3016, 4 , 64 , 64 , 256},//ok
	{"W25Q32"      ,0xEF4016, 4 , 64 , 64 , 256},//ok
	//{"QH25FL032"   ,0x898912, 4 , 64 , 64 , 256},
	{"S25FL032A"   ,0x010215, 4 , 64 , 64 , 256},//ok
	//{"SST25VF032"  ,0xBF254A, 4 , 64 , 64 , 256},
	{"GD25Q32"     ,0xC84016, 4 , 64 , 64 , 256},
	{"AT25DF641"   ,0x1F4800, 8 , 64 , 128, 256},
	{"A25L064"     ,0x373017, 8 , 64 , 128, 256},
	{"EN25P64"     ,0x1C2017, 8 , 64 , 128, 256},
	{"EN25Q64"     ,0x1C3017, 8 , 64 , 128, 256},//ok
	{"M25P64"      ,0x202017, 8 , 64 , 128, 256},//ok
	{"MX25L6405D"  ,0xc22017, 8 , 64 , 128, 256},//ok
	{"W25X64"      ,0xEF3017, 8 , 64 , 128, 256},
	{"W25Q64"      ,0xEF4017, 8 , 64 , 128, 256},//FL064K
	//{"QH25FL064"   ,0x898913, 8 , 64 , 128, 256},
	{"S25FL064A"   ,0x010216, 8 , 64 , 128, 256},//ok
	{"GD25Q64"     ,0xC84017, 8 , 64 , 128, 256},
	{"GD25Q128"    ,0xC84018, 16, 64 , 256, 256},
	{"AT25DF128"   ,0x1F4900, 16, 64 , 256, 256},
	{"A25L128"     ,0x373018, 16, 64 , 256, 256},
	{"EN25P128"    ,0x1C2018, 16, 64 , 256, 256},
	{"EN25Q128"    ,0x1C3018, 16, 64 , 256, 256},
	//{"M25P128"     ,0x202018, 16, 256, 64 , 256},
	{"N25Q128"     ,0x20BA18, 16, 64 , 256, 256},//ok
	{"N25Q128"     ,0x20BB18, 16, 64 , 256, 256},//ok
	{"MX25L12805"  ,0xc22018, 16, 64 , 256, 256},//MX25L12845
	// 32M当做16M使用清库存
	{"MX25L25635"  ,0xc22019, 16, 64 , 256, 256},//MX25L25635
	// 32M,当前E8B只用16M，后续需要时再修改成32M
	{"S25FL256S"   ,0X010219, 16, 64 , 256, 256},//S25FL256SAGMFI001
	{"W25X128"     ,0xEF3018, 16, 64 , 256, 256},
	{"W25Q128"     ,0xEF4018, 16, 64 , 256, 256},//ok
	//{"S25FL128P"   ,0x012018, 16, 256, 64 , 256},
	{"S25FL128P"   ,0x012018, 16, 64 , 256, 256},//S25L129P
	{"Unknown"     ,0x0     , 0 , 0  , 0  , 0  }
};

/* 定义LED参数 */
const struct led_dev rtl8676_leds[] = {
#ifdef CONFIG_RTL8271BVN
	{ led_power_green, GPIO_I_E, LED_GPIO_LOW, led_on_trig, LED_BOOT_ON, 0, 0 },
#endif	
	{ led_power_red, GPIO_G_0, LED_GPIO_LOW, led_off_trig, LED_BOOT_OFF, 0, 0 },
	{ led_internet_green, GPIO_H_1, LED_GPIO_LOW, led_off_trig, LED_BOOT_OFF, 0, 0 },
	{ led_internet_red, GPIO_G_1, LED_GPIO_LOW, led_off_trig, LED_BOOT_OFF, 0, 0 },
	{ led_usb, GPIO_F_6, LED_GPIO_LOW, led_off_trig, LED_BOOT_OFF, 0, 0 },
#ifdef CONFIG_RTL8271BVN	
	{ led_usb_red, GPIO_I_D, LED_GPIO_LOW, led_off_trig, LED_BOOT_OFF, 0, 0},
#endif	
	{ led_usb_green, GPIO_F_6, LED_GPIO_LOW, led_off_trig, LED_BOOT_OFF, 0, 0 },
#ifdef CONFIG_RTL8192CD	
	{ led_wlan, RTL8192CD_GPIO_8, led_off_trig, LED_BOOT_OFF, 0, 0 },
	{ led_wps, RTL8192CD_GPIO_3, led_off_trig, LED_BOOT_OFF, 0, 0 },
	{ led_wps_green, RTL8192CD_GPIO_3, led_off_trig, LED_BOOT_OFF, 0, 0 },
	{ led_wps_red, RTL8192CD_GPIO_0, led_off_trig, LED_BOOT_OFF, 0, 0 },
	{ led_wps_yellow, RTL8192CD_GPIO_1, led_off_trig, LED_BOOT_OFF, 0, 0 },
#endif	
	{ led_dsl, GPIO_H_3, LED_GPIO_LOW, led_off_trig, LED_BOOT_OFF, 0, 0 },
#ifdef CONFIG_RTL8271BVN
	{ led_lan_green, GPIO_I_F, LED_GPIO_LOW, led_off_trig, LED_BOOT_OFF, 0, 0 },
#endif	
	{ led_3gmode_green, GPIO_F_6, LED_GPIO_LOW, led_off_trig, LED_BOOT_OFF, 0, 0 },
#ifdef CONFIG_RTL8271BVN
	{ led_3gmode_red, GPIO_I_D, LED_GPIO_LOW, led_off_trig, LED_BOOT_OFF, 0, 0},
#endif	
	{ led_end, 0, LED_GPIO_HIGH, led_off_trig, LED_BOOT_OFF, 0, 0 },
};

/* 定义按钮参数 */
const struct btn_dev rtl8676_btns[] = {
	{ btn_reset, GPIO_G_7, LED_GPIO_LOW, BTN_UP, 0, 0 },
#ifdef CONFIG_RTL8192CD		
	{ btn_wps, RTL8192CD_GPIO_2, LED_GPIO_LOW, BTN_UP, 0, 0 },		/*由无线芯片驱动*/
#endif	
	{ btn_wlan, GPIO_H_0, LED_GPIO_LOW, BTN_UP, 0, 0 },
	{ btn_end, 0, LED_GPIO_LOW, BTN_UP, 0, 0 },
};

#ifdef _LINUX_KERNEL_H
#include <linux/module.h>

EXPORT_SYMBOL_GPL(rtl8676_leds);
#endif

#endif /* __LINUX_RTL8676_PRODUCT_H_INCLUDED__ */
