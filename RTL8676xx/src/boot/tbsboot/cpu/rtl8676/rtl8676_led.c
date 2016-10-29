#include <common.h>
#include <types.h>
#include "rtl8676_asicregs.h"
#include "rtl8676_gpio.h"
#include "led.h"

extern struct led_dev rtl8676_leds[];
static unsigned int system_led_state;

void SetLed(led_name led , char state)
{
	volatile u32 val;
	int i;
	struct led_dev *p;
	struct rtl8676_gpio_para para;

	for(i =0; rtl8676_leds[i].name != led_end; i++)		
		{
		if(rtl8676_leds[i].name == led)
			{
			break;
			}
		}
	p = &(rtl8676_leds[i]);
	if(get_rtl8676_gpio_para(p->gpio, &para) < 0)
		{
		return;
		}
//	leddebug("\nGPIOnumber=%d, CNR=0x%08x, PTYPE=0x%08x, DIR=0x%08x, DATA=0x%08x Before\n", p->gpio, REG32(para.ctrl_addr), 
//		REG32(para.ptype_addr), REG32(para.dir_addr), REG32(para.data_addr));
	/* Set LED Pins as GPIO output */
	if(2 == para.group)
		{
		return;
		}
	rtl8676_set_gpio_dir(&para, GPIO_FUNC_OUTPUT);
	val = REG32(para.data_addr);
	if (state == LED_BOOT_ON)
		{
		if(rtl8676_leds[i].level == LED_GPIO_HIGH)
			val |= para.pins;
		else
			val &= ~(para.pins);
		}
	else
		{
		if(rtl8676_leds[i].level == LED_GPIO_HIGH)
			val &= ~(para.pins);
		else
			val |=  para.pins;
		}
	REGWRITE32(para.data_addr, val);
//	leddebug("\nGPIOnumber=%d, CNR=0x%08x, PTYPE=0x%08x, DIR=0x%08x, DATA=0x%08x After\n", p->gpio, REG32(para.ctrl_addr), 
//		REG32(para.ptype_addr), REG32(para.dir_addr), REG32(para.data_addr));	
}

void set_all_led(char state)
{
	int i;
	
	for(i=0; rtl8676_leds[i].name != led_end; i++)
		{
		if(state < 0)
			{
			SetLed(rtl8676_leds[i].name, rtl8676_leds[i].boot_default);
			}
		else
			{
			SetLed(rtl8676_leds[i].name, state);
			}
		
		}
}

int init_led(void)
{
	u32 val;
		
	val = REG32(BSP_MISC_PINMUX);/* Disable JTAG fuction */
	if((val & BSP_JTAG_GPIO_PINMUX) == BSP_JTAG_GPIO_PINMUX)
		{
		leddebug("JTAG function Disabled!\n");
		val &= ~BSP_JTAG_GPIO_PINMUX;
		REGWRITE32(BSP_MISC_PINMUX, val);
		}
	#ifdef CONFIG_RTL8271BVN
	if(0 == (val & BSP_AFED_GPIO))
		{
		leddebug("AFE RXD set as GPIO!\n");
		val |= BSP_AFED_GPIO;
		REGWRITE32(BSP_MISC_PINMUX, val);
		}
	#endif
	
	val = REG32(BSP_RGMII_PAD_CTRL);/* Set USBLED1 to GPIO mode */
	if((val & BSP_USBLED_GPIO_SEL) == BSP_USBLED_GPIO_SEL)
		{
		leddebug("Set USB led as GPIO!\n");
		val &= ~BSP_USBLED_GPIO_SEL;
		REGWRITE32(BSP_RGMII_PAD_CTRL, val);
		}
	set_all_led(-1);
	SetLed(led_power_red, LED_BOOT_ON);
	system_led_state = 0;
	
	return 0;
}

void tbs_led_system_fault_blinking(led_name led)
{
	if(!system_led_state)
		{
		SetLed(led, LED_BOOT_ON);
		system_led_state = 1;
		}
	else
		{
		SetLed(led, LED_BOOT_OFF);
		system_led_state = 0;
		}
}
