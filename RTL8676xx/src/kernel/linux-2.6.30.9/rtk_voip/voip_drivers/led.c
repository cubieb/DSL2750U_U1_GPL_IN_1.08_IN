#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <rtk_voip.h>
#include "led.h"

/* =================== For 8972B V100 EV Board LED Control ======================== */
/* =================== For 8954C V100 EV Board LED Control ======================== */
/* =================== For 8954C V200 EV Board LED Control ======================== */
#if defined( CONFIG_RTK_VOIP_GPIO_8972B ) || defined( CONFIG_RTK_VOIP_DRIVERS_PCM89xxC ) 

#include "gpio/gpio.h"
#include "voip_init.h"

static struct timer_list leds_timer;

static unsigned int leds_state;				/* each LED occupies a bit */
static unsigned int leds_state_blinking;
static unsigned int leds_state_blinking_onoff;

#ifdef CONFIG_RTK_VOIP_GPIO_8972B
static const uint32 leds_gpio_id[] = {
	PIN_VOIP1_LED,  // CH0 - FXS1: output
	PIN_VOIP2_LED,	// CH1 - FXS2: output
	PIN_VOIP3_LED,  // CH2 - FXS3: output
	PIN_VOIP4_LED,	// CH3 - FXS4: output
	//RFU			// DAA CH0
	//RFU			// DAA CH1
	//RFU			// SIP LED ( = VOIP_CH_NUM = SLIC_CH_NUM + DAA_CH_NUM )
};
#elif defined( CONFIG_RTK_VOIP_GPIO_8954C_V100 ) || defined( CONFIG_RTK_VOIP_GPIO_8954C_V200 )
static const uint32 leds_gpio_id[] = {
	PIN_VOIP1_LED,  // CH0 - FXS1: output
	PIN_VOIP2_LED,	// CH1 - FXS2: output
	PIN_PSTN_LED,	// DAA CH0
	//RFU			// DAA CH1
	//RFU			// SIP LED ( = VOIP_CH_NUM = SLIC_CH_NUM + DAA_CH_NUM )
};
#elif defined( CONFIG_RTK_VOIP_GPIO_8954C_V400 ) 
static const uint32 leds_gpio_id[] = {
	PIN_VOIP0_LED,
	PIN_VOIP1_LED,
	PIN_VOIP2_LED
};
#endif

#define NUM_OF_LEDS_GPIO	( sizeof( leds_gpio_id ) / sizeof( leds_gpio_id[ 0 ] ) )

/* We can use virtual CHID to represent SIP LED */
#define SIP_LED_VIRTUAL_CHID	VOIP_CH_NUM

static void display_led_state( unsigned int chid, unsigned int onoff )
{
	if( chid >= NUM_OF_LEDS_GPIO )
		return;

	if( onoff ) {	/* on */
		_rtl_generic_setGpioDataBit( leds_gpio_id[ chid ], 0 );
	} else {		/* off */
		_rtl_generic_setGpioDataBit( leds_gpio_id[ chid ], 1 );
	}
}

static void leds_blinking_timer( unsigned long data )
{
	unsigned int chid;
	unsigned int bit;

	if( leds_state_blinking == 0 )
		return;
	
	for( chid = 0; chid < NUM_OF_LEDS_GPIO; chid ++ ) {
		
		bit = 1 << chid;
		
		if( !( leds_state_blinking & bit ) )
			continue;
		
		display_led_state( chid, leds_state_blinking_onoff & bit );
		
		leds_state_blinking_onoff ^= bit;
	}
	
	/* add timer again */
	leds_timer.expires = jiffies + LED_BLINKING_FREQ;
	add_timer( &leds_timer );
}

