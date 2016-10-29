/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.

 文件名称: ip_conntrack_devctl.c

 文件描述: 多终端上网控制内核模块

 函数列表:


 修订记录:
           1 作者 : 匡素文
             日期 : 2009-03-26
             描述 : 创建

**********************************************************************/

#include <linux/types.h>
#include <linux/ctype.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/socket.h>
#include <linux/ctype.h>
#include <linux/errno.h>
#include <linux/timer.h>
#include <linux/time.h>
#include <linux/config.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/ip.h>
#include <linux/udp.h>
#include <linux/icmp.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>

#include <net/ip.h>
#include <net/sock.h>
#include <net/udp.h>
#include <net/tcp.h>
#include <net/icmp.h>
#include <net/route.h>

#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_tuple.h>
#include <net/netfilter/nf_nat_protocol.h>
#include <net/netfilter/nf_nat_core.h>
#include <linux/netfilter_ipv4/ip_conntrack_devctl.h>
#include <linux/tbs_msg.h>



//#define DEVCTL_DEBUG 1
#ifdef DEVCTL_DEBUG
#define DEBUGP(fmt, args...) \
{\
    if(s_stConfig.print)\
    {\
        printk("%4d %20s: "fmt,  __LINE__, __FUNCTION__, ## args);\
    }\
}

#define TRACE(fmt, args...)  \
{\
    if(s_stConfig.print)\
    {\
        printk(fmt, ## args);\
    }\
}
#else
#define DEBUGP(fmt, args...)
#define TRACE(fmt, args...)
#endif

/* 不限制ppp拨号上网的用户 */
//#define CONFIG_DONT_CONTROL_PPP         1


/* 宏定义 */
#define DC_SUCCESS                      0

#define TBSMSG_LEN(len)                 (sizeof(ST_MSG) + len)
#define SET_ULONG(p, value) \
    *( (unsigned long *)p ) = value; \
    p += sizeof(unsigned long);

/* 搜索结果消息 */
typedef struct tagST_DC_SearchResultMsg
{
    ST_MSGHEAD msghead;
    ulong result;
    ST_DC_SearchResult data;
}ST_DC_SearchResultMsg;


/* hash节点 */
typedef struct DC_HashNode
{
    struct DC_HashNode  *next;
    ST_DC_DeviceInfo    dev;
}ST_DC_HashNode;

/* hash表 */
typedef struct
{
    ST_DC_HashNode  **list;
    size_t          list_size;
    size_t          count;
    rwlock_t        lock;
}ST_DC_HashTab;


/******************************************************************************
*                   Function declare
******************************************************************************/

/* hash相关函数 */



/* 调试信息输出 */


/* 设备信息 检查/更新/查找 */
static int  dc_check_config(ST_DC_Config *config);

static ST_DC_DeviceInfo* dc_find_devinfo(EN_DC_FilterType type, char *value);
static int dc_copy_devinfo(EN_DC_FilterType type, char *value, int maxCnt, ST_DC_SearchResult *result);

static void dc_update_config(ST_DC_Config *pstConfig);
static inline void dc_update_devonline(ST_DC_DeviceInfo *pstOnline, ulong time);
static inline void dc_update_devinfo(ST_DC_DeviceInfo *pstDevInfo, ulong ipaddr, char *mac, EN_DC_DevType type);

/* NetLink消息收发 */
static void dc_recv_handler(struct sock * sk, int len);
static int  dc_send_handler(struct sock * sk, struct nlmsghdr *nlh_recv, void *data, int data_len);
static void dc_response_simple_ack(struct sock * sk, struct nlmsghdr *nlh_recv, ushort msg_type, int msg_error);

/* 检查网络数据包 */
static ST_DC_DeviceInfo *dc_get_devinfo(struct sk_buff **pskb);
static int dc_control_device_by_type(struct sk_buff **pskb, ST_DC_DeviceInfo *dev, int type);
static int dc_control_device(struct sk_buff **pskb);
static unsigned int dc_ip_devctl_out(unsigned int hooknum, struct sk_buff *skb,
	const struct net_device *in, const struct net_device *out,
	int (*okfn)(struct sk_buff *));


/* 模块加载/注销 */
static void __init dc_init_config(void);
static int  __init dc_init(void);
static void __exit dc_fini(void);



/******************************************************************************
*                   Global vaiables
******************************************************************************/
static ST_DC_Config             s_stConfig;

static ST_DC_HashTab            *s_pstDeviceTable = NULL;
static ST_DC_DevTypeStatus      s_astDeviceTypeStatus[DC_DevType_Max];

static rwlock_t                 s_devinfo_lock;
static struct sock*             s_netlink_sock = NULL;


/******************************************************************************
*                   Function
******************************************************************************/


#define __HASH_SEC__


/*
创建一个新的数据节点, 并复制传入的值
node->new
*/
static inline ST_DC_HashNode *dc_node_new(ulong ip, ST_DC_DeviceInfo *dev)
{
    ST_DC_HashNode *new_node = kmalloc(sizeof(ST_DC_HashNode),GFP_KERNEL);
    if (new_node == NULL)
    {
        return NULL;
    }

    new_node->next = NULL;
    memcpy(&(new_node->dev), dev, sizeof(ST_DC_DeviceInfo));

    return new_node;
}

/*
为节点设置新值
node->free
*/
static inline void dc_node_update(ST_DC_HashNode *node, ST_DC_DeviceInfo *dev)
{
    /* 这里覆盖原来的内容就可以 */
    memcpy(&(node->dev), dev, sizeof(ST_DC_DeviceInfo));
}


/*
释放某个数据节点的内存,并返回下个节点的指针
node->free
*/
static inline ST_DC_HashNode *dc_node_free(ST_DC_HashNode *node)
{
    ST_DC_HashNode *next = node->next;

    kfree(node);
    node = NULL;

    return (next);
}


/*
查找数据节点链表中的节点
hashlist->find
*/
static inline ST_DC_HashNode *dc_list_find(ST_DC_HashNode **head, ulong ip)
{
    ST_DC_HashNode *pos = NULL;

    /* 遍历节点链表, 依次比较 */
    for (pos = (*head); pos != NULL; pos = pos->next)
    {
        if (pos->dev.ipaddr == ip)
        {
            return pos;
        }
    }

    return NULL;
}


/*
在数据节点链表中添加节点
hashlist->append
*/
static inline ST_DC_HashNode *dc_list_append(ST_DC_HashNode **head, ulong ip, ST_DC_DeviceInfo *dev)
{
    ST_DC_HashNode *n = NULL;
    ST_DC_HashNode *pos = NULL;

    /* 新建一个节点 */
    n = dc_node_new(ip, dev);
    if (n == NULL)
    {
        return NULL;
    }

    /* 将节点附加到节点链表上去 */
    if (*head == NULL)
    {
        *head = n;
    }
    else
    {
        for(pos = *head; pos->next != NULL; pos = pos->next)
        {
            ;
        }
        pos->next = n;
    }

    return n;
}


/*
移除数据节点链表中某个节点
hashlist->remove
*/
static inline int dc_list_remove(ST_DC_HashNode **head, ulong ip)
{
    ST_DC_HashNode *pos = NULL;
    ST_DC_HashNode *prev = NULL;

    /* 节点链表为空,返回错误 */
    if (NULL == *head)
        return -1;

    /* 要删除的就是头结点 */
    if ((*head)->dev.ipaddr == ip)
    {
        *head = dc_node_free(*head);
        return 0;
    }

    /* 遍历链表查找要删除的节点 */
    prev = *head;
    for (pos = prev->next; NULL != pos; pos = pos->next)
    {
        if (pos->dev.ipaddr == ip)
        {
            prev->next = dc_node_free(pos);
            return 0;
        }
        prev = pos;
    }

    return -1;
}


/*
创建hash表
hashtab->create
*/
static ST_DC_HashTab *dc_hash_create(int size)
{
    int len = 0;

    /* 申请hash表结构自身的内存 */
    ST_DC_HashTab *tab = kmalloc(sizeof(ST_DC_HashTab), GFP_KERNEL);
    if (tab != NULL)
    {
        /* hash表清零 */
        memset(tab, 0, sizeof(ST_DC_HashTab));

        /* 申请hash表中的索引数组的内存, len = size * 4 */
        len = size * sizeof(ST_DC_HashNode *);
        tab->list = kmalloc(len, GFP_KERNEL);
        if (tab->list == NULL)
        {
    	    kfree(tab);
            return NULL;
        }

        /* 初始化hash表的成员 */
        tab->list_size   = size;
        tab->count  = 0;
        rwlock_init(&(tab->lock));  //初始化锁
        memset(tab->list, 0, len);
    }

    return tab;
}

/*
销毁hash表
hashtab->destory
*/
static void dc_hash_destory(ST_DC_HashTab *tab)
{
    int i;
    ST_DC_HashNode *pos = NULL;

    /* 遍历hash表的索引数组, 依次释放数据节点链表中的节点 */
    for (i = 0; i < tab->list_size; i++)
    {
        for (pos = tab->list[i]; pos != NULL; pos = dc_node_free(pos))
        {
            ;
        }
        tab->list[i] = NULL;
    }

    /* 释放索引数组 */
	kfree(tab->list);

    /* 释放自身 */
	kfree(tab);

    tab =NULL;
}


/*
计算hash值
hashtab->index
*/
static inline uint dc_hash_index(ST_DC_HashTab *tab, ulong ip)
{
    //return (HOST_ADDR(ip, (~0xff)) % (tab->list_size));
    /* 就是ip地址的最后一位 */
    return (HOST_ADDR(ip, (~0xff)));
}


/*
查找hash表中的数据
hashtab->get
*/
static inline ST_DC_DeviceInfo *dc_hash_get(ST_DC_HashTab *tab, ulong ip)
{
    ST_DC_HashNode *find = NULL;
    uint index = dc_hash_index(tab, ip);

    find = dc_list_find(&(tab->list[index]), ip);
    if (find != NULL)
    {
        return &find->dev;
    }

    return NULL;
}

/*
往hash表中添加键值对(确定此键值对不存在的情况下调用)
hashtab->add
*/
static inline ST_DC_DeviceInfo *dc_hash_add(ST_DC_HashTab *tab, ulong ip, ST_DC_DeviceInfo *dev)
{
    ST_DC_HashNode *node = NULL;
    uint index = dc_hash_index(tab, ip);

    /* hash节点数不能太多, count < list_size*4 */
    if (tab->count >= (tab->list_size*4))
        return NULL;

    node = dc_list_append(&(tab->list[index]), ip, dev);
    if (node == NULL)
    {
        return NULL;
    }
    tab->count++;

    return &(node->dev);
}


/*
往hash表中设置键值对(键不存在则添加,存在则更新值)
hashtab->set
*/
static inline ST_DC_DeviceInfo *dc_hash_set(ST_DC_HashTab *tab, ulong ip, ST_DC_DeviceInfo *dev)
{
    ST_DC_HashNode *find = NULL;
    uint index = dc_hash_index(tab, ip);

    /* 查找hash表中是否存在此键值对 */
    find = dc_list_find(&(tab->list[index]), ip);

    /* 不存在则添加新的键值对 */
    if (find == NULL)
    {
        return dc_hash_add(tab, ip, dev);
    }
    /* 已存在则更新键值 */
    else
    {
        dc_node_update(find, dev);
        return &find->dev;
    }
}

/*
往hash表中设置键值对(键不存在则添加,存在则更新值)
hashtab->remove
*/
static inline void dc_hash_remove(ST_DC_HashTab *tab, ulong ip)
{
    uint index = dc_hash_index(tab, ip);
    if (0 == dc_list_remove(&(tab->list[index]), ip))
    {
        tab->count--;
    }
}

/*
获取hash表中的元素个数
hashtab->count
*/
static inline unsigned int dc_hash_count(ST_DC_HashTab *tab)
{
    return tab->count;
}

/*
遍历hash表
hashtab->foreach
*/
#define dc_hash_foreach(tab, pos, index)\
    for (index = 0; index < tab->list_size; index++)\
        for (pos = tab->list[index]; NULL != pos; pos = pos->next)

#define __DEBUG_SEC__

#ifdef DEVCTL_DEBUG


/*
打印16进制数据
*/
static void dc_print_hex(const void *buf, int len)
{
    const int LINE_WIDTH = 16;
    int i = 0, j = 0;
    int tail = 0;
    const unsigned char *pucBuf = NULL;

    if (len == 0 || NULL == buf)
    {
        return;
    }

    pucBuf = buf;

    for(; i<len; i++)
    {
        if(i>0 && i%LINE_WIDTH == 0)
        {
            j = i - LINE_WIDTH;
            TRACE("; ");
            for(; j<i; j++)
            {
                if(pucBuf[j] < 16)
                {
                    TRACE("%c", '.');
                }
                else
                {
                    TRACE("%c", pucBuf[j]);
                }
            }

            TRACE("\n");
        }

        TRACE("%02X ", pucBuf[i]);
    }

    tail = len%LINE_WIDTH == 0 ? len-LINE_WIDTH:(len/LINE_WIDTH)*LINE_WIDTH;
    if(tail != len-LINE_WIDTH)
    {
        for(i=0; i<48-(len-tail)*3; i++)
        {
            TRACE("%c", ' ');
        }
    }

    TRACE("; ");

    for(i=tail; i<len; i++)
    {
        if(pucBuf[i] < 16)
        {
            TRACE("%c", '.');
        }
        else
        {
            TRACE("%c", pucBuf[i]);
        }
    }

    TRACE("\n");
}

/*
调试辅助函数，将数值形式MsgType转换为字符串可读形式。
*/
const char* tbsGetMsgTypeName(unsigned short usMsgType)
{
	#define MSG_TYPE_NAME(name) case name: return #name

	switch (usMsgType)
    {
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_ADD);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_ADD_ACK);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_DEL);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_DEL_ACK);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_QUERY);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_QUERY_ACK);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_CONFIG);
        MSG_TYPE_NAME(MSG_BLP_DEVCTL_CONFIG_ACK);

     	default:
		{
			static char s_szUnsupported[64];
			sprintf(s_szUnsupported, "UNKNOWN_MSGTYPE(%lu)", (unsigned long)usMsgType);
			return s_szUnsupported;
		}
	}
}


