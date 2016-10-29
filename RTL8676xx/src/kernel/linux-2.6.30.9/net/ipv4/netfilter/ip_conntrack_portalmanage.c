/**********************************************************************
 Copyright (c), 1991-2009, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称: ip_conntrack_portalmanage.c
 文件描述: 强制门户控制内核模块
 函数列表:
 修订记录:
           1 作者 : 陈赞辉
             日期 : 2009-05-15
             描述 : 创建
**********************************************************************/
#include <linux/init.h>
#include <asm/system.h>
#include <linux/sched.h>
#include <linux/socket.h>
#include <linux/net.h>
#include <linux/un.h>
#include <linux/in.h>
#include <linux/inet.h>
#include <linux/config.h>
#include <linux/ctype.h>
#include <linux/module.h>
#include <linux/netfilter.h>
#include <linux/ip.h>
#include <net/ip.h>
#include <net/tcp.h>
#include <net/route.h>
#include <net/netfilter/nf_conntrack_helper.h>
#include <net/netfilter/nf_conntrack_tuple.h>
#include <linux/skbuff.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/if.h>
#include <linux/spinlock.h>
#include <linux/inetdevice.h>

#include <linux/netfilter_ipv4/ip_conntrack_devctl.h>

#ifdef CONFIG_BRIDGE_NETFILTER
#include <linux/netfilter_bridge.h>
#endif
/******************************************************************************
*                               GLOBAL VAR                                   *
******************************************************************************/
/*调用Devctl内核模块获取设备类型，必须编译到内核中*/
#define GETDEVTYPE		

/*定义: 如果没有URL就关闭强门户功能*/
#define ENABLE_DEVICE	

/*使用超时维护强制门户列表*/
/*问题:设备关联功能还末实现*/
#define TIMEOUT_ENABLE

/*调试打印*/
//#define PORTAL_DEBUG
//#define LOCALPORTAL


#define HTTP_BUF_LEN 	16384
#define MAX_LIST_SIZE 	64
#define HTTP_PORT 		80

#ifdef TIMEOUT_ENABLE
/* 20s*/
#define TIMER_VALUE		20			
/* 强制门户超时间:1h */
#define TO_MACVALUE		3600000		
#define PM_TIMEOUT	"time="
#define STATICADDR 0xc0a80101 //192.168.1.1
#endif
/* 定义命令字符*/
#define PCURL 		"PC_url="
#define STBURL 		"STB_url="
#define PHONEURL 	"Phone_url="
#define ALLURL 		"All_url="
#define DEL_MAC		"Del="    
#define DEVREGON    "DevRegOn"
#define DEVREGOFF	"DevRegOff"
#define DEVREGURL	"DevRegUrl="

#define CMD_ON		"on"
#define CMD_OFF		"off"
#define CMD_QUERY	"query"
#define CMD_FLUSH	"flush"
#define INTERNET_OK         "internet_ok"
#define INTERNET_NOT_OK     "internet_not_ok"
#define FIRST_PERFORM     "first_perfrom"
#define NOT_FIRST_PERFORM     "not_first_perfrom"

#define PM_TO_ENABLE 	1
#define PM_TO_DISABLE 	0
/* 定义调试打印宏*/
#ifdef PORTAL_DEBUG
#define DEBUGP(fmt, args...) printk("%4d %15s: "fmt,  __LINE__, __FUNCTION__, ## args);
#define TRACE(fmt, args...)  printk(fmt, ## args);
#else
#define DEBUGP(fmt, args...)
#define TRACE(fmt, args...)
#endif
/* 定时器*/
DEFINE_SPINLOCK(pm_dev_list_lock);
#define LOCK_BH(l) spin_lock_bh(l)
#define UNLOCK_BH(l) spin_unlock_bh(l)

/* 终端记录结构体*/
static struct list_head pm_dev_list;
typedef struct {
	struct list_head list;
	int devtype;
	char mac[ETH_ALEN];
	
#ifdef TIMEOUT_ENABLE	
	uint time;
#endif
	int iFirstChk;
}ST_PM_DEVINFO;
static struct list_head pm_mac_haed;
typedef struct {
	struct list_head list;
	char mac[ETH_ALEN];
}MAC_LIST;
//bridge:: br_input.c
int register_result;
int   t = 0;



/*=============================
设备类型:
 0:none
 1:Computer
 3:Camera
 2:STB
 4:Phone
================================*/
enum PM_DEV_TYPE {
	PM_DEV_NONE= 0,
	PM_DEV_PC,
	PM_DEV_CAMERA,
	PM_DEV_STB,
	PM_DEV_PHONE,
	PM_LOGIC_ID
};
/* 全局参数结构体*/
typedef struct {
	atomic_t EnableForce; 
	atomic_t En_logicID;
	atomic_t internet_not_ok; 
	atomic_t first_perform;
	atomic_t aqui_flag;
#ifdef TIMEOUT_ENABLE	
	uint timeout;   				//Uint:ms
#endif	
	uint device_enable[6];
	char pPortalPC[256];
	char pPortalSTB[256];
	char pPortalPhone[256];
	char LogicURL[256];
}PM_CONFIG;

static PM_CONFIG pstConfig;
/* 强制页面模板*/
static char* pszHttpRedirectHead = 
	"HTTP/1.1 302 Object Moved\r\n"
	"Location: http://%s\r\n"
	"Server: adsl-router-gateway\r\n"
	"Content-Type: text/html\r\n"
	"Content-Length: %d\r\n"
	"\r\n"
	"%s\r\n";
static char* pszHttpRedirectContent = 
	"<html><head><title>Object Moved</title></head>"
	"<body><h1>Object Moved</h1>This Object may be found in "
	"<a HREF=\"http://%s\">here</a>.</body></html>";


extern int br_fdb_ifname_get_mac(char *ifname, int port_no, struct list_head *plisthead);

/******************************************************************************
*                               FUNCTION                                     *
******************************************************************************/
void dev_ip_addr(char *devname, unsigned char *bin_buf)
{
	struct net_device *dev = __dev_get_by_name(&init_net, devname);
	struct in_device *ip = dev->ip_ptr;
	struct in_ifaddr *in;

	if ((ip == NULL) || ((in = ip->ifa_list) == NULL)) {
		printk(KERN_WARNING "dev_ip_addr - device not assigned an "
		       "IP address\n");
		return;
	}
	memcpy(bin_buf, &in->ifa_address, sizeof(in->ifa_address));
}
#ifdef TIMEOUT_ENABLE

