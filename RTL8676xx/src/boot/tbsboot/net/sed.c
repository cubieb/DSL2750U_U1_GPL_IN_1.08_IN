/* 
 * Ethernet Driver.
 * A Very Simple set of ethernet driver primitives.  The ethernet (3com Mbus)
 * interface is controlled by busy-waiting, the application is handed the
 * location of on-board packet buffers, and allowed to fill in the
 * transmit buffer directly.  The interface is entirely blocking.
 * 
 * Written March, 1986 by Geoffrey Cooper
 *
 * Copyright (C) 1986, IMAGEN Corporation
 *  "This code may be duplicated in whole or in part provided that [1] there
 *   is no commercial gain involved in the duplication, and [2] that this
 *   copyright notice is preserved on all copies.  Any other duplication
 *   requires written notice of the author."
 * 
 * Primitives:
 *  sed_Init()  -- Initialize the package
 *  sed_FormatPacket( destEAddr ) => location of transmit buffer
 *  sed_Send( pkLength ) -- send the packet that is in the transmit buffer
 *  sed_Receive( recBufLocation ) -- enable receiving packets.
 *  sed_IsPacket() => location of packet in receive buffer
 *  sed_CheckPacket( recBufLocation, expectedType )
 *  sed_shutdown();
 *
 * Global Variables:
 *  sed_lclEthAddr -- Ethernet address of this host.
 *  sed_ethBcastAddr -- Ethernet broadcast address.
 */

/* #include <adam2.h> */


#include <types.h>
#include <common.h>
#include <malloc.h>
#include "errno.h"
#define RESET_BASE 	0xa8611600
#define  _malloc malloc

//#include <psbl/sysdefs.h>
//#include <psbl/heapmgr.h>
//#include <psbl/errno.h>
//#include <psbl/sysconf.h>
//#include <psbl/debug.h>
//#include <psbl/string.h>
//#include <psbl/stdio.h>
//#include <psbl/hw.h>
#include <tcp.h>
#include <net.h>
//#include "emacphy.h"
//#include "main.h"



#define E10P_MIN        60              /* Minimum Ethernet packet size */


#define TCB_ENTRIES      16
#define RCB_ENTRIES      32
#define BUFFER_SIZE    1518
#define TX_COMP_ENTRIES  16
#define RX_COMP_ENTRIES  32

#define DISABLE     0x80000000
#define CLRP        0x20000000
#define TXPACE      0x00000100
#define NO_LOOP     0x08000000
#define FULL_DUPLEX 0x00000001
#define SPEED_100   0x00000002
#define CAF         0x00000400
#define CMF         0x00002000
#define PEFCE       0x00080000


#define EMACA_RESET_BIT 17 
#define EMACB_RESET_BIT 21

void *GetRxPacket(void);
void ExitCall( int );
void delay_usecs(unsigned int usecs);
void DispStr(char *val);

extern unsigned int _CpuFrequency;
extern u1 sys_et_addr[6];

bit32u emacbase  = 0xa8610000;
bit32u resetbase = RESET_BASE;

#define PRSTCTRL      (*(volatile unsigned *)(resetbase+0x000))

/* Ethernet MAC register definitions */

#define DMACONFIG      (*(volatile unsigned *)(emacbase+0x000))
#define INTSTS         (*(volatile unsigned *)(emacbase+0x004))
#define INTMASK        (*(volatile unsigned *)(emacbase+0x008))

#define WRAPCLK        (*(volatile unsigned *)(emacbase+0x340))
#define STATSBASE      (*(volatile unsigned *)(emacbase+0x400))
 
#define TCRPTR         (*(volatile unsigned *)(emacbase+0x100))
#define TCRSIZE        (*(volatile unsigned *)(emacbase+0x104))
#define TCRINTTHRESH   (*(volatile unsigned *)(emacbase+0x108))
#define TCRTOTENT      (*(volatile unsigned *)(emacbase+0x10C))
#define TCRFREEENT     (*(volatile unsigned *)(emacbase+0x110))
#define TCRPENDENT     (*(volatile unsigned *)(emacbase+0x114))
#define TCRENTINC      (*(volatile unsigned *)(emacbase+0x118))
#define TXISRPACE      (*(volatile unsigned *)(emacbase+0x11c))