static inline void dc_print_nlmsghdr(struct nlmsghdr *nlh)
{
    TRACE("nlmsghdr:\n");
    TRACE("len:%d, type:0x%04x, flags:0x%04x, seq:%d, pid:%d\n",
        nlh->nlmsg_len, nlh->nlmsg_type,
        nlh->nlmsg_flags, nlh->nlmsg_seq, nlh->nlmsg_pid);
}

static inline void dc_print_nlmsg(struct nlmsghdr *nlh)
{
    dc_print_nlmsghdr(nlh);
    dc_print_hex(NLMSG_DATA(nlh), nlh->nlmsg_len-NLMSG_HDRLEN);
}

static inline void dc_print_tbsmsg(const ST_MSG *pstMsg)
{
    TRACE("MsgHead:\n");
    TRACE("SrcMID:0x%04x,DstMID:0x%04x, MsgID:0x%08lx, MsgType:%s, 0x%04x BodyLength:%lu\n",
           pstMsg->stMsgHead.usSrcMID,
           pstMsg->stMsgHead.usDstMID,
           pstMsg->stMsgHead.ulMsgID,
           tbsGetMsgTypeName(pstMsg->stMsgHead.usMsgType),
           pstMsg->stMsgHead.ulBodyLength);
    TRACE("MsgBody:\n");
    dc_print_hex(pstMsg->szMsgBody, pstMsg->stMsgHead.ulBodyLength);
}