struct timer_list pm_timeout;
/*=======================================================================        
  功能: 更新设备超时时间       
  参数: 
  			up_switch= 
  			0:Mac 
  			1:pstDevinfo
  返回: 
  备注:                                                          
=======================================================================*/
static void pm_uptime(ST_PM_DEVINFO *pstDevinfo,char *sMac,uint up_switch)
{
	struct list_head *pCur_item,*pNext;
	ST_PM_DEVINFO *psDevInfo;

	if(up_switch && pstDevinfo){   /*当pstDevinfo为空而up_switch为1时，会导至非法访问*/
		pstDevinfo->time = jiffies_to_msecs(jiffies);
	}
	else{
		LOCK_BH(&pm_dev_list_lock);
		list_for_each_safe(pCur_item,pNext, &pm_dev_list) { 
			psDevInfo = list_entry(pCur_item, ST_PM_DEVINFO, list);	
			if(memcmp(sMac,psDevInfo->mac, ETH_ALEN) == 0)
			/* 更新记录时间*/
			psDevInfo->time = jiffies_to_msecs(jiffies);
		}
		UNLOCK_BH(&pm_dev_list_lock);
	}
	
}
/*=======================================================================    
  功能: 检查超时间     
  参数: ST_PM_DEVINFO 设备记录信息
  返回: 
  备注:                                                              
=======================================================================*/
static uint pm_timeout_checked(ST_PM_DEVINFO *pstDevinfo)
{
	if(jiffies_to_msecs(jiffies)-pstDevinfo->time > pstConfig.timeout)
		return PM_TO_ENABLE;
	else
		return PM_TO_DISABLE;
}
/*=======================================================================    
  功能: 定时器处理函数      
  参数: 
  返回:     
  备注:                                                          
=======================================================================*/
static void pm_time_process(unsigned long data)
{
	struct list_head *pCur_item,*pNext;
	ST_PM_DEVINFO *psDevInfo;

	del_timer(&pm_timeout);
	pm_timeout.expires  = jiffies + TIMER_VALUE * HZ;
	add_timer(&pm_timeout);
	if(atomic_read(&pstConfig.EnableForce) == 0) 
		return;

	LOCK_BH(&pm_dev_list_lock);
	list_for_each_safe(pCur_item,pNext, &pm_dev_list) { 
		psDevInfo = list_entry(pCur_item, ST_PM_DEVINFO, list);	
		/*如果超时则删除MAC记录*/
		if(pm_timeout_checked(psDevInfo)){
			if(psDevInfo->iFirstChk==1){
				/*add fun send ping packer*/
				psDevInfo->iFirstChk=2;
			}
			else{
			DEBUGP("TimeOut del perv_list\n");
			list_del(&psDevInfo->list);
			kfree(psDevInfo);
			}		
		}
	}
	UNLOCK_BH(&pm_dev_list_lock);
}
/*=======================================================================
  功能: 初始化定时器   
  参数: 
  返回:                                                              
=======================================================================*/
static void pm_init_timer(uint value)
{
	init_timer(&pm_timeout);
	pm_timeout.function = pm_time_process;
	pm_timeout.data     = 0;
	pm_timeout.expires  = jiffies + value * HZ;
	add_timer(&pm_timeout);
	
}
#endif
/*=======================================================================    
  功能: 删除指定MAC表项    
  参数: 
  返回:     
  备注:                                                          
=======================================================================*/
static void Del_mac_process(void)
{
	struct list_head *pCur_item,*pNext;
	struct list_head *pCurmac,*pMacnext;
	ST_PM_DEVINFO *psDevInfo;
	MAC_LIST *pstmac;

	if(atomic_read(&pstConfig.EnableForce) == 0) 
		return;
	list_for_each_safe(pCurmac,pMacnext, &pm_mac_haed) {
		pstmac = list_entry(pCurmac, MAC_LIST, list);	
		
			LOCK_BH(&pm_dev_list_lock);
			list_for_each_safe(pCur_item,pNext, &pm_dev_list) { 
				psDevInfo = list_entry(pCur_item, ST_PM_DEVINFO, list);	
				/*删除MAC记录*/
				if((!memcmp(psDevInfo->mac, pstmac->mac, ETH_ALEN))){
					DEBUGP("Del NoLink Mac %s\n",psDevInfo->mac);
					list_del(&psDevInfo->list);
					kfree(psDevInfo);
				}
			}
			UNLOCK_BH(&pm_dev_list_lock);
		//删除br_fdb分配的空间
		list_del(&pstmac->list);
		kfree(pstmac);
	}
}
/*======================================================================= 
  功能: 检查路由表    
  参数: skb  接收的skb报文
  		hook	NF_IP_LOCAL_OUT
  返回:       
  备注:                                                        
=======================================================================*/
static inline struct rtable *route_reverse(struct sk_buff *skb, int hook)
{
	struct iphdr *iph = ip_hdr(skb);
	struct dst_entry *odst;
	struct flowi fl = {};
	struct rtable *rt;

	/* We don't require ip forwarding to be enabled to be able to
	 * send a RST reply for bridged traffic. */
	if (hook != NF_IP_FORWARD
#ifdef CONFIG_BRIDGE_NETFILTER
		|| (skb->nf_bridge && skb->nf_bridge->mask & BRNF_BRIDGED)
#endif
		) {
		/*源IP*/
		fl.nl_u.ip4_u.daddr = iph->saddr;
		if (hook == NF_IP_LOCAL_IN)
			fl.nl_u.ip4_u.saddr = iph->daddr;
		fl.nl_u.ip4_u.tos = RT_TOS(iph->tos);

		if (ip_route_output_key(&init_net, &rt, &fl) != 0)
			return NULL;
	} else {
		/* non-local src, find valid iif to satisfy
		 * rp-filter when calling ip_route_input. */
		fl.nl_u.ip4_u.daddr = iph->daddr;
		if (ip_route_output_key(&init_net, &rt, &fl) != 0)
			return NULL;

		odst = skb->dst;
		if (ip_route_input(skb, iph->saddr, iph->daddr,
			RT_TOS(iph->tos), rt->u.dst.dev) != 0) {
			dst_release(&rt->u.dst);
			return NULL;
		}
		dst_release(&rt->u.dst);
		rt = (struct rtable *)skb->dst;
		skb->dst = odst;
	}

	if (rt->u.dst.error) {
		dst_release(&rt->u.dst);
		rt = NULL;
	}

	return rt;
}
/*=======================================================================    
  功能: 回复RST报文    
  参数: oldskb  接收的skb报文
  		hook	NF_IP_LOCAL_OUT
  返回:         
  备注:                                                      
=======================================================================*/
static void send_reset(struct sk_buff *oldskb, int hook)
{
	struct sk_buff *nskb;
	struct iphdr *iph = ip_hdr(oldskb);
	struct tcphdr _otcph, *oth, *tcph;
	struct rtable *rt;
	u_int16_t tmp_port;
	u_int32_t tmp_addr;
	int needs_ack;
	int hh_len;

	/* IP header checks: fragment. */
	if (ip_hdr(oldskb)->frag_off & htons(IP_OFFSET))
		return;
	/* 获取tcp头数据*/
	oth = skb_header_pointer(oldskb, ip_hdr(oldskb)->ihl * 4,
				 sizeof(_otcph), &_otcph);
	if (oth == NULL)
 		return;

	/* No RST for RST. */
	if (oth->rst)
		return;

	/* Check checksum */
	if (nf_ip_checksum(oldskb, hook, iph->ihl * 4, IPPROTO_TCP))
		return;
    /* 获取数据包路由信息*/
	if ((rt = route_reverse(oldskb, hook)) == NULL)
		return;

	hh_len = LL_RESERVED_SPACE(rt->u.dst.dev);

	/* We need a linear, writeable skb.  We also need to expand
	   headroom in case hh_len of incoming interface < hh_len of
	   outgoing interface */
	nskb = skb_copy_expand(oldskb, hh_len, skb_tailroom(oldskb),
			       GFP_ATOMIC);
	if (!nskb) {
		dst_release(&rt->u.dst);
		return;
	}

	dst_release(nskb->dst);
	nskb->dst = &rt->u.dst;

	/* This packet will not be the same as the other: clear nf fields */
	nf_reset(nskb);
	nskb->mark = 0;
	skb_init_secmark(nskb);

	tcph = (struct tcphdr *)((u_int32_t*)ip_hdr(nskb) + ip_hdr(nskb)->ihl);

	/* Swap source and dest */
	tmp_addr = ip_hdr(nskb)->saddr;
	ip_hdr(nskb)->saddr = ip_hdr(nskb)->daddr;
	ip_hdr(nskb)->daddr = tmp_addr;
	tmp_port = tcph->source;
	tcph->source = tcph->dest;
	tcph->dest = tmp_port;

	/* Truncate to length (no data) */
	tcph->doff = sizeof(struct tcphdr)/4;
	skb_trim(nskb, ip_hdr(nskb)->ihl*4 + sizeof(struct tcphdr));
	ip_hdr(nskb)->tot_len = htons(nskb->len);

	if (tcph->ack) {
		needs_ack = 0;
		tcph->seq = oth->ack_seq;
		tcph->ack_seq = 0;
	} else {
		needs_ack = 1;
		tcph->ack_seq = htonl(ntohl(oth->seq) + oth->syn + oth->fin
				      + oldskb->len - ip_hdr(oldskb)->ihl*4
				      - (oth->doff<<2));
		tcph->seq = 0;
	}

	/* Reset flags */
	((u_int8_t *)tcph)[13] = 0;
	tcph->rst = 1;
	tcph->ack = needs_ack;

	tcph->window = 0;
	tcph->urg_ptr = 0;

	/* Adjust TCP checksum */
	tcph->check = 0;
	tcph->check = tcp_v4_check(sizeof(struct tcphdr),
				   ip_hdr(nskb)->saddr,
				   ip_hdr(nskb)->daddr,
				   csum_partial((char *)tcph,
						sizeof(struct tcphdr), 0));

	/* Adjust IP TTL, DF */
	ip_hdr(nskb)->ttl = dst_metric(nskb->dst, RTAX_HOPLIMIT);
	/* Set DF, id = 0 */
	ip_hdr(nskb)->frag_off = htons(IP_DF);
	ip_hdr(nskb)->id = 0;

	/* Adjust IP checksum */
	ip_hdr(nskb)->check = 0;
	ip_hdr(nskb)->check = ip_fast_csum((unsigned char *)ip_hdr(nskb), 
					   ip_hdr(nskb)->ihl);
	DEBUGP("RST Never happens %d: %d\n",dst_mtu(nskb->dst),nskb->len);
	/* "Never happens" */
	if (nskb->len > dst_mtu(nskb->dst))
		goto free_nskb;

	nf_ct_attach(nskb, oldskb);

	NF_HOOK(PF_INET, NF_IP_LOCAL_OUT, nskb, NULL, nskb->dst->dev,
		dst_output);
	return;

 free_nskb:
	kfree_skb(nskb);
}

