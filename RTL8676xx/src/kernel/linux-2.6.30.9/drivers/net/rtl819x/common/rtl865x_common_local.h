#ifndef	RTL865X_COMMON_LOCAL_H
#define	RTL865X_COMMON_LOCAL_H

#include "rtl865x_common.h"



//#define VLAN_NUMBER						4096
#define VLAN_NUMBER						32
#define NETIF_NUMBER						4


#define TBLFIELD(tbl, field)					((tbl).field)

#define RTL8651_MAC_NUMBER				6
#define RTL8651_PORT_NUMBER				RTL8651_MAC_NUMBER
#define RTL8651_MII_PORTNUMBER                 	5
#define RTL8651_MII_PORTMASK                    	0x20
#define RTL8651_PHY_NUMBER				5
#define RTL8651_AGGREGATOR_NUMBER		(RTL8651_PORT_NUMBER+rtl8651_totalExtPortNum)
#define RTL8651_PSOFFLOAD_RESV_PORT		(RTL8651_PORT_NUMBER+rtl8651_totalExtPortNum+1)	/* port reserved for protocol stack offloading */
#define RTL8651_ALLPORTMASK				((1<<RTL8651_AGGREGATOR_NUMBER)-1)
#define RTL8651_PHYSICALPORTMASK			((1<<RTL8651_MAC_NUMBER)-1)

#define RTL8651_VLAN_NUMBER				8

#ifdef CONFIG_RTL865XC
extern  int32		rtl8651_totalExtPortNum; /*this replaces all RTL8651_EXTPORT_NUMBER defines*/
#endif

//#define RTL8651_IPTABLE_SIZE				16

/* ACL Rule type Definition */
#define RTL8651_ACL_MAC						0x00
#define RTL8651_ACL_IP						0x01
#define RTL8651_ACL_ICMP						0x02
#define RTL8651_ACL_IGMP						0x03
#define RTL8651_ACL_TCP						0x04
#define RTL8651_ACL_UDP						0x05
/* 6-8*/ 
#define RTL8652_ACL_IP_RANGE					0x0A
#define RTL8652_ACL_ICMP_IPRANGE				0x0B
#define RTL8652_ACL_TCP_IPRANGE				0x0C
#define RTL8652_ACL_IGMP_IPRANGE				0x0D
#define RTL8652_ACL_UDP_IPRANGE				0x0E
#define RTL8652_ACL_SRCFILTER_IPRANGE 		0x09
#define RTL8652_ACL_DSTFILTER_IPRANGE 		0x0F


/* MAC ACL rule Definition */
#define dstMac_				un_ty.MAC._dstMac
#define dstMacMask_			un_ty.MAC._dstMacMask
#define srcMac_				un_ty.MAC._srcMac
#define srcMacMask_			un_ty.MAC._srcMacMask
#define typeLen_				un_ty.MAC._typeLen
#define typeLenMask_			un_ty.MAC._typeLenMask

/* IFSEL ACL rule Definition */
#define gidxSel_				un_ty.IFSEL._gidxSel

/* Common IP ACL Rule Definition */
#define srcIpAddr_				un_ty.L3L4._srcIpAddr
#define srcIpAddrMask_			un_ty.L3L4._srcIpAddrMask
#define srcIpAddrUB_			un_ty.L3L4._srcIpAddr
#define srcIpAddrLB_			un_ty.L3L4._srcIpAddrMask
#define dstIpAddr_				un_ty.L3L4._dstIpAddr
#define dstIpAddrMask_		un_ty.L3L4._dstIpAddrMask
#define dstIpAddrUB_			un_ty.L3L4._dstIpAddr
#define dstIpAddrLB_			un_ty.L3L4._dstIpAddrMask
#define tos_					un_ty.L3L4._tos
#define tosMask_				un_ty.L3L4._tosMask
/* IP Rrange */
#if 0
#define srcIpAddrStart_		un_ty.L3L4._srcIpAddr
#define srcIpAddrEnd_			un_ty.L3L4._srcIpAddrMask
#define dstIpAddrStart_		un_ty.L3L4._dstIpAddr
#define dstIpAddrEnd_			un_ty.L3L4._dstIpAddrMask
#endif

