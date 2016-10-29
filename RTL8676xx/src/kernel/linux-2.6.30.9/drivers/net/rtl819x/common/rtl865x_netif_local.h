/*
* Copyright c                  Realtek Semiconductor Corporation, 2008  
* All rights reserved.
* 
* Program : network interface driver header file
* Abstract : 
* Author : hyking (hyking_liu@realsil.com.cn)  
*/

#ifndef RTL865X_NETIF_LOCAL_H
#define RTL865X_NETIF_LOCAL_H

#include <net/rtl/rtl865x_netif.h>
#if !defined(REDUCE_MEMORY_SIZE_FOR_16M)
#define REDUCE_MEMORY_SIZE_FOR_16M
#endif

#if defined(REDUCE_MEMORY_SIZE_FOR_16M)
#ifdef CONFIG_RTL_MULTI_ETH_WAN
#define RTL865X_ACL_CHAIN_NUMBER	128
#else
#define RTL865X_ACL_CHAIN_NUMBER	32
#endif
#else
#define RTL865X_ACL_CHAIN_NUMBER	16
#endif
#define RTL865X_ACL_SYSTEM_USED	-10000

#ifdef CONFIG_RTL_LAYERED_DRIVER_ACL
typedef struct _rtl865x_acl_chain_s
{
	int32 ruleCnt;
	int32 priority; /*chain priosity: the minimum value is the highest proirity*/
	rtl865x_AclRule_t *head,*tail;
	struct _rtl865x_acl_chain_s *preChain,*nextChain;
	uint8 force; /* Kevin, if force=1, it means that this chain has to be added into acl.   If not ,it is maybe not added to acl when the all chains with force=1 occupy acl entry. */
}rtl865x_acl_chain_t;
#endif

/*the following fields are invalid when the interface is slave interface:
* inAclStart, inAclEnd, outAclStart, outAclEnd,asicIdx,chainListHead
*/
typedef struct rtl865x_netif_local_s
{
	uint16 	vid; /*netif->vid*/
	uint16 	mtu; /*netif's MTU*/	
	uint16 	macAddrNumber; /*how many continuous mac is attached*/
	uint16 	inAclStart, inAclEnd, outAclStart, outAclEnd; /*acl index*/
	uint16 	enableRoute; /*enable route*/
	uint32 	valid:1,	/*valid?*/
		if_type:5, /*interface type, IF_ETHER, IF_PPPOE*/
		refCnt:5, /*referenc count by other table entry*/
		asicIdx:4,
		is_wan:1, /*this interface is wan?*/
		is_defaultWan:1, /*if there is multiple wan interface, which interface is default wan*/
		dmz:1,	/*dmz interface?*/
		is_slave:1; /*is slave interface*/
	
	ether_addr_t macAddr;
	uint8	name[MAX_IFNAMESIZE];
#ifdef CONFIG_RTL_LAYERED_DRIVER_ACL
	rtl865x_acl_chain_t  *chainListHead[2]; /*0: ingress acl chain, 1: egress acl chain*/
#endif //CONFIG_RTL_LAYERED_DRIVER_ACL
	struct rtl865x_netif_local_s *master; /*point master interface when this interface is slave interface*/
#if defined (CONFIG_RTL_IPTABLES_RULE_2_ACL) && !defined(CONFIG_RTL8676_Dynamic_ACL)
	__be32 ipv4_addr;
#endif
}rtl865x_netif_local_t;

#define	RTL_ACL_INGRESS	0
#define	RTL_ACL_EGRESS	1

#ifdef CONFIG_RTL_LAYERED_DRIVER_ACL
typedef struct rtl865x_aclBuf_s
{
	int16 totalCnt;
	int16 freeCnt;
	rtl865x_AclRule_t *freeHead;
}rtl865x_aclBuf_t;
#endif


int32 rtl865x_enableNetifRouting(rtl865x_netif_local_t *netif);
int32 rtl865x_disableNetifRouting(rtl865x_netif_local_t *netif);
rtl865x_netif_local_t *_rtl865x_getNetifByName(char *name);
rtl865x_netif_local_t *_rtl865x_getSWNetifByName(char * name);
rtl865x_netif_local_t *_rtl865x_getDefaultWanNetif(void);
int32 _rtl865x_setDefaultWanNetif(char *name);
int32 _rtl865x_clearDefaultWanNetif(char * name);
int32 _rtl865x_getNetifIdxByNameExt(uint8 *name);
char* _rtl865x_getNetifNameByidx(int32 idx);

#define ForEachMasterNetif_Declaration\
		extern rtl865x_netif_local_t * getNetifTbl(void);\
		rtl865x_netif_local_t *netif_table = getNetifTbl();\
		int netif_idx;

#define ForEachMasterNetif_Start\
		for(netif_idx=0;netif_idx<NETIF_SW_NUMBER;netif_idx++)\
		{\
			rtl865x_netif_local_t *netif = &(netif_table[netif_idx]);\
			if(netif_table[netif_idx].valid == 0 || netif_table[netif_idx].is_slave)\
				continue;
				
#define ForEachMasterNetif_End\
		}


#define ForEachMasterWanNetif_Declaration\
		extern rtl865x_netif_local_t * getNetifTbl(void);\
		rtl865x_netif_local_t *netif_table = getNetifTbl();\
		int netif_idx;

#define ForEachMasterWanNetif_Start\
		for(netif_idx=0;netif_idx<NETIF_SW_NUMBER;netif_idx++)\
		{\
			rtl865x_netif_local_t *netif = &(netif_table[netif_idx]);\
			if(netif_table[netif_idx].valid == 0 || netif_table[netif_idx].is_slave || netif_table[netif_idx].is_wan==0)\
				continue;
				
#define ForEachMasterWanNetif_End\
		}

#endif
