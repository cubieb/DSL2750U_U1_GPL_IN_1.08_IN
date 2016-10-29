/**
 * TBS_TAG: ZhangYu 2008-12-2
 * Desc:    Infineon Amazon-SE方案的按钮驱动
 * 
 **/

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

#include <btn.h>



//#define BTN_DEBUG

#ifdef BTN_DEBUG
    #define BTN_TRACE printk
#else
    #define BTN_TRACE(str, args...)  do { ; } while(0);
#endif

extern struct btn_dev amazon_se_btns[];  /* 定义在ar7130_pb42_product.c中 */

/* Amazon-SE的原版驱动，定义在arch/mips下 */
extern int amazon_se_port_reserve_pin(int port, int pin, int module_id);
extern int amazon_se_port_set_open_drain(int port, int pin, int module_id); 
extern int amazon_se_port_clear_altsel0(int port, int pin, int module_id);
extern int amazon_se_port_clear_altsel1(int port, int pin, int module_id);
extern int amazon_se_port_set_dir_in(int port, int pin, int module_id);
extern int amazon_se_port_get_input(int port, int pin, int module_id);

/* 核心驱动通过此函数获得按钮状态 */
btn_status amazon_se_get_status(struct btn_dev *btn)
{
	int ret;

	ret = amazon_se_port_get_input(0, btn->gpio, (unsigned int)THIS_MODULE);
	
	BTN_TRACE("button %d is on sattus %d\n", btn->name, ret);

	/* 如果是低电平触发 */
	if(btn->level == BTN_LEVEL_LOW){
		if(ret == 0)
			return BTN_DOWN;
		else
			return BTN_UP;
	}
	else{   /* 如果是高电平触发 */
		if(ret == 0)
			return BTN_UP;
		else
			return BTN_DOWN;
	}

}

static int __init amazon_se_btn_init(void)
{
	int ret;
	int i;
	
	for(i=0;amazon_se_btns[i].name != btn_end; i++){

		/* 初始化GPIO接口的工作状态 */
		ret = amazon_se_port_reserve_pin (0, amazon_se_btns[i].gpio, (unsigned int)THIS_MODULE);
		
		if (ret != 0) {
			printk (KERN_ERR "ERROR:The GPIO %d has been used\n", amazon_se_btns[i].gpio);
			return -1;
		}

		amazon_se_port_clear_altsel0(0, amazon_se_btns[i].gpio, (unsigned int)THIS_MODULE);
		amazon_se_port_clear_altsel1(0, amazon_se_btns[i].gpio, (unsigned int)THIS_MODULE);
		amazon_se_port_set_dir_in(0, amazon_se_btns[i].gpio, (unsigned int)THIS_MODULE);


		/* 初始各个BTN设备的初始状态 */
		amazon_se_btns[i].get_status = amazon_se_get_status;

		ret = btn_dev_register(&amazon_se_btns[i]);
		
		if (ret != 0) {
			printk (KERN_ERR "Unable to register btn_dev %d(error %d)\n", amazon_se_btns[i].name, ret);
			return -1;
		}
	
	}

	return 0;

}


static void __exit amazon_se_btn_exit(void)
{
	/* do nothing */
}

module_init(amazon_se_btn_init);
module_exit(amazon_se_btn_exit);

MODULE_AUTHOR("Zhang Yu");
MODULE_DESCRIPTION("Amazon-SE buttons driver");
MODULE_LICENSE("GPL");
