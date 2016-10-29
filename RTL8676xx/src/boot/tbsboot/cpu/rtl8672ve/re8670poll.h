

/*
 *  data structure
 */
typedef volatile struct _nic_frame_descriptor_
        {
               unsigned long    StsLen;
               unsigned long    DataPtr;
               unsigned long    VLan;
               unsigned long    Reserved;
               //unsigned long    Reserved2;
        } NIC_FD_T;

/*
 *  general definitions
 */
 
/*================================== Declaration ==================================*/
/* Symbolic offsets to registers. */
#define VPint32   *(volatile unsigned int *)
#define VPint16	*(volatile unsigned short *)
#define VPint8  *(volatile unsigned char *)
int regbase;

#define NIC_ID0				(VPint32(regbase+0x0000))		/* Interrupt Mitigation register */
#define NIC_ID1				(VPint32(regbase+0x0004))		/* Interrupt Mask Register */
#define CR					(VPint8(regbase+0x003B))		/* Control register */
#define EIMR				(VPint16(regbase+0x003C))			/* Interrupt Status */
#define EISR				(VPint16(regbase+0x003E))				/* Interrupt Mask of MAC */
#define TCR					(VPint32(regbase+0x0040))		/* Type Match 0 Register */
#define RCR					(VPint32(regbase+0x0044))		/* Interrupt Status Register */
#define MSR					(VPint8(regbase+0x0058))		/* Media Independent Interface Access Register */
#define TxFDP1				(VPint32(regbase+0x1300))			/* Interrupt Routing */
#define TxCDO1				(VPint16(regbase+0x1304))		/* Ethernet ID */
#define RxFDP				(VPint32(regbase+0x13F0))		/* Starting Address of Tx Descriptor */
#define RxCDO				(VPint16(regbase+0x13F4))		/* Starting Address of Rx Descriptor */
#define RxRingSize			(VPint8(regbase+0x13F6))		/* Type Match 3 Regsiter */
#define EthrntRxCPU_Des_Num	(VPint8(regbase+0x1430))		/* Type Match 2 Register */
#define Rx_Pse_Des_Thres	(VPint8(regbase+0x1432))		/* Type Match 1 Register */
#define IO_CMD				(VPint32(regbase+0x1434))		/* Multicast register */





/* Descriptor setting */
#define TX_DESC_NUM				32			// must be exponent of 2
#define RX_DESC_NUM				16			// must be exponent of 2
#define TX_MAX_SIZE				1518
#define MAX_PKT_SIZE			2048
#define RX_DESC_BUFFER_SIZE		0x800
#define SHIFT_RX				((RX_DESC_NUM-1)<<2)
#define OWN_BIT					0x80000000
#define EOR_BIT					0x40000000
#define FS_BIT					0x20000000
#define LS_BIT					0x10000000
#define TES_BIT					0x00800000
#define IPCS					0x4
#define TCPCS					0x5
#define UDPCS					0x6
#define CS_SH					16
#define VLANTAG					0x1234
#define PROTO_IP_0				0x08
#define PROTO_IP_1				0x0
#define PROTO_TCP				0x6
#define PROTO_UDP				0x11


/* Misc */

#define UNCACHE_MASK			0x20000000

enum {
	/* RCR */
 	AcceptErr			= 0x20,	     /* Accept packets with CRC errors */
	AcceptRunt			= 0x10,	     /* Accept runt (<64 bytes) packets */
	AcceptBroadcast		= 0x08,	     /* Accept broadcast packets */
	AcceptMulticast		= 0x04,	     /* Accept multicast packets */
	AcceptMyPhys		= 0x02,	     /* Accept pkts with our MAC as dest */
	AcceptAllPhys		= 0x01,	     /* Accept all pkts w/ physical dest */
	AcceptAll 			= AcceptBroadcast | AcceptMulticast | AcceptMyPhys |  AcceptAllPhys | AcceptErr | AcceptRunt,
	AcceptNoBroad 		= AcceptMulticast |AcceptMyPhys |  AcceptAllPhys | AcceptErr | AcceptRunt,
	AcceptNoMulti 		= AcceptMyPhys |  AcceptAllPhys | AcceptErr | AcceptRunt,
	NoErrAccept 		= AcceptBroadcast | AcceptMulticast | AcceptMyPhys,
	NoErrPromiscAccept 	= AcceptBroadcast | AcceptMulticast | AcceptMyPhys |  AcceptAllPhys,

	/* TCR */
	TCR_IFG				= 0x3,
	TCR_IFG_OFFSET	= 10,
	TCR_NORMAL		= 0,
	TCR_LOOPBACK		= 3,
	TCR_MODE_OFFSET	= 8,

	/* Rx flow control descriptor threshold (0~31) */
	RX_FC_THRESHOLD	= 8,

	/* MSR */
	TXFCE				= 1<<7,
	RXFCE				= 1<<6,
	SP100				= 1<<3,
	LINK				= 1<<2,
	TXPF				= 1<<1,
	RXPF				= 1<<0,
	FORCE_TX 			= 1<<5,

	/* Ethernet IO CMD */
	RX_MIT 				= 3,
	RX_TIMER 			= 1,
	RX_FIFO 			= 2,
	TX_FIFO 			= 1,
	TX_MIT				= 7,	
	TE					= 1,
	RE					= 1,
	CMD_CONFIG 		= RE << 5  | TE << 4  | RX_MIT << 8 | RX_FIFO << 11 |  RX_TIMER << 13 | TX_MIT << 16 | TX_FIFO<<19 | 0x1<<30, //tylo, for8672 TE,RE changed, packet processor disabled
	RX_DISALBE 			= TE << 2  | RX_MIT << 8 | RX_FIFO << 11 |  RX_TIMER << 13 | TX_MIT << 16 | TX_FIFO<<19,
};