/* IP ACL Rule Definition */
#define ipProto_				un_ty.L3L4.is.ip._proto
#define ipProtoMask_			un_ty.L3L4.is.ip._protoMask
#define ipFlagMask_			un_ty.L3L4.is.ip._flagMask
#define ipFOP_      			un_ty.L3L4.is.ip._FOP
#define ipFOM_      			un_ty.L3L4.is.ip._FOM
#define ipHttpFilter_      		un_ty.L3L4.is.ip._httpFilter
#define ipHttpFilterM_			un_ty.L3L4.is.ip._httpFilterM
#define ipIdentSrcDstIp_   		un_ty.L3L4.is.ip._identSrcDstIp
#define ipIdentSrcDstIpM_		un_ty.L3L4.is.ip._identSrcDstIpM
#define ipFlag_				un_ty.L3L4.is.ip.un._flag
#define ipDF_					un_ty.L3L4.is.ip.un.s._DF
#define ipMF_					un_ty.L3L4.is.ip.un.s._MF

/* ICMP ACL Rule Definition */
#define icmpType_				un_ty.L3L4.is.icmp._type
#define icmpTypeMask_			un_ty.L3L4.is.icmp._typeMask	
#define icmpCode_				un_ty.L3L4.is.icmp._code
#define icmpCodeMask_			un_ty.L3L4.is.icmp._codeMask

/* IGMP ACL Rule Definition */
#define igmpType_				un_ty.L3L4.is.igmp._type
#define igmpTypeMask_			un_ty.L3L4.is.igmp._typeMask

/* TCP ACL Rule Definition */
#define tcpl2srcMac_			un_ty.L3L4.is.tcp._l2srcMac		// for srcMac & destPort ACL rule
#define tcpl2srcMacMask_		un_ty.L3L4.is.tcp._l2srcMacMask
#define tcpSrcPortUB_			un_ty.L3L4.is.tcp._srcPortUpperBound
#define tcpSrcPortLB_			un_ty.L3L4.is.tcp._srcPortLowerBound
#define tcpDstPortUB_			un_ty.L3L4.is.tcp._dstPortUpperBound
#define tcpDstPortLB_			un_ty.L3L4.is.tcp._dstPortLowerBound
#define tcpFlagMask_			un_ty.L3L4.is.tcp._flagMask
#define tcpFlag_				un_ty.L3L4.is.tcp.un._flag
#define tcpURG_				un_ty.L3L4.is.tcp.un.s._urg
#define tcpACK_				un_ty.L3L4.is.tcp.un.s._ack
#define tcpPSH_				un_ty.L3L4.is.tcp.un.s._psh
#define tcpRST_				un_ty.L3L4.is.tcp.un.s._rst
#define tcpSYN_				un_ty.L3L4.is.tcp.un.s._syn
#define tcpFIN_				un_ty.L3L4.is.tcp.un.s._fin

/* UDP ACL Rule Definition */
#define udpl2srcMac_			un_ty.L3L4.is.udp._l2srcMac		// for srcMac & destPort ACL rule
#define udpl2srcMacMask_		un_ty.L3L4.is.udp._l2srcMacMask
#define udpSrcPortUB_			un_ty.L3L4.is.udp._srcPortUpperBound
#define udpSrcPortLB_			un_ty.L3L4.is.udp._srcPortLowerBound
#define udpDstPortUB_			un_ty.L3L4.is.udp._dstPortUpperBound
#define udpDstPortLB_			un_ty.L3L4.is.udp._dstPortLowerBound

#if 1
/* Source Filter ACL Rule Definition */
#define srcFilterMac_			un_ty.SRCFILTER._srcMac
#define srcFilterMacMask_		un_ty.SRCFILTER._srcMacMask
#define srcFilterPort_			un_ty.SRCFILTER._srcPort
#define srcFilterPortMask_		un_ty.SRCFILTER._srcPortMask
#define srcFilterVlanIdx_		un_ty.SRCFILTER._srcVlanIdx
#define srcFilterVlanId_			un_ty.SRCFILTER._srcVlanIdx
#define srcFilterVlanIdxMask_	un_ty.SRCFILTER._srcVlanIdxMask
#define srcFilterVlanIdMask_		un_ty.SRCFILTER._srcVlanIdxMask
#define srcFilterIpAddr_			un_ty.SRCFILTER._srcIpAddr
#define srcFilterIpAddrMask_	un_ty.SRCFILTER._srcIpAddrMask
#define srcFilterIpAddrUB_		un_ty.SRCFILTER._srcIpAddr
#define srcFilterIpAddrLB_		un_ty.SRCFILTER._srcIpAddrMask
#define srcFilterPortUpperBound_	un_ty.SRCFILTER._srcPortUpperBound
#define srcFilterPortLowerBound_	un_ty.SRCFILTER._srcPortLowerBound
#define srcFilterIgnoreL3L4_		un_ty.SRCFILTER._ignoreL3L4
#define srcFilterIgnoreL4_		un_ty.SRCFILTER._ignoreL4