/*=======================================================================
  功能: Send http redirect response
  参数:                       
  返回: NF_ACCEPT
  备注:
=======================================================================*/
static void send_redirect(struct sk_buff *oldskb, int hook, int iDevType)
{
	struct sk_buff *nskb;
	struct tcphdr otcph, *tcph;
	u_int16_t tmp_port;
	u_int32_t tmp_addr;
	int needs_ack;
	char szRedirectPack[512];
	char szRedirectContent[260];
	char *dptr = NULL;
	struct rtable *rt;

	/* IP header checks: fragment. */
	if (ip_hdr(oldskb)->frag_off & htons(IP_OFFSET)){
		DEBUGP("err in fragment\n");
		return;
	}
	/* 强制包路由处理*/
	if ((rt = route_reverse(oldskb, hook)) == NULL){
		DEBUGP("get route table fail\n");
		return;
	}

	if (skb_copy_bits(oldskb, ip_hdr(oldskb)->ihl*4,
			  &otcph, sizeof(otcph)) < 0){
		DEBUGP("copy otcph from oldskb fail\n");
 		return;
	}
	/* 检查数据包的合法性*/
	if (otcph.rst){
		DEBUGP("err is a RST package\n");
		return;
	}
	/* 根据设备类型强制到相应的URL*/
	switch(iDevType) {
		case PM_DEV_STB:
			sprintf(szRedirectContent, pszHttpRedirectContent, pstConfig.pPortalSTB);
			sprintf(szRedirectPack, pszHttpRedirectHead, pstConfig.pPortalSTB,  
				strlen(szRedirectContent), szRedirectContent); 
			break;
		case PM_DEV_PHONE:
			sprintf(szRedirectContent, pszHttpRedirectContent, pstConfig.pPortalPhone);
			sprintf(szRedirectPack, pszHttpRedirectHead, pstConfig.pPortalPhone,  
				strlen(szRedirectContent), szRedirectContent); 
			break;
		case PM_LOGIC_ID:
			sprintf(szRedirectContent, pszHttpRedirectContent, pstConfig.LogicURL);
			sprintf(szRedirectPack, pszHttpRedirectHead, pstConfig.LogicURL,  
				strlen(szRedirectContent), szRedirectContent); 
			break;
		case PM_DEV_PC:
		default:
			sprintf(szRedirectContent, pszHttpRedirectContent, pstConfig.pPortalPC);
			sprintf(szRedirectPack, pszHttpRedirectHead, pstConfig.pPortalPC,  
				strlen(szRedirectContent), szRedirectContent); 
			break;
	}
	/* We need a linear, writeable skb.  We also need to expand
	   headroom in case hh_len of incoming interface < hh_len of
	   outgoing interface */
	nskb = skb_copy_expand(oldskb, LL_MAX_HEADER, 
		skb_tailroom(oldskb) + strlen(szRedirectPack), GFP_ATOMIC);

	if (!nskb) {
		DEBUGP("nskb expand fail\n");
		dst_release(&rt->u.dst);
		return;
	}

	skb_put(nskb, strlen(szRedirectPack)); 
	/*释放当前路由Cache*/
	dst_release(nskb->dst);
	/*更新路由信息*/
	nskb->dst = &rt->u.dst;
	/* This packet will not be the same as the other: clear nf fields */
	nf_reset(nskb);
	nskb->mark = 0;
	skb_init_secmark(nskb);
	tcph = (struct tcphdr *)((u_int32_t*)ip_hdr(nskb) + ip_hdr(nskb)->ihl);
	/* Swap source and dest */
	tmp_addr = ip_hdr(nskb)->saddr;
	ip_hdr(nskb)->saddr = ip_hdr(nskb)->daddr;
	ip_hdr(nskb)->daddr = tmp_addr;
	tmp_port = tcph->source;
	tcph->source = tcph->dest;
	tcph->dest = tmp_port;
	/* Truncate to length (no data) */
	tcph->doff = sizeof(struct tcphdr)/4;
	skb_trim(nskb, ip_hdr(nskb)->ihl*4 + sizeof(struct tcphdr) + strlen(szRedirectPack));
	ip_hdr(nskb)->tot_len = htons(nskb->len);

	if (tcph->ack) {
		tcph->seq = otcph.ack_seq;
	} else {		
		tcph->seq = 0;
	}

	tcph->ack_seq = htonl(ntohl(otcph.seq) + otcph.syn + otcph.fin
	      + oldskb->len - ip_hdr(oldskb)->ihl*4
	      - (otcph.doff<<2));
	needs_ack = 1;
	/* Reset flags */
	((u_int8_t *)tcph)[13] = 0;
	tcph->ack = needs_ack;
	tcph->psh = 1;
	tcph->window = 0;
	tcph->urg_ptr = 0;
	/* fill in data */
	dptr =  (char*)tcph  + tcph->doff * 4;
	memcpy(dptr, szRedirectPack, strlen(szRedirectPack));
	/* Adjust TCP checksum */
	tcph->check = 0;
	tcph->check = tcp_v4_check(sizeof(struct tcphdr) + strlen(szRedirectPack),
				   ip_hdr(nskb)->saddr,
				   ip_hdr(nskb)->daddr,
				   csum_partial((char *)tcph,
						sizeof(struct tcphdr) + strlen(szRedirectPack), 0));
	/* Set DF, id = 0 数据分片*/
	ip_hdr(nskb)->frag_off = htons(IP_DF);
	ip_hdr(nskb)->id = 0;

	nskb->ip_summed = CHECKSUM_NONE;
	/* Adjust IP TTL, DF */
	ip_hdr(nskb)->ttl = MAXTTL;
	/* Adjust IP checksum */
	ip_hdr(nskb)->check = 0;
	ip_hdr(nskb)->check = ip_fast_csum((unsigned char *)ip_hdr(nskb), 
					   ip_hdr(nskb)->ihl);
	
	/* "Never happens" */
	if (nskb->len > dst_mtu(nskb->dst))
		goto free_nskb;
	nf_ct_attach(nskb, oldskb);
	
	/* send PM pack*/
	/*NF_HOOK(PF_INET, NF_IP_LOCAL_OUT, nskb, NULL, nskb->dst->dev,dst_output);*/
	ip_output(nskb);
	
	/*contine the oldskb send, modify oldskb as a "reset" tcp pack*/
	send_reset(oldskb, NF_INET_PRE_ROUTING);
	return;

free_nskb:
	kfree_skb(nskb);
}