static void set_led_state( unsigned int chid, unsigned int state )
{
#if defined (CONFIG_RTK_VOIP_DRIVERS_ATA_DECT) && defined (CONFIG_RTK_VOIP_GPIO_8954C_V200)
	unsigned int bit;
	chid = chid - 1;
	bit = 1 << chid;
#else
	unsigned int bit = 1 << chid;
#endif
	unsigned int old_state;
	
	switch( state ) {
	case 0:	//off 
		leds_state &= ~bit;
		leds_state_blinking &= ~bit;
		
		display_led_state( chid, 0 );
		break;
	
	case 1:	//on
		leds_state |= bit;
		leds_state_blinking &= ~bit;
		
		display_led_state( chid, 1 );
		break;
	
	case 2:	//blinking
		old_state = leds_state_blinking;
	
		leds_state |= bit;
		leds_state_blinking |= bit;
		leds_state_blinking_onoff |= bit;
		
		display_led_state( chid, 1 );
		
		if( old_state == 0 ) {
			/* start timer */
			init_timer( &leds_timer );
			leds_timer.expires = jiffies + LED_BLINKING_FREQ;
			leds_timer.function = leds_blinking_timer;
			add_timer( &leds_timer );
		}
		
		break;
	
	default:
		printk( "Wrong ch(%u) LED state(%u)\n", chid, state );
		break;
	}
}

// FXS
void fxs_led_state( unsigned int chid, unsigned int state )
{
	if( chid >= SLIC_CH_NUM )
		return;
	
	set_led_state( chid, state );
}

// FXO
#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
void fxo_led_state( unsigned int chid, unsigned int state )
{
	if( chid < SLIC_CH_NUM || chid >= VOIP_CH_NUM )
		return;
	
	set_led_state( chid, state );
}
#endif

// SIP
void sip_led_state( unsigned int state )
{
	//set_led_state( SIP_LED_VIRTUAL_CHID, state );
}

static int __init LED_Init( void )
{
	int i = 0;
	
	/* global variables */
	leds_state = 0;
	leds_state_blinking = 0;
	leds_state_blinking_onoff = 0;

	for ( i=0; i < NUM_OF_LEDS_GPIO; i++ ) {
		_rtl_generic_initGpioPin( leds_gpio_id[ i ], GPIO_CONT_GPIO,
								GPIO_DIR_OUT, GPIO_INT_DISABLE );
	}
	
	/* set FSX state */
	for ( i=0; i < SLIC_CH_NUM; i++ )
	{		
		fxs_led_state( i, 0 );
	}
	
	return 0;
}

voip_initcall_led( LED_Init );
#endif 


#if 0
/* =================== For V210 EV Board LED Control ======================== */

static struct timer_list fxs_led_timer;
static struct timer_list fxs_one_led_timer;
static struct timer_list sip_led_timer;

#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
static struct timer_list fxo_led_timer;
#endif

/***** FXS *****/
static void fxs_one_led_blinking(unsigned long data)
{
	static unsigned int gpio_on_off = 0;	//0: off, 1: on.
	
	if (gpio_on_off == 1)
	{
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
		GPAB_DATA &= 0xfffffff7;
#elif defined CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT &= 0xfffffbff;
#elif defined (CONFIG_RTK_VOIP_DRIVERS_PCM8671) && defined (CONFIG_RTK_VOIP_DRIVERS_ATA_SLIC)
		GPB_DATA &= 0xfff7ffff;
#endif
	}
	else
	{
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
		GPAB_DATA |= 0x08;	
#elif defined CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT |= 0x400;
#elif defined (CONFIG_RTK_VOIP_DRIVERS_PCM8671) && defined (CONFIG_RTK_VOIP_DRIVERS_ATA_SLIC)
		GPB_DATA |= 0x00200000;
#endif
	}
	
	gpio_on_off ^= 1;
	fxs_one_led_timer.expires = jiffies + LED_BLINKING_FREQ;
	add_timer(&fxs_one_led_timer);
}

