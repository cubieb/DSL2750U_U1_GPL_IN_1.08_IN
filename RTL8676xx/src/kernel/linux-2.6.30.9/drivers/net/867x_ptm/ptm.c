/* re8670_ptm.c: A Linux PTM driver for the RealTek 8672 chips. */
/*
	Copyright 2001,2002 Jeff Garzik <jgarzik@mandrakesoft.com>

	Copyright (C) 2001, 2002 David S. Miller (davem@redhat.com) [tg3.c]
	Copyright (C) 2000, 2001 David S. Miller (davem@redhat.com) [sungem.c]
	Copyright 2001 Manfred Spraul				    [natsemi.c]
	Copyright 1999-2001 by Donald Becker.			    [natsemi.c]
       	Written 1997-2001 by Donald Becker.			    [8139too.c]
	Copyright 1998-2001 by Jes Sorensen, <jes@trained-monkey.org>. [acenic.c]

	This software may be used and distributed according to the terms of
	the GNU General Public License (GPL), incorporated herein by reference.
	Drivers based on or derived from this code fall under the GPL and must
	retain the authorship, copyright and license notice.  This file is not
	a complete program and may only be used when the entire operating
	system is licensed under the GPL.

	See the file COPYING in this distribution for more information.

 */

#define DRV_NAME		"RTL8672_PTM0"
#define DRV_VERSION		"0.0.1"
#define DRV_RELDATE		"Aug 14, 2008"
//#define PRE_TEST
#if !defined(PRE_TEST)
#define PTM2
#ifdef PTM2
#define PTMCTL_LOG
//#define ENABLE_LOOPBACK
//#define HDLC_MODE
#endif
#endif


#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/delay.h>
#include <linux/ethtool.h>
#include <linux/mii.h>
#include <linux/if_vlan.h>
#include <linux/crc32.h>
#include <asm/io.h>
#include <asm/uaccess.h>
#include <linux/slab.h>


#ifdef CONFIG_RTL8671
  #ifdef CONFIG_UCLINUX
    #include "../../arch/mipsnommu/rtl8670/lx5280.h"
  #else
    #include "../../arch/mips/realtek/rtl8670/lx5280.h"
    #include "../../arch/mips/realtek/rtl8670/gpio.h"
  #endif
#endif

#ifdef CONFIG_RTL8672	
#include <bspchip.h>
#include <linux/version.h>
#include "../../arch/rlx/bsp/gpio.h"
#endif

#ifdef CONFIG_RTL867X_PACKET_PROCESSOR
#include "../packet_processor/rtl8672PacketProcessor.h"
#endif



unsigned int disable_rx_message = 1;

static int ptm_start_xmit (struct sk_buff *skb, struct net_device *dev);
int __ptm_start_xmit(struct sk_buff *skb, struct net_device *dev, short fast, char channel);

static struct net_device *ptm_net_dev;
extern struct net_device *sar_dev;

#define WITH_NAPI		""


/* These identify the driver base version and may not be removed. */
static char version[] __devinitdata =
KERN_INFO DRV_NAME " Ethernet driver v" DRV_VERSION " (" DRV_RELDATE ")" WITH_NAPI "\n";

MODULE_AUTHOR("Rupert Chang <rupert@mail.realtek.com.tw>");
MODULE_DESCRIPTION("RealTek RTL-8670 series 10/100 Ethernet driver");
MODULE_LICENSE("GPL");


/* Maximum number of multicast addresses to filter (vs. Rx-all-multicast).
   The RTL chips use a 64 element hash table based on the Ethernet CRC.  */
static int multicast_filter_limit = 32;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 9)	//shlee 2.6
module_param(multicast_filter_limit, int, S_IRUGO|S_IWUSR)
#else
MODULE_PARM (multicast_filter_limit, "i");
#endif
MODULE_PARM_DESC (multicast_filter_limit, "8139cp maximum number of filtered multicast addresses");

#define PFX			DRV_NAME ": "
#define CP_DEF_MSG_ENABLE	(NETIF_MSG_DRV		| \
				 NETIF_MSG_PROBE 	| \
				 NETIF_MSG_LINK)
#define CP_REGS_SIZE		(0xff + 1)
#define RE8670_RX_RING_SIZE	8//16~256 descriptors
#define RE8670_TX_RING_SIZE	4

#define DESC_ALIGN		0x100
#define UNCACHE_MASK		0xa0000000

#define RE8670_RXRING_BYTES	( (sizeof(struct dma_desc) * (RE8670_RX_RING_SIZE+1)) + DESC_ALIGN)

#define RE8670_TXRING_BYTES	( (sizeof(struct dma_desc) * (RE8670_TX_RING_SIZE+1)) + DESC_ALIGN)
		


#define NEXT_TX(N)		(((N) + 1) & (RE8670_TX_RING_SIZE - 1))
#define NEXT_RX(N)		(((N) + 1) & (RE8670_RX_RING_SIZE - 1))

#define PKT_BUF_SZ		1536	/* Size of each temporary Rx buffer.*/
#define RX_OFFSET		2


/* The following settings are log_2(bytes)-4:  0 == 16 bytes .. 6==1024, 7==end of packet. */

/* Time in jiffies before concluding the transmitter is hung. */
#define TX_TIMEOUT		(6*HZ)

/* hardware minimum and maximum for a single frame's data payload */
#define CP_MIN_MTU		60	/* TODO: allow lower, but pad */
#define CP_MAX_MTU		4096


#define ETHBASE	0xB8400000  // PTM 0
#define SARCHEM_BASE	0xB8A80000 

int flag=0;

#define _CRCLNG_				4

#define RTL_W32(reg, value)			(*(volatile u32*)(ETHBASE+reg)) = (u32)value
#define RTL_W16(reg, value)			(*(volatile u16*)(ETHBASE+reg)) = (u16)value
#define RTL_W8(reg, value)			(*(volatile u8*)(ETHBASE+reg)) = (u8)value
#define RTL_R32(reg)				(*(volatile u32*)(ETHBASE+reg))
#define RTL_R16(reg)				(*(volatile u16*)(ETHBASE+reg))
#define RTL_R8(reg)					(*(volatile u8*)(ETHBASE+reg))





enum PHY_REGS{
	TXFCE= 1<<7,
	RXFCE=1 <<6,
	SP100=1<<3,
	LINK=1<<2,
	TXPF=1<<1,
	RXPF=1<<0,
	FORCE_TX = 1<<5

};


enum {
//PTM Module registers
	IDR0 = 0,			/* Ethernet ID */
	IDR1 = 0x1,			/* Ethernet ID */
	IDR2 = 0x2,			/* Ethernet ID */
	IDR3 = 0x3,			/* Ethernet ID */
	IDR4 = 0x4,			/* Ethernet ID */
	IDR5 = 0x5,			/* Ethernet ID */
	MAR0 = 0x8,			/* Multicast register */
	MAR1 = 0x9,
	MAR2 = 0xa,
	MAR3 = 0xb,
	MAR4 = 0xc,
	MAR5 = 0xd,
	MAR6 = 0xe,
	MAR7 = 0xf,
	TXDMAOKCNT_CH0_F = 0x10,
	TXDMAOKCNT_CH1_F = 0x14,
	TXDMAOKCNT_CH2_F = 0x18,
	TXDMAOKCNT_CH3_F = 0x1c,
	TXDMAOKCNT_CH0_S = 0x20,
	TXDMAOKCNT_CH1_S = 0x24,
	TXDMAOKCNT_CH2_S = 0x28,
	TXDMAOKCNT_CH3_S = 0x2c,
	//TxUndrn=0x12,
	RxOKCNT_L_F = 0x50,
	RxOKCNT_H_F = 0x54,
	RxOKCNT_L_S = 0x58,
	RxOKCNT_H_S = 0x5c,
	RxErr_L_F = 0x60,
	RxErr_H_F= 0x64,
	RxErr_L_S = 0x68,
	RxErr_H_S = 0x6c,
	//TX1COL = 0x1c,
	MissPkt_L_F = 0x70,
	MissPkt_H_F = 0x74,
	MissPkt_L_S = 0x78,
	MissPkt_H_S = 0x7c,
	RxOkPhy_L_F = 0x80,
	RxOkPhy_H_F = 0x84,
	RxOkPhy_L_S = 0x88,
	RxOkPhy_H_S = 0x8c,
	RxOkBrd_L_F = 0x90,
	RxOkBrd_H_F = 0x94,
	RxOkBrd_L_S = 0x98,
	RxOkBrd_H_S = 0x9c,
	RxOkMu1_L_F = 0xa0,
	RxOkMu1_H_F = 0xa4,
	RxOkMu1_L_S = 0xa8,
	RxOkMu1_H_S = 0xac,
	//TxErr = 0x30,
	TRSR = 0xb0,
	CMD = 0xb7,
	IMR0 = 0xb8,
	ISR0 = 0xbc,
	IMR1 = 0xc0,
	TCR = 0xc2,
	ISR1 = 0xc4,
	RCR = 0xc6,
	PCR = 0xc8,
	//MSR=0x58,
	TCTX_PKT = 0xcc,
	TCRX_PKT = 0xd0,
	TCRX_GOODPKT = 0xd4,
	TCRX_BADPKT = 0xd8,
	TCRX_INVALIDPKT = 0xdc,
	SWINT_REG = 0xec,
	MBR = 0xf0,

//QoS register
	DMA_APR32_F = 0x1000,
	DMA_APR10_F = 0x1004,
	DMA_APR32_S = 0x1008,
	DMA_APR10_S = 0x100c,
	BOND_FRAG_LF = 0x1060,
	BOND_FRAG_HF = 0x1064,
	BOND_FRAG_LS = 0x1068,
	BOND_FRAG_HS = 0x106c,
	BD_SLV_NUM = 0x10d0,
	BD_TIMEOUT = 0x10d4,

//CPU Interface registers
	TxFDP0_F = 0x1300,
	TxCDO0_F = 0x1304,
	TxFDP1_F = 0x1310,
	TxCDO1_F = 0x1314,
	TxFDP2_F = 0x1320,
	TxCDO2_F = 0x1324,
	TxFDP3_F = 0x1330,
	TxCDO3_F = 0x1334,
	TxFDP0_S = 0x1340,
	TxCDO0_S = 0x1344,
	TxFDP1_S = 0x1350,
	TxCDO1_S = 0x1354,
	TxFDP2_S = 0x1360,
	TxCDO2_S = 0x1364,
	TxFDP3_S = 0x1370,
	TxCDO3_S = 0x1374,
	RxFDP_L_F = 0x13e0,
	RxCDO_L_F = 0x13e4,
	RxFDP_H_F = 0x13e8,
	RxCDO_H_F = 0x13ec,
	RxFDP_L_S = 0x13f0,
	RxCDO_L_S = 0x13f4,
	RxFDP_H_S = 0x13f8,
	RxCDO_H_S = 0x13fc,
	//RxRingSize=0x13F6,
	//RxFDP_H=0x13F8,
	//RxCDO_H=0x13Fc,
	//RxRingSize_H=0x13FE,	
	//RXPLEN=0x1403,
	//LASTRXCDO=0x1402,
	//RXPFDP=0x1404,
	//RXPAGECNT=0x1408,
	//RXSCRATCHDES=0x1410,
	//EthrntRxCPU_Des_Num_H = 0x1420,
	//EthrntRxCPU_Des_Wrap_H =0x1421,	//high queue threshold on register for flow control
	//Rx_Pse_Des_Thres_H = 	0x1422,	//high queue threshold off register for flow control 
	
	//EthrntRxCPU_Des_Num=0x1430,
	//EthrntRxCPU_Des_Wrap =	0x1431,
	//Rx_Pse_Des_Thres = 	0x1432,	
	
	IO_CMD = 0x1434,
	CH_PRI = 0x1438,
	RX_IntNum_Shift = 0x8,             /// ????
	TX_OWN = (1<<5),
	RX_OWN = (1<<4),
	MII_RE = (1<<3),
	MII_TE = (1<<2),
	TX_FNL = (1<<1),
	TX_FNH = (1),
	/*TX_START= MII_RE | MII_TE | TX_FNL | TX_FNH,
	TX_START = 0x8113c,*/
	RXMII = MII_RE,
	TXMII= MII_TE,
};


enum RE8670_STATUS_REGS
{
	/* Tx and Rx status descriptors */
	DescOwn		= (1 << 31), /* Descriptor is owned by NIC */
	RingEnd		= (1 << 30), /* End of descriptor ring */
	FirstFrag	= (1 << 29), /* First segment of a packet */
	LastFrag	= (1 << 28), /* Final segment of a packet */
	TxError		= (1 << 23), /* Tx error summary */
	RxError		= (1 << 20), /* Rx error summary */
	IPCS		= (1 << 18), /* Calculate IP checksum */
	UDPCS		= (1 << 17), /* Calculate UDP/IP checksum */
	TCPCS		= (1 << 16), /* Calculate TCP/IP checksum */
	TxVlanTag	= (1 << 17), /* Add VLAN tag */
	RxVlanTagged	= (1 << 16), /* Rx VLAN tag available */
	IPFail		= (1 << 15), /* IP checksum failed */
	UDPFail		= (1 << 14), /* UDP/IP checksum failed */
	TCPFail		= (1 << 13), /* TCP/IP checksum failed */
	NormalTxPoll	= (1 << 6),  /* One or more normal Tx packets to send */
	PID1		= (1 << 17), /* 2 protocol id bits:  0==non-IP, */
	PID0		= (1 << 16), /* 1==UDP/IP, 2==TCP/IP, 3==IP */
	RxProtoTCP	= 1,
	RxProtoUDP	= 2,
	RxProtoIP	= 3,
	TxFIFOUnder	= (1 << 25), /* Tx FIFO underrun */
	TxOWC		= (1 << 22), /* Tx Out-of-window collision */
	TxLinkFail	= (1 << 21), /* Link failed during Tx of packet */
	TxMaxCol	= (1 << 20), /* Tx aborted due to excessive collisions */
	TxColCntShift	= 16,	     /* Shift, to get 4-bit Tx collision cnt */
	TxColCntMask	= 0x01 | 0x02 | 0x04 | 0x08, /* 4-bit collision count */
	RxErrFrame	= (1 << 27), /* Rx frame alignment error */
	RxMcast		= (1 << 26), /* Rx multicast packet rcv'd */
	RxErrCRC	= (1 << 18), /* Rx CRC error */
	RxErrRunt	= (1 << 19), /* Rx error, packet < 64 bytes */
	RWT		= (1 << 21), /* Rx  */
	E8023		= (1 << 22), /* Receive Ethernet 802.3 packet  */
	TxCRC		= (1 <<23),
	
