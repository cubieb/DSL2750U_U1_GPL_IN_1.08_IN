/*
* ----------------------------------------------------------------
* TBS GPIO DRIVER FOR RTL8676
* Author: xiachaoren 2011-12-28
* ---------------------------------------------------------------
*/

#include <linux/config.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/mman.h>
#include <linux/ioctl.h>
#include <linux/fd.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/delay.h>
#include <linux/proc_fs.h>
#include <linux/serial_core.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <asm/byteorder.h>
#if defined(CONFIG_RTL8670)
#include "lx4180.h"
#elif defined(CONFIG_RTL8671)// 8671
#include "lx5280.h"
#else
#include <bspchip.h>
#endif
#include "rtl8676_gpio.h"

static int rtl8676_get_gpio_para(int gpio_num, void *ptr)
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
		p->ctrl_addr = (unsigned long)GPIO_PABCD_CNR;
		p->ptype_addr = (unsigned long)GPIO_PABCD_PTYPE;
		p->dir_addr = (unsigned long)GPIO_PABCD_DIR;
		p->data_addr = (unsigned long)GPIO_PABCD_DAT;
		p->isr_addr = (unsigned long)GPIO_PABCD_ISR;
		if(gpio_num <= GPIO_B_7)
			{
			p->imr_addr = GPIO_PAB_IMR;
			}
		else
			{
			p->imr_addr = GPIO_PCD_IMR;
			}
		}
	else if(gpio_num <= GPIO_H_7)
		{
		p->group = 1;
		p->pins = 1 << (gpio_num - GPIO_E_0);
		p->ctrl_addr = (unsigned long)GPIO_PEFGH_CNR;
		p->ptype_addr = (unsigned long)GPIO_PEFGH_PTYPE;		
		p->dir_addr = (unsigned long)GPIO_PEFGH_DIR;
		p->data_addr = (unsigned long)GPIO_PEFGH_DAT;
		p->isr_addr = (unsigned long)GPIO_PEFGH_ISR;
		if(gpio_num <= GPIO_F_7)
			{
			p->imr_addr = GPIO_PEF_IMR;
			}
		else
			{
			p->imr_addr = GPIO_PGH_IMR;
			}
		}
	else if(gpio_num <= RTL8192CD_GPIO_7)
		{  
		p->group = 2;
		p->pins = (gpio_num - RTL8192CD_GPIO_0);
		p->ctrl_addr = (unsigned long)RTL8192CD_GPIO_PIN_CTRL;
		p->ptype_addr = (unsigned long)RTL8192CD_GPIO_PIN_CTRL;	
		p->dir_addr = (unsigned long)RTL8192CD_GPIO_PIN_CTRL;
		p->data_addr = (unsigned long)RTL8192CD_GPIO_PIN_CTRL;
		p->isr_addr = (unsigned long)RTL8192CD_GPIO_FSISR;
		p->imr_addr = (unsigned long)RTL8192CD_GPIO_FSIMR;
		}
	else if(gpio_num <= RTL8192CD_GPIO_A)
		{
		p->group = 3;
		p->pins = (gpio_num - RTL8192CD_GPIO_8);
		p->ctrl_addr = (unsigned long)RTL8192CD_GPIO_LEDCFG;
		p->ptype_addr = (unsigned long)RTL8192CD_GPIO_LEDCFG;	
		p->dir_addr = (unsigned long)RTL8192CD_GPIO_LEDCFG;
		p->data_addr = (unsigned long)RTL8192CD_GPIO_LEDCFG;
		}
	else if(gpio_num <= GPIO_I_F)
		{
		p->group = 4;
		p->pins = 1 << (gpio_num - GPIO_I_0);
		p->data_addr = (unsigned long)BSP_MISC_PINOCR;
		}
	return 0;
}