static void fxs_led_state_port1(unsigned int state)
{
	static int fxs_one_timer_init=0;
	
	switch (state) {
	case 0:	//off 
		del_timer_sync(&fxs_one_led_timer);
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
		GPAB_DATA |= 0x08;
#elif defined CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT |= 0x400;
#elif defined (CONFIG_RTK_VOIP_DRIVERS_PCM8671) && defined (CONFIG_RTK_VOIP_DRIVERS_ATA_SLIC)
		GPB_DATA |= 0x00200000;
#endif
		fxs_one_timer_init = 0;
		break;
		
	case 1:	//on
		del_timer_sync(&fxs_one_led_timer);
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
		GPAB_DATA &= 0xfffffff7;
#elif defined CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT &= 0xfffffbff;
#elif defined (CONFIG_RTK_VOIOP_DRIVERS_PCM8671) && defined (CONFIG_RTK_VOIP_DRIVERS_ATA_SLIC)
		GPB_DATA &= 0xfff7ffff;
#endif
		fxs_one_timer_init = 0;
		break;
		
	case 2:	//blinking
		if (fxs_one_timer_init == 0)
		{
			init_timer(&fxs_one_led_timer);
			fxs_one_led_timer.expires = jiffies + LED_BLINKING_FREQ;
			fxs_one_led_timer.function = fxs_one_led_blinking;
			add_timer(&fxs_one_led_timer);
			fxs_one_timer_init = 1;
		}
		break;
		
	default:
		printk("Wrong FXS LED state\n");
		break;			
	}
}

static void fxs_led_blinking(unsigned long data)
{
	static unsigned int gpio_on_off = 0;	//0: off, 1: on.
	
	if (gpio_on_off == 1)
	{
#ifdef _V210_EV_BOARD_
		GPAB_DATA &= 0xffffffef;
#endif
#ifdef _V210_Ed_
		GPAB_DATA &= 0xfffffffb;
#endif	
#ifdef CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT &= 0xffffefff;
#endif
#if defined (CONFIG_RTK_VOIP_DRIVERS_PCM8671) && defined (CONFIG_RTK_VOIP_DRIVERS_ATA_SLIC)
		GPA_DATA &= 0xffdfffff;
#endif
	}
	else
	{
#ifdef _V210_EV_BOARD_
		GPAB_DATA |= 0x10;
#endif
#ifdef _V210_Ed_
		GPAB_DATA |= 0x04;
#endif			
#ifdef CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT |= 0x1000;
#endif			
#if defined (CONFIG_RTK_VOIP_DRIVERS_PCM8671) && defined (CONFIG_RTK_VOIP_DRIVERS_ATA_SLIC)
		GPA_DATA |= 0x200000;
#endif			
	}
	
	gpio_on_off ^= 1;
	fxs_led_timer.expires = jiffies + LED_BLINKING_FREQ;
	add_timer(&fxs_led_timer);
}

static void fxs_led_state_port0(unsigned int state)
{
	static int fxs_timer_init=0;
	
	switch (state) {
	case 0:	//off 
		del_timer_sync(&fxs_led_timer);
#ifdef _V210_EV_BOARD_
		GPAB_DATA |= 0x10;
#endif
#ifdef _V210_Ed_
		GPAB_DATA |= 0x04;
#endif	
#ifdef CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT |= 0x1000;
#endif
#if defined (CONFIG_RTK_VOIP_DRIVERS_PCM8671) && defined (CONFIG_RTK_VOIP_DRIVERS_ATA_SLIC)
		GPA_DATA |= 0x200000;
#endif
		fxs_timer_init = 0;
		break;

	case 1:	//on
		del_timer_sync(&fxs_led_timer);
#ifdef _V210_EV_BOARD_
		GPAB_DATA &= 0xffffffef;
#endif
#ifdef _V210_Ed_
		GPAB_DATA &= 0xfffffffb;
#endif	
#ifdef CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT &= 0xffffefff;
#endif	
#if defined (CONFIG_RTK_VOIP_DRIVERS_PCM8671) && defined (CONFIG_RTK_VOIP_DRIVERS_ATA_SLIC)
		GPA_DATA &= 0xffdfffff;
#endif
		fxs_timer_init = 0;
		break;

	case 2:	//blinking
		if (fxs_timer_init == 0)
		{
			init_timer(&fxs_led_timer);
			fxs_led_timer.expires = jiffies + LED_BLINKING_FREQ;
			fxs_led_timer.function = fxs_led_blinking;
			add_timer(&fxs_led_timer);
			fxs_timer_init = 1;
		}
		break;

	default:
		printk("Wrong FXS1 LED state\n");
		break;			
	}
}

