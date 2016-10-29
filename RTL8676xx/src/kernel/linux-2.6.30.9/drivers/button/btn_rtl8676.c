/**
 * 文件名: btn_rtl8676.c
 * 说  明: RTL8676方案的按钮驱动
 * 作  者: xiachaoren
 **/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/config.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/signal.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/resource.h>
#include <linux/timer.h>
#include <linux/sched.h>
#include <asm/types.h>
#include <asm/irq.h>
#include <btn.h>
#include <bspchip.h>
#include <rtl8676_gpio.h>

#ifdef CONFIG_BUTTON_DEBUG
#define BTN_TRACE printk
#else
#define BTN_TRACE(str, args...)  do { ; } while(0);
#endif


/* 定义在product.c中 */
extern struct btn_dev rtl8676_btns[];



#ifdef CONFIG_BUTTON_TRIGGER_TIMER
/*=======================================================================
* 使用定时器轮询检查按钮状态
=======================================================================*/

/* 核心驱动通过此函数获得按钮状态 */
btn_status rtl8676_get_button_status(struct btn_dev *btn)
{
	int ret;

	ret =  gpioRead(btn->gpio);
	BTN_TRACE("button %d is on sattus %d\n", btn->name, ret);

	if(btn->level == BTN_LEVEL_LOW)
		{	/* 低电平触发 */
		if(ret == 0)
			return BTN_DOWN;
		else
			return BTN_UP;
		}
	else
		{    /* 高电平触发 */
		if(ret == 0)
			return BTN_UP;
		else
			return BTN_DOWN;
		}
}

static int __init rtl8676_btn_init(void)
{
	int ret = 0;
	int i;

	for(i=0;rtl8676_btns[i].name != btn_end; i++)
		{
		/*设置为GPIO模式*/
		gpioConfigCNR(rtl8676_btns[i].gpio, 0);
		/* 初始化GPIO接口的工作状态，设置为输入模式，避免误触发 */
		gpioConfig(rtl8676_btns[i].gpio, GPIO_FUNC_INPUT);
		/* 设置获取按钮状态句柄 */
		rtl8676_btns[i].get_status = rtl8676_get_button_status;
		ret = btn_dev_register(&rtl8676_btns[i]);
		if(ret != 0)
			{
			printk (KERN_ERR "Unable to register btn_dev %d(error %d)\n", rtl8676_btns[i].name, ret);
			ret = -1;
			break;
			}
		}
	return ret;
}

static void __exit rtl8676_btn_exit(void)
{
    int i;

    /* 注销所有的btn_dev */
    for(i=0;rtl8676_btns[i].name != btn_end; i++)
	{
		btn_dev_unregister(&rtl8676_btns[i]);
	}
}


#else

#define DEYLAY_TIME     20           //抖动延迟时间(毫秒)
struct timer_list btn_timer;            /* 抖动消除定时器 */


/************************************************************************
* 使用中断模式检测按钮状态
************************************************************************/

