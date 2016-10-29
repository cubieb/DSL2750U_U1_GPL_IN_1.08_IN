/****************************************************************************/
/*                                                                          */
/* bf_vmac.h                                                                */
/*                                                                          */
/* Bandit Family Ethernet (ARC Intl' VMAC) Hardware Definitions             */
/*                                                                          */
/****************************************************************************/

#ifndef BF_VMAC_H
#define BF_VMAC_H

/* C structure mapping onto the Ethernet block registers */

typedef unsigned char	U8;
typedef unsigned char	BYTE;



typedef volatile struct {
    unsigned ID;          /* 0x00 */
    unsigned STAT;        /* 0x04 */
    unsigned ENABLE;      /* 0x08 */
    unsigned CONTROL;     /* 0x0c */
    unsigned POLLRATE;    /* 0x10 */
    unsigned RXERR;       /* 0x14 */
    unsigned MISS;        /* 0x18 */
    unsigned TXRINGPTR;   /* 0x1c */
    unsigned RXRINGPTR;   /* 0x20 */
    unsigned ADDRL;       /* 0x24 */
    unsigned ADDRH;       /* 0x28 */
    unsigned LAFL;        /* 0x2c */
    unsigned LAFH;        /* 0x30 */
    unsigned MDIO;        /* 0x34 */
    unsigned TXPTRREAD;   /* 0x38 */
    unsigned RXPTRREAD;   /* 0x3c */
    unsigned XTRACTRL;    /* 0x40 */
    unsigned XTRASTAT;    /* 0x44 */
} BF_VMAC_REGS;

#define offsetof(_T, _M) ((size_t)((char *)&(((_T *)0)->_M) - (char *)0))

#define BF_VMAC_ID_VALUE             (0x0007fd02)  /* ID register contents    */
#define BF_VMAC_0                    (0x30000400)  /* 10/100BT Ethernet MAC#0 */

/* Interrupt Status Register Bit Masks */
#define BF_VMAC_STAT_TXINT           (0x00000001)  /* Tx interrupt pending    */
#define BF_VMAC_STAT_RXINT           (0x00000002)  /* Rx interrupt pending    */
#define BF_VMAC_STAT_ERR             (0x00000004)  /* Error interrupt pending */
#define BF_VMAC_STAT_BUFFERR         (0x00000008)  /* Error in Tx buffer      */
#define BF_VMAC_STAT_TXCH            (0x00000008)  /* Error in Tx buffer      */
#define BF_VMAC_STAT_MISSERR         (0x00000010)  /* Missed pkt count roll   */
#define BF_VMAC_STAT_SQE             (0x00000020)  /* SQE error               */
#define BF_VMAC_STAT_RXCRC           (0x00000100)  /* RXCRC count roll over   */
#define BF_VMAC_STAT_RXFRAME         (0x00000200)  /* RX frame count roll ovr */
#define BF_VMAC_STAT_RXOFLOW         (0x00000400)  /* RX overflow count roll  */
#define BF_VMAC_STAT_MDIO            (0x00001000)  /* MDIO complete           */
#define BF_VMAC_STAT_TXPOLL          (0x80000000)  /* Poll Tx descriptors     */

/* Interrupt Enable Register Masks */
/* Same masks as ISR above */

/* Control Register Bit Masks */
#define BF_VMAC_CONTROL_ENABLE       (0x00000001)  /* Disable/Enable ethernet */
#define BF_VMAC_CONTROL_TXRUN        (0x00000008)  /* Disable/Enable Tx       */
#define BF_VMAC_CONTROL_RXRUN        (0x00000010)  /* Disable/Enable Rx       */
#define BF_VMAC_CONTROL_RXINT2TX     (0x00000020)  /* Rx int activates Tx int */
#define BF_VMAC_CONTROL_DISBDCST     (0x00000100)  /* Disable/Enb Rx b'casts  */
#define BF_VMAC_CONTROL_DISMCAST     (0x00000200)  /* Disable/Enb Rx m'casts  */
#define BF_VMAC_CONTROL_ENBFULL      (0x00000400)  /* Disable/Enb full duplex */
#define BF_VMAC_CONTROL_PROM         (0x00000800)  /* Enable promiscuous mode */
#define BF_VMAC_CONTROL_ENB2PART     (0x00001000)  /* Disable 2 part deferral */
#define BF_VMAC_CONTROL_TEST         (0x00002000)  /* Silicon testing         */
#define BF_VMAC_CONTROL_DISRETRY     (0x00004000)  /* Disable/Enb Tx retries  */
#define BF_VMAC_CONTROL_DISADDFCS    (0x00008000)  /* Disable/Enb FCS append  */
#define BF_VMAC_CONTROL_TXBDTLEN     (0x00FF0000)  /* No. BDTs Tx ring 1-255  */
#define BF_VMAC_CONTROL_TXBDTLEN_SHIFT       (16)  /* bit shift  for TXBDTLEN */
#define BF_VMAC_CONTROL_RXBDTLEN     (0xFF000000)  /* No. BDTs Rx ring 1-255  */
#define BF_VMAC_CONTROL_RXBDTLEN_SHIFT       (24)  /* bit shift for RXBDTLEN  */