	RxVlanOn	= (1 << 2),  /* Rx VLAN de-tagging enable */
	RxChkSum	= (1 << 1),  /* Rx checksum offload enable */		

};

enum RE8670_THRESHOLD_REGS{
//	THVAL		= 2,
	TH_ON_VAL = 0x06,	// flow control assert threshold: available desc <= 6
	TH_OFF_VAL= 0x30,	// flow control de-assert threshold : available desc>=48
//	RINGSIZE	= 0x0f,	//2,

	LOOPBACK	= (0x3 << 8),
 	AcceptErr	= 0x20,	     /* Accept packets with CRC errors */
	AcceptRunt	= 0x10,	     /* Accept runt (<64 bytes) packets */
	AcceptBroadcast	= 0x08,	     /* Accept broadcast packets */
	AcceptMulticast	= 0x04,	     /* Accept multicast packets */
	AcceptMyPhys	= 0x02,	     /* Accept pkts with our MAC as dest */
	AcceptAllPhys	= 0x01,	     /* Accept all pkts w/ physical dest */
	AcceptAll = AcceptBroadcast | AcceptMulticast | AcceptMyPhys |  AcceptAllPhys | AcceptErr | AcceptRunt,
	AcceptNoBroad = AcceptMulticast |AcceptMyPhys |  AcceptAllPhys | AcceptErr | AcceptRunt,
	AcceptNoMulti =  AcceptMyPhys |  AcceptAllPhys | AcceptErr | AcceptRunt,
	NoErrAccept = AcceptBroadcast | AcceptMulticast | AcceptMyPhys,
	NoErrPromiscAccept = AcceptBroadcast | AcceptMulticast | AcceptMyPhys |  AcceptAllPhys,
	
};



enum PTM_ISR_REGS{
	TDU_CH3_S		= (1 <<31),
	TDU_CH2_S		= (1 <<30),
	TDU_CH1_S		= (1 <<29),
	TDU_CH0_S		= (1 <<28),
	TDU_CH3_F		= (1 <<27),
	TDU_CH2_F		= (1 <<26),
	TDU_CH1_F		= (1 <<25),
	TDU_CH0_F		= (1 <<24),
	TOK_CH3_S		= (1 <<23),
	TOK_CH2_S		= (1 <<22),
	TOK_CH1_S		= (1 <<21),
	TOK_CH0_S		= (1 <<20),
	TOK_CH3_F		= (1 <<19),
	TOK_CH2_F		= (1 <<18),
	TOK_CH1_F		= (1 <<17),
	TOK_CH0_F		= (1 <<16),
	RER_OVF_H_S	= (1 <<15),
	RER_OVF_L_S	= (1 <<14),
	RER_OVF_H_F	= (1 <<13),
	RER_OVF_L_F	= (1 <<12),
	RDU_H_S		= (1 <<11),
	RDU_L_S		= (1 <<10),
	RDU_H_F		= (1 <<9),
	RDU_L_F		= (1 <<8),
	RUNT_H_S		= (1 <<7),
	RUNT_L_S		= (1 <<6),
	RUNT_H_F		= (1 <<5),
	RUNT_L_F		= (1 <<4),
	ROK_H_S		= (1 << 3),
	ROK_L_S		= (1 << 2),
	ROK_H_F		= (1 << 1),
	ROK_L_F		= (1 << 0),
};

enum RE8670_IOCMD_REG
{
	RX_MIT 		= 7,
	RX_TIMER 	= 1,
	RX_FIFO 	= 2,
	TX_FIFO		= 1,
	TX_MIT		= 7,	
	TX_POLL_CH3S	= 1 << 7,
	TX_POLL_CH2S	= 1 << 6,
	TX_POLL_CH1S	= 1 << 5,
	TX_POLL_CH0S	= 1 << 4,
	TX_POLL_CH3F	= 1 << 3,
	TX_POLL_CH2F	= 1 << 2,
	TX_POLL_CH1F	= 1 << 1,
	TX_POLL_CH0F	= 1 << 0,
	//TX_POLL2	= 1 << 1,
	//TX_POLL		= 1 << 0,
//	CMD_CONFIG = 0x3c | RX_MIT << 8 | RX_FIFO << 11 |  RX_TIMER << 13 | TX_MIT << 16 | TX_FIFO<<19,
#ifdef CONFIG_RTL867X_PACKET_PROCESSOR
	CMD_CONFIG=0x00091030
#else
	//CMD_CONFIG = 0x40081030	       // rxfth = 64 bytes, mitigation=0=> 1pkt trigger int
	CMD_CONFIG = 0x58001000
#endif
};


typedef enum
{
	FLAG_WRITE		= (1<<31),
	FLAG_READ		= (0<<31),
	
	MII_PHY_ADDR_SHIFT	= 26, 
	MII_REG_ADDR_SHIFT	= 16,
	MII_DATA_SHIFT		= 0,
}MIIAR_MASK;

unsigned int re8670_intr_mask =
	ROK_H_S | RUNT_H_S | RDU_H_S | RER_OVF_H_S 
	| ROK_L_S | RUNT_L_S | RDU_L_S | RER_OVF_L_S 
	| ROK_H_F | RUNT_H_F | RDU_H_F | RER_OVF_H_F 
	| ROK_L_F | RUNT_L_F | RDU_L_F | RER_OVF_L_F
	| TDU_CH3_S | TDU_CH2_S | TDU_CH1_S | TDU_CH0_S
	| TDU_CH3_F | TDU_CH2_F | TDU_CH1_F | TDU_CH0_F //;
	| TOK_CH3_S | TOK_CH2_S | TOK_CH1_S | TOK_CH0_S
	| TOK_CH3_F | TOK_CH2_F | TOK_CH1_F | TOK_CH0_F;

unsigned int check_interrupts =
	ROK_H_S | RUNT_H_S | RDU_H_S | RER_OVF_H_S 
	| ROK_L_S | RUNT_L_S | RDU_L_S | RER_OVF_L_S 
	| ROK_H_F | RUNT_H_F | RDU_H_F | RER_OVF_H_F 
	| ROK_L_F | RUNT_L_F | RDU_L_F | RER_OVF_L_F
	| TDU_CH3_S | TDU_CH2_S | TDU_CH1_S | TDU_CH0_S
	| TDU_CH3_F | TDU_CH2_F | TDU_CH1_F | TDU_CH0_F//;
	| TOK_CH3_S | TOK_CH2_S | TOK_CH1_S | TOK_CH0_S
	| TOK_CH3_F | TOK_CH2_F | TOK_CH1_F | TOK_CH0_F;


typedef struct dma_desc {
	u32		opts1;
	u32		addr;
	u32		opts2;
	u32		opts3;

}DMA_DESC;


struct ring_info {
	struct sk_buff		*skb;
	dma_addr_t		mapping;
	unsigned		frag;
};

struct cp_extra_stats {
	unsigned long		rx_frags;
	unsigned long tx_timeouts;
	// JONAH
	unsigned long tx_cnt;
};

struct re_private {

	unsigned		tx_fdpf_head[4];
	unsigned		tx_fdps_head[4];
	unsigned		tx_fdpf_tail[4];
	unsigned		tx_fdps_tail[4];

//#ifdef PTM2
#if defined(PTM2) || defined(PRE_TEST)
	unsigned			rx_tail[4];
	unsigned int		sts_reg;	//for recording PTM int status
#endif
	void			*regs;
	struct net_device	*dev;
	spinlock_t		lock;

//#ifdef PTM2
#if defined(PTM2) || defined(PRE_TEST)
	DMA_DESC		*rx_hsring; // 0
	DMA_DESC		*rx_lsring;  // 1
	DMA_DESC		*rx_hfring; // 2
	DMA_DESC		*rx_lfring; // 3	
	//DMA_DESC		*rx_ring[4];
#endif

//#ifdef PTM2
#if defined(PTM2) || defined(PRE_TEST)
	DMA_DESC		*tx_fdpf_ring[4];
	DMA_DESC		*tx_fdps_ring[4];

	struct ring_info	tx_fdpf_skb[4][RE8670_TX_RING_SIZE];
	struct ring_info	tx_fdps_skb[4][RE8670_TX_RING_SIZE];
#endif

//#ifdef PTM2
#if defined(PTM2) || defined(PRE_TEST)
	struct ring_info	rx_hs_skb[RE8670_RX_RING_SIZE];
	struct ring_info	rx_ls_skb[RE8670_RX_RING_SIZE];
	struct ring_info	rx_hf_skb[RE8670_RX_RING_SIZE];
	struct ring_info	rx_lf_skb[RE8670_RX_RING_SIZE];	
	//ruct ring_info	rx_skb[4][RE8670_RX_RING_SIZE];
#endif
	unsigned		rx_buf_sz;
	dma_addr_t		ring_dma;

	u32			msg_enable;

	struct net_device_stats net_stats;
	struct cp_extra_stats	cp_stats;

	struct pci_dev		*pdev;
	u32			rx_config;

	struct sk_buff		*frag_skb;
	unsigned		dropping_frag : 1;
//#ifdef PTM2
#if defined(PTM2) || defined(PRE_TEST)
	//char*			rxdesc_buf[4];
	char*			rxdesc_buf_hs;
	char*			rxdesc_buf_ls;
	char*			rxdesc_buf_hf;
	char*			rxdesc_buf_lf;

	char				txdesc_buf_f[4];
	char				txdesc_buf_s[4];

#endif
	struct mii_if_info	mii_if;
	struct tasklet_struct rx_tasklets;
	struct tasklet_struct rx_h_tasklets;
#ifdef PTMCTL_LOG
	unsigned int slow_txcnt[4];
	unsigned int fast_txcnt[4];
	unsigned int rxcnt[4];
#endif
};


//for alignment issue
#define READWD(addr) ((unsigned char *)addr)[0]<<24 | ((unsigned char *)addr)[1]<<16 | ((unsigned char *)addr)[2]<<8 | ((unsigned char *)addr)[3]
#define READHWD(addr) ((unsigned char *)addr)[0]<<8 | ((unsigned char *)addr)[1]
//tylo, struct for IGMP snooping

static void __ptm_set_rx_mode (struct net_device *dev);
static void ptm_tx (struct re_private *cp);
static void ptm_clean_rings (struct re_private *cp);
static void ptm_tx_timeout (struct net_device *dev);

extern void enable_lx4180_irq(int irq);
extern void disable_lx4180_irq(int irq);



static unsigned int ptm_rx_count=0;
static unsigned char eth_close=0;

//#define RE8670_MAX_ALLOC_RXSKB_NUM 150
#define RE8670_MAX_ALLOC_RXSKB_NUM 150

#define SKB_BUF_SIZE  1600

static unsigned int ptm_iocmd_reg=CMD_CONFIG;//=0x4009113d;	

//for ethctl
struct eth_arg{
	unsigned char cmd;
	unsigned int cmd2;
	unsigned int cmd3;
	unsigned int cmd4;
#ifdef PTM2
	unsigned int cmd5;
#endif
};
#ifdef PTM2
#define test_len PKT_BUF_SZ
#else
#define test_len 140
#endif
static struct sk_buff testskb2, testskb1;

