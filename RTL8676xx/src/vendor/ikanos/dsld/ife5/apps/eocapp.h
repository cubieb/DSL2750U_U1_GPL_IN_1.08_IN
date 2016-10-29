/**************************************************************
$Workfile:   eocapp.h  $
***************************************************************
**
** FIRMWARE, Copyright (C) 2000-2002
** Ikanos Communications, Fremont, CA
** All rights reserved.
**
** The information and contents of this file are the
** proprietary information of Ikanos Communication and may not
** be disclosed or used without the formal written approval of
** Ikanos Communication
**
$Revision: 1.1 $
$Date: 2008/07/18 11:08:38 $
***************************************************************/


/**************************************************************

	This file contains definitions and function prototype for
		EOC related Apps:
		
		EOC stream loopback
		
		
**************************************************************/
#ifndef EOCAPPH
#define EOCAPPH
#include <dapi/dapi_parms.h>
#ifdef INI_SUPPORT
#include 	<api/HostPtmfApi.h>
#endif
/* EOC message type */
#define 	EOC_DATA        0x0
#define 	EOC_ACK         0x1
#define 	EOC_BUSY        0x2
#define 	EOC_REJECT      0x3
#define 	EOC_RE_TX       0x4
#define 	EOC_RE_START    0x5
#define 	EOC_START       0x6
#define 	EOC_END         0x7
#define 	EOC_FAR_END_COUNTERS_REQ  0xa
#define 	EOC_FAR_END_COUNTERS_RESP  0xb
#define 	EOC_REQ_FAR_END_SNR_ATTN 0xc
#define 	EOC_RESP_FAR_END_SNR_ATTN 0xd
#define 	EOC_LOOPBACK_REQ		  0xe
#define 	EOC_LOOPBACK_RESP		  0xf
#define 	EOC_HEARTBEAT_CTL		  0x10

#define		EOC_PTMF_ADD_MAC_REQ	  0x11
#define		EOC_PTMF_REMOVE_MAC_REQ	  0x12
#define		EOC_PTMF_SHOW_MAC_REQ	  0x13

#define		EOC_PTMF_READ_CONFIG_REQ	      0x14
#define		EOC_PTMF_WRITE_CONFIG_REQ	      0x15
	
#define		EOC_PTMF_SHOW_MAC_RESP	  0x16
#define		EOC_PTMF_FLUSH_LEARN_ADDR_REQ   0x17

#define		EOC_PTMF_CONFIG_MAC_REQ 	0x18

#define		EOC_PTMF_READ_CONFIG_RESP	      0x19

#define		EOC_RFM_PROTOCOL		0x20
						 
#define		EOC_ELM_PROTOCOL		0x21
						 
#define		EOC_PROTOCOL_VERSION		0x22
#define		EOC_RMM_PROTOCOL		0x23
#define     EOC_ENET_STATUS_RESP    0x24

/* this message type is detected by inspecting the message.  That's why it does not fit in an U8 */

#define		EOC_VIRTUAL_MESSAGE_1		0x100

/* frams type */

#define EOC_DOWNLOAD    0x8

/* header */

#define EOC_CTL_B       0x0
#define EOC_TYPE_B      (EOC_CTL_B + 1)
#define EOC_LEN_B       (EOC_TYPE_B + 1)
#define EOC_SEQ_B       (EOC_LEN_B + 1)

#define MAX_IE_PARSER_ENTRIES	50
#define IE_NO_INFO		0
#define IE_SEVERE_ERROR		128
#define  PROBLEM_PARSING	2
#define  INPUT_OVERFLOW		3
#define  OUTPUT_OVERFLOW	4
#define  FEATURE_NOT_SUPPORTED	5
#define IE_SEVERE_ERROR_LEN	1

#define XTRACT_4BYTE(val, ptr) {\
	val = (*((unsigned char *) ptr++) << 24) ;\
	val |= (*((unsigned char *) ptr++) << 16) ;\
	val |= (*((unsigned char *) ptr++) << 8) ;\
	val |= (*((unsigned char *) ptr++))  ;\
}