/* Poll Rate Register Bit Masks */
#define BF_VMAC_POLLRATE_MASK        (0x0000FFFF)  /* Units = 1024 cycles     */

/* Rx Error Register Bit Masks */
#define BF_VMAC_RXERR_RXCRC          (0x000000FF)  /* CRC errors count        */
#define BF_VMAC_RXERR_RXFRAM         (0x0000FF00)  /* Frame errors count      */
#define BF_VMAC_RXERR_RXOFLOW        (0x00FF0000)  /* Overflow errors count   */

/* Missed Packet Register Bit Mask */
#define BF_VMAC_MISS_MASK            (0x000000FF)  /* Missed packet counter   */

/* Tx Ring Pointer Address Register Bit Mask */
#define BF_VMAC_TXRINGPTR_MASK       (0xFFFFFFFF)  /* Address of Tx BDT ring  */

/* Rx Ring Pointer Address Register Bit Mask */
#define BF_VMAC_RXRINGPTR_MASK       (0xFFFFFFFF)  /* Address of Rx BDT ring  */

/* Ethernet MAC Address Register Bit Masks */
#define BF_VMAC_ADDRL_MASK           (0xFFFFFFFF)  /* Lower bits for 48-bit
                                                         ethernet MAC address */
#define BF_VMAC_ADDRH_MASK           (0x0000FFFF)  /* Higher bits for 48-bit
                                                         ethernet MAC address */

/* Logical Address Filter Register Bit Masks */
#define BF_VMAC_LAFL_MASK            (0xFFFFFFFF)  /* Lower bits of
                                                              Logical Address */
#define BF_VMAC_LAFH_MASK            (0xFFFFFFFF)  /* Higher bits of
                                                              Logical Address */

/* MDIO_DATA register bits */
#define BF_VMAC_MDIO_SFD             (0x40000000)
#define BF_VMAC_MDIO_OP_WRITE        (0x10000000)
#define BF_VMAC_MDIO_OP_READ         (0x20000000)
#define BF_VMAC_MDIO_PHY_MASK        (0x0F800000)
#define BF_VMAC_MDIO_PHY_SHIFT	     (23)
#define BF_VMAC_MDIO_PHY_MASK_NOT    (~BF_VMAC_MDIO_PHY_MASK)     
#define BF_VMAC_MDIO_REG_MASK        (0x007C0000)
#define BF_VMAC_MDIO_REG_SHIFT       (18)
#define BF_VMAC_MDIO_REG_MASK_NOT    (~BF_VMAC_MDIO_REG_MASK)
#define BF_VMAC_MDIO_TA              (0x00020000)
#define BF_VMAC_MDIO_DATA_MASK       (0x0000FFFF)
#define BF_VMAC_MDIO_DATA_MASK_NOT   (~BF_VMAC_MDIO_DATA_MASK)

