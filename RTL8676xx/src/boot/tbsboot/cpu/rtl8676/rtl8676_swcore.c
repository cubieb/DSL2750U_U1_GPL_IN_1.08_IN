#include <common.h>
#include <net.h>
#include <command.h>
#include <malloc.h>
#include "rtl8676_swmain.h"
#include "rtl8676_asicregs.h"
#include "rtl8676_swcore.h"
#include "rtl8676_phy.h"

#define WRITE_MEM32(addr, val)   (*(volatile unsigned int *) (addr)) = (val)
#define WRITE_MEM16(addr, val)   (*(volatile unsigned short *) (addr)) = (val)
#define READ_MEM32(addr)         (*(volatile unsigned int *) (addr))

#if CONFIG_RTL865XC
static unsigned char fidHashTable[]={0x00,0x0f,0xf0,0xff};

/*#define rtl8651_asicTableAccessAddrBase(type) (RTL8651_ASICTABLE_BASE_OF_ALL_TABLES + 0x10000 * (type)) */
#define		RTL8651_ASICTABLE_BASE_OF_ALL_TABLES		0xBB000000
#define		rtl8651_asicTableAccessAddrBase(type) (RTL8651_ASICTABLE_BASE_OF_ALL_TABLES + ((type)<<16) )
#define 	RTL865X_FAST_ASIC_ACCESS
#define		RTL865XC_ASIC_WRITE_PROTECTION				/* Enable/Disable ASIC write protection */
#define		RTL8651_ASICTABLE_ENTRY_LENGTH (8 * sizeof(u32))
#define		RTL865X_TLU_BUG_FIXED		1


#ifdef RTL865X_FAST_ASIC_ACCESS
static u32 _rtl8651_asicTableSize[] =
{
	2 /*TYPE_L2_SWITCH_TABLE*/,
	1 /*TYPE_ARP_TABLE*/,
    2 /*TYPE_L3_ROUTING_TABLE*/,
	3 /*TYPE_MULTICAST_TABLE*/,
	1 /*TYPE_PROTOCOL_TRAP_TABLE*/,
	5 /*TYPE_VLAN_TABLE*/,
	3 /*TYPE_EXT_INT_IP_TABLE*/,
    1 /*TYPE_ALG_TABLE*/,
    4 /*TYPE_SERVER_PORT_TABLE*/,
    3 /*TYPE_L4_TCP_UDP_TABLE*/,
    3 /*TYPE_L4_ICMP_TABLE*/,
    1 /*TYPE_PPPOE_TABLE*/,
    8 /*TYPE_ACL_RULE_TABLE*/,
    1 /*TYPE_NEXT_HOP_TABLE*/,
    3 /*TYPE_RATE_LIMIT_TABLE*/,
};
#endif

static void _rtl8651_asicTableAccessForward(u32 tableType, u32 eidx, void *entryContent_P) {

	while ( (READ_MEM32(SWTACR) & ACTION_MASK) != ACTION_DONE );//Wait for command done

#ifdef RTL865X_FAST_ASIC_ACCESS

	{
		register u32 index;

		for( index = 0; index < _rtl8651_asicTableSize[tableType]; index++ )
		{
			WRITE_MEM32(TCR0+(index<<2), *((u32 *)entryContent_P + index));
		}

	}
#else
	WRITE_MEM32(TCR0, *((u32 *)entryContent_P + 0));
	WRITE_MEM32(TCR1, *((u32 *)entryContent_P + 1));
	WRITE_MEM32(TCR2, *((u32 *)entryContent_P + 2));
	WRITE_MEM32(TCR3, *((u32 *)entryContent_P + 3));
	WRITE_MEM32(TCR4, *((u32 *)entryContent_P + 4));
	WRITE_MEM32(TCR5, *((u32 *)entryContent_P + 5));
	WRITE_MEM32(TCR6, *((u32 *)entryContent_P + 6));
	WRITE_MEM32(TCR7, *((u32 *)entryContent_P + 7));
#endif	
	WRITE_MEM32(SWTAA, ((u32) rtl8651_asicTableAccessAddrBase(tableType) + eidx * RTL8651_ASICTABLE_ENTRY_LENGTH));//Fill address
}

