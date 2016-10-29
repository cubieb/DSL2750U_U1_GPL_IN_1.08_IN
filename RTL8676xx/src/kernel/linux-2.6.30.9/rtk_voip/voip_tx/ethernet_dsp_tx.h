#ifndef ETHERNET_DSP_TX
#define ETHERNET_DSP_TX

#include "../voip_dsp/rtp/RtpPacket.h"
#include "../voip_dsp/rtp/RtcpPacket.h"

extern int ethernetDspSentL2ControlPacket(unsigned short cmd, unsigned int chid, void* pMgr, unsigned short mgrLen);
extern int ethernetDspSentL2ControlPacketNoChannel(unsigned short cmd, void* pMgr, unsigned short mgrLen);
extern int ethernetDspSentL2ResponsePacket(unsigned short cmd, unsigned short seq_no, void* pdata, unsigned short data_len);
extern int ethernetDspCheckL2RespPacket(int cmd, void* pCont, unsigned short* pDsp_id);
extern void ethernetDsp_RtpTx(RtpPacket* pst);
extern void ethernetDsp_RtcpTx(RtcpPacket* pst);
extern void ethernetDsp_T38Tx( unsigned int chid, unsigned int sid, void* packet, unsigned int pktLen);

#include "voip_control.h"
extern int voip_dsp_L2_pkt_tx(unsigned short category, unsigned char pkt_type, unsigned char* pdata, unsigned short data_len, TstTxPktCtrl* txCtrl);

#endif

