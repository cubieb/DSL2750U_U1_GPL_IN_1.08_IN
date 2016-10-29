/******************************************************************************

                               Copyright (c) 2006 
                            Infineon Technologies AG 
                     Am Campeon 1-12; 81726 Munich, Germany 

  THE DELIVERY OF THIS SOFTWARE AS WELL AS THE HEREBY GRANTED NON-EXCLUSIVE, 
  WORLDWIDE LICENSE TO USE, COPY, MODIFY, DISTRIBUTE AND SUBLICENSE THIS 
  SOFTWARE IS FREE OF CHARGE. 

  THE LICENSED SOFTWARE IS PROVIDED "AS IS" AND INFINEON EXPRESSLY DISCLAIMS 
  ALL REPRESENTATIONS AND WARRANTIES, WHETHER EXPRESS OR IMPLIED, INCLUDING 
  WITHOUT LIMITATION, WARRANTIES OR REPRESENTATIONS OF WORKMANSHIP, 
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, DURABILITY, THAT THE 
  OPERATING OF THE LICENSED SOFTWARE WILL BE ERROR FREE OR FREE OF ANY THIRD 
  PARTY CLAIMS, INCLUDING WITHOUT LIMITATION CLAIMS OF THIRD PARTY INTELLECTUAL 
  PROPERTY INFRINGEMENT. 

  EXCEPT FOR ANY LIABILITY DUE TO WILFUL ACTS OR GROSS NEGLIGENCE AND EXCEPT 
  FOR ANY PERSONAL INJURY INFINEON SHALL IN NO EVENT BE LIABLE FOR ANY CLAIM 
  OR DAMAGES OF ANY KIND, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
  DEALINGS IN THE SOFTWARE. 

******************************************************************************/


#ifndef _VDSL2_FWDES_UTOPIAPOSPHYMSG_H_
#define _VDSL2_FWDES_UTOPIAPOSPHYMSG_H_

/** \file
   
*/

#ifndef __PACKED__
#if defined (__GNUC__) || defined (__GNUG__)
   /* GNU C or C++ compiler */
   #define __PACKED__ __attribute__ ((packed))
#else
   /* Byte alignment adjustment */
   #pragma pack(1)
      #if !defined (__PACKED__)
   #define __PACKED__      /* nothing */
#endif
#endif
#endif


/** @defgroup _VDSL2_FWDES_UTOPIAPOSPHYMSG_ 
 *  @{
 */