/*=======================================================================         
  功能: 字符长度             
  参数:                              
  返回: 长度  
  备注:                                                            
=========================================================================*/
static int line_str_len(const char *line, const char *limit)
{
	const char *k = line;

	while ((line <= limit) && (*line == '\r' || *line == '\n'))
		line++;

	while (line <= limit) {
		if (*line == '\r' || *line == '\n')
			break;
		line++;
	}

	return line - k;
}
/*=======================================================================         
  功能: 字符搜索             
  参数:                              
  返回: NF_ACCEPT  
  备注:                                                            
=========================================================================*/
static const char* line_str_search(const char *needle, const char *haystack, 
			size_t needle_len, size_t haystack_len) 
{
	const char *limit = haystack + (haystack_len - needle_len);

	while (haystack <= limit) {
		if (strnicmp(haystack, needle, needle_len) == 0)
			return haystack;
		haystack++;
	}
	return NULL;
}

/*=======================================================================         
  功能: 钩子处理函数             
  参数:                              
  返回: NF_ACCEPT  
  备注:                                                            
=========================================================================*/
static unsigned int
ip_pm_in(unsigned int hooknum,
			struct sk_buff *skb,
			const struct net_device *in,
			const struct net_device *out,
			int (*okfn)(struct sk_buff *))
{
	int iRet = NF_ACCEPT;
	int iFirstChk = 0;
	int i;
	int iDataoff;
	unsigned int iDevType;
	struct list_head *pCur_item,*pNext;
	ST_PM_DEVINFO *psDevInfo;
	struct tcphdr *tcph;
	struct nf_conn *ct;
	enum ip_conntrack_info ctinfo;
	enum ip_conntrack_dir dir;
	__be32 inetAddr;
#ifdef TIMEOUT_ENABLE
	char pMac[ETH_ALEN];
#endif
	char *pHttp_Buffer;
	static char* http_request_cmd[8] = {
	"GET", "POST", "PUT", "OPTIONS", "HEAD", "DELETE", "TRACE", NULL};

	/* 判断强制功能是否全使能 */
	if ((atomic_read(&pstConfig.EnableForce) == 0) && (atomic_read(&pstConfig.En_logicID) == 0))
		return iRet;

	ct = nf_ct_get(skb, &ctinfo);
	dir = CTINFO2DIR(ctinfo);

	/* 检查数据包是否有效 */
	if (dir == IP_CT_DIR_REPLY || !skb_mac_header(skb))
		goto out;

	/* 检查是否TCP数据包 */
	if (ip_hdr(skb)->protocol != IPPROTO_TCP){
#ifdef TIMEOUT_ENABLE
		if (ip_hdr(skb)->protocol == IPPROTO_ICMP){ /*为什么通过ICMP来更新时间*/
			DEBUGP("IPPROTO_ICMP\n");
			memcpy(pMac, skb_mac_header(skb) + 6, ETH_ALEN);
			pm_uptime(NULL, pMac, 0);
		}
#endif	
		goto out;
	}

	tcph = (void *)ip_hdr(skb) + ip_hdr(skb)->ihl * 4;

	/* 只强制80端口HTTP访问 */
	if (tcph->dest != htons(80))
		goto out;

	iDataoff = ip_hdr(skb)->ihl * 4 + tcph->doff * 4;

	/* No data? */
	if (iDataoff >= skb->len)
		goto out;
#ifdef LOCALPORTAL
	dev_ip_addr("br0", &inetAddr);
	DEBUGP("Local IP=0x%08x skb saddr=0x%08x\n", inetAddr, ip_hdr(skb)->daddr);
	if (ip_hdr(skb)->daddr == inetAddr)
		return iRet;
#endif

	/* 这里只是用来比较，应该可以不用申请空间，用原报文比较可以减少运行时间 */
	pHttp_Buffer = kmalloc(skb->len - iDataoff, GFP_ATOMIC);
	if (!pHttp_Buffer) {
		DEBUGP("kmalloc http_buffer failed\n");
		goto out;
	}

	if (skb_copy_bits(skb, iDataoff, pHttp_Buffer, skb->len - iDataoff) != 0) {
		DEBUGP("Copy http_buffer form skb Fail\n");
		kfree(pHttp_Buffer);
		goto out;
	}

	/* check only http request pack, just send_redirect() */
	if (line_str_search("HTTP", pHttp_Buffer, strlen("HTTP"),
		line_str_len(pHttp_Buffer, pHttp_Buffer + skb->len - iDataoff)) == NULL) {
		DEBUGP("is not http head pack\n");
		kfree(pHttp_Buffer);
		/* is not http head pack */
		goto out;
	}

	/* 检查是否HTTP请求包 */
	for (i = 0; http_request_cmd[i]; i++) {
		if (line_str_search(http_request_cmd[i],
			pHttp_Buffer, strlen(http_request_cmd[i]),
			line_str_len(pHttp_Buffer, pHttp_Buffer + skb->len - iDataoff)) != NULL) {
			iFirstChk = 1;
			break;
		}
	}
	kfree(pHttp_Buffer);
	

#if 0 //西电的强制门户的第一次时是针对CPE，而不是针对LAN侧通讯设备
	if(iFirstChk == 0) {
	    /*is not http request head pack*/
		goto out; 
	}

	/*reset the var for below use*/
	iFirstChk = 0;
	if(atomic_read(&pstConfig.En_logicID) == 0)
	{
		LOCK_BH(&pm_dev_list_lock);
		list_for_each_safe(pCur_item,pNext, &pm_dev_list) { 
			psDevInfo = list_entry(pCur_item, ST_PM_DEVINFO, list);
			if(memcmp(skb->mac.raw + 6,psDevInfo->mac, ETH_ALEN) != 0)
				continue;
#ifdef TIMEOUT_ENABLE		
			/*有数据请求根据Mac更新超时时间*/
			pm_uptime(psDevInfo,NULL,1);
#endif		
			/* 检查对应设备是否设置了URL*/
#ifdef ENABLE_DEVICE 
				DEBUGP("device_enable[%d]=%d\n",psDevInfo->devtype,pstConfig.device_enable[psDevInfo->devtype]);
				if (pstConfig.device_enable[psDevInfo->devtype] == 0){			
					UNLOCK_BH(&pm_dev_list_lock);
					goto out;
				}
#endif 
			/* 检查是否第一次强制*/
			if(!psDevInfo->iFirstChk) {
				/*发送强制包处理*/
				send_redirect(skb, NF_IP_PRE_ROUTING, psDevInfo->devtype);
				psDevInfo->iFirstChk = 1;
			}			
			iFirstChk = 1;
			break;
		}

		if(iFirstChk == 1) {
			UNLOCK_BH(&pm_dev_list_lock);
			DEBUGP("already has send_redirect()\n");		
			goto out;
		}

	//  when we arrive here, it must be a new device, but dhcp did not notify us, so we name it PC type default

#ifdef ENABLE_DEVICE
		if (pstConfig.device_enable[PM_DEV_PC] == 0) {
			DEBUGP("PM_DEV_PC:device_enable=0\n");
			UNLOCK_BH(&pm_dev_list_lock);
			goto out;
		}
#endif
			
		DEBUGP("a new addr is adding in\n");
		psDevInfo = (ST_PM_DEVINFO *)kmalloc(sizeof(ST_PM_DEVINFO), GFP_ATOMIC);
		if(!psDevInfo) {
			UNLOCK_BH(&pm_dev_list_lock);
			DEBUGP("kmalloc record failed\n");
			iRet = NF_DROP;
			goto out;
		}
		memcpy(psDevInfo->mac, skb->mac.raw + 6, ETH_ALEN);
		/* 根据mac地址或者ip地址，获取对应终端的设备类型 */
#ifdef GETDEVTYPE	
		iDevType = dc_get_devtype(DC_FT_ByMac, psDevInfo->mac);	
		DEBUGP("iDevtypy=%d\n",iDevType);
#else
		iDevType = PM_DEV_PC;	
#endif
		psDevInfo->devtype =iDevType;
#ifdef TIMEOUT_ENABLE
		/*记录超时时间*/
		pm_uptime(psDevInfo,NULL,1);
#endif	
		DEBUGP("newadd: SOURCE_MAC=%02x-%02x-%02x-%02x-%02x-%02x\n", 
				skb->mac.ethernet->h_source[0],
				skb->mac.ethernet->h_source[1],
				skb->mac.ethernet->h_source[2],
				skb->mac.ethernet->h_source[3],
				skb->mac.ethernet->h_source[4],
				skb->mac.ethernet->h_source[5]);
		
		DEBUGP("call send_redirect()\n");
		send_redirect(skb, NF_IP_PRE_ROUTING, psDevInfo->devtype);
		psDevInfo->iFirstChk = 1;
		list_add(&psDevInfo->list, &pm_dev_list);	
		UNLOCK_BH(&pm_dev_list_lock);
	}
	else
	{
		DEBUGP("device_enable[%d]=%d\n",PM_LOGIC_ID,pstConfig.device_enable[PM_LOGIC_ID]);
		if (pstConfig.device_enable[PM_LOGIC_ID] == 0){			
			goto out;
		}
		DEBUGP("LOGIC ID call send_redirect\n");
		send_redirect(skb, NF_IP_PRE_ROUTING, PM_LOGIC_ID);
	}
#else
#if 0
       //if internet is not ok, then perform force-portal
        if (atomic_read(&pstConfig.internet_not_ok))
        {
            send_redirect(skb, NF_IP_PRE_ROUTING, PM_DEV_PC);
            goto out;
        }
#endif
        if(1)
        {
            //if pre-gui aqui button not press down, then perform force-portal
            if (!atomic_read(&pstConfig.aqui_flag) && (t == 0 || jiffies - t > 100) && (ip_hdr(skb)->daddr != STATICADDR))
            {
                send_redirect(skb, NF_INET_PRE_ROUTING, PM_DEV_PC);
		   t = jiffies;		
                goto out;
            }
	     #if 0			
	     else
	     {
                if(skb->nh.iph->daddr == STATICADDR)//0xc0a86fde is 192.168.111.222
                {
                    if ((atomic_read(&pstConfig.internet_not_ok) )&& (t == 0 || jiffies - t > 100))
                    {
                        //redirect to the specify page different from welcome page(to 
                        //be done, current use welcome page)
                        send_redirect(skb, NF_IP_PRE_ROUTING, PM_DEV_PC);
                        t = jiffies;
                        goto out;
                    }
                }
	     }
	     #endif	 
        }    
#endif
	
out:
	return iRet;
}