static inline void dc_show_config(void)
{
    int i = 0;
    TRACE("\n==============================Config Info======================\n");
    TRACE("Kernel Module Config:\n");
    TRACE("mode=%d\n", s_stConfig.mode);
	TRACE("maxTotalNumber=%d\n", s_stConfig.maxTotalNumber);
    TRACE("lanIP="NIPQUAD_FMT"\n", NIPQUAD(s_stConfig.lanIP));
    TRACE("lanMask="NIPQUAD_FMT"\n", NIPQUAD(s_stConfig.lanMask));
	TRACE("pppBeginIP="NIPQUAD_FMT"\n", NIPQUAD(s_stConfig.pppBeginIP));
	TRACE("pppEndIP="NIPQUAD_FMT"\n", NIPQUAD(s_stConfig.pppEndIP));
	TRACE("timeout=%u\n", s_stConfig.timeout);
	TRACE("print=%u\n", s_stConfig.print);
    if (s_pstDeviceTable != NULL)
        TRACE("Total terminals count:%d\n", dc_hash_count(s_pstDeviceTable));
    for(i=0; i<DC_DevType_Max; i++)
    {
        TRACE("type=%d enable=%d maxNumber=%d currentNumber=%d\n",
            i,
    	    s_stConfig.devTypeSettings[i].enable,
            s_stConfig.devTypeSettings[i].maxNumber,
            s_astDeviceTypeStatus[i].currentNumber);
    }
    TRACE("==============================End  Info========================\n");
}


static inline void dc_show_devinfo(void)
{
    int i = 0;
    ST_DC_HashNode *pos = NULL;

    TRACE("\n==============================Device Info======================\n");
    TRACE("Current register device list:\n");

    /* 加锁 */
    read_lock_bh(&s_devinfo_lock);
    TRACE("Total terminals count:%d\n", dc_hash_count(s_pstDeviceTable));
    dc_hash_foreach(s_pstDeviceTable, pos, i)
    {
        //if (pos->dev.time != 0)
        {
            TRACE("ip="NIPQUAD_FMT", mac="NMACQUAD_FMT", type=%d, time=%lu\n",
                NIPQUAD(pos->dev.ipaddr),
                NMACQUAD(pos->dev.mac),
                pos->dev.type, pos->dev.time);
        }
    }
    /* 解锁 */
    read_unlock_bh(&s_devinfo_lock);

    TRACE("==============================End Info=========================\n");
}


#else

#define dc_print_hex(buf, len)
#define dc_print_nlmsghdr(nlh)
#define dc_print_nlmsg(nlh)
#define dc_print_tbsmsg(nlh)
#define dc_show_config()
#define dc_show_onlineinfo()
#define dc_show_devinfo()


#endif



#define __DATA_SEC__

/* 检验子网掩码 */
static int dc_check_mask(ulong mask)
{
    /* 不能全为0 */
    if (mask == 0)
        return 0;

    while ( mask & 0x80000000 )
	{
		mask <<= 1;
        DEBUGP("mask continue:"NIPQUAD_FMT",%0lx\n", NIPQUAD(mask), mask);
	}

	mask &= 0xffffffff;

	if ( mask == 0 )
	{
		return 1;
	}
	else
	{
		return 0;
	}
}


/* 校验配置信息 */
static int dc_check_config(ST_DC_Config *config)
{
    int iRet = DC_SUCCESS;
    int i = 0;

    /* 检查限制模式 */
    if (config->mode != 0
        && config->mode != 1
        && config->mode != 2
    )
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("mode is invalid.\n");
        goto out;
    }

    /* 检查限制总数 */
    if ( config->mode > 0
        && (config->maxTotalNumber < 0 || config->maxTotalNumber > 254)
    )
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("maxTotalNumber(%d) is invalid.\n", config->maxTotalNumber);
        goto out;
    }

    /* 检查lanMask */
    if (!dc_check_mask(ntohl(config->lanMask)))
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("lanmask("NIPQUAD_FMT") is invalid.\n",
                NIPQUAD(config->lanMask));
        goto out;
    }

    /* 检查lanIP,不能为广播地址或者网络地址 */
    if (IS_NET_ADDR(config->lanIP, config->lanMask)
        || IS_BROAD_ADDR(config->lanIP, config->lanMask)
    )
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("lanIP("NIPQUAD_FMT")/lanmask("NIPQUAD_FMT") is invalid.\n",
                NIPQUAD(config->lanIP), NIPQUAD(config->lanMask));
        goto out;
    }

    /* 检查pppoe地址段, 要在lan网段中 */
    if ( !IN_SAME_SUBNET(config->lanIP, config->pppBeginIP, config->lanMask)
        || !IN_SAME_SUBNET(config->lanIP, config->pppEndIP, config->lanMask)
        || (config->pppBeginIP >= config->pppEndIP)
    )
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("pppBeginIP("NIPQUAD_FMT")/pppEndIP("NIPQUAD_FMT") is invalid.\n",
            NIPQUAD(config->pppBeginIP), NIPQUAD(config->pppEndIP));
        goto out;
    }

    /* 检查超时 */
    if (config->timeout <= 0)
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("timeout is invalid.\n");
        goto out;
    }

    /* 检查各类型限制数量 */
    for (i=1; i<DC_DevType_Max; i++)
    {
        if( config->devTypeSettings[i].enable
            && (config->devTypeSettings[i].maxNumber < 0 || config->devTypeSettings[i].maxNumber > 254)
        )
        {
            iRet = ERR_DEVCTL_INVALID_CONFIG;
            DEBUGP("specify devtype maxNumber is invalid.\n");
            break;
        }
    }

