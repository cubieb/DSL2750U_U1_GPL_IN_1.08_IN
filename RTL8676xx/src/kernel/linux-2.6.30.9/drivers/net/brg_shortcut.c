#include "brg_shortcut.h"
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/crc32.h>
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/proc_fs.h>
#ifdef CONFIG_RTL_IPTABLES_RULE_2_ACL
#include <linux/netfilter_ipv4/ip_tables.h>
#endif
#ifdef CONFIG_RTL_MULTI_ETH_WAN
#include <linux/if_smux.h>
#endif

#ifdef CONFIG_VLAN_MODE
#include <net/br_vlan.h>
#endif

//#include <linux/imq.h>


//#define CONFIG_RTL8676_ETHSKB_CONTROL_POOL
//#define CONFIG_RTK_ETHUP 

#ifdef CONFIG_RTL8676_ETHSKB_CONTROL_POOL
extern int net_smallpkt_heavytraffic;
#endif
#ifdef CONFIG_RTL8676_BRIDGE_FASTPATH
#define MAX_BRG_SC_ENTRY_NUM	8




int BRG_ENTRY_AGING_TIME =BRG_ENTRY_AGING_TIME_NORMAL;
//int BRG_ENTRY_FORCE_TIMEOUT=BRG_ENTRY_FORCE_TIMEOUT_NORMAL;



typedef struct brg_shortcut_entry {
	unsigned char enable;
	unsigned short macPair[6];
	struct net_device *srcDev;
	struct net_device *dstDev;
	unsigned int mark;//for IP QoS
	unsigned long tick;
}BRG_SHORTCUT_ENTRY;


#ifdef CONFIG_RTL8676
__DRAM
#endif 
static BRG_SHORTCUT_ENTRY fbTbl[MAX_BRG_SC_ENTRY_NUM]; // for downstream


#ifdef CONFIG_RTL8676
__DRAM
#endif 
static BRG_SHORTCUT_ENTRY fbTblup[MAX_BRG_SC_ENTRY_NUM]; //for upstream


static unsigned int brg_shortcut_enable = 1;
static unsigned int brg_shortcut_upload_forbit = 1;
static unsigned char disport[10] = {0};

void brgClearTableByDev(struct net_device *dev)
{
	BRG_SHORTCUT_ENTRY *pfbTbl;
	int i;
	
	pfbTbl=fbTblup;
	for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
	{
		if ((pfbTbl[i].srcDev->name[0]==dev->name[0]) || (pfbTbl[i].dstDev->name[0]==dev->name[0]))
			pfbTbl[i].enable=0;
	}
	pfbTbl=fbTbl;
	for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
	{
		if ((pfbTbl[i].srcDev->name[0]==dev->name[0]) || (pfbTbl[i].dstDev->name[0]==dev->name[0]))
			pfbTbl[i].enable=0;
	}
}

/*when src mac learned by a new interface, all related brgSC entry should be deleted*/
int brgScDelete(unsigned char *mac)
{
	BRG_SHORTCUT_ENTRY *pfbTbl;
	unsigned short *pmac = (unsigned short *)mac;
	unsigned short *smac, *dmac;
	//unsigned long tick_tmp=0;
	int i;

	pfbTbl = fbTblup;
DEL:
	for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
	{
		if (!pfbTbl[i].enable)
			continue;

			smac = &pfbTbl[i].macPair[3];
			dmac = &pfbTbl[i].macPair[0];
			if (!((pmac[0]^smac[0])|(pmac[1]^smac[1])|(pmac[2]^smac[2])) ||
				!((pmac[0]^dmac[0])|(pmac[1]^dmac[1])|(pmac[2]^dmac[2]))){
				pfbTbl[i].enable = 0;
		}
	}

	if (pfbTbl == fbTblup) {
		pfbTbl = fbTbl;
		goto DEL;
	}

	return 0;
}