/* Destination Filter ACL Rule Definition */
#define dstFilterMac_			un_ty.DSTFILTER._dstMac
#define dstFilterMacMask_		un_ty.DSTFILTER._dstMacMask
#define dstFilterVlanIdx_		un_ty.DSTFILTER._vlanIdx
#define dstFilterVlanIdxMask_	un_ty.DSTFILTER._vlanIdxMask
#define dstFilterVlanId_			un_ty.DSTFILTER._vlanIdx
#define dstFilterVlanIdMask_		un_ty.DSTFILTER._vlanIdxMask
#define dstFilterIpAddr_		un_ty.DSTFILTER._dstIpAddr
#define dstFilterIpAddrMask_	un_ty.DSTFILTER._dstIpAddrMask
#define dstFilterPortUpperBound_	un_ty.DSTFILTER._dstPortUpperBound
#define dstFilterIpAddrUB_		un_ty.DSTFILTER._dstIpAddr
#define dstFilterIpAddrLB_		un_ty.DSTFILTER._dstIpAddrMask
#define dstFilterPortLowerBound_	un_ty.DSTFILTER._dstPortLowerBound
#define dstFilterIgnoreL3L4_		un_ty.DSTFILTER._ignoreL3L4
#define dstFilterIgnoreL4_		un_ty.DSTFILTER._ignoreL4
#endif

/* ACL Rule Action type Definition */
#define RTL8651_ACL_PERMIT			0x01
#define RTL8651_ACL_DROP				0x02
#define RTL8651_ACL_CPU				0x03
#define RTL8651_ACL_DROP_LOG			0x04
#define RTL8651_ACL_DROP_NOTIFY		0x05
#define RTL8651_ACL_L34_DROP			0x06	/* special for default ACL rule */

/* ACL Rule type Definition */
#define RTL8651_ACL_MAC				0x00
#define RTL8651_ACL_IP					0x01
#define RTL8651_ACL_ICMP				0x02
#define RTL8651_ACL_IGMP				0x03
#define RTL8651_ACL_TCP					0x04
#define RTL8651_ACL_UDP				0x05
/* 6-8*/ 
#define RTL8652_ACL_IP_RANGE			0x0A
#define RTL8652_ACL_ICMP_IPRANGE		0x0B
#define RTL8652_ACL_TCP_IPRANGE		0x0C
#define RTL8652_ACL_IGMP_IPRANGE		0x0D
#define RTL8652_ACL_UDP_IPRANGE		0x0E
#define RTL8652_ACL_SRCFILTER_IPRANGE 0x09
#define RTL8652_ACL_DSTFILTER_IPRANGE 0x0F



/* Spanning Tree Port State Definition */
#define RTL8651_PORTSTA_DISABLED		0x00
#define RTL8651_PORTSTA_BLOCKING		0x01
#define RTL8651_PORTSTA_LISTENING		0x02
#define RTL8651_PORTSTA_LEARNING		0x03
#define RTL8651_PORTSTA_FORWARDING	0x04

//Ethernet port bandwidth control
#define RTL8651_BC_FULL		0x00
#define RTL8651_BC_128K		0x01
#define RTL8651_BC_256K		0x02
#define RTL8651_BC_512K		0x03
#define RTL8651_BC_1M		0x04
#define RTL8651_BC_2M		0x05
#define RTL8651_BC_4M		0x06
#define RTL8651_BC_8M		0x07
#define RTL8651_BC_16M		0x08
#define RTL8651_BC_32M		0x09
#define RTL8651_BC_64M		0x0A

#define RTL865XC_NETINTERFACE_NUMBER		8
#define RTL865XC_MAC_NETINTERFACE_NUMBER		4
#define RTL865XC_PORT_NUMBER				9
#define RTL865XC_VLAN_NUMBER				4096
#define RTL865XC_LAGHASHIDX_NUMBER			8	/* There are 8 hash values in RTL865xC Link Aggregation. */

