
/*      @doc RTL_LAYEREDDRV_API

        @module rtl865x_outputQueue.c - RTL865x Home gateway controller Layered driver API documentation       |
        This document explains the API interface of the table driver module. Functions with rtl865x prefix
        are external functions.
        @normal Hyking Liu (Hyking_liu@realsil.com.cn) <date>

        Copyright <cp>2008 Realtek<tm> Semiconductor Cooperation, All Rights Reserved.

        @head3 List of Symbols |
        Here is a list of all functions and variables in this module.
        
        @index | RTL_LAYEREDDRV_API
*/
#include <linux/config.h>
#include <net/rtl/rtl_types.h>
#include <net/rtl/rtl_glue.h>
#include <common/rtl8651_tblDrvProto.h>
#include <common/rtl865x_eventMgr.h>
#include <common/rtl865x_vlan.h>
#include <net/rtl/rtl865x_netif.h>
#include <common/rtl865x_netif_local.h>
#include <net/rtl/rtl865x_outputQueue.h>
//#include "assert.h"
//#include "rtl_utils.h"
#include <common/rtl_errno.h>
#if defined (CONFIG_RTL_LOCAL_PUBLIC)
#include <l3Driver/rtl865x_localPublic.h>
#endif




#ifdef CONFIG_RTL_LAYERED_ASIC_DRIVER
#include <AsicDriver/asicRegs.h>
#include <AsicDriver/rtl865x_asicCom.h>
#include <AsicDriver/rtl865x_asicL2.h>
#else
#include <AsicDriver/asicRegs.h>
#include <AsicDriver/rtl8651_tblAsicDrv.h>
#endif


/* Warning ! If you want to create more than 4 output queues , you have to decrese Per-Queue physical length gap Register (0xBB80-4500)
     In order to let pkts enqueue into higher queues (Queue4 , 5) */
#define UPSTREAM_Quene_NUM			4
#define DOWNSTREAM_Quene_NUM		UPSTREAM_Quene_NUM-1 /* It is has to be different with UPSTREAM_Quene_NUM !!*/
#define Default_ACLPriority		0


rtl865x_qos_rule_t		*rtl865x_qosRuleHead = NULL;
static uint8    priorityDecisionArray[] = {	1,		/* port base */
									4,		/*         802.1p base */ 
									1,		/*         dscp base */                   
									8,		/*         acl base */    
									1		/* nat base */
								};

/* this setting will be overwirited when settting IPQoS in run time */
static uint8    priorityMatrix[RTL8651_OUTPUTQUEUE_SIZE][TOTAL_VLAN_PRIORITY_NUM] =
                                                                {{0,0,0,0,0,0,0,0},
                                                                {0,0,0,0,5,5,5,5},
                                                                {0,0,0,0,1,1,5,5},
                                                                {0,0,0,1,2,2,5,5},
                                                                {0,0,0,1,2,3,5,5},
                                                                {0,0,1,2,3,4,5,5}};

typedef struct aclpriority_mapping_data_s
{
	uint8 valid;
	uint8 Qid;
	uint32 remark_8021p;
	uint32 remark_dscp;
} aclpriority_mapping_data_t;
static aclpriority_mapping_data_t aclpriority_mapping[NETIF_NUMBER][TOTAL_VLAN_PRIORITY_NUM];
static int32 defPriority[NETIF_NUMBER] = {0};
static uint32 Qid2HandleMapping[NETIF_NUMBER][RTL8651_OUTPUTQUEUE_SIZE] = {{0}};



/*   Common private func.  */
static int8	(*rtl865x_compFunc)(rtl865x_qos_t	*entry1, rtl865x_qos_t	*entry2);

//static void _rtl865x_SetQueueNum(char* netifname, uint32 qnum);
static rtl865x_netif_local_t* _rtl865x_getQosNetif(char* ifname);
static int32 _rtl865x_qosArrangeRuleByNetif(uint8 *netIfName);
static int32 __inner_qosArrangeRuleByNetif(uint8 *netIfName);
static int8 _rtl865x_sameQosRule(rtl865x_qos_rule_t *rule1, rtl865x_qos_rule_t *rule2);

static int32 _rtl865x_qosQIDByHandleMappingGet(char* netifname, uint32 handle);
static int _rtl865x_qosQIDHandleMappingSet(char* netifname, uint32 handle[RTL8651_OUTPUTQUEUE_SIZE]);
static void _rtl865x_qosQIDHandleMappingClear(char* netifname);

static int _rtl865x_qosACLPriorityMappingGet(char* netifname,uint32 handler,uint32 remark_8021p,uint32 remark_dscp);
static int _rtl865x_qosACLPriorityMappingSetDefault(char* netifname,uint32 handler,uint32 remark_8021p,uint32 remark_dscp);
static void _rtl865x_qosACLPriorityMappingClearDefault(char* netifname);
static void _rtl865x_qosACLPriorityMappingClear(char* netifname);
static int _rtl865x_qosACLPriorityMappingTakeEffect(char* netifname);


void rtl865x_qosShowDebugInfo(int idx)
{
	int i;
	
	printk("Queue Idx <----> handler : ");
	for(i=0;i<RTL8651_OUTPUTQUEUE_SIZE;i++)
		printk("0x%08X  ",Qid2HandleMapping[idx][i]);
	printk("\n");

	printk("acl priority <----> \tQueue Idx  \tRemark_802.1p \tRemark_DSCP  \n");
	for(i=0;i<TOTAL_VLAN_PRIORITY_NUM;i++)
		printk("%d[%s] \t\t\t%d \t\t0x%X \t\t0x%X\n"
		,i,aclpriority_mapping[idx][i].valid?"Y":"N"
		,aclpriority_mapping[idx][i].Qid
		,aclpriority_mapping[idx][i].remark_8021p
		,aclpriority_mapping[idx][i].remark_dscp);

	printk("def acl priority : %d\n",defPriority[idx]);
	
}
	
int32 rtl865x_qosSetBandwidth(uint8 *netIfName, uint32 bps)
{
	uint32	memberPort, wanMemberPort;
	rtl865x_netif_local_t	*netIf, *wanNetIf;
	rtl865x_vlan_entry_t	*vlan;
	uint32	port;
	uint32	asicBandwidth;
	uint32	wanPortAsicBandwidth;


	netIf = _rtl865x_getQosNetif(netIfName);

	
	if(netIf == NULL)
		return FAILED;
	vlan = _rtl8651_getVlanTableEntry(netIf->vid);
	if(vlan == NULL)
		return FAILED;


	memberPort = vlan->memberPortMask;

	///////////////////////////////////////////////
	/*	Egress bandwidth granularity was 64Kbps	*/
	asicBandwidth = bps>>EGRESS_BANDWIDTH_GRANULARITY_BITLEN;
	if (asicBandwidth>0 && (bps&(1<<(EGRESS_BANDWIDTH_GRANULARITY_BITLEN-1)))==1)
	{
		asicBandwidth++;
	}

#if defined(CONFIG_RTL_PUBLIC_SSID)
	if(strcmp(netIf->name,RTL_GW_WAN_DEVICE_NAME) == 0)
#else
	if(strcmp(netIf->name,RTL_DRV_WAN0_NETIF_NAME)==0)
#endif
	{
		//Adjust for wan port egress asic bandwidth
		asicBandwidth+=3;
	}
	if(strcmp(netIf->name,RTL_DRV_LAN_NETIF_NAME)==0)
	{
		//Adjust for lan port egress asic bandwidth
		asicBandwidth++;
	}
	////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////
	/*	Ingress bandwidth granularity was 16Kbps	*/
	wanPortAsicBandwidth=bps>>INGRESS_BANDWIDTH_GRANULARITY_BITLEN;
	if (wanPortAsicBandwidth>0 && (bps&(1<<(INGRESS_BANDWIDTH_GRANULARITY_BITLEN-1)))==1)
	{
		wanPortAsicBandwidth++;
	}

	//Adjust for wan port ingress asic bandwidth
	wanPortAsicBandwidth+=5;

	if(strcmp(netIf->name,RTL_DRV_LAN_NETIF_NAME)==0)
	{
		//To set wan port ingress asic bandwidth
#if defined(CONFIG_RTL_PUBLIC_SSID)
		wanNetIf= _rtl865x_getNetifByName(RTL_GW_WAN_DEVICE_NAME);
#else
		wanNetIf= _rtl865x_getNetifByName(RTL_DRV_WAN0_NETIF_NAME);
#endif
		if(wanNetIf != NULL)
		{
			wanMemberPort=rtl865x_getVlanPortMask(wanNetIf->vid);
			for(port=0;port<=CPU;port++)
			{
				if(((1<<port)&wanMemberPort)==0)
					continue;
				rtl8651_setAsicPortIngressBandwidth(port,wanPortAsicBandwidth);
			}
		}
	}
	////////////////////////////////////////////////////////////////
	
	rtl865xC_lockSWCore();
	for(port=0;port<=CPU;port++)
	{
		if(((1<<port)&memberPort)==0)
			continue;
		rtl8651_setAsicPortEgressBandwidth(port, asicBandwidth);
	}
	rtl865xC_waitForOutputQueueEmpty();
	rtl8651_resetAsicOutputQueue();
	rtl865xC_unLockSWCore();
	return SUCCESS;
}

int32 rtl865x_qosFlushBandwidth(uint8 *netIfName)
{
	uint32	memberPort;
	rtl865x_netif_local_t	*netIf;
	rtl865x_vlan_entry_t	*vlan;
	uint32	port;	


	netIf = _rtl865x_getQosNetif(netIfName);


	if(netIf == NULL)
		return FAILED;
	
	vlan = _rtl8651_getVlanTableEntry(netIf->vid);
	if(vlan == NULL)
		return FAILED;

	memberPort = vlan->memberPortMask;

	rtl865xC_lockSWCore();
	for(port=0;port<=CPU;port++)
	{
		if(((1<<port)&memberPort)==0)
			continue;
		rtl8651_setAsicPortEgressBandwidth(port, APR_MASK>>APR_OFFSET);

		//To flush wan port Ingress bandwidth limit
//		if(strcmp(netIf->name,"eth1")==0)
		{
			rtl8651_setAsicPortIngressBandwidth(port,0);
		}
	}

	rtl865x_raiseEvent(EVENT_FLUSH_QOSRULE, NULL);
	
	rtl865xC_waitForOutputQueueEmpty();
	rtl8651_resetAsicOutputQueue();
	rtl865xC_unLockSWCore();
	return SUCCESS;
}

static int32 _rtl865x_qosQIDByHandleMappingGet(char* netifname, uint32 handle)
{
	int	j;
	int32 idx;

	if(!rtl865x_netifExist(netifname))
		return -1;

	idx = _rtl865x_getNetifIdxByNameExt(netifname);
	if(idx==-1)
		return -1;

	for(j=0; j < TOTAL_VLAN_PRIORITY_NUM; j++)
	{
		if (Qid2HandleMapping[idx][j] == handle)
		{
			return j;
		}
	}

	return -1;
}


static int _rtl865x_qosQIDHandleMappingSet(char* netifname, uint32 handle[RTL8651_OUTPUTQUEUE_SIZE])
{
	int32  idx;
	if(!rtl865x_netifExist(netifname))
		return FAILED;	

	idx= _rtl865x_getNetifIdxByNameExt(netifname);
	if(idx==-1)
		return FAILED;	

	if(rtl865x_netif_is_wan(netifname))
	{
		ForEachMasterWanNetif_Declaration
		ForEachMasterWanNetif_Start
		idx = _rtl865x_getNetifIdxByNameExt(netif->name);
		memcpy(Qid2HandleMapping[idx],handle,RTL8651_OUTPUTQUEUE_SIZE*sizeof(int));
		ForEachMasterWanNetif_End
	}
	else		
		memcpy(Qid2HandleMapping[idx],handle,RTL8651_OUTPUTQUEUE_SIZE*sizeof(int));
	
	return SUCCESS;
}


