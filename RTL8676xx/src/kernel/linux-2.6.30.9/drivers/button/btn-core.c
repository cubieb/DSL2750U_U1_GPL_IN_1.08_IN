/*
 * 文件名:btn-class.c
 *
 * 说明:TBS按钮驱动核心部分
 *
 * 作者:张喻
 *
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/sysdev.h>
#include <linux/timer.h>
#include <linux/err.h>
#include <linux/ctype.h>
#include <linux/netlink.h>
#include <net/sock.h>
#include <linux/proc_fs.h>
#include <asm/uaccess.h>
#include <btn.h>
#include <tbs_message.h>
#include <autoconf.h>


#ifdef CONFIG_BUTTON_DEBUG
#define BTN_TRACE printk
#else
#define BTN_TRACE(str, args...)  do { ; } while(0);
#endif

#ifdef CONFIG_BUTTON_TRIGGER_TIMER  /* 轮询方式 */
DEFINE_RWLOCK(btn_list_lock);
LIST_HEAD(btn_list);
#endif

btn_status current_btn_status;
static int btn_query_lock;

rwlock_t  netlink_lock;
static struct sock *netlink_btn_sock;
static int blp_cmm_pid = 0;

#ifdef CONFIG_BUTTON_TRIGGER_TIMER  /* 轮询方式 */
struct timer_list trigger_timer;
#define TRIGGER_TIMER_DEYLAY		500
#endif

struct timer_list query_timer;
#define QUERY_TIMER_DEYLAY		20



#define BLP_BTN_CMM_SRCMID		0x1
#define BLP_BTN_CMM_DSTMID		0x2
#define BLP_BTN_CMM_MSGID		0x3
#define BLP_BTN_CMM_MSGTYPE_CHANGE	0x4

static void recv_handler(struct sk_buff * skb)
{
	//wake_up(sk->sk_sleep);
}

/* 通过netlink方式发送消息到应用层 */
static void send_to_user(struct btn_dev *btn, btn_status btn_status)
{

	int ret;
	int size;
	ST_MSG *tbs_msg;
	struct btn_msg *button_msg;
	unsigned char *old_tail;
	struct sk_buff *skb;
	struct nlmsghdr *nlh;

    /* 如果是中断方式，则不需要做任何处理 */
	BTN_TRACE("gpio=%d, btn_name=%d, btn_status=%d\n",btn->gpio, btn->name, btn_status);

	if(blp_cmm_pid == 0)
	{
		printk("blp_cmm's pid = %d.Please run blp_cmm.\n",blp_cmm_pid);
	}


	size = sizeof(ST_MSGHEAD) + sizeof(struct btn_msg);


	/*  size = msg header + data size */

	size = NLMSG_SPACE(size);

	skb = alloc_skb(size, GFP_ATOMIC);
	old_tail = skb->tail;

	/* init msg header */
	nlh = NLMSG_PUT(skb, 0, 0, NETLINK_TBS, size-sizeof(*nlh));

	/* point to msg data area */
	tbs_msg = NLMSG_DATA(nlh);

	/* fill data for sending */
	memset(tbs_msg, 0, size);

	tbs_msg->stMsgHead.usSrcMID = BLP_BTN_CMM_SRCMID;
	tbs_msg->stMsgHead.usDstMID = BLP_BTN_CMM_DSTMID;
	tbs_msg->stMsgHead.ulMsgID  = BLP_BTN_CMM_MSGID;
	tbs_msg->stMsgHead.usMsgType = BLP_BTN_CMM_MSGTYPE_CHANGE;

	button_msg = (struct btn_msg *)tbs_msg->szMsgBody;

	button_msg->name = btn->name;

	if(btn_status == BTN_DOWN)
	{
		button_msg->state = BTN_DOWN;
	}
	else
	{
		button_msg->state = BTN_UP;
	}


	//printk("button_driver: netlink sending msg is : %s \n\r", tbs_msg->szMsgBody);
	/* get netlink msg length */
	nlh->nlmsg_len = skb->tail - old_tail;

	NETLINK_CB(skb).pid = 0;
	NETLINK_CB(skb).dst_group = 0;
	//NETLINK_CB(skb).dst_pid = blp_cmm_pid; // by XiaChaoRen, kernel2.6.30 removed dst_pid directly to netlink_unicast() since 2.6.20

	/* send msg */
	read_lock_bh(&netlink_lock);
	ret = netlink_unicast(netlink_btn_sock, skb, blp_cmm_pid, MSG_DONTWAIT);
	read_unlock_bh(&netlink_lock);

	BTN_TRACE("Button netlink message send! ret=%x \n",ret);
	return ;

 nlmsg_failure:
 	printk("Fail to send netlink message.\n");
	if(skb)
		kfree_skb(skb);

	return;

}

#ifdef CONFIG_BUTTON_TRIGGER_TIMER
static void trigger_timer_function(unsigned long data)
{
	struct btn_dev *btn;

	list_for_each_entry(btn,&btn_list,node)
	{
		if( btn->get_status(btn) == BTN_DOWN)
		{
			btn_status_query(btn);  /* 进入状态机 */
		}

		//break;
	}

	trigger_timer.expires  = jiffies + msecs_to_jiffies(QUERY_TIMER_DEYLAY);
	add_timer( &trigger_timer);
}
#endif