//目前仅读first)perform的值
ssize_t read_dev_url(char *buf, char **start, off_t offset, int len, int *eof, void *data)
{
	int ret;
	char *print = buf;

	ret = snprintf(print, len, "is %s first perform:%d",
				   atomic_read(&pstConfig.first_perform) == 1 ? "" : "not",
				   atomic_read(&pstConfig.first_perform));
	*eof = 1;

	return ret;
}

/*=======================================================================       
  功能: write dev url	   
  参数:             
  返回:         
  备注:  有没有针对某种设备来使用门户网站的情况
=======================================================================*/
int write_dev_url(struct file *file, const char *buffer, 
				unsigned long count, void *data)
{
	char buf[128] = "";
	struct list_head *cur_item,*next;
	ST_PM_DEVINFO *record;
	int i = 0;
	char iFname[50]="";
	int port_no=0;

	if (count > 255) {
		DEBUGP("url add is too long\n");
		return -EFAULT;
	}
	if (copy_from_user(buf, buffer, count))
		return -EFAULT;

	/* 设置所有设备终端URL*/
	if (memcmp(buf, ALLURL,strlen(ALLURL)-1) == 0)
	{
		DEBUGP("all_url\n");
		memset(pstConfig.pPortalPC ,'\0', 256);
		memset(pstConfig.pPortalSTB ,'\0', 256);
		memset(pstConfig.pPortalPhone ,'\0', 256);
		if (strlen(buf) == (strlen(ALLURL)+1) || strlen(buf) == (strlen(ALLURL)+2)) {
			for (i = 0; i < 5; i++)
				pstConfig.device_enable[i] = 0;
			goto back;
		}
		memcpy(pstConfig.pPortalPC, buf + strlen(ALLURL), strlen(buf) - strlen(ALLURL));
		if (pstConfig.pPortalPC[strlen(pstConfig.pPortalPC) - 1] == '\n')
			pstConfig.pPortalPC[strlen(pstConfig.pPortalPC) - 1] = '\0';
		memcpy(pstConfig.pPortalSTB, pstConfig.pPortalPC, strlen(pstConfig.pPortalPC));
		memcpy(pstConfig.pPortalPhone, pstConfig.pPortalPC, strlen(pstConfig.pPortalPC));
		for (i = 0; i < 5; i++)
			pstConfig.device_enable[i] = 1;
		goto back;
	}

    /* 设置PC终端强制URL*/
	if (memcmp(buf, PCURL, strlen(PCURL)-1) == 0) {
		DEBUGP("PC_url\n");
		memset(pstConfig.pPortalPC, '\0', 256);
		if (strlen(buf) == (strlen(PCURL)+1) || \
			strlen(buf) == (strlen(PCURL)+2)) {
			pstConfig.device_enable[PM_DEV_PC] = 0;
			goto back;
		}
		pstConfig.device_enable[PM_DEV_PC] = 1;
		memcpy(pstConfig.pPortalPC, buf + strlen(PCURL),\
			   strlen(buf) - strlen(PCURL));
		
		if (pstConfig.pPortalPC[strlen(pstConfig.pPortalPC) - 1] == '\n')
			pstConfig.pPortalPC[strlen(pstConfig.pPortalPC) - 1] = '\0';
		goto back;
	}

	/* 设置STB终端强制URL*/
	if (memcmp(buf,STBURL, strlen(STBURL)-1) ==0) {
		DEBUGP("STB_url\n");
		memset(pstConfig.pPortalSTB, '\0', 256);
		if (strlen(buf) == (strlen(STBURL)+1) || \
			strlen(buf) == (strlen(STBURL)+2)) {
			pstConfig.device_enable[PM_DEV_STB] = 0;
			goto back;
		}
		pstConfig.device_enable[PM_DEV_STB] = 1;
		memcpy(pstConfig.pPortalSTB, buf + strlen(STBURL), \
			   strlen(buf) - strlen(STBURL));
		
		if (pstConfig.pPortalSTB[strlen(pstConfig.pPortalSTB) - 1] == '\n')
			pstConfig.pPortalSTB[strlen(pstConfig.pPortalSTB) - 1] = '\0';
		goto back;
	}

	/* 设置电话终端强制URL*/
	if (memcmp(buf, PHONEURL, strlen(PHONEURL)-1) ==0)
	{
		DEBUGP("Phone_url\n");
		memset(pstConfig.pPortalPhone, '\0', 256);
		if (strlen(buf) == (strlen(PHONEURL)+1) || \
			strlen(buf) == (strlen(PHONEURL)+2)) {
			pstConfig.device_enable[PM_DEV_PHONE] = 0;
			goto back;
		}
		pstConfig.device_enable[PM_DEV_PHONE] = 1;
		memcpy(pstConfig.pPortalPhone, buf + strlen(PHONEURL),\
			strlen(buf) - strlen(PHONEURL));
		
		if (pstConfig.pPortalPhone[strlen(pstConfig.pPortalPhone) - 1] == '\n')
			pstConfig.pPortalPhone[strlen(pstConfig.pPortalPhone) - 1] = '\0';
		goto back;
	}

	/* 设置逻辑ID认证强制URL */
	if (memcmp(buf, DEVREGURL, strlen(DEVREGURL) - 1) == 0)
	{
		DEBUGP("LogicURL\n");
		memset(pstConfig.LogicURL, '\0', 256);
		if (strlen(buf) == (strlen(DEVREGURL)+1) || \
			strlen(buf) == (strlen(DEVREGURL)+2))
		{
			pstConfig.device_enable[PM_LOGIC_ID] = 0;
			goto back;
		}
		pstConfig.device_enable[PM_LOGIC_ID] = 1;
		memcpy(pstConfig.LogicURL, buf + strlen(DEVREGURL),\
			strlen(buf) - strlen(DEVREGURL));
		if (pstConfig.LogicURL[strlen(pstConfig.LogicURL) - 1] == '\n')
			pstConfig.LogicURL[strlen(pstConfig.LogicURL) - 1] = '\0';
		goto back;
	}

	/* 接收Ethlan模块发过来的Down消息执行MAC列表维护*/
	if(memcmp(buf,DEL_MAC ,strlen(DEL_MAC)) == 0)
	{
		memcpy(iFname, buf + strlen(DEL_MAC),strlen(buf) - strlen(DEL_MAC));
		if(sscanf(iFname, "vport%d", &port_no))
		{
			if(br_fdb_ifname_get_mac("br0",port_no,&pm_mac_haed))
			{
				Del_mac_process();
			}
			else
			{
				DEBUGP("fine vport%d mac fail!\n",port_no);
			}
		}
	}

	/* 设置强制记录闲置超时时间*/
#ifdef TIMEOUT_ENABLE			
	if (memcmp(buf, PM_TIMEOUT,strlen(PM_TIMEOUT)) ==0)
	{
		sscanf(buf+ strlen(PM_TIMEOUT),"%d",&pstConfig.timeout);
		printk(KERN_INFO"TimeOut =%d\n",pstConfig.timeout);
		pstConfig.timeout = pstConfig.timeout*1000;
		goto back;
	}
#endif

	/* 打开强制门户功能*/
	if (memcmp(buf,CMD_ON,strlen(CMD_ON)) == 0)
	{
		atomic_set(&pstConfig.EnableForce, 1);
		printk(KERN_INFO"PortalManage Enable\n");
		goto back;
	}

	/* 关闭强制门户功能*/
	if (memcmp(buf, CMD_OFF,strlen(CMD_OFF)) ==0)
	{
		atomic_set(&pstConfig.EnableForce,0);
		printk(KERN_INFO"PortalManage Disable\n");
		goto back;
	}

	//inform that internet is ok now
	if (memcmp(buf,INTERNET_OK,strlen(INTERNET_OK)) == 0)
	{
		atomic_set(&pstConfig.internet_not_ok, 0);
		printk(KERN_INFO"PortalManage Enable\n");
		goto back;
	}

	//inform that internet is not ok now
	if (memcmp(buf,INTERNET_NOT_OK,strlen(INTERNET_NOT_OK)) == 0)
	{
		atomic_set(&pstConfig.internet_not_ok, 1);
		printk(KERN_INFO"PortalManage Enable\n");
		goto back;
	}

	//is first perform
	if (memcmp(buf,FIRST_PERFORM ,strlen(FIRST_PERFORM)) == 0)
	{
		atomic_set(&pstConfig.first_perform, 1);
		printk(KERN_INFO"PortalManage Enable\n");
		goto back;
	}

	if (memcmp(buf, NOT_FIRST_PERFORM ,strlen(NOT_FIRST_PERFORM)) == 0)
	{
		atomic_set(&pstConfig.first_perform, 0);
		printk(KERN_INFO"PortalManage Enable\n");
		goto back;
	}

	//is pre-gui aqui button flag
	if (memcmp(buf, "aqui_flag_ok" ,strlen("aqui_flag_ok")) == 0)
	{
		atomic_set(&pstConfig.aqui_flag, 1);
		printk(KERN_INFO"PortalManage Enable\n");
		goto back;
	}

	if (memcmp(buf, "aqui_flag_nok" ,strlen("aqui_flag_nok")) == 0)
	{
		atomic_set(&pstConfig.aqui_flag, 0);
		printk(KERN_INFO"PortalManage Enable\n");
		goto back;
	}
	  
	/* LOGIC ID PortaManage enable*/
	if (memcmp(buf, DEVREGON,strlen(DEVREGON)) ==0)
	{
		atomic_set(&pstConfig.En_logicID,1);
		register_result = 1;
		printk(KERN_INFO"LOIGIC ID PortaManage Enable\n");
		goto back;
	}

	/* LOGIC ID PortaManage disable*/
	if (memcmp(buf, DEVREGOFF,strlen(DEVREGOFF)) ==0){
		atomic_set(&pstConfig.En_logicID,0);
		register_result = 0;
		printk(KERN_INFO"LOIGIC ID PortaManage Disable\n");
		goto back;
	}

	/* 清除强制MAC记录命令处理*/
	if (memcmp(buf,CMD_FLUSH,strlen(CMD_FLUSH)) == 0)
	{
		LOCK_BH(&pm_dev_list_lock);
		list_for_each_safe(cur_item, next, &pm_dev_list) { 
			record = list_entry(cur_item, ST_PM_DEVINFO, list);
			list_del(&record->list);
			kfree(record);
		}
		UNLOCK_BH(&pm_dev_list_lock);	
		printk(KERN_INFO"Flush Success\n");
		goto back;
	}

	/* 显示强制门户状态信息 */
	if (memcmp(buf, CMD_QUERY, strlen(CMD_QUERY)) == 0)
	{
		printk(KERN_EMERG"====================================================================\n");
		LOCK_BH(&pm_dev_list_lock);
		list_for_each_safe(cur_item, next, &pm_dev_list) { 
			record = list_entry(cur_item, ST_PM_DEVINFO, list);
			printk(KERN_INFO"the mac is %02x-%02x-%02x-%02x-%02x-%02x,  the state is %s, the type is %s\n", 
					record->mac[0]&0xff,
					record->mac[1]&0xff,
					record->mac[2]&0xff,
					record->mac[3]&0xff,
					record->mac[4]&0xff,
					record->mac[5]&0xff,
					record->iFirstChk== 1 ? "checked" : "unchecked",
					record->devtype == 1 ? "Computer": (record->devtype ==3 ? "STB": "Phone"));
		}
		UNLOCK_BH(&pm_dev_list_lock);	
#ifdef TIMEOUT_ENABLE
		printk(KERN_INFO"the enable is %d timeout is %ds\n", atomic_read(&pstConfig.EnableForce),
			pstConfig.timeout/1000);
#endif
		printk(KERN_INFO"the Computer direct is %s, %s\n", pstConfig.pPortalPC, 
			pstConfig.device_enable[PM_DEV_PC] == 1 ? "Enable" : "Disable");
		printk(KERN_INFO"the STB direct is %s, %s\n", pstConfig.pPortalSTB, 
			pstConfig.device_enable[PM_DEV_STB] == 1 ? "Enable" : "Disable");
		printk(KERN_INFO"the Phone direct is %s, %s\n", pstConfig.pPortalPhone,
			pstConfig.device_enable[PM_DEV_PHONE] == 1 ? "Enable" : "Disable");
		printk(KERN_INFO"the Logic ID is %s, %s\n", pstConfig.LogicURL,
			pstConfig.device_enable[PM_LOGIC_ID] == 1 ? "Enable" : "Disable");
		printk(KERN_INFO"internet is now %s\n",
			atomic_read(&pstConfig.internet_not_ok) == 1 ? "not ok" : "ok");
		printk(KERN_INFO"is %s first perform\n",
			atomic_read(&pstConfig.first_perform) == 1 ? "" : "not");
		printk(KERN_INFO"====================================================================\n");
	}

back:
	return count;
}
/*=======================================================================
  钩子挂载NF_INET_PRE_ROUTING
  备注：使用NF_INET_PRE_ROUTING hook点时，访问CPE http也会触发强制门户，与
  电信E8需求有不同
=======================================================================*/
static struct nf_hook_ops pm_hook_ops = {
	.hook		= ip_pm_in,
	.owner		= THIS_MODULE,
	.pf		    = PF_INET,
	.hooknum	= NF_INET_PRE_ROUTING,
	.priority	= NF_IP_PRI_CONNTRACK + 1,
};