/* set GPIO_CNR to 0 as GPIO pin, 1 as dedicated peripheral pin */
void gpioConfigCNR(int gpio_num, int mode)
{
	struct rtl8676_gpio_para pa;
	unsigned long pins;

	if(rtl8676_get_gpio_para(gpio_num, &pa) < 0)
		{
		return;
		}
	switch(pa.group)
		{
		case 4:
			return;
		case 3:	/* 1=GPIO */
			pins = (1 << (pa.pins + 22));
			mode = ~mode;
			break;
		case 2:
			pins = (1 << (pa.pins + RTL8192CD_GPIO_CNR_OFFSET));
			break;
		default:
			pins = pa.pins;
			break;
		}
	
	if(mode)
		{/* set GPIO_CNR to 0 as GPIO pin, 1 as dedicated peripheral pin */
		REG32(pa.ctrl_addr) |= pins;
		}
	else
		{
		REG32(pa.ctrl_addr) &= ~(pins);
		}
}

void gpioConfig(int gpio_num, int gpio_func)
{
	struct rtl8676_gpio_para pa;
	unsigned long pins;

	if(rtl8676_get_gpio_para(gpio_num, &pa) < 0)
		{
		return;
		}
	switch(pa.group)
		{
		case 4:
			return;
		case 3:
			pins = (1 << (pa.pins + 21));
			break;
		case 2:
			pins = (1 << (pa.pins + RTL8192CD_GPIO_IO_SEL_OFFSET));
			break;
		default:
			pins = pa.pins;
			break;
		}	
	if(GPIO_FUNC_INPUT == gpio_func)
		{
		REG32(pa.dir_addr) &= ~pins;
		}
	else
		{
		REG32(pa.dir_addr) |= pins;
		}
}

/* read interrupt status register */
int gpioGetIntrStatus(int gpio_num)
{
	struct rtl8676_gpio_para pa;
	unsigned long pins;
	int ret = 0;

	if(rtl8676_get_gpio_para(gpio_num, &pa) < 0)
		{
		goto err;
		}
	if(pa.group >= 3)
		{
		goto err;
		}
	else if(2 == pa.group)
		{
		REG32(RTL8192CD_GPIO_GP70ISR) |= 1;
		pins = (1 << (pa.pins + RTL8192CD_GPIO_ISR_OFFSET));
		}
	else
		{
		pins = pa.pins;
		}
	ret = (REG32(pa.isr_addr) & pins) ? 1 : 0;
	if(1 == ret)
		{/* Write 1 to clear interrupt first */
		REG32(pa.isr_addr) |= pins;
		}
err:	
	return ret;
}

/* GPIO interrupt mode */
void gpioConfigIntr(int gpio_num, int mode)
{
	struct rtl8676_gpio_para pa;
	unsigned long pins;

	if(rtl8676_get_gpio_para(gpio_num, &pa) < 0)
		{
		return;
		}
	if(pa.group >= 3)
		{
		return;
		}
	if(2 == pa.group)
		{
		REG32(RTL8192CD_GPIO_INTM) |= (1 << pa.pins);   /* negative edge is sensed as interrupt for GPIO2 */
		pins = (1 << (pa.pins + RTL8192CD_GPIO_ISR_OFFSET));
		if(mode)
			{
			REG32(pa.imr_addr) |= pins;
			REG32(RTL8192CD_GPIO_GP70IMR) |= (1 << 0);   /* Enable GPIO70 interrupt source */
			}
		else
			{
			REG32(pa.imr_addr) &= ~pins;
			REG32(RTL8192CD_GPIO_GP70IMR) &= ~(1 << 0);   /* Disable GPIO70 interrupt source */
			}
		}
	else
		{
		pins = (gpio_num - pa.group * 32) * 2 - 32;
		REG32(pa.imr_addr) = (REG32(pa.imr_addr) & (~(GPIO_IRQ_EDGE_BOTH << pins))) | (mode << pins);
		}
	
}

void rtl8676_gpio_set(int gpio_num, int mode)
{
	struct rtl8676_gpio_para pa;
	unsigned long pins;

	if(rtl8676_get_gpio_para(gpio_num, &pa) < 0)
		{
		return;
		}
	if(3 == pa.group)
		{
		pins = (1 << (pa.pins + 19));
		}
	else if(2 == pa.group)
		{
		pins = (1 << (pa.pins + RTL8192CD_GPIO_OUT_OFFSET));
		}
	else
		{
		pins = pa.pins;
		}
	if(mode)
		{
		REG32(pa.data_addr) |= pins;
		}
	else
		{
		REG32(pa.data_addr) &= ~pins;
		}	
}