out:
    return iRet;
}


/*
查找设备信息
*/
static ST_DC_DeviceInfo* dc_find_devinfo(EN_DC_FilterType type, char *value)
{
    ST_DC_DeviceInfo *devInfo = NULL;
    ST_DC_HashNode *pos = NULL;
    int i = 0;

    /* 根据IP查找 */
    if (type == DC_FT_ByIP)
    {
        devInfo = dc_hash_get(s_pstDeviceTable, *(ulong *)value);
    }
    /* 根据MAC查找 */
    else if (type == DC_FT_ByMac)
    {
        dc_hash_foreach(s_pstDeviceTable, pos, i)
        {
            if (memcmp(pos->dev.mac, value, DEV_MAC_LEN) == 0)
        	{
                devInfo = &(pos->dev);
                break;
        	}
        }
    }
    /* 根据设备类型查找 */

    return devInfo;
}



/*
接口: 根据mac地址或者ip地址，获取对应终端的设备类型
*/
EN_DC_DevType dc_get_devtype(EN_DC_FilterType type, char *value)
{
    ST_DC_DeviceInfo *devInfo = NULL;

    /* 加锁 */
    read_lock_bh(&s_devinfo_lock);

    /* 查找 */
    devInfo = dc_find_devinfo(type, value);

    /* 解锁 */
    read_unlock_bh(&s_devinfo_lock);

    return (devInfo == NULL)? DC_DevType_None : devInfo->type;
}
EXPORT_SYMBOL_GPL(dc_get_devtype);


/*
查找并复制设备信息
*/
static int dc_copy_devinfo(EN_DC_FilterType type, char *value, int maxCnt,
    ST_DC_SearchResult *result)
{
    ST_DC_DeviceInfo *devInfo = NULL;
    ST_DC_HashNode *pos = NULL;
    int i = 0;

    /* 初始化 */
    result->count = 0;

    /* 根据IP查找 */
    if (type == DC_FT_ByIP)
    {
        devInfo = dc_hash_get(s_pstDeviceTable, *(ulong *)value);
        if (devInfo != NULL)
        {
            result->count = 1;
            memcpy(result->devlist, devInfo, sizeof(ST_DC_DeviceInfo));
            return 1;
        }
    }
    /* 根据MAC查找 */
    else if (type == DC_FT_ByMac)
    {
        dc_hash_foreach(s_pstDeviceTable, pos, i)
        {
            if (memcmp(pos->dev.mac, value, DEV_MAC_LEN) == 0)
        	{
                devInfo = &(pos->dev);

                memcpy(&result->devlist[result->count], devInfo, sizeof(ST_DC_DeviceInfo));
                result->count = 1;
                return 1;
        	}
        }
    }
    /* 根据设备类型查找 */
    else
    {
        dc_hash_foreach(s_pstDeviceTable, pos, i)
        {
            if (pos->dev.type == *((EN_DC_DevType *)value))
        	{
                devInfo = &(pos->dev);

                /* 最多允许maxCnt个结果 */
                if (result->count >= maxCnt)
                    break;

                memcpy(&result->devlist[result->count], devInfo, sizeof(ST_DC_DeviceInfo));
                result->count++;
        	}
        }
    }

    return result->count;
}

/*
更新模块配置信息
*/
static inline void dc_update_config(ST_DC_Config *pstConfig)
{
    /* 覆盖配置信息 */
    memcpy(&s_stConfig, pstConfig, sizeof(ST_DC_Config));
}


/*
更新终端在线信息
*/
static inline void dc_update_devonline(ST_DC_DeviceInfo *pstOnline, ulong time)
{
    pstOnline->time = time;
}

/*
更新终端类型信息
*/
static inline void dc_update_devinfo(ST_DC_DeviceInfo *pstDevInfo,
    ulong ipaddr, char *mac, EN_DC_DevType type)
{
    pstDevInfo->ipaddr = ipaddr;
    memcpy(pstDevInfo->mac, mac, DEV_MAC_LEN);
    pstDevInfo->type = type;
}


#define __API_SEC__