#define RTL8651_ACLTBL_NOOP				7 /* No operation. Don't apply this rule. */
#define RTL8651_ETHER_AUTO_100FULL	0x00
#define RTL8651_ETHER_AUTO_100HALF	0x01
#define RTL8651_ETHER_AUTO_10FULL		0x02
#define RTL8651_ETHER_AUTO_10HALF	0x03
#define RTL8651_ETHER_AUTO_1000FULL	0x08
#define RTL8651_ETHER_AUTO_1000HALF	0x09
/* chhuang: patch for priority issue */
#define RTL8651_ETHER_FORCE_100FULL	0x04
#define RTL8651_ETHER_FORCE_100HALF	0x05
#define RTL8651_ETHER_FORCE_10FULL	0x06
#define RTL8651_ETHER_FORCE_10HALF	0x07


#define MAX_IFNAMESIZE				16
#define LRCONFIG_END					{ "", 0, 0, 0, 0, 0, 0, 0, {{0}} }

enum IF_TYPE
{
	IF_ETHER = 0,
	IF_PPPOE = 1,
	IF_PPTP = 2,
	IF_L2TP = 3,
#ifdef CONFIG_RTL865X_HW_PPTPL2TP
	IF_PPP = IF_PPPOE,
#endif	
};


struct rtl865x_vlanConfig {
	uint8 			ifname[16];
	uint8			isWan;
	enum IF_TYPE		if_type;
	uint16			vid;
	uint16			fid;
	uint32			memPort;
	uint32			untagSet;
	uint32			mtu;
	ether_addr_t		mac;
};

struct rtl8651_tblDrv_networkIntfTable_s;
typedef struct rtl8651_tblDrv_vlanTable_s {
	ether_addr_t macAddr;
	uint32 memberPortMask;		//port i at 1<<i position, from 0 to 31. The port means link aggregator instead of actual port
	uint32 memberPortUpStatus; //Whether member aggregator is up
	uint32 untagPortMask;		//port i at 1<<i position, 1 means enable untag 0 means disable untag.
	uint16 macAddrNumber;		// The number of consecutive mac address, 0 will disable ENRTR (Enable Routing)
	uint16 vid;				// VLAN ID
	uint16 fid;				//Filtering Database Index
	uint32  inAclStart, inAclEnd, outAclStart, outAclEnd;
	uint16	mtu;
	uint16 	internal:1,		//Whether vlan is internal VLAN. This is configured by NAT control APIs
			ipAttached:1,	//IP interface attached over it
			manualAssign:1,	//Whether MAC address is manually assigned
			promiscuous:1,
			fwdTxMirror:1,		//The port members of this VLAN is used to mirror CPU generated 
			DMZflag:1,
			valid:1,			//Whether entry is valid
			softRoute:1,		//pure software routing on this vlan
#if defined (CONFIG_RTL865XB_ENRT) || defined(CONFIG_RTL865XC)	/* refer to _rtl8651_addVlan() for the meaning of this compile flag */
			macNonExist:1,	/* If this bit is set, only L2 forwarding is performed over this VLAN. */
#endif
			extPortAsicAlwaysTag:1,	/* Extension Port would always be TAGGED in ASIC ? For 865xB B-Cut PPPoE DMA bug. */
			broadcastToCpu:1;	//Broadcast to CPU ?
	struct rtl8651_tblDrv_vlanTable_s *fvlan_t; //if not NULL, it is a sub vlan
#ifdef CONFIG_RTL865XB_EXP_INVALID
	SLIST_HEAD(, rtl8651_tblDrv_networkIntfTable_s) netHead;
#else
	struct rtl8651_tblDrv_networkIntfTable_s *netif_t;
#endif /* CONFIG_RTL865XB_EXP_INVALID */
} rtl8651_tblDrv_vlanTable_t;


#if defined (RTL865XB_DCUT_SWVLAN) || defined(CONFIG_RTL865XC)
typedef struct rtl8651_tblDrv_swVlanTable_s {
	uint32 memberPortMask;
	uint32 memberPortUpStatus;
	uint32 untagPortMask;
	uint16 vid;
	uint16 fid;
	uint16	valid:1;
}rtl8651_tblDrv_swVlanTable_t;
#endif


/*
	VLAN releated process
*/