#ifdef __cplusplus
   extern "C" {
#endif

/* ----- Include section ----- */
/* ----- Include section (End) ----- */

/* ----- Define section ----- */
/* ----- Define section (End) ----- */

/**
   Defines for the SAR configuration
*/
/** LLC Encapsultion Bridged Protocol */
#define SAR_LLC_ENCAP_BP 3
/** VC Multiplexing Bridged Protocol */
#define SAR_VC_MUX_BP 1
/** Filter on Destination MAC address */
#define SAR_FILTER_MAC_ADDR 0
/** Filter on VLAN ID */
#define SAR_FILTER_VLAN_ID 1
/** Filter on VLAN Priority */
#define SAR_FILTER_VLAN_PRIO 2
/** Discard the packet on mismatch classification */
#define SAR_DISCARD_PACKET 0
/** Filter on Default VPI/VCI */
#define SAR_DEF_VPI_VCI 1


/* ----- Constants Definition section ----- */
#define  CMD_LinkControl_IFX_DISABLE  0x00000000
#define  CMD_LinkControl_IFX_ENABLE  0x00000001
#define  CMD_UTOPIA_L2_Init_BITS_8  0x00000000
#define  CMD_UTOPIA_L2_Init_BITS_16  0x00000001
#define  CMD_UTOPIA_L2_Modify_ODD_PARITY  0x00000000
#define  CMD_UTOPIA_L2_Modify_EVEN_PARITY  0x00000001
#define  CMD_UTOPIA_L2_Modify_IFX_DISABLE  0x00000000
#define  CMD_UTOPIA_L2_Modify_IFX_ENABLE  0x00000001
#define  CMD_POSPHY_L2_Modify_ODD_PARITY  0x00000000
#define  CMD_POSPHY_L2_Modify_EVEN_PARITY  0x00000001
#define  CMD_POSPHY_L2_Modify_IFX_DISABLE  0x00000000
#define  CMD_POSPHY_L2_Modify_IFX_ENABLE  0x00000001
#define  CMD_EthernetOAM_Configure_PASSIVE  0x00000000
#define  CMD_EthernetOAM_Configure_ACTIVE  0x00000001
#define  CMD_EthernetInsertExtractControl_SYSTEM_SIDE  0x00000000
#define  CMD_EthernetInsertExtractControl_LINE_SIDE  0x00000001
#define  CMD_EthernetInsertExtractControl_SYSTEM_LINE_SIDE  0x00000002
#define  CMD_EthernetExtractConfigure_FV25  25
#define  CMD_EthernetExtractConfigure_SYSTEM_SIDE  0x00000000
#define  CMD_EthernetExtractConfigure_LINE_SIDE  0x00000001
#define  CMD_EthernetExtractConfigure_SYSTEM_LINE_SIDE  0x00000002
#define  CMD_EthernetExtractConfigure_NO_FILTER  0x00000000
#define  CMD_EthernetExtractConfigure_DISCARD  0x00000001
#define  CMD_EthernetExtractConfigure_EXTRACT  0x00000000
#define  CMD_EthernetExtractConfigure_FILTER_MAC_DST_ADDR  0x00000001
#define  CMD_EthernetExtractConfigure_FILTER_MAC_SRC_ADDR  0x00000002
#define  CMD_EthernetExtractConfigure_FILTER_VLAN_ID  0x00000003
#define  CMD_EthernetExtractConfigure_FILTER_VLAN_PRIO  0x00000004
#define  ACK_EthernetExtractConfigure_FV25  25
#define  EVT_EthernetFrameLineExtract_NEXT_FRAGMENT  0
#define  EVT_EthernetFrameLineExtract_START_OF_FRAME  1
#define  EVT_EthernetFrameLineExtract_END_OF_FRAME  2
#define  EVT_EthernetFrameLineExtract_FULL_FRAME  3
#define  EVT_EthernetFrameLineExtract_FILTER1  0
#define  EVT_EthernetFrameLineExtract_FILTER2  1
#define  EVT_EthernetFrameLineExtract_OAM_FILTER  2
#define  EVT_EthernetFrameSysExtract_NEXT_FRAGMENT  0
#define  EVT_EthernetFrameSysExtract_START_OF_FRAME  1
#define  EVT_EthernetFrameSysExtract_END_OF_FRAME  2
#define  EVT_EthernetFrameSysExtract_FULL_FRAME  3
#define  EVT_EthernetFrameSysExtract_FILTER1  0
#define  EVT_EthernetFrameSysExtract_FILTER2  1
#define  EVT_EthernetFrameSysExtract_OAM_FILTER  2
/* ----- Constants Definition section (End) ----- */

/** Message ID for CMD_LinkControl */
#define CMD_LINKCONTROL 0x0151

/**
   This command is used to enable/disable the transmit and receive flow at the
   UTOPIA/POS-PHY interface.
*/
typedef struct CMD_LinkControl CMD_LinkControl_t;

/** Message ID for ACK_LinkControl */
#define ACK_LINKCONTROL 0x0151

/**
   
*/
typedef struct ACK_LinkControl ACK_LinkControl_t;

/** Message ID for CMD_UTOPIA_L2_Init */
#define CMD_UTOPIA_L2_INIT 0x0251

/**
   This command is used to configure the UTOPIA interface.
*/
typedef struct CMD_UTOPIA_L2_Init CMD_UTOPIA_L2_Init_t;

/** Message ID for ACK_UTOPIA_L2_Init */
#define ACK_UTOPIA_L2_INIT 0x0251

/**
   
*/
typedef struct ACK_UTOPIA_L2_Init ACK_UTOPIA_L2_Init_t;

/** Message ID for CMD_UTOPIA_L2_Modify */
#define CMD_UTOPIA_L2_MODIFY 0x0351

/**
   This command is used to modify the default settings of the UTOPIA interface.
*/
typedef struct CMD_UTOPIA_L2_Modify CMD_UTOPIA_L2_Modify_t;

/** Message ID for ACK_UTOPIA_L2_Modify */
#define ACK_UTOPIA_L2_MODIFY 0x0351

/**
   
*/
typedef struct ACK_UTOPIA_L2_Modify ACK_UTOPIA_L2_Modify_t;

/** Message ID for CMD_POSPHY_L2_Init */
#define CMD_POSPHY_L2_INIT 0x0451

/**
   This command is used to initialize the POS-PHY interface.
*/
typedef struct CMD_POSPHY_L2_Init CMD_POSPHY_L2_Init_t;

/** Message ID for ACK_POSPHY_L2_Init */
#define ACK_POSPHY_L2_INIT 0x0451

/**
   
*/
typedef struct ACK_POSPHY_L2_Init ACK_POSPHY_L2_Init_t;

/** Message ID for CMD_POSPHY_L2_Modify */
#define CMD_POSPHY_L2_MODIFY 0x0551

/**
   This command is used to modify the default settings of the POS-PHY interface.
*/
typedef struct CMD_POSPHY_L2_Modify CMD_POSPHY_L2_Modify_t;

/** Message ID for ACK_POSPHY_L2_Modify */
#define ACK_POSPHY_L2_MODIFY 0x0551

/**
   
*/
typedef struct ACK_POSPHY_L2_Modify ACK_POSPHY_L2_Modify_t;

/** Message ID for CMD_SegmentationReassemblyConfig */
#define CMD_SEGMENTATIONREASSEMBLYCONFIG 0x0A51

/**
   This command is used to configure the general settings for the SAR process.
   Note: The link must be disabled before this command can be sent. The command is
   accepted in modem state RESET only.
*/
typedef struct CMD_SegmentationReassemblyConfig CMD_SegmentationReassemblyConfig_t;

/** Message ID for ACK_SegmentationReassemblyConfig */
#define ACK_SEGMENTATIONREASSEMBLYCONFIG 0x0A51

/**
   Acknowledgement for the message CMD_SegmentationReassemblyConfig.
*/
typedef struct ACK_SegmentationReassemblyConfig ACK_SegmentationReassemblyConfig_t;

/** Message ID for CMD_Segmentation_VCC_Config */
#define CMD_SEGMENTATION_VCC_CONFIG 0x0B51

/**
   This command configures the ethernet forwarding table, up to 8 entries can be
   defined.
   Note: The command is accepted in modem state RESET only.
   Note: The link must be disabled before this command can be sent.
   Note: The general settings for the SAR process must be configured before with
   CMD_SegmentationReassemblyConfig.
*/
typedef struct CMD_Segmentation_VCC_Config CMD_Segmentation_VCC_Config_t;

/** Message ID for ACK_Segmentation_VCC_Config */
#define ACK_SEGMENTATION_VCC_CONFIG 0x0B51

/**
   Acknowledgement for the message CMD_Segmentation_VCC_Config.
*/
typedef struct ACK_Segmentation_VCC_Config ACK_Segmentation_VCC_Config_t;

/** Message ID for CMD_SegmentationClassFilter_Add */
#define CMD_SEGMENTATIONCLASSFILTER_ADD 0x0C51

/**
   This command adds a classification filter to the segmentation classification table,
   up to 16 entries can be defined.
   Note: The link must be disabled before this command can be sent.
   Note: The Ethernet Forwarding Table must not be empty (see
   CMD_Segmentation_VCC_Config).
*/
typedef struct CMD_SegmentationClassFilter_Add CMD_SegmentationClassFilter_Add_t;

/** Message ID for ACK_SegmentationClassFilter_Add */
#define ACK_SEGMENTATIONCLASSFILTER_ADD 0x0C51

/**
   Acknowledgement to the message CMD_SegmentationClassFilter_Add.
*/
typedef struct ACK_SegmentationClassFilter_Add ACK_SegmentationClassFilter_Add_t;

/** Message ID for CMD_ReassemblyClassFilter_Add */
#define CMD_REASSEMBLYCLASSFILTER_ADD 0x0D51

/**
   This command adds a classification filter to the reassembly classification table,
   upto 8 entries can be defined.
   Note: The link must be disabled before this command can be sent.
*/
typedef struct CMD_ReassemblyClassFilter_Add CMD_ReassemblyClassFilter_Add_t;

/** Message ID for ACK_ReassemblyClassFilter_Add */
#define ACK_REASSEMBLYCLASSFILTER_ADD 0x0D51

/**
   Acknowledgement for the message CMD_ReassemblyClassFilter_Add.
*/
typedef struct ACK_ReassemblyClassFilter_Add ACK_ReassemblyClassFilter_Add_t;

/** Message ID for CMD_VCC_MappingGet */
#define CMD_VCC_MAPPINGGET 0x0E11

/**
   Requests the mapping between the ATM-entry number (0...7) in the MAC-VPI/VCI map
   table and the VCC value.
   Note: The link must be disabled before this command can be sent.
*/
typedef struct CMD_VCC_MappingGet CMD_VCC_MappingGet_t;

/** Message ID for ACK_VCC_MappingGet */
#define ACK_VCC_MAPPINGGET 0x0E11

/**
   Acknowledgement to a CMD_VCC_MappingGet command. Provides the mapping between the
   ATM-entry number (0...7) in the MAC-VPI/VCI map table and the VCC value.
*/
typedef struct ACK_VCC_MappingGet ACK_VCC_MappingGet_t;

/** Message ID for CMD_EthernetOAM_Control */
#define CMD_ETHERNETOAM_CONTROL 0x4051

/**
   This message enables/disables the OAM layer.
*/
typedef struct CMD_EthernetOAM_Control CMD_EthernetOAM_Control_t;

/** Message ID for ACK_EthernetOAM_Control */
#define ACK_ETHERNETOAM_CONTROL 0x4051

/**
   Acknowledgment for message CMD_EthernetOAM_Control.
*/
typedef struct ACK_EthernetOAM_Control ACK_EthernetOAM_Control_t;

/** Message ID for CMD_EthernetOAM_Configure */
#define CMD_ETHERNETOAM_CONFIGURE 0x4151

/**
   This message configures the OAM operation.
   The command is to be applied locally at both xTU-C and xTU-R only after TC
   configuration. (See also Attention note at Page581).
*/
typedef struct CMD_EthernetOAM_Configure CMD_EthernetOAM_Configure_t;

/** Message ID for ACK_EthernetOAM_Configure */
#define ACK_ETHERNETOAM_CONFIGURE 0x4151

/**
   This message acknowledges the CMD_EthernetOAM_Configure message.
*/
typedef struct ACK_EthernetOAM_Configure ACK_EthernetOAM_Configure_t;

/** Message ID for CMD_EthernetInsertExtractControl */
#define CMD_ETHERNETINSERTEXTRACTCONTROL 0x4451

/**
   This command enables/disables the ethernet insert/extract to the system/line side.
   The command is to be applied locally. For a hostless CPE the Insert/Extract
   operation is enabled by default.
*/
typedef struct CMD_EthernetInsertExtractControl CMD_EthernetInsertExtractControl_t;

/** Message ID for ACK_EthernetInsertExtractControl */
#define ACK_ETHERNETINSERTEXTRACTCONTROL 0x4451

/**
   Acknowledgment for message CMD_EthernetInsertExtractControl.
*/
typedef struct ACK_EthernetInsertExtractControl ACK_EthernetInsertExtractControl_t;

/** Message ID for CMD_EthernetFrameLineInsert */
#define CMD_ETHERNETFRAMELINEINSERT 0x4551

/**
   This command writes an ethernet frame or fragments of it to be inserted to the Line
   side in transmit direction.
   The length of an ethernet frame migth be longer than the mailbox size, then it is
   send via a sequence of messages.
   The frame is transmitted when the message is received with a positive fragment
   length. A frame length of 0 indicates that the all data from the message is copied
   to the frame transmit buffer but the frame is not transmitted yet.
   Applicable at xTU-C and xTU-R.
   Example:
   For inserting an OAM PDU, the firmware assumes that the data fragments contain the
   following structure:
   6 Bytes: Ethernet Destination Address: 01-80-c2-00-00-02
   6 Bytes: Ethernet Source Address.
   2 Bytes: Length/Type field: 88-09 [Slow Protocols]
   1 Byte: Subtype = 0x03 [OAM]
   2 Bytes: Flags
   1 Byte: Code
   42-1496 Bytes: Data/Padding
   4 Bytes: FCS (automatically appended by Vinax)
   The FCS must not be in the data structure but is instead automatically appended. The
   first 4 bytes of the data structure which contain the length of the ethernet frame
   are not transmitted.
*/
typedef struct CMD_EthernetFrameLineInsert CMD_EthernetFrameLineInsert_t;

/** Message ID for ACK_EthernetFrameLineInsert */
#define ACK_ETHERNETFRAMELINEINSERT 0x4551

/**
   This message acknowledges the CMD_EthernetFrameLineInsert message.
*/
typedef struct ACK_EthernetFrameLineInsert ACK_EthernetFrameLineInsert_t;

/** Message ID for CMD_EthernetFrameSysInsert */
#define CMD_ETHERNETFRAMESYSINSERT 0x4651

/**
   This command writes an ethernet frame or fragments of it to be inserted to the
   system side in transmit direction.
   The length of an ethernet frame migth be longer than the mailbox size, then it is
   send via a sequence of messages.
   The frame is transmitted when the message is received with a positive fragment
   length. A frame length of 0 indicates that the all data from the message is copied
   to the frame transmit buffer but the frame is not transmitted yet.
   Applicable at xTU-C and xTU-R (with host).
   Example:
   For inserting an OAM PDU, the firmware assumes that the data fragments contain the
   following structure:
   6 Bytes: Ethernet Destination Address: 01-80-c2-00-00-02
   6 Bytes: Ethernet Source Address.
   2 Bytes: Length/Type field: 88-09 [Slow Protocols]
   1 Byte: Subtype = 0x03 [OAM]
   2 Bytes: Flags
   1 Byte: Code
   42-1496 Bytes: Data/Padding
   4 Bytes: FCS (automatically appended by Vinax)
   The FCS must not be in the data structure but is instead automatically appended. The
   first 4 bytes of the data structure which contain the length of the ethernet frame
   are not transmitted.
*/
typedef struct CMD_EthernetFrameSysInsert CMD_EthernetFrameSysInsert_t;

/** Message ID for ACK_EthernetFrameSysInsert */
#define ACK_ETHERNETFRAMESYSINSERT 0x4651

/**
   This message acknowledges the CMD_EthernetFrameSysInsert message.
*/
typedef struct ACK_EthernetFrameSysInsert ACK_EthernetFrameSysInsert_t;

/** Message ID for CMD_EthernetExtractConfigure */
#define CMD_ETHERNETEXTRACTCONFIGURE 0x4351

/**
   This message configures the ethernet packet extraction filters. There are extraction
   units at system and at line side. For each side there are two programmable filters
   and 3 fixed filters for PAUSE, MAC Control and Slow Protocol frames. The fixed
   filters can only be used to discard frames. The filter units can be configured such
   that the filtered frames are extracted and copied to the mailbox or the extracted
   frames can also be discarded.
   The message is to be applied locally before CMD_EthernetInsertExtractControl! (but
   after TC configuration, see Attention note at Page586).
   At the hostless CPE the message becomes effective only if the "LAN Update Enable"
   parameter in CMD_FW_UpdateLAN_Configure was DISABLED. Otherwise
   CMD_EthernetInsertExtractControl has no effect here!
   Application example:
   Spanning Tree Protocol BPDUÕs shall not be transported from LAN to WAN. BPDUÕs can
   be identified by the ethernet multicast destination address 01-80-C2-00-00-00. In
   order to block all ethernet frames with this destination address with filter unit 1,
   the following settings have to be programmed:
   Side: System Side
   Filter1Operation: Discard
   Filter1Config: FILTER_MAC_DST_ADDRESS
   MacAddr1_0_3: 0x0180C200
   MacAddr1_4_5: 0x00000000
   AddrMask1_0_3: 0x00000000
   AddrMask1_4_5: 0x00000000
   This will extract all BPDU frames identified via the ethernet multicast address.
*/
typedef struct CMD_EthernetExtractConfigure CMD_EthernetExtractConfigure_t;

/** Message ID for ACK_EthernetExtractConfigure */
#define ACK_ETHERNETEXTRACTCONFIGURE 0x4351

/**
   This message acknowledges the CMD_EthernetExtractConfigure message.
*/
typedef struct ACK_EthernetExtractConfigure ACK_EthernetExtractConfigure_t;

/** Message ID for EVT_EthernetFrameLineExtract */
#define EVT_ETHERNETFRAMELINEEXTRACT 0x4711

/**
   This event indicates the extraction of an ethernet frame from the line side in
   receive direction.
   Frames which are longer than the mailbox are transported in fragments.
   This EVT is generated at xTU-C and xTU-R with host.
*/
typedef struct EVT_EthernetFrameLineExtract EVT_EthernetFrameLineExtract_t;

/** Message ID for EVT_EthernetFrameSysExtract */
#define EVT_ETHERNETFRAMESYSEXTRACT 0x4811

/**
   This event indicates the extraction of an ethernet frame from the system side in
   transmit direction.
   Frames which are longer than the mailbox are transported in fragments.
   This EVT is generated at xTU-C and xTU-R with host.
*/
typedef struct EVT_EthernetFrameSysExtract EVT_EthernetFrameSysExtract_t;

/**
   This command is used to enable/disable the transmit and receive flow at the
   UTOPIA/POS-PHY interface.
*/
struct CMD_LinkControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Transmit Control Mode */
   DSL_uint32_t TX_Mode;
   /** Receive Control Mode */
   DSL_uint32_t RX_Mode;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Transmit Control Mode */
   DSL_uint32_t TX_Mode;
   /** Receive Control Mode */
   DSL_uint32_t RX_Mode;
#endif
} __PACKED__ ;