#define TDMASTATE0     (*(volatile unsigned *)(emacbase+0x120))
#define TDMASTATE1     (*(volatile unsigned *)(emacbase+0x124))
#define TDMASTATE2     (*(volatile unsigned *)(emacbase+0x128))
#define TDMASTATE3     (*(volatile unsigned *)(emacbase+0x12C))
#define TDMASTATE4     (*(volatile unsigned *)(emacbase+0x130))
#define TDMASTATE5     (*(volatile unsigned *)(emacbase+0x134))
#define TDMASTATE6     (*(volatile unsigned *)(emacbase+0x138))
#define TDMASTATE7     (*(volatile unsigned *)(emacbase+0x13C))
#define TXPADDCNT      (*(volatile unsigned *)(emacbase+0x140))
#define TXPADDSTART    (*(volatile unsigned *)(emacbase+0x144))
#define TXPADDEND      (*(volatile unsigned *)(emacbase+0x148))
#define TXQFLUSH       (*(volatile unsigned *)(emacbase+0x14C))
 
#define RCRPTR         (*(volatile unsigned *)(emacbase+0x200))
#define RCRSIZE        (*(volatile unsigned *)(emacbase+0x204))
#define RCRINTTHRESH   (*(volatile unsigned *)(emacbase+0x208))
#define RCRTOTENT      (*(volatile unsigned *)(emacbase+0x20C))
#define RCRFREEENT     (*(volatile unsigned *)(emacbase+0x210))
#define RCRPENDENT     (*(volatile unsigned *)(emacbase+0x214))
#define RCRENTINC      (*(volatile unsigned *)(emacbase+0x218))
#define RXISRPACE      (*(volatile unsigned *)(emacbase+0x21c))
 
#define RDMASTATE0     (*(volatile unsigned *)(emacbase+0x220))
#define RDMASTATE1     (*(volatile unsigned *)(emacbase+0x224))
#define RDMASTATE2     (*(volatile unsigned *)(emacbase+0x228))
#define RDMASTATE3     (*(volatile unsigned *)(emacbase+0x22C))
#define RDMASTATE4     (*(volatile unsigned *)(emacbase+0x230))
#define RDMASTATE5     (*(volatile unsigned *)(emacbase+0x234))
#define RDMASTATE6     (*(volatile unsigned *)(emacbase+0x238))
#define RDMASTATE7     (*(volatile unsigned *)(emacbase+0x23C))
#define FBLADDCNT      (*(volatile unsigned *)(emacbase+0x240))
#define FBLADDSTART    (*(volatile unsigned *)(emacbase+0x244))
#define FBLADDEND      (*(volatile unsigned *)(emacbase+0x248))
#define RXONOFF        (*(volatile unsigned *)(emacbase+0x24C))
 
#define FBL0NEXTD      (*(volatile unsigned *)(emacbase+0x280))
#define FBL0LASTD      (*(volatile unsigned *)(emacbase+0x284))
#define FBL0COUNTD     (*(volatile unsigned *)(emacbase+0x288))
#define FBL0BUFSIZE    (*(volatile unsigned *)(emacbase+0x28C))
 
#define MACCONTROL     (*(volatile unsigned *)(emacbase+0x300))
#define MACSTATUS      (*(volatile unsigned *)(emacbase+0x304))
#define MACADDRHI      (*(volatile unsigned *)(emacbase+0x308))
#define MACADDRLO      (*(volatile unsigned *)(emacbase+0x30C))
#define MACHASH1       (*(volatile unsigned *)(emacbase+0x310))
#define MACHASH2       (*(volatile unsigned *)(emacbase+0x314))
 
#define WRAPCLK        (*(volatile unsigned *)(emacbase+0x340))
#define BOFTEST        (*(volatile unsigned *)(emacbase+0x344))
#define PACTEST        (*(volatile unsigned *)(emacbase+0x348))
#define PAUSEOP        (*(volatile unsigned *)(emacbase+0x34C))
 
#define MDIOCONTROL    (*(volatile unsigned *)(emacbase+0x380))
#define MDIOUSERACCESS (*(volatile unsigned *)(emacbase+0x384))
#define MDIOACK        (*(volatile unsigned *)(emacbase+0x388))
#define MDIOLINK       (*(volatile unsigned *)(emacbase+0x38C))
#define MDIOMACPHY     (*(volatile unsigned *)(emacbase+0x390))

