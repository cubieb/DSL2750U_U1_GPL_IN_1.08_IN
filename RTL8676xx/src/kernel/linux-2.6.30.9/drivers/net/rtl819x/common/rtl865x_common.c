#include "rtl_glue.h"
#include "rtl_types.h"
//#include "rtl_queue.h"
#include "mbuf.h"
#include "assert.h"
#include "rtl8651_aclLocal.h"
#include "types.h"
#include "rtl8651_hwPatch.h"		/* define for chip related spec */
#include "rtl865xC_tblAsicDrv.h"
#include <rtl865xc_swNic.h>
#include <rtl865x_common.h>
#include <rtl865x_layer2.h>
#include <rtl865x_layer2_local.h>
#include <rtl865x_layer3.h>
#include <rtl865x_layer3_local.h>
#include <rtl865x_layer4.h>
#include <rtl865x_layer4_local.h>
#ifndef  NETIF_NUMBER
#define NETIF_NUMBER 4
#endif

struct lr_cpu_stats lr_stats;


static int32 lr_vlan_init(void);
static int32 lr_vlan_create(struct rtl865x_vlanConfig *);
static int32 lr_vlan_remove(uint32);
static int32 lr_vlan_hash(uint16);
static int32 lr_vlan_set_mac(struct rtl865x_vlanConfig *vlanconfig);
static int32 lr_vlan_set_mtu(struct rtl865x_vlanConfig *vlanconfig);
static int32 lr_if_init(void);
static int32 lr_if_attach(struct rtl865x_vlanConfig *);
static int32 lr_if_detach(uint8 *);
static struct if_entry *lr_if_lookup(uint8 *, ipaddr_t );
static int32 lr_if_up(uint8 *, ipaddr_t , ipaddr_t, uint32, ether_addr_t * );
static int32 lr_if_down(uint8 *name);

static int32 lr_acl_init(void);
static int32 lr_acl_arrange(void);
static int32 lr_acl_add(uint32, _rtl8651_tblDrvAclRule_t *, enum ACL_FLAGS);
static int32 lr_acl_del(uint32, _rtl8651_tblDrvAclRule_t *, enum ACL_FLAGS);

static void rtl865x_lightromeLinkChange(uint32, int8);


extern int32 rtl8651_setAsicAclRule(uint32 index, _rtl8651_tblDrvAclRule_t *rule);
extern int32 rtl8651_getAsicAclRule(uint32 index, _rtl8651_tblDrvAclRule_t *rule);

extern int32 rtl8651_setAsicPvid(uint32 port, uint32 pvidx);
extern int32 rtl8651_getAsicPVlanId(uint32 port, uint32 *pvid);
#define rtl8651_getAsicPvid rtl8651_getAsicPVlanId

/*======================================
 *  VLAN Table - vlan_tbl
 *======================================*/

struct vlan_table vlan_tbl = {
	vlan_init:			lr_vlan_init,				/* Initialize VLAN table */
	vlan_create:		lr_vlan_create,			/* Create a vlan, total 4K vlan support */
	vlan_remove:		lr_vlan_remove,			/* Remove an existing vlan */
	vlan_hash:		lr_vlan_hash,				/* Mapping 4k vlan to 8-entry vlan space */

	vlan_asic_get:		rtl8651_getAsicVlan,		/* ASIC Interface: query asic vlan */
	vlan_asic_set:		rtl8651_setAsicVlan,		/* ASIC Interface: configure asic vlan */
	vlan_asic_del:		rtl8651_delAsicVlan,		/* ASIC Interface: remove asic vlan */

	vlan_set_mac:	lr_vlan_set_mac,
	vlan_set_mtu:		lr_vlan_set_mtu,
#ifdef CONFIG_RTL865XC
	intf_asic_set:         rtl8651_setAsicNetInterface,        /*ASIC INterface: configure asic net interface*/
	intf_asic_get:         rtl8651_getAsicNetInterface,      /*ASIC Interface: query asic net interface*/
#endif
};

 