void gpioSet(int gpio_num)
{
	rtl8676_gpio_set(gpio_num, 1);
}

void gpioClear(int gpio_num)
{
	rtl8676_gpio_set(gpio_num, 0);
}

int gpioRead(int gpio_num) 
{
	struct rtl8676_gpio_para pa;
	unsigned long pins;
	int ret = 0;
	
	if(rtl8676_get_gpio_para(gpio_num, &pa) < 0)
		{
		goto err;
		}
	if(3 == pa.group)
		{
		goto err;
		}
	else if(2 == pa.group)
		{
		pins = (1 << (pa.pins + RTL8192CD_GPIO_IN_OFFSET));
		}
	else
		{
		pins = pa.pins;
		}

	ret = (REG32(pa.data_addr) & pins) ? 1 : 0;
	
err:	
	return ret;
}

void PCIE_reset_pin(int *reset)
{
	int gpio_num = GPIO_H_2;

	/*设置为GPIO模式*/
	gpioConfigCNR(gpio_num, 0);
	*reset = gpio_num;
}

#ifdef CONFIG_RTL8676_PWR_SAVE

static int lowp_proc_w(struct file *file, const char *buffer,
				unsigned long count, void *data)
{	
	char tmpbuf[100];
	int num = 0;
	//u32 saved,status;
	//u32 gimr, flags;
	extern void rtl8672_low_power(int);
	
#if 1
	if (buffer && !copy_from_user(tmpbuf, buffer, count)) {
		sscanf(tmpbuf, "%d", &num);
		rtl8672_low_power(num);					
    }
#endif	
	return count;
}

static int lowp_proc_r(char *buf, char **start, off_t offset,
			int length, int *eof, void *data)
{	
	int pos = 0;

	if (length >= 128) {
		pos += sprintf(&buf[pos], "length=%d CLK=%u\n", length, SYSCLK);
		pos += sprintf(&buf[pos], "b8002000=%02x\n", REG8(0xb8002000));
		if( IS_RLE0315 || IS_6166 ) {
			pos += sprintf(&buf[pos], "b8003200=%08x\n", REG32(0xb8003200));
			pos += sprintf(&buf[pos], "b800332C=%08x\n", REG32(0xb800332C));
		}
	}

	//pos += sprintf(&buf[pos], "%d", wifi_debug_level);
	//pos += sprintf(&buf[pos], "\n");

	return pos;
}
#endif

int __init gpio_init(void)
{
	unsigned int sicr;
	#ifdef CONFIG_RTL8676_PWR_SAVE
	struct proc_dir_entry *proc_file1;
	
	proc_file1 = create_proc_entry("low_power",	0644, NULL);
	if(proc_file1 == NULL)
		{
		printk("can't create proc: low_power\r\n");
		}
	else
		{
		proc_file1->write_proc = lowp_proc_w;
		proc_file1->read_proc  = lowp_proc_r;
		}
	#endif
	sicr = REG32(BSP_MISC_PINMUX);	
	if (sicr & BSP_JTAG_GPIO_PINMUX)
		{
		printk( "<<<<<<<disable GPIO JTAG function.\n" );
		REG32(BSP_MISC_PINMUX) = sicr& ~BSP_JTAG_GPIO_PINMUX;
		}
	//init GPIO_PABCD_CNR=0
	REG32(GPIO_PABCD_CNR) = 0x0;
	//init GPIO_PABCD_PTYPE=0
	REG32(GPIO_PABCD_PTYPE) = 0x0;
	/* Set USBLED1 to GPIO mode */
	REG32(BSP_RGMII_PAD_CTRL) &= ~BSP_USBLED_GPIO_SEL;

	return 0;  
}

static void __exit gpio_exit (void)
{
}

EXPORT_SYMBOL_GPL(gpioConfigCNR);
EXPORT_SYMBOL_GPL(gpioConfig);
EXPORT_SYMBOL_GPL(rtl8676_gpio_set);
EXPORT_SYMBOL_GPL(gpioSet);
EXPORT_SYMBOL_GPL(gpioClear);
module_init(gpio_init);
module_exit(gpio_exit);