/* 更新设备信息 */
static void on_msg_dev_add(struct sock *sk, struct nlmsghdr *nlh_recv, ST_MSG *pstMsg)
{
    int iRet = DC_SUCCESS;
    ST_DC_DeviceInfo *devInfo = (ST_DC_DeviceInfo *)pstMsg->szMsgBody;
    ST_DC_DeviceInfo *oldDevInfo = NULL;

    TRACE("\n============================Add Device=========================\n");
    TRACE("ip="NIPQUAD_FMT", mac="NMACQUAD_FMT", type=%d\n",
        NIPQUAD(devInfo->ipaddr), NMACQUAD(devInfo->mac), devInfo->type);
    TRACE("\n============================End Add============================\n");

    /* 检查消息中参数合法性 */
    if (!IN_SAME_SUBNET(devInfo->ipaddr, s_stConfig.lanIP, s_stConfig.lanMask))
    {
        iRet = ERR_DEVCTL_INVALID_IP;
        DEBUGP("the ip("NIPQUAD_FMT") is not in address pool\n",
            NIPQUAD(devInfo->ipaddr));
        goto out;
    }
    if (IS_NET_ADDR(devInfo->ipaddr, s_stConfig.lanMask)
        || IS_BROAD_ADDR(devInfo->ipaddr, s_stConfig.lanMask))
    {
        iRet = ERR_DEVCTL_INVALID_IP;
        DEBUGP("the ip("NIPQUAD_FMT") is net address or broadcast address\n",
            NIPQUAD(devInfo->ipaddr));
        goto out;
    }
    if (devInfo->type <= DC_DevType_None || devInfo->type >= DC_DevType_Max)
    {
        iRet = ERR_DEVCTL_INVALID_DEVTYPE;
        DEBUGP("the devtype is invalid.\n");
        goto out;
    }

    /* 加锁 */
    write_lock_bh(&s_devinfo_lock);

    /* 查询此终端信息是否注册 */
    oldDevInfo = dc_hash_get(s_pstDeviceTable, devInfo->ipaddr);

    /* 如果当前终端已经注册了并且在线，检测终端信息是否有变化，有则更新终端信息,并将终端下线 */
    if (oldDevInfo != NULL)
    {
        /* 终端信息有变化，需要更新终端信息 */
        if (oldDevInfo->type != devInfo->type || memcpy(oldDevInfo->mac ,devInfo->mac, DEV_MAC_LEN) != 0)
        {
            /* 更新终端信息 */
            dc_update_devinfo(oldDevInfo, devInfo->ipaddr, devInfo->mac, devInfo->type);

            /* 如果终端在线, 还要将终端设置下线 */
            if (oldDevInfo->time != 0)
            {
                /* 终端数减一 */
                (s_astDeviceTypeStatus[(s_stConfig.mode == 1) ? 0 : oldDevInfo->type].currentNumber)--;

                /* 将终端下线 */
                dc_update_devonline(oldDevInfo, 0);
            }
        }
        /* 终端信息没有变化则不做任何操作，防止终端续约时被抢占上网权限 */
        else
        {
            //nothing to do
        }
    }
    /* 终端信息尚未注册, 则将新终端信息添加到表中, 默认设置为下线 */
    else
    {
        oldDevInfo = dc_hash_add(s_pstDeviceTable, devInfo->ipaddr, devInfo);
        if (oldDevInfo == NULL)
        {
            /* 解锁 */
            write_unlock_bh(&s_devinfo_lock);

            iRet = ERR_DEVCTL_LIST_FULL;
            DEBUGP("the device list is full, can not store more device\n");
            goto out;
        }
        dc_update_devonline(oldDevInfo, 0);
    }

    /* 解锁 */
    write_unlock_bh(&s_devinfo_lock);

    //dc_show_devinfo();
out:
    dc_response_simple_ack(sk, nlh_recv,
        TBS_MSG_ACK_TYPE(pstMsg->stMsgHead.usMsgType), iRet);
}


/* 删除设备信息 */
static void on_msg_dev_del(struct sock * sk, struct nlmsghdr *nlh_recv, ST_MSG *pstMsg)
{
    int iRet = DC_SUCCESS;
    ST_DC_SearchFilter *searchFilter = (ST_DC_SearchFilter *)pstMsg->szMsgBody;
    ST_DC_DeviceInfo *oldDevInfo = NULL;

    /* 检查消息中参数合法性 */
    if (searchFilter->type != DC_FT_ByIP && searchFilter->type != DC_FT_ByMac)
    {
        iRet = ERR_DEVCTL_INVALID_PARA;
        DEBUGP("the search type is not support.\n");
        goto out;
    }
    if (searchFilter->type == DC_FT_ByIP
        && !IN_SAME_SUBNET(*(ulong *)(searchFilter->value), s_stConfig.lanIP, s_stConfig.lanMask))
    {
        iRet = ERR_DEVCTL_INVALID_IP;
        DEBUGP("the delete ip is not in address pool.\n");
        goto out;
    }

    /* 加锁 */
    write_lock_bh(&s_devinfo_lock);

    /* 查找对应的设备信息结构 */
    oldDevInfo = dc_find_devinfo(searchFilter->type, searchFilter->value);
    if (oldDevInfo == NULL)
    {
        /* 解锁 */
        write_unlock_bh(&s_devinfo_lock);

        iRet = ERR_DEVCTL_NOT_FOUND;
        DEBUGP("found devce failed.\n");
        goto out;
    }
    /* 找到终端信息, 从表中删除 */
    else
    {
        /* 终端在线 */
        if (oldDevInfo->time != 0)
        {
            /* 终端数减一 */
            (s_astDeviceTypeStatus[(s_stConfig.mode == 1) ? 0 : oldDevInfo->type].currentNumber)--;
        }

        /* 删除终端信息 */
        dc_hash_remove(s_pstDeviceTable, oldDevInfo->ipaddr);
    }

    /* 解锁 */
    write_unlock_bh(&s_devinfo_lock);

    //dc_show_devinfo();
out:
    dc_response_simple_ack(sk, nlh_recv,
        TBS_MSG_ACK_TYPE(pstMsg->stMsgHead.usMsgType), iRet);
}


/* 查询设备信息 */
static void on_msg_dev_query(struct sock * sk, struct nlmsghdr *nlh_recv, ST_MSG *pstMsg)
{
    int iRet = DC_SUCCESS;
    ST_DC_SearchFilter *searchFilter = (ST_DC_SearchFilter *)pstMsg->szMsgBody;

    ST_DC_SearchResultMsg *sendMsg = NULL;
    uint maxCount = 0;
    uint cnt = 0;

    /* 检查消息中参数合法性 */
    if (searchFilter->type != DC_FT_ByIP
        && searchFilter->type != DC_FT_ByMac
        && searchFilter->type != DC_FT_ByDevType)
    {
        iRet = ERR_DEVCTL_INVALID_PARA;
        DEBUGP("the search type is not support.\n");
        goto out;
    }
    if (searchFilter->type == DC_FT_ByIP
        && !IN_SAME_SUBNET(*(ulong *)(searchFilter->value), s_stConfig.lanIP, s_stConfig.lanMask))
    {
        iRet = ERR_DEVCTL_INVALID_IP;
        DEBUGP("the query ip is not in address pool.\n");
        goto out;
    }
    if (searchFilter->type == DC_FT_ByDevType
        && ( *(EN_DC_DevType*)(searchFilter->value) <= DC_DevType_None
            || *(EN_DC_DevType*)(searchFilter->value) >= DC_DevType_Max)
    )
    {
        iRet = ERR_DEVCTL_INVALID_DEVTYPE;
        DEBUGP("the query devtype is invalid.\n");
        goto out;
    }

    //dc_show_config();
    //dc_show_devinfo();

    /* 申请内存 */
    maxCount = dc_hash_count(s_pstDeviceTable);
    sendMsg = kmalloc(
        sizeof(ST_DC_SearchResultMsg) + sizeof(ST_DC_DeviceInfo) * maxCount,
        GFP_KERNEL);
    if (sendMsg == NULL)
    {
        iRet = ERR_DEVCTL_MEM_OUT;
        DEBUGP("kmalloc failed.\n");
        goto out;
    }
    memset(sendMsg, 0,
        sizeof(ST_DC_SearchResultMsg) + sizeof(ST_DC_DeviceInfo) * maxCount);

     /* 加锁 */
    read_lock_bh(&s_devinfo_lock);

    /* 提取符合条件的设备信息 */
    cnt = dc_copy_devinfo(searchFilter->type, searchFilter->value,
        maxCount, &(sendMsg->data));

    /* 解锁 */
    read_unlock_bh(&s_devinfo_lock);

    sendMsg->msghead.usMsgType = TBS_MSG_ACK_TYPE(pstMsg->stMsgHead.usMsgType);
    sendMsg->msghead.ulBodyLength = sizeof(ST_DC_SearchResultMsg) + sizeof(ST_DC_DeviceInfo) * sendMsg->data.count;
    sendMsg->result = DC_SUCCESS;
out:
    if (iRet != DC_SUCCESS)
    {
        dc_response_simple_ack(sk, nlh_recv,
            TBS_MSG_ACK_TYPE(pstMsg->stMsgHead.usMsgType), iRet);
    }
    else
    {
        dc_send_handler(sk, nlh_recv, sendMsg, TBSMSG_LEN(sendMsg->msghead.ulBodyLength));
    }

    if (sendMsg != NULL)
    {
        kfree(sendMsg);
    }
}