/* Kevin, when the skb pass through br_flood, it cannot record rest. if , so delete the entry */

void brgEntryDelete(unsigned short *s_mac,unsigned short *d_mac,int dir)
{
    BRG_SHORTCUT_ENTRY *pfbTbl;
    unsigned short *smac, *dmac;
    int i;

    if (dir == DIR_LAN)
		pfbTbl = fbTblup;
	else
		pfbTbl = fbTbl;


    for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
	{
		if (!pfbTbl[i].enable)
			continue;

		smac = &pfbTbl[i].macPair[3];
		dmac = &pfbTbl[i].macPair[0];
		if (!((s_mac[0]^smac[0])|(s_mac[1]^smac[1])|(s_mac[2]^smac[2])) &&
			!((d_mac[0]^dmac[0])|(d_mac[1]^dmac[1])|(d_mac[2]^dmac[2])))
			pfbTbl[i].enable = 0;		
	}


}



/*
 * dst: source device
 * mac: source mac
 */
int brgScFind(struct net_device *dst, unsigned char *mac, unsigned long *tick)
{
	BRG_SHORTCUT_ENTRY *pfbTbl;
	unsigned short *pmac = (unsigned short *)mac;
	unsigned short *smac;
	unsigned long tick_tmp=0;
	int i;

	pfbTbl = fbTblup;
FIND:
	for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
	{
		if (!pfbTbl[i].enable)
			continue;

		if (pfbTbl[i].srcDev->name[0] == dst->name[0]) {
			smac = &pfbTbl[i].macPair[3];
			if (!((pmac[0]^smac[0])|(pmac[1]^smac[1])|(pmac[2]^smac[2]))){
				if ((jiffies-pfbTbl[i].tick) > BRG_ENTRY_AGING_TIME) {
					pfbTbl[i].enable = 0;
					continue;
		}

				if (tick_tmp == 0)
					tick_tmp = pfbTbl[i].tick;
				else
					tick_tmp = (tick_tmp >= pfbTbl[i].tick)?tick_tmp:pfbTbl[i].tick;
			}
			}
		}

	if (tick_tmp) {
		*tick = tick_tmp;
				return 1;
			}

	if (pfbTbl == fbTblup) {
		pfbTbl = fbTbl;
		goto FIND;
	}

	return 0;
}

//rx will learn source mac and destination mac, and tx will learn related destination interface
/*****************************************************************
** NAME: brgShortcutLearnMac
** PARA: pMac - ethernet header, that is DA/SA
              srcDev - packet receive device.
** RETURN: void
*****************************************************************/
static void  brgShortcutLearnMac(unsigned short *pMac, struct net_device *srcDev, int dir)
{
	BRG_SHORTCUT_ENTRY *pfbTbl;
	int i, index=-1, selected=0;
	unsigned long maxelapse=0;

	if (dir == DIR_LAN)
		pfbTbl = fbTblup;
	else
		pfbTbl = fbTbl;
	for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
	{
		if (pfbTbl[i].enable)
		{
			unsigned short *pmac = pfbTbl[i].macPair;
			if (!((pmac[0]^pMac[0])|(pmac[1]^pMac[1])|(pmac[2]^pMac[2])|(pmac[3]^pMac[3])|(pmac[4]^pMac[4])|(pmac[5]^pMac[5])))
			{
				if(pfbTbl[i].srcDev != srcDev)
				{
					pfbTbl[i].srcDev = srcDev;
					pfbTbl[i].dstDev = NULL;
					pfbTbl[i].tick = jiffies;
				}
				return;
			}

			if ((maxelapse==0) || (time_after_eq(maxelapse, pfbTbl[i].tick))){
				maxelapse = pfbTbl[i].tick;
				selected = i;
			}
		}
		else{
			if (index == -1)
				index = i;
		}
	}

	index = index<0?selected:index;
	pfbTbl[index].macPair[0] = pMac[0];
	pfbTbl[index].macPair[1] = pMac[1];
	pfbTbl[index].macPair[2] = pMac[2];
	pfbTbl[index].macPair[3] = pMac[3];
	pfbTbl[index].macPair[4] = pMac[4];
	pfbTbl[index].macPair[5] = pMac[5];
	pfbTbl[index].srcDev = srcDev;
	pfbTbl[index].dstDev = NULL;
	pfbTbl[index].tick = jiffies;
	pfbTbl[index].enable = 1;

	
}

