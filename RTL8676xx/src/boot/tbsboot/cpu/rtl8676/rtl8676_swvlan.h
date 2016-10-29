/*
* ----------------------------------------------------------------
* Copyright c                  Realtek Semiconductor Corporation, 2002  
* All rights reserved.
* 
* $Header: /usr/local/dslrepos/boot0412/tftpnaive/sw/include/rtl8650/vlanTable.h,v 1.1.1.1 2011/06/10 08:06:31 yachang Exp $
*
* Abstract: Switch core vlan table access header file.
*
* $Author: yachang $
*
* $Log: vlanTable.h,v $
* Revision 1.1.1.1  2011/06/10 08:06:31  yachang
*
*
* Revision 1.4  2006/09/15 03:53:39  ghhuang
* +: Add TFTP download support for RTL8652 FPGA
*
* Revision 1.3  2005/09/22 05:22:31  bo_zhao
* *** empty log message ***
*
* Revision 1.1.1.1  2005/09/05 12:38:24  alva
* initial import for add TFTP server
*
* Revision 1.2  2004/03/31 01:49:20  yjlou
* *: all text files are converted to UNIX format.
*
* Revision 1.1  2004/03/16 06:36:13  yjlou
* *** empty log message ***
*
* Revision 1.1.1.1  2003/09/25 08:16:56  tony
*  initial loader tree 
*
* Revision 1.1.1.1  2003/05/07 08:16:07  danwu
* no message
*
* ---------------------------------------------------------------
*/

#ifndef _VLANTABLE_H_
#define _VLANTABLE_H_

#include "rtl8676_swmain.h"

/* VLAN table access routines 
*/

/* Create vlan 
Return: EEXIST- Speicified vlan already exists.
        ENFILE- Destined slot occupied by another vlan.*/
int vlanTable_create(u32 vid, rtl_vlan_param_t * param);

/* Destroy vlan 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_destroy(u32 vid);

/* Add a member port
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_addMemberPort(u32 vid, u32 portNum);

/* Remove a member port 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_removeMemberPort(u32 vid, u32 portNum);

/* Set a member port list 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_setMemberPort(u32 vid, u32 portList);

/* Set ACL rule 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_setAclRule(u32 vid, u32 inACLStart, u32 inACLEnd,
                                u32 outACLStart, u32 outACLEnd);

/* Get ACL rule 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_getAclRule(u32 vid, u32 *inACLStart_P, u32 *inACLEnd_P,
                                u32 *outACLStart_P, u32 *outACLEnd_P);

/* Set vlan as internal interface 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_setInternal(u32 vid);

/* Set vlan as external interface 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_setExternal(u32 vid);

/* Enable hardware routing for this vlan 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_enableHardwareRouting(u32 vid);

/* Disable hardware routing for this vlan 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_disableHardwareRouting(u32 vid);

/* Set spanning tree status 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_setPortStpStatus(u32 vid, u32 portNum, u32 STPStatus);

/* Get spanning tree status 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_getPortStpStatus(u32 vid, u32 portNum, u32 *STPStatus_P);

/* Set spanning tree status 
Return: ENOENT- Specified vlan id does not exist.*/
int vlanTable_setStpStatus(u32 vid, u32 STPStatus);


/* Get vlan id 
Return: ENOENT- Specified slot does not exist.*/
int vlanTable_getVidByIndex(u32 eidx, u32 * vid_P);

int swCore_netifCreate(u32 idx, rtl_netif_param_t * param);


#ifdef CONFIG_RTL865XC
/* Hardware bit allocation of VLAN table 
*/
typedef struct {
	 /* word 0 */
	u32	vid:12;	//CONFIG_RTL8196D
	u32	fid:2;
	u32     extEgressUntag  : 3;
	u32     egressUntag : 6;
	u32     extMemberPort   : 3;
	u32     memberPort  : 6;

    /* word 1 */
    u32          reservw1;
    /* word 2 */
    u32          reservw2;
    /* word 3 */
    u32          reservw3;
    /* word 4 */
    u32          reservw4;
    /* word 5 */
    u32          reservw5;
    /* word 6 */
    u32          reservw6;
    /* word 7 */
    u32          reservw7;
} vlan_table_t;

typedef struct {
    /* word 0 */
    u32          mac18_0:19;
    u32          vid		 : 12;
    u32          valid       : 1;	
    /* word 1 */
    u32         inACLStartL:2;	
    u32         enHWRoute : 1;	
    u32         mac47_19:29;

    /* word 2 */
    u32         mtuL       : 3;
    u32         macMask :3;	
    u32         outACLEnd : 7;	
    u32         outACLStart : 7;	
    u32         inACLEnd : 7;	
    u32         inACLStartH: 5;	
    /* word 3 */
    u32          reserv10   : 20;
    u32          mtuH       : 12;

    /* word 4 */
    u32          reservw4;
    /* word 5 */
    u32          reservw5;
    /* word 6 */
    u32          reservw6;
    /* word 7 */
    u32          reservw7;
} netif_table_t;