/**
   
*/
struct ACK_LinkControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command is used to configure the UTOPIA interface.
*/
struct CMD_UTOPIA_L2_Init
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data Width */
   DSL_uint32_t Width;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Data Width */
   DSL_uint32_t Width;
#endif
} __PACKED__ ;


/**
   
*/
struct ACK_UTOPIA_L2_Init
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command is used to modify the default settings of the UTOPIA interface.
*/
struct CMD_UTOPIA_L2_Modify
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** UDF2 Byte */
   DSL_uint32_t UDF2_bytes;
   /** Parity Mode */
   DSL_uint32_t ParityMode;
   /** Cell Discard if Parity Error */
   DSL_uint32_t CDPE;
   /** UTOPIA Port Numbers */
   DSL_uint32_t PortNum;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** UDF2 Byte */
   DSL_uint32_t UDF2_bytes;
   /** Parity Mode */
   DSL_uint32_t ParityMode;
   /** Cell Discard if Parity Error */
   DSL_uint32_t CDPE;
   /** UTOPIA Port Numbers */
   DSL_uint32_t PortNum;
#endif
} __PACKED__ ;


/**
   
*/
struct ACK_UTOPIA_L2_Modify
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command is used to initialize the POS-PHY interface.
*/
struct CMD_POSPHY_L2_Init
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   
*/
struct ACK_POSPHY_L2_Init
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command is used to modify the default settings of the POS-PHY interface.
*/
struct CMD_POSPHY_L2_Modify
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Parity Mode */
   DSL_uint32_t ParityMode;
   /** Burst Transfer Size */
   DSL_uint32_t ChunkSize;
   /** POS-PHY Bus Frequency */
   DSL_uint32_t Frequency;
   /** Packet Error Marking if Parity Error */
   DSL_uint32_t PMPE;
   /** POS-PHY Port Numbers */
   DSL_uint32_t PortNum;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Parity Mode */
   DSL_uint32_t ParityMode;
   /** Burst Transfer Size */
   DSL_uint32_t ChunkSize;
   /** POS-PHY Bus Frequency */
   DSL_uint32_t Frequency;
   /** Packet Error Marking if Parity Error */
   DSL_uint32_t PMPE;
   /** POS-PHY Port Numbers */
   DSL_uint32_t PortNum;