static void _rtl865x_qosQIDHandleMappingClear(char* netifname)
{
	int32  idx;
	if(!rtl865x_netifExist(netifname))
		return;	

	idx= _rtl865x_getNetifIdxByNameExt(netifname);
	if(idx==-1)
		return;

	if(rtl865x_netif_is_wan(netifname))
	{
		ForEachMasterWanNetif_Declaration
		ForEachMasterWanNetif_Start
		idx = _rtl865x_getNetifIdxByNameExt(netif->name);
		memset(Qid2HandleMapping[idx], 0, RTL8651_OUTPUTQUEUE_SIZE*sizeof(int));
		ForEachMasterWanNetif_End
	}
	else		
		memset(Qid2HandleMapping[idx], 0, RTL8651_OUTPUTQUEUE_SIZE*sizeof(int));
}


/* return ...     -1:   no avaliable acl priority mapping   others: acl priority  */
static int _rtl865x_qosACLPriorityMappingGet(char* netifname,uint32 handler,uint32 remark_8021p,uint32 remark_dscp)
{
	int32  idx,i,empty_idx,queue_idx;

	if(!rtl865x_netifExist(netifname))
		return -1;

	idx= _rtl865x_getNetifIdxByNameExt(netifname);
	if(idx==-1)
		return -1;

	queue_idx = _rtl865x_qosQIDByHandleMappingGet(netifname,handler);
	if(queue_idx==-1)
		return -1;		

	for(i=0;i<TOTAL_VLAN_PRIORITY_NUM;i++)
	{
		if( aclpriority_mapping[idx][i].valid 
			&& aclpriority_mapping[idx][i].Qid 			== queue_idx
			&& aclpriority_mapping[idx][i].remark_8021p	== remark_8021p
			&& aclpriority_mapping[idx][i].remark_dscp	== remark_dscp)
			return i;
	}

	/* cannot find out matching entry, create the new one*/
	empty_idx = -1;
	for(i=0;i<TOTAL_VLAN_PRIORITY_NUM;i++)
	{
		if(aclpriority_mapping[idx][i].valid==0)
		{
			empty_idx = i;
			break;
		}
	}
	if(empty_idx==-1)
		return -1; /* no empty entry */

	if(rtl865x_netif_is_wan(netifname))
	{
		/*  all WAN interface apply the same priority2HandleMapping 
		     (because all WAN interface use the same WAN port)
		*/
		ForEachMasterWanNetif_Declaration

		//sanity check
		ForEachMasterWanNetif_Start
		idx = _rtl865x_getNetifIdxByNameExt(netif->name);
		if(aclpriority_mapping[idx][empty_idx].valid)		
			return -1; /* bug */	
		ForEachMasterWanNetif_End


		ForEachMasterWanNetif_Start
		idx = _rtl865x_getNetifIdxByNameExt(netif->name);
		aclpriority_mapping[idx][empty_idx].valid 			= 1	;
		aclpriority_mapping[idx][empty_idx].Qid 			= queue_idx;
		aclpriority_mapping[idx][empty_idx].remark_8021p 	= remark_8021p;
		aclpriority_mapping[idx][empty_idx].remark_dscp 	= remark_dscp;
		ForEachMasterWanNetif_End
	}
	else
	{
		aclpriority_mapping[idx][empty_idx].valid 			= 1	;
		aclpriority_mapping[idx][empty_idx].Qid 			= queue_idx;
		aclpriority_mapping[idx][empty_idx].remark_8021p 	= remark_8021p;
		aclpriority_mapping[idx][empty_idx].remark_dscp 	= remark_dscp;
	}

	return empty_idx;
	
}

/*  clear all acl priority mapping (expect dor default priority) */
static void _rtl865x_qosACLPriorityMappingClear(char* netifname)
{
	int32  idx,i;
	if(!rtl865x_netifExist(netifname))
		return;

	idx = _rtl865x_getNetifIdxByNameExt(netifname);
	if(idx==-1)
		return;

	for(i=0;i<TOTAL_VLAN_PRIORITY_NUM;i++)
	{
		if(i==defPriority[idx])
			continue;
		
		if(rtl865x_netif_is_wan(netifname))
		{
			ForEachMasterWanNetif_Declaration
			ForEachMasterWanNetif_Start
			idx = _rtl865x_getNetifIdxByNameExt(netif->name);
			memset(&aclpriority_mapping[idx][i], 0,sizeof(aclpriority_mapping_data_t));
			ForEachMasterWanNetif_End
		}
		else		
			memset(&aclpriority_mapping[idx][i], 0,sizeof(aclpriority_mapping_data_t));	
		
	}
	
}


/* Note. it is recommend to lock 0412 switch core before use this funcion */
static int _rtl865x_qosACLPriorityMappingTakeEffect(char* netifname)
{	
	int32  idx,i;	

	if(!rtl865x_netifExist(netifname))
		return FAILED;

	idx= _rtl865x_getNetifIdxByNameExt(netifname);
	if(idx==-1)
		return FAILED;
	
	for(i=0;i<TOTAL_VLAN_PRIORITY_NUM;i++)		
	{
		if(rtl865x_netif_is_wan(netifname))
		{
			if(rtl8651_setAsicPriorityToQIDMappingTable(UPSTREAM_Quene_NUM, i, aclpriority_mapping[idx][i].Qid)==FAILED)
				return FAILED;
			if(rtl8651_SetPortRemark(i,aclpriority_mapping[idx][i].remark_8021p,aclpriority_mapping[idx][i].remark_dscp)==FAILED)
				return FAILED;
		}
		else
		{
			if(rtl8651_setAsicPriorityToQIDMappingTable(DOWNSTREAM_Quene_NUM, i, aclpriority_mapping[idx][i].Qid)==FAILED)
				return FAILED;
			/* Downstream cannot do remarking*/
		}
	}

	return SUCCESS;
	
}

static int _rtl865x_qosACLPriorityMappingSetDefault(char* netifname,uint32 handler,uint32 remark_8021p,uint32 remark_dscp)
{
	int32  idx,default_priority;	
	
	if(!rtl865x_netifExist(netifname))
		return FAILED;	

	idx= _rtl865x_getNetifIdxByNameExt(netifname);
	if(idx==-1)
		return FAILED;

	default_priority=_rtl865x_qosACLPriorityMappingGet(netifname,handler,remark_8021p,remark_dscp);

	/* we have to assure that default_priority taken from aclpriority_mapping is equal to Default_ACLPriority */
	if(default_priority!=Default_ACLPriority)
		return FAILED;

	if(rtl865x_netif_is_wan(netifname))
	{
		ForEachMasterWanNetif_Declaration
		ForEachMasterWanNetif_Start
		idx = _rtl865x_getNetifIdxByNameExt(netif->name);
		defPriority[idx] = default_priority;
		ForEachMasterWanNetif_End
	}
	else
		defPriority[idx] = default_priority;
	
	return SUCCESS;
}

static void _rtl865x_qosACLPriorityMappingClearDefault(char* netifname)
{
	int32  idx;
	if(!rtl865x_netifExist(netifname))
		return;

	idx = _rtl865x_getNetifIdxByNameExt(netifname);
	if(idx==-1)
		return;	

	if(rtl865x_netif_is_wan(netifname))
	{
		ForEachMasterWanNetif_Declaration
		ForEachMasterWanNetif_Start
		idx = _rtl865x_getNetifIdxByNameExt(netif->name);
		if(defPriority[idx]<0)
			return;
		memset(&aclpriority_mapping[idx][defPriority[idx]], 0,sizeof(aclpriority_mapping_data_t));
		defPriority[idx] = -1;
		ForEachMasterWanNetif_End
	}
	else
	{
		if(defPriority[idx]<0)
			return;
		memset(&aclpriority_mapping[idx][defPriority[idx]], 0,sizeof(aclpriority_mapping_data_t));
		defPriority[idx] = -1;
	}

}

#if 1
/*
	_rtl865x_compare2Entry Return Value:
		>0		means entry1 > entry2
		<0		means entry1 < entry2
		=0		means entry1 = entry2
*/
static int8 _rtl865x_compare2Entry(rtl865x_qos_t	*entry1, rtl865x_qos_t	*entry2)
{
	if (entry1->handle>entry2->handle)
		return 1;
	else if (entry1->handle<entry2->handle)
		return -1;
	else
		return 0;
}

int32	rtl865x_registerQosCompFunc(int8 (*p_cmpFunc)(rtl865x_qos_t	*entry1, rtl865x_qos_t	*entry2))
{
	if (p_cmpFunc==NULL)
		rtl865x_compFunc = _rtl865x_compare2Entry;
	else
		rtl865x_compFunc = p_cmpFunc;

	return SUCCESS;
}

static int my_gcd(int numA, int numB)
{
	int	tmp;
	int	divisor;

	if (numA<numB)
	{
		tmp = numA;
		numA = numB;
		numB = tmp;
	}

	divisor = numA%numB;
	while(divisor)
	{
		numA = numB;
		numB = divisor;
		divisor = numA%numB;
	}

	return numB;
}