/* 配置模块参数 */
static void on_msg_dev_config(struct sock * sk, struct nlmsghdr *nlh_recv, ST_MSG *pstMsg)
{
    int iRet = DC_SUCCESS;
    int i = 0;
    ST_DC_Config *newConfig = (ST_DC_Config *)pstMsg->szMsgBody;
    ST_DC_HashNode *pos = NULL;

    /* 检查消息中参数合法性 */
    if (pstMsg->stMsgHead.ulBodyLength != sizeof(ST_DC_Config))
    {
        iRet = ERR_DEVCTL_INVALID_CONFIG;
        DEBUGP("the config info is invalid in msg.\n");
        goto out;
    }

    iRet = dc_check_config(newConfig);
    if (iRet != DC_SUCCESS)
    {
        goto out;
    }

    /* 显示原来的配置信息 */
    dc_show_config();

    /* 加锁 */
    write_lock_bh(&s_devinfo_lock);

    /* 更新配置信息 */
    dc_update_config(newConfig);

    /* 清空所有在线信息 */
    dc_hash_foreach(s_pstDeviceTable, pos, i)
    {
        dc_update_devonline(&(pos->dev), 0);
    }

    /* 清空各类型终端统计信息 */
    memset(&s_astDeviceTypeStatus, 0, sizeof(ST_DC_DevTypeStatus) * DC_DevType_Max);

    /* 解锁 */
    write_unlock_bh(&s_devinfo_lock);

    /* 显示新的的配置信息 */
    dc_show_config();

out:
    dc_response_simple_ack(sk, nlh_recv,
        TBS_MSG_ACK_TYPE(pstMsg->stMsgHead.usMsgType), iRet);
}



#define __NETLINK_SEC__

/*初始化信号量*/
DECLARE_MUTEX(s_recv_sem);

/* 处理应用层的通过netlink发送的请求消息 */
static void dc_recv_handler(struct sock * sk, int len)
{
    struct sk_buff *skb;
    struct nlmsghdr *nlh = NULL;
    ST_MSG *pstMsg = NULL;
    ulong data_size = 0;

    /*获取信号量*/
    if(down_trylock(&s_recv_sem))
        return;

    while ((skb = skb_dequeue(&sk->sk_receive_queue))!= NULL)
    {
        do
        {
            /* 检查netlink消息格式 */
            nlh = (struct nlmsghdr *)skb->data;
            if ( !NLMSG_OK(nlh, skb->len) )
            {
                DEBUGP("the length of netlink msg is wrong!\n");
                continue;
            }

            data_size = nlh->nlmsg_len - NLMSG_HDRLEN;
            pstMsg = (ST_MSG *)NLMSG_DATA(nlh);

            //TRACE("\n============================Netlink Msg========================\n");
            //dc_print_nlmsghdr(nlh);
            //dc_print_tbsmsg(pstMsg);
            //TRACE("\n============================End Message========================\n");

            /* 检查tbs消息格式 */
            if (data_size < sizeof(ST_MSGHEAD)
                || pstMsg->stMsgHead.ulBodyLength > (data_size-sizeof(ST_MSGHEAD))
            )
            {
                DEBUGP("the length of tbs msg is too small.\n");
                continue;
            }

            /* 处理tbs消息 */
            switch( pstMsg->stMsgHead.usMsgType )
            {
                case MSG_BLP_DEVCTL_ADD:
                    on_msg_dev_add(sk, nlh, pstMsg);
                    break;
                case MSG_BLP_DEVCTL_DEL:
                    on_msg_dev_del(sk, nlh, pstMsg);
                    break;
                case MSG_BLP_DEVCTL_QUERY:
                    on_msg_dev_query(sk, nlh, pstMsg);
                    break;
                case MSG_BLP_DEVCTL_CONFIG:
                    on_msg_dev_config(sk, nlh, pstMsg);
                    break;
                default:
                    DEBUGP("invalid msgtype.\n");
                    break;
            }

        } while(0);

        /* 释放skb */
        kfree_skb(skb);
    }

    /*释放信号量*/
    up(&s_recv_sem);
}


/* 通过netlink方式发送消息到应用层 */
static int dc_send_handler(struct sock * sk, struct nlmsghdr *nlh_recv,
    void *data, int data_len)
{
	int ret;
	unsigned char *old_tail = NULL;
	struct sk_buff *skb = NULL;
	struct nlmsghdr *nlh_send = NULL;

     /* 开辟一个新的套接字缓存 */
	skb = alloc_skb(NLMSG_SPACE(data_len), GFP_ATOMIC);
	old_tail = skb->tail;

	/* 填写netlink消息头 */
	nlh_send = NLMSG_PUT(skb, 0,
	    nlh_recv->nlmsg_seq, nlh_recv->nlmsg_type, data_len);

    /* 填写netlink消息体 */
	memcpy(NLMSG_DATA(nlh_send), data, data_len);

	/* 计算经过字节对齐后的数据实际长度 */
	nlh_send->nlmsg_len = skb->tail - old_tail;
    NETLINK_CB(skb).pid = 0;
    //NETLINK_CB(skb).dst_pid = nlh_recv->nlmsg_pid;
    NETLINK_CB(skb).dst_group = 0;

	/* 发送数据 */
	ret = netlink_unicast(sk, skb, nlh_recv->nlmsg_pid, MSG_DONTWAIT);
    if (ret < 0)
    {
        DEBUGP("send msg to app falied, error=%d!\n", ret);
        goto nlmsg_failure;
    }

	//DEBUGP("message send! ret=%d,data_len=%d\n,",ret, data_len);
	return 0;

 nlmsg_failure:
 	DEBUGP("Fail to send netlink message.\n");
	if(skb != NULL)
		kfree_skb(skb);

	return -1;
}