static int32 lr_vlan_init(void)
{
	memset(TBLFIELD(vlan_tbl, vhash), 0, sizeof(struct vlan_entry)*VLAN_NUMBER);
	memset(TBLFIELD(vlan_tbl, asic_vtbl), 0, sizeof(uint16)*RTL8651_VLAN_NUMBER);
	return 0;
}

static int32 lr_vlan_create(struct rtl865x_vlanConfig *vlanconfig)
{
	rtl865x_tblAsicDrv_vlanParam_t vlan;
	rtl865x_tblAsicDrv_intfParam_t intf;
	struct vlan_entry *ventry;
	int32 retval;
	int32 idx;

	if (vlanconfig->vid==0 || vlanconfig->vid==0xfff)
		return (int32)RTL_INVVID;
	
	for (idx = 0; idx < RTL865XC_NETINTERFACE_NUMBER; idx++)
	{
		ventry = &TBLFIELD(vlan_tbl, vhash)[idx];
		if (ventry->valid == 1 && ventry->vid == vlanconfig->vid)
			return (int32)RTL_DUPENTRY;
		if (ventry->valid == 0)
			break;
	}

#ifdef CONFIG_HARDWARE_NAT_DEBUG
/*2007-12-19*/
	rtlglue_printf("%s:%d,idx is %d \n",__FUNCTION__,__LINE__,idx);
#endif

	if (idx == RTL865XC_NETINTERFACE_NUMBER)
		return (int32)RTL_NOBUFFER;

	ventry->valid			= 1;
	ventry->fid			= vlanconfig->fid;
	ventry->mbr			= vlanconfig->memPort;
	ventry->untagSet		= vlanconfig->untagSet;
	ventry->mac			= vlanconfig->mac;
	ventry->vid                  =vlanconfig->vid;

	memset(&vlan, 0, sizeof(rtl865x_tblAsicDrv_vlanParam_t));
	memset(&intf, 0, sizeof(rtl865x_tblAsicDrv_intfParam_t));
	vlan.memberPortMask = vlanconfig->memPort;
	vlan.untagPortMask = vlanconfig->untagSet;
	vlan.fid = vlanconfig->fid;

	intf.enableRoute = 1;
	intf.inAclEnd			= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.inAclStart		= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.outAclEnd		= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.outAclStart		= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.macAddr = vlanconfig->mac;
	intf.macAddrNumber = 1;
	intf.mtu = vlanconfig->mtu;
	intf.valid = 1;
	intf.vid = vlanconfig->vid;
#ifdef CONFIG_HARDWARE_NAT_DEBUG
/*2007-12-19*/
	rtlglue_printf("%s:%d,intf.vid is %d \n",__FUNCTION__,__LINE__,intf.vid);
#endif
	retval =  TBLFIELD(vlan_tbl, intf_asic_set)( RTL865XC_NETIFTBL_SIZE, &intf );
	if ( retval != RTL_SUCCESS ) return retval;

#ifdef CONFIG_HARDWARE_NAT_DEBUG
/*2007-12-19*/
	rtlglue_printf("%s:%d,lrconfig->vid is %d \n",__FUNCTION__,__LINE__,vlanconfig->vid);
#endif

	LR_CONFIG_CHECK(TBLFIELD(vlan_tbl, vlan_asic_set)(vlanconfig->vid, &vlan));
	return (int32)RTL_SUCCESS;
}

static int32 lr_vlan_remove(uint32 vid)
{
	struct if_entry *ife;
	int32 ifindex;

	ife = &TBLFIELD(if_tbl, if_hash)[0];
	for(ifindex=0; ifindex<NETIF_NUMBER; ifindex++, ife++)
		if (IF_UP(ife) && ife->vid==vid)
			return -1;
	TBLFIELD(vlan_tbl, vlan_asic_del)(vid);
	return 0;
}