static void _rtl865x_qosArrangeQueue(rtl865x_qos_t *qosInfo)
{
	int32	queueNum;


	/*	Process the queue type & ratio	*/
	{
		int			divisor;

		if ((qosInfo[0].flags&QOS_DEF_QUEUE)!=0)
			divisor = qosInfo[0].ceil;
		else
			divisor = qosInfo[0].bandwidth;

		for(queueNum=0; queueNum<RTL8651_OUTPUTQUEUE_SIZE; queueNum++)
		{
			if ((qosInfo[queueNum].flags&QOS_VALID_MASK)==0)
				break;
			
			/*	Currently, we set all queue as WFQ		*/
			if ((qosInfo[queueNum].flags&QOS_TYPE_MASK)==QOS_TYPE_WFQ)
			{
				#if 0 /* closed by Kevin , bandwidth would not change even though it is default queue */
				if ((qosInfo[queueNum].flags&QOS_DEF_QUEUE)!=0)
				{
					qosInfo[queueNum].bandwidth=qosInfo[queueNum].ceil;
				}
				#endif
				divisor = my_gcd(qosInfo[queueNum].bandwidth, divisor);
			}
		}

		/*	process WFQ type ratio	*/
		{
			if (divisor)
			{
				int	maxBandwidth;
				int	queueNumBackup;
				maxBandwidth = 0;
				queueNumBackup = queueNum;

				while(queueNum>0)
				{
					if ((qosInfo[queueNum-1].flags&QOS_TYPE_MASK) 
						==QOS_TYPE_WFQ)
					{
						qosInfo[queueNum-1].bandwidth = (qosInfo[queueNum-1].bandwidth/divisor);
						if (maxBandwidth<qosInfo[queueNum-1].bandwidth)
							maxBandwidth = qosInfo[queueNum-1].bandwidth;
					}
					queueNum--;
				}

				if (maxBandwidth>EGRESS_WFQ_MAX_RATIO)
				{
					queueNum = queueNumBackup;
					divisor = (maxBandwidth/EGRESS_WFQ_MAX_RATIO)
						+ ((maxBandwidth%EGRESS_WFQ_MAX_RATIO)>(EGRESS_WFQ_MAX_RATIO>>1)?1:0);
					while(queueNum>0)
					{
						if ((qosInfo[queueNum-1].flags&QOS_TYPE_MASK) 
							==QOS_TYPE_WFQ)
						{
							qosInfo[queueNum-1].bandwidth = 
								(qosInfo[queueNum-1].bandwidth/divisor);
							
							if (qosInfo[queueNum-1].bandwidth==0)
								qosInfo[queueNum-1].bandwidth = 1;
							else if (qosInfo[queueNum-1].bandwidth>EGRESS_WFQ_MAX_RATIO)
								qosInfo[queueNum-1].bandwidth = EGRESS_WFQ_MAX_RATIO;
						}
						queueNum--;
					}
				}
			}
		}
	}

	#if 0 /*   closed by Kevin , do not need queueOrder */
	nStart = nEnd = mStart = mEnd = 0;	/*	reserver 0 for default queue	*/
	queueOrder[0] = 0;
	outputQueue = qosInfo;
	
	for(entry=0; entry<RTL8651_OUTPUTQUEUE_SIZE; entry++, outputQueue++)
	{
		if ((outputQueue->flags&QOS_VALID_MASK)==0)
			break;
		
		//	rtlglue_printf("index %d, queueType %d.\n", entry, outputQueue->queueType); 
		if ((outputQueue->flags&QOS_TYPE_MASK)==QOS_TYPE_WFQ) 
		{
			/*	Do not exceed the max value: 1 ~ 128	*/
			if (outputQueue->bandwidth>((WEIGHT0_MASK>>WEIGHT0_OFFSET)+1))
				outputQueue->bandwidth = (WEIGHT0_MASK>>WEIGHT0_OFFSET)+1;

			/*	this is a NQueue entry	*/
			{
				/*	process m Queue */
				if (mEnd>mStart)
				{
					i = mEnd-1;
					while(i>=mStart)
					{
						queueOrder[i+1] = queueOrder[i];
						i--;
					}
				}
				mEnd++;
				mStart++;	

				/*	process n Queue */
				i = nEnd;
				{
					while(i>nStart)
					{
						if(rtl865x_compFunc(outputQueue, &outputQueue[queueOrder[i-1]])>0)
						{
							queueOrder[i] = queueOrder[i-1];
							i--;
							continue;
						}
						break;
					}
				}
				nEnd++;
				queueOrder[i] = entry;
			}			
		} else if ((outputQueue->flags&QOS_TYPE_MASK)==QOS_TYPE_STR)
		{
			i = mEnd;
			{
				while(i>mStart)
				{
					if (rtl865x_compFunc(outputQueue, &outputQueue[queueOrder[i-1]])>0)
					{
						queueOrder[i] = queueOrder[i-1];
						i--;
						continue;
					}
					break;
				}
			}
			mEnd++;
			queueOrder[i] = entry;
		}
	}
	#endif

	#if 0
	#if 0
	queueNumber[idx] = mEnd;	
	#else
	queueNumber[idx] = 0;
	for(i=0; i<RTL8651_OUTPUTQUEUE_SIZE; i++)	
	{
		if ((qosInfo[i].flags&QOS_VALID_MASK)==0)
			continue;
		else
			queueNumber[idx] ++;
	}
	#endif

	queueNum = 1;
	for(i=0;i<NETIF_NUMBER;i++)
	{
		if (queueNum<queueNumber[i])
			queueNum = queueNumber[i];
	}
	#endif




	
	#if 0
	
	//_rtl865x_SetQueueNum(qosInfo->ifname,queueNum);






	//memset((void*)tmp_qosInfo, 0, RTL8651_OUTPUTQUEUE_SIZE*sizeof(rtl865x_qos_t));


	/*	Record the priority <-> handle mapping relationship	*/


	memset(priority2Handle, 0, TOTAL_VLAN_PRIORITY_NUM*sizeof(int));
	cnt=0;
	for(i=0;i<RTL8651_OUTPUTQUEUE_SIZE;i++)
	{
		if(queueMatrix[queueNum-1][i]<0)
			continue;

		qosInfo[cnt].queueId = i;
		priority2Handle[queueMatrix[queueNum-1][i]] = qosInfo[cnt].handle;
		cnt++;
	}
	_rtl865x_qosPriorityHandleMappingSet(qosInfo->ifname,priority2Handle);

	
	#if 0
	#if 0
	for(i=0;i<queueNumber[idx];i++)
	{
		cnt = -1;
		for(j=0;j<RTL8651_OUTPUTQUEUE_SIZE;j++)
		{
			if (queueMatrix[queueNum-1][j]>=0)
				cnt++;
			else
				continue;

			if (cnt==i)
			{
				memcpy(&tmp_qosInfo[j], &qosInfo[queueOrder[i]], sizeof(rtl865x_qos_t));
				priority2HandleMapping[idx][queueMatrix[queueNum-1][j]] = tmp_qosInfo[j].handle;
				tmp_qosInfo[j].queueId = j;
			}
		}
	}

	#else
	cnt=0;
	for(i=0;i<RTL8651_OUTPUTQUEUE_SIZE;i++)
	{
		if(queueMatrix[queueNum-1][i]<0)
			continue;

		qosInfo[cnt].queueId = i;
		priority2HandleMapping[idx][queueMatrix[queueNum-1][i]] = qosInfo[cnt].handle;
		cnt++;
	}
	#endif		
	#endif	

	//memcpy(qosInfo, tmp_qosInfo, RTL8651_OUTPUTQUEUE_SIZE*sizeof(rtl865x_qos_t));




	/*	Set default priority	*/
	for(i=0;i<RTL8651_OUTPUTQUEUE_SIZE;i++)
	{
		if (!(qosInfo[i].flags&QOS_DEF_QUEUE))
			continue;
		
		//for(j=0; j < TOTAL_VLAN_PRIORITY_NUM; j++)
		//{
		//	if (priority2HandleMapping[idx][j] == qosInfo[i].handle)
		//		break;
		//}
	

		/*	If we do not find the default queue priority
		  *	just keep the default priority 0
		  */
		//if (j==TOTAL_VLAN_PRIORITY_NUM)
		//	j = 0;

		/*	Set default queue priority	*/
		_rtl865x_qosACLPriorityMappingSetDefault(qosInfo->ifname,_rtl865x_qosPriorityByHandleMappingGet(qosInfo->ifname,qosInfo[i].handle));	
		//defPriority[idx] = j;
	}
	return SUCCESS;
	#endif
}
#endif

int32 rtl865x_qosProcessQueue_htb(uint8 *netIfName, rtl865x_qos_t *qosInfo)
{

	rtl865x_netif_local_t	*netIf;
	uint32	queueNum=0;
	int32	asicBandwidth;
	int i;	
	uint32	Qid2Handle[RTL8651_OUTPUTQUEUE_SIZE];
	uint32	default_handler;
	uint32	port,queue;


	if (qosInfo==NULL)
		return FAILED;

	/* 1. Convert name to netif */
	netIf = _rtl865x_getQosNetif(netIfName);	
	if(netIf == NULL)
		return FAILED;


	/* 2.  Re-adjust qosinfo's para , get total number of queue setting */
	_rtl865x_qosArrangeQueue(qosInfo);		
	for(i=0; i<RTL8651_OUTPUTQUEUE_SIZE; i++)	
	{
		if ((qosInfo[i].flags&QOS_VALID_MASK)==0)
			continue;
		else
			queueNum++;
	}	
	

	/* 3. check queue number */
	if(rtl865x_netif_is_wan(netIf->name)) /* output device is wan device */
	{
		if( queueNum<1 || queueNum>UPSTREAM_Quene_NUM)
		{
			printk("(%s)Warning!! 0412 switch only support 1~%d outputqueues in upstream (Your input is %d) \n"
				,__func__,UPSTREAM_Quene_NUM,queueNum);
			return FAILED;	
		}
	}
	else
	{
		if( queueNum<1 || queueNum>DOWNSTREAM_Quene_NUM)
		{
			printk("(%s)Warning!! 0412 switch only support 1~%d outputqueues in downstream (Your input is %d) \n"
				,__func__,DOWNSTREAM_Quene_NUM,queueNum);
			return FAILED;	
		}
	}


	/* 4. Set qosInfo's Qid*/
	for(i=0;i<queueNum;i++)
	{
		qosInfo[i].queueId = i;			
	}


	/* 5.  set QueueID to hander mapping */
	memset(Qid2Handle, 0, RTL8651_OUTPUTQUEUE_SIZE*sizeof(uint32));
	for(i=0;i<queueNum;i++)
	{
		Qid2Handle[i] = qosInfo[i].handle;
	}
	if(_rtl865x_qosQIDHandleMappingSet(netIf->name,Qid2Handle)!=SUCCESS)
		return FAILED;


	/*  6. set default acl priority */
	default_handler = 0;
	for(i=0;i<queueNum;i++)
	{
		if (qosInfo[i].flags&QOS_DEF_QUEUE)
		{
			default_handler = qosInfo[i].handle;
			break;
		}
	}
	if(default_handler == 0)
		return FAILED;
		
	if(_rtl865x_qosACLPriorityMappingSetDefault(netIf->name,default_handler,0,0)!=SUCCESS)
		return FAILED;


	/*  7. arrange all sw qos rules  */
	if(_rtl865x_qosArrangeRuleByNetif(netIf->name)!=SUCCESS)
		return FAILED;


	rtl865xC_lockSWCore(); /* Let's start to setup 0412 switch core ....*/
	
	/* 6. set queue number , queue's bandwidth (enable reamrk function if wan dev)*/
	for(port=PHY0;port<=CPU;port++)
	{
		if(rtl865x_netif_is_wan(netIf->name))
		{
			if(((1<<port)&RTL_WANPORT_MASK)==0)
				continue;
		}
		else
		{
			if(((1<<port)&RTL_LANPORT_MASK)==0)
				continue;			
		}


		if(rtl865x_netif_is_wan(netIf->name))
		{
			rtl8651_setAsicOutputQueueNumber(port, UPSTREAM_Quene_NUM);
			rtl8651_EnablePortRemark(port);
		}
		else
			rtl8651_setAsicOutputQueueNumber(port, DOWNSTREAM_Quene_NUM);

		
		for (queue=0;queue<queueNum;queue++)
		{
			/*	Egress bandwidth granularity was 64Kbps	*/
			asicBandwidth = ((qosInfo[queue].ceil)>>(EGRESS_BANDWIDTH_GRANULARITY_BITLEN)) - 1;
			if ((qosInfo[queue].ceil)&(1<<(EGRESS_BANDWIDTH_GRANULARITY_BITLEN-1)))
				asicBandwidth += 1;

			if ((qosInfo[queue].flags&QOS_TYPE_MASK)==QOS_TYPE_STR)
			{
				rtl8651_setAsicQueueRate(port, qosInfo[queue].queueId, 
					PPR_MASK>>PPR_OFFSET, 
					L1_MASK>>L1_OFFSET, 
					asicBandwidth);
				rtl8651_setAsicQueueWeight(port, qosInfo[queue].queueId, STR_PRIO, 0);
			}
			else
			{
				rtl8651_setAsicQueueRate(port, qosInfo[queue].queueId, 
					0>>PPR_OFFSET, 
					L1_MASK>>L1_OFFSET, 
					asicBandwidth);				
				rtl8651_setAsicQueueWeight(port, qosInfo[queue].queueId, WFQ_PRIO, qosInfo[queue].bandwidth-1);
			}
		}		
					
	}

	/* 7. set acl priority mapping into hw */
	if(_rtl865x_qosACLPriorityMappingTakeEffect(netIf->name)!=SUCCESS)
	{	
		rtl865xC_waitForOutputQueueEmpty();
		rtl8651_resetAsicOutputQueue();
		rtl865xC_unLockSWCore();
		return FAILED;
	}

	rtl865xC_waitForOutputQueueEmpty();
	rtl8651_resetAsicOutputQueue();
	rtl865xC_unLockSWCore();		

	return SUCCESS;
}