/* 回复简单消息 */
static void dc_response_simple_ack(struct sock * sk, struct nlmsghdr *nlh_recv,
    ushort msg_type, int msg_error)
{
    char buff[sizeof(ST_MSG)+sizeof(ulong)] = {0};
    ST_MSG *pstMsg = NULL;
    char *pcPos = NULL;

    pstMsg = (ST_MSG *)buff;
    pstMsg->stMsgHead.usMsgType = msg_type;
    pstMsg->stMsgHead.ulBodyLength = sizeof(ulong);

    pcPos = pstMsg->szMsgBody;
    SET_ULONG(pcPos, msg_error);

    dc_send_handler(sk, nlh_recv, pstMsg, TBSMSG_LEN(pstMsg->stMsgHead.ulBodyLength));
}


#define __NET_SEC__

/*
钩子函数
*/
static unsigned int dc_ip_devctl_out(unsigned int hooknum,
	struct sk_buff *skb,
	const struct net_device *in,
	const struct net_device *out,
	int (*okfn)(struct sk_buff *))
{
	int ret = NF_ACCEPT;

    //DEBUGP("mac: "NMACQUAD_FMT "->" NMACQUAD_FMT ", ip: " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
    //    NMACQUAD((*pskb)->mac.ethernet->h_source), NMACQUAD((*pskb)->mac.ethernet->h_dest),
    //    NIPQUAD((*pskb)->nh.iph->saddr), NIPQUAD((*pskb)->nh.iph->daddr));

    /* 模式为0，则不进行任何限制 */
    if (s_stConfig.mode == 0)
    {
        //DEBUGP("Accept: mode equal 0, no limit.\n ");
        return NF_ACCEPT;
    }

    /* 其他模式，需要进行终端数量限制 */
    else
    {
    	/* 源地址在DHCP地址池内,即来自LAN端的数据,?不支持多个lan网段?桥模式控制 */
    	if (IN_SAME_SUBNET(ip_hdr(skb)->saddr, s_stConfig.lanIP, s_stConfig.lanMask))
    	{
            /* 自身发的包允许通过 */
            if (ip_hdr(skb)->saddr == s_stConfig.lanIP)
            {
                return NF_ACCEPT;
            }

            /*
             通过ppp代理拨号上网的终端不限制
            */
            #ifdef CONFIG_DONT_CONTROL_PPP
            /* 源地址在DHCP地址池内，并且也在PPP的地址池内 */
            if ( ntohl(ip_hdr(skb)->saddr) >= ntohl(s_stConfig.pppBeginIP)
                && ntohl(ip_hdr(skb)->saddr) <= ntohl(s_stConfig.pppEndIP) )
            {
                return NF_ACCEPT;
            }
            #endif

            /* 目的地址在DHCP地址池内，即访问内网 */
        	if (IN_SAME_SUBNET(ip_hdr(skb)->daddr, s_stConfig.lanIP, s_stConfig.lanMask))
        	{
                DEBUGP("Accept: destip is in address pool, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                    NIPQUAD(ip_hdr(skb)->saddr),
                    NIPQUAD(ip_hdr(skb)->daddr));
        		return NF_ACCEPT;
        	}
            /* 目的地址在DHCP地址池外，即访问外网,需要进行限制 */
            else
            {
                /* 组播地址/E类地址/广播地址允许通过, >223.255.255.255 ?如果是子网广播地址怎么处理*/
                if (ntohl(ip_hdr(skb)->daddr) > 0xdfffffff)
                {
                    DEBUGP("Accept: destip is in multicast or broadcast address, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                        NIPQUAD(ip_hdr(skb)->saddr),
                        NIPQUAD(ip_hdr(skb)->daddr));
                    return NF_ACCEPT;
                }

                write_lock_bh(&s_devinfo_lock);
        	    ret = dc_control_device(&skb);
                write_unlock_bh(&s_devinfo_lock);
            }
    	}
        /* WAN端数据/LAN端桥接PPPoE拨号之后数据, 直接允许通过 */
        else
        {
            DEBUGP("Accept: srcip is out of address pool, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                NIPQUAD(ip_hdr(skb)->saddr),
                NIPQUAD(ip_hdr(skb)->daddr));
    		return NF_ACCEPT;
        }
    }

	return ret;
}

/*
获取终端类型
*/
static ST_DC_DeviceInfo *dc_get_devinfo(struct sk_buff **pskb)
{
    ST_DC_DeviceInfo *pstFindDev = dc_hash_get(s_pstDeviceTable, ip_hdr(*pskb)->saddr);

    if (pstFindDev != NULL)
    {
        /* 在终端信息表中找到此终端信息，返回终端类型，并记录IP和MAC地址 */
        dc_update_devinfo(pstFindDev,
            ip_hdr(*pskb)->saddr, eth_hdr(*pskb)->h_source, pstFindDev->type);
    }
    else
    {
        /* 在终端信息表中找不到此终端信息，则默认为Computer类型,记录IP和MAC地址 */
        ST_DC_DeviceInfo stNewDev;
        dc_update_devonline(&stNewDev, 0);
        dc_update_devinfo(&stNewDev,
            ip_hdr(*pskb)->saddr, eth_hdr(*pskb)->h_source, DC_DevType_Computer);

        pstFindDev = dc_hash_add(s_pstDeviceTable, stNewDev.ipaddr, &stNewDev);
    }

    return pstFindDev;
}


static int dc_control_device(struct sk_buff **pskb)
{
    /* 模式0，不限制 */
	if(s_stConfig.mode == 0)
	{
        //DEBUGP("Accept: mode change to 0, no limit.\n ");
		return NF_ACCEPT;
	}
    else
    {
        /* 获取数据包对应的终端类型 */
        ST_DC_DeviceInfo *pstFindDev =  dc_get_devinfo(pskb);
        if (pstFindDev == NULL)
        {
            /* 终端数已经超过最大限制,后来的包直接拒绝 */
            return NF_DROP;
        }

        /* 模式1，不区分终端类型 */
    	if(s_stConfig.mode == 1)
    	{
    		//DEBUGP("Limit all terminal:\n");
    		return dc_control_device_by_type(pskb, pstFindDev, DC_DevType_None);
    	}
        /* 模式2，限制指定类型终端 */
    	else
    	{
    		//DEBUGP("Limit specify the type of terminal:\n");
    		return dc_control_device_by_type(pskb, pstFindDev, pstFindDev->type);
    	}
    }
}