#endif
} __PACKED__ ;


/**
   
*/
struct ACK_POSPHY_L2_Modify
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command is used to configure the general settings for the SAR process.
   Note: The link must be disabled before this command can be sent. The command is
   accepted in modem state RESET only.
*/
struct CMD_SegmentationReassemblyConfig
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** CPCS Protocol */
   DSL_uint32_t CPCS_Protocol;
   /** FCS Preserved within the PDU */
   DSL_uint32_t FCS_Present;
   /** Segmentation Classification Filter */
   DSL_uint32_t SegClassFilter;
   /** Segmentation Default Action */
   DSL_uint32_t SegDefAction;
   /** Segmentation Default Virtual Path Identifier */
   DSL_uint32_t SegDef_VPI;
   /** Segmentation Default Virtual Channel Identifier */
   DSL_uint32_t SegDef_VCI;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** CPCS Protocol */
   DSL_uint32_t CPCS_Protocol;
   /** FCS Preserved within the PDU */
   DSL_uint32_t FCS_Present;
   /** Segmentation Classification Filter */
   DSL_uint32_t SegClassFilter;
   /** Segmentation Default Action */
   DSL_uint32_t SegDefAction;
   /** Segmentation Default Virtual Path Identifier */
   DSL_uint32_t SegDef_VPI;
   /** Segmentation Default Virtual Channel Identifier */
   DSL_uint32_t SegDef_VCI;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_SegmentationReassemblyConfig.
