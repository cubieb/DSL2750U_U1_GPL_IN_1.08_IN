/*
 * Buttons for IKANOS vx180
 *
 * Zhang Yu
 *
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>


#include <linux/config.h>
#include <linux/module.h>
#include <linux/signal.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/resource.h>
#include <asm/types.h>
#include <asm/irq.h>
#include <ikan6850.h>

#include <btn.h>



extern struct btn_dev vx180_btns[];  /* 定义在product.c中 */

/* GPIO驱动接口*/
extern void gpioSetIntrEN(unsigned short pin);
extern void gpioClearIntrEN(unsigned short pin);
extern void gpioSetEdge(unsigned short pin);
extern void gpioClearEdge(unsigned short pin);
extern void gpioSetPolar(unsigned short pin);
extern void gpioClearPolar(unsigned short pin);

extern volatile unsigned short *gpio_flag_set1;
extern volatile unsigned short *gpio_flag_clear1;

extern volatile unsigned short *gpio_flag_set2;
extern volatile unsigned short *gpio_flag_clear2;



void vx180_button_irq(struct btn_dev *btn)
{
    if (btn->ignore) {
		gpioSetIntrEN(btn->gpio);  /* Enable interrupt */
		gpioClearEdge(btn->gpio);  /* Level mode */
		gpioSetPolar(btn->gpio);   /* Low level interrupt */
	    btn->ignore = 0;
		btn->cur_status = BTN_UP;
		printk("%d button up\n",btn->name);
        
    }else
    	{
		gpioSetIntrEN(btn->gpio);  /* Enable interrupt */
		gpioClearEdge(btn->gpio);  /* Level mode */
		gpioClearPolar(btn->gpio); /* High level interrupt */
		btn->ignore = 1;
		btn->cur_status =  BTN_DOWN;
		printk("%d button down\n",btn->name);
		btn_status_query(btn);
    	}
	
    return ;
	
}

irqreturn_t vx180_gpio_irq_dispatch(int cpl, void *dev_id, struct pt_regs *regs)
{
	unsigned short int gpio_value;
	int i;

	if((int)dev_id == GPIO_INT)
	{
		gpio_value = *gpio_flag_set1;

		for(i=0;vx180_btns[i].name != btn_end;i++)
		{
			if((vx180_btns[i].gpio<16) && ((0x1 << vx180_btns[i].gpio) & gpio_value )) 
			{
				*gpio_flag_clear1 = (1 << vx180_btns[i].gpio);
				vx180_button_irq(&vx180_btns[i]);  /* 对触发中断的GPIO进行处理 */
	
			}
		}
 
	}else {
		if((int)dev_id == GPIO2_INT)
		{
			gpio_value = *gpio_flag_set2;
			for(i=0;vx180_btns[i].name != btn_end;i++)
			{
				if((vx180_btns[i].gpio>=16) && ((0x1 << (vx180_btns[i].gpio-16)) & gpio_value )) 
				{
	
					*gpio_flag_clear2 = (1 << (vx180_btns[i].gpio-16));
					vx180_button_irq(&vx180_btns[i]);  /* 对触发中断的GPIO进行处理 */
				
				}
			}
		

		}
	}
	
	return IRQ_HANDLED;
}


/* 核心驱动通过此函数获得按钮状态 */
btn_status vx180_get_status(struct btn_dev *btn)
{
			return btn->cur_status;
}



static int __init vx180_btn_init(void)
{

	int i;
	int ret;
	
	for(i=0;vx180_btns[i].name != btn_end; i++){

		/* 初始化GPIO接口的工作状态，设置为中断模式 */
		gpioSetIntrEN(vx180_btns[i].gpio);  /* Enable interrupt */
		gpioClearEdge(vx180_btns[i].gpio);  /* Level mode */
		gpioSetPolar(vx180_btns[i].gpio);   /* Low level interrupt */

		/* 初始各个BTN设备的初始状态 */
		vx180_btns[i].ignore = 0;
		vx180_btns[i].cur_status = BTN_UP;
		vx180_btns[i].get_status = vx180_get_status;
	
	}
	

	ret = request_irq (GPIO_INT, vx180_gpio_irq_dispatch, 0,
                       "GPIO", (void *)GPIO_INT);
	if (ret != 0) 
	{
		printk (KERN_ERR "unable to request IRQ for GPIO (error %d)\n", ret);
		return -1;
	}

	ret = request_irq (GPIO2_INT, vx180_gpio_irq_dispatch, 0,
                       "GPIO2",(void *)GPIO2_INT);
	if (ret != 0) 
	{
		printk (KERN_ERR "unable to request IRQ for GPIO2 (error %d)\n", ret);
		return -1;
	}

	return 0;
}

static void __exit vx180_btn_exit(void)
{
	/* do nothing */
}

module_init(vx180_btn_init);
module_exit(vx180_btn_exit);

MODULE_AUTHOR("Zhang Yu");
MODULE_DESCRIPTION("vx180 buttons driver");
MODULE_LICENSE("GPL");
