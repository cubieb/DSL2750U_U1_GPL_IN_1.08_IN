/* Linux: tbs port mirror */

#include <linux/module.h>
#include <asm/uaccess.h>
#include <asm/system.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/netdevice.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>


struct tbs_mirror_info {
	char mirrordev_name[IFNAMSIZ];    //be sent
	char wandev_name[IFNAMSIZ];	//be mirrored
	u32 direction_flag;	//out =0x01 in = 0x02 both = 0x03
};

enum mirror_dir {
	MIRROR_OUT = 1,   //wan out
	MIRROR_IN,      //wan in
	MIRROR_ALL	    //out & in
};	

#define HBUFFERLEN 50
static struct tbs_mirror_info *cur_mirror_info = NULL;

static void tbs_release_mirror_info(struct tbs_mirror_info *mirrorinfo)
{
	struct net_device *old_wandev;

	if(NULL == mirrorinfo)
		{
		return;
		}
	old_wandev = __dev_get_by_name(&init_net, mirrorinfo->wandev_name);
	if(NULL != old_wandev)
		{/*reset mirrored port*/
		write_lock(&dev_base_lock);	
		old_wandev->mirror_tx = NULL;
		if(NULL != old_wandev->mirror_dev)
			{
			dev_put(old_wandev->mirror_dev);
			old_wandev->mirror_dev = NULL;
			}
		old_wandev->direct_flags = 0;
		write_unlock(&dev_base_lock);
	}	
	kfree(mirrorinfo);
}

static void tbs_mirror_tx(struct sk_buff *skb, struct net_device *dev, u32 dir)
{
	struct sk_buff *nskb;
	unsigned int len;

	nskb= skb_clone(skb, GFP_ATOMIC);

	if(NULL != nskb)
		{
		if(dir & 0x2)
			{/*is input*//*in rx packets ,skb->data has been put ethernet header*/	
			skb_push(nskb, ETH_HLEN);
			}
		len = skb_headroom(nskb);
		if(len < ETH_HLEN)
			{
			len = ETH_HLEN - len;
			if(pskb_expand_head(nskb, (int)len, 0, GFP_ATOMIC))
				{
				kfree_skb(nskb);
				return;
				}
			}

		/*
		  *expand a new ethernet header, type is 0x6558 (RFC 1701)
		  * bug: nskb->len = skb->len + 14 +14, if skb->len=1500, nskb->len
		  * will more than 1518 (ethernet mtu) and the nskb won't be send
		  */
		skb_push(nskb, ETH_HLEN);
		memset(nskb->data, 0xFF, 12); /*set dst mac of packets to broadcast*/
		*(unsigned short *)(nskb->data + 12) = htons(0x6558);
		nskb->dev = dev;
		nskb->protocol = htons(ETH_P_802_3);
		dev->hard_start_xmit(nskb,dev);
		}
}

static int tbs_set_mirror_info(struct tbs_mirror_info *mirrorinfo)
{
	struct net_device *wandev, *mirrordev;
	struct tbs_mirror_info *tmp_mirror_info;

	/*check direction flags,if it's invalid return -EINVAL*/
	if((mirrorinfo->direction_flag > 3) || (mirrorinfo->direction_flag < 1))
		{
		return -EINVAL;
		}
	/*get mirrored dev,it is a wan device*/
	wandev =  __dev_get_by_name(&init_net, mirrorinfo->wandev_name);
	if(NULL == wandev)
		{
		return -ENODEV;
		}
	/*get mirror dev*/
	mirrordev =  __dev_get_by_name(&init_net, mirrorinfo->mirrordev_name);
	if(NULL == mirrordev)
		{
		return -ENODEV;
		}
		/*release the old mirror*/
	if(NULL != cur_mirror_info)
		{
		tmp_mirror_info = cur_mirror_info;
		tbs_release_mirror_info(tmp_mirror_info);
		}
	cur_mirror_info = mirrorinfo;
	
	/*set to mirror dev*/
	write_lock(&dev_base_lock);	
	wandev->mirror_dev = mirrordev;
	dev_hold(mirrordev);
	
	/*set tx funtion*/
	wandev->mirror_tx = tbs_mirror_tx;
	wandev->direct_flags = mirrorinfo->direction_flag;
	write_unlock(&dev_base_lock);		
	
	return 0;
}

static ssize_t tbs_wan_mirror_read(struct file *file, char __user *userbuf, size_t count, loff_t *off)
{
	char buffer[HBUFFERLEN] = {0};
	loff_t offs = *off;

	static int finishflag = 0;   /*is exit flag*/

	if(0 != finishflag)
		{
		finishflag = 0;
		return 0;
		}	
		
	if(count > PAGE_SIZE)
		{
		count = PAGE_SIZE;
		}
	sprintf(buffer,"#mirrordev\twandev\tdirection\n");

	if(cur_mirror_info)
		{
		sprintf(buffer, "%s %s\t%s\t%d\n", buffer, cur_mirror_info->mirrordev_name, cur_mirror_info->wandev_name, cur_mirror_info->direction_flag);
		}
	count = strlen(buffer);
	if(copy_to_user(userbuf, buffer, count))
		{
		return -EFAULT;
		}
	finishflag = 1;
	*off = offs + count;

	return count;
}

static ssize_t tbs_wan_mirror_write(struct file *file, const char __user *userbuf, size_t count, loff_t *off)
{
	struct tbs_mirror_info *tmp_info;
	char buffer[HBUFFERLEN] = {0};
	int ret;

	if(count > PAGE_SIZE)
		{
		count = PAGE_SIZE;
		}
	if (copy_from_user(buffer, userbuf, count))
		{
		return -EFAULT;
		}
	if(!strncmp(buffer, "disable", 7))
		{/*disable mirror function*/
		tbs_release_mirror_info(cur_mirror_info);
		cur_mirror_info = NULL;
		return count;
		}
	if(count < 9)
		{
		printk("input error!\n");
		return -EINVAL;
		}
	tmp_info = kzalloc(sizeof(struct tbs_mirror_info), GFP_KERNEL);
	if(!tmp_info)
		{
		return -ENOMEM;
		}
	memset(tmp_info, 0, sizeof(struct tbs_mirror_info));
	sscanf(buffer, "%s %s %u", tmp_info->mirrordev_name, tmp_info->wandev_name, &(tmp_info->direction_flag));	
	ret = tbs_set_mirror_info(tmp_info);
	if(ret < 0)
		goto fail;

	return count;

fail:
	kfree(tmp_info);
	return ret;
}

static struct file_operations tbs_wan_mirror_fops = {
	.owner       = THIS_MODULE,
	.read        = tbs_wan_mirror_read,
	.write       = tbs_wan_mirror_write,
};

static int __init tbs_mirror_proc_init(void)
{
	printk(KERN_INFO "TBS port mirror initialized ");

	if(!proc_net_fops_create(&init_net, "tbs_mirror", S_IRUGO, &tbs_wan_mirror_fops))
		{
		printk("failed!\n");
		return -ENOMEM;
		}
	else
		{
		printk("successfully!\n");
		return 0;
		}
}

static void __exit tbs_mirror_proc_exit(void)
{		
	tbs_release_mirror_info(cur_mirror_info);
	proc_net_remove(&init_net, "tbs_mirror");
	printk(KERN_INFO "tbs_mirror proc node removed successfully!\n");
}

module_init(tbs_mirror_proc_init);
module_exit(tbs_mirror_proc_exit);
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("TBS WAN Mirror Kernel Module");
MODULE_AUTHOR("XiaChaoRen");


