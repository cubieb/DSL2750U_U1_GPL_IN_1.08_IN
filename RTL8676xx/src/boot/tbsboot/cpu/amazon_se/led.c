#include <common.h>
#include "amazon_se_reg.h"

extern struct led_dev amazon_se_leds[];


void set_amazon_se_led(struct led_dev *led , char state)
{

	if(led->gpio < 0 || led->gpio > 31)
		return;

	if(led->gpio < 16)  /* port0 处理*/
	{
		*AMAZON_SE_GPIO_P0_OD = (*AMAZON_SE_GPIO_P0_OD | (1 << led->gpio)) & 0xffff;
	
		*AMAZON_SE_GPIO_P0_ALTSEL0 = (*AMAZON_SE_GPIO_P0_ALTSEL0 & ~(1 << led->gpio)) & 0xffff;
		
		*AMAZON_SE_GPIO_P0_ALTSEL1 = (*AMAZON_SE_GPIO_P0_ALTSEL1 & ~(1 << led->gpio)) & 0xffff;

		*AMAZON_SE_GPIO_P0_DIR = (*AMAZON_SE_GPIO_P0_DIR | (1 << led->gpio)) & 0xffff;

		if( state == LED_BOOT_ON)
		{
			if(led->level == LED_GPIO_HIGH)
				*AMAZON_SE_GPIO_P0_OUT = (*AMAZON_SE_GPIO_P0_OUT | (1 << led->gpio)) & 0xffff;
			else
				*AMAZON_SE_GPIO_P0_OUT = (*AMAZON_SE_GPIO_P0_OUT & ~(1 << led->gpio)) & 0xffff;
		}
		else
		{
			if(led->level == LED_GPIO_HIGH)
				*AMAZON_SE_GPIO_P0_OUT = (*AMAZON_SE_GPIO_P0_OUT & ~(1 << led->gpio)) & 0xffff;
			else
				*AMAZON_SE_GPIO_P0_OUT = (*AMAZON_SE_GPIO_P0_OUT | (1 << led->gpio)) & 0xffff;
		}
	}
	else   /* port1 处理*/
	{
		*AMAZON_SE_GPIO_P1_OD = (*AMAZON_SE_GPIO_P1_OD | (1 << (led->gpio-16))) & 0xffff;
	
		*AMAZON_SE_GPIO_P1_ALTSEL0 = (*AMAZON_SE_GPIO_P1_ALTSEL0 & ~(1 << (led->gpio-16))) & 0xffff;
		
		*AMAZON_SE_GPIO_P1_ALTSEL1 = (*AMAZON_SE_GPIO_P1_ALTSEL1 & ~(1 << (led->gpio-16))) & 0xffff;
		
		*AMAZON_SE_GPIO_P1_DIR = (*AMAZON_SE_GPIO_P1_DIR | (1 << (led->gpio-16))) & 0xffff;

		if( state == LED_BOOT_ON)
		{
			if(led->level == LED_GPIO_HIGH)
				*AMAZON_SE_GPIO_P1_OUT = (*AMAZON_SE_GPIO_P1_OUT | (1 << (led->gpio-16))) & 0xffff;
			else
				*AMAZON_SE_GPIO_P1_OUT = (*AMAZON_SE_GPIO_P1_OUT & ~(1 << (led->gpio-16))) & 0xffff;
		}
		else
		{
			if(led->level == LED_GPIO_HIGH)
				*AMAZON_SE_GPIO_P1_OUT = (*AMAZON_SE_GPIO_P1_OUT & ~(1 << (led->gpio-16))) & 0xffff;
			else
				*AMAZON_SE_GPIO_P1_OUT = (*AMAZON_SE_GPIO_P1_OUT | (1 << (led->gpio-16))) & 0xffff;
		}
	}
	
}

void _SetLed( led_name led , char state )
{
	int i;

	
	for( i = 0 ;  amazon_se_leds[i].name != led_end ; i++ )
	{
		if( amazon_se_leds[i].name == led )
		{
			set_amazon_se_led(&amazon_se_leds[i], state);	
			break;
		}
	}
}




int _init_led( void )
{
	int i;
	for( i = 0 ;  amazon_se_leds[i].name != led_end ; i++ )
	{
		_SetLed( amazon_se_leds[i].name , amazon_se_leds[i].boot_default );
	}
	
	return 0;
}