int32 rtl865x_qosProcessQueue_prio(uint8 *netIfName,uint32 root_handle,uint32 bands_num)
{
	rtl865x_netif_local_t	*netIf;		
	uint32 	Qid2Handle[RTL8651_OUTPUTQUEUE_SIZE];
	int i;
	uint32	port;



	/* 1. Convert name to netif */
	netIf = _rtl865x_getQosNetif(netIfName);	
	if(netIf == NULL)
		return FAILED;

	

	/* 2. check queue number */
	if(rtl865x_netif_is_wan(netIf->name)) /* output device is wan device */
	{
		if( bands_num<1 || bands_num>UPSTREAM_Quene_NUM)
		{
			printk("(%s)Warning!! 0412 switch only support 1~%d outputqueues in upstream (Your input is %d) \n"
				,__func__,UPSTREAM_Quene_NUM,bands_num);
			return FAILED;	
		}
	}
	else
	{
		if( bands_num<1 || bands_num>DOWNSTREAM_Quene_NUM)
		{
			printk("(%s)Warning!! 0412 switch only support 1~%d outputqueues in downstream (Your input is %d) \n"
				,__func__,DOWNSTREAM_Quene_NUM,bands_num);
			return FAILED;	
		}
	}
	

	/* 3.  set QueueID to hander mapping */
	memset(Qid2Handle, 0, RTL8651_OUTPUTQUEUE_SIZE*sizeof(uint32));
	if(rtl865x_netif_is_wan(netIf->name))
	{
		for(i=0;i<bands_num;i++)		
			Qid2Handle[UPSTREAM_Quene_NUM-1-i] = root_handle | (i+1);
	}
	else
	{
		for(i=0;i<bands_num;i++)		
			Qid2Handle[DOWNSTREAM_Quene_NUM-1-i] = root_handle | (i+1);
	}	
	if(_rtl865x_qosQIDHandleMappingSet(netIf->name,Qid2Handle)!=SUCCESS)
		return FAILED;


	/*  4. set default acl priority 
	          default : enqueue pkts into the lowest priotity queue and no any reamrk	*/
	if(_rtl865x_qosACLPriorityMappingSetDefault(netIf->name,(root_handle|bands_num),0,0)!=SUCCESS)
		return FAILED;


	
	/*  5. arrange all sw qos rules  */
	if(_rtl865x_qosArrangeRuleByNetif(netIf->name)!=SUCCESS)
		return FAILED;


	rtl865xC_lockSWCore(); /* Let's start to setup 0412 switch core ....*/
	/* 6. set queue number (enable reamrk function if wan dev)*/
	for(port=PHY0;port<=CPU;port++)
	{
		if(rtl865x_netif_is_wan(netIf->name))
		{
			if(((1<<port)&RTL_WANPORT_MASK)==0)
				continue;
			rtl8651_EnablePortRemark(port);
			rtl8651_setAsicOutputQueueNumber(port, UPSTREAM_Quene_NUM);
		}
		else
		{
			if(((1<<port)&RTL_LANPORT_MASK)==0)
				continue;

			rtl8651_setAsicOutputQueueNumber(port, DOWNSTREAM_Quene_NUM);
		}			
	}


	/* 7. set acl priority mapping into hw */
	if(_rtl865x_qosACLPriorityMappingTakeEffect(netIf->name)!=SUCCESS)
	{
		rtl865xC_waitForOutputQueueEmpty();
		rtl8651_resetAsicOutputQueue();
		rtl865xC_unLockSWCore();
		return FAILED;
	}

	rtl865xC_waitForOutputQueueEmpty();
	rtl8651_resetAsicOutputQueue();
	rtl865xC_unLockSWCore();
	
	return SUCCESS;
	
}


rtl865x_netif_local_t* _rtl865x_getQosNetif(char* ifname)
{	
	if(strncmp(ifname,"imq",3))	
		return _rtl865x_getNetifByName(ifname);
	else		
		return _rtl865x_getDefaultWanNetif();		
}


/* Sync all sw qos rules with hw */
int32 rtl865x_qosArrangeRuleByNetif(void)
{
	/* This a optimization : _rtl865x_qosArrangeRuleByNetif will pass all wan dev , so we only need to call the one of wan dev */
	int pass_wan = 0;

	ForEachMasterNetif_Declaration

	#if 0
	ForEachMasterNetif_Start
		rtl865x_flush_allAcl_fromChain(netif->name, RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS);
		rtl865x_flush_allAcl_fromChain(netif->name, RTL865X_ACL_QOS_USED1, RTL865X_ACL_INGRESS);
	ForEachMasterNetif_End
	#endif

	ForEachMasterNetif_Start

		if(rtl865x_netif_is_wan(netif->name))
		{
			if(pass_wan==0)
			{
				if(_rtl865x_qosArrangeRuleByNetif(netif->name)!=SUCCESS)
					return FAILED;
				pass_wan = 1;
			}
			else
				continue;
		}
		else
		{
			if(_rtl865x_qosArrangeRuleByNetif(netif->name)!=SUCCESS)
				return FAILED;
		}
	ForEachMasterNetif_End
	#if 0
	int		i;
	
	for(i=0;i<NETIF_NUMBER;i++)
	{
		if (netIfNameArray[i][0]!=0)
		{
			rtl865x_flush_allAcl_fromChain(netIfNameArray[i], RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS);
			rtl865x_flush_allAcl_fromChain(netIfNameArray[i], RTL865X_ACL_QOS_USED1, RTL865X_ACL_INGRESS);
		}
	}

	for(i=0;i<NETIF_NUMBER;i++)
	{
		if (netIfNameArray[i][0]!=0)
		{
			_rtl865x_qosArrangeRuleByNetif(netIfNameArray[i]);
		}
	}	
	#endif

	return SUCCESS;
}


/* Sync the sw qos rules that output netif is netIfName with 0412 hw acl and acl priotiy mapping */
static int32 _rtl865x_qosArrangeRuleByNetif(uint8 *netIfName)
{
	rtl865x_netif_local_t* output_netif;
	
	output_netif = _rtl865x_getQosNetif(netIfName);	
	if(output_netif == NULL)
		return FAILED;

	/* before arranging all sw qos rule, it has to ...
		(i)  clear all acl_priority mapping to re-calculate all ruls (out are netIfName)'s acl prioity mapping
		(ii) clear all 802.1p priority to acl prioity mapping
	*/
	_rtl865x_qosACLPriorityMappingClear(output_netif->name);
	rtl8651_flushAsicDot1qAbsolutelyPriority();

	/*  If output netif iw WAN , because all WAN dev use the same acl priority mapping
	     Therefore it is essential to sync all sw qos rules together that output netif is WAN dev */
	if(rtl865x_netif_is_wan(output_netif->name))
	{
		ForEachMasterWanNetif_Declaration	
		ForEachMasterWanNetif_Start	
		if(__inner_qosArrangeRuleByNetif(netif->name)!=SUCCESS)
			return FAILED;
		ForEachMasterWanNetif_End
	}
	else
	{
		if(__inner_qosArrangeRuleByNetif(output_netif->name)!=SUCCESS)
			return FAILED;
	}
					
	rtl865xC_lockSWCore(); /* Let's start to setup 0412 switch core ....*/
	/*  let new acl_priority mapping take effect */	
	if(_rtl865x_qosACLPriorityMappingTakeEffect(output_netif->name)!=SUCCESS)
	{
		rtl865xC_waitForOutputQueueEmpty();
		rtl8651_resetAsicOutputQueue();
		rtl865xC_unLockSWCore();
		return FAILED;
	}	
	rtl865xC_waitForOutputQueueEmpty();
	rtl8651_resetAsicOutputQueue();
	rtl865xC_unLockSWCore();

	return SUCCESS;
}



/*  Do not call this func explicitly ,use _rtl865x_qosArrangeRuleByNetif() instead 
      This function will ...
       (i)  review all soft qos rules that output netif is netIfName and refresh them in hw acl  (add new qos rule / the existed rule change its original priority)
       (ii) set default priroty rule about traffic whose output netif is netIfName

       It will NOT 
       (i)  reset/clear the original acl priority mapping , it is caller's responsibility
       (ii) write new acl priority mapping into 0412 hw.
*/
static int32 __inner_qosArrangeRuleByNetif(uint8 *netIfName)
{
	rtl865x_qos_rule_t	*qosRule;
	int32			priority;
	rtl865x_netif_local_t* output_netif;
	int32	idx,tmp_idx;
//	rtl865x_AclRule_t	aclRule;

	/* 1. Convert name to netif */	
	output_netif = _rtl865x_getQosNetif(netIfName);	
	if(output_netif == NULL)
		return FAILED;

	idx = _rtl865x_getNetifIdxByNameExt(output_netif->name);
	if(idx<0)
		return FAILED;	


	/* pass all sw qos rules into netif's acl rules */
	for(qosRule = rtl865x_qosRuleHead; qosRule; qosRule=qosRule->next)
	{		

		/*  2. skip the rule without matching traffic direction  */
		if(rtl865x_netif_is_wan(output_netif->name)) /* output device is wan device */
		{
			/* only pass upstream rule :  
				(1) NULL -> NULL
				(2) NULL -> WAN
				(3) LAN   -> NULL
				(4) LAN   -> WAN
			*/
			if(qosRule->inIfname[0]!='\0' && rtl865x_netif_is_wan(qosRule->inIfname))
				continue;	
			
			if(qosRule->outIfname[0]!='\0' && !rtl865x_netif_is_wan(qosRule->outIfname))
				continue;
			
		}
		else /* output device is lan device */
		{		 
			/* only pass downstream rule :  
				(1) NULL -> NULL
				(2) NULL -> NAN
				(3) WAN   -> NULL
				(4) WAN   -> LAN
			*/
			if(qosRule->inIfname[0]!='\0' && !rtl865x_netif_is_wan(qosRule->inIfname))
				continue;
			
			if(qosRule->outIfname[0]!='\0' && rtl865x_netif_is_wan(qosRule->outIfname))
				continue;
		}		


		/*  3. skip the rules that output_netif not matching */
		if(qosRule->outIfname[0]!='\0')
		{
			/*	assigned egress netif	*/
			tmp_idx = _rtl865x_getNetifIdxByNameExt(qosRule->outIfname);
			if (tmp_idx!=idx)
				continue;
		}
		
		/* 4. delete the original acl rules (skip 802.1p , this type is a dummy acl type )*/
		if(qosRule->rule->ruleType_!=RTL865X_ACL_802D1P)
		{
			if (qosRule->inIfname[0]!='\0')
			{
				rtl865x_del_acl(qosRule->rule, qosRule->inIfname, RTL865X_ACL_QOS_USED0);
			}
			else
			{
				ForEachMasterNetif_Declaration
				ForEachMasterNetif_Start

				if(rtl865x_netif_is_wan(output_netif->name)) /* upstream*/		
				{
					if(rtl865x_netif_is_wan(netif->name))
						continue;			
				}
				else /* downstream*/
				{
					if(!rtl865x_netif_is_wan(netif->name))
						continue;				
				}
				
				rtl865x_del_acl(qosRule->rule, netif->name, RTL865X_ACL_QOS_USED0);			
				ForEachMasterNetif_End
			}
		}

		
		/* 5. skip the rules that it cannot use CLASSFIY or MARK to find Qdisc handler */
		if (qosRule->handle==0)
			continue;


		/* 6. skip the rules that it cannot use Qdisc handler acl priority mapping
			if ok, set acl rule's priority
		*/
		if(rtl865x_netif_is_wan(output_netif->name)) /* upstream*/	
		{
			if ((priority=_rtl865x_qosACLPriorityMappingGet(output_netif->name,qosRule->handle,qosRule->remark_8021p,qosRule->remark_dscp))==-1)
				continue;
		}
		else /* downstream , cannot remark */	
		{
			if ((priority=_rtl865x_qosACLPriorityMappingGet(output_netif->name, qosRule->handle,0,0))==-1)
				continue;			
		}		
		qosRule->rule->priority_ = priority;
		


		/* 7. Finally, start to add acl rule*/			
		if(qosRule->rule->ruleType_!=RTL865X_ACL_802D1P)
		{
			if (qosRule->inIfname[0]!='\0')
			{
				/*	assigned ingress netif	*/		
				rtl865x_add_acl(qosRule->rule, qosRule->inIfname, RTL865X_ACL_QOS_USED0);
				#if defined (CONFIG_RTL_LOCAL_PUBLIC)
				#if defined(CONFIG_RTL_PUBLIC_SSID)
				if (memcmp(RTL_GW_WAN_DEVICE_NAME, qosRule->inIfname, 4)==0
					&&qosRule->rule->direction_==RTL_ACL_INGRESS)
				#else
				if (memcmp(RTL_DRV_WAN0_NETIF_NAME, qosRule->inIfname, 4)==0
					&&qosRule->rule->direction_==RTL_ACL_INGRESS)
				#endif
				{
					rtl_checkLocalPublicNetifIngressRule(qosRule->rule);
				}
				#endif
			}
			else
			{				
				/*	do not assigned ingress netif	*/				
				ForEachMasterNetif_Declaration
			
				ForEachMasterNetif_Start
				if(rtl865x_netif_is_wan(output_netif->name)) /* upstream*/	
				{
					if(rtl865x_netif_is_wan(netif->name))
						continue;	
				}
				else /* downstream*/
				{
					if(!rtl865x_netif_is_wan(netif->name))
						continue;
				}		
				
				rtl865x_add_acl(qosRule->rule, netif->name, RTL865X_ACL_QOS_USED0);	
				ForEachMasterNetif_End
				#if 0
				for(i=0;i<NETIF_NUMBER;i++)
				{
					if (netIfNameArray[i][0]!=0)
					{
						tmp_idx = _rtl865x_getNetifIdxByNameExt(netIfNameArray[i]);
						if (tmp_idx>=0&&tmp_idx<NETIF_NUMBER&&tmp_idx!=idx)
						{
							rtl865x_del_acl(qosRule->rule, netIfNameArray[i], RTL865X_ACL_QOS_USED0);
							rtl865x_add_acl(qosRule->rule, netIfNameArray[i], RTL865X_ACL_QOS_USED0);
						}
					}
				}
				#endif
				#if defined (CONFIG_RTL_LOCAL_PUBLIC)
				if (qosRule->rule->direction_==RTL_ACL_INGRESS)
				{
					rtl_checkLocalPublicNetifIngressRule(qosRule->rule);
				}
				#endif
			}
		}
		else /* ruleType_==RTL865X_ACL_802D1P */
		{
			rtl8651_setAsicDot1qAbsolutelyPriority(qosRule->rule->vlanTagPri_, qosRule->rule->priority_);
		}
		
	}



	#if 0 /* move adding default priority from acl to per port setting  */
	/* 8.  Add default priority  rule */
	memset(&aclRule, 0, sizeof(rtl865x_AclRule_t));
	aclRule.actionType_ = RTL865X_ACL_PRIORITY;
	aclRule.pktOpApp_ = RTL865X_ACL_ALL_LAYER;	

	{
	ForEachMasterNetif_Declaration
	ForEachMasterNetif_Start

		if(rtl865x_netif_is_wan(output_netif->name)) /* upstream*/	
		{
			if(rtl865x_netif_is_wan(netif->name))
				continue;	
		}
		else /* downstream*/
		{
			if(!rtl865x_netif_is_wan(netif->name))
				continue;
		}		

		/* delete the old default priority rule */
		rtl865x_del_acl(&aclRule, netif->name, RTL865X_ACL_QOS_USED1);

		if(defPriority[idx]>=0)
		{
			aclRule.priority_ = defPriority[idx];
			rtl865x_add_acl(&aclRule, netif->name, RTL865X_ACL_QOS_USED1);
		}

		
	ForEachMasterNetif_End
	}		
	#endif

	rtl865x_raiseEvent(EVENT_CHANGE_QOSRULE, NULL);

	return SUCCESS;
}