typedef struct {
   union {
        struct {
            /* word 0 */
            u16          dMacP31_16;
            u16          dMacP15_0;
            /* word 1 */
            u16          dMacM15_0;
            u16          dMacP47_32;
            /* word 2 */
            u16          dMacM47_32;
            u16          dMacM31_16;
            /* word 3 */
            u16          sMacP31_16;
            u16          sMacP15_0;
            /* word 4 */
            u16          sMacM15_0;
            u16          sMacP47_32;
            /* word 5 */
            u16          sMacM47_32;
            u16          sMacM31_16;
            /* word 6 */
            u16          ethTypeM;
            u16          ethTypeP;
        } ETHERNET;
        struct {
            /* word 0 */
            u32          reserv1     : 24;
            u32          gidxSel     : 8;
            /* word 1~6 */
            u32          reserv2[6];
        } IFSEL;
        struct {
            /* word 0 */
            IPaddr_t        sIPP;
            /* word 1 */
            IPaddr_t        sIPM;
            /* word 2 */
            IPaddr_t        dIPP;
            /* word 3 */
            IPaddr_t        dIPM;
            union {
                struct {
                    /* word 4 */
                    u8           IPProtoM;
                    u8           IPProtoP;
                    u8           IPTOSM;
                    u8           IPTOSP;
                    /* word 5 */
                    u32          reserv0     : 20;
                    u32          identSDIPM  : 1;
                    u32          identSDIPP  : 1;
                    u32          HTTPM       : 1;
                    u32          HTTPP       : 1;
                    u32          FOM         : 1;
                    u32          FOP         : 1;
                    u32          IPFlagM     : 3;
                    u32          IPFlagP     : 3;
                    /* word 6 */
                    u32          reserv1;
                } IP;
                struct {
                    /* word 4 */
                    u8           ICMPTypeM;
                    u8           ICMPTypeP;
                    u8           IPTOSM;
                    u8           IPTOSP;
                    /* word 5 */
                    u16          reserv0;
                    u8           ICMPCodeM;
                    u8           ICMPCodeP;
                    /* word 6 */
                    u32          reserv1;
                } ICMP;
                struct {
                    /* word 4 */
                    u8           IGMPTypeM;
                    u8           IGMPTypeP;
                    u8           IPTOSM;
                    u8           IPTOSP;
                    /* word 5,6 */
                    u32          reserv0[2];
                } IGMP;
                struct {
                    /* word 4 */
                    u8           TCPFlagM;
                    u8           TCPFlagP;
                    u8           IPTOSM;
                    u8           IPTOSP;
                    /* word 5 */
                    u16          TCPSPLB;
                    u16          TCPSPUB;
                    /* word 6 */
                    u16          TCPDPLB;
                    u16          TCPDPUB;
                } TCP;
                struct {
                    /* word 4 */
                    u16          reserv0;
                    u8           IPTOSM;
                    u8           IPTOSP;
                    /* word 5 */
                    u16          UDPSPLB;
                    u16          UDPSPUB;
                    /* word 6 */
                    u16          UDPDPLB;
                    u16          UDPDPUB;
                } UDP;
            } is;
        } L3L4;

        struct {
            /* word 0 */
            u16          sMacP31_16;
            u16          sMacP15_0;
            /* word 1 */
	     u16           reserv1:3;
	     u16		   spaP:9;
             u16         sMacM3_0:4;
            u16          sMacP47_32;
	/* word 2 */
	     u32	          reserv3:2;
            u32          sVidM:12;
	     u32          sVidP:12;
	     u32		   reserv2:6;
            /* word 3 */
            u32          reserv5     : 6;
            u32          protoType   : 2;
     	     u32          reserv4        : 24;
		/* word 4 */
            IPaddr_t        sIPP;
            /* word 5 */
            IPaddr_t        sIPM;
            /* word 6 */
            u16          SPORTLB;
            u16          SPORTUB;
        } SRC_FILTER;
        struct {
            /* word 0 */
            u16          dMacP31_16;
            u16          dMacP15_0;
            /* word 1 */
	     u16 	   vidM:12;	
            u16          dMacM3_0:4;
            u16          dMacP47_32;			
            /* word 2 */
	     u32          reserv2:20;
	     u32          vidP:12;			
            /* word 3 */
            u32          reserv4     : 24;
            u32          protoType   : 2;
	     u32          reserv3:6;
            /* word 4 */
            IPaddr_t        dIPP;
            /* word 5 */
            IPaddr_t        dIPM;
            /* word 6 */
            u16          DPORTLB;
            u16          DPORTUB;
        } DST_FILTER;

    } is;
    /* word 7 */
    u32          reserv0     : 5;
    u32          pktOpApp    : 3;
    u32          PPPoEIndex  : 3;
    u32          vid         : 3;
    u32          nextHop     : 10; //index of l2, next hop, or rate limit tables
    u32          actionType  : 4;
    u32          ruleType    : 4;
} acl_table_t;

#endif /* RTL865XC */


#endif /*_VLANTABLE_H_*/