/*****************************************************************
** NAME: brgShortcutGetEntry
** PARA: pMacPair - ethernet header, that is DA/SA
              srcDev - packet receive device.
** RETURN: bridge shortcut entry
*****************************************************************/
__IRAM BRG_SHORTCUT_ENTRY * brgShortcutGetEntry(unsigned short *pMacPair, struct net_device *srcDev, int dir)
{
	BRG_SHORTCUT_ENTRY *pfbTbl;
	unsigned short *pmac;
	int i;
	BRG_SHORTCUT_ENTRY * pEntry = NULL;
	
	if (dir == DIR_LAN)
		pfbTbl = fbTblup;
	else
		pfbTbl = fbTbl;
	for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
	{
		if (pfbTbl[i].enable && pfbTbl[i].dstDev && (pfbTbl[i].srcDev->name[0]==srcDev->name[0]))
		{
			{
#ifdef CONFIG_RTL8676_ETHSKB_CONTROL_POOL			
				if(!net_smallpkt_heavytraffic)
#endif					
				{
					if((jiffies-pfbTbl[i].tick) > BRG_ENTRY_AGING_TIME){
						pfbTbl[i].enable = 0;
						continue;
					}
				}	
			}
			pmac = pfbTbl[i].macPair;
			if ( !((pmac[0]^pMacPair[0])|(pmac[1]^pMacPair[1])|(pmac[2]^pMacPair[2])|(pmac[3]^pMacPair[3])|(pmac[4]^pMacPair[4])|(pmac[5]^pMacPair[5])) 
				&& !strcmp(pfbTbl[i].srcDev->name,srcDev->name) )
			{
				//get entry, update age time.				
				pfbTbl[i].tick = jiffies;
				pEntry = pfbTbl+i;
				break;
			}
		}
	}

	return pEntry;
}