void fxs_led_state(unsigned int chid, unsigned int state)
{
	if (chid == 0)
	{
		fxs_led_state_port0(state);
	}
	else if (chid == 1)
	{
		fxs_led_state_port1(state);
	}
	else
	{
		printk(AC_FORE_RED "Error! No LED for FXS channel = %d\n" AC_RESET, chid);
	}
}


/***** FXO *****/
#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO

static void fxo_led_blinking(unsigned long data)
{
	static unsigned int gpio_on_off = 0;	//0: off, 1: on.
	
	if (gpio_on_off == 1)
	{
#ifdef _V210_EV_BOARD_	
		GPAB_DATA &= 0xfffffffb;
#endif
#ifdef _V210_Ed_
		GPAB_DATA &= 0xfffffff7;
#endif		
#ifdef CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT &= 0xffffdfff;
#endif
	}
	else
	{
#ifdef _V210_EV_BOARD_	
		GPAB_DATA |= 0x04;	
#endif
#ifdef _V210_Ed_
		GPAB_DATA |= 0x08;	
#endif
#ifdef CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT |= 0x2000;
#endif
	}

	gpio_on_off ^= 1;
	fxo_led_timer.expires = jiffies + LED_BLINKING_FREQ;
	add_timer(&fxo_led_timer);
}

static void fxo_led_on(unsigned long data)
{
#ifdef _V210_EV_BOARD_	
	GPAB_DATA &= 0xfffffffb;
#endif	
#ifdef _V210_Ed_
	GPAB_DATA &= 0xfffffff7;
#endif	
#ifdef CONFIG_RTK_VOIP_GPIO_8962
	PEFGH_DAT &= 0xffffdfff;
#endif	
	fxo_led_timer.expires = jiffies + 1;
	add_timer(&fxo_led_timer);
}

static void fxo_led_state_port0(unsigned int state)
{

	static int fxo_blink_timer_init=0;
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
	static int fxo_on_timer_init=0;
#endif

	switch (state)
	{
		case 0:	//off 
			del_timer_sync(&fxo_led_timer);
#ifdef _V210_EV_BOARD_	
			GPAB_DATA |= 0x04;
#endif		
#ifdef _V210_Ed_	
			GPAB_DATA |= 0x08;
#endif	
	
#ifdef CONFIG_RTK_VOIP_GPIO_8962
			PEFGH_DAT |= 0x2000;
#elif defined CONFIG_RTK_VOIP_DRIVERS_PCM8186
			fxo_on_timer_init = 0;
#endif
			fxo_blink_timer_init = 0;
			break;
		
		case 1:	//on
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
			if (fxo_on_timer_init==0)
			{
				del_timer_sync(&fxo_led_timer);
				fxo_on_timer_init = 1;
				init_timer(&fxo_led_timer);
				fxo_led_timer.expires = jiffies + 1;
				fxo_led_timer.function = fxo_led_on;
				add_timer(&fxo_led_timer);
			}
#elif defined CONFIG_RTK_VOIP_GPIO_8962
			del_timer_sync(&fxo_led_timer);
			PEFGH_DAT &= 0xffffdfff;
#endif	
			fxo_blink_timer_init = 0;
			break;
		
		case 2:	//blinking
			if (fxo_blink_timer_init==0)
			{
				del_timer_sync(&fxo_led_timer);
				fxo_blink_timer_init = 1;
				init_timer(&fxo_led_timer);
				fxo_led_timer.expires = jiffies + LED_BLINKING_FREQ;
				fxo_led_timer.function = fxo_led_blinking;
				add_timer(&fxo_led_timer);
			}
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
			fxo_on_timer_init = 0;
#endif
			
			break;

		case 3:
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
			printk("GPAB_DATA=%x\n",GPAB_DATA);
#elif defined CONFIG_RTK_VOIP_GPIO_8962
			printk("PEFGH_DAT=%x\n",PEFGH_DAT);
#endif
			break;

		default:
			printk("Wrong FXO LED state\n");
			break;			
	}
}