#define SAVE1 (*(volatile unsigned *)(0x9401f000))
#define SAVE2 (*(volatile unsigned *)(0x9401f004))
#define SAVE3 (*(volatile unsigned *)(0x9401f008))
#define SAVE4 (*(volatile unsigned *)(0x9401f00c))
#define SAVE5 (*(volatile unsigned *)(0x9401f010))
#define SAVE6 (*(volatile unsigned *)(0x9401f014))
#define SAVE7 (*(volatile unsigned *)(0x9401f018))
#define SAVE8 (*(volatile unsigned *)(0x9401f01c))

#ifdef GG30E
#define	GG30E_EPLD_RESET_CTRL_REG (*(volatile short *)(0x9C800004))
#define	GG30E_EPLD_RESET_MIIA 0x0080
#define	GG30E_EPLD_RESET_MIIB 0x0040
#endif	

#define        phys(a)                 (((int)a)&~0xe0000000)
#define        virt_nc(a)              ((phys(a))|0xa0000000)

int rcb_index,rcbt_index;
bit32u PhyState;

struct txcompring_s
  {
  int status;
  int fblid_noofbufs;
  int sof_list;
  int eof_list;
  };

struct txcompring_s *TxCompPtr;
int TxCompRingMask;
int TxCompRingBase;
int txcompring_size;
int txcompring_index;

struct rxcompring_s
  {
  int status;
  int sof_list;
  int eof_list;
  int flen_errs;
  };

struct rxcompring_s *RxCompPtr;
int RxCompRingMask;
int RxCompRingBase;
int rxcompring_size;
int rxcompring_index;

volatile unsigned *StatTab; 

char *temp_RxCompPtr, *temp_TxCompPtr;

struct rcb_s
  {
  int size;
  int BufPtr;
  int Res1;
  int Res2;
  int *DatPtr;
  int Res3;
  int Res4;
  int Res5;
  }rcb_array_i[RCB_ENTRIES];

struct rcb_s *rcb_array,*CurrentRcb;

volatile struct tcb_s
  {
  int mode;
  int BufPtr;
  int Res1;
  int DO_FBLID;
  void *RcbPtr;
  int Res3;
  int Res4;
  int Res5;
  }tcb_array_i[TCB_ENTRIES];

struct tcb_s *tcb_array;

int tcb_array_index;

#define en10pages        ((en10size) >> pageshift)

octet *sed_va;                          /* virtual address of ethernet card */
eth_HwAddress sed_lclEthAddr;           /* local ethernet address */
eth_HwAddress sed_ethBcastAddr = {0xFFFF, 0xFFFF, 0xFFFF} ;         /* Ethernet broadcast address */
BOOL sed_respondARPreq;                 /* controls responses to ARP req's */
char bufAinUse, bufBinUse;              /* tell whether bufs are in use */

//static BOOL staticBufferUsage;

char abuffer[4096];

int sed_Send( pkLengthInOctets )
  register int pkLengthInOctets;
  {
  char *cp;

  pkLengthInOctets += 14;             /* account for Ethernet header */
  pkLengthInOctets = (pkLengthInOctets + 1) & (~1);

  if (pkLengthInOctets < E10P_MIN) 
    pkLengthInOctets = E10P_MIN; /* and min. ethernet len */

  cp=&abuffer[2];

eth_send((int *)virt_nc(cp), pkLengthInOctets);
  
  //IssueSend((int *)virt_nc(cp),pkLengthInOctets);
  /* Now wait for Tx completion */
  //while (TCRFREEENT == txcompring_size)
   // {
   // }
  //TCRENTINC=1;
  return(0);
  }



char *sed_FormatPacket(register unsigned char *destEAddr, unsigned long ethType)
  {
  octet *xMitBuf;
  bit32u *dp;
  int i;
    
  for(i=0,dp=(bit32u *)abuffer;i<((sizeof abuffer)/4);i++) 
    *dp++=0;

  xMitBuf = (octet *)&abuffer[2];
  memcpy(xMitBuf, destEAddr, 6);
  memcpy(xMitBuf + 6, sed_lclEthAddr, 6);
  *((short *)(xMitBuf+12)) = wfix(ethType);
  return(xMitBuf+14);
  }


octet *
sed_IsPacket()
  {
  octet *pb;
    
  pb= NULL; //GetRxPacket();
  if (pb)
    {
    pb+=14;
    }
  return(pb);
  }


