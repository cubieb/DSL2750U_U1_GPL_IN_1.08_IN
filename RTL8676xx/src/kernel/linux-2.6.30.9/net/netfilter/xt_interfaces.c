/*
 *	xt_interfaces - Netfilter module to match NFMARK value
 *
 *	(C) 1999-2001 Marc Boucher <marc@mbsi.ca>
 *	Copyright © CC Computer Consultants GmbH, 2007 - 2008
 *	Jan Engelhardt <jengelh@computergmbh.de>
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2 as
 *	published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/skbuff.h>

#include <linux/netfilter/xt_interfaces.h>
#include <linux/netfilter/x_tables.h>
#include <linux/route.h>
#include <net/route.h>
#include <net/xfrm.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Stefen Huang, Yanbing Deng");
MODULE_DESCRIPTION("Xtables: packet interfaces match");


static bool
interfaces_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
    const struct xt_interfaces_info *info = par->matchinfo;
    struct rtable * rtble=skb->rtable;
		
    if ((info == NULL) || (skb == NULL) || (rtble == NULL))
        return 0;
            
    if((skb->dev != NULL) && (skb->dev->flags&IFF_LOOPBACK))
        return 0;

    if(info->interfaces == WAN_INTERFACES ) //wan
    {       
            if((info->direction&IN_DIR) && (skb->dev!=NULL)&& (skb->dev->priv_flags&IFF_DOMAIN_WAN))   // wan in
                {
                    return 1;
                } 
            else if((info->direction&OUT_DIR) &&(rtble->u.dst.dev!=NULL)&& (rtble->u.dst.dev->priv_flags&IFF_DOMAIN_WAN)) //wan out
                {    
                    return 1;
                } 
            else 
                {     
                    return 0;
                }
     }
     else if(info->interfaces == LAN_INTERFACES) //lan
     {
         if((info->direction&IN_DIR) &&(skb->dev!=NULL) &&  !(skb->dev->priv_flags&IFF_DOMAIN_WAN))  //lan in
         {       
             return 1;
         }
         else if((info->direction&OUT_DIR) && (rtble->u.dst.dev!=NULL) && !(rtble->u.dst.dev->priv_flags&IFF_DOMAIN_WAN)) //lan out
         {      
             return 1;
         }
         else 
         {       
             return 0;
         }
      }
      else if (info->interfaces == WAN_LAN)//all wan & lan 
      {
    	    if((info->direction&IN_DIR) && (skb->dev!=NULL)&& (skb->dev->priv_flags&IFF_DOMAIN_WAN))   // wan in
          {
              return 1;
          }
          else if((info->direction&OUT_DIR) &&(rtble->u.dst.dev!=NULL)&& (rtble->u.dst.dev->priv_flags&IFF_DOMAIN_WAN)) //wan out
          {    
              return 1;
          }
          else if((info->direction&IN_DIR) &&(skb->dev!=NULL) &&  !(skb->dev->priv_flags&IFF_DOMAIN_WAN))  //lan in
          {       
              return 1;
          }
          else if((info->direction&OUT_DIR) && (rtble->u.dst.dev!=NULL) && !(rtble->u.dst.dev->priv_flags&IFF_DOMAIN_WAN)) //lan out
          {      
              return 1;
          }
          else 
          {       
             return 0;
          }
      }	 
      else  //specific lan
      {
      	  /*match lan devname*/	
		  if ((info->name[0] != '\0'))
		  {
		      //match lan/wlan
    		  if ((skb->switch_port != NULL) && (0 == strncmp(skb->switch_port, info->name, strlen(skb->switch_port))))
    		  {
              	  if((info->direction&IN_DIR) &&(skb->dev!=NULL) &&  !(skb->dev->priv_flags&IFF_DOMAIN_WAN))  //lan in
                  {       
                      return 1;
                  }
                  else if((info->direction&OUT_DIR) && (rtble->u.dst.dev!=NULL) && !(rtble->u.dst.dev->priv_flags&IFF_DOMAIN_WAN)) //lan out
                  {      
                      return 1;
                  }
                  else 
                  {       
                      return 0;
                  }
    		  }
		  }
		  return 0;
      }	 
      return 0;
}

static bool interfaces_mt_check(const struct xt_mtchk_param *par)
{
	const struct xt_interfaces_info *minfo = par->matchinfo;

	if (minfo->direction !=OUT_DIR && minfo->direction!=IN_DIR && minfo->direction!=BOTH_DIR) 
	{
		printk(KERN_WARNING "interfaces: direction:%d wrong\n",minfo->direction);
		return false;
	}

	if((minfo->interfaces != WAN_INTERFACES) 
		&& (minfo->interfaces != LAN_INTERFACES) 
		&& (minfo->interfaces != WAN_LAN)
		&& ((minfo->interfaces == SPECIFIC_LAN) && (minfo->name[0] == '\0')))
    {
        printk(KERN_WARNING "interfaces: interface wrong\n");
        return false;
    }
	return true;
}

static struct xt_match interfaces_mt_reg  __read_mostly = {
		.name		= "interfaces",
		.revision	= 0,
		.family		= NFPROTO_UNSPEC,
		.checkentry	= interfaces_mt_check,
		.match		= interfaces_mt,
		.matchsize	= sizeof(struct xt_interfaces_info),
		.me		= THIS_MODULE,
};

static int __init interfaces_mt_init(void)
{
	return xt_register_match(&interfaces_mt_reg);
}

static void __exit interfaces_mt_exit(void)
{
	xt_unregister_match(&interfaces_mt_reg);
}

module_init(interfaces_mt_init);
module_exit(interfaces_mt_exit);