static int32 lr_vlan_hash(uint16 vid)
{
	return (vid&(RTL8651_VLAN_NUMBER-1));
}

struct vlan_entry*  lr_get_vlan(uint32 vid)
{
	struct vlan_entry *ventry;
#ifdef CONFIG_RTL865XC  
	uint32 idx;
#endif
	if (vid==0 || vid==0xfff)
		return NULL;
	
#ifdef CONFIG_RTL865XC	
	for (idx = 0; idx < RTL865XC_NETINTERFACE_NUMBER; idx++)
	{
		ventry = &TBLFIELD(vlan_tbl, vhash)[idx];
		if ( ventry->vid == vid){
			if (ventry->valid == 1)
				return ventry;
			else
				return NULL;
		}
	}
#else
	ventry = &TBLFIELD(vlan_tbl, vhash)[vid];
#endif
	if (ventry->valid == 1)
		return ventry;
	else
		return NULL;

}

struct vlan_entry*  lr_get_vlan_byidx(uint32 vidx)
{
	struct vlan_entry *ventry;
	if (vidx < 0 || vidx > RTL8651_VLAN_NUMBER -1)
		return NULL;
#ifdef CONFIG_RTL865XC
	ventry = &TBLFIELD(vlan_tbl, vhash)[vidx];	
#else
	uint32 vid;
	vid = TBLFIELD(vlan_tbl, asic_vtbl)[vidx];
	if (vid < 0 || vid > VLAN_NUMBER)
		return NULL;
	ventry = &TBLFIELD(vlan_tbl, vhash)[vid];	
#endif
	if (ventry->valid == 1)
		return ventry;
	else
		return NULL;

}

static int32 lr_vlan_set_mac(struct rtl865x_vlanConfig *vlanconfig)
{
	rtl865x_tblAsicDrv_vlanParam_t vlan;
	rtl865x_tblAsicDrv_intfParam_t intf;
	struct vlan_entry *ventry;
	int32 retval;

	ventry=lr_get_vlan(vlanconfig->vid);
	if (ventry == NULL)
		return (int32)RTL_INVVID;

	ventry->mac=vlanconfig->mac;
	
	memset(&vlan, 0, sizeof(rtl865x_tblAsicDrv_vlanParam_t));
	memset(&intf, 0, sizeof(rtl865x_tblAsicDrv_intfParam_t));
	vlan.memberPortMask = ventry->mbr;
	vlan.untagPortMask = ventry->untagSet;
	vlan.fid = ventry->fid;

	intf.enableRoute = 1;
	intf.inAclEnd			= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.inAclStart		= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.outAclEnd		= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.outAclStart		= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.macAddr =ventry->mac;
	intf.macAddrNumber = 1;
	intf.mtu = vlanconfig->mtu;
	intf.valid = 1;
	intf.vid = ventry->vid;
	retval =  TBLFIELD(vlan_tbl, intf_asic_set)( RTL865XC_NETIFTBL_SIZE, &intf );
	if ( retval != RTL_SUCCESS ) return retval;
	LR_CONFIG_CHECK(TBLFIELD(vlan_tbl, vlan_asic_set)(vlanconfig->vid, &vlan));
	return (int32)RTL_SUCCESS;
}