int32 rtl865x_qosAddRule(rtl865x_qos_rule_t *rule)
{
	
	rtl865x_qos_rule_t	*qosRule;	



	if(strcmp(rule->inIfname,"") && !rtl865x_netifExist(rule->inIfname))
		return RTL_EINVALIDINPUT;

	if(strcmp(rule->outIfname,"") && !rtl865x_netifExist(rule->outIfname))
		return RTL_EINVALIDINPUT;


	/* if it ok to add into qos rule, change acl_rule's action type to RTL865X_ACL_PRIORITY */
	rule->rule->pktOpApp_ = RTL865X_ACL_ALL_LAYER;
	rule->rule->actionType_ = RTL865X_ACL_PRIORITY;
	rule->rule->aclIdx = 0;	
	

	/* find whether there is the same qos rule */
	for(qosRule = rtl865x_qosRuleHead; qosRule; qosRule=qosRule->next)
	{
		if(_rtl865x_sameQosRule(qosRule,rule)==TRUE)
		{
			qosRule->classify = (rule->classify==0)?(qosRule->classify):(rule->classify);
			qosRule->mark = (rule->mark==0)?(qosRule->mark):(rule->mark);
			qosRule->remark_8021p = (rule->remark_8021p==0)?(qosRule->remark_8021p):(rule->remark_8021p);
			qosRule->remark_dscp = (rule->remark_dscp==0)?(qosRule->remark_dscp):(rule->remark_dscp);
			qosRule->handle = (rule->handle==0)?(qosRule->handle):(rule->handle);
			break;
		}
	}

	/* it is a new qos rule */	
	if(qosRule==NULL)
	{
		rtl865x_AclRule_t *qosAclRule;

		qosRule = rtl_malloc(sizeof(rtl865x_qos_rule_t));
		if (qosRule==NULL)
			return RTL_ENOFREEBUFFER;
		
		qosAclRule = kmalloc(sizeof(rtl865x_AclRule_t), GFP_ATOMIC);
		if (qosAclRule==NULL)
		{
			rtl_free(qosRule);
			return RTL_ENOFREEBUFFER;
		}
		memcpy(qosRule, rule, sizeof(rtl865x_qos_rule_t));
		memcpy(qosAclRule, rule->rule, sizeof(rtl865x_AclRule_t));			
		qosRule->rule = qosAclRule;

		if (rtl865x_qosRuleHead==NULL)
		{
			qosRule->next = NULL;
			rtl865x_qosRuleHead = qosRule;						
		}
		else
		{
			//the first matching priority rule has the highest prioirty in 0412acl
			//     , but the policy is opposite in iptables CLASSIFY/MARK rule (go through all rules and take the last matching rule)
			// Therefore, we add qos rule from iptables rule in the opposite order		
			qosRule->next = rtl865x_qosRuleHead;
			rtl865x_qosRuleHead = qosRule;
		}

	}



	

	if(strcmp(rule->outIfname,""))
	{
		if(_rtl865x_qosArrangeRuleByNetif(rule->outIfname)!=SUCCESS)
			return FAILED;
	}
	else
	{
		if(rtl865x_qosArrangeRuleByNetif()!=SUCCESS)
			return FAILED;
	}
		


	#if 0
	{
	ForEachMasterNetif_Declaration
	ForEachMasterNetif_Start
	if(_rtl865x_qosArrangeRuleByNetif(netif->name)!=SUCCESS)	
		return FAILED;
	
	ForEachMasterNetif_End
	}
	
	for(i=0;i<NETIF_NUMBER;i++)
	{
		if (netIfNameArray[i][0]!=0)
		{
			_rtl865x_qosArrangeRuleByNetif(netIfNameArray[i]);
		}
	}
	#endif

	
	return SUCCESS;
}

static int8 _rtl865x_sameQosRule(rtl865x_qos_rule_t *rule1, rtl865x_qos_rule_t *rule2)
{
	if(_rtl865x_sameAclRule(rule1->rule,rule2->rule)==FALSE)		
		return FALSE;	
	else
	{
		if(strcmp(rule1->inIfname,rule2->inIfname) || strcmp(rule1->outIfname,rule2->outIfname))		
			return FALSE;		
		else
			return TRUE;
	}	
}

int32 rtl865x_qosCheckNaptPriority(rtl865x_AclRule_t *qosRule)
{
	rtl865x_AclRule_t		*rule_p;

	rule_p = rtl865x_matched_layer4_aclChain(_rtl865x_getNetifNameByidx(qosRule->netifIdx_), RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS, qosRule);
	//rule_p = rtl865x_matched_layer4_aclChain(netIfNameArray[qosRule->netifIdx_], RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS, qosRule);

	if (!rule_p)
		rule_p = rtl865x_matched_layer2_aclChain(_rtl865x_getNetifNameByidx(qosRule->netifIdx_), RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS, qosRule);
		//rule_p = rtl865x_matched_layer2_aclChain(netIfNameArray[qosRule->netifIdx_], RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS, qosRule);

	if (rule_p)
	{
		qosRule->priority_ = rule_p->priority_;
		qosRule->aclIdx = rule_p->aclIdx;
		qosRule->upDown_=rule_p->upDown_;
		return SUCCESS;
	}
	else
	{
		qosRule->priority_ = defPriority[qosRule->netifIdx_];
		return FAILED;
	}
}

int32 rtl865x_qosFlushRule(void)
{
	rtl865x_qos_rule_t	*qosRule;
//	int	i;

	/*  clear all sw qos rules */
	while(rtl865x_qosRuleHead)
	{
		qosRule = rtl865x_qosRuleHead->next;
		rtl_free(rtl865x_qosRuleHead->rule);
		rtl_free(rtl865x_qosRuleHead);
		rtl865x_qosRuleHead = qosRule;
	}

	/* do not clear default qos rule , clear default qos rule only in rtl865x_closeQos() when closing relationship between kernel Qdiscs and 0412 outputqueue */
	{
		ForEachMasterNetif_Declaration
		ForEachMasterNetif_Start		
			rtl865x_flush_allAcl_fromChain(netif->name, RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS);
			//rtl865x_flush_allAcl_fromChain(netif->name, RTL865X_ACL_QOS_USED1, RTL865X_ACL_INGRESS);	
			_rtl865x_qosACLPriorityMappingClear(netif->name);	
		ForEachMasterNetif_End
	}	
	rtl8651_flushAsicDot1qAbsolutelyPriority();

	
	#if 0
	for(i=0;i<NETIF_NUMBER;i++)
	{
		if (netIfNameArray[i][0]!=0)
		{
			rtl865x_flush_allAcl_fromChain(netIfNameArray[i], RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS);
			rtl865x_flush_allAcl_fromChain(netIfNameArray[i], RTL865X_ACL_QOS_USED1, RTL865X_ACL_INGRESS);
			_rtl865x_qosArrangeRuleByNetif(netIfNameArray[i]);
		}
	}
	#endif

	rtl865x_raiseEvent(EVENT_FLUSH_QOSRULE, NULL);
	
	return SUCCESS;
}