/* XTRACTRL register bits - CNXT modifications to VMAC */
#define BF_VMAC_XTRACTRL_RESETMASK   (0xFFC00000)  /* MS bits not reset       */
#define BF_VMAC_XTRACTRL_CLRLOCKOUT  (0x04000000)  /* Reset rxbf_unload_lockout if enable=0*/
#define BF_VMAC_XTRACTRL_DRAINENB    (0x02000000)  /* Enable Drain fix (broken?)    */
#define BF_VMAC_XTRACTRL_MISSBUFDIS  (0x01000000)  /* Stop MISSED affecting RX FIFO */
#define BF_VMAC_XTRACTRL_LOCKFIX     (0x00800000)  /* Auto lockup get-out-of-jail   */
#define BF_VMAC_XTRACTRL_XTRAENB     (0x00400000)  /* Enable XTRA_STAT & CTRL bits  */
#define BF_VMAC_XTRACTRL_PAUSEDIS    (0x00200000)  /* 1=Disable PAUSE RX      */
#define BF_VMAC_XTRACTRL_PADDRDIS    (0x00100000)  /* 1=Disable PADDR decode  */
#define BF_VMAC_XTRACTRL_BVCIFIX     (0x00080000)  /* 1=Enb BVCI HBUSREQ fix  */
#define BF_VMAC_XTRACTRL_TXAUTOPAD   (0x00040000)  /* 1=Auto Pad TX to 64-byte*/
#define BF_VMAC_XTRACTRL_RXBURST8    (0x00020000)  /* 1=8-word DMA writes     */
#define BF_VMAC_XTRACTRL_TXBURST8    (0x00010000)  /* 1=8-word DMA reads      */ 
#define BF_VMAC_XTRACTRL_TXDEPTH     (0x0000FC00)  /* log2(TX FIFO depth)     */
#define BF_VMAC_XTRACTRL_TXDEPTH_SHIFT       (10)  /* bit shift  for TXDEPTH  */
#define BF_VMAC_XTRACTRL_TXTHRESH    (0x000003FF)  /* TX FIFO start threshold */
#define BF_VMAC_XTRACTRL_TXTHRESH_SHIFT       (0)  /* bit shift for TXTHRESH  */

/* XTRASTAT register bits - CNXT modifications to VMAC */
#define BF_VMAC_XTRASTAT_RXBFDATA    (0xFF000000)  // rxbf_data
#define BF_VMAC_XTRASTAT_RXBFTAG     (0x00800000)  // rxbf_tag
#define BF_VMAC_XTRASTAT_RXBFVALID   (0x00400000)  // rxbf_valid
#define BF_VMAC_XTRASTAT_BUFLENPRE0  (0x00080000)  // rx_bufflen_pre[0]
#define BF_VMAC_XTRASTAT_BUFLENPRE_0 (0x00040000)  // (rx_bufflen_pre == 0)
#define BF_VMAC_XTRASTAT_FIFO0VLID_0 (0x00020000)  // (rx_fifo0_valid == 0)
#define BF_VMAC_XTRASTAT_BYTECTR_0   (0x00010000)  // (rx_byte_ctr == 0)
#define BF_VMAC_XTRASTAT_FIFO_EMPTY  (0x00008000)  // rx_fifo_empty
#define BF_VMAC_XTRASTAT_OWN         (0x00004000)  // rx_own
#define BF_VMAC_XTRASTAT_BUFFERR     (0x00002000)  // rx_bufferr
#define BF_VMAC_XTRASTAT_LAST        (0x00001000)  // rx_last
#define BF_VMAC_XTRASTAT_FIRST       (0x00000800)  // rx_first
#define BF_VMAC_XTRASTAT_RSPVALDONE  (0x00000400)  // burst_rspval_done
#define BF_VMAC_XTRASTAT_UL_LOCKOUT  (0x00000200)  // rxbf_unload_lockout
#define BF_VMAC_XTRASTAT_STARTPEND   (0x00000100)  // rx_start_pending
#define BF_VMAC_XTRASTAT_INFOPOLL    (0x00000080)  // rx_info_poll
#define BF_VMAC_XTRASTAT_DMAINPROG   (0x00000040)  // rx_dma_inprogress
#define BF_VMAC_XTRASTAT_RECYCLE     (0x00000020)  // rx_recycle_desc
#define BF_VMAC_XTRASTAT_STATE_MASK  (0x0000001F)  // DMA Current State