static int32  lr_vlan_set_mtu(struct rtl865x_vlanConfig *vlanconfig)
{
	struct vlan_entry *ventry;
	struct if_entry *ife;
	int32 ifindex;
	
	rtl865x_tblAsicDrv_vlanParam_t vlan;
	rtl865x_tblAsicDrv_intfParam_t intf;
	
	int32 retval;

	ventry=lr_get_vlan(vlanconfig->vid);
	if (ventry == NULL)
		return (int32)RTL_INVVID;

	ife = &TBLFIELD(if_tbl, if_hash)[0];
	for(ifindex=0; ifindex<NETIF_NUMBER; ifindex++, ife++)
	{
		if(ife->vid==vlanconfig->vid)
		{
			ife->mtu=vlanconfig->mtu;
		}	
	}
	
	memset(&vlan, 0, sizeof(rtl865x_tblAsicDrv_vlanParam_t));
	memset(&intf, 0, sizeof(rtl865x_tblAsicDrv_intfParam_t));
	vlan.memberPortMask = ventry->mbr;
	vlan.untagPortMask = ventry->untagSet;
	vlan.fid = ventry->fid;
	
	intf.enableRoute = 1;
	intf.inAclEnd			= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.inAclStart		= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.outAclEnd		= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.outAclStart		= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
	intf.macAddr = ventry->mac;
	intf.macAddrNumber = 1;
	intf.mtu = vlanconfig->mtu;
	intf.valid = 1;
	intf.vid = ventry->vid;
	retval =  TBLFIELD(vlan_tbl, intf_asic_set)( RTL865XC_NETIFTBL_SIZE, &intf );
	if ( retval != RTL_SUCCESS ) return retval;
	LR_CONFIG_CHECK(TBLFIELD(vlan_tbl, vlan_asic_set)(vlanconfig->vid, &vlan));
	return (int32)RTL_SUCCESS;
}


/*======================================
 *  ACL Table - acl_tbl
 *======================================*/
struct acl_table acl_tbl = {
	acl_init:			lr_acl_init,				/* Initialize ACL table */
	acl_add:			lr_acl_add,				/* add ACL entry */
	acl_del:			lr_acl_del,				/* remove ACL entry */
	acl_arrange:		lr_acl_arrange,			/* arrange ACL entry to ASIC */

	acl_asic_set:		rtl8651_setAsicAclRule,		/* ASIC Interface: add acl entry to ASIC */
	acl_asic_get:		rtl8651_getAsicAclRule,		/* ASIC Interface: query acl entry */
};

static int32 lr_acl_init(void)
{	
	int32 i;
	
	memset(TBLFIELD(acl_tbl, free_list), 0, sizeof(_rtl8651_tblDrvAclRule_t)*RTL865x_ACL_NUMBER);
	for(i=0; i<RTL8651_VLAN_NUMBER; i++) {
		CTAILQ_INIT(&TBLFIELD(acl_tbl, acl_in_head)[i]);
		CTAILQ_INIT(&TBLFIELD(acl_tbl, acl_eg_head)[i]);
	}
	CTAILQ_INIT(&TBLFIELD(acl_tbl, free_head));
	for(i=0; i<RTL865x_ACL_NUMBER; i++) {
		memset(&TBLFIELD(acl_tbl, free_list)[i], 0, sizeof(_rtl8651_tblDrvAclRule_t));
		CTAILQ_INSERT_HEAD(&TBLFIELD(acl_tbl, free_head), 
				&(acl_tbl.free_list[i]), nextRule); 
	}
	SET_DEFAULT_ACL((ACL_PERMIT+DEFAULT_INC), RTL8651_ACL_PERMIT);
	SET_DEFAULT_ACL((ACL_DROP+DEFAULT_INC), RTL8651_ACL_DROP);
	SET_DEFAULT_ACL((ACL_CPU+DEFAULT_INC), RTL8651_ACL_CPU);
	TBLFIELD(acl_tbl, def_action) = ACL_PERMIT;
	TBLFIELD(acl_tbl, acl_arrange)();
	return 0;
}