int32 rtl865x_closeQos(uint8 *netIfName)
{
	rtl865x_netif_local_t	*netIf;
	uint32	port,i;


	
	/* 1. Convert name to netif */
	netIf = _rtl865x_getQosNetif(netIfName);
	if(netIf == NULL)
		return FAILED;

	/* 2.  clear Qid to hander mapping and acl priotiy mapping */
	_rtl865x_qosQIDHandleMappingClear(netIf->name);	

	/*  3. clear default acl priority */
	_rtl865x_qosACLPriorityMappingClearDefault(netIf->name);
	rtl8651_flushAsicDot1qAbsolutelyPriority();

	/*  4. arrange all sw qos rules	
	          ( After clear all QIDHandleMapping , sw qos rules cannot get Qid by hander when arrange qos rules
	             ,therefore it cannot get acl priority) */	
	if(_rtl865x_qosArrangeRuleByNetif(netIf->name)!=SUCCESS)
		return FAILED;


	
	rtl865xC_lockSWCore(); /* Let's start to setup 0412 switch core ....*/
	
	/* 5. set queue number back to 1 and reset all queues (disable reamrk function if wan dev) */		
	for(port=PHY0;port<=CPU;port++)
	{		
		if(rtl865x_netif_is_wan(netIf->name))
		{
			if(((1<<port)&RTL_WANPORT_MASK)==0)
				continue;
		}
		else
		{
			if(((1<<port)&RTL_LANPORT_MASK)==0)
				continue;
		}
		
		rtl8651_setAsicOutputQueueNumber(port, 1);
		for(i=0;i<RTL8651_OUTPUTQUEUE_SIZE;i++)
		{
			rtl8651_setAsicQueueWeight(port, i, STR_PRIO, 0);
			rtl8651_setAsicQueueRate(port,i, PPR_MASK>>PPR_OFFSET, L1_MASK>>L1_OFFSET,APR_MASK>>APR_OFFSET);
		}
		
		if(rtl865x_netif_is_wan(netIf->name))
			rtl8651_DisablePortRemark(port);
	}


	/* 7. set acl priority mapping into hw */	
	if(_rtl865x_qosACLPriorityMappingTakeEffect(netIf->name)!=SUCCESS)
	{
		rtl865xC_waitForOutputQueueEmpty();
		rtl8651_resetAsicOutputQueue();
		rtl865xC_unLockSWCore();
		return FAILED;
	}	
	
	rtl865xC_waitForOutputQueueEmpty();
	rtl8651_resetAsicOutputQueue();
	rtl865xC_unLockSWCore();


	return SUCCESS;
}

int __init rtl865x_initOutputQueue(void)
{
	int	i,j;


	rtl865xC_lockSWCore();
	for(i=0;i<RTL8651_OUTPUTQUEUE_SIZE;i++)
	{
		for(j=0;j<TOTAL_VLAN_PRIORITY_NUM;j++)
			rtl8651_setAsicPriorityToQIDMappingTable(i+1, j, priorityMatrix[i][j]);
	}
#if 0
	for(i=CPU;i<=MULTEXT;i++)
	{
		/* mapping all priority to cpu port output queue 0	*/
		for(j=0;j<TOTAL_VLAN_PRIORITY_NUM;j++)
			if(j < 4)
				rtl8651_setAsicCPUPriorityToQIDMappingTable(i, j, 0);
			else
				rtl8651_setAsicCPUPriorityToQIDMappingTable(i,j,5);
	}
#endif
	for(j=0;j<8;j++)
		rtl8651_setAsicCPUPriorityToQIDMappingTable(CPU, j, j<=6?0:5);
	
	rtl8651_setAsicPriorityDecision(priorityDecisionArray[PORT_BASE], 
		priorityDecisionArray[D1P_BASE], priorityDecisionArray[DSCP_BASE], 
		priorityDecisionArray[ACL_BASE], priorityDecisionArray[NAT_BASE]);


	rtl8651_flushAsicDot1qAbsolutelyPriority();
	for(i=0; i<RTL8651_PORT_NUMBER + 3; i++)
		rtl8651_setAsicPortDefaultPriority(i,Default_ACLPriority);


	#if 0 /* close by Kevin, for wifi throughput issue */
	for(i =0; i < RTL8651_OUTPUTQUEUE_SIZE; i++)
	{
		for(j=PHY0;j<=CPU;j++)
		{
			if (rtl8651_setAsicQueueFlowControlConfigureRegister(j, i, FALSE)!=SUCCESS)
			{
				QOS_DEBUGP("Set Queue Flow Control Para Error.\n");
			}
		}
	}
	#endif

	rtl865xC_waitForOutputQueueEmpty();
	rtl8651_resetAsicOutputQueue();
	rtl865xC_unLockSWCore();
	
	//memcpy(netIfNameArray, netIfName, NETIF_NUMBER*IFNAMSIZ);

	

	{
		ForEachMasterNetif_Declaration
		ForEachMasterNetif_Start
			rtl865x_regist_aclChain(netif->name, RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS,RTL865X_ACL_NOT_FORCE);
			rtl865x_regist_aclChain(netif->name, RTL865X_ACL_QOS_USED1, RTL865X_ACL_INGRESS,RTL865X_ACL_NOT_FORCE);
		ForEachMasterNetif_End
	}

	#if 0
	for(i=0;i<NETIF_NUMBER;i++)
	{
		if (netIfNameArray[i][0]!=0)
		{
			rtl865x_regist_aclChain(netIfNameArray[i], RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS,RTL865X_ACL_NOT_FORCE);
			rtl865x_regist_aclChain(netIfNameArray[i], RTL865X_ACL_QOS_USED1, RTL865X_ACL_INGRESS,RTL865X_ACL_NOT_FORCE);
		}
	}
	#endif

	memset(defPriority, -1, NETIF_NUMBER*sizeof(uint32));
	//memset(queueNumber, 0, NETIF_NUMBER*sizeof(uint32));
	//memset(priority2HandleMapping, 0, NETIF_NUMBER*TOTAL_VLAN_PRIORITY_NUM*sizeof(int));
	memset(aclpriority_mapping, 0, NETIF_NUMBER*TOTAL_VLAN_PRIORITY_NUM*sizeof(aclpriority_mapping_data_t));

	rtl865x_qosRuleHead = NULL;
	if (rtl865x_compFunc==NULL)
		rtl865x_compFunc = _rtl865x_compare2Entry;

	
	return SUCCESS;
}

void __exit rtl865x_exitOutputQueue(void)
{
	ForEachMasterNetif_Declaration
	ForEachMasterNetif_Start
		rtl865x_unRegist_aclChain(netif->name, RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS);
		rtl865x_unRegist_aclChain(netif->name, RTL865X_ACL_QOS_USED1, RTL865X_ACL_INGRESS);
		rtl865x_qosFlushBandwidth(netif->name);
		rtl865x_closeQos(netif->name);
	ForEachMasterNetif_End

	#if 0
	int	i;
	
	for(i=0;i<NETIF_NUMBER;i++)
	{
		if (netIfNameArray[i][0]!=0)
		{
			rtl865x_unRegist_aclChain(netIfNameArray[i], RTL865X_ACL_QOS_USED0, RTL865X_ACL_INGRESS);
			rtl865x_unRegist_aclChain(netIfNameArray[i], RTL865X_ACL_QOS_USED1, RTL865X_ACL_INGRESS);
			rtl865x_qosFlushBandwidth(netIfNameArray[i]);
			rtl865x_closeQos(netIfNameArray[i]);
		}
	}
	#endif
}

#if defined(CONFIG_RTL_PROC_DEBUG)
int32 rtl865x_show_allQosAcl(void)
{
	rtl865x_qos_rule_t	*qosRule;
	for(qosRule = rtl865x_qosRuleHead;qosRule;qosRule=qosRule->next)
	{
		rtl865x_show_QosAcl(qosRule);
	}
	return SUCCESS;
}
#endif