*/
struct ACK_SegmentationReassemblyConfig
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command configures the ethernet forwarding table, up to 8 entries can be
   defined.
   Note: The command is accepted in modem state RESET only.
   Note: The link must be disabled before this command can be sent.
   Note: The general settings for the SAR process must be configured before with
   CMD_SegmentationReassemblyConfig.
*/
struct CMD_Segmentation_VCC_Config
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** VCC Identifier */
   DSL_uint32_t VCC_id;
   /** Virtual Path Identifier */
   DSL_uint32_t VPI;
   /** Virtual Channel Identifier */
   DSL_uint32_t VCI;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** VCC Identifier */
   DSL_uint32_t VCC_id;
   /** Virtual Path Identifier */
   DSL_uint32_t VPI;
   /** Virtual Channel Identifier */
   DSL_uint32_t VCI;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_Segmentation_VCC_Config.
*/
struct ACK_Segmentation_VCC_Config
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command adds a classification filter to the segmentation classification table,
   up to 16 entries can be defined.
   Note: The link must be disabled before this command can be sent.
   Note: The Ethernet Forwarding Table must not be empty (see
   CMD_Segmentation_VCC_Config).
*/
struct CMD_SegmentationClassFilter_Add
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** MAC Address part 1 */
   DSL_uint32_t MAC_Address_1;
   /** MAC Address part 2 */
   DSL_uint32_t MAC_Address_2;
   /** VLAN Identifier */
   DSL_uint32_t VLAN_id;
   /** VLAN priority */
   DSL_uint32_t VLAN_prio;
   /** VCC Forwarding Identifier */
   DSL_uint32_t VCC_id;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** MAC Address part 1 */
   DSL_uint32_t MAC_Address_1;
   /** MAC Address part 2 */
   DSL_uint32_t MAC_Address_2;
   /** VLAN Identifier */
   DSL_uint32_t VLAN_id;
   /** VLAN priority */
   DSL_uint32_t VLAN_prio;
   /** VCC Forwarding Identifier */
   DSL_uint32_t VCC_id;
#endif
} __PACKED__ ;