static int32 lr_acl_add(uint32 vid, _rtl8651_tblDrvAclRule_t *rule, enum ACL_FLAGS flags)
{
	CTAILQ_HEAD(_aclStruct, _rtl8651_tblDrvAclRule_s) *aclHead;
	_rtl8651_tblDrvAclRule_t *acl_rule;
	int32 vhash;
#ifdef CONFIG_RTL865XC
	struct vlan_entry *ventry;
	vhash = rtl8651_vlanTableIndex(vid);
	ventry = &TBLFIELD(vlan_tbl, vhash)[vhash];
	if (ventry->vid != vid)
		return RTL_INVVID;
#else
	vhash = TBLFIELD(vlan_tbl, vlan_hash)(vid);
	if (vid == 0 || vid == 0xfff ||
	   TBLFIELD(vlan_tbl, asic_vtbl)[vhash] != vid)
		return RTL_INVVID;
#endif
	acl_rule = CTAILQ_FIRST(&TBLFIELD(acl_tbl, free_head));
	if ( acl_rule == NULL || TBLFIELD(acl_tbl, entry)>=RTL865x_ACL_NUMBER)
		return RTL_NOBUFFER;
	
	if (flags == ACL_INGRESS)
		aclHead = (struct _aclStruct *)&TBLFIELD(acl_tbl, acl_in_head)[vhash];
	else  aclHead = (struct _aclStruct *)&TBLFIELD(acl_tbl, acl_eg_head)[vhash];

	CTAILQ_REMOVE(&TBLFIELD(acl_tbl, free_head), 
			 acl_rule,  nextRule);

	memcpy(acl_rule, rule, sizeof(_rtl8651_tblDrvAclRule_t));
	CTAILQ_INSERT_TAIL(aclHead, acl_rule, nextRule);
	TBLFIELD(acl_tbl, acl_arrange)();
	return RTL_SUCCESS;
}


static int32 lr_acl_del(uint32 vid, _rtl8651_tblDrvAclRule_t *rule, enum ACL_FLAGS flags)
{
	CTAILQ_HEAD(_aclStruct, _rtl8651_tblDrvAclRule_s) *aclHead;
	_rtl8651_tblDrvAclRule_t *acl_rule;
	int32 vhash;

#ifdef CONFIG_RTL865XC
	struct vlan_entry *ventry;
	vhash = rtl8651_vlanTableIndex(vid);
	ventry = &TBLFIELD(vlan_tbl, vhash)[vhash];
	if (ventry->vid != vid)
		return RTL_INVVID;
#else
	vhash = TBLFIELD(vlan_tbl, vlan_hash)(vid);
	if (vid == 0 || vid == 0xfff ||
	   TBLFIELD(vlan_tbl, asic_vtbl)[vhash] != vid)
		return RTL_INVVID;
#endif
	if (flags == ACL_INGRESS)
		aclHead = (struct _aclStruct *)&TBLFIELD(acl_tbl, acl_in_head)[vhash];
	else aclHead = (struct _aclStruct *)&TBLFIELD(acl_tbl, acl_eg_head)[vhash];
		
	CTAILQ_FOREACH(acl_rule, aclHead, nextRule) {
		if (memcmp(rule, acl_rule, 
		    sizeof(_rtl8651_tblDrvAclRule_t)-sizeof(rule->nextRule)))
		    continue;
		/* found, remove it! */
		CTAILQ_REMOVE(aclHead, acl_rule, nextRule);
		CTAILQ_INSERT_HEAD(&TBLFIELD(acl_tbl, free_head), acl_rule, nextRule);
		TBLFIELD(acl_tbl, acl_arrange)();
		return RTL_SUCCESS;
	}
	return RTL_NOTFOUND;
}