/* XTRASTAT STATE field decodes */
#define BF_VMAC_XTRASTAT_S_RESET     (0x00000010)  // Go To IDLE
#define BF_VMAC_XTRASTAT_S_IDLE      (0x00000000)  // Wait for RX/TX poll
#define BF_VMAC_XTRASTAT_S_POLLTX1   (0x00000001)  // Init various regs for TX DMA
#define BF_VMAC_XTRASTAT_S_POLLTX2   (0x00000002)  // Perform the TX DMA
#define BF_VMAC_XTRASTAT_S_POLLTX3   (0x00000003)  // Wait for TX data cleanup
#define BF_VMAC_XTRASTAT_S_POLLRX1   (0x00000011)  // Init various regs for RX DMA
#define BF_VMAC_XTRASTAT_S_POLLRX2   (0x00000012)  // Perform the RX DMA
#define BF_VMAC_XTRASTAT_S_POLLRX3   (0x00000013)  // Wait for RX data cleanup
#define BF_VMAC_XTRASTAT_S_RESTRTX1  (0x0000000C)  // Reset TX BD ptr/len/flags (restart)
#define BF_VMAC_XTRASTAT_S_RECYCRX1  (0x0000001C)  // Reset RX BD ptr/len/flags (recycle)
#define BF_VMAC_XTRASTAT_S_TXDMA1    (0x00000009)  // Setup TX data DMA
#define BF_VMAC_XTRASTAT_S_TXDMA2    (0x0000000A)  // Wait for TX data DMA to finish
#define BF_VMAC_XTRASTAT_S_TXDMA3    (0x0000000B)  // Writeback TX BD
#define BF_VMAC_XTRASTAT_S_RXDMA1    (0x00000019)  // Setup RX data DMA
#define BF_VMAC_XTRASTAT_S_RXDMA2    (0x0000001A)  // Wait for RX data DMA to finish
#define BF_VMAC_XTRASTAT_S_RXDMA3    (0x0000001B)  // Writeback RX BD
#define BF_VMAC_XTRASTAT_S_TXDESC1   (0x00000004)  // Setup TX BD writeback
#define BF_VMAC_XTRASTAT_S_TXDESC2   (0x00000005)  // Write out TX BD INFO word
#define BF_VMAC_XTRASTAT_S_TXDESC3   (0x00000006)  // Increment TX BD ptr
#define BF_VMAC_XTRASTAT_S_TXDESC4   (0x00000007)  // Cleanup
#define BF_VMAC_XTRASTAT_S_RXDESC1   (0x00000014)  // Setup TX BD writeback
#define BF_VMAC_XTRASTAT_S_RXDESC2   (0x00000015)  // Write out RX BD INFO word
#define BF_VMAC_XTRASTAT_S_RXDESC3   (0x00000016)  // Increment RX BD ptr
#define BF_VMAC_XTRASTAT_S_RXDESC4   (0x00000017)  // Cleanup

/************************************************************************
 ** The #define's below relate to the descriptor tables, not hardware. **
 ************************************************************************/

/* Tx Control Information Written By the CPU - Bit Masks */

#define BF_VMAC_TXINFO_CPU_TX_LEN (0x000007FF)    /* Tx Length in this buffer to be Xmitted */
#define BF_VMAC_TXINFO_CPU_FIRST  (0x00010000)    /* First Tx buffer in the packet          */
#define BF_VMAC_TXINFO_CPU_LAST   (0x00020000)    /* Last Tx buffer in the packet           */
#define BF_VMAC_TXINFO_CPU_ADDCRC (0x00040000)    /* Add the CRC tp the pkt                 */
#define BF_VMAC_TXINFO_CPU_OWN    (0x80000000)    /* CPU/VMAC Ownership of buffer           */

/* Tx Control Information Written By the VMAC - Bit Masks */

#define BF_VMAC_TXINFO_TX_LEN     (0x000007FF)    /* Tx Length in this buffer to be Xmitted */
#define BF_VMAC_TXINFO_FIRST      (0x00010000)    /* First Tx buffer in the packet          */
#define BF_VMAC_TXINFO_LAST       (0x00020000)    /* Last Tx buffer in the packet           */
#define BF_VMAC_TXINFO_ADDCRC     (0x00040000)    /* Add the CRC to the pkt that is Xmitted */
#define BF_VMAC_TXINFO_CARR_LOSS  (0x00200000)    /* Carrier Lost during xmission           */
#define BF_VMAC_TXINFO_DEFER      (0x00400000)    /* xmission deferred due to traffic       */
#define BF_VMAC_TXINFO_DROPPED    (0x00800000)    /* pkt dropped after 16 retries           */
#define BF_VMAC_TXINFO_RETRY      (0x0F000000)    /* Retry count for Tx                     */
#define BF_VMAC_TXINFO_LATE_COLL  (0x10000000)    /* Late Collision                         */
#define BF_VMAC_TXINFO_UFLO       (0x20000000)    /* Data not available on time             */
#define BF_VMAC_TXINFO_BUFF       (0x40000000)    /* Buffer error - bad FIRST and LAST      */
#define BF_VMAC_TXINFO_OWN        (0x80000000)    /* CPU/VMAC Ownership of buffer           */

