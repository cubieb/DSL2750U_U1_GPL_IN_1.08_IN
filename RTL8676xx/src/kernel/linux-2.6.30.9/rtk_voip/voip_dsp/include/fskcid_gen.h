
#ifndef _FSK_GEN_H
#define _FSK_GEN_H

#include "rtk_voip.h"
#include "voip_params.h"

#define FSK_CLID_BAUD_RATE	1200
#define MAX_CLID_DATA_SIZE	80
#define FSK_MDMF_SIZE		5


typedef struct
{
	unsigned int ch_seizure_cnt;
	unsigned int mark_cnt;
	TfskClidGain mark_gain;
	TfskClidGain space_gain;
	unsigned char type2_expected_ack_tone;
	unsigned int delay_after_1st_ring;	//ms
	unsigned int delay_before_2nd_ring;	//ms
}
TstFskClidPara;

typedef struct
{
        TfskParaType type; // para_type
        char data[MAX_CLID_DATA_SIZE];
}
TstFskClidData;

typedef struct
{
	unsigned char cid_mode;
	unsigned char cid_msg_type;
	unsigned char cid_setup;
	unsigned char cid_states;
	unsigned char cid_complete;
	unsigned long time_out;
	TstFskClidData cid_data[FSK_MDMF_SIZE];
}
TstVoipFskClid;

extern unsigned char gFskType2AckTone[MAX_VOIP_CH_NUM];
extern unsigned int gDelayAfert1stRing[MAX_VOIP_CH_NUM];
extern unsigned int gDelayBefore2ndRing[MAX_VOIP_CH_NUM];
extern unsigned int gChannelSeizureDuration[MAX_VOIP_CH_NUM];
extern unsigned int gMarkDuration[MAX_VOIP_CH_NUM];
extern unsigned int gMsgDuration[MAX_VOIP_CH_NUM];


unsigned char checkSum( char * string );
void VMWIMessage(TstFskClidData* clid_data);
void init_softfskcidGen(uint32_t chid);
void genSoftFskCID(uint32_t chid);
int fsk_gen(uint32_t chid);
void cid_fsk_gen(uint32_t chid, uint32_t sid, int bWideband);
void fsk_cid_para_set(uint32_t chid, TfskArea area, TstFskClidPara *fsk_para);
void fsk_cid_para_get(uint32_t chid, TfskArea area, TstFskClidPara *fsk_para);

#endif