#ifdef CONFIG_RTL865XC
static int32 lr_acl_arrange(void)
{
	CTAILQ_HEAD(_aclStruct, _rtl8651_tblDrvAclRule_s) *aclHead;
	rtl865x_tblAsicDrv_intfParam_t netIf;	
	_rtl8651_tblDrvAclRule_t *acl_rule;
	int32 vhash, acl_start=0, acl_end=0, egin;
	uint16 vid;
	
	TBLFIELD(acl_tbl, entry) = 3;
	for(vhash=0; vhash<RTL8651_VLAN_NUMBER; vhash++) {
		struct vlan_entry *ventry;
		ventry = &TBLFIELD(vlan_tbl, vhash)[vhash];
		if ( ventry->valid == 0 || !(vid = ventry->vid))
			continue;
		netIf.vid = vid;
		TBLFIELD(vlan_tbl, intf_asic_get)(RTL865XC_NETIFTBL_SIZE, &netIf);

		for(egin=0; egin<2; egin++) {
			aclHead = (!egin)? (struct _aclStruct *)&TBLFIELD(acl_tbl, acl_in_head)[vhash]:
					(struct _aclStruct *)&TBLFIELD(acl_tbl, acl_eg_head)[vhash];
			CTAILQ_FOREACH(acl_rule, aclHead, nextRule) {
				TBLFIELD(acl_tbl, acl_asic_set)(acl_end, acl_rule);
				TBLFIELD(acl_tbl, entry) ++;
				acl_end ++;
			}
			if (acl_end == acl_start) {	/* no rule */	
				if (!egin) {
					netIf.inAclEnd	 	= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
					netIf.inAclStart 	= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
				} else {
					netIf.outAclEnd 	= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
					netIf.outAclStart	= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
				}
			}
			else {
				SET_DEFAULT_ACL(acl_end, TBLFIELD(acl_tbl, def_action));
				if (!egin) {
					netIf.inAclEnd		= acl_end;
					netIf.inAclStart	= acl_start;
				} else {
					netIf.outAclEnd	= acl_end;
					netIf.outAclStart	= acl_start;
				}
				acl_start = acl_end + 1;
				acl_end = acl_start;
				TBLFIELD(acl_tbl, entry) ++;
			}
			TBLFIELD(vlan_tbl, intf_asic_set)(RTL865XC_NETIFTBL_SIZE, &netIf);
		}
	}
	return 0;
	
}
#else
static int32 lr_acl_arrange(void)
{
	CTAILQ_HEAD(_aclStruct, _rtl8651_tblDrvAclRule_s) *aclHead;
	rtl865x_tblAsicDrv_vlanParam_t vlan;
	_rtl8651_tblDrvAclRule_t *acl_rule;
	int32 vhash, acl_start=0, acl_end=0, egin;
	uint16 vid;

	TBLFIELD(acl_tbl, entry) = 3;
	for(vhash=0; vhash<RTL8651_VLAN_NUMBER; vhash++) {		
		if (!(vid=TBLFIELD(vlan_tbl, asic_vtbl)[vhash]))
			continue;
		TBLFIELD(vlan_tbl, vlan_asic_get)(vid, &vlan);

		for(egin=0; egin<2; egin++) {
			aclHead = (!egin)? (struct _aclStruct *)&TBLFIELD(acl_tbl, acl_in_head)[vhash]:
					(struct _aclStruct *)&TBLFIELD(acl_tbl, acl_eg_head)[vhash];
			CTAILQ_FOREACH(acl_rule, aclHead, nextRule) {
				TBLFIELD(acl_tbl, acl_asic_set)(acl_end, acl_rule);
				TBLFIELD(acl_tbl, entry) ++;
				acl_end ++;
			}
			if (acl_end == acl_start) {	/* no rule */	
				if (!egin) {
					vlan.inAclEnd	 	= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
					vlan.inAclStart 	= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
				} else {
					vlan.outAclEnd 	= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
					vlan.outAclStart	= TBLFIELD(acl_tbl, def_action) + DEFAULT_INC;
				}
			}
			else {
				SET_DEFAULT_ACL(acl_end, TBLFIELD(acl_tbl, def_action));
				if (!egin) {
					vlan.inAclEnd		= acl_end;
					vlan.inAclStart	= acl_start;
				} else {
					vlan.outAclEnd	= acl_end;
					vlan.outAclStart	= acl_start;
				}
				acl_start = acl_end + 1;
				acl_end = acl_start;
				TBLFIELD(acl_tbl, entry) ++;
			}
			TBLFIELD(vlan_tbl, vlan_asic_set)(&vlan);
		}
	}
	return 0;
}
#endif