/* 
 *  Check to make sure that the packet that you received was the one that
 * you expected to get.
 */
int sed_CheckPacket(word *recBufLocation, word expectedType)
  {
  if ( recBufLocation[-1] != wfix(expectedType) ) 
    return(0);
   else
    return(1);
  }

#if 0

/* 
 *  Initialize the Ethernet Interface, and this package.  Enable input on
 * both buffers.
 */

void stop(char *str)
  {
  sys_printf("ERROR: %s.\n",str);
  //ExitCall(1);            //mask by xgl 2008 8 11
  }

void InitRcb(int size)
  {
  int i;
  int *pTmp;

  rcb_index=0;
  rcbt_index=0;
  rcb_array=(struct rcb_s *)virt_nc(&rcb_array_i[0]);
  for(i=0;i<RCB_ENTRIES;i++)
    {
    pTmp=_malloc(size);
    if (!pTmp) 
      stop("Cannot allocate memory for RCBs");
    rcb_array[i].size=0;
    rcb_array[i].BufPtr=phys(pTmp);
    rcb_array[i].Res1=0;
    rcb_array[i].Res2=0;
    rcb_array[i].DatPtr=(int *)virt_nc(pTmp);
    }
  SAVE4=(int)rcb_array;
  }

void init_txcomp(int num)
  {
  int *pTmp,iTmp,i,j;

  /* Make num power of two */

  num*=2;
  num--;
  for(i=0,j=0x80000000;i<32;i++,j>>=1)
    if (j&num) num&=j;
  txcompring_size=num;

  /* Allocate so that it is on a power of size alignment */

  iTmp=num*16*2;
  pTmp=_malloc(iTmp);
  temp_TxCompPtr = (char*)pTmp;
  if (!pTmp) 
    stop("Cannot allocate memory for Tx Completion Ring");
  iTmp=(int)virt_nc(pTmp);
  iTmp+=((num*16)-1);
  iTmp&=~((num*16)-1);
  SAVE2=iTmp;
  pTmp=(int *)iTmp;
  TxCompPtr=(struct txcompring_s *)pTmp;
  for(i=0;i<num;i++)
    {
    *pTmp++=0;
    *pTmp++=0;
    *pTmp++=0;
    *pTmp++=0;
    }

  /* allocate Tx completion ring (num entries) */

  TCRPTR    = phys(TxCompPtr);      
  TCRSIZE   = num-1;
  TCRTOTENT = num;         
  TCRENTINC = num; 
  TxCompRingMask=(num-1)<<4;
  TxCompRingBase=(~(((num-1)<<4)|0xf))&((int)TxCompPtr);
  }

void init_rxcomp(int num)
  {
  int *pTmp,iTmp,i,j;

  /* Make num power of two */

  num*=2;
  num--;
  for(i=0,j=0x80000000;i<32;i++,j>>=1)
    if (j&num) num&=j;
  rxcompring_size=num;

  /* Allocate so that it is on a power of size alignment */

  iTmp=num*16*2;
  pTmp=_malloc(iTmp);
  temp_RxCompPtr = (char*)pTmp;
  if (!pTmp) 
    stop("Cannot allocate memory for Rx Completion Ring");
  iTmp=(int)virt_nc(pTmp);
  iTmp+=((num*16)-1);
  iTmp&=~((num*16)-1);
  SAVE3=iTmp;
  pTmp=(int *)iTmp;
  RxCompPtr=(struct rxcompring_s *)pTmp;
  for(i=0;i<num;i++)
    {
    *pTmp++=0;
    *pTmp++=0;
    *pTmp++=0;
    *pTmp++=0;
    }

  /* allocate Tx completion ring (num entries) */

  RCRPTR    = phys(RxCompPtr);      
  RCRSIZE   = num-1;
  RCRTOTENT = num;         
  RCRENTINC = num; 
  RxCompRingMask=(num-1)<<4;
  RxCompRingBase=(~(((num-1)<<4)|0xf))&((int)RxCompPtr);
  }

void RxFree(struct rcb_s *rcb)
  {

  rcb->Res1=0;
  rcb->Res2=0;

  while((FBLADDCNT&0x80000000)==0)
    {
    }

  FBLADDCNT = 1;  
  FBLADDSTART = phys(rcb);
  FBLADDEND   = phys(rcb);
  }