/**
   Acknowledgement to the message CMD_SegmentationClassFilter_Add.
*/
struct ACK_SegmentationClassFilter_Add
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command adds a classification filter to the reassembly classification table,
   upto 8 entries can be defined.
   Note: The link must be disabled before this command can be sent.
*/
struct CMD_ReassemblyClassFilter_Add
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Virtual Path Identifier */
   DSL_uint32_t VPI;
   /** Virtual Channel Identifier */
   DSL_uint32_t VCI;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Virtual Path Identifier */
   DSL_uint32_t VPI;
   /** Virtual Channel Identifier */
   DSL_uint32_t VCI;
#endif
} __PACKED__ ;


/**
   Acknowledgement for the message CMD_ReassemblyClassFilter_Add.
*/
struct ACK_ReassemblyClassFilter_Add
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   Requests the mapping between the ATM-entry number (0...7) in the MAC-VPI/VCI map
   table and the VCC value.
   Note: The link must be disabled before this command can be sent.
*/
struct CMD_VCC_MappingGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number (=Bearer Channel Number) */
   DSL_uint32_t LinkNo;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number (=Bearer Channel Number) */
   DSL_uint32_t LinkNo;
#endif
} __PACKED__ ;


/**
   Acknowledgement to a CMD_VCC_MappingGet command. Provides the mapping between the
   ATM-entry number (0...7) in the MAC-VPI/VCI map table and the VCC value.
*/
struct ACK_VCC_MappingGet
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number (=Bearer Channel Number) */
   DSL_uint32_t LinkNo;
   /** VCC Value for Channel 0 */
   DSL_uint32_t VCC0;
   /** VCC Value for Channel 1 */
   DSL_uint32_t VCC1;
   /** VCC Value for Channel 2 */
   DSL_uint32_t VCC2;
   /** VCC Value for Channel 3 */
   DSL_uint32_t VCC3;
   /** VCC Value for Channel 4 */
   DSL_uint32_t VCC4;
   /** VCC Value for Channel 5 */
   DSL_uint32_t VCC5;
   /** VCC Value for Channel 6 */
   DSL_uint32_t VCC6;
   /** VCC Value for Channel 7 */
   DSL_uint32_t VCC7;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number (=Bearer Channel Number) */
   DSL_uint32_t LinkNo;
   /** VCC Value for Channel 0 */
   DSL_uint32_t VCC0;
   /** VCC Value for Channel 1 */
   DSL_uint32_t VCC1;
   /** VCC Value for Channel 2 */
   DSL_uint32_t VCC2;
   /** VCC Value for Channel 3 */
   DSL_uint32_t VCC3;
   /** VCC Value for Channel 4 */
   DSL_uint32_t VCC4;
   /** VCC Value for Channel 5 */
   DSL_uint32_t VCC5;
   /** VCC Value for Channel 6 */
   DSL_uint32_t VCC6;
   /** VCC Value for Channel 7 */
   DSL_uint32_t VCC7;
#endif
} __PACKED__ ;


/**
   This message enables/disables the OAM layer.
*/
struct CMD_EthernetOAM_Control
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Ethernet OAM enable */
   DSL_uint32_t OAM;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Ethernet OAM enable */
   DSL_uint32_t OAM;
#endif
} __PACKED__ ;


/**
   Acknowledgment for message CMD_EthernetOAM_Control.
*/
struct ACK_EthernetOAM_Control
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This message configures the OAM operation.
   The command is to be applied locally at both xTU-C and xTU-R only after TC
   configuration. (See also Attention note at Page581).
*/
struct CMD_EthernetOAM_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** OAM PDU Size */
   DSL_uint32_t OAMPDU_Size;
   /** OAM Mode */
   DSL_uint32_t OAM_Mode;
   /** Variable Retrieval */
   DSL_uint32_t VarRetrieval;
   /** Link Events */
   DSL_uint32_t LinkEvents;
   /** OAM Loopback */
   DSL_uint32_t Loopback;
   /** Unidirectional operation */
   DSL_uint32_t UnidirOAMsend;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** OAM PDU Size */
   DSL_uint32_t OAMPDU_Size;
   /** OAM Mode */
   DSL_uint32_t OAM_Mode;
   /** Variable Retrieval */
   DSL_uint32_t VarRetrieval;
   /** Link Events */
   DSL_uint32_t LinkEvents;
   /** OAM Loopback */
   DSL_uint32_t Loopback;
   /** Unidirectional operation */
   DSL_uint32_t UnidirOAMsend;
#endif
} __PACKED__ ;


/**
   This message acknowledges the CMD_EthernetOAM_Configure message.
*/
struct ACK_EthernetOAM_Configure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command enables/disables the ethernet insert/extract to the system/line side.
   The command is to be applied locally. For a hostless CPE the Insert/Extract
   operation is enabled by default.
*/
struct CMD_EthernetInsertExtractControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Control */
   DSL_uint32_t Control;
   /** Direction */
   DSL_uint32_t direction;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Control */
   DSL_uint32_t Control;
   /** Direction */
   DSL_uint32_t direction;
#endif
} __PACKED__ ;


/**
   Acknowledgment for message CMD_EthernetInsertExtractControl.
*/
struct ACK_EthernetInsertExtractControl
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command writes an ethernet frame or fragments of it to be inserted to the Line
   side in transmit direction.
   The length of an ethernet frame migth be longer than the mailbox size, then it is
   send via a sequence of messages.
   The frame is transmitted when the message is received with a positive fragment
   length. A frame length of 0 indicates that the all data from the message is copied
   to the frame transmit buffer but the frame is not transmitted yet.
   Applicable at xTU-C and xTU-R.
   Example:
   For inserting an OAM PDU, the firmware assumes that the data fragments contain the
   following structure:
   6 Bytes: Ethernet Destination Address: 01-80-c2-00-00-02
   6 Bytes: Ethernet Source Address.
   2 Bytes: Length/Type field: 88-09 [Slow Protocols]
   1 Byte: Subtype = 0x03 [OAM]
   2 Bytes: Flags
   1 Byte: Code
   42-1496 Bytes: Data/Padding
   4 Bytes: FCS (automatically appended by Vinax)
   The FCS must not be in the data structure but is instead automatically appended. The
   first 4 bytes of the data structure which contain the length of the ethernet frame
   are not transmitted.