void fxo_led_state(unsigned int chid, unsigned int state)
{
#ifdef CONFIG_RTK_VOIP_DRIVERS_SI3050

	if (chid == SLIC_CH_NUM)
	{
		fxo_led_state_port0(state);
	}
	else if (chid == (SLIC_CH_NUM+1))
	{
		printk(AC_FORE_RED "Error! No LED for FXO channel = %d\n" AC_RESET, chid);
		//fxo_led_state_port1(state);
	}
	else
	{
		printk(AC_FORE_RED "Error! No LED for FXO channel = %d\n" AC_RESET, chid);
	}

#elif defined CONFIG_RTK_VOIP_DRIVERS_VIRTUAL_DAA
	fxo_led_state_port0(state);
#endif
}

#endif


/***** SIP *****/
static void sip_led_blinking(unsigned long data)
{
	static unsigned int gpio_on_off = 0;	//0: off, 1: on.
	
	if (gpio_on_off == 1)
	{
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
		GPAB_DATA &= 0xffffffdf;
#elif defined CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT &= 0xffffdfff;
#endif
	}
	else
	{
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
		GPAB_DATA |= 0x20;	
#elif defined CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT |= 0x2000;
#endif
	}
	
	gpio_on_off ^= 1;
	sip_led_timer.expires = jiffies + LED_BLINKING_FREQ;
	add_timer(&sip_led_timer);
}

void sip_led_state(unsigned int state)
{
	
	switch (state) {
	case 0:	//off 
		del_timer_sync(&sip_led_timer);
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
		GPAB_DATA |= 0x20;
#elif defined CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT |= 0x2000;
#endif
		break;
		
	case 1:	//on
		del_timer_sync(&sip_led_timer);
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
		GPAB_DATA &= 0xffffffdf;
#elif defined CONFIG_RTK_VOIP_GPIO_8962
		PEFGH_DAT &= 0xffffdfff;
#endif
		break;
		
	case 2:	//blinking
		del_timer_sync(&sip_led_timer);
		init_timer(&sip_led_timer);
		sip_led_timer.expires = jiffies + LED_BLINKING_FREQ;
		sip_led_timer.function = sip_led_blinking;
		add_timer(&sip_led_timer);
		break;
		
	default:
		printk("Wrong SIP LED state\n");
		break;			
	}
}

void LED_Init(void)
{
	int i = 0;
	
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8186
	GPAB_DIR = GPAB_DIR | 0x3c;
#elif defined CONFIG_RTK_VOIP_GPIO_8962
	//PEFGH_CR &= 0xffffcbff;
        PEFGH_DIR = PEFGH_DIR | 0x3400;
        //PEFGH_DAT = PEFGH_DAT | 0x3400;
#elif defined (CONFIG_RTK_VOIP_DRIVERS_PCM8671) && defined (CONFIG_RTK_VOIP_DRIVERS_ATA_SLIC)
	GPA_DIR = GPA_DIR | 0x00200000;
	GPB_DIR = GPA_DIR | 0x00080000;
#endif

	for (i=0; i < SLIC_CH_NUM; i++)
	{
		fxs_led_state(i, 0);
	}

#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
	for (i=SLIC_CH_NUM; i < VOIP_CH_NUM; i++)
	{
		fxo_led_state(i, 1);	
	}
#endif
}


/* =========================================================================== */
#endif 


