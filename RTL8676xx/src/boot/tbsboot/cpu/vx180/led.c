#include <common.h>

extern void gpioSetDirIn(unsigned short pin);
extern void gpioSetDirOut(unsigned short pin);
extern void gpioSetFlag(unsigned short pin);
extern void gpioClearFlag(unsigned short pin);
extern void gpioSetIntrEN(unsigned short pin);
extern void gpioClearIntrEN(unsigned short pin);

extern struct led_dev vx180_leds[];

void _SetLed( led_name led , char state )
{
	int i;
	for( i = 0 ;  vx180_leds[i].name != led_end ; i++ )
	{
		if( vx180_leds[i].name == led )
		{
			break;
		}
	}
	
	gpioClearIntrEN(vx180_leds[i].gpio);
	gpioSetDirOut(vx180_leds[i].gpio);
	
	switch( state )
	{
		case LED_BOOT_ON:
		{
			if( vx180_leds[i].level == LED_GPIO_HIGH )
			{
				gpioSetFlag(vx180_leds[i].gpio);
				udelay( 10 );
				break;
			}
			else
			{
				gpioClearFlag(vx180_leds[i].gpio);
				udelay( 10 );
				break;
			}
		}
		case LED_BOOT_OFF:
		{
			if( vx180_leds[i].level == LED_GPIO_HIGH )
			{
				gpioClearFlag(vx180_leds[i].gpio);
				udelay( 10 );
				break;
			}
			else
			{
				gpioSetFlag(vx180_leds[i].gpio);
				udelay( 10 );
				break;
			}
		}
	}

}

int _init_led( void )
{
	int i;
	for( i = 0 ;  vx180_leds[i].name != led_end ; i++ )
	{
		_SetLed( vx180_leds[i].name , vx180_leds[i].boot_default );
	}
	
	return 0;
}