#if 0
void clockwait(unsigned int et)
  {
  unsigned int ct;

  et/=2;
  t_clear();
  do
    {
    ct=t_get();
    }while(ct<et);
  }
#endif

void InitTcb()
  {
  int i;

  tcb_array_index=0;
  tcb_array=(struct tcb_s *)virt_nc(&tcb_array_i[0]);
  for(i=0;i<TCB_ENTRIES;i++)
    {
    tcb_array[i].mode=0;
    tcb_array[i].BufPtr=0;
    tcb_array[i].Res1=0;
    tcb_array[i].DO_FBLID=0;
    tcb_array[i].RcbPtr=0;
    }
  SAVE1=(int)tcb_array;
  }

/* This function starts the send process: used by sed_Send */
void IssueSend(volatile int *buffer,int size)
  {
  int itmp;
  struct tcb_s *tcb_ptr;

  size+=4; /* Deal with CRC padding */

  itmp=tcb_array_index++; 
  if (tcb_array_index>=TCB_ENTRIES) 
    tcb_array_index=0;

  tcb_array[itmp].mode=(size+4)|0xc0000000;
  tcb_array[itmp].BufPtr=phys((int *)buffer);
  tcb_array[itmp].DO_FBLID=(((bit32u)buffer)&3)<<16;
  tcb_ptr=&tcb_array[itmp];

  while((TXPADDCNT&0x80000000)==0)
    {
    }

  TXPADDCNT = 1;    
  TXPADDSTART = phys(tcb_ptr);
  TXPADDEND   = phys(tcb_ptr);
  }

void sed_shutdown()
  {
    /*
     * Free the Rx buffers 
     */

    int i;          
    for(i=0;i<RCB_ENTRIES;i++)
    {
        _free((void*)KSEG0(rcb_array[i].DatPtr));
    }
    _free((void*)KSEG0(temp_TxCompPtr));
    _free((void*)KSEG0(temp_RxCompPtr));
    
	/* Take the MAC out of reset */
	if(emacbase ==  0xa8610000)
	{
		PRSTCTRL  &= ~(1<<EMACA_RESET_BIT);
	}
	else
	{
  		PRSTCTRL  &= ~(1<<EMACB_RESET_BIT);
	}

#ifdef GG30E
	/* Put the PHY in reset */
	if(emacbase ==  0xa8610000)
	{
		GG30E_EPLD_RESET_CTRL_REG &= ~GG30E_EPLD_RESET_MIIA;
	}
	else
	{
		GG30E_EPLD_RESET_CTRL_REG &= ~GG30E_EPLD_RESET_MIIB;
	}
#endif	

  }


