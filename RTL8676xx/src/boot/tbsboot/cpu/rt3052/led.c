#include <common.h>
#include <asm/byteorder.h>
#include "autoconf.h"
#include "rt_mmap.h"

#define RALINK_SYSCTL_ADDR			RALINK_SYSCTL_BASE	// system control
#define RALINK_REG_GPIOMODE		(RALINK_SYSCTL_ADDR + 0x60)

#define RALINK_IRQ_ADDR			RALINK_INTCL_BASE
#define RALINK_REG_INTENA			(RALINK_IRQ_ADDR + 0x34)
#define RALINK_REG_INTDIS			(RALINK_IRQ_ADDR + 0x38)

#define RALINK_PRGIO_ADDR			RALINK_PIO_BASE // Programmable I/O
#define RALINK_REG_PIOINT			(RALINK_PRGIO_ADDR + 0)
#define RALINK_REG_PIOEDGE			(RALINK_PRGIO_ADDR + 0x04)
#define RALINK_REG_PIORENA			(RALINK_PRGIO_ADDR + 0x08)
#define RALINK_REG_PIOFENA			(RALINK_PRGIO_ADDR + 0x0C)
#define RALINK_REG_PIODATA			(RALINK_PRGIO_ADDR + 0x20)
#define RALINK_REG_PIODIR			(RALINK_PRGIO_ADDR + 0x24)
#define RALINK_REG_PIOSET			(RALINK_PRGIO_ADDR + 0x2C)
#define RALINK_REG_PIORESET		(RALINK_PRGIO_ADDR + 0x30)


#define RALINK_GPIOMODE_I2C		0x01
#define RALINK_GPIOMODE_SPI		0x02
#define RALINK_GPIOMODE_UARTF		0x1C
#define RALINK_GPIOMODE_UARTL		0x20
#define RALINK_GPIOMODE_JTAG		0x40
#define RALINK_GPIOMODE_MDIO		0x80
#define RALINK_GPIOMODE_SDRAM		0x100
#define RALINK_GPIOMODE_RGMII		0x200

#define RALINK_GPIOMODE_DFT		(RALINK_GPIOMODE_UARTF)

extern struct led_dev rt3052_leds[];
extern struct led_dev rt3052_btns[];

#if 0
int GetKeyState( btn_name btn )
{
	int i;
	int gpiomode;
	int KeyValue;
		
	for( i = 0 ;  rt3052_btns[i].name != led_end ; i++ )
	{
		if( rt3052_btns[i].name == btn )
		{
			break;
		}
	}

	gpiomode = le32_to_cpu(*(volatile u32 *)(RALINK_REG_GPIOMODE));
	gpiomode |= RALINK_GPIOMODE_DFT;
	*(volatile u32 *)(RALINK_REG_GPIOMODE) = cpu_to_le32(gpiomode);
	udelay(10);

	*(volatile u32 *)(RALINK_REG_PIODIR) &= cpu_to_le32( ~ (1L << rt3052_btns[i].gpio));
	udelay(10);
	
	KeyValue = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODATA));
	KeyValue = (KeyValue >> rt3052_btns[i].gpio) & 1L;
	
	return KeyValue;
}

int KeyPauseBoot( void )
{
	return GetKeyState( btn_reset );
}


#endif

void _SetLed( led_name led , char state )
{
	int i;
	int gpiomode;
	
	for( i = 0 ;  rt3052_leds[i].name != led_end ; i++ )
	{
		if( rt3052_leds[i].name == led )
		{
			break;
		}
	}
	
	gpiomode = le32_to_cpu(*(volatile u32 *)(RALINK_REG_GPIOMODE));
	gpiomode |= RALINK_GPIOMODE_DFT;
	*(volatile u32 *)(RALINK_REG_GPIOMODE) = cpu_to_le32(gpiomode);
	udelay(10);
	
	*(volatile u32 *)(RALINK_REG_PIODIR) |= cpu_to_le32(1L << rt3052_leds[i].gpio);
	udelay(10);
	
	switch( state )
	{
		case LED_BOOT_ON:
		{
			if( rt3052_leds[i].level == LED_GPIO_HIGH )
			{				
				*(volatile u32 *)(RALINK_REG_PIODATA)  |= cpu_to_le32(1L << rt3052_leds[i].gpio);
				udelay( 10 );
				break;
			}
			else
			{
				*(volatile u32 *)(RALINK_REG_PIODATA) &= cpu_to_le32( ~ (1L << rt3052_leds[i].gpio));
				udelay( 10 );
				break;
			}
		}
		case LED_BOOT_OFF:
		{
			if( rt3052_leds[i].level == LED_GPIO_HIGH )
			{
				*(volatile u32 *)(RALINK_REG_PIODATA) &= cpu_to_le32( ~ (1L << rt3052_leds[i].gpio));
				udelay( 10 );
				break;
			}
			else
			{				
				*(volatile u32 *)(RALINK_REG_PIODATA)  |= cpu_to_le32(1L << rt3052_leds[i].gpio);
				udelay( 10 );
				break;
			}
		}
	}
}

int _init_led( void )
{
	int i;
	for( i = 0 ;  rt3052_leds[i].name != led_end ; i++ )
	{
		_SetLed( rt3052_leds[i].name , rt3052_leds[i].boot_default );
	}

	return 0;
}