#ifdef CONFIG_RTL865XC
/*
@func uint32 | rtl8651_vlanTableIndex | Get VLAN table index for given VID
@parm uint16 | vid | VLAN ID.
@rvalue Index | Index in Rome Driver VLAN table for given VID.
@comm
Lookup utility. Use VLAN ID to find the corresponding VLAN table index in Rome Driver.
This API would return RTL865XC_NETINTERFACE_NUMBER if the VID does not exist in Rome Driver VLAN table.
*/
uint32 rtl8651_vlanTableIndex(uint16 vid)
{
	uint32 idx;
	struct vlan_entry *ventry;


	for ( idx = 0, ventry = &TBLFIELD(vlan_tbl, vhash)[0]; idx < RTL865XC_NETINTERFACE_NUMBER; idx++, ventry++ )
	{
		if ( ventry->valid == 1 && ventry->vid == vid )
			break;
	}

	return idx;
}
 #endif


/*======================================
 *  Netwrok Interface Table - if_tbl
 *======================================*/
struct if_table if_tbl = {
	if_init:			lr_if_init,					/* Initialize if_table */
	if_attach:			lr_if_attach,				/* Create a network interface and bind to a specified vlan */
	if_detach:		lr_if_detach,				/* Remove an existing network interface and unbind from vlan */
	if_lookup:		lr_if_lookup,				/* Look up a specified network */
	if_up:			lr_if_up,					/* Assign IP/session to an interface */
	if_down:			lr_if_down,				/* Remove IP/session from an interface */
};

static int32 lr_if_init(void)
{
	memset(TBLFIELD(if_tbl, if_hash), 0, sizeof(struct if_entry)*NETIF_NUMBER);
	return 0;
}


static int32 lr_if_attach(struct rtl865x_vlanConfig *vlanconfig)
{
	struct if_entry *ife;
	int32 ifindex;

	if ( vlanconfig->ifname[0]=='\0' ||lr_if_lookup(vlanconfig->ifname, 0) )
		return (int32)RTL_DUPENTRY;

	ife = &TBLFIELD(if_tbl, if_hash)[0];
	for(ifindex=0; ifindex<NETIF_NUMBER; ifindex++, ife++)
		if (ife->name[0] == '\0')
			break;
	if (ifindex == NETIF_NUMBER)
		return (int32)RTL_NOBUFFER;

	memcpy(ife->name, vlanconfig->ifname, MAX_IFNAMESIZE);
	ife->ipaddr_			= 0;
	ife->mask_			= 0;
	ife->if_type			= vlanconfig->if_type;
	ife->mtu				= vlanconfig->mtu;
	ife->vid				= vlanconfig->vid;
#ifdef CONFIG_RTL865XC
	ife->isWan                   = vlanconfig->isWan;
#endif
//	ife->fid				= vlanconfig->fid;
//	ife->mac				= vlanconfig->mac;
	return (int32)RTL_SUCCESS;
}	


static int32 lr_if_detach(uint8 *name)
{
	return 0;
}


static struct if_entry *lr_if_lookup(uint8 *name, ipaddr_t ip)
{
	struct if_entry *ife;
	int32 ifindex;

	ife = &TBLFIELD(if_tbl, if_hash)[0];
	for(ifindex=0; ifindex<NETIF_NUMBER; ifindex++, ife++) {
		if ( (name&&ife->name[0] == '\0') ||
			(ip&&(ife->ipaddr_!=(ip&ife->mask_))) )		
			continue;
		if ( (name&&!strcmp(ife->name, name)) || 
			(IF_UP(ife)&&ip&&(ife->ipaddr_==(ip&ife->mask_))) ) {
			return ife;
		}
	}
	return (struct if_entry *)0;
}