/*==========================================================================
 *  Light ROME Driver may support the following two modes:
 *    a.Linux with bridge support
 *    b.Linux without bridge support
 *
 *  a.Linux With Bridge Support
 *    In this mode, the Light ROME Driver simulates 6 interfaces for Linux
 *    (from eth0 to eth5 and one to one mapping to physical port).
 *    Therefore, the Linux can utilize bridge to cascade every physical port of RTL865x.
 *
 *  +---------------------------------------------+
 *  |        a.Linux With Bridge Support          |
 *  |                                             |
 *  | +-----------+   +-------------------------+ |
 *  | |    br0    |   |           br1           | |
 *  | +-----------+   +-------------------------+ |
 *  |    |     |         |      |     |      |    |
 *  | +----+ +----+   +----+ +----+ +----+ +----+ |
 *  +-|eth0|-|eth1|---|eth2|-|eth3|-|eth4|-|eth5|-+
 *    +----+ +----+   +----+ +----+ +----+ +----+
 *       |     |         |      |     |      |
 *  +---------------------------------------------+
 *  |          Light ROME Driver NIC              |
 *  +---------------------------------------------+
 *       |     |         |      |     |      |
 *    +----+ +----+   +----+ +----+ +----+ +----+
 *    | P0 | | P1 |   | P2 | | P3 | | P4 | | P5 |
 *    +----+ +----+   +----+ +----+ +----+ +----+
 *
 *  [How to configure Light ROME Driver?]
 *     addInterface( "eth0", ... );
 *     addInterface( "eth1", ... );
 *     addInterface( "eth2", ... );
 *     addInterface( "eth3", ... );
 *     addInterface( "eth4", ... );
 *     addInterface( "eth5", ... );
 *     configInterface( "eth0", PORT0, ... );
 *     configInterface( "eth1", PORT1, ... );
 *     configInterface( "eth2", PORT2, ... );
 *     configInterface( "eth3", PORT3, ... );
 *     configInterface( "eth4", PORT4, ... );
 *     configInterface( "eth5", PORT5, ... );
 *
 *==========================================================================
 *  b.Linux WITHOUT Bridge Support
 *    In this mode, the Light ROME Driver simulates 2 interfaces for Linux (eth0 for WAN, and eth1 for LAN).
 *    The Light ROME Driver must provide bridge functionality, including MAC learning and bridge.
 *    The bridge functionality can be implemented by ASIC (both 865xB and 865xC).
 *
 *  +---------------------------------------------+
 *  |       b.Linux WITHOUT Bridge Support        |
 *  |                                             |
 *  |                                             |
 *  |                                             |
 *  | +-----------+   +-------------------------+ |
 *  +-|   eth0    |---|           eth1          |-+
 *    +-----------+   +-------------------------+
 *          |                      |          
 *  +---------------------------------------------+
 *  |      Light ROME Driver NIC (Bridging)       |
 *  +---------------------------------------------+
 *        |    |            |     |    |    |
 *     +----+----+        +----+----+----+----+
 *     | P0 | P1 |        | P2 | P3 | P4 | P5 |
 *     +----+----+        +----+----+----+----+
 *
 *   [How to configure Light ROME Driver?]
 *     addInterface( "eth0", ... );
 *     addInterface( "eth1", ... );
 *     configInterface( "eth0", PORT0|PORT1, ... );
 *     configInterface( "eth1", PORT2|PORT3|PORT4|PORT5, ... );
 *  
 */


enum IF_FLAGS
{
	IF_NONE,
	IF_INTERNAL = (0<<1), /* This is an internal interface. */
	IF_EXTERNAL = (1<<1), /* This is an external interface. */
};



enum REG_FLAGS
{
	REG_BR_RX_ACTION = 1,	/* Light Rome Driver doesn't support layer2 function */
	REG_RX_ACTION = 2,		/* Light Rome Driver supports layer2 function  */
	REG_LINK_CHANGE = 3,
};

/* ACL Numbers */
#define RTL865x_ACL_NUMBER				128
#define DEFAULT_INC						124
#define SET_DEFAULT_ACL(idx, action)					\
{													\
	_rtl8651_tblDrvAclRule_t def_rule;					\
	memset(&def_rule, 0, sizeof(_rtl8651_tblDrvAclRule_t));	\
	def_rule.actionType_  = action;						\
	TBLFIELD(acl_tbl, acl_asic_set)((idx), &def_rule);		\
}



//rtl8651_tblDrv_vlanTable_t *_rtl8651_getVlanTableEntry(uint16 vid);
uint16 _rtl8651_getOneVidx(int8 internal);

int32 _rtl8651_setPvid(uint32 port, uint16 vid);
uint16 _rtl8651_getPvid(uint32 port);



struct acl_table 
{
	CTAILQ_HEAD(, _rtl8651_tblDrvAclRule_s)	free_head;
	CTAILQ_HEAD(, _rtl8651_tblDrvAclRule_s)	acl_in_head[RTL8651_VLAN_NUMBER];
	CTAILQ_HEAD(, _rtl8651_tblDrvAclRule_s)	acl_eg_head[RTL8651_VLAN_NUMBER];
	