int debugBridge;
/*****************************************************************
** NAME: brgShortcutLearnDestItf
** PARA: skb - transmit packet
              dstDev -packet transmit device.
** RETURN: 
*****************************************************************/
void brgShortcutLearnDestItf(struct sk_buff *skb, struct net_device *dstDev)
{
	BRG_SHORTCUT_ENTRY *pfbTbl;
	int i;
	unsigned short *pMacPair,*pmac;

	if(brg_shortcut_enable){
		pmac = (unsigned short *)skb->data;

		if((*(unsigned char *)pmac) & 0x1)	/*not support multicast*/
			return;

		pfbTbl = fbTblup;

FIND:
		for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
		{
			if (pfbTbl[i].enable)
			{			
				pMacPair = pfbTbl[i].macPair;			
				if (!((pmac[0]^pMacPair[0])|(pmac[1]^pMacPair[1])|(pmac[2]^pMacPair[2])|(pmac[3]^pMacPair[3])|(pmac[4]^pMacPair[4])|(pmac[5]^pMacPair[5])))
				{ 
					if ( (dstDev->priv_flags & IFF_DOMAIN_WAN) && brg_shortcut_upload_forbit)
					{
					       pfbTbl[i].enable = 0;
						return;
					}
					
                                   if(skb->floodflag ==PACTET_FLOOD) 
                                   {
                                        pfbTbl[i].enable = 0;
                                        return;
                                   }
												
					pfbTbl[i].tick = jiffies;
					pfbTbl[i].dstDev = dstDev;					
					#if defined (CONFIG_RTL_IPTABLES_RULE_2_ACL) && defined(CONFIG_RTL8676_Dynamic_ACL)

					/* upstream , if  the pkts from someone LAN dev only forward to only one WAN dev (ex. port mapping or there is only one bridged interface)
                                                                    we set L2 permit rule at WAN interface (because the pvid of LAN port is WAN's vid)
					*/
					if( (pfbTbl[i].srcDev->priv_flags & IFF_DOMAIN_ELAN) && dstDev->name[0] == 'n' )
					{
						unsigned char *input_mac = (unsigned char *)pfbTbl[i].macPair;
						#ifdef CONFIG_RTL_MULTI_ETH_WAN		
						if (isBrgWandevPortmappingEnable(dstDev) || isSrcDevAndDstDevInSameGroup(pfbTbl[i].srcDev, dstDev))
						#endif
							rtl865x_add_filter_L2_permit(&input_mac[6],&input_mac[0], dstDev->name);

						#ifdef CONFIG_RTL_MULTI_ETH_WAN
						//rtl865x_add_filter_L2_redirect(&input_mac[6],&input_mac[0], pfbTbl[i].srcDev->name, pfbTbl[i].dstDev->name);
						#endif
					}

					/*  downstream  */
					else if( pfbTbl[i].srcDev->name[0] == 'n' && (dstDev->priv_flags & IFF_DOMAIN_ELAN))
					{
						unsigned char *input_mac = (unsigned char *)pfbTbl[i].macPair;
						rtl865x_add_filter_L2_permit(&input_mac[6],&input_mac[0], pfbTbl[i].srcDev->name);

						#ifdef CONFIG_RTL_MULTI_ETH_WAN
						//rtl865x_add_filter_L2_redirect(&input_mac[6],&input_mac[0], pfbTbl[i].srcDev->name, pfbTbl[i].dstDev->name);
						#endif
					}

					#if 0
					if (((pfbTbl[i].srcDev->priv_flags & IFF_DOMAIN_ELAN) && (dstDev->name[0] == 'n')) ||
						 ((pfbTbl[i].srcDev->name[0] == 'n') && (dstDev->priv_flags & IFF_DOMAIN_ELAN))) {
						unsigned char *input_mac = (unsigned char *)pfbTbl[i].macPair;
						#ifdef CONFIG_RTL_MULTI_ETH_WAN
						if (dstDev->name[0] == 'n')//upstream
						{
							if (isBrgWandevPortmappingEnable(dstDev))
								rtl865x_add_filter_L2_permit(&input_mac[6],&input_mac[0], dstDev->name);
							else {
								if (isSrcDevAndDstDevInSameGroup(pfbTbl[i].srcDev, dstDev))
									rtl865x_add_filter_L2_permit(&input_mac[6],&input_mac[0], pfbTbl[i].srcDev->name);
							}
						}
						else//downstream
							rtl865x_add_filter_L2_permit(&input_mac[6],&input_mac[0], pfbTbl[i].srcDev->name);
						#else
						rtl865x_add_filter_L2_permit(&input_mac[6],&input_mac[0], pfbTbl[i].srcDev->name);
						#endif
						#ifdef CONFIG_RTL_MULTI_ETH_WAN
						//rtl865x_add_filter_L2_redirect(&input_mac[6],&input_mac[0], pfbTbl[i].srcDev->name, pfbTbl[i].dstDev->name);
						#endif
					}
					#endif

					//add l2 permit acl for Lan<->Lan traffic
					if( (pfbTbl[i].srcDev->priv_flags & IFF_DOMAIN_ELAN) && (dstDev->priv_flags & IFF_DOMAIN_ELAN))
					{
						unsigned char *input_mac = (unsigned char *)pfbTbl[i].macPair;
						rtl865x_add_filter_L2_permit(&input_mac[6],&input_mac[0], RTL_DRV_LAN_NETIF_NAME);
					}
					#endif
					
					return;
				}
			}
		}

       
        if(pfbTbl== fbTblup)
        {
            pfbTbl = fbTbl;
            goto FIND;
        }
        
	}
}
EXPORT_SYMBOL(brgShortcutLearnDestItf);