/* Pointer To Tx Buffer Bit Mask */

#define BF_VMAC_TX_BUFFER_PTR     (0xFFFFFFFF)    /* Address of start of the buffer of data */

/* Rx Control Information Written By the VMAC - Bit Masks */

#define BF_VMAC_RXINFO_RX_LEN     (0x000007FF)    /* Rx Length in this buffer to be Xmitted */
#define BF_VMAC_RXINFO_FIRST      (0x00010000)    /* First Rx buffer in the packet          */
#define BF_VMAC_RXINFO_LAST       (0x00020000)    /* Last Rx buffer in the packet           */
#define BF_VMAC_RXINFO_LOCK       (0x20000000)    /* Lockup error                           */
#define BF_VMAC_RXINFO_BUFF       (0x40000000)    /* Buffer chaining error                  */
#define BF_VMAC_RXINFO_OWN        (0x80000000)    /* CPU/VMAC Ownership of buffer           */

/* Rx Control Information Written By the CPU - Bit Masks */

#define BF_VMAC_RXINFO_CPU_RX_LEN (0x000007FF)    /* Rx Length in this buffer to be Xmitted */
#define BF_VMAC_RXINFO_CPU_FIRST  (0x00010000)    /* First Rx buffer in the packet          */
#define BF_VMAC_RXINFO_CPU_LAST   (0x00020000)    /* Last Rx buffer in the packet           */
#define BF_VMAC_RXINFO_CPU_OWN    (0x80000000)    /* CPU/VMAC Ownership of buffer           */

/* Pointer To Rx Buffer Bit Mask */

#define BF_VMAC_RX_BUFFER_PTR     (0xFFFFFFFF)    /* Address of start of the buffer of data */



/* ------ OBSOLETE DEFINITIONS BELOW THIS POINT WILL BE REMOVED WHEN ------- */
/* --------- THE TRANSITION TO C STRUCTURE DEFINITIONS IS COMPLETE --------- */

/* Register offsets in the Ethernet block */
#define BF_VMAC_ID                   (0x000)
#define BF_VMAC_STAT                 (0x004)
#define BF_VMAC_ENABLE               (0x008)
#define BF_VMAC_CONTROL              (0x00c)
#define BF_VMAC_POLLRATE             (0x010)
#define BF_VMAC_RXERR                (0x014)
#define BF_VMAC_MISS                 (0x018)
#define BF_VMAC_TXRINGPTR            (0x01c)
#define BF_VMAC_RXRINGPTR            (0x020)
#define BF_VMAC_ADDRL                (0x024)
#define BF_VMAC_ADDRH                (0x028)
#define BF_VMAC_LAFL                 (0x02c)
#define BF_VMAC_LAFH                 (0x030)
#define BF_VMAC_MDIO                 (0x034)
#define BF_VMAC_TXPTRREAD            (0x038)
#define BF_VMAC_RXPTRREAD            (0x03c)


/*--------------------RESET GPIO-----------------------------------------*/