	_rtl8651_tblDrvAclRule_t				free_list[RTL865x_ACL_NUMBER];
	enum ACL_ACFLAGS					def_action;
	uint32								entry;

	int32						(*acl_init)(void);
	int32						(*acl_add)(uint32, _rtl8651_tblDrvAclRule_t *, enum ACL_FLAGS);
	int32						(*acl_del)(uint32, _rtl8651_tblDrvAclRule_t *, enum ACL_FLAGS);
	int32						(*acl_arrange)(void);
	
	int32						(*acl_asic_set)(uint32, _rtl8651_tblDrvAclRule_t *);
	int32						(*acl_asic_get)(uint32, _rtl8651_tblDrvAclRule_t *);
};




struct vlan_entry {
	uint32						valid;
	uint32						mbr;
	uint32 						untagSet;
	uint32						fid;
	ether_addr_t					mac;
	uint32                                         vid;
};


struct vlan_table {
	struct vlan_entry				vhash[VLAN_NUMBER];
	uint16						asic_vtbl[RTL8651_VLAN_NUMBER];

	/* Software Table Interface */
	int32						(*vlan_init)(void);
	int32						(*vlan_create)(struct rtl865x_vlanConfig *);
	int32						(*vlan_remove)(uint32);
	int32						(*vlan_hash)(uint16);

	/* ASIC Table Interface */
	int32 						(*vlan_asic_get)(uint16, rtl865x_tblAsicDrv_vlanParam_t *);
#ifdef CONFIG_RTL865XC
	int32 						(*vlan_asic_set)(uint16 , rtl865x_tblAsicDrv_vlanParam_t *); 
#else
	int32 						(*vlan_asic_set)(rtl865x_tblAsicDrv_vlanParam_t *);
#endif
	int32						(*vlan_asic_del)(uint16);

	int32						(*vlan_set_mac)(struct rtl865x_vlanConfig *vlanconfig);
	int32						(*vlan_set_mtu)(struct rtl865x_vlanConfig *vlanconfig);
#ifdef CONFIG_RTL865XC
	int32 						(*intf_asic_set)( uint32 idx, rtl865x_tblAsicDrv_intfParam_t *intfp );
	int32 						(*intf_asic_get)( uint32 idx, rtl865x_tblAsicDrv_intfParam_t *intfp );
#endif
};



struct if_entry {
	uint8				name[MAX_IFNAMESIZE];
#ifdef CONFIG_RTL865XC
	int8                            isWan;
#endif
	uint16 				vid;
	//uint16				fid;
	uint16				mtu;
	//ether_addr_t			mac;
	uint16				up;
	enum IF_TYPE			if_type;
	
	union {
		struct {
			ipaddr_t		ipaddr;
			ipaddr_t		mask;
			uint16		arp_start;
			uint16		arp_end;
		} eth;
		struct {
			uint16		sid;
			ether_addr_t	pmac;
		} pppoe;
	} un;
	
#define IF_UP(ife)			(ife->up)
#define ipaddr_			un.eth.ipaddr
#define mask_			un.eth.mask
#define arp_start_			un.eth.arp_start
#define arp_end_			un.eth.arp_end
#define sid_				un.pppoe.sid
#define pmac_			un.pppoe.pmac
};

struct if_table {
	struct if_entry				if_hash[4];
	struct if_entry *				(*if_lookup)(uint8 *, ipaddr_t);
	int32 						(*if_init)(void);
	int32						(*if_attach)(struct rtl865x_vlanConfig *);
	int32						(*if_detach)(uint8 *);
	int32						(*if_up)(uint8 *, ipaddr_t, ipaddr_t, uint32, ether_addr_t *);
	int32						(*if_down)(uint8 *);
};

struct vlan_entry*  lr_get_vlan(uint32 vid);
struct vlan_entry*  lr_get_vlan_byidx(uint32 vidx);


/*2007-12-19*/
extern struct vlan_table					vlan_tbl;
extern struct acl_table						acl_tbl;


extern struct if_table						if_tbl;
struct lr_cpu_stats {
	uint32				rx_packets;
	uint32				rx_bytes;
	uint32				rx_drop;
	uint32				rx_mcast;
	uint32				rx_bcast;

	uint32				tx_packets;
	uint32				tx_bytes;
	uint32				tx_drop;
	uint32				tx_mcast;
	uint32				tx_bcast;

	uint32				br_packets;
};

extern struct lr_cpu_stats 					lr_stats;

#endif