static unsigned char ptm_testskbdata[test_len], ptm_testskbdata1[test_len];;
static struct timer_list rxtest_timer2;
static unsigned char rxtest_running2=0;
static unsigned char ptm_testdata2[558] = 
{
  0x00, 0x3d, 0x00, 0x2a, 0xff, 0x9f, 0x00, 0x0e, 0x2e, 0xf1, 0x50, 0x2c, 0x08, 0x00, 0x45, 0x00
, 0x02, 0x20, 0x01, 0x70, 0x00, 0x00, 0x80, 0x11, 0xad, 0x5b, 0xc0, 0xa8, 0x04, 0x64, 0xc0, 0xa8
, 0x04, 0x4d, 0x0e, 0xb9, 0x04, 0x2f, 0x02, 0x0c, 0x76, 0x3a, 0x00, 0x03, 0x00, 0x5c, 0xdb, 0x99
, 0xef, 0x88, 0x63, 0x2b, 0xc0, 0xfd, 0x95, 0x25, 0x40, 0x89, 0x47, 0xb2, 0x4e, 0xac, 0x39, 0x48
, 0x36, 0x01, 0xc5, 0x78, 0x3e, 0x0b, 0x24, 0xd9, 0x08, 0x24, 0x46, 0x36, 0x9e, 0x56, 0xba, 0x93
, 0xdb, 0xd4, 0xd4, 0x4d, 0xd5, 0x1c, 0x83, 0xc2, 0xd7, 0xe1, 0xa2, 0xbe, 0x89, 0x6e, 0x35, 0x7e
, 0x56, 0xd7, 0x92, 0x05, 0x6c, 0xa8, 0xd5, 0x2a, 0x97, 0x71, 0x8c, 0x78, 0xdb, 0xa5, 0x3e, 0x78
, 0x92, 0x25, 0x86, 0xf5, 0x7c, 0x21, 0xed, 0x00, 0x4a, 0x6e, 0xc9, 0x74, 0xb8, 0x83, 0xaa, 0xa6
, 0x14, 0xef, 0x5d, 0xe1, 0x22, 0x4a, 0xcd, 0x80, 0x29, 0xef, 0xf4, 0x4b, 0xa8, 0xe4, 0xee, 0x83
, 0x92, 0xfd, 0xaf, 0xaa, 0x01, 0x98, 0x71, 0xc4, 0xb9, 0xfb, 0xe6, 0xf7, 0x79, 0x4e, 0xe5, 0x33
, 0xdd, 0x3f, 0x85, 0x1b, 0xb0, 0x03, 0x34, 0xb2, 0x1f, 0x9e, 0x34, 0xba, 0x31, 0x51, 0x5f, 0xc0
, 0xd4, 0x98, 0x2c, 0xdd, 0xc5, 0x3a, 0x83, 0x36, 0xfa, 0x0f, 0x83, 0x48, 0x51, 0xee, 0x8c, 0x66
, 0x3a, 0x7a, 0xfd, 0xc9, 0xb9, 0xdd, 0x70, 0x2f, 0x7f, 0xec, 0xbd, 0xdb, 0x98, 0x9a, 0x96, 0xd2
, 0xaa, 0x80, 0x0b, 0x91, 0x58, 0x9a, 0xbb, 0x82, 0x1d, 0xd9, 0x47, 0xaa, 0x9f, 0x6c, 0x71, 0x68
, 0x21, 0x29, 0x35, 0xc4, 0x23, 0xdc, 0x7c, 0xd2, 0x3e, 0x29, 0x39, 0x00, 0x7e, 0x4d, 0x2e, 0xba
, 0x1d, 0x5d, 0xd5, 0xdd, 0x1d, 0x89, 0x37, 0xd5, 0x05, 0x53, 0xbd, 0xf5, 0x82, 0x6c, 0x5f, 0x08
, 0x23, 0x43, 0x0c, 0x11, 0xe2, 0x07, 0xcf, 0x45, 0xbe, 0xb0, 0x93, 0x40, 0xc5, 0x83, 0x6e, 0xe8
, 0xf2, 0x6e, 0xcb, 0xd4, 0xe6, 0x43, 0x90, 0xeb, 0xa5, 0xe1, 0x15, 0x65, 0xba, 0x52, 0xcf, 0x94
, 0x33, 0x12, 0xac, 0x66, 0x43, 0x4b, 0xff, 0xec, 0x3e, 0x30, 0xbc, 0x9e, 0x2c, 0xec, 0xf6, 0x53
, 0xdc, 0x4f, 0x38, 0xb0, 0x4b, 0xa1, 0x88, 0x54, 0xe4, 0xb4, 0x15, 0x58, 0x98, 0x87, 0xf0, 0xdd
, 0x68, 0x41, 0xcb, 0x75, 0x33, 0x49, 0x7e, 0x7b, 0x73, 0xce, 0xf3, 0x41, 0xea, 0x4e, 0xea, 0x99
, 0x1f, 0x82, 0x31, 0x8b, 0x31, 0xdf, 0xa7, 0x22, 0xac, 0x44, 0x31, 0xa1, 0xea, 0x65, 0xad, 0xb0
, 0xc8, 0x72, 0x91, 0xdf, 0x1e, 0x11, 0x06, 0xb5, 0x65, 0x38, 0x30, 0x50, 0x35, 0x0d, 0x3e, 0x39
, 0x18, 0x1b, 0x6c, 0x31, 0x40, 0x2c, 0xc3, 0x7d, 0x92, 0x88, 0xf0, 0x37, 0x29, 0xce, 0x97, 0xfa
, 0x5c, 0x13, 0x50, 0x3c, 0xc6, 0xb2, 0x47, 0x7a, 0xeb, 0xa8, 0x6e, 0xf1, 0x54, 0xd1, 0xf5, 0xac
, 0xc3, 0x0f, 0xc6, 0x42, 0x8d, 0xa6, 0xbb, 0xa2, 0x66, 0xba, 0x08, 0x72, 0x7c, 0xb6, 0x04, 0x67
, 0xfc, 0xdd, 0xb1, 0x62, 0x1d, 0xe2, 0xf4, 0xa6, 0x91, 0x80, 0x95, 0x9c, 0x52, 0x1a, 0x77, 0xf9
, 0x8a, 0x9f, 0x29, 0x20, 0xc3, 0xda, 0xb7, 0x95, 0x49, 0x33, 0xf5, 0x56, 0x92, 0x77, 0x29, 0x26
, 0x02, 0x8a, 0x60, 0x80, 0x11, 0x3a, 0x02, 0x01, 0xba, 0x48, 0x32, 0x5f, 0xb4, 0x3f, 0x60, 0x1f
, 0x67, 0xa7, 0xbe, 0x5b, 0xde, 0x11, 0xd7, 0xd3, 0x10, 0x74, 0xea, 0x49, 0x7f, 0xa8, 0x92, 0xb0
, 0x46, 0x55, 0x6b, 0xb0, 0xbf, 0x51, 0x0f, 0xba, 0x7d, 0x36, 0x48, 0x85, 0x6c, 0x18, 0xec, 0x40
, 0x11, 0x2e, 0xb1, 0x25, 0x05, 0x78, 0x71, 0x43, 0x05, 0x3f, 0x9a, 0xc9, 0x13, 0x5e, 0x98, 0xff
, 0x65, 0xcd, 0x87, 0x25, 0x96, 0x27, 0x05, 0xb7, 0xe8, 0x2b, 0xb9, 0xf9, 0x00, 0xf5, 0x15, 0x0d
, 0xae, 0xee, 0xda, 0x20, 0x4b, 0xf2, 0x0f, 0x20, 0xb9, 0x25, 0xc6, 0xd9, 0xed, 0x32, 0xc8, 0xa6
, 0x7e, 0x4c, 0xb1, 0x58, 0xbe, 0x8c, 0x84, 0x08, 0xe8, 0x11, 0x39, 0x46, 0xda, 0x7e 
};

unsigned int ptm_debug=0;

void Dumpreg(unsigned long Buffer, unsigned int size){

	int	k;	
	if(size%4)	size=size+4-(size%4);

	
	if ((Buffer&0xF0000000)==0x80000000) Buffer |= 0xA0000000;
	printk("Address  : Data");
	for(k=0;k<(size/4);k++){
		if ((k%4)==0) {
			printk ("\n");
			printk("%08X : ",Buffer+k*4);
		}
		//mdelay(100);
		printk("%08X  ", (*(volatile unsigned int *)((unsigned int)((Buffer)+k*4))));
	}
	printk("\n");


}

void memDump (void *start, u32 size, char * strHeader)
{
	int row, column, index, index2, max;
//	uint32 buffer[5];
	u8 *buf, *line, ascii[17];
	char empty = ' ';

	//return; //czyao
	if (!ptm_debug)
		return;

	if(!start ||(size==0))
		return;
	line = (u8*)start;

	/*
	16 bytes per line
	*/
	if (strHeader)
		printk("%s", strHeader);
	column = size % 16;
	row = (size / 16) + 1;
	for (index = 0; index < row; index++, line += 16) 
	{
		buf = line;

		memset (ascii, 0, 17);

		max = (index == row - 1) ? column : 16;
		if ( max==0 ) break; /* If we need not dump this line, break it. */

		printk("\n%08x ", (u32) line);
		
		//Hex
		for (index2 = 0; index2 < max; index2++)
		{
			if (index2 == 8)
			printk("  ");
			printk("%02x ", (u8) buf[index2]);
			ascii[index2] = ((u8) buf[index2] < 32) ? empty : buf[index2];
		}

		if (max != 16)
		{
			if (max < 8)
				printk("  ");
			for (index2 = 16 - max; index2 > 0; index2--)
				printk("   ");
		}

		//ASCII
		printk("  %s", ascii);
	}
	printk("\n");
	return;
}

struct sk_buff *ptm_getAlloc(void)  //czyao for 0437FPGA
{	
struct sk_buff *skb=NULL;
//int ptm_rxskb_num=0;	
//printk("skb=%x cb[0]=%d\n",(unsigned int)skb , skb->cb[0] );
//    if ( ptm_rxskb_num < RE8670_MAX_ALLOC_RXSKB_NUM ) {
	//printk("free pre-alloc skb - unknow state...\n");
	skb = dev_alloc_skb(SKB_BUF_SIZE);
/*
        if (skb!=NULL) {
            ptm_rxskb_num++;
            skb->src_port=IF_ETH;
        };
*/
//    }
    if(skb==NULL)
		printk("Cannot allocate skb, not enough memory\n");
    	//printk("Cannot allocate skb, ptm_rxskb_num %d\n", ptm_rxskb_num);
    return skb;
}

static inline void ptm_set_rxbufsize (struct re_private *cp)
{
	unsigned int mtu = cp->dev->mtu;
	
	if (mtu > ETH_DATA_LEN)
		/* MTU + ethernet header + FCS + optional VLAN tag */
		cp->rx_buf_sz = mtu + ETH_HLEN + 8;
	else
		cp->rx_buf_sz = PKT_BUF_SZ;
}

#if !defined(PTM2)
static atomic_t lock_tx_tail2 = ATOMIC_INIT(0);
#endif
static inline void ptm_rx_skb (struct re_private *cp, struct sk_buff *skb,
			      DMA_DESC *desc)
{
	skb->dev = ptm_net_dev;


	cp->net_stats.rx_bytes += skb->len;
	skb->protocol = eth_type_trans (skb, cp->dev);

	cp->net_stats.rx_packets++;
	cp->dev->last_rx = jiffies;

	//printk("ptm_rx_skb: skb->switch_port = %s\n", skb->switch_port);
	//memDump(skb->data, skb->len, "loopback pkt"); //
	netif_rx(skb);
//	dev_kfree_skb(skb); //directly free it - shlee

}

static void ptm_rx_err_acct (struct re_private *cp, unsigned rx_tail,
			    u32 status, u32 len)
{
	if (netif_msg_rx_err (cp))
		printk (KERN_DEBUG
			"%s: rx err, slot %d status 0x%x len %d\n",
			cp->dev->name, rx_tail, status, len);
	cp->net_stats.rx_errors++;
	if (status & RxErrFrame)
		cp->net_stats.rx_frame_errors++;
	if (status & RxErrCRC)
		cp->net_stats.rx_crc_errors++;
	if (status & RxErrRunt)
		cp->net_stats.rx_length_errors++;
}


int TX_BUFFS_AVAIL(struct re_private *cp, int channel, int fast)
{
	int ret;
	if (fast)
	{
		if (cp->tx_fdpf_tail[channel] <= cp->tx_fdpf_head[channel])
		{
			ret = (cp->tx_fdpf_tail[channel]+(RE8670_TX_RING_SIZE-1) - cp->tx_fdpf_head[channel]);
			//printk("[%s, %d] ret = %d, tx_fdpf_tail = %d, tx_fdpf_head = %d \n",
			//	__FUNCTION__, __LINE__,ret,cp->tx_fdpf_tail[channel], cp->tx_fdpf_head[channel]);
		}
		else
		{
			ret = cp->tx_fdpf_tail[channel] - cp->tx_fdpf_head[channel] - 1;
			//printk("[%s, %d] ret = %d, tx_fdpf_tail = %d, tx_fdpf_head = %d \n",
			//	__FUNCTION__, __LINE__,ret,cp->tx_fdpf_tail[channel], cp->tx_fdpf_head[channel]);
		}
	}
	else
	{
		if (cp->tx_fdps_tail[channel] <= cp->tx_fdps_head[channel])
		{
			ret = (cp->tx_fdps_tail[channel]+(RE8670_TX_RING_SIZE-1) - cp->tx_fdps_head[channel]);
			//printk("[%s, %d] ret = %d, tx_fdpf_tail = %d, tx_fdpf_head = %d \n",
			//	__FUNCTION__, __LINE__,ret,cp->tx_fdps_tail[channel], cp->tx_fdps_head[channel]);
		}
		else
		{
			ret = cp->tx_fdps_tail[channel] - cp->tx_fdps_head[channel] - 1;
			//printk("[%s, %d] ret = %d, tx_fdpf_tail = %d, tx_fdpf_head = %d \n",
			//	__FUNCTION__, __LINE__,ret,cp->tx_fdps_tail[channel], cp->tx_fdps_head[channel]);
		}
	}
	return ret;
}

static inline unsigned int ptm_rx_csum_ok (u32 status)
{
	unsigned int protocol = (status >> 16) & 0x3;
	
	if (likely((protocol == RxProtoTCP) && (!(status & TCPFail))))
		return 1;
	else if ((protocol == RxProtoUDP) && (!(status & UDPFail)))
		return 1;
	else if ((protocol == RxProtoIP) && (!(status & IPFail)))
		return 1;
	return 0;
}