typedef volatile struct {
    unsigned CONTROL1;     /* 0x00 GPIO control register 1         */
    unsigned CONTROL2;     /* 0x04 GPIO control register 2         */
    unsigned CONTROL3;     /* 0x08 GPIO control register 3         */
    unsigned CONTROL4;     /* 0x0c GPIO control register 4         */
    unsigned CONTROL5;     /* 0x10 GPIO control register 5         */
    unsigned CONTROL6;     /* 0x14 GPIO control register 6         */
    unsigned CONTROL7;     /* 0x18 GPIO control register 7         */
    unsigned CONTROL8;     /* 0x1c GPIO control register 8         */

    unsigned _reserved_20; /* 0x20 reserved */
    unsigned _reserved_24; /* 0x24 reserved */
    unsigned _reserved_28; /* 0x28 reserved */
    unsigned _reserved_2c; /* 0x2c reserved */
    unsigned _reserved_30; /* 0x30 reserved */
    unsigned _reserved_34; /* 0x34 reserved */
    unsigned _reserved_38; /* 0x38 reserved */
    unsigned _reserved_3c; /* 0x3c reserved */

    unsigned OUTPUT1;      /* 0x40 GPIO output register 1          */
    unsigned OUTPUT2;      /* 0x44 GPIO output register 2          */

    unsigned INPUT1;       /* 0x48 GPIO input register 1           */
    unsigned INPUT2;       /* 0x4c GPIO input register 2           */

    unsigned SET1;         /* 0x50 GPIO set register 1             */
    unsigned SET2;         /* 0x54 GPIO set register 2             */

    unsigned CLEAR1;       /* 0x58 GPIO clear register 1           */
    unsigned CLEAR2;       /* 0x5c GPIO clear register 2           */
    
    unsigned _reserved_60; /* 0x60 reserved */
    unsigned _reserved_64; /* 0x60 reserved */
    unsigned _reserved_68; /* 0x60 reserved */
    unsigned _reserved_6c; /* 0x60 reserved */
    unsigned _reserved_70; /* 0x60 reserved */
    unsigned _reserved_74; /* 0x60 reserved */
    unsigned _reserved_78; /* 0x60 reserved */
    unsigned _reserved_7c; /* 0x60 reserved */
    unsigned _reserved_80; /* 0x60 reserved */
    unsigned _reserved_84; /* 0x60 reserved */
    unsigned _reserved_88; /* 0x60 reserved */
    unsigned _reserved_8c; /* 0x60 reserved */
    unsigned _reserved_90; /* 0x60 reserved */
    unsigned _reserved_94; /* 0x60 reserved */
    unsigned _reserved_98; /* 0x60 reserved */
    unsigned _reserved_9c; /* 0x60 reserved */
    unsigned _reserved_a0; /* 0x60 reserved */
    unsigned _reserved_a4; /* 0x60 reserved */
    unsigned _reserved_a8; /* 0x60 reserved */
    unsigned _reserved_ac; /* 0x60 reserved */
    unsigned _reserved_b0; /* 0x60 reserved */
    unsigned _reserved_b4; /* 0x60 reserved */
    unsigned _reserved_b8; /* 0x60 reserved */
    unsigned _reserved_bc; /* 0x60 reserved */
    unsigned _reserved_c0; /* 0x60 reserved */
    unsigned _reserved_c4; /* 0x60 reserved */
    unsigned _reserved_c8; /* 0x60 reserved */
    unsigned _reserved_cc; /* 0x60 reserved */
    unsigned _reserved_d0; /* 0x60 reserved */
    unsigned _reserved_d4; /* 0x60 reserved */
    unsigned _reserved_d8; /* 0x60 reserved */
    unsigned _reserved_dc; /* 0x60 reserved */
    unsigned _reserved_e0; /* 0x60 reserved */
    unsigned _reserved_e4; /* 0x60 reserved */
    unsigned _reserved_e8; /* 0x60 reserved */
    unsigned _reserved_ec; /* 0x60 reserved */
    unsigned _reserved_f0; /* 0x60 reserved */
    unsigned _reserved_f4; /* 0x60 reserved */
    unsigned _reserved_f8; /* 0x60 reserved */
    unsigned _reserved_fc; /* 0x60 reserved */
} BF_GPIO_REGS;

#define BF_NETBOOT_PHY_GPIO_RESET    (4)
#define REGIO(_dev, _reg) ( ((BF_##_dev##_REGS *)(BF_##_dev))->_reg )
#define BF_GPIO 		     (0x40000100)





/* MDIO_DATA register bits */
#define SOLOS_ETHER_MDIO_SFD             (0x40000000)
#define SOLOS_ETHER_MDIO_OP_WRITE        (0x10000000)
#define SOLOS_ETHER_MDIO_OP_READ         (0x20000000)
#define SOLOS_ETHER_MDIO_PHY_MASK        (0x0F800000)
#define SOLOS_ETHER_MDIO_PHY_SHIFT           (23)
#define SOLOS_ETHER_MDIO_PHY_MASK_NOT    (~SOLOS_ETHER_MDIO_PHY_MASK)
#define SOLOS_ETHER_MDIO_REG_MASK        (0x007C0000)
#define SOLOS_ETHER_MDIO_REG_SHIFT       (18)
#define SOLOS_ETHER_MDIO_REG_MASK_NOT    (~SOLOS_ETHER_MDIO_REG_MASK)
#define SOLOS_ETHER_MDIO_TA              (0x00020000)
#define SOLOS_ETHER_MDIO_DATA_MASK       (0x0000FFFF)
#define SOLOS_ETHER_MDIO_DATA_MASK_NOT   (~SOLOS_ETHER_MDIO_DATA_MASK)


/* DEVCONF register */
#define SOLOS_DEVCONF_BASE               (0x40000000)
#define SOLOS_DEVCONF_SOLOSW             (0x0)



#endif
