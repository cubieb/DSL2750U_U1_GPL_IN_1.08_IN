#ifndef	RTL865X_COMMON_H
#define	RTL865X_COMMON_H

#include "rtl_types.h"

#ifdef CONFIG_RTL865XC
#define GET_IF_VID(ifa)					(TBLFIELD(vlan_tbl, vhash)[rtl8651_vlanTableIndex((ifa)->vid)].fid)
#else
#define GET_IF_VID(ifa)					(TBLFIELD(vlan_tbl, vhash)[(ifa)->vid].fid)
#endif

#define RTL865X_PPTP_HWACC_PORTMASK		0x80
#define RTL865X_PPTP_HWACC_VLANID			10
#define GATEWAY_MODE				0
#define BRIDGE_MODE				1
#define WISP_MODE					2
//#define MULTIPLE_VLAN_BRIDEG_MODE 3
//#define MULTIPLE_VLAN_WISP_MODE 	4
#define LR_CONFIG_CHECK					LR_INIT_CHECK 

#define LR_INIT_CHECK(expr) do {\
	if(((int32)expr)!=SUCCESS){\
		rtlglue_printf("Error >>> %s:%d failed !\n", __FUNCTION__,__LINE__);\
			return FAILED;\
	}\
	}while(0)

#define INIT_CHECK(expr) do {\
	if(((int32)expr)!=SUCCESS){\
		rtlglue_printf("Error >>> %s:%d failed !\n", __FUNCTION__,__LINE__);\
			return FAILED;\
	}\
}while(0)

typedef struct rtl865x_aclRule_s {
	union {
		/* (1) MAC ACL rule */
		struct {
			ether_addr_t _dstMac, _dstMacMask;
			ether_addr_t _srcMac, _srcMacMask;
			uint16 _typeLen, _typeLenMask;
		} MAC; 
		
		/* (2) IP Group ACL rule */
		struct {
			ipaddr_t _srcIpAddr, _srcIpAddrMask;
			ipaddr_t _dstIpAddr, _dstIpAddrMask;
			uint8 _tos, _tosMask;
			union {
				/* (2.1) IP ACL rle */
				struct {
					uint8 _proto, _protoMask, _flagMask;// flag & flagMask only last 3-bit is meaningful
					uint32 _FOP:1, _FOM:1, _httpFilter:1, _httpFilterM:1, _identSrcDstIp:1, _identSrcDstIpM:1;
					union {
						uint8 _flag;
						struct {
							uint8 pend1:5,
								 pend2:1,
								 _DF:1,	//don't fragment flag
								 _MF:1;	//more fragments flag
						} s;
					} un;							
				} ip; 
				/* (2.2) ICMP ACL rule */
				struct {
					uint8 _type, _typeMask, _code, _codeMask;
				} icmp; 
				/* (2.3) IGMP ACL rule */
				struct {
					uint8 _type, _typeMask;
				} igmp; 
				/* (2.4) TCP ACL rule */
				struct {
					ether_addr_t _l2srcMac, _l2srcMacMask;
					uint8 _flagMask;
					uint16 _srcPortUpperBound, _srcPortLowerBound;
					uint16 _dstPortUpperBound, _dstPortLowerBound;					
					union {
						uint8 _flag;
						struct {
							uint8 _pend:2,
								  _urg:1, //urgent bit
								  _ack:1, //ack bit
								  _psh:1, //push bit
								  _rst:1, //reset bit
								  _syn:1, //sync bit
								  _fin:1; //fin bit
						}s;
					}un;	
				}tcp; 
				/* (2.5) UDP ACL rule */
				struct {					
					ether_addr_t _l2srcMac, _l2srcMacMask;
					uint16 _srcPortUpperBound, _srcPortLowerBound;
					uint16 _dstPortUpperBound, _dstPortLowerBound;					
				}udp; 
			}is;
		}L3L4; 
	}un_ty;

	uint32	ruleType_:4;
	uint32	actionType_:4;
} rtl865x_aclRule_t;

enum ACL_FLAGS
{
	ACL_INGRESS = 1,
	ACL_EGRESS,
};

enum ACL_ACFLAGS
{
	ACL_PERMIT = 1, 	/* RTL8651_ACLTBL_PERMIT_ALL: 125 */
	ACL_DROP = 2, 	/* RTL8651_ACLTBL_DROP_ALL: 126 */
	ACL_CPU = 3, 	/* RTL8651_ACLTBL_ALL_TO_CPU: 127 */
};

enum RTL_RESULT
{
	/* Common error code */
	RTL_SUCCESS = 0,                     		/* Function Success */
	RTL_FAILED = -1,                     		/* General Failure, not recommended to use */
	RTL_ERROR_PARAMETER = -2,            	/* The given parameter error */
	RTL_EXIST = -3,                      		/* The entry you want to add has been existed, add failed */
	RTL_NONEXIST = -4,                   	/* The specified entry is not found */

	RTL_NOBUFFER = -1000,				/* Out of Entry Space */
	RTL_INVAPARAM = -1001,			/* Invalid parameters */
	RTL_NOTFOUND = -1002,			/* Entry not found */
	RTL_DUPENTRY = -1003,				/* Duplicate entry found */
	RTL_RTUNREACH = -1004,			/* Unreachable route */
	RTL_INVVID = -1005,				/* Invalid VID */
	RTL_IFDOWN = -1006,				/* Interface is down */
	RTL_NOARPSPACE = -1007,			/* Out of free ARP space */
	RTL_INVIF = -1008,					/* Invalid interface */
	RTL_INUSE = -1009,					/* Forbidden due to in use */
	RTL_INVEXTIP = -1010,				/* Invalid external IP address */
	RTL_COLLISION = 1011,				/* NAPT entry collision */
	RTL_NULLMACADDR = -1012,			/*null mac address */
	RTL_NOFREEBUFFER = -1013,			/*no free buffer */
};

#ifdef CONFIG_RTL865XC
uint32 rtl8651_vlanTableIndex(uint16 vid);
#else
#define rtl8651_vlanTableIndex(vid)  		(vid& (RTL8651_VLAN_NUMBER-1))
#endif

enum RTL_RESULT rtl865x_addAclRule(uint32 vid, rtl865x_aclRule_t *rule_t, enum ACL_FLAGS flags);
enum RTL_RESULT rtl865x_delAclRule(uint32 vid, rtl865x_aclRule_t *rule_t, enum ACL_FLAGS flags);
enum RTL_RESULT rtl865x_setAclDefaultAction(enum ACL_ACFLAGS defAction);
int  rtl865x_netif_isWan( uint8* ifname );
#endif

