#include <common.h>

/*port A pin*/
#define GPIO_PA7	0x00000080
#define GPIO_PA6	0x00000040
#define GPIO_PA5	0x00000020
#define GPIO_PA4	0x00000010
#define GPIO_PA3	0x00000008
#define GPIO_PA2	0x00000004
#define GPIO_PA1	0x00000002
#define GPIO_PA0	0x00000001
/*port B pin*/
#define GPIO_PB7	0x00008000
#define GPIO_PB6	0x00004000
#define GPIO_PB5	0x00002000
#define GPIO_PB4	0x00001000
#define GPIO_PB3	0x00000800
#define GPIO_PB2	0x00000400
#define GPIO_PB1	0x00000200
#define GPIO_PB0	0x00000100
/*port C pin*/
#define GPIO_PC7	0x00800000
#define GPIO_PC6	0x00400000
#define GPIO_PC5	0x00200000
#define GPIO_PC4	0x00100000
#define GPIO_PC3	0x00080000
#define GPIO_PC2	0x00040000
#define GPIO_PC1	0x00020000
#define GPIO_PC0	0x00010000
/*port D pin*/
#define GPIO_PD7	0x80000000
#define GPIO_PD6	0x40000000
#define GPIO_PD5	0x20000000
#define GPIO_PD4	0x10000000
#define GPIO_PD3	0x08000000
#define GPIO_PD2	0x04000000
#define GPIO_PD1	0x02000000
#define GPIO_PD0	0x01000000

//cathy
#define IS_6028A		(((*( unsigned int * )(0xB8003200) & 0x00f10000) == 0x00f10000) ? 1:0)	//b21=1,b20=1, b16=1
#define IS_6028B		(((*( unsigned int * )(0xB8003200) & 0x00f10000) == 0x00f00000) ? 1:0)	//b21=1,b20=1, b16=0
#define IS_6028C		(((*( unsigned int * )(0xB8003200) & 0x00110000) == 0x00110000) ? 1:0)	//b21=1,b20=1, b16=1
#define IS_6085		(((*( unsigned int * )(0xB8003200) & 0x00110000) == 0x00010000) ? 1:0)	//b21=1,b20=0, b16=1
#define IS_RLE0315 (((*( unsigned int * )(0xB8003200) & 0x00f10000) == 0x00d10000) ? 1:0) //b21=0, b20=1, b16=1
#define IS_6166 (((*( unsigned int * ) & 0x00f10000) == 0x00c10000) ? 1:0) //b21=0, b20=0, b16=1


/*
 * System MISC Control Register
 */
#define MISCCR_BASE	0xB8003300
#define MISC_PINMUX	(MISCCR_BASE+0x00)
#define MISC_PINOCR	(MISCCR_BASE+0x04)
#define MISC_PINSR	(MISCCR_BASE+0x08)


extern struct led_dev rtl8672_leds[];

#if 0
extern struct btn_dev rtl8672_btns[];

int GetKeyState( void )
{
	int KeyValue = *( unsigned int * )GPIO_PABCD_DAT;
	printf("KeyValue=%x\n",KeyValue);
	return KeyValue;
}
#define CFG_BTN_GPIO_RESET		6
#define CFG_BTN_GPIO_WPS		15
#define CFG_BTN_GPIO_WLAN		7

int KeyDown( btn_name key )
{
	int KeyValue;

	int i;
	
	for( i = 0 ;  rtl8672_btns[i].name != led_end ; i++ )
	{
		if( rtl8672_btns[i].name == key )
		{
			break;
		}
	}
	
	*( unsigned int * )GPIO_PABCD_CNR &= ~( ( 1<< CFG_BTN_GPIO_RESET ) |( 1<< CFG_BTN_GPIO_WPS ) |( 1 << CFG_BTN_GPIO_WLAN ) );//~( 1 << rtl8672_btns[i].gpio );
	*( unsigned int * )GPIO_PABCD_PTYPE &= ~( ( 1<< CFG_BTN_GPIO_RESET ) |( 1<< CFG_BTN_GPIO_WPS ) |( 1 << CFG_BTN_GPIO_WLAN ) );//~( 1 << rtl8672_btns[i].gpio );
	*( unsigned int * )GPIO_PABCD_DIR &= ~( ( 1<< CFG_BTN_GPIO_RESET ) |( 1<< CFG_BTN_GPIO_WPS ) |( 1 << CFG_BTN_GPIO_WLAN ) );//~( 1 << rtl8672_btns[i].gpio );

	KeyValue = GetKeyState();
	printf("KeyValue0000=%x\n",KeyValue);
	if( ( KeyValue & ( 1 << rtl8672_btns[i].gpio ) ) == 0 )
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
	return KeyDown( btn_reset);
}

#endif

void _SetLed( led_name led , char state )
{
	int i;       
    
	for( i = 0 ;  rtl8672_leds[i].name != led_end ; i++ )
	{
		if( rtl8672_leds[i].name == led )
		{
			break;
		}
	}

	*( unsigned int * )GPIO_PABCD_CNR &= ~( 1 << rtl8672_leds[i].gpio );
	*( unsigned int * )GPIO_PABCD_PTYPE &= ~( 1 << rtl8672_leds[i].gpio );
	*( unsigned int * )GPIO_PABCD_DIR |= ( 1 << rtl8672_leds[i].gpio );

/*
	switch( state )
	{
		case LED_BOOT_ON:
		{
			if( rtl8672_leds[i].level == LED_GPIO_HIGH )
			{
				*( unsigned int * )GPIO_PABCD_DAT |= ( 1 << rtl8672_leds[i].gpio ); 
				udelay( 1000 );				
				break;
			}
			else
			{
				*( unsigned int * )GPIO_PABCD_DAT &= ~( 1 << rtl8672_leds[i].gpio ); 				
				break;
			}
		}
		case LED_BOOT_OFF:
		{
			if( rtl8672_leds[i].level == LED_GPIO_HIGH )
			{
				*( unsigned int * )GPIO_PABCD_DAT &= ~( 1 << rtl8672_leds[i].gpio ); 				
				break;
			}
			else
			{
				*( unsigned int * )GPIO_PABCD_DAT |= ( 1 << rtl8672_leds[i].gpio ); 				
				break;
			}
		}
	}*/

}

int _init_led( void )
{
	int i;
	
	/* Enable GPIO D2, D3, D4, D6 */
  if( IS_6028B || IS_6085 ) 
  {
      *( unsigned int * )(MISC_PINOCR) |= 0x000c0000; 
  }
  else if( IS_RLE0315 )
  {
     *( unsigned int * )(MISC_PINOCR) = 0x003c0000; //enable GPIO D1, D2 ,D3,D4,D5
     *( unsigned int * )(MISC_PINOCR) &= (~(1<<22));   //enable GPIO D0
 	}
 			 
	for( i = 0 ;  rtl8672_leds[i].name != led_end ; i++ )
	{
		_SetLed( rtl8672_leds[i].name , rtl8672_leds[i].boot_default );
	}
		
	*( unsigned int * )GPIO_PABCD_DAT |= 0xff000000;
	*( unsigned int * )GPIO_PABCD_DAT &= (~(1<<29));

	return 0;
}