unsigned int length=60;
void tx_loopback(struct net_device *dev, unsigned int data)
{
	static struct sk_buff *pskb;
	unsigned int loopcnt;
	//length = data;

	pskb = dev_alloc_skb(length);

	for(loopcnt=0;loopcnt<length;loopcnt++)
		pskb->data[loopcnt]=0;
	pskb->len=length;
	//pskb->dev = dev;
	//da
	pskb->data[0]=0xff;
	pskb->data[1]=0xff;
	pskb->data[2]=0xff;
	pskb->data[3]=0xff;
	pskb->data[4]=0xff;
	pskb->data[5]=0xff;
	//sa
	pskb->data[6]=0x00;
	pskb->data[7]=0xe0;
	pskb->data[8]=0x4c;
	pskb->data[9]=0x11;
	pskb->data[10]=0x22;
	pskb->data[11]=0x55;
	//EtherType
	pskb->data[12]=0x08;
	pskb->data[13]=0x06;

	pskb->data[14]=0x00;
	pskb->data[15]=0x01;
	pskb->data[16]=0x08;
	pskb->data[17]=0x00;
	pskb->data[18]=0x06;
	pskb->data[19]=0x04;
	pskb->data[20]=0x00;
	pskb->data[21]=0x01;
	pskb->data[22]=0x6c;
	pskb->data[23]=0xf0;
	pskb->data[24]=0x49;
	pskb->data[25]=0x1f;
	pskb->data[26]=0xc3;
	pskb->data[27]=0x15;
	pskb->data[28]=0xac;
	pskb->data[29]=0x15;
	pskb->data[30]=0x94;
	pskb->data[31]=0x73;
	pskb->data[32]=0x00;
	pskb->data[33]=0x00;
	pskb->data[34]=0x00;
	pskb->data[35]=0x00;
	pskb->data[36]=0x00;
	pskb->data[37]=0x00;
	pskb->data[38]=0xac;
	pskb->data[39]=0x15;
	pskb->data[40]=0x94;
	pskb->data[41]=0x05;
	pskb->data[42]=0x00;
	pskb->data[43]=0x00;
	pskb->data[44]=0x00;
	pskb->data[45]=0x00;
	pskb->data[46]=0x00;
	pskb->data[47]=0x00;
	pskb->data[48]=0x00;
	pskb->data[49]=0x00;
	pskb->data[50]=0x00;
	pskb->data[51]=0x00;
	pskb->data[52]=0x00;
	pskb->data[53]=0x00;
	pskb->data[54]=0x00;
	pskb->data[55]=0x00;
	pskb->data[56]=0x00;
	pskb->data[57]=0x00;
	pskb->data[58]=0x00;
	pskb->data[59]=0x00;
	pskb->data[60]=0x00;
	pskb->data[61]=0x00;
	pskb->data[62]=0x00;
	pskb->data[63]=0x00;

	pskb->tail=pskb->len+pskb->data;

	ptm_start_xmit(pskb, dev);
	
}

static void __ptm_rx(struct re_private *cp, int queue)
{
	unsigned rx_tail = cp->rx_tail[queue];
	u32 status, len;
	dma_addr_t mapping;
	struct sk_buff *skb, *new_skb;
	DMA_DESC *desc;
	unsigned rx_work = 100;

	while (rx_work--)
	{
		//skb = cp->rx_skb[queue][rx_tail].skb;
		if (queue==0) 
		{
			skb = cp->rx_hs_skb[rx_tail].skb;
		}
		else if (queue==1)
		{
			skb = cp->rx_ls_skb[rx_tail].skb;
		}		
		else if (queue==2)
		{
			skb = cp->rx_hf_skb[rx_tail].skb;
		}
		else
		{
			skb = cp->rx_lf_skb[rx_tail].skb;
		}
		if (!skb)
			BUG();
		//else
		//	memDump(skb->data, 16, "skb"); 

		//desc = &cp->rx_ring[rx_tail];
		//desc = &cp->rx_ring[queue][rx_tail];
		switch (queue)
		{
			case 0:
				desc = &cp->rx_hsring [rx_tail];
				//printk("cp->rx_hsring[0]=%x", cp->rx_hsring[rx_tail]);
				break;
			case 1:
				desc = &cp->rx_lsring [rx_tail];
				//printk("cp->rx_lsring[1]=%x", cp->rx_lsring[rx_tail]);
				break;
			case 2:
				desc = &cp->rx_hfring [rx_tail];
				//printk("cp->rx_hfring[2]=%x", cp->rx_hfring[rx_tail]);
				break;
			case 3:
				desc = &cp->rx_lfring [rx_tail];
				//printk("cp->rx_lfring[3]=%x", cp->rx_lfring[rx_tail]);
				break;
		}
		status = desc->opts1;
		
		if (status & DescOwn)
		{
			break;
		}

//if(disable_rx_message ==0 )
//		printk("NIC Rx(lq): status %08x, addr %08x opts2 %08x opts3 %08x\n", status, desc->addr, desc->opts2, desc->opts3);	

		//shlee for testing short pkt    len = (status & 0x07ff) - 4;
		len = (status & 0x07ff) ;	//shlee for testing short pkt

		//mapping = cp->rx_skb[rx_tail].mapping;

		if ((status & (FirstFrag | LastFrag)) != (FirstFrag | LastFrag)) {
			//tylo
			printk("PTM frag (status %x)\n", status);
			ptm_rx_err_acct(cp, rx_tail, status, len);
			//shlee let it thru for dumping payload     
			goto rx_next;
		}

		if (status & (RxErrCRC)) { //czyao adds
			printk("PTM CRC error (status %x)\n", status);
			ptm_rx_err_acct(cp, rx_tail, status, len);
			goto rx_next;
		}

		if (status & (RxErrRunt)) { //czyao adds
			printk("PTM runt (status %x)\n", status);
			ptm_rx_err_acct(cp, rx_tail, status, len);
			goto rx_next;
		}

		if (status & (RxError)) {
			printk("PTM rxError (status %x)\n", status);
			ptm_rx_err_acct(cp, rx_tail, status, len);
			//shlee let it thru for dumping payload     
			goto rx_next;
		}

		
#ifdef PTM2
		len = len - _CRCLNG_;
#endif

if(disable_rx_message ==0 )  //czyao adds new debug message
		printk("PTM Rx(lq): status %08x, addr %08x opts2 %08x opts3 %08x\n", status, desc->addr, desc->opts2, desc->opts3); 

		new_skb = ptm_getAlloc();
		if (!new_skb) {
                        //In 100M utilization smartbit stress injection test, avoid nic stop receiving pkt, give it a second chance
                      printk("##########   %s: CANNOT ALLOCATE SKB!!!!       ###########\n",__FUNCTION__);
			tasklet_hi_schedule(&cp->rx_tasklets);	
			break;
			//goto rx_next;
		}
		/* Handle checksum offloading for incoming packets. */
		/*
		if (ptm_rx_csum_ok(status))
			skb->ip_summed = CHECKSUM_UNNECESSARY;
		else
			skb->ip_summed = CHECKSUM_NONE;
		*/
		skb_reserve(skb, RX_OFFSET);
		dma_cache_wback_inv((unsigned long)skb->data, len);
		skb_put(skb, len);

		switch (queue)
		{
			case 0:
				mapping =
				cp->rx_hs_skb[rx_tail].mapping = (u32)new_skb->tail | UNCACHE_MASK;
				break;
			case 1:
				mapping =
				cp->rx_ls_skb[rx_tail].mapping = (u32)new_skb->tail | UNCACHE_MASK;
				break;
			case 2:
				mapping =
				cp->rx_hf_skb[rx_tail].mapping = (u32)new_skb->tail | UNCACHE_MASK;
				break;
			case 3:
				mapping =
				cp->rx_lf_skb[rx_tail].mapping = (u32)new_skb->tail | UNCACHE_MASK;
				break;
		}
		//mapping =
		//cp->rx_skb[queue][rx_tail].mapping = (u32)new_skb->tail | UNCACHE_MASK;
		
		ptm_rx_count++;

#if 0		
	if(!disable_rx_message){		
		printk("PTM Rx(lq): RxCDO %x sw_tail %x, len = %d", RTL_R16(RxCDO), rx_tail, len);
		int i;
		for (i=0; i<len ; i++){
			if(i%16==0)	printk("\n");
			printk("0x%02x", *(skb->data+i));
			if(i!=len-1)	printk(", ");			
		}
		
		printk("\n");
	}
#endif


//		RTL_R8(EthrntRxCPU_Des_Num)=rx_tail;	//flowctrl mechanism: driver need to maintain this pointer	
		//printk("opts1=%x\n", desc->opts1);
		//printk("addr=%x\n", desc->addr);
		//printk("opts2=%x\n", desc->opts2);
		//printk("opts3=%x\n", desc->opts3);
		memDump(skb->data, len, "loopback pkt");
		ptm_rx_skb(cp, skb, desc);
#ifdef PTMCTL_LOG
		cp->rxcnt[queue]++;
#endif
		
		//cp->rx_skb[queue][rx_tail].skb = new_skb;
		//cp->rx_ring[queue][rx_tail].addr = mapping& 0x1fffffff;
		switch (queue)
		{
			case 0:
				cp->rx_hs_skb[rx_tail].skb = new_skb;
				cp->rx_hsring[rx_tail].addr = mapping& 0x1fffffff;
				break;
			case 1:
				cp->rx_ls_skb[rx_tail].skb = new_skb;
				cp->rx_lsring[rx_tail].addr = mapping& 0x1fffffff;
				break;
			case 2:
				cp->rx_hf_skb[rx_tail].skb = new_skb;
				cp->rx_hfring[rx_tail].addr = mapping& 0x1fffffff;
				break;
			case 3:
				cp->rx_lf_skb[rx_tail].skb = new_skb;
				cp->rx_lfring[rx_tail].addr = mapping& 0x1fffffff;
				break;
		}
rx_next: //czyao opens the rx_next stats
		//cp->rx_ring[rx_tail].opts2 = 0;
		desc->opts1 = (DescOwn | cp->rx_buf_sz) | 
		    ((rx_tail == (RE8670_RX_RING_SIZE - 1))?RingEnd:0);

		rx_tail = NEXT_RX(rx_tail);
	}
	cp->rx_tail[queue] = rx_tail;
}


int cnt=1;
static void ptm_rx (struct re_private *cp)
{

	//protect eth rx while reboot
	if(eth_close == 1)
		return;

	if(cp->sts_reg & ( ROK_H_S | RUNT_H_S | RER_OVF_H_S/* | TX_ERR*/))
	{
		__ptm_rx(cp, 0);
	}
	if(cp->sts_reg & ( ROK_L_S | RUNT_L_S | RER_OVF_L_S/* | TX_ERR*/))
	{
		__ptm_rx(cp, 1);
	}
	if(cp->sts_reg & ( ROK_H_F | RUNT_H_F | RER_OVF_H_F/* | TX_ERR*/))
	{
		__ptm_rx(cp, 2);
	}
	if(cp->sts_reg & ( ROK_L_F | RUNT_L_F | RER_OVF_L_F/* | TX_ERR*/))
	{
		__ptm_rx(cp, 3);
	}

#ifdef CONFIG_RTL8672	

        /*
         * Clear RDU interrupt status first then re-enable RDU interrupt on NIC 
         */
	if((RTL_R32(IMR0)&RDU_H_S)==0){
		RTL_R32(ISR0)|=(u32)(RDU_H_S);	//clear RDU status
		//RTL_W32(ISR0, RTL_R32(ISR0)|(u32)(RDU_H_S));
		RTL_R32(IMR0)|=(u32)(RDU_H_S);	//re-enable RDU int
		//RTL_W32(IMR0, RTL_R32(IMR0)|(u32)(RDU_H_S));
	}
	if((RTL_R32(IMR0)&RDU_L_S)==0){
		RTL_R32(ISR0)|=(u32)(RDU_L_S);	//clear RDU status
		//RTL_W32(ISR0, RTL_R32(ISR0)|(u32)(RDU_L_S));
		RTL_R32(IMR0)|=(u32)(RDU_L_S);	//re-enable RDU int
		//RTL_W32(IMR0, RTL_R32(IMR0)|(u32)(RDU_L_S));
	}
	if((RTL_R32(IMR0)&RDU_H_F)==0){
		RTL_R32(ISR0)|=(u32)(RDU_H_F);	//clear RDU status
		//RTL_W32(ISR0, RTL_R32(ISR0)|(u32)(RDU_H_F));
		RTL_R32(IMR0)|=(u32)(RDU_H_F);	//re-enable RDU int
		//RTL_W32(IMR0, RTL_R32(IMR0)|(u32)(RDU_H_F));
	}
	if((RTL_R32(IMR0)&RDU_L_F)==0){
		RTL_R32(ISR0)|=(u32)(RDU_L_F);	//clear RDU status
		//RTL_W32(ISR0, RTL_R32(ISR0)|(u32)(RDU_L_F));
		RTL_R32(IMR0)|=(u32)(RDU_L_F);	//re-enable RDU int
		//RTL_W32(IMR0, RTL_R32(IMR0)|(u32)(RDU_L_F));
	}

#endif

	if (cnt==1)
	{
		//tx_loopback(ptm_net_dev, 2);
		cnt=2;
	}

}  

