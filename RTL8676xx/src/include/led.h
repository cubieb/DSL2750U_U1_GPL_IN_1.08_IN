/*
 * 文件名:led.h
 * 说明:TBS LED驱动头文件
 * 
 * 作者:Zhang Yu
 *
 */
 
#ifndef __LINUX_LEDS_CORE_H_INCLUDED
#define __LINUX_LEDS_CORE_H_INCLUDED

#ifdef __KERNEL__
#include <linux/kernel.h>
#endif

#ifdef CONFIG_LED_DEBUG
#define leddebug(fmt, arg...)		printk(fmt, arg...)
#else
#define leddebug(fmt, arg...)        do{}while(0)
#endif

typedef enum  {
	led_power                = 0,
	led_power_green          = 1,
	led_power_red            = 2,
	led_internet             = 3,
	led_internet_green       = 4,
	led_internet_red         = 5,
	led_usb                  = 6,
    led_usb_red              = 7,
    led_usb_green            = 8,
	led_wlan                 = 9,
	led_wps                  = 10,
	led_wps_green            = 11,
	led_wps_red              = 12,		
	led_wps_yellow           = 13,
	led_dsl                  = 14,
	led_register             = 15,
	led_phone_1              = 16,
	led_phone_2              = 17,		
	led_line                 = 18,
	led_prov_alm             = 19,
	led_wps_wifi             = 20,
	led_lan_green            = 21,
	led_lan_red              = 22,
	led_3gmode_green         = 23, 
	led_3gmode_red           = 24, 
	led_acc                  = 25,
	led_update_key           = 26,
	led_end,		/* 结束 */
}led_name;

typedef enum  {
	led_off_trig	         = 0,
	led_on_trig              = 1,
	led_flash_trig           = 2,
	led_flash_2hz_trig       = 3,
	led_flash_4hz_trig       = 4,
	led_dataflash_trig       = 5,
	led_wps_inprogress_trig  = 6,
	led_wps_error_trig       = 7,
	led_wps_overlap_trig     = 8,
	led_wps_success_trig     = 9,
	led_wlan_beacon_trig     = 10,
	led_blinking_trig        = 11,
	led_end_trig,
}trig_name;

typedef enum  {
	LED_GPIO_LOW  = 0,
	LED_GPIO_HIGH,
}led_level;


typedef enum  {
	LED_BOOT_OFF  = 0,
	LED_BOOT_ON,
}led_boot_default;

struct led_dev {
	led_name name;
	int gpio;				/* GPIO号 */
	led_level level;				/* 高低电平触发 */
	trig_name kernel_default;		/* kernel默认状态 */
	led_boot_default boot_default;		/* Bootloader默认状态 */
	struct tbs_led_trigger	*cur_trig;			/* 当前trigger */
	void *trigger_data;
};

struct tbs_led_trigger {
	trig_name name;
	void (*activate)(struct led_dev *led_cdev);
	void (*deactivate)(struct led_dev *led_cdev);
	unsigned int *flash_seq;
};


struct led_hw_handler {
	void (*led_on)(struct led_dev *led_cdev);
	void (*led_off)(struct led_dev *led_cdev);
};

#ifdef _LINUX_KERNEL_H
#include <linux/timer.h>
#include <linux/interrupt.h>
struct tbs_led_trigger_data {
	struct led_dev *dev;
	unsigned int phase;
	struct timer_list timer;
	int pending;
};

struct tbs_ledcore {
	struct tbs_led_trigger_data bdata[led_end];
	struct tbs_led_trigger_data flash_led;
	struct led_hw_handler *handler;
	void (*led_blinking)(led_name led);
	int (*trigger_set)(led_name led, trig_name trigger);
	void (*flash_blinking)(void);
};

extern struct tbs_ledcore *ledcore;   /* LED点灯句柄，全局使用 */
#endif

int led_dev_register(struct led_dev *led);
int led_dev_unregister(struct led_dev *led);
int led_hw_handle_register(struct led_hw_handler *handler);
int led_hw_handle_unregister(struct led_hw_handler *handler);
void tbs_led_data_blinking(led_name led);
void tbs_led_system_fault_blinking(led_name led);
void tbs_led_trigger_set(led_name led, trig_name trigger);
void led_wps_set(trig_name trigger);
void tbs_led_flash_erase_write(void);
void set_all_led(char state);



/* ioctl led 参数定义 */

struct tbs_ioctl_led_parms
{
	led_name led;
	trig_name trig;
};

#endif		/* __LINUX_LEDS_CORE_H_INCLUDED */