/*****************************************************************
** NAME: brgShortcutProcess
** PARA: skb - transmit packet
		srcDev -packet receive device.
** RETURN: 1 - go through bridge shortcut.
		    0 - transfer to upper layer
*****************************************************************/
__IRAM_NIC int brgShortcutProcess(struct sk_buff *skb, struct net_device *srcDev, int dir)
{
	BRG_SHORTCUT_ENTRY *pFbEntry;
	unsigned short *pMacHdr;
	unsigned long flags;
	struct net_device *indev;

	if(*(unsigned char *)skb->data & 0x1) /*not support multicast packet*/
		return 0;

	local_irq_save(flags);
	
	if(brg_shortcut_enable){
		pMacHdr = (unsigned short *)skb->data;
		if(pMacHdr[6] == 0x8100){	/*not support vlan packet*/
			local_irq_restore(flags);
			return 0;
		}
        
		if((pFbEntry = brgShortcutGetEntry(pMacHdr, srcDev, dir)))
		{
			if(memcmp(disport,pFbEntry->dstDev->name,6) == 0){
				return 0;
			}
    		indev = skb->dev;
			skb->dev = pFbEntry->dstDev;
			if(pFbEntry->dstDev->hard_start_xmit(skb,pFbEntry->dstDev))
			{
				pFbEntry->enable = 0;
				dev_kfree_skb(skb);
				printk("fast bridge tx error!\n");
			}
			local_irq_restore(flags);
			return 1;
		}
		else
			brgShortcutLearnMac(pMacHdr, srcDev, dir);
	}

	local_irq_restore(flags);
	return 0;
}


void brgClearTable(void)
{
	BRG_SHORTCUT_ENTRY *pfbTbl;
	int i;
	pfbTbl=fbTblup;
	
	for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
	{
		pfbTbl[i].enable=0;
	}
	pfbTbl=fbTbl;
	for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
	{
		pfbTbl[i].enable=0;
	}
	BRG_ENTRY_AGING_TIME =BRG_ENTRY_AGING_TIME_NORMAL;
 	//BRG_ENTRY_FORCE_TIMEOUT=BRG_ENTRY_FORCE_TIMEOUT_NORMAL;

	
}


static int fastbridge_read_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
	int i;
	char srcportbuf[16];
	char dstportbuf[16];

	printk("\nrtk fast bridge is %s\n",brg_shortcut_enable?"enabled":"disabled");
	printk("************************fast bridge table***************************\n");
	printk("Index   Enabled   TimeOut   DstMac           SrcMac           SrcItf     DstItf    MARK   DIR\n");
	for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
	{
		if (fbTbl[i].enable){
			if (fbTbl[i].srcDev)
				sprintf(srcportbuf,"%s",fbTbl[i].srcDev->name);
			else
				sprintf(srcportbuf,"---");
			
			if(fbTbl[i].dstDev){
				sprintf(dstportbuf,"%s",fbTbl[i].dstDev->name);
			}else{
				sprintf(dstportbuf,"---");
			}
			printk("%-5d   %-7d   %-7d   %04x:%04x:%04x   %04x:%04x:%04x   %-10s   %-9s   %-6u  DOWN\n",
				i,fbTbl[i].enable,(jiffies-fbTbl[i].tick)>BRG_ENTRY_AGING_TIME?1:0,fbTbl[i].macPair[0],fbTbl[i].macPair[1],fbTbl[i].macPair[2],
				fbTbl[i].macPair[3],fbTbl[i].macPair[4],fbTbl[i].macPair[5],srcportbuf,dstportbuf, fbTbl[i].mark);
		}
	}
	for (i=0; i<MAX_BRG_SC_ENTRY_NUM; i++)
	{
		if (fbTblup[i].enable){
			if (fbTblup[i].srcDev)
				sprintf(srcportbuf,"%s",fbTblup[i].srcDev->name);
			else
				sprintf(srcportbuf,"---");
			
			if(fbTblup[i].dstDev){
				sprintf(dstportbuf,"%s",fbTblup[i].dstDev->name);
			}else{
				sprintf(dstportbuf,"---");
			}
			printk("%-5d   %-7d   %-7d   %04x:%04x:%04x   %04x:%04x:%04x   %-10s   %-9s  %10x  UP\n",
				i,fbTblup[i].enable,(jiffies-fbTblup[i].tick)>BRG_ENTRY_AGING_TIME?1:0,fbTblup[i].macPair[0],fbTblup[i].macPair[1],fbTblup[i].macPair[2],
				fbTblup[i].macPair[3],fbTblup[i].macPair[4],fbTblup[i].macPair[5],srcportbuf,dstportbuf, fbTblup[i].mark);
		}
	}
	
	return 0;
}