static void PM_Kernel_ModuleDestory(void);
/*======================================================================= 
  功能: 加载强制门户模块	         
  参数: 无   
  返回: 无    
  备注:                                                          
=======================================================================*/
static int __init PM_Kernel_ModuleInit(void)
{
	int iRet = 0;
	struct proc_dir_entry *PM_DevUrl;

	printk(KERN_INFO"PortalManage Module: Load ...\n");

	iRet = nf_register_hooks(&pm_hook_ops, 1);
	if (iRet < 0) {
		DEBUGP("PortalManage Module: Can't Register Hooks.\n");
		goto out;
	}

	/* 创建pm_devurl文件 /proc/pm_devurl*/
	PM_DevUrl = create_proc_entry("pm_devurl", 0, NULL);
	if (PM_DevUrl)
	{
		/* 注册文件的写处理函数 */
		PM_DevUrl->write_proc = write_dev_url;
		PM_DevUrl->read_proc = read_dev_url;
		//PM_DevUrl->owner = THIS_MODULE;
	}

	LOCK_BH(&pm_dev_list_lock);
	INIT_LIST_HEAD(&pm_dev_list);
	UNLOCK_BH(&pm_dev_list_lock);
	INIT_LIST_HEAD(&pm_mac_haed);
#ifdef TIMEOUT_ENABLE
	/*初始化超时参数*/
	pm_init_timer(TIMER_VALUE);
	pstConfig.timeout = TO_MACVALUE;
#endif
	atomic_set(&pstConfig.En_logicID, 0);
	atomic_set(&pstConfig.internet_not_ok, 0);//先让它OK，免得意外时访问不了外网
	atomic_set(&pstConfig.first_perform, 0);
	register_result = 0;
	printk(KERN_INFO"PortalManage Module: Load Success\n");

out:
	if (iRet != 0) {
		PM_Kernel_ModuleDestory();
	}
	return(0);
}
/*=======================================================================      
  功能: 缷载强制门户模块    
  参数: 无             
  返回: 无
  备注:                                                             
=======================================================================*/
static void PM_Kernel_ModuleDestory(void)
{
	struct list_head *cur_item,*next;
	ST_PM_DEVINFO *psDevInfo;

	printk(KERN_INFO"PortalManage Module: UnLoad\n");

	remove_proc_entry("pm_devurl", NULL);
	nf_unregister_hooks(&pm_hook_ops,1);

	LOCK_BH(&pm_dev_list_lock);
	list_for_each_safe(cur_item, next, &pm_dev_list) { 
		psDevInfo = list_entry(cur_item, ST_PM_DEVINFO, list);
		list_del(&psDevInfo->list);
		kfree(psDevInfo);
	}

#ifdef TIMEOUT_ENABLE
	del_timer(&pm_timeout);
#endif
	UNLOCK_BH(&pm_dev_list_lock);	

	printk(KERN_INFO"PortalManage Module: UnLoad Success\n");
}
MODULE_LICENSE("GPL");
MODULE_AUTHOR("chenzanhui <chenzanhui@twsz.com>");
MODULE_DESCRIPTION("portal manage module");

module_init(PM_Kernel_ModuleInit);
module_exit(PM_Kernel_ModuleDestory);
/******************************************************************************
*                                 END                                        *
******************************************************************************/