/*
按照设备类型控制终端访问外网
0:None
1:Computer
2:Camera
3:STB
4:Phone
*/
static int dc_control_device_by_type(struct sk_buff **pskb,
    ST_DC_DeviceInfo *dev, int type)
{
    int i = 0;
	int maxTypeNumber = 0;
	int *pTypeNumber;
	unsigned int bTypeEnable = 0;

    unsigned long srcIP = ip_hdr(*pskb)->saddr;
    unsigned long destIP = ip_hdr(*pskb)->daddr;

    if ( type == DC_DevType_None)
    {
        bTypeEnable = TRUE;
        maxTypeNumber = s_stConfig.maxTotalNumber;
        pTypeNumber = &(s_astDeviceTypeStatus[type].currentNumber);
    }
    else
    {
        bTypeEnable = s_stConfig.devTypeSettings[type].enable;
		maxTypeNumber = s_stConfig.devTypeSettings[type].maxNumber;
		pTypeNumber = &(s_astDeviceTypeStatus[type].currentNumber);
    }

    DEBUGP("TotalLimit: type=%d, typeEnable=%d, maxTypeNumber=%d, typeNumber=%d\n",
            type, bTypeEnable, maxTypeNumber, *pTypeNumber);

	/* 如果限制开关关闭, 则允许通过 */
	if (bTypeEnable == FALSE)
	{
		DEBUGP("Accept: limit is disabled, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
            NIPQUAD(srcIP), NIPQUAD(destIP));
		return NF_ACCEPT;
	}

    /* 当前数据包对应的终端已经在线，则更新访问时间，并允许通过 */
    if ( dev->time > 0 )
    {
        dc_update_devonline(dev, jiffies);

        DEBUGP("Accept: this terminal has online, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
            NIPQUAD(srcIP), NIPQUAD(destIP));
        return NF_ACCEPT;
    }
    /* 终端不在线，则需要判断在线终端数是否达到上限 */
    else
    {
        /* 如果还未达到最大限制数，则将当前数据包对应的终端设置为在线，并允许通过 */
        if (*pTypeNumber < maxTypeNumber)
        {
            dc_update_devonline(dev, jiffies);
            (*pTypeNumber)++;

            DEBUGP("Accept: this terminal set to online, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                NIPQUAD(srcIP), NIPQUAD(destIP));
            return NF_ACCEPT;
        }
        /* 如果已经达到最大限制数，则将超时的终端设置为下线 */
        else
        {
            /* 遍历终端列表，将当前终端类型的超时终端设置为下线 */
            ST_DC_HashNode *pos = NULL;
            dc_hash_foreach(s_pstDeviceTable, pos, i)
            {
                if (pos->dev.time != 0
                    && (pos->dev.type == type || type == DC_DevType_None)
                    && (jiffies - pos->dev.time) > s_stConfig.timeout * HZ )
            	{
                    dc_update_devonline(&(pos->dev), 0);
                    if ((*pTypeNumber) > 0)
                    {
                        (*pTypeNumber)--;
                    }


                    DEBUGP("this terminal(" NIPQUAD_FMT ") has timeout, set to offline\n",
                        NIPQUAD(pos->dev.ipaddr));
            	}
            }

            /*
             再次判断在线终端数是否达到最大限制数
            */

            /* 如果未达到上限，则将当前数据包对应的终端设置为在线，并允许通过 */
            if (*pTypeNumber < maxTypeNumber)
            {
                dc_update_devonline(dev, jiffies);
                (*pTypeNumber)++;

                DEBUGP("Accept: this terminal set to online, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                    NIPQUAD(srcIP), NIPQUAD(destIP));
                return NF_ACCEPT;
            }
            /* 如果已经达到最大限制数，则将当前数据包对应的终端设置为离线，并拒绝通过 */
            else
            {
                dc_update_devonline(dev, 0);

                DEBUGP("DROP: Terminals up to limit, so this terminal set to offline, " NIPQUAD_FMT "->" NIPQUAD_FMT "\n",
                    NIPQUAD(srcIP), NIPQUAD(destIP));

                return NF_DROP;
            }
        }
    }

    return NF_ACCEPT;
}


#define __INIT_SEC__

/*
钩子
不挂在FORWARD链上是因为NAT会改变源IP,这里我们挂在PREROUTING链上,优先级比NAT高
*/
static struct nf_hook_ops ip_devicectl_out_ops = {
	.hook		= dc_ip_devctl_out,
	.owner		= THIS_MODULE,
	.pf			= PF_INET,
	.hooknum	= NF_INET_PRE_ROUTING,
	.priority   = NF_IP_PRI_FIRST,
};

/* 初始化参数 */
static void __init dc_init_config(void)
{
    /* 初始化配置 */
    memset(&s_stConfig, 0, sizeof(ST_DC_Config));
    /* 初始化状态数组 */
    memset(&s_astDeviceTypeStatus, 0, sizeof(ST_DC_DevTypeStatus) * DC_DevType_Max);
    /* 初始化锁 */
    rwlock_init(&s_devinfo_lock);

    dc_show_config();
}


/* 模块初始化 */
static int __init dc_init(void)
{
	int ret = 0;

    DEBUGP("module load...\n");

    /* 初始化设置信息 */
	dc_init_config();
    s_stConfig.print = 1;

    /* 为设备信息表分配内存 */
    s_pstDeviceTable = dc_hash_create(MAX_DEVICE_COUNT);
    if (s_pstDeviceTable == NULL)
    {
        DEBUGP("ip_conntrack_devctl: can't get memory for devinfo.\n");
		goto out;
    }

	/* 注册在PREROUTING上注册钩子 */
	ret = nf_register_hook(&ip_devicectl_out_ops);
	if (ret < 0)
    {
		DEBUGP("ip_conntrack_devctl: can't register pre-routing hook.\n");
		goto out;
	}

    /* 创建netlink套接字监听应用层的控制请求 */
    s_netlink_sock = netlink_kernel_create(&init_net, NETLINK_TBS_DEVCTL, 0,
        dc_recv_handler, NULL, THIS_MODULE);
	if (s_netlink_sock == NULL)
    {
        DEBUGP("Fail to create netlink socket.\n");
        ret = -EIO;
        goto out;
    }

	DEBUGP("module load...success\n");

out:
	if(ret != 0)
		dc_fini();

	return ret;
}

/* 模块注销 */
static void __exit dc_fini(void)
{
	DEBUGP("module unload...\n");

    /* 关闭netlink套接字 */
    if (s_netlink_sock != NULL)
        sock_release(s_netlink_sock->sk_socket);

    /* 卸载钩子 */
	nf_unregister_hook(&ip_devicectl_out_ops);

    /* 销毁设备信息表 */
    if (s_pstDeviceTable != NULL)
        dc_hash_destory(s_pstDeviceTable);

	DEBUGP("module unload...success\n");
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("kuangsuwen <kuagnsuwen@twsz.com>");
MODULE_DESCRIPTION("multi-terminals access internet control module");

module_init(dc_init);
module_exit(dc_fini);