static int32 lr_if_up(uint8 *name, ipaddr_t ip, ipaddr_t mask, uint32 sid, ether_addr_t *mac)
{
	struct if_entry *ife;
	
	ife = TBLFIELD(if_tbl, if_lookup)(name, 0);
	if (ife == NULL || IF_UP(ife))
		return RTL_INVIF;
	if (ife->if_type == IF_ETHER) {
		ife->ipaddr_ = ip&mask;
		ife->mask_  = mask;	
#ifdef CONFIG_RTL865X_LAYERED_DRIVER
#else
		if (TBLFIELD(arpt_tbl, arp_tbl_alloc)(ife)) {
			ife->ipaddr_ = 0;
			ife->mask_  = 0;	
			return (int32)RTL_NOARPSPACE;
		}
#endif		
	} else {
		ife->sid_ = sid;
		ife->pmac_ = *mac;
	}
	ife->up = 1;
	return (int32)RTL_SUCCESS;
}


static int32 lr_if_down(uint8 *name)
{
	struct if_entry *ife;

	ife = TBLFIELD(if_tbl, if_lookup)(name, 0);
	if (ife == NULL || !IF_UP(ife))
		return RTL_INVIF;
	if (ife->if_type == IF_ETHER) {
#ifdef CONFIG_RTL865X_LAYERED_DRIVER
#else
		TBLFIELD(arpt_tbl, arp_tbl_free)(ife);
		ife->ipaddr_ = 0;
		ife->mask_  = 0;
#endif		
	}
	else ife->sid_ = 0;
	ife->up = 0;
	return SUCCESS;
}




/*======================================
 *  Port Attribute Table - port_attr
 *======================================*/
struct port_attribute port_attr = {
	activePortMask:	0,
	link_change:		rtl865x_lightromeLinkChange,/* Link change callback function */
	
	pvid_asic_set:	rtl8651_setAsicPvid,		/* ASIC Interface: congiure pvid */
	pvid_asic_get:	rtl8651_getAsicPvid,		/* ASIC Interface: query pvid */
};


static void rtl865x_lightromeLinkChange(uint32 port, int8 linkUp)
{
	port_attr.activePortMask = 
			(linkUp==TRUE)? 
			( port_attr.activePortMask | (1<<port) ) : 
			( port_attr.activePortMask & ~(1<<port) );
}


enum RTL_RESULT rtl865x_addAclRule(uint32 vid, rtl865x_aclRule_t *rule_t, enum ACL_FLAGS flags) 
{
	_rtl8651_tblDrvAclRule_t rule;
	int32 rc;

	if (rule_t->actionType_ > 0x03)
		return RTL_INVAPARAM;	    
	memset(&rule, 0, sizeof(_rtl8651_tblDrvAclRule_t));
	memcpy(&rule, rule_t, sizeof(rtl865x_aclRule_t));
	rc = TBLFIELD(acl_tbl, acl_add)(vid, &rule, flags);
	return rc;
}


enum RTL_RESULT rtl865x_delAclRule(uint32 vid, rtl865x_aclRule_t *rule_t, enum ACL_FLAGS flags)
{
	_rtl8651_tblDrvAclRule_t rule;
	int32 rc;
	if (rule_t->actionType_ > 0x03)
		return RTL_INVAPARAM;
	memset(&rule, 0, sizeof(_rtl8651_tblDrvAclRule_t));
	memcpy(&rule, rule_t, sizeof(rtl865x_aclRule_t));
	rc = TBLFIELD(acl_tbl, acl_del)(vid, &rule, flags);
	return rc;
}



enum RTL_RESULT rtl865x_setAclDefaultAction(enum ACL_ACFLAGS defAction)
{
	if (defAction == TBLFIELD(acl_tbl, def_action))
		return RTL_SUCCESS;
	TBLFIELD(acl_tbl, def_action) = defAction;
	TBLFIELD(acl_tbl, acl_arrange)();
	return RTL_SUCCESS;
}

int  rtl865x_netif_isWan( uint8* ifname )
{
	struct if_entry *ife0;

	ife0 = TBLFIELD(if_tbl, if_lookup)(ifname, 0);
	if (!ife0)
		return 0;

	return (ife0->isWan? 1:0);
}

