/*
 * 文件名:btn.h
 * 说明:TBS按钮驱动头文件
 * 
 * 作者:Zhang Yu
 *
 */

#ifndef __LINUX_BTN_H_INCLUDED
#define __LINUX_BTN_H_INCLUDED

#ifndef __KERNEL__
#undef CONFIG_BUTTON_TRIGGER_TIMER
#endif

#ifdef __BOOT__
#undef CONFIG_BUTTON_TRIGGER_TIMER
#endif

#ifdef CONFIG_BUTTON_TRIGGER_TIMER  /* 轮询方式 */
#include <linux/list.h>
#include <linux/spinlock.h>
#endif

/* 定义按钮名字 */
typedef enum  {
    btn_reset   = 0,
    btn_wps,
    btn_wlan,
    btn_end,  /* 数组结束 */
}btn_name;


typedef enum{
    BTN_UP      = 0,
    BTN_DOWN    = 1,
    BTN_START   = 2,
}btn_status;

typedef enum  {
    BTN_LEVEL_LOW  = 0,
    BTN_LEVEL_HIGH,
}btn_level;


struct btn_dev {
    btn_name 			name;
	int					gpio;				/* GPIO号 */
    btn_level			level;				/* 触发电平高低 */
	btn_status			cur_status;			/* 用于中断模式下保存各个按钮的当前状态，以防止混淆 */
    btn_status (*get_status)(struct btn_dev *);
    int					ignore;
#ifdef CONFIG_BUTTON_TRIGGER_TIMER  /* 轮询方式 */
    struct list_head    node;          /* 按钮设备列表,用于轮询方式 */
#endif
};

/* 定义按钮消息，通过netlink传递给应用层 */
struct btn_msg {
    btn_name name;
    btn_status state;
};


#ifdef CONFIG_BUTTON_TRIGGER_TIMER  /* 轮询方式 */
extern rwlock_t btn_list_lock;
extern struct list_head btn_list;
#endif

extern void btn_status_query(struct btn_dev *btn);
extern int btn_dev_register(struct btn_dev *btn);
extern void btn_dev_unregister(struct btn_dev *btn);
extern void btn_dev_suspend(struct btn_dev *btn);
extern void btn_dev_resume(struct btn_dev *btn);


#endif      /* __LINUX_BTN_H_INCLUDED */
