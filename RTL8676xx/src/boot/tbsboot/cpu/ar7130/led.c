#include <common.h>

extern struct led_dev ar7130_leds[];
#if 0
int GetKeyState( void )
{
	int KeyValue = *( unsigned int * )GPIO_IN;
	return KeyValue;
}

int KeyDown( int key )
{
	int KeyValue;
	
	KeyValue = GetKeyState();

	if( ( KeyValue & key ) == 0 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int KeyPauseBoot( void )
{
	return KeyDown( RESET_KEY);
}

void AllLedOn( void )
{
	SetLed( USB_LED | JUMPSTART_LED | WPS_LED, LED_ON );
}

void AllLedOff( void )
{
	SetLed( USB_LED | JUMPSTART_LED | WPS_LED , LED_OFF );
}

void SetLed( int led , char state )
{
	*( unsigned int * )GPIO_OE |= led;
	udelay( 10 );
	switch( state )
	{
		case LED_ON:
		{
			*( unsigned int * )GPIO_SET |= led; 
			udelay( 10 );
			break;
		}
		case LED_OFF:
		{
			*( unsigned int * )GPIO_CLEAR = led; 
			udelay( 10 );
			break;
		}
	}
}

#endif

void _SetLed( led_name led , char state )
{
	int i;
	for( i = 0 ;  ar7130_leds[i].name != led_end ; i++ )
	{
		if( ar7130_leds[i].name == led )
		{
			break;
		}
	}
	
	*( unsigned int * )GPIO_OE |= ( 1 << ar7130_leds[i].gpio );
	udelay( 10 );
	
	switch( state )
	{
		case LED_BOOT_ON:
		{
			if( ar7130_leds[i].level == LED_GPIO_HIGH )
			{
				*( unsigned int * )GPIO_SET |= ( 1 << ar7130_leds[i].gpio ); 
				udelay( 10 );
				break;
			}
			else
			{
				*( unsigned int * )GPIO_CLEAR = ( 1 << ar7130_leds[i].gpio ); 
				udelay( 10 );
				break;
			}
		}
		case LED_BOOT_OFF:
		{
			if( ar7130_leds[i].level == LED_GPIO_HIGH )
			{
				*( unsigned int * )GPIO_CLEAR = ( 1 << ar7130_leds[i].gpio ); 
				udelay( 10 );
				break;
			}
			else
			{
				*( unsigned int * )GPIO_SET |= ( 1 << ar7130_leds[i].gpio ); 
				udelay( 10 );
				break;
			}
		}
	}
}

int _init_led( void )
{
	int i;
	for( i = 0 ;  ar7130_leds[i].name != led_end ; i++ )
	{
		_SetLed( ar7130_leds[i].name , ar7130_leds[i].boot_default );
	}
	
	return 0;
}