int sed_Init(BOOL useStaticBuffers)
  {
  register int i,EmacDuplex,EmacSpeed,PhyNum;
  bit32u uitmp;
  char *cp;
  int time_out;

  staticBufferUsage = useStaticBuffers;
  
#if EMAC_DEBUG
  DispStr("EmacInit");  
  sys_printf("sed_Init();\n"); 
#endif
    
#ifdef GG30E
	/* Take the PHY out of reset */
	if(emacbase ==  0xa8610000)
	{
		GG30E_EPLD_RESET_CTRL_REG |= GG30E_EPLD_RESET_MIIA;
	}
	else
	{
		GG30E_EPLD_RESET_CTRL_REG |= GG30E_EPLD_RESET_MIIB;
	}
#endif	
    
  /* accept packets addressed for us and broadcast packets */

  InitTcb();
  InitRcb(BUFFER_SIZE);    /* Initialize RCB with 1518 byte buffers */


	/* Take the MAC out of reset */
	if(emacbase ==  0xa8610000)
	{
		PRSTCTRL  &= ~(1<<EMACA_RESET_BIT);
  		delay_usecs(200);
  		PRSTCTRL  |= (1<<EMACA_RESET_BIT);
  		delay_usecs(200);
	}
	else
	{
  		PRSTCTRL  &= ~(1<<EMACB_RESET_BIT);
  		delay_usecs(200);
  		PRSTCTRL  |= (1<<EMACB_RESET_BIT);
  		delay_usecs(200);
	}

  /* Reset the mac */

  DMACONFIG = 0x80000000;
  DMACONFIG = 0x00000000;

  /* allocate Tx completion ring (TX_COMP_ENTRIES entries) */

  init_txcomp(TX_COMP_ENTRIES);

  /* Set DMA burst length to 16 words */

  TDMASTATE7 = 0x00100000; 

  /* allocate Rx completion ring (four entries) */

  init_rxcomp(RX_COMP_ENTRIES);

  /* Set Rx DMA state stuff */

  RDMASTATE0 = 0x00000200; 

  /* Set Rx Buffer Size */

  FBL0BUFSIZE = BUFFER_SIZE;

  /* Initialize MDIO Phy State machine */

  PhyState=0;
  EmacMdioInit(emacbase,&PhyState,_CpuFrequency,FALSE);

  DispStr("Phy Wait");
  time_out = 200;
  
  while(!EmacMdioGetLinked(emacbase,&PhyState))
    {
        delay_usecs(10000); /* delay 10 Ms */
        /* EmacMdioTic(emacbase,&PhyState); */
        time_out--;
        if(time_out < 0) {
            sys_printf("Ethernet Link Down.\n");
            return -1;
        }                
    }

  EmacDuplex=EmacMdioGetDuplex(emacbase,&PhyState);
  EmacSpeed=EmacMdioGetSpeed(emacbase,&PhyState);
  PhyNum=EmacMdioGetPhyNum(emacbase,&PhyState);

#if EMAC_DEBUG
  sys_printf("Phy= %d, Speed=%s, Duplex=%s\n",PhyNum,(EmacSpeed)?"100":"10",(EmacDuplex)?"Full":"Half");
#endif
  /* Enable Rx and Tx DMA, and unreset MAC */

  DMACONFIG = 0x00000003;

  /* Set Copy All Frames mode */   

  INTSTS  = 0x1ff; /* Clear all pending interrupts */
 

	/* Set the MAC address for the PHY */
#if EMAC_DEBUG  
    cp=(char *)sed_lclEthAddr;
	sys_printf("Setting MAC address to: %x:%x:%x:%x:%x:%x\n",
			cp[0],cp[1],cp[2],cp[3],cp[4],cp[5]);
#endif
    
  cp=(char *)sed_lclEthAddr;
  uitmp =(((bit32u)*cp++)&0x0ff);
  uitmp|=(((bit32u)*cp++)&0x0ff)<<8;
  uitmp|=(((bit32u)*cp++)&0x0ff)<<16;
  uitmp|=(((bit32u)*cp++)&0x0ff)<<24;
  MACADDRHI=uitmp;
  uitmp =(((bit32u)*cp++)&0x0ff);
  uitmp|=(((bit32u)*cp++)&0x0ff)<<8;
  MACADDRLO=uitmp;

  /* Set mode of the MAC  */   
  
  MDIOMACPHY=PhyNum|0x80;

  MACCONTROL = ((EmacDuplex)?FULL_DUPLEX:0)|((EmacSpeed)?SPEED_100:0)|NO_LOOP|TXPACE|CLRP;

  for(i=0;i<RCB_ENTRIES;i++)
    RxFree(&rcb_array[i]);

  /* Now put some data in Tx buffer                   */

  return(0);
  }

/* 
 * Format an ethernet header in the transmit buffer, and say where it is.
 * Note that because of the way the 3Com interface works, we need to know
 * how long the packet is before we know where to put it.  The solution is
 * that we format the packet at the BEGINNING of the transmit buffer, and
 * later copy it (carefully) to where it belongs.  Another hack would be
 * to be inefficient about the size of the packet to be sent (always send
 * a larger ethernet packet than you need to, but copying should be ok for
 * now.
 */

char abuffer[4096];

octet *
sed_FormatPacket( destEAddr, ethType )
        register octet *destEAddr;
  {
  octet *xMitBuf;
  bit32u *dp;
  int i;
    
  for(i=0,dp=(bit32u *)abuffer;i<((sizeof abuffer)/4);i++) 
    *dp++=0;

  xMitBuf = (octet *)&abuffer[2];
  memcpy(xMitBuf, destEAddr, 6);
  memcpy(xMitBuf + 6, sed_lclEthAddr, 6);
  *((short *)(xMitBuf+12)) = wfix(ethType);
  return(xMitBuf+14);
  }

/*
 *  Send a packet out over the ethernet.  The packet is sitting at the
 * beginning of the transmit buffer.  The routine returns when the
 * packet has been successfully sent.
 */