#ifdef CONFIG_RTL8672
static irqreturn_t ptm_interrupt(int irq, void * dev_instance, struct pt_regs *regs)
#else
static void ptm_interrupt (int irq, void *dev_instance, struct pt_regs *regs)
#endif
{
	struct net_device *dev = dev_instance;
	struct re_private *cp = dev->priv;

	u32 status;
	if (!(status = RTL_R32(ISR0)))  
#ifdef CONFIG_RTL8672
	{
		printk("%s: no status indicated in interrupt, weird!\n", __func__);	
		return IRQ_RETVAL(IRQ_NONE);
	}
#else
		return;
#endif

	//if ((status & TDU_CH0_S)|(status & TDU_CH0_F))
	if (status & (TDU_CH3_S | TDU_CH2_S | TDU_CH1_S | TDU_CH0_S | TDU_CH3_F | TDU_CH2_F | TDU_CH1_F | TDU_CH0_F))
	{
		if(ptm_debug)
			printk("%s: tx interrupt status 0x%08X\n", __func__,status);	//debug
#if !defined(PTM2)
		if (!atomic_read(&lock_tx_tail2))
#endif
		{
			ptm_tx(cp);   //czyao turn it off!
		}
	}

	if (status & (check_interrupts))
	{
		if(ptm_debug)
			printk("%s: rx interrupt status 0x%08X\n", __func__,status);	//debug

               /*
                 * Check if RDU happens?
                 * RDU(RX_EMPTY) will cause infinited loop (system hang) in Linux 2.6 kernel IRQ handler
                 * IRQ dispatch->NIC int handler->timer handler->NIC->timer ...
                 * NIC bottom half handler will never be scheduled because RDU state is always up.
                 *
                 * Disable RDU in NIC interrupt and then enable it in NIC bottom half handler later.
                 */
		if(status & RDU_H_S)
			RTL_R32(IMR0)&=(u32)(~RDU_H_S);
		if(status & RDU_L_S)
			RTL_R32(IMR0)&=(u32)(~RDU_L_S);
		if(status & RDU_H_F)
			RTL_R32(IMR0)&=(u32)(~RDU_H_F);
		if(status & RDU_L_F)
			RTL_R32(IMR0)&=(u32)(~RDU_L_F);

		tasklet_hi_schedule(&cp->rx_tasklets);
		
		//7/5/05' hrchen, clear NIC ISR of 8671 will not clear RDU if no rx desc available.
		//The IRQ will never stop and RX_BH will never run and free rx desc.
		//Clear NIC GIMR here if RDU. nic_rx must reopen NIC_IM.
		//if (status&RX_EMPTY) {  //RDU happened, clear NIC_IM
		//};
	}

	ptm_tx(cp);  //czyao turns off
	RTL_W32(ISR0,status);

	cp->sts_reg = status;	

#ifdef CONFIG_RTL8672
	return IRQ_RETVAL(IRQ_HANDLED);
#endif
	//spin_unlock(&cp->lock);
}


static void ptm_tx (struct re_private *cp)
{

	unsigned tx_tail, channel;
	u32 status;
	struct sk_buff *skb;
	DMA_DESC  *txd;

	//-----------------------fast channel ----------------------------//
	for (channel=0; channel<4; channel++)
	{
		tx_tail = cp->tx_fdpf_tail[channel];
		//printk("[%s, line %d] channel = %d, tx_tail = %d\n",__FUNCTION__,__LINE__,channel, tx_tail);

		txd = &(cp->tx_fdpf_ring[channel][tx_tail]);

		while(!((status  = txd->opts1)& DescOwn))
		{
			//if(ptm_debug)
			//	printk("[%s, line %d] status = 0x%08X\n",__FUNCTION__,__LINE__,status);
		
			skb = cp->tx_fdpf_skb[channel][tx_tail].skb;
			if (!skb){
			    //printk("[%s, line %d] channel = %d, tx_tail = %d\n",__FUNCTION__,__LINE__,channel, tx_tail);	
			    break;
			}
	
		
		rmb();
			cp->net_stats.tx_packets++;
			cp->net_stats.tx_bytes += skb->len;
#ifdef PTMCTL_LOG
			cp->fast_txcnt[channel]++;
#endif
			
			if(skb->destructor==0)
			{
			    dev_kfree_skb(skb);
			}
			else
			    dev_kfree_skb_irq(skb);

			cp->tx_fdpf_skb[channel][tx_tail].skb = NULL;
			tx_tail = NEXT_TX(tx_tail);
		}
		cp->tx_fdpf_tail[channel] = tx_tail;
	}

	//-----------------------slow channel ----------------------------//
	for (channel=0; channel<4; channel++)
	{
		tx_tail = cp->tx_fdps_tail[channel];
		txd = &(cp->tx_fdps_ring[channel][tx_tail]);

		while(!((status=txd->opts1)& DescOwn))
		{
			//if(ptm_debug)
			//	printk("[%s, line %d] status = 0x%08X\n",__FUNCTION__,__LINE__,status);

			skb = cp->tx_fdps_skb[channel][tx_tail].skb;
			if (!skb)
			    break;
				//BUG();
		
			rmb();
			cp->net_stats.tx_packets++;
			cp->net_stats.tx_bytes += skb->len;
#ifdef PTMCTL_LOG
			cp->slow_txcnt[channel]++;
#endif
			
			if(skb->destructor==0)
			{
			    dev_kfree_skb(skb);
			}
			else
			    dev_kfree_skb_irq(skb);

			cp->tx_fdps_skb[channel][tx_tail].skb = NULL;
			tx_tail = NEXT_TX(tx_tail);
		}	
		cp->tx_fdps_tail[channel] = tx_tail;   //czyao 8672c ptm
	}

	if (netif_queue_stopped(cp->dev) && (TX_BUFFS_AVAIL(cp, 0, 1) > (MAX_SKB_FRAGS + 1)))
		netif_wake_queue(cp->dev);

}

static void ptm_tx_ch (struct re_private *cp, short fast, char channel )
{

	unsigned tx_tail;
	u32 status;
	struct sk_buff *skb;
	DMA_DESC  *txd;

	if(fast){
		tx_tail = cp->tx_fdpf_tail[channel];
		txd = &(cp->tx_fdpf_ring[channel][tx_tail]);

		while (!((status = (txd->opts1))& DescOwn))
		{
			skb = cp->tx_fdpf_skb[channel][tx_tail].skb;
			if (!skb){
			   	break;
			}

			/*if(ptm_debug){
				printk("[%s, line %d] channel = %d, tx_tail = %d, status = 0x%08X\n"
				,__FUNCTION__,__LINE__,channel, tx_tail,status);
			}*/
	
			rmb();
			cp->net_stats.tx_packets++;
			cp->net_stats.tx_bytes += skb->len;
#ifdef PTMCTL_LOG
			cp->fast_txcnt[channel]++;
#endif
			
			if(skb->destructor==0)
			{
			    dev_kfree_skb(skb);
			}
			else
			    dev_kfree_skb_irq(skb);

			cp->tx_fdpf_skb[channel][tx_tail].skb = NULL;
			tx_tail = NEXT_TX(tx_tail);
		}
		cp->tx_fdpf_tail[channel] = tx_tail;
		
	}else{
		tx_tail = cp->tx_fdps_tail[channel];
		txd = &(cp->tx_fdps_ring[channel][tx_tail]);

		while (!((status = (cp->tx_fdps_ring[channel][tx_tail].opts1))& DescOwn))
		{

			skb = cp->tx_fdps_skb[channel][tx_tail].skb;
			if (!skb){
			    break;
			}
				//BUG();

			/*if(ptm_debug){
				printk("[%s, line %d] channel = %d, tx_tail = %d, status = 0x%08X\n"
				,__FUNCTION__,__LINE__,channel, tx_tail,status);
			}*/
				
			rmb();
			cp->net_stats.tx_packets++;
			cp->net_stats.tx_bytes += skb->len;
#ifdef PTMCTL_LOG
			cp->slow_txcnt[channel]++;
#endif
			
			if(skb->destructor==0)
			{
			    dev_kfree_skb(skb);
			}
			else
			    dev_kfree_skb_irq(skb);

			cp->tx_fdps_skb[channel][tx_tail].skb = NULL;
			tx_tail = NEXT_TX(tx_tail);
		}	
		cp->tx_fdps_tail[channel] = tx_tail;   //czyao 8672c ptm
	}
	

	if (netif_queue_stopped(cp->dev) && (TX_BUFFS_AVAIL(cp, channel, fast) > (MAX_SKB_FRAGS + 1)))
		netif_wake_queue(cp->dev);

}


#ifdef ENABLE_LOOPBACK
int loopback_en=0;
#endif
short ptm_tx_fast=0, ptm_tx_channel=0;
int __ptm_start_xmit(struct sk_buff *skb, struct net_device *dev, short fast, char channel)
{
	struct re_private *cp = dev->priv;
	unsigned entry;
	u32 eor,mapping, len, io_cmd=0;//, tx_pkt=0;
	DMA_DESC  *txd;
	int i,countd;	
	unsigned long flags;
	
	if(eth_close == 1 || fast>1 || channel >3 || channel<0 )  //czyao
		return 0;

	// JONAH
	cp->cp_stats.tx_cnt++;
	ptm_tx_ch(cp,fast,channel); //clear tx buffer for freeing allocated skb - shlee
	
	spin_lock_irqsave(&cp->lock, flags);

#if 1
	/* This is a hard error, log it. */
	if (TX_BUFFS_AVAIL(cp, 0, 1) <= (skb_shinfo(skb)->nr_frags + 1))
	{
		netif_stop_queue(dev);
		spin_unlock_irqrestore(&cp->lock, flags);
//		printk(KERN_ERR PFX "%s: BUG! Tx Ring full when queue awake!\n", dev->name);
		printk("%s: BUG! Tx Ring full when queue awake!\n", __func__);	
		ptm_tx_ch(cp,fast,channel);			
		return 1;
	}
#endif

	if ( (skb->len>1518) ) // with vlan tag
	{
		printk("error tx len = %d \n",skb->len);
		spin_unlock_irqrestore(&cp->lock, flags);
		dev_kfree_skb(skb);
		return 0;
	}

	if (fast)
		entry = cp->tx_fdpf_head[channel];
	else
		entry = cp->tx_fdps_head[channel];
			
	eor = (entry == (RE8670_TX_RING_SIZE - 1)) ? RingEnd : 0;
	if (skb_shinfo(skb)->nr_frags == 0) 
	{
		if (fast)
			txd = &cp->tx_fdpf_ring[channel][entry];
		else
			txd = &cp->tx_fdps_ring[channel][entry];

		//txd = &cp->tx_fdp0f_ring[entry];
		len = ETH_ZLEN < skb->len ? skb->len : ETH_ZLEN;
		//eor = (entry == (RE8670_TX_RING_SIZE - 1)) ? RingEnd : 0;

/*
		if(!disable_rx_message ){
			countd=0;
			printk("Tx: packet tail = %d ",len);
				for (i=len-32; i<len ; i++){
					if(countd%16==0)	printk("\n");
					printk("0x%02x", *(skb->data+i));
					if(i!=len-1)	printk(", ");
					countd++;
			}
			printk("\n");
		}
*/


        if(!disable_rx_message){
			printk("PTM Tx: Desc = %d, len = %d\n",entry,len);
		}

		//printk("[%s, line %d] channel = %d, entry = %d\n",__FUNCTION__,__LINE__,channel,entry);
		mapping = (u32)skb->data|UNCACHE_MASK;
		dma_cache_wback_inv((unsigned long)skb->data, skb->len);
		
		if (fast)
		{
			cp->tx_fdpf_skb[channel][entry].skb = skb;
			cp->tx_fdpf_skb[channel][entry].mapping = mapping;
			cp->tx_fdpf_skb[channel][entry].frag = 0;
		}
		else
		{
			cp->tx_fdps_skb[channel][entry].skb = skb;
			cp->tx_fdps_skb[channel][entry].mapping = mapping;
			cp->tx_fdps_skb[channel][entry].frag = 0;
		}

		//printk("[%s, line %d]  ADDR(txd->addr) = 0x%08X, Val = 0x%08X\n",__FUNCTION__,__LINE__, &txd->addr,txd->addr);
		txd->addr = (mapping & 0x1fffffff);
		
		txd->opts1 = (eor | skb->len | DescOwn | FirstFrag | LastFrag | TxCRC);
		
		dma_cache_wback_inv((unsigned long)txd, 16);
		memDump(&txd->opts1, 32, "opts1");
		wmb();
		
		entry = NEXT_TX(entry);
	} 

	if (fast)
		cp->tx_fdpf_head[channel] = entry;	
	else
		cp->tx_fdps_head[channel] = entry;	


	if (netif_msg_tx_queued(cp))
		printk("%s: tx queued, slot %d, skblen %d\n",
		       dev->name, entry, skb->len);
/*		       
	if (TX_HQBUFFS_AVAIL(cp) <= (MAX_SKB_FRAGS + 1))
		netif_stop_queue(dev);
*/
	spin_unlock_irqrestore(&cp->lock, flags);
	//----//dma_cache_wback_inv((unsigned long)skb->data, skb->len);
//	tx_pkt++;
//   }	//while(tx_pkt < 5)
    	memDump(skb->data, skb->len, "tx pkt");
	io_cmd = 1<<channel;

	if (!fast){ 
		io_cmd = io_cmd<<4;
	}
	
	RTL_W32(IO_CMD, RTL_R32(IO_CMD)| io_cmd);// | TX_POLL_CH1S | TX_POLL_CH2S | TX_POLL_CH3S);
#ifdef ENABLE_LOOPBACK
	if (loopback_en==0)
	{
		//*(volatile u32*)(0xb8a85810) = 0x00005286;
		*(volatile u32*)(SARCHEM_BASE + 0x5810) = 0x00005286;

		loopback_en=1;
	}
#endif		
	dev->trans_start = jiffies;

	//printk("<---------->\n");
	//Dumpreg((unsigned long)(0xb8400000), (unsigned int)0x100);
	return 0;
}