*/
struct CMD_EthernetFrameLineInsert
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Fragment length */
   DSL_uint32_t FragLength;
   /** Fragment data */
   DSL_uint32_t Data[62];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Fragment length */
   DSL_uint32_t FragLength;
   /** Fragment data */
   DSL_uint32_t Data[62];
#endif
} __PACKED__ ;


/**
   This message acknowledges the CMD_EthernetFrameLineInsert message.
*/
struct ACK_EthernetFrameLineInsert
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This command writes an ethernet frame or fragments of it to be inserted to the
   system side in transmit direction.
   The length of an ethernet frame migth be longer than the mailbox size, then it is
   send via a sequence of messages.
   The frame is transmitted when the message is received with a positive fragment
   length. A frame length of 0 indicates that the all data from the message is copied
   to the frame transmit buffer but the frame is not transmitted yet.
   Applicable at xTU-C and xTU-R (with host).
   Example:
   For inserting an OAM PDU, the firmware assumes that the data fragments contain the
   following structure:
   6 Bytes: Ethernet Destination Address: 01-80-c2-00-00-02
   6 Bytes: Ethernet Source Address.
   2 Bytes: Length/Type field: 88-09 [Slow Protocols]
   1 Byte: Subtype = 0x03 [OAM]
   2 Bytes: Flags
   1 Byte: Code
   42-1496 Bytes: Data/Padding
   4 Bytes: FCS (automatically appended by Vinax)
   The FCS must not be in the data structure but is instead automatically appended. The
   first 4 bytes of the data structure which contain the length of the ethernet frame
   are not transmitted.
*/
struct CMD_EthernetFrameSysInsert
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Fragment length */
   DSL_uint32_t FragLength;
   /** Fragment data */
   DSL_uint32_t Data[62];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Fragment length */
   DSL_uint32_t FragLength;
   /** Fragment data */
   DSL_uint32_t Data[62];
#endif
} __PACKED__ ;


/**
   This message acknowledges the CMD_EthernetFrameSysInsert message.
*/
struct ACK_EthernetFrameSysInsert
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This message configures the ethernet packet extraction filters. There are extraction
   units at system and at line side. For each side there are two programmable filters
   and 3 fixed filters for PAUSE, MAC Control and Slow Protocol frames. The fixed
   filters can only be used to discard frames. The filter units can be configured such
   that the filtered frames are extracted and copied to the mailbox or the extracted
   frames can also be discarded.
   The message is to be applied locally before CMD_EthernetInsertExtractControl! (but
   after TC configuration, see Attention note at Page586).
   At the hostless CPE the message becomes effective only if the "LAN Update Enable"
   parameter in CMD_FW_UpdateLAN_Configure was DISABLED. Otherwise
   CMD_EthernetInsertExtractControl has no effect here!
   Application example:
   Spanning Tree Protocol BPDUÕs shall not be transported from LAN to WAN. BPDUÕs can
   be identified by the ethernet multicast destination address 01-80-C2-00-00-00. In
   order to block all ethernet frames with this destination address with filter unit 1,
   the following settings have to be programmed:
   Side: System Side
   Filter1Operation: Discard
   Filter1Config: FILTER_MAC_DST_ADDRESS
   MacAddr1_0_3: 0x0180C200
   MacAddr1_4_5: 0x00000000
   AddrMask1_0_3: 0x00000000
   AddrMask1_4_5: 0x00000000
   This will extract all BPDU frames identified via the ethernet multicast address.