static int fastbridge_write_proc(struct file *file, const char *buffer,	unsigned long count, void *data)
{
	unsigned char flag;

	if (count < 2)
		return -EFAULT;
	
	if (buffer && !copy_from_user(&flag, buffer, 1)) {
		if(flag == '0')
		{
			brg_shortcut_enable = 0;
            printk("rtk fast bridge is disabled\n");
		}
		else if(flag == '1')
		{
			brg_shortcut_enable = 1;
			brg_shortcut_upload_forbit = 1;
            printk("rtk fast bridge down is enabled\n");
		}
		else if(flag == '2')
		{
			brg_shortcut_enable = 1;
			brg_shortcut_upload_forbit = 0;
			printk("rtk fast bridge up and down is enabled\n");
		}
		else if(flag == '3')
		{                    
            brgClearTable();
            printk("clean rtk fast bridge table\n");     
		}
		return count;
	}else
		return -EFAULT;
}

static int fastbridge_disport_read_proc(char *page, char **start, off_t off,
				int count, int *eof, void *data)
{
    printk("%s\n",disport);
	
	return 0;
}
/********************************************************
** NAME: fastbridge_disport_write_proc
** PARA: file - "fastbridge_disport"
		 buffer - user data
		 count - user data sizer
********************************************************/
static int fastbridge_disport_write_proc(struct file *file, const char *buffer,	unsigned long count, void *data)
{
	if (count < 2){
		if(disport[0] != '0')
			memset(disport,0,sizeof(disport));
		return -EFAULT;
	}
	
	if (disport[0] == 'v'){
		return -EFAULT;
	}
	
	if(count > sizeof(disport)){
		count = sizeof(disport);
	}
	if (buffer && !copy_from_user(disport, buffer, count-1)){
		brgClearTable();
        printk("clean rtk fast bridge table\n"); 
		return count;
	}else
		return -EFAULT;
}
static int __init fastbridge_init(void) 
{

	struct proc_dir_entry *entry=NULL;	
	struct proc_dir_entry *entry1=NULL;	
	entry = create_proc_entry("fastbridge", 0, NULL);
	if (entry) {
		entry->read_proc = fastbridge_read_proc;
		entry->write_proc = fastbridge_write_proc;
	}
	else {
		printk("Realtek fastbridge, create proc failed!\n");
	}

	entry1 = create_proc_entry("fastbridge_disport", 0, NULL);
	if (entry1) {
		entry1->write_proc = fastbridge_disport_write_proc;
		entry1->read_proc = fastbridge_disport_read_proc;
	}
	else {
		printk("Realtek fastbridge, create proc(fastbridge_disport) failed!\n");
	}

	return 0;
}

static void __exit fastbridge_exit(void) 
{

}

module_init(fastbridge_init);
module_exit(fastbridge_exit);

#endif