static unsigned int con_tx_counter = 0;
static int ptm_start_xmit (struct sk_buff *skb, struct net_device *dev)	
{
	//re8670_start_xmit_ptm(skb, dev, -1, -1);
	__ptm_start_xmit(skb, dev, 1, 0);
	return 0;
	
}

/* Set or clear the multicast filter for this adaptor.
   This routine is not state sensitive and need not be SMP locked. */

static void __ptm_set_rx_mode (struct net_device *dev)
{
	/*struct re_private *cp = dev->priv;*/
	u32 mc_filter[2];	/* Multicast hash filter */
	int i, rx_mode;
	/*u32 tmp;*/


	// Kao
	//printk("%s: %s %d Still Unimplemented !!!!!!!\n",__FILE__,__FUNCTION__,__LINE__);
	//return ;
	/* Note: do not reorder, GCC is clever about common statements. */
	if (dev->flags & IFF_PROMISC) {
		/* Unconditionally log net taps. */
		printk (KERN_NOTICE "%s: Promiscuous mode enabled.\n",
			dev->name);
		rx_mode =
		    AcceptBroadcast | AcceptMulticast | AcceptMyPhys |
		    AcceptAllPhys;
		mc_filter[1] = mc_filter[0] = 0xffffffff;
	} else if ((dev->mc_count > multicast_filter_limit)
		   || (dev->flags & IFF_ALLMULTI)) {
		/* Too many to filter perfectly -- accept all multicasts. */
		rx_mode = AcceptBroadcast | AcceptMulticast | AcceptMyPhys;
		mc_filter[1] = mc_filter[0] = 0xffffffff;
	} else {
		struct dev_mc_list *mclist;
		rx_mode = AcceptBroadcast | AcceptMyPhys;
		mc_filter[1] = mc_filter[0] = 0;
		for (i = 0, mclist = dev->mc_list; mclist && i < dev->mc_count;
		     i++, mclist = mclist->next) {
			int bit_nr = ether_crc(ETH_ALEN, mclist->dmi_addr) >> 26;

			mc_filter[bit_nr >> 5] |= 1 << (bit_nr & 31);
			rx_mode |= AcceptMulticast;
		}
	}

	/* We can safely update without stopping the chip. */
	// Kao, 2004/01/07
	RTL_W32(MAR0, mc_filter[0]);
	RTL_W32(MAR4, mc_filter[1]);
	rx_mode=0x3f;
	printk("rx_mode=%x\n", rx_mode);
	RTL_W16(RCR, rx_mode);
}

static void ptm_set_rx_mode (struct net_device *dev)
{
	unsigned long flags;
	struct re_private *cp = dev->priv;
	// Kaohj
#ifdef CONFIG_EXT_SWITCH
	if (dev->base_addr == 0) {
		// not the root device
		return;
	}
#endif

	spin_lock_irqsave (&cp->lock, flags);
	__ptm_set_rx_mode(dev);
	spin_unlock_irqrestore (&cp->lock, flags);
}

// Kao, 2004/01/07
static int (*my_eth_mac_addr)(struct net_device *, void *);
static int ptm_set_mac_addr(struct net_device *dev, void *addr)
{
	u32 *hwaddr;
	int err;
	err = my_eth_mac_addr(dev, addr);
	if (!err)
	{
		hwaddr = (u32 *)dev->dev_addr;
		RTL_W32(IDR0, *hwaddr);
		hwaddr = (u32 *)(dev->dev_addr+4);
		RTL_W32(IDR4, *hwaddr);
	}
	
	return err;
}

static void __ptm_get_stats(struct re_private *cp)
{
	/* XXX implement */
}

static struct net_device_stats *ptm_get_stats(struct net_device *dev)
{
	struct re_private *cp = dev->priv;

	/* The chip only need report frame silently dropped. */
	spin_lock_irq(&cp->lock);
 	if (netif_running(dev) && netif_device_present(dev))
 		__ptm_get_stats(cp);
	spin_unlock_irq(&cp->lock);

	return &cp->net_stats;
}

static void ptm_stop_hw (struct re_private *cp)
{
	int queue;

	RTL_W32(IO_CMD,0); /* timer  rx int 1 packets*/
	RTL_W32(IMR0, 0);
	RTL_W32(ISR0, 0xffffffff);

	//synchronize_irq();
	synchronize_irq(cp->dev->irq);
	udelay(10);

	for (queue=0; queue<4; queue++)
	{
		cp->rx_tail[queue] = 0;
		cp->tx_fdpf_head[queue] = cp->tx_fdpf_tail[queue] = 0;
		cp->tx_fdps_head[queue] = cp->tx_fdps_tail[queue] = 0;
	}

}

static void ptm_reset_hw (struct re_private *cp)
{
	unsigned work = 1000;

   	RTL_W8(CMD,0x1);	 /* Reset */	
	while (work--) {
		if (!(RTL_R8(CMD) & 0x1))
			return;
		set_current_state(TASK_UNINTERRUPTIBLE);
		schedule_timeout(10);
	}
   	RTL_W8(CMD,0x1);	 /* Reset */	
	while (work--) {
		if (!(RTL_R8(CMD) & 0x1))
			return;
		set_current_state(TASK_UNINTERRUPTIBLE);
		schedule_timeout(10);
	}


   	RTL_W8(CMD,0x2);	 /* checksum */	

	printk(KERN_ERR "%s: hardware reset timeout\n", cp->dev->name);
}

static inline void ptm_start_hw (struct re_private *cp)
{
	RTL_W32(IO_CMD,ptm_iocmd_reg); /* timer  rx int 1 packets*/
}

static void ptm_init_hw (struct re_private *cp)
{
	struct net_device *dev = cp->dev;
	//u8 status;
	// Kao
	u32 *hwaddr;

	ptm_reset_hw(cp);
	RTL_W8(CMD,0x2);	 /* checksum */
	
	RTL_W32(BD_SLV_NUM, 0x00000002);
	RTL_W32(BOND_FRAG_LF, 0x000000b4);
	RTL_W32(BOND_FRAG_HF, 0x00000058);
	RTL_W32(BOND_FRAG_LS, 0x000000ac);
	RTL_W32(BOND_FRAG_HS, 0x00000050);
	RTL_W32(BD_TIMEOUT, 0x000061a8);

	RTL_W16(ISR1, 0);
	RTL_W32(ISR0,0xffffffff);

	RTL_W32(IMR0, re8670_intr_mask);   	

	RTL_W32(RxFDP_H_S, (((u32)(cp->rx_hsring))&0x1fffffff));
	RTL_W16(RxCDO_H_S, 0);
	RTL_W32(RxFDP_L_S, ((u32)(cp->rx_lsring))&0x1fffffff);
	RTL_W16(RxCDO_L_S, 0);
	RTL_W32(RxFDP_H_F, ((u32)(cp->rx_hfring))&0x1fffffff);
	RTL_W16(RxCDO_H_F, 0);
	RTL_W32(RxFDP_L_F, ((u32)(cp->rx_lfring))&0x1fffffff);
	RTL_W16(RxCDO_L_F, 0);


	{
		int channel;
		for(channel=0; channel<4; channel++)
		{
			RTL_W32((TxFDP0_S+(0x10*channel)), ((u32)(cp->tx_fdps_ring[channel]))&0x1fffffff);
			RTL_W16(TxCDO0_S+(0x10*channel), 0);
			RTL_W32((TxFDP0_F+(0x10*channel)), ((u32)(cp->tx_fdpf_ring[channel]))&0x1fffffff);
			RTL_W16(TxCDO0_F+(0x10*channel), 0);
		}			
	}


	// Kao, set hw ID for physical match
	hwaddr = (u32 *)cp->dev->dev_addr;
	RTL_W32(IDR0, *hwaddr);
	hwaddr = (u32 *)(cp->dev->dev_addr+4);
	RTL_W32(IDR4, *hwaddr);


#ifdef CONFIG_PTM_HDLC
	RTL_W32(PCR, 0x00000002);
#else
	RTL_W32(PCR, 0x00000119);  //64/65 mode turns on short packet mode! BUG!!!
	printk("[%s, line %d] PCR value = 0x%08X\n",__func__,__LINE__,RTL_R32(PCR));
#endif

	ptm_start_hw(cp);
	__ptm_set_rx_mode(dev);
}

static int ptm_refill_rx (struct re_private *cp)
{
	unsigned i;

	printk("Allocating %d skb for %s low RX ring, rx_buf_sz=%d\n", RE8670_RX_RING_SIZE, DRV_NAME, cp->rx_buf_sz);
	for (i = 0; i < RE8670_RX_RING_SIZE; i++) 
	{
		struct sk_buff *skb;
// 0
		skb = ptm_getAlloc();
		if (!skb)
			goto err_out;

		skb->dev = cp->dev;

		cp->rx_hs_skb[i].skb = skb;
		cp->rx_hs_skb[i].frag = 0;
		if ((u32)skb->data &0x3)
			printk(KERN_DEBUG "skb->data unaligment %8x\n",(u32)skb->data);

		cp->rx_hsring[i].addr = ((u32)skb->data|UNCACHE_MASK)&0x1fffffff;
		if (i == (RE8670_RX_RING_SIZE - 1))
			cp->rx_hsring[i].opts1 = (DescOwn | RingEnd | cp->rx_buf_sz);
		else
			cp->rx_hsring[i].opts1 =(DescOwn | cp->rx_buf_sz);

		cp->rx_hsring[i].opts2 = 0;

// 1
		skb = ptm_getAlloc();
		if (!skb)
			goto err_out;

		skb->dev = cp->dev;

		cp->rx_ls_skb[i].skb = skb;
		cp->rx_ls_skb[i].frag = 0;
		if ((u32)skb->data &0x3)
			printk(KERN_DEBUG "skb->data unaligment %8x\n",(u32)skb->data);

		cp->rx_lsring[i].addr = ((u32)skb->data|UNCACHE_MASK)&0x1fffffff;
		if (i == (RE8670_RX_RING_SIZE - 1))
			cp->rx_lsring[i].opts1 = (DescOwn | RingEnd | cp->rx_buf_sz);
		else
			cp->rx_lsring[i].opts1 =(DescOwn | cp->rx_buf_sz);

		cp->rx_lsring[i].opts2 = 0;

// 2
		skb = ptm_getAlloc();
		if (!skb)
			goto err_out;

		skb->dev = cp->dev;

		cp->rx_hf_skb[i].skb = skb;
		cp->rx_hf_skb[i].frag = 0;
		if ((u32)skb->data &0x3)
			printk(KERN_DEBUG "skb->data unaligment %8x\n",(u32)skb->data);

		cp->rx_hfring[i].addr = ((u32)skb->data|UNCACHE_MASK)&0x1fffffff;
		if (i == (RE8670_RX_RING_SIZE - 1))
			cp->rx_hfring[i].opts1 = (DescOwn | RingEnd | cp->rx_buf_sz);
		else
			cp->rx_hfring[i].opts1 =(DescOwn | cp->rx_buf_sz);

		cp->rx_hfring[i].opts2 = 0;

// 3
		skb = ptm_getAlloc();
		if (!skb)
			goto err_out;

		skb->dev = cp->dev;

		cp->rx_lf_skb[i].skb = skb;
		cp->rx_lf_skb[i].frag = 0;
		if ((u32)skb->data &0x3)
			printk(KERN_DEBUG "skb->data unaligment %8x\n",(u32)skb->data);

		cp->rx_lfring[i].addr = ((u32)skb->data|UNCACHE_MASK)&0x1fffffff;
		if (i == (RE8670_RX_RING_SIZE - 1))
			cp->rx_lfring[i].opts1 = (DescOwn | RingEnd | cp->rx_buf_sz);
		else
			cp->rx_lfring[i].opts1 =(DescOwn | cp->rx_buf_sz);

		cp->rx_lfring[i].opts2 = 0;

	}

	return 0;

err_out:
	ptm_clean_rings(cp);
	return -ENOMEM;
}