#define XTRACT_2BYTE(val, ptr) {\
	val = (*((unsigned char *) ptr++) << 8) ; \
	val |= (*((unsigned char *) ptr++))  ; \
}


typedef struct port_error_counters {
	int	countNackBusy;
	int	countNackError;
	int	totalErrors;
} port_error_counters;

typedef int (*ieEncoder)( const void *pDataStruct, U8 **pPmsg, U8 ieLen );
typedef int (*ieDecoder)( U8 *pMsg, void *pDataStruct, U8 ieLen );

typedef struct {
	ieEncoder	pIeEncoder;
	ieDecoder	pIeDecoder;
	U16		eocProtocol;
	U8		ieType;
	U8		ieSubtype;
} ieParserEntry, *pIeParserEntry;

#define OHM_MAX_TONE    1024
#define MAX_LATENCY_PATH 1 
typedef struct _tOhmPmdPmsCntrs
{
  UINT32 aulFec[MAX_LATENCY_PATH] ;  //Counter of the FEC-x anomalies
  UINT32 aulCrc[MAX_LATENCY_PATH] ;  //Counter of the CRC-x anomalies
  UINT32 ulFecEs ;              //FEC errored seconds counter
  UINT32 ulEs ;                //Errored seconds counter
  UINT32 ulSEs ;              //Severely errored seconds counter
  UINT32 ulLosEs ;              //LOS errored seconds counter
  UINT32 ulUnEs ;              //Unavailable errored seconds counter
} tOhmPmdPmsCntrs ;

typedef struct _tOhmTpsCntrs
{
  UINT32 ulHec ;          //Counter of the HEC anomalies
  UINT32 ulHecCells ;  //Counter of total cells passed through HEC function
  UINT32 ulUpCells ;   //Counter of total cells passed to the upper layer ATM function
  UINT32 ulBitErrs ;    //Counter of total bit errors detected in ATM idle cells payload
} tOhmTpsCntrs ;

typedef struct _tOhmMgmtCounters {
  
  tOhmPmdPmsCntrs   pmdPms ;
  tOhmTpsCntrs          atm[MAX_LATENCY_PATH] ;
} tOhmMgmtCounters ;



															
/* This struct MUST match the oamp_cb_msg struct in oampfuncs.c !! */

/* master EOC message receiver parser	*/
/* Used as the callback fucntion for EOC Stream Receive: iposCbEocDataRelay	*/
void	appEocMsgParser(ipos_port_id	pid, U8 uid, U32 upar, int len, U8 *data);
				   
/* The respond to EOC LB REQ	*/
void	appEocLbResp(ipos_port_id	pid, U8 uid, U32 upar, int len, U8 *data);

/* The EOC loopback Response	*/
void	appEocLbResult(ipos_port_id	pid, U8 uid, U32 upar, int len, U8 *data);

/* To initiate the Eoc Lb Test	*/
void	appEocLbReq(ipos_port_id	pid);
 
/* For EOC Loopback Stress tests */

#define  EOC_LB_MAGIC	0xab
#define  EOC_LB_MAGIC_B		(EOC_CTL_B + 1)


extern void appEocValidateData(int port, int valid);
extern void	appEocFeCntr(ipos_port_id	pid);
void	appEocFeCntrReq(ipos_port_id	pid);
void	appEocFeCntrResp(ipos_port_id	pid, U8 uid, U32 upar, int len, U8 *data);
void	appEocFeCntrResult(ipos_port_id	pid, U8 uid, U32 upar, int len, U8 *data);
#ifdef POLL_FE_SNR
extern void	appEocFeSnrAttn(ipos_port_id	pid);
#else
#define appEocFeSnrAttn appEocFeSnrAttnReq
#endif
void	appEocFeSnrAttnReq(ipos_port_id	pid);
void	appEocFeSnrAttnResp(ipos_port_id	pid, U8 uid, U32 upar, int len, U8 *data);
void	appEocFeSnrAttnResult(ipos_port_id	pid, U8 uid, U32 upar, int len, U8 *data);