/* 状态机处理函数 */
static void query_timer_function(unsigned long p_btn_cdev)
{
	struct btn_dev *btn = (struct btn_dev *)p_btn_cdev;

	if(current_btn_status == BTN_START)
	{
		if( btn->get_status(btn) == BTN_DOWN)
		{
			current_btn_status = BTN_DOWN;
			send_to_user(btn,BTN_DOWN);
			query_timer.expires  = jiffies + msecs_to_jiffies(QUERY_TIMER_DEYLAY);
			add_timer( &query_timer);
		}
		else  //BTN_UP
		{
			current_btn_status = BTN_UP;
			query_timer.expires  = jiffies + msecs_to_jiffies(QUERY_TIMER_DEYLAY);
			add_timer( &query_timer);
		}

		return;
	}


	if(current_btn_status == BTN_DOWN)
	{
		if(btn->get_status(btn) == BTN_UP)
		{
			current_btn_status = BTN_UP;
			query_timer.expires  = jiffies + msecs_to_jiffies(QUERY_TIMER_DEYLAY);
			add_timer( &query_timer);

		}
		else    //BTN_DOWN
		{
			query_timer.expires  = jiffies + msecs_to_jiffies(QUERY_TIMER_DEYLAY);
			add_timer( &query_timer);
		}


		return;

	}


	if(current_btn_status == BTN_UP)
	{
		if(btn->get_status(btn) == BTN_UP)
		{
			current_btn_status = BTN_START;
			btn_query_lock = 0; /* 释放锁，此时可以由其它按钮触发状态机 */
			send_to_user(btn,BTN_UP);
		}
 		else  //BTN_DOWN
		{
			current_btn_status = BTN_DOWN;
			query_timer.expires  = jiffies + msecs_to_jiffies(QUERY_TIMER_DEYLAY);
			add_timer( &query_timer);
		}

		return;
	}


}


/* 状态机触发函数，由此进入状态机 */
void btn_status_query(struct btn_dev *btn)
{
	if(btn_query_lock)   /* 锁定,每次只处理一个按钮 */
	{
		return;
	}
	btn_query_lock = 1;
	query_timer.expires  = jiffies + msecs_to_jiffies(QUERY_TIMER_DEYLAY);
	query_timer.data = (unsigned long)btn;
	current_btn_status = BTN_START;
	add_timer( &query_timer);
}
EXPORT_SYMBOL_GPL(btn_status_query);



/**
 * 按钮注册函数，将一个按钮注册到核心的链表中
 *
 */
int btn_dev_register(struct btn_dev *btn)
{

#ifdef CONFIG_BUTTON_TRIGGER_TIMER
	/* 将按钮加入到核心链表中 */
	write_lock(&btn_list_lock);
	list_add_tail(&btn->node, &btn_list);
	write_unlock(&btn_list_lock);
#endif

	/* 如果是中断方式，则不需要做任何处理 */
	printk(KERN_INFO "Registered button device: %d\n",btn->name);

	return 0;
}
EXPORT_SYMBOL_GPL(btn_dev_register);


void btn_dev_unregister(struct btn_dev *btn)
{
#ifdef CONFIG_BUTTON_TRIGGER_TIMER
	write_lock(&btn_list_lock);
	list_del(&btn->node);
	write_unlock(&btn_list_lock);
#endif
}
EXPORT_SYMBOL_GPL(btn_dev_unregister);

/**
 *  从 /proc/blp_cmm_pid 获得应用层监听进程的ID
 */

static int proc_blp_cmm_read(char *buf, char **start, off_t offset, int len, int *eof, void *data)
{
	int ret;
	ret = snprintf(buf,len,"%d",blp_cmm_pid);
	return ret;
}

ssize_t proc_blp_cmm_write( struct file *filp, const char __user *buf,unsigned long len, void *data )
{
	int ret;
	ret = sscanf(buf,"%d",&blp_cmm_pid);
	return len;
}



static int __init btn_init(void)
{

	struct proc_dir_entry *proc_blp_cmm;   /** /proc/blp_cmm_pid **/

	netlink_btn_sock = netlink_kernel_create(&init_net, NETLINK_TBS, 0, recv_handler, NULL, THIS_MODULE); //by XiaChaoRen,

	if (!netlink_btn_sock) {
        printk("Fail to create netlink socket.\n");
        return -1;
    }

/**  轮询方式下初始化定时器  **/
#ifdef CONFIG_BUTTON_TRIGGER_TIMER
	init_timer(&trigger_timer);
	trigger_timer.function = trigger_timer_function;
	trigger_timer.expires  = jiffies + msecs_to_jiffies(TRIGGER_TIMER_DEYLAY);
	trigger_timer.data = (unsigned long)NULL;
	add_timer( &trigger_timer);
#endif

	init_timer(&query_timer);
	query_timer.function = query_timer_function;


	/** 注册 /proc/blp_cmm_pid **/
	proc_blp_cmm = create_proc_entry( "blp_cmm_pid", 0644, NULL);
	proc_blp_cmm->read_proc  = proc_blp_cmm_read;
	proc_blp_cmm->write_proc = proc_blp_cmm_write;

	return 0;
}

static void __exit btn_exit(void)
{
	sock_release(netlink_btn_sock->sk_socket);
	remove_proc_entry("blp_cmm_pid",NULL);
}

module_init(btn_init);
module_exit(btn_exit);

MODULE_AUTHOR("Zhang Yu");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Button Core Interface");