#if 0
static void ptm_tx_timeout (struct net_device *dev)
{
	struct re_private *cp = dev->priv;

	unsigned tx_head = cp->tx_fdpf_head[0];
	unsigned tx_tail = cp->tx_fdpf_tail[0];

	cp->cp_stats.tx_timeouts++;

	spin_lock_irq(&cp->lock);
	while (tx_tail != tx_head) {
		struct sk_buff *skb;
		u32 status;
		rmb();

		status = (cp->tx_fdpf_ring[0][tx_tail].opts1);
		if (status & DescOwn)
			break;

		skb = cp->tx_fdpf_skb[0][tx_tail].skb;
		if (!skb)
			BUG();
		
		cp->net_stats.tx_packets++;
		cp->net_stats.tx_bytes += skb->len;
		dev_kfree_skb(skb);

		cp->tx_fdpf_skb[0][tx_tail].skb = NULL;

		tx_tail = NEXT_TX(tx_tail);
	}

	cp->tx_fdpf_tail[0] = tx_tail;

	spin_unlock_irq(&cp->lock);
	if (netif_queue_stopped(cp->dev))
		netif_wake_queue(cp->dev);

}
#else
static void ptm_tx_timeout (struct net_device *dev)
{
	struct re_private *cp = dev->priv;
	unsigned tx_head, tx_tail ;
	int fast=0, channel=0;

	cp->cp_stats.tx_timeouts++;

	for(fast=0; fast<2; fast++){
		for(channel=0; channel<4; channel++){

			spin_lock_irq(&cp->lock);
			
			if(fast==0){  //slow channel
				tx_head = cp->tx_fdps_head[channel];
				tx_tail = cp->tx_fdps_tail[channel];
			}else{	     //fast channel
				tx_head = cp->tx_fdpf_head[channel];
				tx_tail = cp->tx_fdpf_tail[channel];
			}

			while (tx_tail != tx_head) {
				struct sk_buff *skb;
				u32 status;
				rmb();

				if(fast==0)
					status = (cp->tx_fdps_ring[channel][tx_tail].opts1);
				else
					status = (cp->tx_fdpf_ring[channel][tx_tail].opts1);
				
				if (status & DescOwn)
					break;
				
				if(fast==0)
					skb = cp->tx_fdps_skb[channel][tx_tail].skb;
				else
					skb = cp->tx_fdpf_skb[channel][tx_tail].skb;
				
				if (!skb)
					BUG();
				
				cp->net_stats.tx_packets++;
				cp->net_stats.tx_bytes += skb->len;
				dev_kfree_skb(skb);

				if(fast==0)
					cp->tx_fdps_skb[channel][tx_tail].skb = NULL;
				else
					cp->tx_fdpf_skb[channel][tx_tail].skb = NULL;

				tx_tail = NEXT_TX(tx_tail);
			}

			if(fast==0)
				cp->tx_fdps_tail[channel] = tx_tail;
			else
				cp->tx_fdpf_tail[channel] = tx_tail;

			spin_unlock_irq(&cp->lock);
			if (netif_queue_stopped(cp->dev))
				netif_wake_queue(cp->dev);

		}
	}
}
#endif

static int ptm_init_rings (struct re_private *cp)
{
	int queue;
	for (queue=0;queue<4;queue++)
	{
		cp->rx_tail[queue]=0;
		cp->tx_fdpf_head[queue] = cp->tx_fdpf_tail[queue] = cp->tx_fdps_head[queue] = cp->tx_fdps_tail[queue] = 0;
	}

	return ptm_refill_rx (cp);
}

static int ptm_alloc_rings (struct re_private *cp)
{
	void*	pBuf;
	
	{
		pBuf = kmalloc(RE8670_RXRING_BYTES, GFP_KERNEL);
		if (!pBuf)
			goto ErrMem;
		cp->rxdesc_buf_hs = pBuf;
		pBuf = (void*)( (u32)(pBuf + DESC_ALIGN-1) &  ~(DESC_ALIGN -1) ) ;
		cp->rx_hsring = (DMA_DESC*)((u32)(pBuf) | UNCACHE_MASK);
	}
	{
		pBuf = kmalloc(RE8670_RXRING_BYTES, GFP_KERNEL);
		if (!pBuf)
			goto ErrMem;
		cp->rxdesc_buf_ls = pBuf;
		pBuf = (void*)( (u32)(pBuf + DESC_ALIGN-1) &  ~(DESC_ALIGN -1) ) ;
		cp->rx_lsring = (DMA_DESC*)((u32)(pBuf) | UNCACHE_MASK);
	}
	{
		pBuf = kmalloc(RE8670_RXRING_BYTES, GFP_KERNEL);
		if (!pBuf)
			goto ErrMem;
		cp->rxdesc_buf_hf = pBuf;
		pBuf = (void*)( (u32)(pBuf + DESC_ALIGN-1) &  ~(DESC_ALIGN -1) ) ;
		cp->rx_hfring = (DMA_DESC*)((u32)(pBuf) | UNCACHE_MASK);
	}
	{
		pBuf = kmalloc(RE8670_RXRING_BYTES, GFP_KERNEL);
		if (!pBuf)
			goto ErrMem;
		cp->rxdesc_buf_lf = pBuf;
		pBuf = (void*)( (u32)(pBuf + DESC_ALIGN-1) &  ~(DESC_ALIGN -1) ) ;
		cp->rx_lfring = (DMA_DESC*)((u32)(pBuf) | UNCACHE_MASK);
	}

	{
		int channel;
		for (channel=0; channel<4; channel++)
		{
			pBuf= kmalloc(RE8670_TXRING_BYTES, GFP_KERNEL);
			memset(pBuf, 0, RE8670_TXRING_BYTES);
			if (!pBuf)
				goto ErrMem;
			
			cp->txdesc_buf_s[channel] = pBuf;
			pBuf = (void*)( (u32)(pBuf + DESC_ALIGN-1) &  ~(DESC_ALIGN -1) ) ;
			cp->tx_fdps_ring[channel] = (DMA_DESC*)((u32)(pBuf));
		}

		for (channel=0; channel<4; channel++)
		{
			pBuf= kmalloc(RE8670_TXRING_BYTES, GFP_KERNEL);
			memset(pBuf, 0, RE8670_TXRING_BYTES);
			if (!pBuf)
				goto ErrMem;
			
			cp->txdesc_buf_f[channel] = pBuf;
			pBuf = (void*)( (u32)(pBuf + DESC_ALIGN-1) &  ~(DESC_ALIGN -1) ) ;
			cp->tx_fdpf_ring[channel] = (DMA_DESC*)((u32)(pBuf));
		}
	}

	return ptm_init_rings(cp);

ErrMem:

	printk("ErrMemory![%s]\n", __FUNCTION__);
	//for(loopcnt=0;loopcnt<4;loopcnt++)
	//	if (cp->rxdesc_buf[loopcnt])
	//		kfree(cp->rxdesc_buf[loopcnt]);
	if (cp->rxdesc_buf_hs)
		kfree(cp->rxdesc_buf_hs);
	if (cp->rxdesc_buf_ls)
		kfree(cp->rxdesc_buf_ls);
	if (cp->rxdesc_buf_hf)
		kfree(cp->rxdesc_buf_hf);
	if (cp->rxdesc_buf_lf)
		kfree(cp->rxdesc_buf_lf);

	{
		int channel;
		for(channel=0; channel<4; channel++)
		{
			if (cp->txdesc_buf_s[channel])
				kfree(cp->txdesc_buf_s[channel]);
		}

		for(channel=0; channel<4; channel++)
		{
			if (cp->txdesc_buf_f[channel])
				kfree(cp->txdesc_buf_f[channel]);
		}

	}


	return -ENOMEM;

	
}

static void ptm_clean_rings (struct re_private *cp)
{
	unsigned i;
	struct sk_buff *skb;
	int channel;

	for (i = 0; i < RE8670_RX_RING_SIZE; i++) 
	{
		if (cp->rx_hs_skb[i].skb)
		{
			dev_kfree_skb(cp->rx_hs_skb[i].skb);
		}
	}
	for (i = 0; i < RE8670_RX_RING_SIZE; i++) 
	{
		if (cp->rx_ls_skb[i].skb)
		{
			dev_kfree_skb(cp->rx_ls_skb[i].skb);
		}
	}
	for (i = 0; i < RE8670_RX_RING_SIZE; i++) 
	{
		if (cp->rx_hf_skb[i].skb)
		{
			dev_kfree_skb(cp->rx_hf_skb[i].skb);
		}
	}
	for (i = 0; i < RE8670_RX_RING_SIZE; i++) 
	{
		if (cp->rx_lf_skb[i].skb)
		{
			dev_kfree_skb(cp->rx_lf_skb[i].skb);
		}
	}

	for (i = 0; i < RE8670_TX_RING_SIZE; i++) {
		int channel;
		for(channel=0; channel<4; channel++)
		{
			skb = cp->tx_fdps_skb[channel][i].skb;
			dev_kfree_skb(skb);
			cp->net_stats.tx_dropped++;
		}

		for(channel=0; channel<4; channel++)
		{
			skb = cp->tx_fdpf_skb[channel][i].skb;
			dev_kfree_skb(skb);
			cp->net_stats.tx_dropped++;
		}
	}

	memset(&cp->rx_hs_skb, 0, sizeof(struct ring_info) * RE8670_RX_RING_SIZE);
	memset(&cp->rx_ls_skb, 0, sizeof(struct ring_info) * RE8670_RX_RING_SIZE);
	memset(&cp->rx_hf_skb, 0, sizeof(struct ring_info) * RE8670_RX_RING_SIZE);
	memset(&cp->rx_lf_skb, 0, sizeof(struct ring_info) * RE8670_RX_RING_SIZE);


	for(channel=0; channel<4; channel++)
	{
		memset(&cp->tx_fdps_skb[channel][0], 0, sizeof(struct ring_info) * RE8670_TX_RING_SIZE);
		memset(&cp->tx_fdpf_skb[channel][0], 0, sizeof(struct ring_info) * RE8670_TX_RING_SIZE);
	}
	
}

static void ptm_free_rings (struct re_private *cp)
{
	ptm_clean_rings(cp);
	/*pci_free_consistent(cp->pdev, CP_RING_BYTES, cp->rx_ring, cp->ring_dma);*/

	kfree(cp->rxdesc_buf_hs);
	kfree(cp->rxdesc_buf_ls);
	kfree(cp->rxdesc_buf_hf);
	kfree(cp->rxdesc_buf_lf);

	int channel;
	for(channel=0; channel<4; channel++)
	{
		kfree(cp->txdesc_buf_s[channel]);
		kfree(cp->txdesc_buf_f[channel]);
	}

	cp->rx_hsring = NULL;
	cp->rx_lsring = NULL;
	cp->rx_hfring = NULL;
	cp->rx_lfring = NULL;

	for(channel=0; channel<4; channel++)
	{
		cp->tx_fdps_ring[channel] = NULL;
		cp->tx_fdpf_ring[channel] = NULL;
	}

}

static int ptm_open (struct net_device *dev)
{
	struct re_private *cp = dev->priv;
	int rc;

	// Kaohj
	eth_close=0;
	if (netif_msg_ifup(cp))
		printk(KERN_DEBUG "%s: enabling interface\n", dev->name);

#ifdef ENABLE_LOOPBACK
	*(volatile u32*)(SARCHEM_BASE + 0x5800) = 0x00000006;  //czyao
	*(volatile u32*)(SARCHEM_BASE + 0x6800) = 0x0000002a;  //czyao
#ifdef CONFIG_PTM_HDLC
	*(volatile u32*)(SARCHEM_BASE + 0x5824) = 0x0001c101; //czyao
#else
	*(volatile u32*)(SARCHEM_BASE + 0x5824) = 0x0002c101; //czyao
#endif
	//*(volatile u32*)(0xb8a85810) = 0x00000006;
	//Dumpreg((unsigned long)0xb8a85824, (unsigned int)4);
	//Dumpreg((unsigned long)0xb8a85810, (unsigned int)4);
	Dumpreg((unsigned long)(SARCHEM_BASE + 0x5824), (unsigned int)4);
	Dumpreg((unsigned long)(SARCHEM_BASE + 0x5810), (unsigned int)4);
#endif

	ptm_set_rxbufsize(cp);	/* set new rx buf size */
	rc = ptm_alloc_rings(cp);
	if (rc)
		return rc;

	ptm_init_hw(cp);
	// Kaohj

	cp->sts_reg = 0;	//clear PTM int status
	rc = request_irq(dev->irq, (irq_handler_t)ptm_interrupt, IRQF_DISABLED, dev->name, dev);
	if (rc)
		goto err_out_hw;
	
	netif_start_queue(dev);	

	//tx_loopback(dev, 1);
	return 0;

err_out_hw:
	ptm_stop_hw(cp);
	ptm_free_rings(cp);
	return rc;
}

static int ptm_close (struct net_device *dev)
{
	struct re_private *cp = dev->priv;

	// Kaohj
	eth_close=1;

	if (netif_msg_ifdown(cp))
		printk(KERN_DEBUG "%s: disabling interface\n", dev->name);

	netif_stop_queue(dev);
	ptm_stop_hw(cp);
	free_irq(dev->irq, dev);
	ptm_free_rings(cp);
	return 0;
}


static void show_rx2(void){
	if(rxtest_running2==0)
		return;
	printk("time out!\n");
	rxtest_timer2.expires=jiffies+HZ;
	rxtest_timer2.function=(void (*)(unsigned long))show_rx2;
	ptm_rx_count=0;
	add_timer(&rxtest_timer2);
}