void rtl8676_button_timer_handler(unsigned long data)
{
    struct btn_dev *btn = (struct btn_dev *)data;
    int state;
	int i;

	state = gpioRead(btn->gpio);

    BTN_TRACE("\nS %s: GIMR=0x%x, GPIO_PEFGH_CNR=0x%x, GPIO_PEFGH_PTYPE=0x%x, GPIO_PEFGH_DIR=0x%x\n"
              "GPIO_PEFGH_DAT=0x%x, GPIO_PEFGH_ISR=0x%x, GPIO_PEF_IMR=0x%x, GPIO_PGH_IMR=0x%x, gpio(%d)=%#x\n", 
              __func__, REG32(BSP_GIMR), REG32(GPIO_PEFGH_CNR), REG32(GPIO_PEFGH_PTYPE), REG32(GPIO_PEFGH_DIR),
              REG32(GPIO_PEFGH_DAT), REG32(GPIO_PEFGH_ISR), REG32(GPIO_PEF_IMR), REG32(GPIO_PGH_IMR),
              btn->gpio, gpioRead(btn->gpio)
    );
	
    if((state == btn->level) && (BTN_UP == btn->cur_status))
    	{
        btn->cur_status = BTN_DOWN;
		printk("Button %d pressed down, gpio number=%d\n", btn->name, btn->gpio);
		btn_status_query(btn);	
    	}
    else if ((state != btn->level) && (BTN_DOWN == btn->cur_status))
    	{
		btn->cur_status = BTN_UP;
		printk("Button %d released up, gpio number=%d\n", btn->name, btn->gpio);	
    	}

		for(i=0; rtl8676_btns[i].name != btn_end; i++)
			{/* 重新开启GPIO中断 */
			if(btn_wps != rtl8676_btns[i].name)
				{
				gpioConfigIntr(rtl8676_btns[i].gpio, GPIO_IRQ_EDGE_BOTH);
				}
			}	
		
	BTN_TRACE("\nE %s: GIMR=0x%x, GPIO_PEFGH_CNR=0x%x, GPIO_PEFGH_PTYPE=0x%x, GPIO_PEFGH_DIR=0x%x\n"
		"GPIO_PEFGH_DAT=0x%x, GPIO_PEFGH_ISR=0x%x, GPIO_PEF_IMR=0x%x, GPIO_PGH_IMR=0x%x, gpio(%d)=%#x\n", 
              __func__, REG32(BSP_GIMR), REG32(GPIO_PEFGH_CNR), REG32(GPIO_PEFGH_PTYPE), REG32(GPIO_PEFGH_DIR),
              REG32(GPIO_PEFGH_DAT), REG32(GPIO_PEFGH_ISR), REG32(GPIO_PEF_IMR), REG32(GPIO_PGH_IMR),
              btn->gpio, gpioRead(btn->gpio)
    );

}


static irqreturn_t rtl8676_gpio_irq_handler(int irq, void *dev_id)
{
	int i;

	for(i=0; rtl8676_btns[i].name != btn_end; i++)
		{/* 关闭GPIO中断，防止重入 */
		if(btn_wps != rtl8676_btns[i].name)
			{
			gpioConfigIntr(rtl8676_btns[i].gpio, GPIO_IRQ_EDGE_DISABLE);
			}
		}
	for(i=0; rtl8676_btns[i].name != btn_end; i++)
		{
		if(btn_wps != rtl8676_btns[i].name)
			{
			if(gpioGetIntrStatus(rtl8676_btns[i].gpio))
				{/* 抖动消除 */
				setup_timer(&btn_timer, rtl8676_button_timer_handler, (unsigned long)(&rtl8676_btns[i]));
				mod_timer(&btn_timer, jiffies + msecs_to_jiffies(DEYLAY_TIME));
				}
			}
		}
	return IRQ_RETVAL(IRQ_HANDLED);
}


/* 核心驱动通过此函数获得按钮状态 */
btn_status rtl8676_get_button_status(struct btn_dev *btn)
{
	return btn->cur_status;
}

