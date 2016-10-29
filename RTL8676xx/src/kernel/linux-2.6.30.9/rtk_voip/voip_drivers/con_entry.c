#include "rtk_voip.h"
#include "voip_init.h"
#ifdef CONFIG_RLE0412
#include <bspchip.h> 
#endif


#if defined CONFIG_RTK_VOIP_DRIVERS_PCM8186
#if RTL8186PV_RELAY_USE_Z  // For Z-version board 8186PV
#define RELAY_SW_CTRL_GPIOC	//for gpioC use. pull relay high. for Z-version board 1*FXS 1*FXO.
#else
#if (!defined (CONFIG_RTK_VOIP_DRIVERS_PCM8186V_OC)) && (!defined (CONFIG_RTK_VOIP_DRIVERS_PCM8651_2S_OC))
#ifdef CONFIG_RTK_VOIP_DRIVERS_8186V_ROUTER
#define RELAY_SW_CTRL_GPIOD	//for gpioD used.pull relay high. for new 4-LAN EV board. 2006-07.
#else
#define RELAY_SW_CTRL_GPIOE	//for gpioE used.pull relay high.
#endif
#endif
#endif
#endif

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8672
/* Configure Cache at write allocate mode */
void set_write_allocate( void )
{
	unsigned int  temp;
__asm__ __volatile__ \
("	;\n\t"
	"mfc0	$8, $20		;\n\t"
	"nop					;\n\t"
	"or 		$8, $8, 0x80	;\n\t"
	"nop					;\n\t"
	"mtc0	$8, $20		;\n\t"
	"nop					;\n\t"
	"nop					;\n\t"
	"mfc0	%0, $20		;\n\t"
	:"=r" (temp)
	:
);
	//printk("\r\nset_write_allocate : temp:%X", temp);

}

void read_write_allocate( void )
{
	unsigned int  temp;
__asm__ __volatile__ \
("	;\n\t"
	"nop					;\n\t"
	"mfc0	%0, $20		;\n\t"
	:"=r" (temp)
	:
);
	printk("\r\nread_write_allocate : temp:%X", temp);

}

unsigned int testa[20];
unsigned int testb[20];
void test_instr()
{

	unsigned int *x= &testa[0];
	unsigned int *y= &testb[0];

	read_write_allocate();
	printk("\r\nx:%X", x);
//	testa[0] = 1;
//	testa[1] = 2;
//	memcpy64s(x, y, 8);
//	x = (int)x | 0xa0000000;
#if 1
__asm__ __volatile__ \
("	\n\t"
	"nop\n\t"
	"move $14 ,%0  \n\t"
	"stp $12, ($14)+8	\n\t"
	"nop					\n\t"
	: /* no output */
	: "r" (x)
);
#endif
}

void test_instr2()
{

	unsigned int *x= &testa[0];
//	testa[0] = 1;
//	testa[1] = 2;
__asm__ __volatile__ \
("	\n\t"
	"nop\n\t"
	"move $14 ,%0  \n\t"
	"swp $12, ($14)+4 \n\t"
	"swp $13, ($14)+4	\n\t"
	"nop					\n\t"
	: /* no output */
	: "r" (x)
	);

}
#endif /*CONFIG_RTK_VOIP_DRIVERS_PCM8672*/

static int __init voip_con_start_setup_init( void )
{
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8672
        int val;
#endif
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8972B_FAMILY
	extern void rtl8972B_hw_init(int mode);
#endif

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM89xxC
	extern void rtl8954C_hw_init(int mode);
#endif

/* move to setup_arch*/
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8672
	set_write_allocate();
	printk("\r\nuse stp\n");
	printk("%s-%d\n", __FUNCTION__, __LINE__);
	test_instr();
	printk("%s-%d\n", __FUNCTION__, __LINE__);
#endif
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8672

#ifdef CONFIG_RLE0412

	*(volatile int*) BSP_IP_SEL |= (BSP_EN_PCM | BSP_EN_VOIPACC);
	printk("BSP_IP_SEL:[%08x]=%08x\n", BSP_IP_SEL, *(volatile int*) BSP_IP_SEL );
	
	*(volatile int*) BSP_MISC_PINMUX |= BSP_VOIP_GPIO_PINMUX_G1;
	*(volatile int*) BSP_MISC_PINMUX &= ~(BSP_VOIP_GPIO_PINMUX_G0);
	printk("BSP_MISC_PINMUX:[%08x]=%08x\n", BSP_MISC_PINMUX, *(volatile int*) BSP_MISC_PINMUX );
#else
	//do we need any settings here?
	//writel(0xb800330C,readl(0xb800330C)|BIT(8));
	val = *(volatile int*)0xb800330C;
	val |=  0x100;
	*(volatile int*)0xb800330C = val;
	printk("PCM:[%08x]=%08x",0xb800330C, *(volatile int*)0xb800330C );
	val = *(volatile int*)0xb8003300;
	val |=  0x400000;//bit 22
	*(volatile int*)0xb8003300 = val;
#endif
	
#endif

#ifdef  RELAY_SW_CTRL_GPIOC//for gpioC used.pull relay high.	
	#define GPCD_DIR  *((volatile unsigned int *)0xbd010134)
	#define GPCD_DATA  *((volatile unsigned int *)0xbd010130)	
	BOOT_MSG("GPCD_DIR = %x\n",GPCD_DIR);
	GPCD_DIR = GPCD_DIR | 0x01; 
	BOOT_MSG("GPCD_DIR = %x\n",GPCD_DIR);
	GPCD_DATA = GPCD_DATA & 0xfffffffe;
#endif

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM8972B_FAMILY
    #ifdef CONFIG_RTK_VOIP_DRIVERS_IIS
	rtl8972B_hw_init(1); /* need init iis */
    #else
	rtl8972B_hw_init(0); /* just init pcm */
    #endif
#endif /* CONFIG_RTK_VOIP_DRIVERS_PCM8972B_FAMILY */

#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM89xxC
    #ifdef CONFIG_RTK_VOIP_DRIVERS_IIS
	rtl8954C_hw_init(1); /* need init iis */
    #else
	rtl8954C_hw_init(0); /* just init pcm */
    #endif
#endif /* CONFIG_RTK_VOIP_DRIVERS_PCM89xxC */
	
	return 0;
}

voip_initcall_entry( voip_con_start_setup_init );