static int _rtl8651_forceAddAsicEntry(u32 tableType, u32 eidx, void *entryContent_P) {

	#ifdef RTL865XC_ASIC_WRITE_PROTECTION
	if (RTL865X_TLU_BUG_FIXED)	/* No need to stop HW table lookup process */
	{	/* No need to stop HW table lookup process */
		WRITE_MEM32(SWTCR0,EN_STOP_TLU|READ_MEM32(SWTCR0));
		while ( (READ_MEM32(SWTCR0) & STOP_TLU_READY)==0);
	}
	#endif

	_rtl8651_asicTableAccessForward(tableType, eidx, entryContent_P);

 	WRITE_MEM32(SWTACR, ACTION_START | CMD_FORCE);//Activate add command
	while ( (READ_MEM32(SWTACR) & ACTION_MASK) != ACTION_DONE );//Wait for command done

	#ifdef RTL865XC_ASIC_WRITE_PROTECTION
	if (RTL865X_TLU_BUG_FIXED)	/* No need to stop HW table lookup process */
	{
		WRITE_MEM32(SWTCR0,~EN_STOP_TLU&READ_MEM32(SWTCR0));
	}
	#endif

	return 0;
}

u32 rtl8651_filterDbIndex(ether_addr_t * macAddr,unsigned short fid) {
    return ( macAddr->octet[0] ^ macAddr->octet[1] ^
                    macAddr->octet[2] ^ macAddr->octet[3] ^
                    macAddr->octet[4] ^ macAddr->octet[5] ^fidHashTable[fid]) & 0xFF;
}

static int rtl8651_setAsicL2Table(ether_addr_t	*mac, u32 column)
{
	rtl865xc_tblAsic_l2Table_t entry;
	u32	row;

	row = rtl8651_filterDbIndex(mac, 0);
	if((row >= RTL8651_L2TBL_ROW) || (column >= RTL8651_L2TBL_COLUMN))
		return -1;
	if(mac->octet[5] != ((row^(fidHashTable[0])^ mac->octet[0] ^ mac->octet[1] ^ mac->octet[2] ^ mac->octet[3] ^ mac->octet[4] ) & 0xff))
		return -1;

	memset(&entry, 0,sizeof(entry));
	entry.mac47_40 = mac->octet[0];
	entry.mac39_24 = (mac->octet[1] << 8) | mac->octet[2];
	entry.mac23_8 = (mac->octet[3] << 8) | mac->octet[4];

//	entry.extMemberPort = 0;   
	entry.memberPort = 7;
	entry.toCPU = 1;
	entry.isStatic = 1;
//	entry.nxtHostFlag = 1;

	/* RTL865xC: modification of age from ( 2 -> 3 -> 1 -> 0 ) to ( 3 -> 2 -> 1 -> 0 ). modification of granularity 100 sec to 150 sec. */
	entry.agingTime = 0x03;
	
//	entry.srcBlock = 0;
	entry.fid=0;
	entry.auth=1;

	return _rtl8651_forceAddAsicEntry(TYPE_L2_SWITCH_TABLE, row<<2 | column, &entry);
}
#endif

//------------------------------------------------------------------------
static void _rtl8651_clearSpecifiedAsicTable(u32 type, u32 count) 
{
	extern int swTable_addEntry(u32 tableType, u32 eidx, void *entryContent_P);
	struct { u32 _content[8]; } entry;
	u32 idx;
	
	memset(&entry, 0, sizeof(entry));
	for (idx=0; idx<count; idx++)// Write into hardware
		swTable_addEntry(type, idx, &entry);
}

void FullAndSemiReset( void )
{
	/* Perform full-reset for sw-core. */ 
	REG32(SIRR) |= FULL_RST;
	delay_msec(50);

	/* Enable TRXRDY */
	REG32(SIRR) |= TRXRDY;
}

int rtl865xC_setAsicEthernetMIIMode(u32 port, u32 mode)
{
	if ( port != 0 && port != RTL8651_MII_PORTNUMBER )
		return -1;
	if ( mode != LINK_RGMII && mode != LINK_MII_MAC && mode != LINK_MII_PHY )
		return -1;

	if ( port == 0 )
	{
		/* MII port MAC interface mode configuration */
		WRITE_MEM32( P0GMIICR, ( READ_MEM32( P0GMIICR ) & ~CFG_GMAC_MASK ) | ( mode << LINKMODE_OFFSET ) );
	}
	else
	{
		/* MII port MAC interface mode configuration */
		WRITE_MEM32( P5GMIICR, ( READ_MEM32( P5GMIICR ) & ~CFG_GMAC_MASK ) | ( mode << LINKMODE_OFFSET ) );
	}
	return 0;

}

