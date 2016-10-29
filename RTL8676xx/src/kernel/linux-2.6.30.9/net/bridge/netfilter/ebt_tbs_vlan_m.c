/*
 * Description: 802.1Q Virtual LAN match support module specialy for tbs,
 * in tbs, we add 'vlan' field in struct sk_buff for easily handling packet 
 * that it's vlan tag has been stripped by vlan protocol handler(vlan_skb_recv).
 *
 * Authors: XuXiaodong  borrowed heavily from ebt_vlan_m.c
 * 2009-03-26
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/if_ether.h>
#include <linux/if_vlan.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/netfilter_bridge/ebtables.h>
#include <linux/netfilter_bridge/ebt_tbs_vlan_m.h>
#include <linux/netfilter/x_tables.h>


#define GET_BITMASK(_BIT_MASK_) info->bitmask & _BIT_MASK_
#define EXIT_ON_MISMATCH(_MATCH_,_MASK_) {if (!((info->_MATCH_ == _MATCH_)^!!(info->invflags & _MASK_))) return false;}

static bool
ebt_tbs_vlan_mt(const struct sk_buff *skb, const struct xt_match_param *par)
{
    struct ebt_tbs_vlan_info *info = (struct ebt_tbs_vlan_info *)par->matchinfo;
    struct vlan_hdr _frame, *fp;
    unsigned short TCI;
    unsigned short id;
    unsigned char prio;

    /*
    * TBS_TAG:add by pengyao 20101119
    * Desc:如果是802.1Q，则匹配vlan header
    */
    TCI = skb->vlan_tci;
    if(eth_hdr(skb)->h_proto == htons(ETH_P_8021Q)) {
        fp = skb_header_pointer(skb, 0, sizeof(_frame), &_frame);
        if (NULL == fp)
            return false;

        /* Tag Control Information (TCI) consists of the following elements:
         * - User_priority. The user_priority field is three bits in length,
         * interpreted as a binary number.
         * - Canonical Format Indicator (CFI). The Canonical Format Indicator
         * (CFI) is a single bit flag value. Currently ignored.
         * - VLAN Identifier (VID). The VID is encoded as
         * an unsigned binary number. */
        TCI = ntohs(fp->h_vlan_TCI);
    }
    /*
    * TBS_TAG:end
    */

    id = TCI & VLAN_VID_MASK;
    prio = (TCI >> 13) & 0x7;
    if (GET_BITMASK(EBT_VLAN_ID))
        EXIT_ON_MISMATCH(id, EBT_VLAN_ID);
    
    if (GET_BITMASK(EBT_VLAN_PRIO))
        EXIT_ON_MISMATCH(prio, EBT_VLAN_PRIO);
        
    return true;
}

static bool ebt_tbs_vlan_mt_check(const struct xt_mtchk_param *par)
{
    struct ebt_tbs_vlan_info *info = par->matchinfo;
    
    if (info->bitmask & ~EBT_VLAN_MASK)
        return false;

    if (info->invflags & ~EBT_VLAN_MASK)
        return false;

    if (GET_BITMASK(EBT_VLAN_ID)) {
        if (info->id >= VLAN_GROUP_ARRAY_LEN)
                return false;     
    }

    if (GET_BITMASK(EBT_VLAN_PRIO)) {
        if ((unsigned char) info->prio > 7) 
            return false;
    }

    return true;
}

static struct xt_match ebt_tbs_vlan_mt_reg __read_mostly = {
	.name		= EBT_VLAN_MATCH,
	.revision	= 0,
	.family		= NFPROTO_BRIDGE,
	.match		= ebt_tbs_vlan_mt,
	.checkentry	= ebt_tbs_vlan_mt_check,
	.matchsize	= XT_ALIGN(sizeof(struct ebt_tbs_vlan_info)),
	.me		= THIS_MODULE,
};


static int __init ebt_tbs_vlan_init(void)
{
	return xt_register_match(&ebt_tbs_vlan_mt_reg);
}

static void __exit ebt_tbs_vlan_fini(void)
{
	xt_unregister_match(&ebt_tbs_vlan_mt_reg);
}

module_init(ebt_tbs_vlan_init);
module_exit(ebt_tbs_vlan_fini);
MODULE_LICENSE("GPL");

