#include <common.h>
#include "bsp_1000.h"

extern struct led_dev c1k_leds[];

void _SetLed(led_name led, char state)
{
	int i;
	for(i=0; c1k_leds[i].name != led_end; i++)
	{
		if(c1k_leds[i].name == led )
		{
			break;
		}
	}
	
	SoC_gpio_enable_output(SoC_gpio_mask(c1k_leds[i].gpio));
	
	switch(state)
	{
		case LED_BOOT_ON:
		{
			if(c1k_leds[i].level == LED_GPIO_HIGH)
			{
				SoC_gpio_set_1(SoC_gpio_mask(c1k_leds[i].gpio));
				break;
			}
			else
			{
				SoC_gpio_set_0(SoC_gpio_mask(c1k_leds[i].gpio));
				break;
			}
		}
		case LED_BOOT_OFF:
		{
			if(c1k_leds[i].level == LED_GPIO_HIGH)
			{
				SoC_gpio_set_0(SoC_gpio_mask(c1k_leds[i].gpio));
				break;
			}
			else
			{
				SoC_gpio_set_1(SoC_gpio_mask(c1k_leds[i].gpio));
				break;
			}
		}
	}
}

int _init_led(void)
{
	int i;
	for(i=0; c1k_leds[i].name != led_end; i++ )
	{
		_SetLed(c1k_leds[i].name, c1k_leds[i].boot_default );
	}
	
	return 0;
}

