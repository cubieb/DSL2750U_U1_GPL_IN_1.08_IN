#include <common.h>
#include <config.h>
#include <types.h>
#include <autoconf.h>
#include "rtl8676_asicregs.h"
#include "rtl8676_gpio.h"


int get_rtl8676_gpio_para(int gpio_num, void *ptr)
{
	struct rtl8676_gpio_para *p = (struct rtl8676_gpio_para *)ptr;

	if((gpio_num >= GPIO_END) || (gpio_num < 0))
		{
		return -1;
		}
	if(gpio_num <= GPIO_D_7)
		{
		p->group = 0;
		p->pins = 1 << gpio_num;
		p->ctrl_addr = (unsigned long)PABCD_CNR;
		p->ptype_addr = (unsigned long)PABCD_PTYPE;
		p->dir_addr = (unsigned long)PABCD_DIR;
		p->data_addr = (unsigned long)PABCD_DAT;
		}
	else if(gpio_num <= GPIO_H_7)
		{
		p->group = 1;
		p->pins = 1 << (gpio_num - GPIO_E_0);
		p->ctrl_addr = (unsigned long)PEFGH_CNR;
		p->ptype_addr = (unsigned long)PEFGH_PTYPE;		
		p->dir_addr = (unsigned long)PEFGH_DIR;
		p->data_addr = (unsigned long)PEFGH_DAT;
		}
#ifdef CONFIG_RTL8192CD		
	else if (gpio_num <= RTL8192CD_GPIO_A)
		{
		p->group = 2;
		}
#endif
#ifdef CONFIG_RTL8271BVN
	else if (gpio_num <= GPIO_I_F)
		{
		p->group = 3;
		p->pins = 1 << (gpio_num - GPIO_I_0);
		p->ctrl_addr = (unsigned long)BSP_MISC_PINMUX;
		p->data_addr = (unsigned long)BSP_MISC_PINOCR;
		}
#endif	
	return 0;
}

void rtl8676_set_gpio_dir(struct rtl8676_gpio_para *para, int gpio_func)
{
	unsigned long val;

	if((para) && ((0 == para->group) || (1 == para->group)))
		{
		/* Set Pins as GPIO */
		val = REG32(para->ctrl_addr);
		val &= ~(para->pins);
		REGWRITE32(para->ctrl_addr, val);
		/* Configure Shared Pins as GPIO Mode */
		val = REG32(para->ptype_addr);
		val &= ~(para->pins);
		REGWRITE32(para->ptype_addr, val);
		val = REG32(para->dir_addr);
		if(GPIO_FUNC_INPUT == gpio_func)
			{
			val &= ~(para->pins);
			}
		else
			{
			val |= para->pins;
			}		
		REGWRITE32(para->dir_addr, val);
		}
}