int rtl865xC_setAsicEthernetRGMIITiming(u32 port, u32 Tcomp, u32 Rcomp)
{
	if ( port != 0 && port != RTL8651_MII_PORTNUMBER )
		return -1;
	if ( Tcomp < RGMII_TCOMP_0NS || Tcomp > RGMII_TCOMP_2NS || Rcomp < RGMII_RCOMP_0NS || Rcomp > RGMII_RCOMP_3DOT5NS )
		return -1;
	
	if ( port == 0 )
	{
		WRITE_MEM32(P0GMIICR, ( ( ( READ_MEM32(P0GMIICR) & ~RGMII_TCOMP_MASK ) | Tcomp ) & ~RGMII_RCOMP_MASK ) | Rcomp );
	}
	else
	{
		WRITE_MEM32(P5GMIICR, ( ( ( READ_MEM32(P5GMIICR) & ~RGMII_TCOMP_MASK ) | Tcomp ) & ~RGMII_RCOMP_MASK ) | Rcomp );
	}

	WRITE_MEM32(P0GMIICR, READ_MEM32(P0GMIICR)|Conf_done);

	return 0;
}


int rtl8651_getAsicEthernetPHYReg(u32 phyId, u32 regId, u32 *rData)
{
	u32 status;

	WRITE_MEM32( MDCIOCR, COMMAND_READ | ( phyId << PHYADD_OFFSET ) | ( regId << REGADD_OFFSET ) );

#ifdef RTL865X_TEST
	status = READ_MEM32( MDCIOSR );
#else
#if defined(CONFIG_RTL8196C)
	delay_msec(10);   //wei add, for 8196C_test chip patch. mdio data read will delay 1 mdc clock.
#endif
	do { status = READ_MEM32( MDCIOSR ); } while ( ( status & STATUS ) != 0 );
#endif

	status &= 0xffff;
	*rData = status;

	return 0;
}

int rtl8651_setAsicEthernetPHYReg(u32 phyId, u32 regId, u32 wData)
{
	WRITE_MEM32( MDCIOCR, COMMAND_WRITE | ( phyId << PHYADD_OFFSET ) | ( regId << REGADD_OFFSET ) | wData );

#ifdef RTL865X_TEST
#else
	while( ( READ_MEM32( MDCIOSR ) & STATUS ) != 0 );		/* wait until command complete */
#endif

	return 0;
}

int rtl8651_restartAsicEthernetPHYNway(u32 port, u32 phyid)
{
	u32 statCtrlReg0;

	/* read current PHY reg 0 */
	rtl8651_getAsicEthernetPHYReg( phyid, 0, &statCtrlReg0 );

	/* enable 'restart Nway' bit */
	statCtrlReg0 |= RESTART_AUTONEGO;

	/* write PHY reg 0 */
	rtl8651_setAsicEthernetPHYReg( phyid, 0, statCtrlReg0 );

	return 0;
}

#ifdef 	EXTERNAL_GIGAPHY
//====================================================================

