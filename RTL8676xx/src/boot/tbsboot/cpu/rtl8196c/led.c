#include <common.h>
#include <types.h>
#include "rtl8196c_gpio.h"
#include "led.h"

extern struct led_dev rtl8196c_leds[];

void SetLed( led_name led , char state )
{
	volatile u32 val;
	int i;


	for(i = 0; rtl8196c_leds[i].name != led_end; i++ )
	{
		if( rtl8196c_leds[i].name == led )
		{
			break;
		}
	}
	
	/* Set LED Pins as GPIO */
	val = REG_READ(GPIO_PABCD_CNR);
	val &= ~( 1 << rtl8196c_leds[i].gpio );
	REG_WRITE(GPIO_PABCD_CNR, val);
	
	/* Configure Shared Pins as GPIO Mode */
	val = REG_READ(PIN_MUX_SEL);
	val |= REG_SHARED_PIN_CFG;
	REG_WRITE(PIN_MUX_SEL, val);

	/* Set LED Pins as GPIO output */
	val = REG_READ(GPIO_PABCD_DIR);
	val |= (1 << rtl8196c_leds[i].gpio);
	REG_WRITE(GPIO_PABCD_DIR, val);

	if (state == LED_BOOT_ON)
		{
		if( rtl8196c_leds[i].level == LED_GPIO_HIGH )
			*( volatile u32 *)GPIO_PABCD_DAT |= ( 1 << rtl8196c_leds[i].gpio );
		else
			*( volatile u32 *)GPIO_PABCD_DAT &= ~( 1 << rtl8196c_leds[i].gpio );
		}
	else
		{
		if( rtl8196c_leds[i].level == LED_GPIO_HIGH )
			*( volatile u32 *)GPIO_PABCD_DAT &= ~( 1 << rtl8196c_leds[i].gpio );
		else
			*( volatile u32 *)GPIO_PABCD_DAT |= ( 1 << rtl8196c_leds[i].gpio );
		}
	
}

int init_led(void)
{
	int i;


	for( i = 0 ;  rtl8196c_leds[i].name != led_end ; i++ )
	{
		SetLed( rtl8196c_leds[i].name , rtl8196c_leds[i].boot_default );
	}

	return 0;
}

void set_all_led(char state)
{
	int i;
	
	for(i=0; rtl8196c_leds[i].name != led_end; i++)
		{
		SetLed(rtl8196c_leds[i].name, state);
		}
}