int sed_Send( pkLengthInOctets )
  register int pkLengthInOctets;
  {
  char *cp;

  pkLengthInOctets += 14;             /* account for Ethernet header */
  pkLengthInOctets = (pkLengthInOctets + 1) & (~1);

  if (pkLengthInOctets < E10P_MIN) 
    pkLengthInOctets = E10P_MIN; /* and min. ethernet len */

  cp=&abuffer[2];
  IssueSend((int *)virt_nc(cp),pkLengthInOctets);
  /* Now wait for Tx completion */
  while (TCRFREEENT == txcompring_size)
    {
    }
  TCRENTINC=1;
  return(0);
  }

/*
 * The following three functions are added to provide the interface to
 * tftp module
 */
int cpmac_tx(char *buff, unsigned int pkt_size)
{
	
	/* before send add 2 to buffer */
    buff += 2;
	
    if(pkt_size < E10P_MIN) {
        pkt_size = E10P_MIN;
    }
    
    /* trigger the send */
    IssueSend((int *)virt_nc(buff),pkt_size);

    /* Now wait for Tx completion */
    while (TCRFREEENT == txcompring_size)
    {
    }
    TCRENTINC=1;
    
    if(!staticBufferUsage) {
        _free((void *)KSEG0(buff - 2));
    }
    
#if CPMAC_DEBUG
    sys_printf("\nPacket at 0x%08x  xmt success.\n", buff);
#endif    
    return(SBL_SUCCESS);
}

unsigned int glob_recv_len;
Status cpmac_rx(char **buf, unsigned int *len) 
{
    *buf = GetRxPacket();
    if(*buf == NULL) {
        return SBL_ETIMEOUT;
    }
    *len = glob_recv_len;
    return SBL_SUCCESS;
}

Status cpmac_init(BOOL useStaticBuffers) {
   
   volatile int i;
   
   memcpy(sed_lclEthAddr, sys_et_addr, sizeof(eth_HwAddress));
   if (sed_Init(useStaticBuffers) == -1) {
        return SBL_EFAILURE;
   }
   
   for(i=0; i< 100000; i++);
   
   return SBL_SUCCESS;
}

void lkdn_banner(void)
{
	sys_printf("Error: Link down\n");
}

/* 
 * Test for the arrival of a packet on the Ethernet interface.  The packet may
 * arrive in either buffer A or buffer B; the location of the packet is
 * returned.  If no packet is returned withing 'timeout' milliseconds,
 * then the routine returns zero.
 * 
 * Note: ignores ethernet errors.  may occasionally return something
 * which was received in error.
 */

octet *
sed_IsPacket()
  {
  octet *pb;
    
  pb=GetRxPacket();
  if (pb)
    {
    pb+=14;
    }
  return(pb);
  }

/* 
 *  Check to make sure that the packet that you received was the one that
 * you expected to get.
 */
int sed_CheckPacket( recBufLocation, expectedType )
    word *recBufLocation;
    word expectedType;
  {

  if ( recBufLocation[-1] != wfix(expectedType) ) 
    return(0);
   else
    return(1);
  }

char ubuff[2048];

void *GetRxPacket(void)
  {
  bit32u uitmpsts,rlen = 0;
  char *uipbufrtn;

  uipbufrtn=0;
  do
    {
    if (RCRPENDENT)
      {
      uitmpsts=RxCompPtr->status;
      CurrentRcb=(struct rcb_s *)virt_nc(RxCompPtr->sof_list);
      rlen=CurrentRcb->size&0x0ffff;
      if (((uitmpsts&0xc0000000)==0x80000000)&&(rlen>=60)&&(rlen<=1514))
        { /* Good frame */
        memcpy(&ubuff[2],CurrentRcb->DatPtr,rlen);
        uipbufrtn=&ubuff[2];
        }
      if (uitmpsts&0x80000000)
        {
        RxFree(CurrentRcb);                         
        }
      RxCompPtr=((struct rxcompring_s *) ((((int)(RxCompPtr+1))&RxCompRingMask)|(RxCompRingBase)));
      RCRENTINC=1;
      }
    }while((RCRPENDENT)&&(uipbufrtn==0));
  glob_recv_len = rlen;
  return(uipbufrtn);
  }

//#include "emacphy.c"


#endif