#ifdef PTMCTL_LOG
unsigned int txpkt_cnt=0;
#endif
static  void ptm_ctl(struct eth_arg * arg){
	unsigned char cmd1;
	unsigned int cmd2,cmd3,i,len;
	unsigned short cmd4;
#ifdef PTM2
	unsigned char cmd5;
#endif
	struct skb_shared_info skbshare;
	struct re_private *cp = ptm_net_dev->priv;
	
	cmd1=arg->cmd;
	cmd2=arg->cmd2;
	cmd3=arg->cmd3;
	cmd4=(unsigned short)arg->cmd4;
#ifdef PTM2
	cmd5=(unsigned char)arg->cmd5;
#endif
	//printk("cmd1:%d, cmd2:%d, cmd3:%d, cmd4:%d\n", cmd1, cmd2, cmd3, cmd4);
	switch(cmd1){
		case 1: //tx
			con_tx_counter = cmd2;
#ifdef PTMCTL_LOG
			if (txpkt_cnt==0)
				txpkt_cnt=cmd2;
			else
				txpkt_cnt--;
#endif
			if(cmd3!=0){
			    if(cmd3 < 64)
			        len = 64;
			    else if(cmd3 > 1514)
			        len = 1514;
			    len = cmd3;
			} else
			    len = test_len;
			    
			skbshare.nr_frags=0;
			
			for(i=0;i< len ;i++){
				ptm_testskbdata1[i]=i%0x100;

				//if(i==len-1)
					//ptm_testskbdata1[i] = 0x7e;
			}			
			testskb1.end=(unsigned char*)&skbshare;
			testskb1.data = ptm_testskbdata1;
			testskb1.len = len ;
#ifdef PTM2
			//if (cmd4==0 && cmd5==0)
			//	ptm_start_xmit(&testskb1,  ptm_net_dev);
			//else
#if 0
			if (cmd4>=0 && cmd5>=0)
				__ptm_start_xmit(&testskb1,  ptm_net_dev, cmd4, cmd5);
			else
#endif
				__ptm_start_xmit(&testskb1,  ptm_net_dev, ptm_tx_fast, ptm_tx_channel);
#else
			ptm_start_xmit(&testskb1,  ptm_net_dev);
#endif
		
/*            skbshare.nr_frags=0;
			testskb1.end=(unsigned char*)&skbshare;
			testskb1.data = ptm_testdata2;
			testskb1.len = cmd3 ;
			ptm_start_xmit(&testskb1,  ptm_net_dev);*/
						
			//prepare test data for low tx queue
#if 0			
			len=54;
			for(i=0;i<len;i++)
				ptm_testskbdata[i] = testPkt5[i];							
			testskb2.end=(unsigned char*)&skbshare;
			testskb2.data=ptm_testskbdata;
			testskb2.len=len;
			ptm_start_xmit(&testskb2,  ptm_net_dev);
			mdelay(1);

			len=60;
			for(i=0;i<len;i++)
				ptm_testskbdata[i] = testPkt0[i];							
			testskb2.end=(unsigned char*)&skbshare;
			testskb2.data=ptm_testskbdata;
			testskb2.len=len;
			ptm_start_xmit(&testskb2,  ptm_net_dev);
			mdelay(1);

			len=80;
			for(i=0;i<len;i++)
				ptm_testskbdata[i] = testPkt1[i];							
			testskb2.end=(unsigned char*)&skbshare;
			testskb2.data=ptm_testskbdata;
			testskb2.len=len;
			ptm_start_xmit(&testskb2,  ptm_net_dev);
			mdelay(1);


			len=60;
			for(i=0;i<len;i++)
				ptm_testskbdata[i] = testPkt2[i];							
			testskb2.end=(unsigned char*)&skbshare;
			testskb2.data=ptm_testskbdata;
			testskb2.len=len;
			ptm_start_xmit(&testskb2,  ptm_net_dev);
			mdelay(1);

			len=80;
			for(i=0;i<len;i++)
				ptm_testskbdata[i] = testPkt3[i];							
			testskb2.end=(unsigned char*)&skbshare;
			testskb2.data=ptm_testskbdata;
			testskb2.len=len;
			ptm_start_xmit(&testskb2,  ptm_net_dev);
			mdelay(1);
#endif						
			break;
		case 2://rx
#if 0		
			if(rxtest_running2==1){
				rxtest_running2=0;
				break;
			}
			rxtest_timer2.expires=jiffies+cmd2*HZ;
			rxtest_timer2.function=(void (*)(unsigned long))show_rx2;
			ptm_rx_count=0;
			rxtest_running2=1;
			add_timer(&rxtest_timer2);
#else
            disable_rx_message = cmd2;
			if(disable_rx_message == 0) {
				//disable_rx_message = 0;
				printk("#############  DISABLE RX DEBUG MESSAGE  ##############\n");
			} else {
				// disable_rx_message = 1;
				printk("#############  ENABLE RX DEBUG MESSAGE  ##############\n");
			}			
#endif
			break;
        case 3:
			printk("!!!!!!!!!!! Reset PTM HW !!!!!!!!!!\n");
			ptm_stop_hw(cp);
			ptm_free_rings(cp);
			ptm_alloc_rings(cp);
			ptm_init_hw(cp);

			break;

	case 4:
			//tx_loopback(ptm_net_dev, cmd2);
			if(cmd2>=0)
			{
				ptm_tx_fast = cmd2;
			}
			if(cmd3>=0)
			{
				ptm_tx_channel = cmd3;
			}
			break;
#ifdef PTM2
	case 5:
			printk("Read Address 0x%08x with length 0x%08x\n", (unsigned int)cmd2, cmd3);
			Dumpreg((unsigned long)cmd2, (unsigned int)cmd3);
			break;
	case 6:
			printk("Write Address 0x%08x Value 0x%08x\n", (unsigned int)cmd2, cmd3);
			*(volatile u32*)(cmd2) = cmd3;
			break;
	case 7:
			ptm_debug = cmd2;
			break;
#endif
	case 8:
			//for wifi test
			//modify  RX_MIT
			if(cmd2==1) //wifi test
				ptm_iocmd_reg = 0x3c | 1 << 8 | RX_FIFO << 11 |  RX_TIMER << 13 | TX_MIT << 16 | TX_FIFO<<19;
			else
				ptm_iocmd_reg=CMD_CONFIG;
			
			printk("(cmd2 = %d) set iocmd_reg:%x\n",cmd2,ptm_iocmd_reg);
			
			break;

		default:
			printk("error cmd\n");
	}
#ifdef PTMCTL_LOG
	if (cmd1==1&&txpkt_cnt==1)
	{
		int loopcnt;
		printk("------------------------------\n");
		printk("TX:\n");
		for(loopcnt=0; loopcnt<4; loopcnt++)
			printk("    channel%d,  fast:%8u packets,  slow:%8u packets\n", loopcnt, cp->fast_txcnt[loopcnt], cp->slow_txcnt[loopcnt]);

		printk("RX:\n");
		printk("     high slow: %8u packets\n", cp->rxcnt[0]);
		printk("     low slow : %8u packets\n", cp->rxcnt[1]);
		printk("     high fast: %8u packets\n", cp->rxcnt[2]);
		printk("     low fast : %8u packets\n", cp->rxcnt[3]);
		txpkt_cnt=0;
	}
#endif
}

static int ptm_ioctl (struct net_device *dev, struct ifreq *rq, int cmd)
{
	int rc = 0;

	if (!netif_running(dev) && cmd!=SIOCETHTEST)
		return -EINVAL;

	switch (cmd) {

	case SIOCETHTEST:
		ptm_ctl((struct eth_arg *)rq->ifr_data);
		break;
	default:
		rc = -EOPNOTSUPP;
		break;
	}

	return rc;
}

static void __devexit cp_remove_one (struct pci_dev *pdev)
{
	struct net_device *dev = pci_get_drvdata(pdev);
	struct re_private *cp = dev->priv;

	if (!dev)
		BUG();
	unregister_netdev(dev);
	iounmap(cp->regs);
	kfree(dev);
}


int __init ptm_probe (void)
{
#ifdef MODULE
	printk("%s", version);
#endif

	struct net_device *dev;
#ifndef CONFIG_RTL867X_PACKET_PROCESSOR
	struct re_private *cp;
#else
	struct mac_private *tp;
#endif
	int rc;
	void *regs=(void*)ETHBASE;	

	unsigned i;

	//Enable PTM module
	(*(volatile u32*)(BSP_IP_SEL))|= (BSP_DSL_MAC_EN);
	
#ifndef MODULE
	static int version_printed;
	if (version_printed++ == 0)
		printk("%s", version);
#endif

#ifdef CONFIG_RTL867X_PACKET_PROCESSOR
	i=0;  //interface of packet processor, PTM 0~7 is ok
	dev = ptmPP_dev = alloc_etherdev(sizeof(struct mac_private));
	if (!dev)
		return -ENOMEM;

	ptm_net_dev=dev;

	//SET_MODULE_OWNER(dev); //linux 2.6.19
	tp = dev->priv;
	((struct mac_private *)(ptmPP_dev->priv))->tx_intf=i;

	dev->open = start_pp_ptm_rx;
	dev->do_ioctl = ptm_ioctl;
	dev->stop=stop_pp_ptm_rx;
	//dev->set_mac_address = rtl8672_pp_set_mac_addr;				
	dev->hard_start_xmit = rtl8672_ptm_vtx_start_xmit;
	dev->get_stats = rtl8672_mac_get_stats; //for counter
	dev->features |= NETIF_F_SG | NETIF_F_HW_CSUM | NETIF_F_HIGHDMA;
#if 1 
	dev->tx_timeout = ptm_tx_timeout;
	dev->watchdog_timeo = TX_TIMEOUT;
#endif

	dev->irq=BSP_PKT_SAR_IRQ;
	//tylo, struct conflict
	dev->priv_flags = IFF_DOMAIN_ELAN;
#ifdef PTM2
	strcpy(dev->name, "eth1");
#endif
	
#else
	dev = alloc_etherdev(sizeof(struct re_private));
	if (!dev)
		return -ENOMEM;
	//SET_MODULE_OWNER(dev);   //linux 2.6.19
	cp = dev->priv;
	ptm_net_dev=dev;
	
	cp->dev = dev;
	spin_lock_init (&cp->lock);
	
	
	dev->base_addr = (unsigned long) regs;
	cp->regs = regs;

	ptm_stop_hw(cp);

	/* read MAC address from EEPROM */
	for (i = 0; i < 3; i++)
		((u16 *) (dev->dev_addr))[i] = i;

	dev->open = ptm_open;
	dev->stop = ptm_close;
	dev->set_multicast_list = ptm_set_rx_mode;
	dev->hard_start_xmit = ptm_start_xmit;
	dev->get_stats = ptm_get_stats;
	// Kao, 2004/01/07, enable set mac address
	my_eth_mac_addr = dev->set_mac_address;
	dev->set_mac_address = ptm_set_mac_addr;
	dev->do_ioctl = ptm_ioctl;
	/*dev->change_mtu = re8670_change_mtu;*/
#if 1 
	dev->tx_timeout = ptm_tx_timeout;
	dev->watchdog_timeo = TX_TIMEOUT;
#endif
	dev->irq = BSP_PTM_IRQ;		//PTM 0
#ifdef PTM2
	strcpy(dev->name, "eth1");
#endif

	// Kaohj
	dev->priv_flags = IFF_DOMAIN_WAN; //shlee make it as wan device, IFF_DOMAIN_ELAN;
#endif  //end CONFIG_RTL867X_PACKET_PROCESSOR

#if 0	
#ifndef CONFIG_RTL867X_PACKET_PROCESSOR
	re8670_stop_hw(cp);
#endif
	/* read MAC address from EEPROM */
	for (i = 0; i < 3; i++)
		((u16 *) (dev->dev_addr))[i] = i;

	dev->open = ptm_open;
	dev->stop = ptm_close;
	dev->set_multicast_list = ptm_set_rx_mode;
	dev->hard_start_xmit = re8670_start_xmit;
	dev->get_stats = ptm_get_stats;
	// Kao, 2004/01/07, enable set mac address
	my_eth_mac_addr = dev->set_mac_address;
	dev->set_mac_address = ptm_set_mac_addr;
	dev->do_ioctl = ptm_ioctl;
	/*dev->change_mtu = re8670_change_mtu;*/
#if 1 
	dev->tx_timeout = ptm_tx_timeout;
	dev->watchdog_timeo = TX_TIMEOUT;
#endif
	dev->irq = PTM_IRQ;		//PTM 0
#ifdef PTM2
	strcpy(dev->name, "eth1");
#endif

	// Kaohj

	dev->priv_flags = IFF_DOMAIN_WAN; //shlee make it as wan device, IFF_DOMAIN_ELAN;
#endif

//#ifndef CONFIG_RTL867X_PACKET_PROCESSOR	
	rc = register_netdev(dev);
	if (rc)
		goto err_out_iomap;
	printk(KERN_INFO "Register %s as WAN netdev to kernel\n", DRV_NAME);		
//#endif

	printk (KERN_INFO "%s: %s at 0x%lx, "
		"%02x:%02x:%02x:%02x:%02x:%02x, "
		"IRQ %d\n",
		dev->name,
		"RTL-8672PTM0",
		dev->base_addr,
		dev->dev_addr[0], dev->dev_addr[1],
		dev->dev_addr[2], dev->dev_addr[3],
		dev->dev_addr[4], dev->dev_addr[5],
		dev->irq);

#ifdef CONFIG_RTL867X_PACKET_PROCESSOR
	;
#else
	memset(&cp->rx_tasklets, 0, sizeof(struct tasklet_struct));
	cp->rx_tasklets.func=(void (*)(unsigned long))ptm_rx;
	cp->rx_tasklets.data=(unsigned long)cp;
#endif
	
	/*
	 * Looks like this is necessary to deal with on all architectures,
	 * even this %$#%$# N440BX Intel based thing doesn't get it right.
	 * Ie. having two NICs in the machine, one will have the cache
	 * line set at boot time, the other will not.
	 */
	return 0;

err_out_iomap:
	iounmap(regs);
	kfree(dev);
	return -1 ;

}

static void __exit ptm_exit (void)
{
}
module_init(ptm_probe);
module_exit(ptm_exit);