static int __init rtl8676_btn_init(void)
{
	int i;
	int ret;

    BTN_TRACE("Before button init:\n"
        "GIMR=0x%x, GPIO_PEFGH_CNR=0x%x, GPIO_PEFGH_PTYPE=0x%x, GPIO_PEFGH_DIR=0x%x\n"
        "GPIO_PEFGH_DAT=0x%x, GPIO_PEFGH_ISR=0x%x, GPIO_PEF_IMR=0x%x, GPIO_PGH_IMR=0x%x\n",
        REG32(BSP_GIMR), REG32(GPIO_PEFGH_CNR), REG32(GPIO_PEFGH_PTYPE), REG32(GPIO_PEFGH_DIR),
        REG32(GPIO_PEFGH_DAT), REG32(GPIO_PEFGH_ISR), REG32(GPIO_PEF_IMR), REG32(GPIO_PGH_IMR)
    );

	for(i = 0; rtl8676_btns[i].name != btn_end; i++)
		{	/* 初始各个BTN设备的初始状态 */
		if(btn_wps != rtl8676_btns[i].name)
			{
			rtl8676_btns[i].ignore = 0;
			rtl8676_btns[i].cur_status = BTN_UP;
			rtl8676_btns[i].get_status = rtl8676_get_button_status;	
			gpioConfigCNR(rtl8676_btns[i].gpio, 0);	/* 设置指定的GPIO为输入模式 */
			gpioConfig(rtl8676_btns[i].gpio, GPIO_FUNC_INPUT);
			gpioConfigIntr(rtl8676_btns[i].gpio, GPIO_IRQ_EDGE_BOTH); /* 开启中断, 设置指定的GPIO的中断触发模式为边沿触发 */
			}
		}
		ret = request_irq(BSP_GPIO_EFGH_IRQ, rtl8676_gpio_irq_handler, IRQF_SHARED, "GPIO_EFGH", (void *)BSP_GPIO_EFGH_IRQ);
		if (ret != 0)
			{
			printk (KERN_ERR "unable to request IRQ for GPIO (error %d)\n", ret);
			return -1;
			}
		printk(KERN_INFO "TBS button driver for rtl8676 initialized\n");
	    BTN_TRACE("After button init:\nGIMR=0x%x, GPIO_PEFGH_CNR=0x%x, GPIO_PEFGH_PTYPE=0x%x, GPIO_PEFGH_DIR=0x%x\n"
        	"GPIO_PEFGH_DAT=0x%x, GPIO_PEFGH_ISR=0x%x, GPIO_PEF_IMR=0x%x, GPIO_PGH_IMR=0x%x\n",
	        REG32(BSP_GIMR), REG32(GPIO_PEFGH_CNR), REG32(GPIO_PEFGH_PTYPE), REG32(GPIO_PEFGH_DIR),
    	    REG32(GPIO_PEFGH_DAT), REG32(GPIO_PEFGH_ISR), REG32(GPIO_PEF_IMR), REG32(GPIO_PGH_IMR)
			);
	
	return 0;
}


static void __exit rtl8676_btn_exit(void)
{
	int i;

    BTN_TRACE("Before button exit:\n"
        "GIMR=0x%x, GPIO_PEFGH_CNR=0x%x, GPIO_PEFGH_PTYPE=0x%x, GPIO_PEFGH_DIR=0x%x\n"
        "GPIO_PEFGH_DAT=0x%x, GPIO_PEFGH_ISR=0x%x, GPIO_PEF_IMR=0x%x, GPIO_PGH_IMR=0x%x\n",
        REG32(BSP_GIMR), REG32(GPIO_PEFGH_CNR), REG32(GPIO_PEFGH_PTYPE), REG32(GPIO_PEFGH_DIR),
        REG32(GPIO_PEFGH_DAT), REG32(GPIO_PEFGH_ISR), REG32(GPIO_PEF_IMR), REG32(GPIO_PGH_IMR)
    );

	for(i=0;rtl8676_btns[i].name != btn_end; i++)
		{
		if(btn_wps != rtl8676_btns[i].name)
			{
			/* 关闭GPIO中断 */
			gpioConfigIntr(rtl8676_btns[i].gpio, GPIO_IRQ_EDGE_DISABLE);		
	        /* 设置指定的GPIO为GPIO模式 */
			gpioConfigCNR(rtl8676_btns[i].gpio, 0);
			/* 设置指定的GPIO为输入模式 */
			gpioConfig(rtl8676_btns[i].gpio, GPIO_FUNC_INPUT);
			rtl8676_btns[i].cur_status = BTN_UP;
			}
		}
	free_irq(BSP_GPIO_EFGH_IRQ, (void *)BSP_GPIO_EFGH_IRQ);
	del_timer(&btn_timer);
}
#endif


module_init(rtl8676_btn_init);
module_exit(rtl8676_btn_exit);

MODULE_AUTHOR("XiaChaoRen");
MODULE_DESCRIPTION("RTL8676 buttons driver");
MODULE_LICENSE("GPL");


