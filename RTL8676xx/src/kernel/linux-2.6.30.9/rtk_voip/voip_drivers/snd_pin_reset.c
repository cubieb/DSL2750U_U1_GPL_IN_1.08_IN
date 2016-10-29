#include <linux/delay.h>
#include "rtk_voip.h"
#include "voip_init.h"
#include "gpio/gpio.h"

#include "snd_pin_reset.h"

static void _do_pin_reset_action( uint32 pin_reset )
{
	printk( "PIN_RESET=%X... ", pin_reset );
	
	_rtl_generic_initGpioPin(pin_reset, GPIO_CONT_GPIO, GPIO_DIR_OUT, GPIO_INT_DISABLE);
	
	//_rtl_generic_setGpioDataBit(pin_reset, 1);	// reset high
	
	//mdelay( 250 );
	
	_rtl_generic_setGpioDataBit(pin_reset, 0);	// set reset low, PCLK and FS id present and stable.
	
	mdelay( 250 );
	
	_rtl_generic_setGpioDataBit(pin_reset, 1);	// release reset
	
	mdelay( 250 );
	
	printk( "done\n" );
}

void do_pin_reset_action( void )
{
	// 250ms is reference to si3217x and si3226

#ifdef PIN_RESET1
	if( PIN_RESET1 != GPIO_ID(GPIO_PORT_UNDEF,0) ) {
		_do_pin_reset_action( PIN_RESET1 );
	}
#endif
	
#ifdef PIN_RESET2 
	if( ( PIN_RESET1 != PIN_RESET2 ) && 
		( PIN_RESET2 != GPIO_ID(GPIO_PORT_UNDEF,0) ) ) 
	{
		_do_pin_reset_action( PIN_RESET2 );
	}
#endif
}

//#ifdef CONFIG_RTK_VOIP_DRIVERS_SLIC_ZARLINK
//void RST_Slic(void)
//{
//	do_pin_reset_action();
//}
//#endif

static int __init voip_snd_reset( void )
{
	extern void inactive_snd_pin_cs( void );

	inactive_snd_pin_cs();
	
	do_pin_reset_action();
	
	return 0;
}

voip_initcall_snd_setup( voip_snd_reset );