void appEocFeEnetStatus(ipos_port_id	pid, U8 uid, U32 upar, int len, U8 *data);
/* Ethernet Address Filtering of the remote CPE over EOC : REQUESTS!!!!! */
#ifdef INI_SUPPORT 
void appEocConfigMacReq (ipos_port_id pid, tsIposPtmfEnetAddrFilterCfg *ptsPtmfAddrFilTab);
void appEocConfigExtendMacReq (ipos_port_id          pid,
                               tsIposPtmfEnetAddrFilterCfg  *ptsPtmfAddrFilTab );
#endif // INI_SUPPORT
void appEocCpeAddMacReq (ipos_port_id pid, U8 bTableIndex, U8 *pbEnetAddr);
void appEocCpeRemoveMacReq (ipos_port_id pid, U8 bTableIndex);
void appEocCpeShowMacReq (ipos_port_id pid);

void appEocCpeReadConfigMacReq (ipos_port_id pid ); 

void appEocFlushLearnedAddrReq (ipos_port_id pid );


/* Ethernet Address Filtering of the remote CPE over EOC : RESPONSES !!!!! */
void appEocCpeAddMacResp (ipos_port_id pid, U8 uid, U32 upar, int len, U8 *data);
void appEocCpeRemoveMacResp (ipos_port_id pid, U8 uid, U32 upar, int len, U8 *data);
void appEocCpeShowMacResp (ipos_port_id pid, U8 uid, U32 upar, int len, U8 *data);
void appEocCpeShowMacComplete (ipos_port_id pid, U8 uid, U32 upar, int len, U8 *data);

void appEocCpeConfigMacResp (ipos_port_id pid,U8 uid,U32 upar,int len,U8 *data);

void appEocCpeReadConfigMacResp ( ipos_port_id pid, 
                                  U8 uid, U32 upar, int len, U8 *data);

void appEocReadConfigMacComplete ( ipos_port_id pid, 
                                      U8 uid, U32 upar, int len, U8 *data);

void appEocCpeConfigExtendMacResp (ipos_port_id pid, 
                                   U8 uid, U32 upar, int len, U8 *data);

void appEocCpeFlushLearnedAddrResp (ipos_port_id pid,
                                    U8 uid, U32 upar, int len, U8 *data);

extern int		decodeOneByte( U8 *pMsg, void *pDataStruct, U8 ieLen );
extern int		decode32Bits( U8 *pMsg, void *pDataStruct, U8 ieLen );
extern int		decode16Bits( U8 *pMsg, void *pDataStruct, U8 ieLen );
extern int		decodeString( U8 *pMsg, void *pDataStruct, U8 ieLen );
extern int		encodeOneByte( const void *pDataStruct, U8 **pPmsg, U8 ieLen );
extern int		encode32Bits( const void *pDataStruct, U8 **pPmsg, U8 ieLen );
extern int		encode16Bits( const void *pDataStruct, U8 **pPmsg, U8 ieLen );
extern int		encodeString( const void *pDataStruct, U8 **pPmsg, U8 ieLen );

extern ieDecoder	appEocGetDecoder( U16 eocProtocol, U8 ieType, U8 ieSubtype );
extern ieEncoder	appEocGetEncoder( U16 eocProtocol, U8 ieType, U8 ieSubtype );

extern int	appEocRegIeParsers(
	U16 eocProtocol, U8 ietype, U8 ieSubtype,
	ieEncoder pTheEncoder, ieDecoder pTheDecoder
);

extern void		appEocProtocolVersionInit( void );
extern const char 	*uiGetVendorID( void );
extern U16		 uiGetEocProtocolVersion( void );
extern void appEocRemoteEnetStatus( ipos_port_id pid);

#endif /* EOCAPPH */