int RTL8367B_probe(unsigned int phyid)
{
	extern int RL6000_write(unsigned int addr, unsigned int data);
	extern int RL6000_read(unsigned int addr, unsigned int *data);
	extern void RL6000_RGMII(void);
	unsigned int id = 0;

	//set port0 RGMII mode
	rtl865xC_setAsicEthernetMIIMode(0, LINK_RGMII);
	//set Port 0 Interface Type Configuration to GMII/MII/RGMII interface 
	WRITE_MEM32(PITCR, (READ_MEM32(PITCR)&0xfffffffc)|Port0_TypeCfg_GMII_MII_RGMII);
	//set external phyid
	WRITE_MEM32(PCRP0, (READ_MEM32(PCRP0)&(~ExtPHYID_MASK))|(phyid << ExtPHYID_OFFSET));
	
	RL6000_write(0x13C2, 0x0249);
	RL6000_read(0x1300, &id);

	if (!id || (0xffff == (id & 0xffff)))
		goto probe_fail;

	rtl865xC_setAsicEthernetRGMIITiming(0, RGMII_TCOMP_2NS, RGMII_RCOMP_1NS);
	WRITE_MEM32(MACCR,(READ_MEM32(MACCR)&0xffffcfff)|0x01<<12);//select 100Mhz system clk
	
	//disable auto-polling
	WRITE_MEM32(PCRP0, (READ_MEM32(PCRP0)&~PollLinkStatus));
	WRITE_MEM32(PCRP1, (READ_MEM32(PCRP1)&~PollLinkStatus));
	WRITE_MEM32(PCRP2, (READ_MEM32(PCRP2)&~PollLinkStatus));
	WRITE_MEM32(PCRP3, (READ_MEM32(PCRP3)&~PollLinkStatus));
	WRITE_MEM32(PCRP4, (READ_MEM32(PCRP4)&~PollLinkStatus));

	//set port0 force mode
	WRITE_MEM32(PCRP0, (READ_MEM32(PCRP0)&(~ForceSpeedMask)&(~ExtPHYID_MASK))|ForceSpeed1000M|EnForceMode);
	mdelay(10);

	//port0 force link
	WRITE_MEM32(PCRP0, READ_MEM32(PCRP0)|ForceLink);

	rtk_switch_init();

	RL6000_RGMII();

	printf("RTL8367B is detected! \n\r");
	return 0;

probe_fail:
	FullAndSemiReset();
	rtl8651_setAsicEthernetPHYReg(0, 31, 0);	//select back to page 0
	return -1;
}

int RTL8211E_probe(unsigned int phyid)
{
	unsigned int uid, tmp;

	//set port0 RGMII mode
	rtl865xC_setAsicEthernetMIIMode(0, LINK_RGMII);
	//set Port 0 Interface Type Configuration to GMII/MII/RGMII interface 
	WRITE_MEM32(PITCR, (READ_MEM32(PITCR)&0xfffffffc)|Port0_TypeCfg_GMII_MII_RGMII);
	//set external phyid
	WRITE_MEM32(PCRP0, (READ_MEM32(PCRP0)&(~ExtPHYID_MASK))|(phyid << ExtPHYID_OFFSET));
	
	rtl8651_getAsicEthernetPHYReg(phyid, 2, &tmp);
	uid = tmp << 16;
	rtl8651_getAsicEthernetPHYReg(phyid, 3, &tmp);
	uid = uid | tmp;

	if (uid != 0x001cc915)
		goto probe_fail;
	
	/* LED setting */
	//select page 7
	rtl8651_setAsicEthernetPHYReg(phyid, 31, 7);
	//extension page 44
	rtl8651_setAsicEthernetPHYReg(phyid, 30, 0x2c);
	//Register 28 bit[2:0] = 111 (set LED 0 On when 10/100/1000Link)
	rtl8651_getAsicEthernetPHYReg(phyid, 28, &tmp);
	tmp |= 0x7;
	rtl8651_setAsicEthernetPHYReg(phyid, 28, tmp);
	//Register 26 bit[4] = 1 (set LED 0 blinking when Activity)
	rtl8651_getAsicEthernetPHYReg(phyid, 26, &tmp);
	tmp |= 0x10;
	rtl8651_setAsicEthernetPHYReg(phyid, 26, tmp);
	//select page 0
	rtl8651_setAsicEthernetPHYReg(phyid, 31, 0);

	rtl865xC_setAsicEthernetRGMIITiming(0, RGMII_TCOMP_2NS, RGMII_RCOMP_1NS);
	WRITE_MEM32(MACCR,(READ_MEM32(MACCR)&0xffffcfff)|0x01<<12);//select 100Mhz system clk

	printf("RTL8211E is detected! \n\r");
	return 0;

probe_fail:
	FullAndSemiReset();
	return -1;
}

//cathy, interface with phyid 0 (ex: RTL8367B) should be probed at the last in the list !!!!
struct external_giga extGigaList[] = {
	{RTL8211E, RTL8211E_PHYID, RTL8211E_probe},
	{RTL8367B, RTL8367B_PHYID, RTL8367B_probe},
	{-1, -1, NULL}
};
#endif