*/
struct CMD_EthernetExtractConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** System or Line side */
   DSL_uint32_t Side;
   /** Pause Frame Filter */
   DSL_uint32_t PauseFilter;
   /** MAC Control Frame Filter */
   DSL_uint32_t MacControlFilter;
   /** Slow Protocol Frame Filter */
   DSL_uint32_t SlowProFilter;
   /** Operation Mode Filter 1 */
   DSL_uint32_t OpModeFilter1;
   /** Operation Mode Filter 2 */
   DSL_uint32_t OpModeFilter2;
   /** Filter type selection for programmable filter 1 */
   DSL_uint32_t Filter1Config;
   /** Filter 1 MAC Address octets 0 to 3 */
   DSL_uint32_t MacAddr1_0_3;
   /** Filter 1 MAC Address octets 4 to 5 */
   DSL_uint32_t MacAddr1_4_5;
   /** Filter 1 MAC Address Mask octets 0 to 3 */
   DSL_uint32_t AddrMask1_0_3;
   /** Filter 1 MAC Address Mask octets 4 to 5 */
   DSL_uint32_t AddrMask1_4_5;
   /** Filter 1 VLAN ID */
   DSL_uint32_t VlanID_1;
   /** Filter 1 VLAN ID Mask */
   DSL_uint32_t VlanID_Mask_1;
   /** Filter 1 VLAN Priority */
   DSL_uint32_t VlanPrio_1;
   /** Filter 1 VLAN Priority Mask */
   DSL_uint32_t VlanPrioMask_1;
   /** Filter type selection for programmable filter 2 */
   DSL_uint32_t Filter2Config;
   /** Filter 2 MAC Address octets 0 to 3 */
   DSL_uint32_t MacAddr2_0_3;
   /** Filter 2 MAC Address octets 4 to 5 */
   DSL_uint32_t MacAddr2_4_5;
   /** Filter 2 MAC Address Mask octets 0 to 3 */
   DSL_uint32_t AddrMask2_0_3;
   /** Filter 2 MAC Address Mask octets 4 to 5 */
   DSL_uint32_t AddrMask2_4_5;
   /** Filter 2 VLAN ID */
   DSL_uint32_t VlanID_2;
   /** Filter 2 VLAN ID Mask */
   DSL_uint32_t VlanID_Mask_2;
   /** Filter 2 VLAN Priority */
   DSL_uint32_t VlanPrio_2;
   /** Filter 2 VLAN Priority Mask */
   DSL_uint32_t VlanPrioMask_2;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** System or Line side */
   DSL_uint32_t Side;
   /** Pause Frame Filter */
   DSL_uint32_t PauseFilter;
   /** MAC Control Frame Filter */
   DSL_uint32_t MacControlFilter;
   /** Slow Protocol Frame Filter */
   DSL_uint32_t SlowProFilter;
   /** Operation Mode Filter 1 */
   DSL_uint32_t OpModeFilter1;
   /** Operation Mode Filter 2 */
   DSL_uint32_t OpModeFilter2;
   /** Filter type selection for programmable filter 1 */
   DSL_uint32_t Filter1Config;
   /** Filter 1 MAC Address octets 0 to 3 */
   DSL_uint32_t MacAddr1_0_3;
   /** Filter 1 MAC Address octets 4 to 5 */
   DSL_uint32_t MacAddr1_4_5;
   /** Filter 1 MAC Address Mask octets 0 to 3 */
   DSL_uint32_t AddrMask1_0_3;
   /** Filter 1 MAC Address Mask octets 4 to 5 */
   DSL_uint32_t AddrMask1_4_5;
   /** Filter 1 VLAN ID */
   DSL_uint32_t VlanID_1;
   /** Filter 1 VLAN ID Mask */
   DSL_uint32_t VlanID_Mask_1;
   /** Filter 1 VLAN Priority */
   DSL_uint32_t VlanPrio_1;
   /** Filter 1 VLAN Priority Mask */
   DSL_uint32_t VlanPrioMask_1;
   /** Filter type selection for programmable filter 2 */
   DSL_uint32_t Filter2Config;
   /** Filter 2 MAC Address octets 0 to 3 */
   DSL_uint32_t MacAddr2_0_3;
   /** Filter 2 MAC Address octets 4 to 5 */
   DSL_uint32_t MacAddr2_4_5;
   /** Filter 2 MAC Address Mask octets 0 to 3 */
   DSL_uint32_t AddrMask2_0_3;
   /** Filter 2 MAC Address Mask octets 4 to 5 */
   DSL_uint32_t AddrMask2_4_5;
   /** Filter 2 VLAN ID */
   DSL_uint32_t VlanID_2;
   /** Filter 2 VLAN ID Mask */
   DSL_uint32_t VlanID_Mask_2;
   /** Filter 2 VLAN Priority */
   DSL_uint32_t VlanPrio_2;
   /** Filter 2 VLAN Priority Mask */
   DSL_uint32_t VlanPrioMask_2;
#endif
} __PACKED__ ;


/**
   This message acknowledges the CMD_EthernetExtractConfigure message.
*/
struct ACK_EthernetExtractConfigure
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
#endif
} __PACKED__ ;


/**
   This event indicates the extraction of an ethernet frame from the line side in
   receive direction.
   Frames which are longer than the mailbox are transported in fragments.
   This EVT is generated at xTU-C and xTU-R with host.
*/
struct EVT_EthernetFrameLineExtract
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Start of Frame / End of Frame Indication */
   DSL_uint32_t SOF_EOF;
   /** Length of the fragment */
   DSL_uint32_t FragLength;
   /** Filter match */
   DSL_uint32_t Filtermatch;
   /** Fragment data */
   DSL_uint32_t Data[60];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Start of Frame / End of Frame Indication */
   DSL_uint32_t SOF_EOF;
   /** Length of the fragment */
   DSL_uint32_t FragLength;
   /** Filter match */
   DSL_uint32_t Filtermatch;
   /** Fragment data */
   DSL_uint32_t Data[60];
#endif
} __PACKED__ ;


/**
   This event indicates the extraction of an ethernet frame from the system side in
   transmit direction.
   Frames which are longer than the mailbox are transported in fragments.
   This EVT is generated at xTU-C and xTU-R with host.
*/
struct EVT_EthernetFrameSysExtract
{
#if DSL_BYTE_ORDER == DSL_BIG_ENDIAN
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Start of Frame / End of Frame Indication */
   DSL_uint32_t SOF_EOF;
   /** Length of the fragment */
   DSL_uint32_t FragLength;
   /** Filter match */
   DSL_uint32_t Filtermatch;
   /** Fragment data */
   DSL_uint32_t Data[60];
#else
   /** Index */
   DSL_uint16_t Index;
   /** Length */
   DSL_uint16_t Length;
   /** Link Number */
   DSL_uint32_t LinkNo;
   /** Start of Frame / End of Frame Indication */
   DSL_uint32_t SOF_EOF;
   /** Length of the fragment */
   DSL_uint32_t FragLength;
   /** Filter match */
   DSL_uint32_t Filtermatch;
   /** Fragment data */
   DSL_uint32_t Data[60];
#endif
} __PACKED__ ;


#ifdef __cplusplus
}
#endif
#if !defined (__GNUC__) && !defined (__GNUG__)
   #pragma pack()
#endif

/** @} */
#endif
