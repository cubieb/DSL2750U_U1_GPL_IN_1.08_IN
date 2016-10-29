/*
 *	Linux bridge vlan
 *
 *	Authors:
 *	china.wu		<wuzhonghua@twsz.com>
 *
 *	2010/11/26
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#ifndef _BR_VLAN_H
#define _BR_VLAN_H

#define VLAN_MAX_TRANSLATE  16
#define VLAN_MAX_TRUNK      32
#define VLAN_MAX_AGGR       8
#define VLAN_MAX_AGGR_N     8

#define VLAN_LAN_NUM       4

enum vlan_err_t
{
    VLAN_OK,
    VLAN_ERR,
    VLAN_PARAM,
    VLAN_DROP
};

enum vlan_mode_e
{
    VLAN_TRANSPARENT,
    VLAN_TRUNK,
    VLAN_TAG,
    VLAN_TRANSLATE,
    VLAN_AGGR,
    VLAN_HYBIRD
};

typedef unsigned short VLAN_T;

typedef struct vlan_mode_translate
{
    VLAN_T old_vlan;
    VLAN_T new_vlan;
}VLAN_TRANSLATE_T;

typedef struct vlan_mode_aggr
{
    unsigned int num;
    VLAN_T vlan_1;
    VLAN_T vlan_n[VLAN_MAX_AGGR_N];
}VLAN_AGGR_T;

typedef struct vlan_mode_trunk
{
    VLAN_T vlan[VLAN_MAX_TRUNK];
}VLAN_TRUNK_T;

#define VLAN_MAX_UNTAG 32
typedef struct vlan_mode_hybird
{
    unsigned int trans_num;
    unsigned int untag_num;
    VLAN_T  trans_vlan[VLAN_MAX_TRUNK];//允许通过VLAN ID列表
    VLAN_T  untag_vlan[VLAN_MAX_UNTAG];//剥离VLAN ID列表
}VLAN_HYBIRD_T;


typedef struct vlan_mode
{
    unsigned int mode;
    VLAN_T pvlan; //default vlan
    unsigned int num;
    union
    {
        VLAN_TRANSLATE_T translate[VLAN_MAX_TRANSLATE];
        VLAN_AGGR_T aggr[VLAN_MAX_AGGR];
        VLAN_T trunk[VLAN_MAX_TRUNK];
        VLAN_HYBIRD_T hybird;
    }u;
}VLAN_MODE_T;

//eth2
#define PORT_IS_LAN(port)       ((port->iflink)==2) 
//eth0
#define PORT_IS_WAN(port)       ((port->name[0])=='n') 

#define PORT_ID_IS_LAN(port)    ((port)<5) 
#define PORT_ID_IS_WAN(port)    ((port)>4) 

//#define PKT_IS_8021Q(skb)       ((skb)->vlan > 0)
#define PKT_IS_8021Q(vhdr)       (ntohs((vhdr)->h_vlan_proto) == ETH_P_8021Q)
#define PKT_VLAN_ID(vhdr)        (ntohs((vhdr)->h_vlan_TCI)& 0xfff)

#define PKT_DOWN_IS_8021Q(skb)  ((skb)->protocol == ntohs(ETH_P_8021Q))
#define PKT_VID(skb)            ((skb)->vlan)
//cjg add start
unsigned int vlan_forward(struct sk_buff **pskb,
				      const struct net_device *in,
				      const struct net_device *out);
int vlan_set_mode(struct net_device * dev, VLAN_MODE_T *mode);
VLAN_MODE_T* vlan_get_mode(struct net_device * dev);
//cjg add end
#endif