void rtl865x_show_QosAcl(rtl865x_qos_rule_t	*qosRule)
{
	
	rtl865x_AclRule_t *rule;
	int8 *actionT[] = { "permit", "redirect to ether", "drop", "to cpu", "legacy drop", 
					"drop for log", "mirror", "redirect to pppoe", "default redirect", "mirror keep match", 
					"drop rate exceed pps", "log rate exceed pps", "drop rate exceed bps", "log rate exceed bps","priority "
					};


	rule = qosRule->rule;
	switch(rule->ruleType_)
	{
		case RTL865X_ACL_MAC:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "Ethernet", actionT[rule->actionType_]);
			printk("\tether type: %x   ether type mask: %x\n", rule->typeLen_, rule->typeLenMask_);
			printk("\tDMAC: %x:%x:%x:%x:%x:%x  DMACM: %x:%x:%x:%x:%x:%x\n",
					rule->dstMac_.octet[0], rule->dstMac_.octet[1], rule->dstMac_.octet[2],
					rule->dstMac_.octet[3], rule->dstMac_.octet[4], rule->dstMac_.octet[5],
					rule->dstMacMask_.octet[0], rule->dstMacMask_.octet[1], rule->dstMacMask_.octet[2],
					rule->dstMacMask_.octet[3], rule->dstMacMask_.octet[4], rule->dstMacMask_.octet[5]
					);
			
			printk( "\tSMAC: %x:%x:%x:%x:%x:%x  SMACM: %x:%x:%x:%x:%x:%x\n",
					rule->srcMac_.octet[0], rule->srcMac_.octet[1], rule->srcMac_.octet[2],
					rule->srcMac_.octet[3], rule->srcMac_.octet[4], rule->srcMac_.octet[5],
					rule->srcMacMask_.octet[0], rule->srcMacMask_.octet[1], rule->srcMacMask_.octet[2],
					rule->srcMacMask_.octet[3], rule->srcMacMask_.octet[4], rule->srcMacMask_.octet[5]
				);
			break;

		case RTL865X_ACL_IP:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "IP", actionT[rule->actionType_]);
			printk( "\tdip: %d.%d.%d.%d dipM: %d.%d.%d.%d\n", (rule->dstIpAddr_>>24),
					((rule->dstIpAddr_&0x00ff0000)>>16), ((rule->dstIpAddr_&0x0000ff00)>>8),
					(rule->dstIpAddr_&0xff), (rule->dstIpAddrMask_>>24), ((rule->dstIpAddrMask_&0x00ff0000)>>16),
					((rule->dstIpAddrMask_&0x0000ff00)>>8), (rule->dstIpAddrMask_&0xff)
					);
			printk("\tsip: %d.%d.%d.%d sipM: %d.%d.%d.%d\n", (rule->srcIpAddr_>>24),
					((rule->srcIpAddr_&0x00ff0000)>>16), ((rule->srcIpAddr_&0x0000ff00)>>8),
					(rule->srcIpAddr_&0xff), (rule->srcIpAddrMask_>>24), ((rule->srcIpAddrMask_&0x00ff0000)>>16),
					((rule->srcIpAddrMask_&0x0000ff00)>>8), (rule->srcIpAddrMask_&0xff)
					);
			printk("\tTos: %x   TosM: %x   ipProto: %x   ipProtoM: %x   ipFlag: %x   ipFlagM: %x\n",
					rule->tos_, rule->tosMask_, rule->ipProto_, rule->ipProtoMask_, rule->ipFlag_, rule->ipFlagMask_
				);
			
			printk("\t<FOP:%x> <FOM:%x> <http:%x> <httpM:%x> <IdentSdip:%x> <IdentSdipM:%x> \n",
					rule->ipFOP_, rule->ipFOM_, rule->ipHttpFilter_, rule->ipHttpFilterM_, rule->ipIdentSrcDstIp_,
					rule->ipIdentSrcDstIpM_
					);
			printk( "\t<DF:%x> <MF:%x>\n", rule->ipDF_, rule->ipMF_); 
				break;
				
		case RTL865X_ACL_IP_RANGE:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "IP Range", actionT[rule->actionType_]);
			printk("\tdipU: %d.%d.%d.%d dipL: %d.%d.%d.%d\n", (rule->dstIpAddr_>>24),
					((rule->dstIpAddr_&0x00ff0000)>>16), ((rule->dstIpAddr_&0x0000ff00)>>8),
					(rule->dstIpAddr_&0xff), (rule->dstIpAddrMask_>>24), ((rule->dstIpAddrMask_&0x00ff0000)>>16),
					((rule->dstIpAddrMask_&0x0000ff00)>>8), (rule->dstIpAddrMask_&0xff)
					);
			printk("\tsipU: %d.%d.%d.%d sipL: %d.%d.%d.%d\n", (rule->srcIpAddr_>>24),
					((rule->srcIpAddr_&0x00ff0000)>>16), ((rule->srcIpAddr_&0x0000ff00)>>8),
					(rule->srcIpAddr_&0xff), (rule->srcIpAddrMask_>>24), ((rule->srcIpAddrMask_&0x00ff0000)>>16),
					((rule->srcIpAddrMask_&0x0000ff00)>>8), (rule->srcIpAddrMask_&0xff)
					);
			printk("\tTos: %x   TosM: %x   ipProto: %x   ipProtoM: %x   ipFlag: %x   ipFlagM: %x\n",
					rule->tos_, rule->tosMask_, rule->ipProto_, rule->ipProtoMask_, rule->ipFlag_, rule->ipFlagMask_
					);
			printk("\t<FOP:%x> <FOM:%x> <http:%x> <httpM:%x> <IdentSdip:%x> <IdentSdipM:%x> \n",
					rule->ipFOP_, rule->ipFOM_, rule->ipHttpFilter_, rule->ipHttpFilterM_, rule->ipIdentSrcDstIp_,
					rule->ipIdentSrcDstIpM_
					);
				printk("\t<DF:%x> <MF:%x>\n", rule->ipDF_, rule->ipMF_); 
				break;			
		case RTL865X_ACL_ICMP:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "ICMP", actionT[rule->actionType_]);
			printk("\tdip: %d.%d.%d.%d dipM: %d.%d.%d.%d\n", (rule->dstIpAddr_>>24),
					((rule->dstIpAddr_&0x00ff0000)>>16), ((rule->dstIpAddr_&0x0000ff00)>>8),
					(rule->dstIpAddr_&0xff), (rule->dstIpAddrMask_>>24), ((rule->dstIpAddrMask_&0x00ff0000)>>16),
					((rule->dstIpAddrMask_&0x0000ff00)>>8), (rule->dstIpAddrMask_&0xff)
					);
			printk("\tsip: %d.%d.%d.%d sipM: %d.%d.%d.%d\n", (rule->srcIpAddr_>>24),
					((rule->srcIpAddr_&0x00ff0000)>>16), ((rule->srcIpAddr_&0x0000ff00)>>8),
					(rule->srcIpAddr_&0xff), (rule->srcIpAddrMask_>>24), ((rule->srcIpAddrMask_&0x00ff0000)>>16),
					((rule->srcIpAddrMask_&0x0000ff00)>>8), (rule->srcIpAddrMask_&0xff)
					);
			printk("\tTos: %x   TosM: %x   type: %x   typeM: %x   code: %x   codeM: %x\n",
					rule->tos_, rule->tosMask_, rule->icmpType_, rule->icmpTypeMask_, 
					rule->icmpCode_, rule->icmpCodeMask_);
			break;
		case RTL865X_ACL_ICMP_IPRANGE:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "ICMP IP RANGE", actionT[rule->actionType_]);
			printk("\tdipU: %d.%d.%d.%d dipL: %d.%d.%d.%d\n", (rule->dstIpAddr_>>24),
					((rule->dstIpAddr_&0x00ff0000)>>16), ((rule->dstIpAddr_&0x0000ff00)>>8),
					(rule->dstIpAddr_&0xff), (rule->dstIpAddrMask_>>24), ((rule->dstIpAddrMask_&0x00ff0000)>>16),
					((rule->dstIpAddrMask_&0x0000ff00)>>8), (rule->dstIpAddrMask_&0xff)
					);
			printk("\tsipU: %d.%d.%d.%d sipL: %d.%d.%d.%d\n", (rule->srcIpAddr_>>24),
					((rule->srcIpAddr_&0x00ff0000)>>16), ((rule->srcIpAddr_&0x0000ff00)>>8),
					(rule->srcIpAddr_&0xff), (rule->srcIpAddrMask_>>24), ((rule->srcIpAddrMask_&0x00ff0000)>>16),
					((rule->srcIpAddrMask_&0x0000ff00)>>8), (rule->srcIpAddrMask_&0xff)
					);
			printk("\tTos: %x   TosM: %x   type: %x   typeM: %x   code: %x   codeM: %x\n",
					rule->tos_, rule->tosMask_, rule->icmpType_, rule->icmpTypeMask_, 
					rule->icmpCode_, rule->icmpCodeMask_);
			break;
		case RTL865X_ACL_IGMP:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "IGMP", actionT[rule->actionType_]);
			printk("\tdip: %d.%d.%d.%d dipM: %d.%d.%d.%d\n", (rule->dstIpAddr_>>24),
					((rule->dstIpAddr_&0x00ff0000)>>16), ((rule->dstIpAddr_&0x0000ff00)>>8),
					(rule->dstIpAddr_&0xff), (rule->dstIpAddrMask_>>24), ((rule->dstIpAddrMask_&0x00ff0000)>>16),
					((rule->dstIpAddrMask_&0x0000ff00)>>8), (rule->dstIpAddrMask_&0xff)
					);
			printk("\tsip: %d.%d.%d.%d sipM: %d.%d.%d.%d\n", (rule->srcIpAddr_>>24),
					((rule->srcIpAddr_&0x00ff0000)>>16), ((rule->srcIpAddr_&0x0000ff00)>>8),
					(rule->srcIpAddr_&0xff), (rule->srcIpAddrMask_>>24), ((rule->srcIpAddrMask_&0x00ff0000)>>16),
					((rule->srcIpAddrMask_&0x0000ff00)>>8), (rule->srcIpAddrMask_&0xff)
					);
			printk("\tTos: %x   TosM: %x   type: %x   typeM: %x\n", rule->tos_, rule->tosMask_,
					rule->igmpType_, rule->igmpTypeMask_
					);
			break;


		case RTL865X_ACL_IGMP_IPRANGE:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "IGMP IP RANGE", actionT[rule->actionType_]);
			printk("\tdip: %d.%d.%d.%d dipM: %d.%d.%d.%d\n", (rule->dstIpAddr_>>24),
					((rule->dstIpAddr_&0x00ff0000)>>16), ((rule->dstIpAddr_&0x0000ff00)>>8),
					(rule->dstIpAddr_&0xff), (rule->dstIpAddrMask_>>24), ((rule->dstIpAddrMask_&0x00ff0000)>>16),
					((rule->dstIpAddrMask_&0x0000ff00)>>8), (rule->dstIpAddrMask_&0xff)
					);
			printk("\tsip: %d.%d.%d.%d sipM: %d.%d.%d.%d\n", (rule->srcIpAddr_>>24),
					((rule->srcIpAddr_&0x00ff0000)>>16), ((rule->srcIpAddr_&0x0000ff00)>>8),
					(rule->srcIpAddr_&0xff), (rule->srcIpAddrMask_>>24), ((rule->srcIpAddrMask_&0x00ff0000)>>16),
					((rule->srcIpAddrMask_&0x0000ff00)>>8), (rule->srcIpAddrMask_&0xff)
					);
			printk("\tTos: %x   TosM: %x   type: %x   typeM: %x\n", rule->tos_, rule->tosMask_,
					rule->igmpType_, rule->igmpTypeMask_
					);
			break;

		case RTL865X_ACL_TCP:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "TCP", actionT[rule->actionType_]);
			printk("\tdip: %d.%d.%d.%d dipM: %d.%d.%d.%d\n", (rule->dstIpAddr_>>24),
					((rule->dstIpAddr_&0x00ff0000)>>16), ((rule->dstIpAddr_&0x0000ff00)>>8),
					(rule->dstIpAddr_&0xff), (rule->dstIpAddrMask_>>24), ((rule->dstIpAddrMask_&0x00ff0000)>>16),
					((rule->dstIpAddrMask_&0x0000ff00)>>8), (rule->dstIpAddrMask_&0xff)
					);
			printk("\tsip: %d.%d.%d.%d sipM: %d.%d.%d.%d\n", (rule->srcIpAddr_>>24),
					((rule->srcIpAddr_&0x00ff0000)>>16), ((rule->srcIpAddr_&0x0000ff00)>>8),
					(rule->srcIpAddr_&0xff), (rule->srcIpAddrMask_>>24), ((rule->srcIpAddrMask_&0x00ff0000)>>16),
					((rule->srcIpAddrMask_&0x0000ff00)>>8), (rule->srcIpAddrMask_&0xff)
					);
			printk("\tTos:%x  TosM:%x  sportL:%d  sportU:%d  dportL:%d  dportU:%d\n",
					rule->tos_, rule->tosMask_, rule->tcpSrcPortLB_, rule->tcpSrcPortUB_,
					rule->tcpDstPortLB_, rule->tcpDstPortUB_
					);
			printk("\tflag: %x  flagM: %x  <URG:%x> <ACK:%x> <PSH:%x> <RST:%x> <SYN:%x> <FIN:%x>\n",
					rule->tcpFlag_, rule->tcpFlagMask_, rule->tcpURG_, rule->tcpACK_,
					rule->tcpPSH_, rule->tcpRST_, rule->tcpSYN_, rule->tcpFIN_
					);
			break;
		case RTL865X_ACL_TCP_IPRANGE:
				printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "TCP IP RANGE", actionT[rule->actionType_]);
				printk("\tdipU: %d.%d.%d.%d dipL: %d.%d.%d.%d\n", (rule->dstIpAddr_>>24),
					((rule->dstIpAddr_&0x00ff0000)>>16), ((rule->dstIpAddr_&0x0000ff00)>>8),
					(rule->dstIpAddr_&0xff), (rule->dstIpAddrMask_>>24), ((rule->dstIpAddrMask_&0x00ff0000)>>16),
					((rule->dstIpAddrMask_&0x0000ff00)>>8), (rule->dstIpAddrMask_&0xff)
					);
				printk("\tsipU: %d.%d.%d.%d sipL: %d.%d.%d.%d\n", (rule->srcIpAddr_>>24),
					((rule->srcIpAddr_&0x00ff0000)>>16), ((rule->srcIpAddr_&0x0000ff00)>>8),
					(rule->srcIpAddr_&0xff), (rule->srcIpAddrMask_>>24), ((rule->srcIpAddrMask_&0x00ff0000)>>16),
					((rule->srcIpAddrMask_&0x0000ff00)>>8), (rule->srcIpAddrMask_&0xff)
					);
				printk("\tTos:%x  TosM:%x  sportL:%d  sportU:%d  dportL:%d  dportU:%d\n",
					rule->tos_, rule->tosMask_, rule->tcpSrcPortLB_, rule->tcpSrcPortUB_,
					rule->tcpDstPortLB_, rule->tcpDstPortUB_
					);
				printk("\tflag: %x  flagM: %x  <URG:%x> <ACK:%x> <PSH:%x> <RST:%x> <SYN:%x> <FIN:%x>\n",
					rule->tcpFlag_, rule->tcpFlagMask_, rule->tcpURG_, rule->tcpACK_,
					rule->tcpPSH_, rule->tcpRST_, rule->tcpSYN_, rule->tcpFIN_
				);
			break;

		case RTL865X_ACL_UDP:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx,"UDP", actionT[rule->actionType_]);
			printk("\tdip: %d.%d.%d.%d dipM: %d.%d.%d.%d\n", (rule->dstIpAddr_>>24),
					((rule->dstIpAddr_&0x00ff0000)>>16), ((rule->dstIpAddr_&0x0000ff00)>>8),
					(rule->dstIpAddr_&0xff), (rule->dstIpAddrMask_>>24), ((rule->dstIpAddrMask_&0x00ff0000)>>16),
					((rule->dstIpAddrMask_&0x0000ff00)>>8), (rule->dstIpAddrMask_&0xff)
					);
			printk("\tsip: %d.%d.%d.%d sipM: %d.%d.%d.%d\n", (rule->srcIpAddr_>>24),
					((rule->srcIpAddr_&0x00ff0000)>>16), ((rule->srcIpAddr_&0x0000ff00)>>8),
					(rule->srcIpAddr_&0xff), (rule->srcIpAddrMask_>>24), ((rule->srcIpAddrMask_&0x00ff0000)>>16),
					((rule->srcIpAddrMask_&0x0000ff00)>>8), (rule->srcIpAddrMask_&0xff)
					);
			printk("\tTos:%x  TosM:%x  sportL:%d  sportU:%d  dportL:%d  dportU:%d\n",
					rule->tos_, rule->tosMask_, rule->udpSrcPortLB_, rule->udpSrcPortUB_,
					rule->udpDstPortLB_, rule->udpDstPortUB_
					);
			break;				
		case RTL865X_ACL_UDP_IPRANGE:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "UDP IP RANGE", actionT[rule->actionType_]);
			printk("\tdipU: %d.%d.%d.%d dipL: %d.%d.%d.%d\n", (rule->dstIpAddr_>>24),
					((rule->dstIpAddr_&0x00ff0000)>>16), ((rule->dstIpAddr_&0x0000ff00)>>8),
					(rule->dstIpAddr_&0xff), (rule->dstIpAddrMask_>>24), ((rule->dstIpAddrMask_&0x00ff0000)>>16),
					((rule->dstIpAddrMask_&0x0000ff00)>>8), (rule->dstIpAddrMask_&0xff)
					);
			printk("\tsipU: %d.%d.%d.%d sipL: %d.%d.%d.%d\n", (rule->srcIpAddr_>>24),
					((rule->srcIpAddr_&0x00ff0000)>>16), ((rule->srcIpAddr_&0x0000ff00)>>8),
					(rule->srcIpAddr_&0xff), (rule->srcIpAddrMask_>>24), ((rule->srcIpAddrMask_&0x00ff0000)>>16),
					((rule->srcIpAddrMask_&0x0000ff00)>>8), (rule->srcIpAddrMask_&0xff)
					);
			printk("\tTos:%x  TosM:%x  sportL:%d  sportU:%d  dportL:%d  dportU:%d\n",
					rule->tos_, rule->tosMask_, rule->udpSrcPortLB_, rule->udpSrcPortUB_,
					rule->udpDstPortLB_, rule->udpDstPortUB_
				);
			break;				

		
		case RTL865X_ACL_SRCFILTER:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "Source Filter", actionT[rule->actionType_]);
			printk("\tSMAC: %x:%x:%x:%x:%x:%x  SMACM: %x:%x:%x:%x:%x:%x\n", 
					rule->srcFilterMac_.octet[0], rule->srcFilterMac_.octet[1], rule->srcFilterMac_.octet[2], 
					rule->srcFilterMac_.octet[3], rule->srcFilterMac_.octet[4], rule->srcFilterMac_.octet[5],
					rule->srcFilterMacMask_.octet[0], rule->srcFilterMacMask_.octet[1], rule->srcFilterMacMask_.octet[2],
					rule->srcFilterMacMask_.octet[3], rule->srcFilterMacMask_.octet[4], rule->srcFilterMacMask_.octet[5]
					);
			printk("\tsvidx: %d   svidxM: %x   sport: %d   sportM: %x   ProtoType: %x\n",
					rule->srcFilterVlanIdx_, rule->srcFilterVlanIdxMask_, rule->srcFilterPort_, rule->srcFilterPortMask_,
					(rule->srcFilterIgnoreL3L4_==TRUE? 2: (rule->srcFilterIgnoreL4_ == 1? 1: 0))
					);
			printk("\tsip: %d.%d.%d.%d   sipM: %d.%d.%d.%d\n", (rule->srcFilterIpAddr_>>24),
					((rule->srcFilterIpAddr_&0x00ff0000)>>16), ((rule->srcFilterIpAddr_&0x0000ff00)>>8),
					(rule->srcFilterIpAddr_&0xff), (rule->srcFilterIpAddrMask_>>24),
					((rule->srcFilterIpAddrMask_&0x00ff0000)>>16), ((rule->srcFilterIpAddrMask_&0x0000ff00)>>8),
					(rule->srcFilterIpAddrMask_&0xff)
					);
			printk("\tsportL: %d   sportU: %d\n", rule->srcFilterPortLowerBound_, rule->srcFilterPortUpperBound_);
			break;

		case RTL865X_ACL_SRCFILTER_IPRANGE:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "Source Filter(IP RANGE)", actionT[rule->actionType_]);
			printk("\tSMAC: %x:%x:%x:%x:%x:%x  SMACM: %x:%x:%x:%x:%x:%x\n", 
					rule->srcFilterMac_.octet[0], rule->srcFilterMac_.octet[1], rule->srcFilterMac_.octet[2], 
					rule->srcFilterMac_.octet[3], rule->srcFilterMac_.octet[4], rule->srcFilterMac_.octet[5],
					rule->srcFilterMacMask_.octet[0], rule->srcFilterMacMask_.octet[1], rule->srcFilterMacMask_.octet[2],
					rule->srcFilterMacMask_.octet[3], rule->srcFilterMacMask_.octet[4], rule->srcFilterMacMask_.octet[5]
					);
			printk("\tsvidx: %d   svidxM: %x   sport: %d   sportM: %x   ProtoType: %x\n",
					rule->srcFilterVlanIdx_, rule->srcFilterVlanIdxMask_, rule->srcFilterPort_, rule->srcFilterPortMask_,
					(rule->srcFilterIgnoreL3L4_==TRUE? 2: (rule->srcFilterIgnoreL4_ == 1? 1: 0))
					);
			printk("\tsipU: %d.%d.%d.%d   sipL: %d.%d.%d.%d\n", (rule->srcFilterIpAddr_>>24),
					((rule->srcFilterIpAddr_&0x00ff0000)>>16), ((rule->srcFilterIpAddr_&0x0000ff00)>>8),
					(rule->srcFilterIpAddr_&0xff), (rule->srcFilterIpAddrMask_>>24),
					((rule->srcFilterIpAddrMask_&0x00ff0000)>>16), ((rule->srcFilterIpAddrMask_&0x0000ff00)>>8),
					(rule->srcFilterIpAddrMask_&0xff)
					);
			printk("\tsportL: %d   sportU: %d\n", rule->srcFilterPortLowerBound_, rule->srcFilterPortUpperBound_);
			break;

		case RTL865X_ACL_DSTFILTER:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "Deatination Filter", actionT[rule->actionType_]);
			printk("\tDMAC: %x:%x:%x:%x:%x:%x  DMACM: %x:%x:%x:%x:%x:%x\n", 
					rule->dstFilterMac_.octet[0], rule->dstFilterMac_.octet[1], rule->dstFilterMac_.octet[2], 
					rule->dstFilterMac_.octet[3], rule->dstFilterMac_.octet[4], rule->dstFilterMac_.octet[5],
					rule->dstFilterMacMask_.octet[0], rule->dstFilterMacMask_.octet[1], rule->dstFilterMacMask_.octet[2],
					rule->dstFilterMacMask_.octet[3], rule->dstFilterMacMask_.octet[4], rule->dstFilterMacMask_.octet[5]
					);
			printk("\tdvidx: %d   dvidxM: %x  ProtoType: %x   dportL: %d   dportU: %d\n",
					rule->dstFilterVlanIdx_, rule->dstFilterVlanIdxMask_, 
					(rule->dstFilterIgnoreL3L4_==TRUE? 2: (rule->dstFilterIgnoreL4_ == 1? 1: 0)), 
					rule->dstFilterPortLowerBound_, rule->dstFilterPortUpperBound_
					);
			printk("\tdip: %d.%d.%d.%d   dipM: %d.%d.%d.%d\n", (rule->dstFilterIpAddr_>>24),
					((rule->dstFilterIpAddr_&0x00ff0000)>>16), ((rule->dstFilterIpAddr_&0x0000ff00)>>8),
					(rule->dstFilterIpAddr_&0xff), (rule->dstFilterIpAddrMask_>>24),
					((rule->dstFilterIpAddrMask_&0x00ff0000)>>16), ((rule->dstFilterIpAddrMask_&0x0000ff00)>>8),
					(rule->dstFilterIpAddrMask_&0xff)
					);
			break;
		case RTL865X_ACL_DSTFILTER_IPRANGE:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "Deatination Filter(IP Range)", actionT[rule->actionType_]);
			printk("\tDMAC: %x:%x:%x:%x:%x:%x  DMACM: %x:%x:%x:%x:%x:%x\n", 
					rule->dstFilterMac_.octet[0], rule->dstFilterMac_.octet[1], rule->dstFilterMac_.octet[2], 
					rule->dstFilterMac_.octet[3], rule->dstFilterMac_.octet[4], rule->dstFilterMac_.octet[5],
					rule->dstFilterMacMask_.octet[0], rule->dstFilterMacMask_.octet[1], rule->dstFilterMacMask_.octet[2],
					rule->dstFilterMacMask_.octet[3], rule->dstFilterMacMask_.octet[4], rule->dstFilterMacMask_.octet[5]
					);
			printk("\tdvidx: %d   dvidxM: %x  ProtoType: %x   dportL: %d   dportU: %d\n",
					rule->dstFilterVlanIdx_, rule->dstFilterVlanIdxMask_, 
					(rule->dstFilterIgnoreL3L4_==TRUE? 2: (rule->dstFilterIgnoreL4_ == 1? 1: 0)), 
					rule->dstFilterPortLowerBound_, rule->dstFilterPortUpperBound_
					);
			printk("\tdipU: %d.%d.%d.%d   dipL: %d.%d.%d.%d\n", (rule->dstFilterIpAddr_>>24),
					((rule->dstFilterIpAddr_&0x00ff0000)>>16), ((rule->dstFilterIpAddr_&0x0000ff00)>>8),
					(rule->dstFilterIpAddr_&0xff), (rule->dstFilterIpAddrMask_>>24),
					((rule->dstFilterIpAddrMask_&0x00ff0000)>>16), ((rule->dstFilterIpAddrMask_&0x0000ff00)>>8),
					(rule->dstFilterIpAddrMask_&0xff)
				);
			break;
		case RTL865X_ACL_802D1P:
			printk(" [%d] rule type: %s   rule action: %s\n", rule->aclIdx, "802.1P", actionT[rule->actionType_]);
			printk("\t802.1p: 0x%X \n", rule->vlanTagPri_);
			break;

			default:
				printk("rule->ruleType_(0x%x)\n", rule->ruleType_);

	}	

	switch (rule->actionType_) 
	{
		case RTL865X_ACL_PERMIT:
		case RTL865X_ACL_REDIRECT_ETHER:
		case RTL865X_ACL_DROP:
		case RTL865X_ACL_TOCPU:
		case RTL865X_ACL_LEGACY_DROP:
		case RTL865X_ACL_DROPCPU_LOG:
		case RTL865X_ACL_MIRROR:
		case RTL865X_ACL_REDIRECT_PPPOE:
		case RTL865X_ACL_MIRROR_KEEP_MATCH:
			printk("\tnetifIdx: %d   pppoeIdx: %d   l2Idx:%d  ", rule->netifIdx_, rule->pppoeIdx_, rule->L2Idx_);
			break;

		case RTL865X_ACL_PRIORITY: 
			printk("\tprioirty: %d   ",  rule->priority_) ;
			break;
			
		case RTL865X_ACL_DEFAULT_REDIRECT:
			printk("\tnextHop:%d  ",  rule->nexthopIdx_);
			break;

		case RTL865X_ACL_DROP_RATE_EXCEED_PPS:
		case RTL865X_ACL_LOG_RATE_EXCEED_PPS:
		case RTL865X_ACL_DROP_RATE_EXCEED_BPS:
		case RTL865X_ACL_LOG_RATE_EXCEED_BPS:
			printk("\tratelimitIdx: %d  ",  rule->ratelimtIdx_);
			break;
		default: 
			;
		
	}
	printk("pktOpApp: %d	Handler:0x%08X   Mark: 0x%08X   Classify: 0x%08X   Remark_8021p: 0x%08X   Remark_DSCP: 0x%08X\n"
		,  rule->pktOpApp_, qosRule->handle, qosRule->mark, qosRule->classify, qosRule->remark_8021p, qosRule->remark_dscp);
	printk("\tInDev: %s	OutDev: %s\n", qosRule->inIfname[0]==0?"NULL":qosRule->inIfname, qosRule->outIfname[0]==0?"NULL":qosRule->outIfname);

}