int swCore_init(volatile unsigned char* mac)
{
	int port = 0;
	int extInf = EXT_NONE;
	u32 val;
	#ifdef 	EXTERNAL_GIGAPHY
	struct external_giga *extGiga;
	#endif

	if(REG32(0xB8000224) >> 16 == 0x0412)	//cathy, enable 25MHz clock out for RTL8211E + RLE0412 only
		{
		REG32(BSP_MISC_PINMUX) |= BSP_CK25M_UR0;
		}

	/* Full reset and semreset */
	FullAndSemiReset();	
#ifdef 	EXTERNAL_GIGAPHY
	for(extGiga = &extGigaList[0]; extGiga->probeFn != NULL; extGiga++)
		{
		if(0 == extGiga->probeFn(extGiga->extPhyId))
			{
			extInf = extGiga->inf;
			break;
			}
		}
#endif
	if(EXT_NONE != extInf)
		{
		port = 1;
		}
	else		
		{
		etdebug("Initializing internal PHY\n\r");
		val = REG32(BSP_MISC_PINMUX);
		/* Enable Ethernet LED */
		val |= (BSP_LEDS0_GPH4	|				//0:gpio H4, 1:LEDS0
				BSP_LEDS1_GPH5	|				//0:gpio H5, 1:LEDS1
			    BSP_LEDS2_GPH6	|				//0:gpio H6, 1:LEDS2
		    	BSP_LEDS3_GPH7	|				//0:gpio H7, 1:LEDS3
			    BSP_LEDP0_GPF0);                //0:gpio F0, 1:LEDF0
		REGWRITE32(BSP_MISC_PINMUX, val);
		}

	if((RTL8211E != extInf) && (REG32(0xB8000224) >> 16 == 0x0412))	//cathy, enable 25MHz clock out for RTL8211E + RLE0412 only
		REG32(BSP_MISC_PINMUX) &= ~BSP_CK25M_UR0;

	/* rtl8651_clearAsicAllTable */
	REG32(0xbb804234) = 0;
	REG32(0xbb804234) = 0x7f;	//initialize all hwnat tables excluding multicast and net interface table

	_rtl8651_clearSpecifiedAsicTable(TYPE_MULTICAST_TABLE, RTL8651_IPMULTICASTTBL_SIZE);
	_rtl8651_clearSpecifiedAsicTable(TYPE_NETINTERFACE_TABLE, RTL865XC_NETINTERFACE_NUMBER);

	REG32(PCRP0) |= AcptMaxLen_16K | EnablePHYIf | MacSwReset;
	REG32(PCRP1) |= (1 << ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf | MacSwReset;
	REG32(PCRP2) |= (2 << ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf | MacSwReset;
	REG32(PCRP3) |= (3 << ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf | MacSwReset;
	REG32(PCRP4) |= (4 << ExtPHYID_OFFSET) | AcptMaxLen_16K | EnablePHYIf | MacSwReset;

	/* Set PVID of all ports to 8 */
	REG32(PVCR0) = (0x8 << 16) | 0x8;
	REG32(PVCR1) = (0x8 << 16) | 0x8;
	REG32(PVCR2) = (0x8 << 16) | 0x8;
	REG32(PVCR3) = (0x8 << 16) | 0x8;
	
	/* Enable L2 lookup engine and spanning tree functionality */
	// REG32(MSCR) = EN_L2 | EN_L3 | EN_L4 | EN_IN_ACL;
	REG32(MSCR) = EN_L2;
	REG32(QNUMCR) = P0QNum_1 | P1QNum_1 | P2QNum_1 | P3QNum_1 | P4QNum_1;

	/* Start normal TX and RX */
	REG32(SIRR) |= TRXRDY;
	
	/*PHY FlowControl. Default enable*/
	for(; port < MAX_PORT_NUMBER; port++)
		{/* Set Flow Control capability. */
		rtl8651_restartAsicEthernetPHYNway(port+1, port);
		rtl8651_setAsicEthernetPHYReg(port, 22, 0x5bd5);
		//select page 1
		rtl8651_setAsicEthernetPHYReg(port, 31, 1);
		//ethernet 100Mhz output voltage to 1.0(v)
		rtl8651_setAsicEthernetPHYReg(port, 18, 0x9004);
		//adjust AOI waveform
		rtl8651_setAsicEthernetPHYReg(port, 19, 0x5400);
		//return to page 0
		rtl8651_setAsicEthernetPHYReg(port, 31, 0);
	}	

	rtl8651_setAsicL2Table((ether_addr_t*)mac, 0); // by xiachaoren 2011-11-28	
	REG32(FFCR) = EN_UNUNICAST_TOCPU | EN_UNMCAST_TOCPU; // rx broadcast and unicast packet

	return 0;
}


