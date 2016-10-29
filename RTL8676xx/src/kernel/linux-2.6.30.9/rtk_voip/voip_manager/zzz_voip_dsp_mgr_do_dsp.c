#include <linux/interrupt.h>
#include <asm/uaccess.h>
#include <linux/delay.h>  	// udelay

#include "rtk_voip.h"
#include "voip_types.h"
#include "voip_control.h"
#include "voip_params.h"
#include "voip_mgr_netfilter.h"
#ifndef CONFIG_DEFAULTS_KERNEL_2_6
#include "voip_mgr_do_dsp.h"
#endif

#ifdef T38_STAND_ALONE_HANDLER
#include "../voip_drivers/t38_handler.h"
#endif /* T38_STAND_ALONE_HANDLER */
#include "../voip_dsp/ivr/ivr.h"
//#include "../voip_drivers/Daa_api.h"
#include "snd_mux_daa.h"
#include "snd_help.h"
#include "../voip_drivers/fsk.h"
#include "../voip_dsp/include/fskcid_gen.h"
#ifdef SW_DTMF_CID
#include "../voip_drivers/dsp_rtk_caller.h"
#endif

#if ! defined (AUDIOCODES_VOIP)
#ifdef SUPPORT_LEC_G168_ISR
#include "../voip_dsp/include/lec.h"
#endif
#include "../voip_dsp/dsp_r0/dspparam.h"
#include "../voip_dsp/include/dtmf_dec.h"
#include "../voip_dsp/dsp_r0/dspcodec_0.h"
#ifdef FXO_BUSY_TONE_DET
#include "../voip_dsp/include/tone_det_i.h"
#endif

#else

#include "RTK_AC49xApi_Interface.h"

#endif

#ifdef VOIP_RESOURCE_CHECK
#include "voip_resource_check.h"
#endif

#include "v152_api.h"

#include "con_mux.h"

#if ! defined (AUDIOCODES_VOIP)

#ifdef FXO_CALLER_ID_DET
extern int fsk_cid_valid[MAX_VOIP_CH_NUM];
extern int dtmf_cid_valid[MAX_VOIP_CH_NUM];
#endif


TstVoipPayLoadTypeConfig astVoipPayLoadTypeConfig[MAX_SESS_NUM];

#else



#endif

extern long voice_gain_spk[];//0 is mute, 1 is -31dB ~~~ 32 is 0dB , 33 is 1dB ~~~ 63 is 31dB
extern long voice_gain_mic[];

int eng_det_flag[PCM_CH_NUM] = {0};

#ifdef PCM_LOOP_MODE_CONTROL
extern TstVoipLoopBcakInfo LoopBackInfo[SESS_NUM];
extern int loop_3way[VOIP_CH_NUM];
#endif

#ifdef VOIP_RESOURCE_CHECK
extern int resource_weight[SESS_NUM];// = {0};
#endif

int g_dynamic_pt_remote[SESS_NUM] = {0};
int g_dynamic_pt_local[SESS_NUM]={0};
#ifdef SUPPORT_V152_VBD
int g_dynamic_pt_remote_vbd[SESS_NUM] = {0};
int g_dynamic_pt_local_vbd[SESS_NUM]={0};
#endif

unsigned int fax_end_flag[MAX_VOIP_CH_NUM];      //for t.38 fax end detect.

//extern TstTwoChannelCfg astTwoChannelCfg[VOIP_CH_NUM];	// pkshih: unused now

extern int g_SIP_Info_play[];		/* 0: stop 1: start play */
extern int g_SIP_Info_tone_buf[][10];
extern int g_SIP_Info_time_buf[][10];
extern int g_SIP_Info_buf_w[];
extern int g_SIP_Info_buf_r[];

extern uint32 cust;

extern int Is_DAA_Channel(int chid);
extern void init_softfskcidGen(uint32_t chid);

#if ! defined (AUDIOCODES_VOIP)

//-------------- For FAX Detection -------------
extern unsigned char fax_offhook[];
uint32 fax_modem_det_mode[MAX_VOIP_CH_NUM] = {0}; /* fax modem det mode, 0:auto. 1:fax. 2:modem */
//----------- For dtmf cid generation ----------
#ifdef SW_DTMF_CID
extern char dtmf_cid_state[];
extern char cid_str[];
#endif
//----------- For dtmf removal -----------------
extern char dtmf_mode[];
extern unsigned char dtmf_removal_flag[];
//----------------------------------------------

extern char cid_dtmf_mode[];

/* agc */
extern unsigned char spk_agc_mode[];
extern unsigned char spk_agc_lvl[];
extern unsigned char spk_agc_gup[];
extern unsigned char spk_agc_gdown[];
extern unsigned char mic_agc_mode[];
extern unsigned char mic_agc_lvl[];
extern unsigned char mic_agc_gup[];
extern unsigned char mic_agc_gdown[];

#else

extern uint32 gSetByassMode[];

extern char dtmf_mode[MAX_VOIP_CH_NUM];
long auto_cid_det[PCM_CH_NUM] = {0};
long cid_type[PCM_CH_NUM] = {0};

char fsk_spec_areas[MAX_VOIP_CH_NUM];	// bit0-2: FSK Type
			// bit 3: Normal Ring
			// bit 4: Fsk Alert Tone
			// bit 5: Short Ring
			// bit 6: Line Reverse
			// bit 7: Date, Time Sync and Name
int fsk_cid_gain[MAX_VOIP_CH_NUM] = {1};
char dtmf_cid_state[MAX_VOIP_CH_NUM]={0};
unsigned char cid_str[21];
char cid_dtmf_mode[MAX_VOIP_CH_NUM];     // for DTMF start/end digit
int tone_idx;
int CustomToneTable[8][24];
unsigned long gFirstRingOffTimeOut[MAX_VOIP_CH_NUM];
unsigned char gRingGenAfterCid[MAX_VOIP_CH_NUM] = {0};
extern int gRingCadOff[MAX_VOIP_CH_NUM];


/* agc */
unsigned char spk_agc_mode[MAX_VOIP_CH_NUM];
unsigned char mic_agc_mode[MAX_VOIP_CH_NUM];
unsigned char agc_enable[MAX_VOIP_CH_NUM]={0};
unsigned char spk_agc_gup;
unsigned char spk_agc_gdown;
unsigned char mic_agc_gup;
unsigned char mic_agc_gdown;

#endif /* AUDIOCODES_VOIP */

extern int pulse_dial_in_cch(uint32 ch_id, char input);

extern unsigned int ethernetDspSentL2ResponsePacket(unsigned short cmd, unsigned short seq_no, unsigned char* pdata, unsigned short data_len);
extern void led_state_watcher(unsigned int chid);

#ifdef AUDIOCODES_VOIP
int SaveCustomTone(TstVoipToneCfg *pToneCfg)
{
	/*
	TONE_TYPE_ADDITIVE = 0
    	TONE_TYPE_MODULATED = 1
    	TONE_TYPE_SUCC = 2
    	TONE_TYPE_SUCC_ADD =3
    	*/

	if (pToneCfg->toneType == 2)//SUCC
	{
		CustomToneTable[tone_idx][0] = CALL_PROGRESS_SIGNAL_TYPE__SPECIAL_INFORMATION_TONE ;
		//PRINT_MSG("SPECIAL_INFO...\n");
	}
	else
	{
		if (pToneCfg->cycle == 0)
		{
			CustomToneTable[tone_idx][0] = CALL_PROGRESS_SIGNAL_TYPE__CONTINUOUS;
			//PRINT_MSG("CONTINUOUS...\n");
		}
		else if (pToneCfg->cycle == 1)
		{
			CustomToneTable[tone_idx][0] = CALL_PROGRESS_SIGNAL_TYPE__BURST; // play one cycle
			//PRINT_MSG("BURST...\n");
		}
		else if (pToneCfg->cycle > 1)
		{
			CustomToneTable[tone_idx][0] = CALL_PROGRESS_SIGNAL_TYPE__CADENCE;
			//PRINT_MSG("CADENCE...\n");
		}
	}

	CustomToneTable[tone_idx][1] = pToneCfg->Freq1;		//ToneAFrequency
	CustomToneTable[tone_idx][2] = pToneCfg->Freq2;		//ToneB_OrAmpModulationFrequency
	CustomToneTable[tone_idx][3] = 10;			//TwistThreshold

	if (pToneCfg->toneType == 2)//SUCC
		CustomToneTable[tone_idx][4] = pToneCfg->Freq3;	//ThirdToneOfTripleBatchDurationTypeFrequency
	else
		CustomToneTable[tone_idx][4] = 0;
	//PRINT_MSG("Freq3=%d\n", CustomToneTable[tone_idx][4]);


	CustomToneTable[tone_idx][5] = 0;			//HighEnergyThreshold
	CustomToneTable[tone_idx][6] = 35;			//LowEnergyThreshold
	CustomToneTable[tone_idx][7] = 15;			//SignalToNoiseRatioThreshold
	CustomToneTable[tone_idx][8] = 10;			//FrequencyDeviationThreshold
	CustomToneTable[tone_idx][9] = pToneCfg->Gain1;		//ToneALevel

	if ((pToneCfg->toneType == 1) || pToneCfg->toneType == 2)//Modulate or SUCC
		CustomToneTable[tone_idx][10] = 0;		//ToneBLevel
	else
		CustomToneTable[tone_idx][10] = pToneCfg->Gain2;

	//PRINT_MSG("ToneBLevel=%d\n", CustomToneTable[tone_idx][10]);

	/*
	AM Factor is the AM Modulation index, its range us between 0%-100%
	AMFACTOR field range is between 0-50 (unit 0.02) corresponding to 0%-100%.
	In the example I chose AMFACTOR=25 (50%)
	*/
       	if (pToneCfg->toneType == 1)//Modulate
		CustomToneTable[tone_idx][11] = 25;		//AmFactor
	else
		CustomToneTable[tone_idx][11] = 0;

	//PRINT_MSG("AmFactor=%d\n", CustomToneTable[tone_idx][11]);

	CustomToneTable[tone_idx][12] = (pToneCfg->CadOn0)/10;	//DetectionTimeOrCadenceFirstOnOrBurstDuration
	CustomToneTable[tone_idx][13] = (pToneCfg->CadOff0)/10;	//CadenceFirstOffDuration
	CustomToneTable[tone_idx][14] = (pToneCfg->CadOn1)/10;	//CadenceSecondOnDuration
	CustomToneTable[tone_idx][15] = (pToneCfg->CadOff1)/10;	//CadenceSecondOffDuration
	CustomToneTable[tone_idx][16] = (pToneCfg->CadOn2)/10;	//CadenceThirdOnDuration
	CustomToneTable[tone_idx][17] = (pToneCfg->CadOff2)/10;	//CadenceThirdOffDuration
	CustomToneTable[tone_idx][18] = (pToneCfg->CadOn3)/10;	//CadenceFourthOnDuration
	CustomToneTable[tone_idx][19] = (pToneCfg->CadOff3)/10;	//CadenceFourthOffDuration
	CustomToneTable[tone_idx][20] = 0;			//CadenceVoiceAddedWhileFirstOff
	CustomToneTable[tone_idx][21] = 0;			//CadenceVoiceAddedWhileSecondOff
	CustomToneTable[tone_idx][22] = 0;			//CadenceVoiceAddedWhileThirdOff
	CustomToneTable[tone_idx][23] = 0;			//CadenceVoiceAddedWhileFourthOff

#if 0
	pToneCfg->cadNUM;
	pToneCfg->PatternOff;
	pToneCfg->ToneNUM;
#endif
	return 0;
}
#endif /* AUDIOCODES_VOIP */

/**
 * @ingroup VOIP_DSP_GENERAL
 * @brief Re-initialize variables when on-hook 
 * @param TstVoipCfg.ch_id Channel ID 
 * @see VOIP_MGR_ON_HOOK_RE_INIT TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_ON_HOOK_RE_INIT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	extern void Init_CED_Det(unsigned char CH);	//thlin+ 2006-02-08
	extern void AEC_re_init(unsigned int chid);
	extern void NLP_g168_init(unsigned int chid);
	extern void NR_re_init(unsigned int chid);

#ifdef FXO_CALLER_ID_DET
	int i;
#endif
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	PRINT_MSG("VOIP_MGR_ON_HOOK_RE_INIT:ch_id = %d, enable = %d\n", stVoipCfg.ch_id, stVoipCfg.enable);
#ifdef _chiminer_slic2_ch1
    stVoipCfg.ch_id=1;
	PRINT_MSG("stVoipCfg.ch_id=%d\n",stVoipCfg.ch_id);
#endif
	Init_CED_Det(stVoipCfg.ch_id);

#ifdef SUPPORT_LEC_G168_ISR
	LEC_re_init(stVoipCfg.ch_id);
#ifdef CONFIG_RTK_VOIP_DRIVERS_IP_PHONE
#ifdef T_TYPE_ECHO_CAN
	LEC_re_init(3);
#endif
#endif
#endif
	
#ifdef EXPER_AEC
	AEC_re_init(stVoipCfg.ch_id);
#endif

#ifdef EXPER_NR
	NR_re_init(stVoipCfg.ch_id);
#endif
#if 0
	frequency_echo_state_reset(stVoipCfg.ch_id);
#endif
#ifdef CONFIG_RTK_VOIP_DRIVERS_IP_PHONE
	AEC_re_init(stVoipCfg.ch_id);
	NLP_g168_init(stVoipCfg.ch_id);
	NR_re_init(stVoipCfg.ch_id);
#endif

	uint32 SessNum, ssid, j;
	SessNum = chanInfo_GetRegSessionNum(stVoipCfg.ch_id);
	for (j=0; j < SessNum; j++)
	{
		ssid = chanInfo_GetRegSessionID(stVoipCfg.ch_id, j);
		RFC2833_receiver_init(ssid);
	}

#ifdef SUPPORT_FAX_V21_DETECT
	init_fax_v21(stVoipCfg.ch_id);
#endif

#ifdef FXO_CALLER_ID_DET
	dmtf_cid_det_init(stVoipCfg.ch_id);
	init_cid_det_si3500(2);
	//for (i=0; i < SLIC_CH_NUM; i++)
	for (i=0; i < CON_CH_NUM; i++)
	{
		if( get_snd_type_cch( i ) != SND_TYPE_FXS )
			continue;
			
		fsk_cid_valid[i] = 0;
		dtmf_cid_valid[i] = 0;
	}
#endif
	return 0;
}
#else
int do_mgr_VOIP_MGR_ON_HOOK_RE_INIT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	extern void flush_fax_modem_fifo(uint32 ch_id);
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));

	dtmf_cid_state[stVoipCfg.ch_id] = 0;
	gSetByassMode[stVoipCfg.ch_id] = 0;
	flush_fax_modem_fifo(stVoipCfg.ch_id);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_GENERAL
 * @brief Set voice (speaker/mic) gain 
 * @param TstVoipValue.value Speaker gain <br> 
 *        (0 is mute, 1 is -31dB ~~~ 32 is 0dB , 33 is 1dB ~~~ 63 is 31dB)
 * @param TstVoipValue.value1 MIC gain <br> 
 *        (0 is mute, 1 is -31dB ~~~ 32 is 0dB , 33 is 1dB ~~~ 63 is 31dB)
 * @see VOIP_MGR_SET_VOICE_GAIN TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_VOICE_GAIN(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));

	voice_gain_spk[stVoipValue.ch_id]=stVoipValue.value+32;//0 is mute, 1 is -31dB ~~~ 32 is 0dB , 33 is 1dB ~~~ 63 is 31dB
	voice_gain_mic[stVoipValue.ch_id]=stVoipValue.value1+32;//0 is mute, 1 is -31dB ~~~ 32 is 0dB , 33 is 1dB ~~~ 63 is 31dB
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_VOICE_GAIN(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));

	/* Voice Gain Adjustment */
	Tac49xVoiceGain mic_gain, spk_gain;
	
	spk_gain = stVoipValue.value+32;  //0 is mute, 1 is -31dB ~~~ 32 is 0dB , 33 is 1dB ~~~ 63 is 31dB
	mic_gain = stVoipValue.value1+32; //0 is mute, 1 is -31dB ~~~ 32 is 0dB , 33 is 1dB ~~~ 63 is 31dB
	
	RtkAc49xApiSetVoiceGain(stVoipValue.ch_id, mic_gain, spk_gain);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_GENERAL
 * @brief Enable/disable energy detection, and retrieve detected energy 
 * @param TstVoipValue.ch_id Channel ID
 * @param TstVoipValue.value 1: enable, 0: disable - energy detect
 * @param [out] TstVoipValue.value2 Detected energy (0~91 dB, or -1 indicates FIFO is empty) 
 * @see VOIP_MGR_ENERGY_DETECT TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_ENERGY_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#ifdef ENERGY_DET_PCM_IN
	extern int energy_out(uint32 ch_id);
	unsigned long flags;
	TstVoipValue stVoipValue;

	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));

	save_flags(flags); cli();
	/*
	energy_out return value : 0~ 91 dB.
	If return value = -1, it means FIFO is empty.
	*/
	eng_det_flag[stVoipValue.ch_id] = stVoipValue.value; // 1: enable, 0: disable - energy detect
	stVoipValue.value2 = energy_out(stVoipValue.ch_id);
	restore_flags(flags);
	//copy_to_user(user, &stVoipValue, sizeof(TstVoipValue));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stVoipValue, sizeof(TstVoipValue)))
	{
		return 0;
	}
#endif
	return 0;
}
#else
int do_mgr_VOIP_MGR_ENERGY_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#ifdef ENERGY_DET_PCM_IN
	extern int energy_out(uint32 ch_id);
	unsigned long flags;
	TstVoipValue stVoipValue;
	
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	save_flags(flags); cli();
	/*
	energy_out return value : max is 0dBm.
	If return value = -1, it means FIFO is empty.
	*/
	eng_det_flag[stVoipValue.ch_id] = stVoipValue.value; // 1: enable, 0: disable - energy detect
	stVoipValue.value2 = energy_out(stVoipValue.ch_id);
	restore_flags(flags);
	//copy_to_user(user, &stVoipValue, sizeof(TstVoipValue));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stVoipValue, sizeof(TstVoipValue)))
	{
		return 0;
	}
#endif
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_CODEC
 * @brief Set payload type (codec type), jitter buffer factor, VAD, frames per packet, and so on
 * @see VOIP_MGR_SETRTPPAYLOADTYPE TstVoipPayLoadTypeConfig 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SETRTPPAYLOADTYPE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	unsigned long flags;
	uint32 ch_id, m_id, s_id;
	TstVoipPayLoadTypeConfig stVoipPayLoadTypeConfig;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipPayLoadTypeConfig, (TstVoipPayLoadTypeConfig*)para, sizeof(TstVoipPayLoadTypeConfig));	

	ch_id = stVoipPayLoadTypeConfig.ch_id;
	m_id = stVoipPayLoadTypeConfig.m_id;
	s_id = API_GetSid(ch_id, m_id);
#ifdef PCM_LOOP_MODE_CONTROL
	if(LoopBackInfo[s_id].isLoopBack == 1) {
 #ifdef CONFIG_RTK_VOIP_SILENCE
		stVoipPayLoadTypeConfig.uPktFormat = rtpPayloadSilence;
 #else
		return 0;
 #endif
	}
#endif

#ifdef VOIP_RESOURCE_CHECK
	int pltype;
	pltype = stVoipPayLoadTypeConfig.uPktFormat;
	
	if ( 1 == GetCurrentVoipResourceStatus(pltype))//VOIP_RESOURCE_AVAILABLE
	{
		SetVoipResourceWeight( s_id, pltype );
	}
	else //VOIP_RESOURCE_UNAVAILABLE
	{
		stVoipPayLoadTypeConfig.uPktFormat = rtpPayloadSilence;
		resource_weight[s_id] = DEFAULT_WEIGHT;
		/* play IVR to user*/
#ifdef CONFIG_RTK_VOIP_IVR_TEXT
		char text[]={IVR_TEXT_ID_NO_RESOURCE, '\0'};
		extern unsigned int PlayIvrText2Speech( unsigned int chid, IvrPlayDir_t dir, const unsigned char *pText2Speech );
		PlayIvrText2Speech(ch_id, IVR_DIR_LOCAL, text);
	
		while(PollIvrPlaying(ch_id)){};
#endif 
		hc_SetPlayTone(ch_id, s_id, DSPCODEC_TONE_SIT_NOCIRCUIT, true, DSPCODEC_TONEDIRECTION_LOCAL);
	}
	

#endif
	PRINT_MSG("VOIP_MGR_SETRTPPAYLOADTYPE\n");
#if 0
	PRINT_MSG("ch_id = %d\n",ch_id);
	PRINT_MSG("m_id = %d\n",m_id);
	PRINT_MSG("s_id = %d\n",s_id);
	PRINT_MSG("uPktFormat = %d\n",stVoipPayLoadTypeConfig.uPktFormat);
	PRINT_MSG("nFramePerPacke t= %d\n",stVoipPayLoadTypeConfig.nFramePerPacket);
	PRINT_MSG("nG723Type = %d\n",stVoipPayLoadTypeConfig.nG723Type);
	PRINT_MSG("bVAD = %d\n",stVoipPayLoadTypeConfig.bVAD);
	PRINT_MSG("nG726Packing = %d\n",stVoipPayLoadTypeConfig.nG726Packing);
#endif

#if 0 // hard code to set G.729
	stVoipPayLoadTypeConfig.uPktFormat = 18;
	g_dynamic_pt_remote[s_id] = 18;
	g_dynamic_pt_local[s_id] = 18;
	PRINT_MSG("remote pt=%d, locat_pt=%d, sid=%d\n",
		g_dynamic_pt_remote[s_id], g_dynamic_pt_local[s_id], s_id);
#else
	g_dynamic_pt_remote[s_id] = stVoipPayLoadTypeConfig.remote_pt;
	g_dynamic_pt_local[s_id] = stVoipPayLoadTypeConfig.local_pt;
	PRINT_MSG("remote pt=%d, locat_pt=%d, sid=%d\n",
		g_dynamic_pt_remote[s_id], g_dynamic_pt_local[s_id], s_id);

#endif

#ifdef SUPPORT_V152_VBD	
	switch( stVoipPayLoadTypeConfig.uPktFormat_vbd ) {
	case rtpPayloadPCMU:
	case rtpPayloadPCMA:
		g_dynamic_pt_remote_vbd[s_id] = stVoipPayLoadTypeConfig.remote_pt_vbd;
		g_dynamic_pt_local_vbd[s_id] = stVoipPayLoadTypeConfig.local_pt_vbd;
		break;
	
	case rtpPayloadUndefined:
	default:
		// others are not support 
		stVoipPayLoadTypeConfig.uPktFormat_vbd = rtpPayloadUndefined;
		g_dynamic_pt_remote_vbd[s_id] = g_dynamic_pt_local_vbd[s_id] = rtpPayloadUndefined;
		break;
	}

	PRINT_MSG("VBD: remote pt=%d, locat_pt=%d, PktFormat=%d, sid=%d\n",
		g_dynamic_pt_remote_vbd[s_id], g_dynamic_pt_local_vbd[s_id], stVoipPayLoadTypeConfig.uPktFormat_vbd, s_id);
#endif

	astVoipPayLoadTypeConfig[s_id] = stVoipPayLoadTypeConfig;
        	
#if 0  //chiminer modify, pkshih: unused now
	astTwoChannelCfg[ch_id].ch_id = ch_id;
	astTwoChannelCfg[ch_id].s_id = s_id;
	astTwoChannelCfg[ch_id].uPktFormat = stVoipPayLoadTypeConfig.uPktFormat;
	astTwoChannelCfg[ch_id].channel_enable = 1;
#endif

#if 0
	/* Now, it is merely an experimental parameter. */
	stVoipPayLoadTypeConfig.nJitterFactor = 7;
#endif

#ifdef SUPPORT_V152_VBD
	V152_InitializeSession( s_id, 
		( stVoipPayLoadTypeConfig.uPktFormat_vbd != rtpPayloadUndefined ) );
#endif

	save_flags(flags); cli();
	DSP_CodecRestart(ch_id, s_id,
					 stVoipPayLoadTypeConfig.uPktFormat,
					 stVoipPayLoadTypeConfig.nFramePerPacket,
					 stVoipPayLoadTypeConfig.nG723Type,
					 stVoipPayLoadTypeConfig.bVAD,
					 stVoipPayLoadTypeConfig.nJitterDelay,
					 stVoipPayLoadTypeConfig.nMaxDelay,
					 stVoipPayLoadTypeConfig.nJitterFactor,
					 stVoipPayLoadTypeConfig.nG726Packing,
					 stVoipPayLoadTypeConfig.nPcmMode);
	restore_flags(flags);
	
#ifdef T38_STAND_ALONE_HANDLER
	if( stVoipPayLoadTypeConfig.uPktFormat == rtpPayloadT38_Virtual ) {
		if( stVoipPayLoadTypeConfig.bT38ParamEnable ) {
			t38Param_t t38Param = T38_DEFAULT_PARAM_LIST();
			t38Param.nMaxBuffer = stVoipPayLoadTypeConfig.nT38MaxBuffer;
			t38Param.nRateManagement = stVoipPayLoadTypeConfig.nT38RateMgt;
			T38_API_Initialize( ch_id, &t38Param );
		} else
			T38_API_Initialize( ch_id, NULL );
		PRINT_MSG("MGR: Initialize T38\n");
		t38RunningState[ ch_id ] = T38_START;
	} else
		t38RunningState[ ch_id ] = T38_STOP;
#endif
	
	return 0;
}
#else
int do_mgr_VOIP_MGR_SETRTPPAYLOADTYPE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	unsigned long flags;
	uint32 s_id;
	TstVoipPayLoadTypeConfig stVoipPayLoadTypeConfig;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipPayLoadTypeConfig, (TstVoipPayLoadTypeConfig*)para, sizeof(TstVoipPayLoadTypeConfig));	
	
	PRINT_MSG("VOIP_MGR_SETRTPPAYLOADTYPE(ch=%d, mid=%d)\n", stVoipPayLoadTypeConfig.ch_id, stVoipPayLoadTypeConfig.m_id);
	s_id = (2*stVoipPayLoadTypeConfig.ch_id + PROTOCOL__RTP - 1);
	save_flags(flags); cli();
	#ifdef T38_STAND_ALONE_HANDLER
	if( stVoipPayLoadTypeConfig.uPktFormat == rtpPayloadT38_Virtual )
	{
		if( stVoipPayLoadTypeConfig.bT38ParamEnable ) {
			t38Param_t t38Param = T38_DEFAULT_PARAM_LIST();
			t38Param.nMaxBuffer = stVoipPayLoadTypeConfig.nT38MaxBuffer;
			t38Param.nRateManagement = stVoipPayLoadTypeConfig.nT38RateMgt;
			T38_API_Initialize( ch_id, &t38Param );
		} else
			T38_API_Initialize( stVoipPayLoadTypeConfig.ch_id, NULL );
		PRINT_MSG("MGR: Initialize T38(%d)\n", stVoipPayLoadTypeConfig.ch_id);
		t38RunningState[ stVoipPayLoadTypeConfig.ch_id ] = T38_START;
		restore_flags(flags);
		return 0;
	}
	else
		t38RunningState[ stVoipPayLoadTypeConfig.ch_id ] = T38_STOP;
	#endif
	
#if 0 // hard code to set G.729
	stVoipPayLoadTypeConfig.uPktFormat = 4;//18;
	g_dynamic_pt_remote[s_id] = 4;//18;
	g_dynamic_pt_local[s_id] = 4;//18;
	PRINT_MSG("remote pt=%d, locat_pt=%d, sid=%d\n",
		g_dynamic_pt_remote[s_id], g_dynamic_pt_local[s_id], s_id);
#else
	g_dynamic_pt_remote[s_id] = stVoipPayLoadTypeConfig.remote_pt;
	g_dynamic_pt_local[s_id] = stVoipPayLoadTypeConfig.local_pt;
	PRINT_MSG("remote pt=%d, locat_pt=%d, sid=%d\n",
		g_dynamic_pt_remote[s_id], g_dynamic_pt_local[s_id], s_id);
#endif

#ifdef SUPPORT_V152_VBD	
	switch( stVoipPayLoadTypeConfig.uPktFormat_vbd ) {
	case rtpPayloadPCMU:
	case rtpPayloadPCMA:
		g_dynamic_pt_remote_vbd[s_id] = stVoipPayLoadTypeConfig.remote_pt_vbd;
		g_dynamic_pt_local_vbd[s_id] = stVoipPayLoadTypeConfig.local_pt_vbd;
		break;
	
	case rtpPayloadUndefined:
	default:
		// others are not support 
		stVoipPayLoadTypeConfig.uPktFormat_vbd = rtpPayloadUndefined;
		g_dynamic_pt_remote_vbd[s_id] = g_dynamic_pt_local_vbd[s_id] = rtpPayloadUndefined;
		break;
	}
	
	PRINT_MSG("VBD: remote pt=%d, locat_pt=%d, PktFormat=%d, sid=%d\n",
		g_dynamic_pt_remote_vbd[s_id], g_dynamic_pt_local_vbd[s_id], stVoipPayLoadTypeConfig.uPktFormat_vbd, s_id);
#endif
	
	RtkAc49xApiSetRtpChannelConfiguration(stVoipPayLoadTypeConfig.ch_id, s_id, g_dynamic_pt_remote[s_id], stVoipPayLoadTypeConfig.nG723Type, stVoipPayLoadTypeConfig.bVAD);
	if (CHANNEL_STATE__ACTIVE_RTP != RtkAc49xGetChannelState(stVoipPayLoadTypeConfig.ch_id))
		RtkAc49xApiSetVoiceJBDelay(stVoipPayLoadTypeConfig.ch_id, 10*stVoipPayLoadTypeConfig.nMaxDelay, 10*stVoipPayLoadTypeConfig.nJitterDelay, stVoipPayLoadTypeConfig.nJitterFactor);
	
	if (gSetByassMode[stVoipPayLoadTypeConfig.ch_id] == 1) // fax bypass
	{
		RtkAc49xApiSetIntoBypassMode(stVoipPayLoadTypeConfig.ch_id, FAX_BYPASS);
	}
	else if (gSetByassMode[stVoipPayLoadTypeConfig.ch_id] == 2) // modem bypass
	{
		RtkAc49xApiSetIntoBypassMode(stVoipPayLoadTypeConfig.ch_id, MODEM_BYPASS);
	}
	
	restore_flags(flags);
	return 0;
}
#endif 

/**
 * @ingroup VOIP_DSP_CODEC
 * @brief Stop codec 
 * @param TstVoipValue.ch_id Channel ID
 * @param TstVoipValue.m_id Media ID 
 * @see VOIP_MGR_DSPCODECSTOP TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_DSPCODECSTOP(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	unsigned long flags;
	uint32 s_id;
	TstVoipValue stVoipValue;

	PRINT_MSG("VOIP_MGR_DSPCODECSTOP\n");
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));	

	s_id = API_GetSid(stVoipValue.ch_id, stVoipValue.m_id);
#if 1
	PRINT_MSG("ch_id =%d, m_id=%d, s_id=%d\n", stVoipValue.ch_id, stVoipValue.m_id, s_id);
#endif
	save_flags(flags); cli();
	DspcodecStop(s_id);
#ifdef T38_STAND_ALONE_HANDLER
	t38RunningState[ stVoipValue.ch_id ] = T38_STOP;
#endif
#ifdef SUPPORT_V152_VBD
	V152_InitializeSession( s_id, 0 );
#endif
	restore_flags(flags);
	return 0;
}
#else
int do_mgr_VOIP_MGR_DSPCODECSTOP(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	unsigned long flags;
	uint32 s_id;
	TstVoipValue stVoipValue;

	PRINT_MSG("VOIP_MGR_DSPCODECSTOP\n");
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));

	save_flags(flags); cli();
#ifdef T38_STAND_ALONE_HANDLER
	t38RunningState[ stVoipValue.ch_id ] = T38_STOP;
#endif
#ifdef SUPPORT_V152_VBD
	V152_InitializeSession( s_id, 0 );
#endif
	restore_flags(flags);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_FAXMODEM
 * @brief Check FAX or MODEM CED tone  
 * @param TstVoipCfg.ch_id Channel ID 
 * @param [out] TstVoipCfg.enable Dectected type. (1: fax, 2: local-modem, 3: remote-modem) 
 * @see VOIP_MGR_FAX_MODEM_PASS_CFG TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FAX_MODEM_PASS_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	extern char CED_routine(int chid);
	TstVoipCfg stVoipCfg;
	
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));

	/* 1: fax, 2: local-modem, 3: remote-modem */
	stVoipCfg.enable = CED_routine(stVoipCfg.ch_id);
	if (fax_modem_det_mode[stVoipCfg.ch_id] == 1)
	{
		if (stVoipCfg.enable > 0)
			stVoipCfg.enable = 1;
	}
	else if (fax_modem_det_mode[stVoipCfg.ch_id] == 2)
	{
		if (stVoipCfg.enable == 1)
			stVoipCfg.enable = 0;
	}
	//copy_to_user(user, &stVoipCfg, sizeof(TstVoipCfg));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stVoipCfg, sizeof(TstVoipCfg)))
	{
		return 0;
	}
	return 0;
}
#else
int do_mgr_VOIP_MGR_FAX_MODEM_PASS_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	extern int fax_modem_out(uint32 ch_id);
	TstVoipCfg stVoipCfg;

	/** FAX/MODEM PASS THROUGH **/
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	/* 1: fax, 2: modem */
	stVoipCfg.enable = fax_modem_out(stVoipCfg.ch_id);
	if (stVoipCfg.enable != 0)
	{
		PRINT_MSG("Get FAX Event from FIFO, ch=%d, enable=%d\n", stVoipCfg.ch_id, stVoipCfg.enable);
	}
	//copy_to_user(user, &stVoipCfg, sizeof(TstVoipCfg));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stVoipCfg, sizeof(TstVoipCfg)))
	{
		return 0;
	}
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_FAXMODEM
 * @brief Set FAX off-hook flag, when phone was off-hook 
 * @param TstVoipCfg.ch_id Channel ID
 * @param TstVoipCfg.enable FAX hook status (0: on-hook, 1: off-hook) 
 * @see VOIP_MGR_FAX_OFFHOOK TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FAX_OFFHOOK(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	//when phone offhook, set fax_offhook[]=1;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	fax_offhook[stVoipCfg.ch_id] = stVoipCfg.enable;
	return 0;
}
#else
int do_mgr_VOIP_MGR_FAX_OFFHOOK(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	//PRINT_MSG("This IO Ctrl is NOT support at AudioCodes solution.\n");
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_FAXMODEM
 * @brief Determine whether FAX is complete 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable FAX end flag (1: end, 0: not end) 
 * @see VOIP_MGR_FAX_END_DETECT TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FAX_END_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	stVoipCfg.enable = fax_end_flag[stVoipCfg.ch_id];
	//copy_to_user(user, &stVoipCfg, sizeof(TstVoipCfg));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no,&stVoipCfg, sizeof(TstVoipCfg)))
	{
		return 0;
	}
	return 0;//t.38 fax end detect, 1:fax end.
}
#else
int do_mgr_VOIP_MGR_FAX_END_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_FAXMODEM
 * @brief Set FAX or modem tone detection 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.cfg Enable (1) or disable (0) tone detection 
 * @see VOIP_MGR_SET_FAX_MODEM_DET TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_FAX_MODEM_DET(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	fax_modem_det_mode[stVoipCfg.ch_id] = stVoipCfg.cfg;
	PRINT_MSG("VOIP_MGR_SET_FAX_MODEM_DET = %d\n", stVoipCfg.cfg);
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_FAX_MODEM_DET(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_FAXMODEM
 * @brief Report FAX DIS detection status 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable DIS detection status (1: detected, 0: not detected)
 * @see VOIP_MGR_FAX_DIS_DETECT TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FAX_DIS_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));

	extern int fax_v21_dis_get(unsigned int chid);
	stVoipCfg.enable = fax_v21_dis_get(stVoipCfg.ch_id);

	//copy_to_user(user, &stVoipCfg, sizeof(TstVoipCfg));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no,&stVoipCfg, sizeof(TstVoipCfg)))
	{
		return 0;
	}
	
	return 0;//t.38 v21 dis detect, 1:dis(Digital Identification Signal) detected.
}
#else
int do_mgr_VOIP_MGR_FAX_DIS_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_FAXMODEM
 * @brief Report FAX DIS TX detection status 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable DIS TX detection status{network -> ATA -> FAX} (1: detected, 0: not detected)
 * @see VOIP_MGR_FAX_DIS_DETECT TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FAX_DIS_TX_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	extern int fax_v21_dis_tx_get(unsigned int chid);
	TstVoipCfg stVoipCfg;

	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));

	stVoipCfg.enable = fax_v21_dis_tx_get(stVoipCfg.ch_id);

	//copy_to_user(user, &stVoipCfg, sizeof(TstVoipCfg));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no,&stVoipCfg, sizeof(TstVoipCfg)))
	{
		return 0;
	}
	
	return 0;//t.38 v21 dis tx direction detect, 1:dis(Digital Identification Signal) detected.
}
#else
int do_mgr_VOIP_MGR_FAX_DIS_TX_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_FAXMODEM
 * @brief Report FAX DIS RX detection status 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable DIS RX detection status{FAX -> ATA -> network} (1: detected, 0: not detected)
 * @see VOIP_MGR_FAX_DIS_DETECT TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FAX_DIS_RX_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	extern int fax_v21_dis_rx_get(unsigned int chid);
	TstVoipCfg stVoipCfg;

	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));

	stVoipCfg.enable = fax_v21_dis_rx_get(stVoipCfg.ch_id);

	//copy_to_user(user, &stVoipCfg, sizeof(TstVoipCfg));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no,&stVoipCfg, sizeof(TstVoipCfg)))
	{
		return 0;
	}
	return 0;//t.38 v21 dis rx direction detect, 1:dis(Digital Identification Signal) detected.
}
#else
int do_mgr_VOIP_MGR_FAX_DIS_RX_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_FAXMODEM
 * @brief Report FAX DCN detection status 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable DCN detection status (1: detected, 0: not detected)
 * @see VOIP_MGR_FAX_DCN_DETECT TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FAX_DCN_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	copy_from_user(&stVoipCfg, (TstVoipCfg *)user, sizeof(TstVoipCfg));

	extern int fax_v21_dcn_get(unsigned int chid);
	stVoipCfg.enable = fax_v21_dcn_get(stVoipCfg.ch_id);
	stVoipCfg.ret_val = 0;
	copy_to_user(user, &stVoipCfg, sizeof(TstVoipCfg));
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no,&stVoipCfg, sizeof(TstVoipCfg)))
	{
		return 0;
	}
	return 0;//t.38 v21 dcn detect, 1:dis(Disconnect) detected.
}
#else
int do_mgr_VOIP_MGR_FAX_DCN_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_FAXMODEM
 * @brief Report FAX DCN TX detection status 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable DCN TX detection status{network -> ATA -> FAX} (1: detected, 0: not detected)
 * @see VOIP_MGR_FAX_DCN_DETECT TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FAX_DCN_TX_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	extern int fax_v21_dcn_tx_get(unsigned int chid);
	TstVoipCfg stVoipCfg;

	copy_from_user(&stVoipCfg, (TstVoipCfg *)user, sizeof(TstVoipCfg));



	stVoipCfg.enable = fax_v21_dcn_tx_get(stVoipCfg.ch_id);
	stVoipCfg.ret_val = 0;
	copy_to_user(user, &stVoipCfg, sizeof(TstVoipCfg));
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no,&stVoipCfg, sizeof(TstVoipCfg)))
	{
		return 0;
	}
	return 0;//t.38 v21 dcn tx direction detect, 1:dcn(Disconnect) detected.
}
#else
int do_mgr_VOIP_MGR_FAX_DCN_TX_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_FAXMODEM
 * @brief Report FAX DCN RX detection status 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable DCN RX detection status{FAX -> ATA -> network} (1: detected, 0: not detected)
 * @see VOIP_MGR_FAX_DCN_DETECT TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FAX_DCN_RX_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	extern int fax_v21_dcn_rx_get(unsigned int chid);
	TstVoipCfg stVoipCfg;

	copy_from_user(&stVoipCfg, (TstVoipCfg *)user, sizeof(TstVoipCfg));



	stVoipCfg.enable = fax_v21_dcn_rx_get(stVoipCfg.ch_id);
	stVoipCfg.ret_val = 0;
	copy_to_user(user, &stVoipCfg, sizeof(TstVoipCfg));
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no,&stVoipCfg, sizeof(TstVoipCfg)))
	{
		return 0;
	}
	return 0;//t.38 v21 dcn rx direction detect, 1:dcn(Disconnect) detected.
}
#else
int do_mgr_VOIP_MGR_FAX_DCN_RX_DETECT(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_LEC
 * @brief Set tail length of LEC 
 * @param TstVoipValue.ch_id Channel ID 
 * @param TstVoipValue.value Tail length (ms) 
 * @see VOIP_MGR_SET_ECHO_TAIL_LENGTH TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_ECHO_TAIL_LENGTH(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	//stVoipValue.value : unit is ms
#ifdef EXPER_AEC
	AEC_g168_set_TailLength(stVoipValue.ch_id, stVoipValue.value);
#endif
#ifdef SUPPORT_LEC_G168_ISR
	#ifdef CONFIG_RTK_VOIP_DRIVERS_IP_PHONE
	LEC_g168_set_TailLength(stVoipValue.ch_id, 16);
	//printk("ippohne_lec_tail\n");
		#ifdef T_TYPE_ECHO_CAN
	LEC_g168_set_TailLength(3, 16);// for handset.
	LEC_g168_init(3, LEC|LEC_NLP);// for handset
		#endif
	#else
	LEC_g168_set_TailLength(stVoipValue.ch_id, stVoipValue.value);
	#endif

	#ifdef CONFIG_RTK_VOIP_DRIVERS_IP_PHONE
		#ifdef T_TYPE_ECHO_CAN
	LEC_g168_init(stVoipValue.ch_id, LEC|LEC_NLP);
		#else
	LEC_g168_init(stVoipValue.ch_id, LEC|LEC_NLP|LEC_NLP_CNG);
		#endif
	#else
	LEC_g168_init(stVoipValue.ch_id, LEC|LEC_NLP);
	#endif
#endif
	PRINT_MSG("Set CH%d Echo Tail Length = %dms\n", stVoipValue.ch_id, stVoipValue.value);
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_ECHO_TAIL_LENGTH(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;

	/** Echo Canceller Length Config **/
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	Tac49xEchoCancelerLength ec_length;
	
	if (stVoipValue.value == 4) //stVoipValue.value : unit is ms
		ec_length = ECHO_CANCELER_LENGTH__4_MSEC;
	else if (stVoipValue.value == 8)
		ec_length = ECHO_CANCELER_LENGTH__8_MSEC;
	else if (stVoipValue.value == 16)
		ec_length = ECHO_CANCELER_LENGTH__16_MSEC;
	else if (stVoipValue.value == 24)
		ec_length = ECHO_CANCELER_LENGTH__24_MSEC;
	else if (stVoipValue.value == 32)
		ec_length = ECHO_CANCELER_LENGTH__32_MSEC;
	else
	{
		ec_length = ECHO_CANCELER_LENGTH__4_MSEC;
		PRINT_G("Warning: LEC tail length %d isn't supported. Use default length = %d msec\n", stVoipValue.value, 4*(ec_length+1));
	}
	RtkAc49xApiUpdateEchoCancellerLength(stVoipValue.ch_id, ec_length);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_LEC
 * @brief Turn on or off LEC 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable Turn on (1) or off (0) LEC 
 * @see VOIP_MGR_SET_G168_LEC_CFG TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_G168_LEC_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));

	/* 0: LEC disable  1: LEC enable */
	if (stVoipCfg.enable == 1)
#ifdef SUPPORT_LEC_G168_ISR
	{
		LEC_g168_enable(stVoipCfg.ch_id);
#ifdef CONFIG_RTK_VOIP_DRIVERS_IP_PHONE
		#ifdef T_TYPE_ECHO_CAN
		LEC_g168_enable(3);// for handset.
		#endif
#endif
	}
	else if (stVoipCfg.enable == 0)
	{
		LEC_g168_disable(stVoipCfg.ch_id);
#ifdef CONFIG_RTK_VOIP_DRIVERS_IP_PHONE
		#ifdef T_TYPE_ECHO_CAN
		LEC_g168_disable(3);// for handset.
		#endif
#endif
	}
#endif
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_G168_LEC_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));

	/* 0: LEC disable  1: LEC enable */
	if (stVoipCfg.enable == 1)
	{
		RtkAc49xApi_LEC_enable(stVoipCfg.ch_id);
	}
	else if (stVoipCfg.enable == 0)
	{
		RtkAc49xApi_LEC_disable(stVoipCfg.ch_id);
	}
	return 0;
}
#endif

//#define AEC_TEST 1

void aec_process_block_10ms(unsigned int chid, int16_t *output, int16_t *est_echo_out, const int16_t *input, const int16_t *echo);
void AEC_g168_set_TailLength(unsigned int chid, unsigned int tail_length);
void AEC_g168_init(unsigned int chid, unsigned char type);

void LEC_g168(char chid, Word16 *pRin, Word16 *pSin, Word16 *pEx);
void LEC_g168_set_TailLength(unsigned int chid, unsigned int tail_length);
void LEC_g168_init(unsigned char chid, unsigned char type);

static int ec_test_mode;
int do_mgr_VOIP_MGR_GET_EC_DEBUG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	PRINT_R("Need to implement do_mgr_VOIP_MGR_GET_EC_DEBUG \n");
	return 0;
}

/**
 * @ingroup VOIP_DSP_DTMF
 * @brief Set DTMF detection threshold
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.cfg DTMF detection threshold
 * @see VOIP_MGR_DTMF_DET_THRESHOLD TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_DTMF_DET_THRESHOLD(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	PRINT_MSG("VOIP_MGR_DTMF_DET_THRESHOLD:ch_id = %d, threshold = -%d dBm\n", stVoipCfg.ch_id, stVoipCfg.cfg);

	dtmf_det_threshold_set(stVoipCfg.ch_id, stVoipCfg.cfg);

	return 0;
}
#else
int do_mgr_VOIP_MGR_DTMF_DET_THRESHOLD(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	PRINT_R("VOIP_MGR_DTMF_DET_THRESHOLD: not support(or implement) for ACMW\n");
	
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_DTMF
 * @brief Enable or disable DTMF detection 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable Enable (1) or disable (0) DTMF detection 
 * @see VOIP_MGR_DTMF_CFG TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_DTMF_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	PRINT_MSG("VOIP_MGR_DTMF_CFG:ch_id = %d, enable = %d\n", stVoipCfg.ch_id, stVoipCfg.enable);
#ifdef _chiminer_slic2_ch1
	stVoipCfg.ch_id=1;
	PRINT_MSG("stVoipCfg.ch_id=%d\n",stVoipCfg.ch_id);
#endif
	if(stVoipCfg.enable == 1)
		dtmf_start((char)stVoipCfg.ch_id);
	else if ( stVoipCfg.enable == 0)
		dtmf_stop((char) stVoipCfg.ch_id);
	return 0;
}
#else
int do_mgr_VOIP_MGR_DTMF_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));

	/** DTMF Event **/
		//PRINT_MSG("VOIP_MGR_DTMF_CFG:ch_id = %d, enable = %d\n", stVoipCfg.ch_id, stVoipCfg.enable);
		//RtkAc49xApiSetDtmfDetection(stVoipCfg.ch_id, stVoipCfg.enable);
		// NOTE:
		// Can NOT call RtkAc49xApiSetDtmfDetection() here, or it will cause acmw to close regular rtp fail.
		// Developer should make sure that enable DTMF det after acmw channel open, and disable DTMF det before acmw channel close.
		// Or it will not work and may result some bad effect.
		// Now, enable / disable DTMF det is integrated in IO Ctrl VOIP_MGR_SLIC_ONHOOK_ACTION and VOIP_MGR_SLIC_OFFHOOK_ACTION.
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_DTMF
 * @brief Retrieve detected DTMF digits 
 * @param TstVoipValue.ch_id Channel ID 
 * @param [out] TstVoipValue.value A DTMF digit. If digit is 'z', it means FIFO is empty. 
 * @see VOIP_MGR_DTMF_GET TstVoipValue 
 */
int do_mgr_VOIP_MGR_DTMF_GET(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#if 1 
	// no usage for DSP device
#else
	extern int dtmf_out(uint32 ch_id, char * output);
	
	unsigned long flags;
	TstVoipValue stVoipValue;

	copy_from_user(&stVoipValue, (TstVoipValue *)user, sizeof(TstVoipValue));
	save_flags(flags); cli();
	dtmf_out(stVoipValue.ch_id, &stVoipValue.value);
	restore_flags(flags);
	//PRINT_MSG(" m:%c ", stVoipValue.value);
	copy_to_user(user, &stVoipValue, sizeof(TstVoipValue));
#endif
	return 0;
}

/**
 * @ingroup VOIP_DSP_DTMF
 * @brief Set DTMF mode to send out 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable DTMF mode. (0:rfc2833  1: sip info  2: inband)
 * @see VOIP_MGR_SET_DTMF_MODE TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_DTMF_MODE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	// 0:rfc2833  1: sip info  2: inband
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	if (stVoipCfg.enable == 2)
	{
		dtmf_removal_flag[stVoipCfg.ch_id] = 0; /* No DTMF removal */
		dtmf_mode[stVoipCfg.ch_id] = 2;
	}
	else if (stVoipCfg.enable == 1)
	{
		dtmf_removal_flag[stVoipCfg.ch_id] = 1; /* Do DTMF removal */
		dtmf_mode[stVoipCfg.ch_id] = 1;
	}
	else if (stVoipCfg.enable == 0)
	{
		dtmf_removal_flag[stVoipCfg.ch_id] = 1; /* Do DTMF removal */
		dtmf_mode[stVoipCfg.ch_id] = 0;
	}
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_DTMF_MODE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	if (stVoipCfg.enable == 2) // Inband
	{
		dtmf_mode[stVoipCfg.ch_id] = 2;
		RtkAc49xApiSetIbsTransferMode(stVoipCfg.ch_id, IBS_TRANSFER_MODE__TRANSPARENT_THROUGH_VOICE);
	}
	else if (stVoipCfg.enable == 1) //SIP Info
	{
		dtmf_mode[stVoipCfg.ch_id] = 1;
		RtkAc49xApiSetIbsTransferMode(stVoipCfg.ch_id, IBS_TRANSFER_MODE__RELAY_DISABLE_VOICE_MUTE);
	}
	else if (stVoipCfg.enable == 0) // RFC2833
	{
		dtmf_mode[stVoipCfg.ch_id] = 0;
		/*
		 * Here, NOT set the ACMW Ibs Transfer Mode to RFC2833.
		 * The time to set ACMW Ibs Transfer Mode to RFC2833 is in time of setting RTP session.
		 * See the IO control VOIP_MGR_SET_SESSION.
		 */
	}
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_DTMF
 * @brief Send RFC2833 DTMF packet 
 * @see VOIP_MGR_SEND_RFC2833_PKT_CFG TstVoip2833 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SEND_RFC2833_PKT_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoip2833 stRFC2833;
	extern int send_2833_by_ap[];

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stRFC2833, (TstVoip2833*)para, sizeof(TstVoip2833));

	//printk( "VOIP_MGR_SEND_RFC2833_PKT_CFG:chid=%d,sid=%d\n", stRFC2833.ch_id, stRFC2833.sid );
#ifdef SUPPORT_RFC_2833

	#ifdef SEND_RFC2833_ISR
	#ifdef CONFIG_RTK_VOIP_IP_PHONE
	send_2833_by_ap[stRFC2833.ch_id] = 1;
	#endif
	if (send_2833_by_ap[stRFC2833.ch_id] == 1)
	{
		extern unsigned char send_2833_count_down[MAX_VOIP_CH_NUM];
		extern int sned_flag[MAX_VOIP_CH_NUM];
		extern int g_digit[MAX_VOIP_CH_NUM];
		
		sned_flag[ stRFC2833.ch_id ] = 1;
		g_digit[ stRFC2833.ch_id ] = stRFC2833.digit;
		send_2833_count_down[ stRFC2833.ch_id ] = stRFC2833.duration / ( PCM_PERIOD * 10 );
		PRINT_R("set send_2833_count_down[%d] to %d\n", stRFC2833.ch_id, send_2833_count_down[ stRFC2833.ch_id ]);
		// = 100 / ( PCM_PERIOD * 10 );	/* 100ms */
	}
	#endif
	/* Thlin: Send RFC2833 packets has been move to PCM_RX() */
#endif
	return 0;
}
#else
int do_mgr_VOIP_MGR_SEND_RFC2833_PKT_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoip2833 stRFC2833;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stRFC2833, (TstVoip2833*)para, sizeof(TstVoip2833));
	/* This IO ctrl is not necessary for acmw. */
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_DTMF
 * @brief Decide RFC2833 sent by AP or DSP 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable Sent by DSP (0) by AP (1) 
 * @see VOIP_MGR_SEND_RFC2833_BY_AP TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SEND_RFC2833_BY_AP(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	extern int send_2833_by_ap[];
	send_2833_by_ap[stVoipCfg.ch_id] = stVoipCfg.enable; /* 0: by DSP 1: by AP */
	return 0;
}
#else
int do_mgr_VOIP_MGR_SEND_RFC2833_BY_AP(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif


/**
 * @ingroup VOIP_DSP_DTMF
 * @brief Limit the Max. RFC2833 DTMF Duration
 * @see VOIP_MGR_LIMIT_MAX_RFC2833_DTMF_DURATION TstVoip2833 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_LIMIT_MAX_RFC2833_DTMF_DURATION(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#ifdef SUPPORT_RFC2833_PLAY_LIMIT
	TstVoip2833 stRFC2833;
	uint32 SessNum, sid, i;
	extern int bRfc2833_play_limit[];
	extern int rfc2833_play_limit_ms[];
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stRFC2833, (TstVoip2833*)para, sizeof(TstVoip2833));
	
	SessNum = chanInfo_GetRegSessionNum(stRFC2833.ch_id);
	
	for(i=0; i<SessNum; i++)
	{
		sid = chanInfo_GetRegSessionID(stRFC2833.ch_id, i);
		bRfc2833_play_limit[sid] = stRFC2833.bEnable;
		rfc2833_play_limit_ms[sid] = stRFC2833.duration;
		//PRINT_MSG("limit: %d, ms: %d, sid=%d\n", bRfc2833_play_limit[sid], rfc2833_play_limit_ms[sid], sid);
	}
#else
	PRINT_R("VOIP_MGR_LIMIT_MAX_RFC2833_DTMF_DURATION is not supported.\n");
#endif
	
	return 0;
}
#else
int do_mgr_VOIP_MGR_LIMIT_MAX_RFC2833_DTMF_DURATION(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoip2833 stRFC2833;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stRFC2833, (TstVoip2833*)para, sizeof(TstVoip2833));
	
	/* This IO ctrl is not support for acmw. */
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_DTMF
 * @brief Play SIP info DTMF locally 
 * @param TstVoipValue.ch_id Channel ID 
 * @param TstVoipValue.m_id Media ID 
 * @param TstVoipValue.value DTMF digit 
 * @param TstVoipValue.value5 DTMF duration (ms) 
 * @see VOIP_MGR_PLAY_SIP_INFO TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_PLAY_SIP_INFO(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	uint32 s_id;
	TstVoipValue stVoipValue;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));

#if defined (PULSE_DIAL_GEN) && defined (OUTBAND_AUTO_PULSE_DIAL_GEN)
	if ( (1 == DAA_Get_Dial_Mode(stVoipValue.ch_id)) && ( 1 == Is_DAA_Channel(stVoipValue.ch_id)) )
	{
		pulse_dial_in_cch(stVoipValue.ch_id, stVoipValue.value);
		
	}
	else
#endif
	{
		int index;
	
		s_id = API_GetSid(stVoipValue.ch_id, stVoipValue.m_id);
		if ( ((g_SIP_Info_buf_w[s_id]+1)%10) == g_SIP_Info_buf_r[s_id])
		{
			PRINT_MSG("SIP Info Buffer Overflow, sid= %d\n", s_id);
			return 0;
		}
		index = g_SIP_Info_buf_w[s_id];
		g_SIP_Info_tone_buf[s_id][index] = stVoipValue.value;
		g_SIP_Info_time_buf[s_id][index] = stVoipValue.value5/10;
		g_SIP_Info_buf_w[s_id] = (g_SIP_Info_buf_w[s_id] + 1)%10;
		//printk("(%d, %d)\n", stVoipValue.value5, g_SIP_Info_time_buf[s_id][index]);
		//printk("(%d, %d)\n", s_id, index);
	}
	
	return 0;
}
#else
int do_mgr_VOIP_MGR_PLAY_SIP_INFO(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	RtkAc49xApiGenDtmfTone(stVoipValue.ch_id, stVoipValue.value, stVoipValue.value5, 0 /*Off-duration*/, IBS_STRING_GENERATOR_REDIRECTION__INTO_DECODER_OUTPUT);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief Generate DTMF caller ID 
 * @param TstVoipCID.ch_id Channel ID 
 * @param TstVoipCID.string Caller ID - phonenumber 
 * @see VOIP_MGR_DTMF_CID_GEN_CFG TstVoipCID 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_DTMF_CID_GEN_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	//ring_struct ring;
	TstVoipCID stCIDstr;
	extern unsigned char ioctrl_ring_set[];
	extern volatile unsigned int fxs_ringing[];
	extern volatile unsigned int daa_ringing;

#ifdef SW_DTMF_CID
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	PRINT_MSG("VOIP_MGR_DTMF_CID_GEN_CFG, ch=%d\n", stCIDstr.ch_id);

	if ( 1 == Is_DAA_Channel(stCIDstr.ch_id))
	{
		//printk("chid%d is not for SLIC, can not gen DTMF caller ID!\n", stCIDstr.ch_id);
		return 0;
	}
	
	if ( (strcmp(stCIDstr.string, "P") != 0) && (strcmp(stCIDstr.string, "O") != 0) )
	{
		//ring.CH = stCIDstr.ch_id;
		//ring.ring_set = 1;

		if(dtmf_cid_info[stCIDstr.ch_id].bBefore1stRing == 0)// send DTMF callerid after ring.
		{
			//FXS_Ring(&ring);
			FXS_Ring( stCIDstr.ch_id, 1 );
			//ioctrl_ring_set[stCIDstr.ch_id] = ring.ring_set + (0x1<<1);
            //printk("m:ioctrl_ring_set[%d]=%d\n", stCIDstr.ch_id, ioctrl_ring_set[stCIDstr.ch_id]);
			mdelay(100);// th: add delay before check 1st Ring off.
            printk("1st ring..\n");

#ifdef CONFIG_RTK_VOIP_LED
            if (daa_ringing == 0)
                fxs_ringing[stCIDstr.ch_id] = 1;
            led_state_watcher(stCIDstr.ch_id);
#endif
		}

		ioctrl_ring_set[stCIDstr.ch_id] = ring.ring_set + (0x1<<1); //Must to keep DSP auto-Ring normal.
		//PRINT_Y("ioctrl_ring_set[%d] = %d\n", stCIDstr.ch_id, ioctrl_ring_set[stCIDstr.ch_id]);

		dtmf_cid_state[stCIDstr.ch_id]=1;
		strcpy(dtmf_cid_info[stCIDstr.ch_id].data, stCIDstr.string);
	}
	else
	{
		//ring.CH = stCIDstr.ch_id;
		//ring.ring_set = 1;
		//FXS_Ring(&ring);
		FXS_Ring( stCIDstr.ch_id, 1 );
		ioctrl_ring_set[stCIDstr.ch_id] = ring.ring_set + (0x1<<1);
		
		PRINT_MSG("Not Support 'Private' DTMF Caller ID(ch=%d).\n", stCIDstr.ch_id);
	}
#endif
	return 0;
}
#else
int do_mgr_VOIP_MGR_DTMF_CID_GEN_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	if ( 1 == Is_DAA_Channel(stCIDstr.ch_id))
	{
		//printk("chid%d is not for SLIC, can not gen DTMF caller ID!\n", stCIDstr.ch_id);
		return 0;
	}

	if ( (strcmp(stCIDstr.string, "P") != 0) && (strcmp(stCIDstr.string, "O") != 0) )
	{
		//ring.CH = stCIDstr.ch_id;
		//ring.ring_set = 1;

		if(dtmf_cid_info[stCIDstr.ch_id].bBefore1stRing == 0)// send DTMF callerid after ring.
		{
			//FXS_Ring(&ring);
			FXS_Ring( stCIDstr.ch_id, 1 );
			//ioctrl_ring_set[stCIDstr.ch_id] = ring.ring_set + (0x1<<1);
            //printk("m:ioctrl_ring_set[%d]=%d\n", stCIDstr.ch_id, ioctrl_ring_set[stCIDstr.ch_id]);
			mdelay(100);// th: add delay before check 1st Ring off.
            printk("1st ring..\n");

#ifdef CONFIG_RTK_VOIP_LED
            if (daa_ringing == 0)
                fxs_ringing[stCIDstr.ch_id] = 1;
            led_state_watcher(stCIDstr.ch_id);
#endif
		}
		
		ioctrl_ring_set[stCIDstr.ch_id] = ring.ring_set + (0x1<<1); //Must to keep DSP auto-Ring normal.

		dtmf_cid_state[stCIDstr.ch_id]=1;
		strcpy(dtmf_cid_info[stCIDstr.ch_id].data, stCIDstr.string);
		RtkAc49xApiSendDtmfCallerId(stCIDstr.ch_id, 0, dtmf_cid_info[stCIDstr.ch_id].data);

		/* Auto Ring */
		if (gRingGenAfterCid[stCIDstr.ch_id] == 0)
		{
			gFirstRingOffTimeOut[stCIDstr.ch_id] = jiffies + (HZ*gRingCadOff[stCIDstr.ch_id]/1000);
			//printk("=>%d, J=%d\n", gFirstRingOffTimeOut[stCIDstr.ch_id], jiffies);
			gRingGenAfterCid[stCIDstr.ch_id] = 1;
			//printk("1: gRingGenAfterCid[%d] = %d\n", stCIDstr.ch_id, gRingGenAfterCid[stCIDstr.ch_id]);
		}
	}
	else
	{
		//ring.CH = stCIDstr.ch_id;
		//ring.ring_set = 1;
		//FXS_Ring(&ring);
		FXS_Ring( stCIDstr.ch_id, 1 );
		ioctrl_ring_set[stCIDstr.ch_id] = ring.ring_set + (0x1<<1);
		PRINT_MSG("Not Support 'Private' DTMF Caller ID(ch=%d).\n", stCIDstr.ch_id);
	}
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief Get caller ID state 
 * @param TstVoipCID.ch_id Channel ID 
 * @param [out] TstVoipCID.cid_state Caller ID state 
 * @see VOIP_MGR_GET_CID_STATE_CFG TstVoipCID 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_GET_CID_STATE_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;
#ifdef SW_DTMF_CID
	
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	stCIDstr.cid_state = dtmf_cid_state[stCIDstr.ch_id];
	//copy_to_user(user, &stCIDstr, sizeof(TstVoipCID));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stCIDstr, sizeof(TstVoipCID)))
	{
		return 0;
	}
	//PRINT_MSG("VOIP_MGR_GET_CID_STATE_CFG = %d\n", stCIDstr.cid_state);
#endif
	return 0;
}
#else
int do_mgr_VOIP_MGR_GET_CID_STATE_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;

	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	stCIDstr.cid_state = dtmf_cid_state[stCIDstr.ch_id];
	//copy_to_user(user, &stCIDstr, sizeof(TstVoipCID));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stCIDstr, sizeof(TstVoipCID)))
	{
		return 0;
	}
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief Generate FSK caller ID 
 * @param TstVoipCID.ch_id Channel ID 
 * @param TstVoipCID.cid_mode Service type (0-type 1 (on-hook), 1-type 2(off-hook))
 * @param TstVoipCID.string Caller ID - Phonenumber  
 * @param TstVoipCID.string2 Caller ID - Date and time 
 * @param TstVoipCID.cid_name Caller ID - Name  
 * @see VOIP_MGR_FSK_CID_GEN_CFG TstVoipCID 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FSK_CID_GEN_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	if ( 1 == Is_DAA_Channel(stCIDstr.ch_id))
	{
		//printk("chid%d is not for SLIC, can not gen FSK caller ID!\n", stCIDstr.ch_id);
		return 0;
	}
	
	TstFskClidData cid_data[FSK_MDMF_SIZE];
	
	if (stCIDstr.string[0] == 'P')
	{
		cid_data[0].type = FSK_PARAM_CLI_ABS;
		strcpy(cid_data[0].data, "P");	// Private
	}
	else if (stCIDstr.string[0] == 'O')
	{
		cid_data[0].type = FSK_PARAM_CLI_ABS;
		strcpy(cid_data[0].data, "O");	// Out of area or Unavailable
	}
	else if (stCIDstr.string[0] != 0)
	{
		cid_data[0].type = FSK_PARAM_CLI;
		strcpy(cid_data[0].data, stCIDstr.string);
	}
	else
	{
		cid_data[0].type = FSK_PARAM_NULL;
		cid_data[0].data[0] = 0;
	}
	
	if (stCIDstr.string2[0] != 0)
	{
		cid_data[1].type = FSK_PARAM_DATEnTIME;
		strcpy(cid_data[1].data, stCIDstr.string2);
	}
	else
	{
		cid_data[1].type = FSK_PARAM_NULL;
		cid_data[1].data[0] = 0;
	}
	
	if (stCIDstr.cid_name[0] != 0)
	{
		cid_data[2].type = FSK_PARAM_CLI_NAME;
		strcpy(cid_data[2].data, stCIDstr.cid_name);
	}
	else
	{
		cid_data[2].type = FSK_PARAM_NULL;
		cid_data[2].data[0] = 0;
	}
	
	cid_data[3].type = FSK_PARAM_NULL;
	cid_data[3].data[0] = 0;
	cid_data[4].type = FSK_PARAM_NULL;
	cid_data[4].data[0] = 0;
	
	SLIC_gen_FSK_CID(stCIDstr.ch_id, stCIDstr.cid_mode, FSK_MSG_CALLSETUP , cid_data);

	return 0;
}
#else
int do_mgr_VOIP_MGR_FSK_CID_GEN_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	if ( 1 == Is_DAA_Channel(stCIDstr.ch_id))
	{
		//printk("chid%d is not for SLIC, can not gen FSK caller ID!\n", stCIDstr.ch_id);
		return 0;
	}

	
	ring.CH = stCIDstr.ch_id;
	ring.ring_set = 1;

	if(!(fsk_spec_areas[stCIDstr.ch_id]&0x08))// send DTMF callerid after ring.
	{
		//FXS_Ring(&ring);
		FXS_Ring( stCIDstr.ch_id, 1 );
		//ioctrl_ring_set[stCIDstr.ch_id] = ring.ring_set + (0x1<<1);
		//printk("m:ioctrl_ring_set[%d]=%d\n", stCIDstr.ch_id, ioctrl_ring_set[stCIDstr.ch_id]);
		mdelay(20);// th: add delay before check 1st Ring off.
		printk("1st ring..\n");

#ifdef CONFIG_RTK_VOIP_LED
		if (daa_ringing == 0)
			fxs_ringing[stCIDstr.ch_id] = 1;
		led_state_watcher(stCIDstr.ch_id);
#endif
	}
#if 0		
	save_flags(flags); cli();
	ioctrl_ring_set[stCIDstr.ch_id] = ring.ring_set + (0x1<<1); //Must to keep DSP auto-Ring normal.
	fsk_cid_state[stCIDstr.ch_id] = 1;
	fsk_cid_enable[stCIDstr.ch_id] = 1;

	RtkAc49xApiSetFskCallerIdParam(stCIDstr.ch_id, stCIDstr.cid_mode, FSK_MSG_CALLSETUP, stCIDstr.string,  stCIDstr.string2, stCIDstr.cid_name);
	restore_flags(flags);
#else
	RtkAc49xApiSendFskCallerId(stCIDstr.ch_id, stCIDstr.cid_mode, FSK_MSG_CALLSETUP, stCIDstr.string,  stCIDstr.string2, stCIDstr.cid_name);
	//PRINT_MSG("\n <RTK> CID = %s\n", stCIDstr.string);
	
	ioctrl_ring_set[stCIDstr.ch_id] = ring.ring_set + (0x1<<1); //Must to keep DSP auto-Ring normal.
	fsk_cid_state[stCIDstr.ch_id] = 1;

	/* Auto Ring */
	if (gRingGenAfterCid[stCIDstr.ch_id] == 0)
	{
		gFirstRingOffTimeOut[stCIDstr.ch_id] = jiffies + (HZ*gRingCadOff[stCIDstr.ch_id]/1000);
		//printk("=>%d, J=%d\n", gFirstRingOffTimeOut[stCIDstr.ch_id], jiffies);
		gRingGenAfterCid[stCIDstr.ch_id] = 1;
		//printk("1: gRingGenAfterCid[%d] = %d\n", stCIDstr.ch_id, gRingGenAfterCid[stCIDstr.ch_id]);
	}
#endif
	return 0;
}
#endif


/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief FSK Caller ID Generation
 * @param chid The FXS channel number.
 * @param pClid The pointer of FSK Caller ID Data
 * @param num_clid_element The number of FSK Caller ID element
 * @see VOIP_MGR_FSK_CID_MDMF_GEN TstFskClid 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FSK_CID_MDMF_GEN(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstFskClid stFskClid;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stFskClid, (TstFskClid*)para, sizeof(TstFskClid));
	

	if ( 1 == Is_DAA_Channel(stFskClid.ch_id))
	{
		//printk("chid%d is not for SLIC, can not gen FSK caller ID!\n", stFskClid.ch_id);
		return 0;
	}

	SLIC_gen_FSK_CID(stFskClid.ch_id, stFskClid.service_type, FSK_MSG_CALLSETUP , stFskClid.cid_data);

	return 0;
}
#else
int do_mgr_VOIP_MGR_FSK_CID_MDMF_GEN(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstFskClid stFskClid;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stFskClid, (TstFskClid*)para, sizeof(TstFskClid));

	if ( 1 == Is_DAA_Channel(stFskClid.ch_id))
	{
		//printk("chid%d is not for SLIC, can not gen FSK caller ID!\n", stFskClid.ch_id);
		return 0;
	}
	fsk_cid_state[stFskClid.ch_id] = 1;
	PRINT_R("Need implement for do_mgr_VOIP_MGR_FSK_CID_MDMF_GEN.\n");

	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief Generate FSK VMWI 
 * @param TstVoipCID.ch_id Channel ID
 * @param TstVoipCID.string Caller ID - Phonenumber 
 * @see VOIP_MGR_SET_FSK_VMWI_STATE TstVoipCID 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_FSK_VMWI_STATE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;
	
	// thlin +
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	if ( 1 == Is_DAA_Channel(stCIDstr.ch_id))
	{
		//printk("chid%d is not for SLIC, can not gen FSK caller ID!\n", stCIDstr.ch_id);
		return 0;
	}
	
	TstFskClidData cid_data[FSK_MDMF_SIZE];
	
	cid_data[0].type = FSK_PARAM_MW;
	strcpy(cid_data[0].data, stCIDstr.string);
	
	cid_data[1].type = FSK_PARAM_NULL;
	cid_data[1].data[0] = 0;
	
	SLIC_gen_VMWI(stCIDstr.ch_id, cid_data);
	
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_FSK_VMWI_STATE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	fsk_cid_state[stCIDstr.ch_id] = 1;
	RtkAc49xApiSendVmwi(stCIDstr.ch_id, FSK_MSG_MWSETUP, stCIDstr.string);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief Set FSK area 
 * @param TstVoipCID.ch_id Channel ID
 * @param TstVoipCID.cid_gain Caller ID gain. Only support multiple 1~5. 
 * @param TstVoipCID.cid_mode Caller ID mode. <br>
 *        bit 0-2: FSK Type. (0:Bellcore 1:ETSI 2:BT 3:NTT) <br>
 *        bit 3: Caller ID Prior First Ring <br>
 *        bit 4: Dual Tone before Caller ID (Fsk Alert Tone) <br>
 *        bit 5: Short Ring before Caller ID <br>
 *        bit 6: Reverse Polarity before Caller ID (Line Reverse) <br>
 *        bit 7: FSK Date & Time Sync and Display Name <br>
 * @see VOIP_MGR_SET_FSK_AREA TstVoipCID 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_FSK_AREA(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	fsk_spec_areas[stCIDstr.ch_id] = stCIDstr.cid_mode; /* 0:Bellcore 1:ETSI 2:BT 3:NTT */
	// fsk_spec_areas[]:
	// bit0-2: FSK Type
	// bit 3: Caller ID Prior First Ring
	// bit 4: Dual Tone before Caller ID (Fsk Alert Tone)
	// bit 5: Short Ring before Caller ID
	// bit 6: Reverse Polarity before Caller ID (Line Reverse)
	// bit 7: FSK Date & Time Sync and Display Name
	
	
	if ((stCIDstr.cid_mode&7) == 0)      PRINT_MSG("Set FSK Caller ID Area to Bellcore, ch=%d.\n", stCIDstr.ch_id);
    else if ((stCIDstr.cid_mode&7) == 1) PRINT_MSG("Set FSK Caller ID Area to ETSI, ch=%d.\n", stCIDstr.ch_id);
    else if ((stCIDstr.cid_mode&7) == 2) PRINT_MSG("Set FSK Caller ID Area to BT, ch=%d.\n", stCIDstr.ch_id);
    else if ((stCIDstr.cid_mode&7) == 3) PRINT_MSG("Set FSK Caller ID Area to NTT, ch=%d.\n", stCIDstr.ch_id);
    else				     PRINT_MSG("NOT Support this FSK Area.\n");
	PRINT_MSG("\nReset the fsk setting %d\n",fsk_spec_areas[stCIDstr.ch_id]);
	
	if ( 1 == Is_DAA_Channel(stCIDstr.ch_id))
	{
		if (stCIDstr.cid_mode&0x08)
			ring_times_set(stCIDstr.ch_id, 1, 0); // chid, ring_on_times, ring_off_times
		else
			ring_times_set(stCIDstr.ch_id, 2, 1);
	}
	
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_FSK_AREA(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	fsk_spec_areas[stCIDstr.ch_id] = stCIDstr.cid_mode; /* 0:Bellcore 1:ETSI 2:BT 3:NTT */
	// fsk_spec_areas[]:
	// bit0-2: FSK Type
	// bit 3: Caller ID Prior First Ring
	// bit 4: Dual Tone before Caller ID (Fsk Alert Tone)
	// bit 5: Short Ring before Caller ID
	// bit 6: Reverse Polarity before Caller ID (Line Reverse)
	// bit 7: FSK Date & Time Sync and Display Name
	fsk_cid_gain[stCIDstr.ch_id] = stCIDstr.cid_gain; /* Only support multiple 1~5 */

	if ((stCIDstr.cid_mode&7) == 0)      PRINT_MSG("Set FSK Caller ID Area to Bellcore, ch=%d.\n", stCIDstr.ch_id);
    else if ((stCIDstr.cid_mode&7) == 1) PRINT_MSG("Set FSK Caller ID Area to ETSI, ch=%d.\n", stCIDstr.ch_id);
    else if ((stCIDstr.cid_mode&7) == 2) PRINT_MSG("Set FSK Caller ID Area to BT, ch=%d.\n", stCIDstr.ch_id);
    else if ((stCIDstr.cid_mode&7) == 3) PRINT_MSG("Set FSK Caller ID Area to NTT, ch=%d.\n", stCIDstr.ch_id);
    else				     PRINT_MSG("NOT Support this FSK Area.\n");
	PRINT_MSG("\nReset the fsk setting %d\n",fsk_spec_areas[stCIDstr.ch_id]);

	if ( 1 == Is_DAA_Channel(stCIDstr.ch_id))
	{
		if (stCIDstr.cid_mode&0x08)
			ring_times_set(stCIDstr.ch_id, 1, 0); // chid, ring_on_times, ring_off_times
		else
			ring_times_set(stCIDstr.ch_id, 2, 1);
	}
	return 0;
}
#endif


/**
 * @ingroup VOIP_FXS_CALLERID
 * @brief Set the FSK Caller ID Parameters
 * @param chid The FXS channel number.
 * @param para The variable pointer of FSK Caller ID Parameters
 * @see VOIP_MGR_SET_FSK_CLID_PARA TstVoipFskPara 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_FSK_CLID_PARA(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipFskPara stVoipFskPara;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipFskPara, (TstVoipFskPara*)para, sizeof(TstVoipFskPara));

	TstFskClidPara clid_para;
	
	clid_para.ch_seizure_cnt = stVoipFskPara.CS_cnt;
	clid_para.mark_cnt = stVoipFskPara.mark_cnt;
	clid_para.mark_gain = stVoipFskPara.mark_gain;
	clid_para.space_gain = stVoipFskPara.space_gain;
	clid_para.type2_expected_ack_tone = stVoipFskPara.type2_expected_ack_tone;
	clid_para.delay_after_1st_ring = stVoipFskPara.delay_after_1st_ring;
	clid_para.delay_before_2nd_ring = stVoipFskPara.delay_before_2nd_ring;
	
	fsk_cid_para_set(stVoipFskPara.ch_id, stVoipFskPara.area, &clid_para);

	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_FSK_CLID_PARA(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	PRINT_R("Not support or need implement for do_mgr_VOIP_MGR_SET_FSK_CLID_PARA().\n");
}
#endif

/**
 * @ingroup VOIP_FXS_CALLERID
 * @brief Get the FSK Caller ID Parameters
 * @param chid The FXS channel number.
 * @param para The variable pointer to save current FSK Caller ID Parameters
 * @see VOIP_MGR_GET_FSK_CLID_PARA TstVoipFskPara 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_GET_FSK_CLID_PARA(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipFskPara stVoipFskPara;

	memcpy(&stVoipFskPara, (TstVoipFskPara*)para, sizeof(TstVoipFskPara));

	TstFskClidPara clid_para;
	
	fsk_cid_para_get(stVoipFskPara.ch_id, stVoipFskPara.area, &clid_para);
	
	stVoipFskPara.CS_cnt = clid_para.ch_seizure_cnt;
	stVoipFskPara.mark_cnt = clid_para.mark_cnt;
	stVoipFskPara.mark_gain = clid_para.mark_gain;
	stVoipFskPara.space_gain = clid_para.space_gain;
	stVoipFskPara.type2_expected_ack_tone = clid_para.type2_expected_ack_tone;	
	stVoipFskPara.delay_after_1st_ring = clid_para.delay_after_1st_ring;
	stVoipFskPara.delay_before_2nd_ring = clid_para.delay_before_2nd_ring;
	
	//copy_to_user(user, &stVoipFskPara, sizeof(TstVoipFskPara));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stVoipFskPara, sizeof(TstVoipFskPara)))
	{
		return 0;
	}

	return 0;
}
#else
int do_mgr_VOIP_MGR_GET_FSK_CLID_PARA(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	PRINT_R("Not support or need implement for do_mgr_VOIP_MGR_GET_FSK_CLID_PARA().\n");
}
#endif

/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief Generate FSK alert 
 * @param TstVoipCID.string Caller ID - Phonenumber 
 * @see VOIP_MGR_FSK_ALERT_GEN_CFG TstVoipCID 
 */
 // not used for DSP
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FSK_ALERT_GEN_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	//fsk_alert_state[stCIDstr.ch_id] = 1;
	strcpy(dtmf_cid_info[stCIDstr.ch_id].data, stCIDstr.string);
	return 0;
}
#else
int do_mgr_VOIP_MGR_FSK_ALERT_GEN_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	//fsk_alert_state[stCIDstr.ch_id] = 1;
	//strcpy(cid_str, stCIDstr.string);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief Set DTMF mode of caller ID 
 * @param TstVoipCID.ch_id Channel ID 
 * @param TstVoipCID.cid_mode Caller ID mode <br> 
 *        bit0-2: FSK Type <br> 
 *        bit 3: Normal Ring <br> 
 *        bit 4: Fsk Alert Tone <br> 
 *        bit 5: Short Ring <br> 
 *        bit 6: Line Reverse <br> 
 *        bit 7: Date, Time Sync and Name <br> 
 * @param TstVoipCID.cid_dtmf_mode Caller ID in DTMF mode. <br>
 *        0-1 bits for starting digit, and 2-3 bits for ending digit. <br>
 *        00: A, 01: B, 02: C, 03: D
 * @see VOIP_MGR_SET_CID_DTMF_MODE TstVoipCID 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_CID_DTMF_MODE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	dtmf_cid_info[stCIDstr.ch_id].bBefore1stRing = stCIDstr.cid_mode&0x08; 	/* set the before ring or after ring send cid */
	dtmf_cid_info[stCIDstr.ch_id].bAuto_StartEnd = stCIDstr.cid_dtmf_mode&0x10;
	dtmf_cid_info[stCIDstr.ch_id].start_digit =  stCIDstr.cid_dtmf_mode&0x3;
	dtmf_cid_info[stCIDstr.ch_id].end_digit =  (stCIDstr.cid_dtmf_mode&0xC)>>2;
	
	if ( 1 == Is_DAA_Channel(stCIDstr.ch_id))
	{
		if (stCIDstr.cid_mode&0x08)
			ring_times_set(stCIDstr.ch_id, 1, 0); // chid, ring_on_times, ring_off_times
		else
			ring_times_set(stCIDstr.ch_id, 2, 1);
	}
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_CID_DTMF_MODE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	dtmf_cid_info[stCIDstr.ch_id].bBefore1stRing = stCIDstr.cid_mode&0x08; 	/* set the before ring or after ring send cid */
	dtmf_cid_info[stCIDstr.ch_id].bAuto_StartEnd = stCIDstr.cid_dtmf_mode&0x10;
	dtmf_cid_info[stCIDstr.ch_id].start_digit =  stCIDstr.cid_dtmf_mode&0x3;
	dtmf_cid_info[stCIDstr.ch_id].end_digit =  (stCIDstr.cid_dtmf_mode&0xC)>>2;
	
	if ( 1 == Is_DAA_Channel(stCIDstr.ch_id))
	{
		if (stCIDstr.cid_mode&0x08)
			ring_times_set(stCIDstr.ch_id, 1, 0); // chid, ring_on_times, ring_off_times
		else
			ring_times_set(stCIDstr.ch_id, 2, 1);
	}
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief Set caller ID detection mode 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable Auto mode selection. <br>
 *        (0: off, 1: auto mode (NTT support), 2: auto mode (NTT not support))
 * @param TstVoipCfg.cfg If auto mode is off, use this parameter to choose detection mode. <br>
 *        0: Bellcore FSK <br>
 *        1: ETSI FSK <br>
 *        2: BT FSK <br>
 *        3: NTT FSK <br>
 *        4: DTMF <br>
 * @see VOIP_MGR_SET_CID_DET_MODE TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_CID_DET_MODE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#ifdef FXO_CALLER_ID_DET
	unsigned long flags;
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	save_flags(flags); cli();
	auto_cid_det[stVoipCfg.ch_id] = stVoipCfg.enable;
	cid_type[stVoipCfg.ch_id] = stVoipCfg.cfg;
	restore_flags(flags);
	
	if (stVoipCfg.enable == 0)
	{
		if (stVoipCfg.cfg == 0)
	                    PRINT_MSG("Set Caller ID Detection Mode to Bellcore FSK.\n");
		else if (stVoipCfg.cfg == 1)
	                	PRINT_MSG("Set Caller ID Detection Mode to ETSI FSK.\n");
		else if (stVoipCfg.cfg == 2)
	                	PRINT_MSG("Set Caller ID Detection Mode to BT FSK.\n");
		else if (stVoipCfg.cfg == 3)
	                	PRINT_MSG("Set Caller ID Detection Mode to NTT FSK.\n");
		else if (stVoipCfg.cfg == 4)
	                	PRINT_MSG("Set Caller ID Detection Mode to DTMF.\n");
	}
	else if (stVoipCfg.enable == 1)
	        	PRINT_MSG("Enable Auto Caller ID Detection Mode (NTT Support).\n");
	else if (stVoipCfg.enable == 2)
	        	PRINT_MSG("Enable Auto Caller ID Detection Mode (NTT Not Support).\n");
#endif
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_CID_DET_MODE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#ifdef FXO_CALLER_ID_DET
	unsigned long flags;
	TstVoipCfg stVoipCfg;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	save_flags(flags); cli();
	auto_cid_det[stVoipCfg.ch_id] = stVoipCfg.enable;
	cid_type[stVoipCfg.ch_id] = stVoipCfg.cfg;
	restore_flags(flags);
	
	if (stVoipCfg.enable == 0)
	{
		PRINT_MSG("Set CH%d Caller ID Detection Mode to ", stVoipCfg.ch_id);
		
		if (stVoipCfg.cfg == 0)
		{
			PRINT_MSG("Bellcore FSK.\n");
			RtkAc49xApiSetCallerIdDetection( stVoipCfg.ch_id, CONTROL__ENABLE, CONTROL__ENABLE, CALLER_ID_STANDARD__TELCORDIA_BELLCORE);
		}
		else if (stVoipCfg.cfg == 1)
		{
			PRINT_MSG("ETSI FSK.\n");
			RtkAc49xApiSetCallerIdDetection( stVoipCfg.ch_id, CONTROL__ENABLE, CONTROL__ENABLE, CALLER_ID_STANDARD__ETSI);
		}
		else if (stVoipCfg.cfg == 2)
		{
			PRINT_MSG("BT FSK.\n");
			RtkAc49xApiSetCallerIdDetection( stVoipCfg.ch_id, CONTROL__ENABLE, CONTROL__ENABLE, CALLER_ID_STANDARD__ETSI);
		}
		else if (stVoipCfg.cfg == 3)
		{
			PRINT_MSG("NTT FSK.\n");
			RtkAc49xApiSetCallerIdDetection( stVoipCfg.ch_id, CONTROL__ENABLE, CONTROL__ENABLE, CALLER_ID_STANDARD__NTT);
		}
		else if (stVoipCfg.cfg == 4)
		{
			PRINT_MSG("DTMF.\n");
			RtkAc49xApiSetCallerIdDetection( stVoipCfg.ch_id, CONTROL__ENABLE, CONTROL__ENABLE, CALLER_ID_STANDARD__DTMF_CLIP_ETSI);
		}
	}
	else if (stVoipCfg.enable == 1)
	{
		RtkAc49xApiSetCallerIdDetection( stVoipCfg.ch_id, CONTROL__ENABLE, CONTROL__ENABLE, CALLER_ID_STANDARD__DTMF_CLIP_ETSI);
		printk(AC_FORE_GREEN "Warning: ACMW doesn't support Auto Caller ID Detection Mode."
		"Set Caller ID Detection Mode to DTMF.\n" AC_RESET);
		//PRINT_MSG("Enable Auto Caller ID Detection Mode (NTT Support).\n");
	}
	else if (stVoipCfg.enable == 2)
	{
		RtkAc49xApiSetCallerIdDetection( stVoipCfg.ch_id, CONTROL__ENABLE, CONTROL__ENABLE, CALLER_ID_STANDARD__DTMF_CLIP_ETSI);
		printk(AC_FORE_GREEN "Warning: ACMW doesn't support Auto Caller ID Detection Mode."
		"Set Caller ID Detection Mode to DTMF.\n" AC_RESET);
		//PRINT_MSG("Enable Auto Caller ID Detection Mode (NTT Not Support).\n");
	}
#endif
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief Get FSK caller ID state 
 * @param TstVoipCID.ch_id Channel ID 
 * @param TstVoipCID.cid_state Caller ID state 
 * @see VOIP_MGR_GET_FSK_CID_STATE_CFG TstVoipCID 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_GET_FSK_CID_STATE_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;
	
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	stCIDstr.cid_state = fsk_cid_state[stCIDstr.ch_id];

		if (!fsk_cid_state[stCIDstr.ch_id])
			init_softfskcidGen(stCIDstr.ch_id);

	//copy_to_user(user, &stCIDstr, sizeof(TstVoipCID));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stCIDstr, sizeof(TstVoipCID)))
	{
		return 0;
	}
	return 0;
}
#else
int do_mgr_VOIP_MGR_GET_FSK_CID_STATE_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipCID stCIDstr;
	
	// Get FSK CID state
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	stCIDstr.cid_state = fsk_cid_state[stCIDstr.ch_id];
	//copy_to_user(user, &stCIDstr, sizeof(TstVoipCID));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stCIDstr, sizeof(TstVoipCID)))
	{
		return 0;
	}
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief Set caller ID FSK generation mode 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable Generation mode (0: hardware FSK caller id, 1:software FSK caller id)
 * @see VOIP_MGR_SET_CID_FSK_GEN_MODE TstVoipCfg 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_CID_FSK_GEN_MODE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// Now, SW FSK caller generation is used as default. 
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_CID_FSK_GEN_MODE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_CALLERID
 * @brief Generate FSK VMWI 
 * @param TstVoipCID.ch_id Channel ID 
 * @param TstVoipCID.cid_mode Caller ID mode (0:on-hook type1, 1:off-hook type2)
 * @param TstVoipCID.cid_msg_type Message type <br> 
 *        0x80: Call Set-up <br> 
 *        0x82: Message Waiting (VMWI) <br> 
 *        0x86: Advice of Charge <br> 
 *        0x89: Short Message Service <br> 
 * @param TstVoipCID.string Caller ID - phonenumber 
 * @param TstVoipCID.string2 Caller ID - Date and time 
 * @param TstVoipCID.cid_name caller ID - Name 
 * @see VOIP_MGR_FSK_CID_VMWI_GEN_CFG TstVoipCID 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_FSK_CID_VMWI_GEN_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	extern void genSoftFskCID (uint32_t chid);
	TstVoipCID stCIDstr;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stCIDstr, (TstVoipCID*)para, sizeof(TstVoipCID));
	
	fsk_cid_state[stCIDstr.ch_id]=1;
	// remember set slic in transmit mode, enable DSP pcm.
	init_softfskcidGen(stCIDstr.ch_id);
	//genSoftFskCID(stCIDstr.ch_id, stCIDstr.cid_mode, stCIDstr.cid_msg_type, stCIDstr.string, stCIDstr.string2, stCIDstr.cid_name);
	PRINT_R("Need implement for do_mgr_VOIP_MGR_FSK_CID_VMWI_GEN_CFG\n");
	return 0;
}
#else
int do_mgr_VOIP_MGR_FSK_CID_VMWI_GEN_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_TONE
 * @brief Play a tone 
 * @see VOIP_MGR_SETPLAYTONE TstVoipPlayToneConfig 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SETPLAYTONE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	unsigned long flags;
	uint32 ch_id, m_id, s_id;
	TstVoipPlayToneConfig stVoipPlayToneConfig;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipPlayToneConfig, (TstVoipPlayToneConfig*)para, sizeof(TstVoipPlayToneConfig));
	
	ch_id = stVoipPlayToneConfig.ch_id;
	m_id = stVoipPlayToneConfig.m_id;
	
#ifdef _chiminer_slic2_ch1
	ch_id=1;
	PRINT_MSG("ch_id=%d\n",ch_id);
#endif
	
	s_id = API_GetSid(ch_id, m_id);
	PRINT_MSG("VOIP_MGR_SETPLAYTONE:ch_id=%d, m_id=%d, s_id=%d, nTone=%d, bFlag=%x, path=%d\n", ch_id, m_id, s_id, stVoipPlayToneConfig.nTone, stVoipPlayToneConfig.bFlag, stVoipPlayToneConfig.path);
	
	save_flags(flags); cli();
	hc_SetPlayTone(ch_id, s_id, stVoipPlayToneConfig.nTone, stVoipPlayToneConfig.bFlag, stVoipPlayToneConfig.path);
	restore_flags(flags);
/*
	save_flags(flags); cli();
	
	if(stVoipPlayToneConfig.bFlag)
	{
		if(s_nCurPlayTone[ch_id] != -1)
		{
			//if(pcfg->nTone != nCurPlayTone)
			DspcodecPlayTone(ch_id, s_nCurPlayTone[ch_id], 0, stVoipPlayToneConfig.path);
			// before open a new tone, must close old first
		}
		s_nCurPlayTone[ch_id] = stVoipPlayToneConfig.nTone;
		DspcodecPlayTone(ch_id, s_nCurPlayTone[ch_id], stVoipPlayToneConfig.bFlag, stVoipPlayToneConfig.path);
	}
	else
	{
		if(s_nCurPlayTone[ch_id] != -1)
		{
			DspcodecPlayTone(ch_id, s_nCurPlayTone[ch_id], stVoipPlayToneConfig.bFlag, stVoipPlayToneConfig.path);
			s_nCurPlayTone[ch_id] = -1;
			//if(pcfg->nTone != nCurPlayTone)
			//DspcodecPlayTone(chid, nCurPlayTone, 0, pcfg->path);
			//before open a new tone, must close old first
		}
	}
	s_tonepath[ch_id] = stVoipPlayToneConfig.path;
	
	restore_flags(flags);
*/
	//PRINT_MSG("%s-%d\n", __FUNCTION__, __LINE__);
	return 0;
}
#else
int do_mgr_VOIP_MGR_SETPLAYTONE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	unsigned long flags;
	uint32 s_id;
	TstVoipPlayToneConfig stVoipPlayToneConfig;

	/** Play Tone **/
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipPlayToneConfig, (TstVoipPlayToneConfig*)para, sizeof(TstVoipPlayToneConfig));
	
	save_flags(flags); cli();
	s_id = 2*stVoipPlayToneConfig.ch_id + PROTOCOL__RTP - 1;
	
	if ((stVoipPlayToneConfig.bFlag == 0x80) ||(stVoipPlayToneConfig.bFlag == 0x81))
		stVoipPlayToneConfig.bFlag = stVoipPlayToneConfig.bFlag - 0x80;
		
	RtkAc49xApiPlayTone(stVoipPlayToneConfig.ch_id, s_id, stVoipPlayToneConfig.nTone, stVoipPlayToneConfig.bFlag, stVoipPlayToneConfig.path);
	
	restore_flags(flags);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_TONE
 * @brief Set the tone, busy tone detection parametes, and SLIC impedance according to the Country
 * @param TstVoipValue.value Country 
 *        - 0: USA 
 *        - 1: UK
 *        - 2: Australia
 *        - 3: HK
 *        - 4: Japan 
 *        - 5: Sweden 
 *        - 6: Germany
 *        - 7: France  
 *        - 8: Taiwan 
 *        - 9: Belgium 
 *        - 10: Finland 
 *        - 11: Italy
 *        - 12: China
 *        - 13: Customer  
 * @see VOIP_MGR_SET_COUNTRY TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_COUNTRY(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	uint32 sid, chid;
	for(sid=0; sid<SESS_NUM; sid++)	//Set the same country for each session.
		DspcodecSetCountry( sid, /*country*/stVoipValue.value);
	//for (chid=0; chid<SLIC_CH_NUM; chid++)
	for (chid=0; chid<CON_CH_NUM; chid++) {
		if( get_snd_type_cch( chid ) != SND_TYPE_FXS )
			continue;
		SLIC_Set_Impendance_Country(chid, stVoipValue.value /*country*/, 0 /* impedance value: reserved */);
	}

#ifdef FXO_BUSY_TONE_DET
	extern void ring_tone_det_cfg_apply( void );
	extern ToneCfgParam_t ToneTable[];
	
	
	switch (stVoipValue.value)
	{
		case 0://USA
		case 3://HK
#if 1
		case 8://Taiwan
#endif
			det_freq[0] = FREQ_480HZ;
			det_freq[1] = FREQ_620HZ;
			break;
		case 7://France
		case 9://Belgium
			det_freq[0] = FREQ_440HZ;
			det_freq[1] = FREQ_NA;
			break;
		case 1://UK
		case 2://Australia
		case 4://Japan
			det_freq[0] = FREQ_400HZ;
			det_freq[1] = FREQ_NA;
			break;
		case 5://Sweden
		case 6://Germany
#if 0
		case 8://TR57
#endif
		case 10://Finland
		case 11://Italy
			det_freq[0] = FREQ_425HZ;
			det_freq[1] = FREQ_NA;
			break;
		case 12://China
			det_freq[0] = FREQ_450HZ;
			det_freq[1] = FREQ_NA;
			break;
	
		case 13://Customer
			//det_freq[0] = FREQ_450HZ;
			//det_freq[1] = FREQ_NA;
			break;
	
		default://USA
			det_freq[0] = FREQ_480HZ;
			det_freq[1] = FREQ_620HZ;
			break;
	}

	stVoiptonedet_parm.frequency1 = ToneTable[ (USA_DIAL-1) + 20*stVoipValue.value + 6].Freq0;
	stVoiptonedet_parm.frequency2 = ToneTable[ (USA_DIAL-1) + 20*stVoipValue.value + 6].Freq1;
	
	int cad_on, cad_off, er_on, er_off;
	
	cad_on = ( ToneTable[ (USA_DIAL-1) + 20*stVoipValue.value + 6].CadOn0 )/10;
	cad_off = ( ToneTable[ (USA_DIAL-1) + 20*stVoipValue.value + 6].CadOff0 )/10;
	/* 12.5% inaccuracy */
	er_on = ( cad_on >> 3 );
	er_off = ( cad_off >> 3 );
	
	stVoiptonedet_parm.busytone_on_low_limit = cad_on - er_on;
	stVoiptonedet_parm.busytone_on_up_limit = cad_on + er_on;
	stVoiptonedet_parm.busytone_off_low_limit = cad_off - er_off;
	stVoiptonedet_parm.busytone_off_up_limit = cad_off + er_off;
	
	busy_tone_det_cfg_apply();
	
	
	stVoiptonedet_parm.frequency1 = ToneTable[ (USA_DIAL-1) + 20*stVoipValue.value + 5].Freq0;
	stVoiptonedet_parm.frequency2 = ToneTable[ (USA_DIAL-1) + 20*stVoipValue.value + 5].Freq1;
	
	cad_on = ( ToneTable[ (USA_DIAL-1) + 20*stVoipValue.value + 5].CadOn0 )/10;
	cad_off = ( ToneTable[ (USA_DIAL-1) + 20*stVoipValue.value + 5].CadOff0 )/10;
	/* 12.5% inaccuracy */
	er_on = ( cad_on >> 3 );
	er_off = ( cad_off >> 3 );
	
	stVoiptonedet_parm.busytone_on_low_limit = cad_on - er_on;
	stVoiptonedet_parm.busytone_on_up_limit = cad_on + er_on;
	stVoiptonedet_parm.busytone_off_low_limit = cad_off - er_off;
	stVoiptonedet_parm.busytone_off_up_limit = cad_off + er_off;
	
	ring_tone_det_cfg_apply();

#endif
	switch (stVoipValue.value)
	{
		case 0://USA
			PRINT_MSG("Set Tone of Country to USA\n");
			break;
		case 1://UK
			PRINT_MSG("Set Tone of Country to UK\n");
			break;
		case 2://Australia
			PRINT_MSG("Set Tone of Country to AUSTRALIA\n");
			break;
		case 3://HK
			PRINT_MSG("Set Tone of Country to HONG KONG\n");
			break;
		case 4://Japan
			PRINT_MSG("Set Tone of Country to JAPAN\n");
			break;
		case 5://Sweden
			PRINT_MSG("Set Tone of Country to SWEDEN\n");
			break;
		case 6://Germany
			PRINT_MSG("Set Tone of Country to GERMANY\n");
			break;
		case 7://France
			PRINT_MSG("Set Tone of Country to FRANCE\n");
			break;
		case 8://TR57
			//PRINT_MSG("Set Tone of Country to TR57\n");
			PRINT_MSG("Set Tone of Country to TAIWAN\n");
			break;
		case 9://Belgium
			PRINT_MSG("Set Tone of Country to BELGIUM\n");
			break;
		case 10://Finland
			PRINT_MSG("Set Tone of Country to FINLAND\n");
			break;
		case 11://Italy
			PRINT_MSG("Set Tone of Country to ITALY\n");
			break;
		case 12://China
			PRINT_MSG("Set Tone of Country to CHINA\n");
			break;
		case 13://Customer
			PRINT_MSG("Set Tone of Country to CUSTOMER\n");
			break;
		default:
			PRINT_MSG("The tone you select is not support!\n");
			break;
	}

	if (stVoipValue.value == 4) // Japan 
	{
#if defined( CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3210 ) || defined( CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3215 )
		LEC_NLP_Config(2);
#elif defined (CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3226) || defined( CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3217x )
		LEC_NLP_Config(2);
#elif defined(CONFIG_RTK_VOIP_DRIVERS_SLIC_LE88221) || defined(CONFIG_RTK_VOIP_DRIVERS_SLIC_LE88266)
		LEC_NLP_Config(5);
#elif defined CONFIG_RTK_VOIP_DRIVERS_SLIC_W682388
		#error "Need to do NTT echo test for SLIC W682388"
#endif
	}
	else 
	{
#if defined (CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3226) || defined( CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3217x )
		LEC_NLP_Config(5); 
#else
		LEC_NLP_Config(6); 
#endif
	}
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_COUNTRY(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	if ( DSPCODEC_COUNTRY_CUSTOME != stVoipValue.value)
	{
		RtkAc49xApiSetCountryTone(stVoipValue.value);
		uint32 chid;
		for (chid=0; chid<SLIC_CH_NUM; chid++)
			SLIC_Set_Impendance_Country(chid, stVoipValue.value /*country*/, 0 /* impedance value: reserved */);
	}
	return 0;
}
#endif

/**
 * @ingroup VOIP_FXS
 * @brief Set the impedance of country 
 * @param TstVoipValue.value Country 
 *        - 0: USA 
 *        - 1: UK
 *        - 2: Australia
 *        - 3: HK
 *        - 4: Japan 
 *        - 5: Sweden 
 *        - 6: Germany
 *        - 7: France  
 *        - 8: Taiwan 
 *        - 9: Belgium 
 *        - 10: Finland 
 *        - 11: Italy
 *        - 12: China
 *        - 13: Customer  
 * @see VOIP_MGR_SET_COUNTRY_IMPEDANCE TstVoipValue 
 */
int do_mgr_VOIP_MGR_SET_COUNTRY_IMPEDANCE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	uint32 chid;
	TstVoipValue stVoipValue;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));

	//for (chid=0; chid<SLIC_CH_NUM; chid++)
	for (chid=0; chid<CON_CH_NUM; chid++) {
		if( get_snd_type_cch( chid ) != SND_TYPE_FXS )
			continue;
			
		SLIC_Set_Impendance_Country(chid, stVoipValue.value /*country*/, 0 /* impedance value: reserved */);
	}


	return 0;
}


/**
 * @ingroup VOIP_DSP_TONE
 * @brief Set tone of country 
 * @param TstVoipValue.value Country 
 *        - 0: USA 
 *        - 1: UK
 *        - 2: Australia
 *        - 3: HK
 *        - 4: Japan 
 *        - 5: Sweden 
 *        - 6: Germany
 *        - 7: France  
 *        - 8: Taiwan 
 *        - 9: Belgium 
 *        - 10: Finland 
 *        - 11: Italy
 *        - 12: China
 *        - 13: Customer  
 * @see VOIP_MGR_SET_COUNTRY_TONE TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_COUNTRY_TONE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	uint32 sid;
	TstVoipValue stVoipValue;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));

	for(sid=0; sid<SESS_NUM; sid++)	//Set the same country for each session.
		DspcodecSetCountry( sid, /*country*/stVoipValue.value);

	switch (stVoipValue.value)
	{
		case 0://USA
			PRINT_MSG("Set Tone of Country to USA\n");
			break;
		case 1://UK
			PRINT_MSG("Set Tone of Country to UK\n");
			break;
		case 2://Australia
			PRINT_MSG("Set Tone of Country to AUSTRALIA\n");
			break;
		case 3://HK
			PRINT_MSG("Set Tone of Country to HONG KONG\n");
			break;
		case 4://Japan
			PRINT_MSG("Set Tone of Country to JAPAN\n");
			break;
		case 5://Sweden
			PRINT_MSG("Set Tone of Country to SWEDEN\n");
			break;
		case 6://Germany
			PRINT_MSG("Set Tone of Country to GERMANY\n");
			break;
		case 7://France
			PRINT_MSG("Set Tone of Country to FRANCE\n");
			break;
		case 8://TR57
			//PRINT_MSG("Set Tone of Country to TR57\n");
			PRINT_MSG("Set Tone of Country to TAIWAN\n");
			break;
		case 9://Belgium
			PRINT_MSG("Set Tone of Country to BELGIUM\n");
			break;
		case 10://Finland
			PRINT_MSG("Set Tone of Country to FINLAND\n");
			break;
		case 11://Italy
			PRINT_MSG("Set Tone of Country to ITALY\n");
			break;
		case 12://China
			PRINT_MSG("Set Tone of Country to CHINA\n");
			break;
		case 13://Customer
			PRINT_MSG("Set Tone of Country to CUSTOMER\n");
			break;
		default:
			PRINT_MSG("The tone you select is not support!\n");
			break;
	}

	if (stVoipValue.value == 4) // Japan 
	{
#if defined( CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3210 ) || defined( CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3215 )
		LEC_NLP_Config(2);
#elif defined (CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3226)
		LEC_NLP_Config(2);
#elif defined(CONFIG_RTK_VOIP_DRIVERS_SLIC_LE88221) || defined(CONFIG_RTK_VOIP_DRIVERS_SLIC_LE88266)
		LEC_NLP_Config(5);
#elif defined CONFIG_RTK_VOIP_DRIVERS_SLIC_W682388
		#error "Need to do NTT echo test for SLIC W682388"
#endif
	}
	else 
	{
#if defined (CONFIG_RTK_VOIP_DRIVERS_SLIC_SI3226)
		LEC_NLP_Config(5); 
#else
		LEC_NLP_Config(6); 
#endif
	}

	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_COUNTRY_TONE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));

	if ( DSPCODEC_COUNTRY_CUSTOME != stVoipValue.value)
	{
		RtkAc49xApiSetCountryTone(stVoipValue.value);
	}

	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_TONE
 * @brief Set customize tone 
 * @param TstVoipValue.value Use n-th customer tone 
 * @see VOIP_MGR_SET_TONE_OF_CUSTOMIZE TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_TONE_OF_CUSTOMIZE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	cust = stVoipValue.value;
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_TONE_OF_CUSTOMIZE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	tone_idx = stVoipValue.value;
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_TONE
 * @brief Set customize tone parameters 
 * @see VOIP_MGR_SET_CUST_TONE_PARAM TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_CUST_TONE_PARAM(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipToneCfg stVoipToneCfg;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipToneCfg, (TstVoipToneCfg*)para, sizeof(TstVoipToneCfg));
	
#if 0
	/*
	Because AudioCodes doesn't support play tone with fixd cycle, RTK could.
	To be identical, when RTK get cycle=2,
	change it to 0(continuous play tone with cadence)
	*/
	if (stVoipToneCfg.cycle == 0)	//Continuous
	{
		//set CadOn0 to non-zero value to ensure play continuous tone
		stVoipToneCfg.CadOn0 = 100;
		// set other CadOn/Off to zero to ensure not enter cadence tone
		stVoipToneCfg.CadOff0 = 0;
		stVoipToneCfg.CadOn1 = 0;
		stVoipToneCfg.CadOff1 = 0;
		stVoipToneCfg.CadOn2 = 0;
		stVoipToneCfg.CadOff2 = 0;
		stVoipToneCfg.CadOn3 = 0;
		stVoipToneCfg.CadOff3 = 0;
	}
	else if (stVoipToneCfg.cycle == 2)	//Cadence
		stVoipToneCfg.cycle = 0;	//Continuous
	
	//web setting unit is (-dBm)
	stVoipToneCfg.Gain1 = (-1)*stVoipToneCfg.Gain1;
	
	// if tone type is succeed, use the Gain1 value as the gain of other frequency.
	if (stVoipToneCfg.toneType == 2)//SUCC
	{
		stVoipToneCfg.Gain2 = stVoipToneCfg.Gain1;
		stVoipToneCfg.Gain3 = stVoipToneCfg.Gain1;
		stVoipToneCfg.Gain4 = stVoipToneCfg.Gain1;
	}
	else
	{
		stVoipToneCfg.Gain2 = (-1)*stVoipToneCfg.Gain2;
		stVoipToneCfg.Gain3 = (-1)*stVoipToneCfg.Gain3;
		stVoipToneCfg.Gain4 = (-1)*stVoipToneCfg.Gain4;
	}
#else
	//web setting unit is (-dBm)
	stVoipToneCfg.Gain1 = (-1)*stVoipToneCfg.Gain1;
	stVoipToneCfg.Gain2 = (-1)*stVoipToneCfg.Gain2;
	stVoipToneCfg.Gain3 = (-1)*stVoipToneCfg.Gain3;
	stVoipToneCfg.Gain4 = (-1)*stVoipToneCfg.Gain4;

#endif
	setTone( ( aspToneCfgParam_t * )&stVoipToneCfg);
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_CUST_TONE_PARAM(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipToneCfg stVoipToneCfg;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipToneCfg, (TstVoipToneCfg*)para, sizeof(TstVoipToneCfg));
	
	SaveCustomTone(&stVoipToneCfg);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_TONE
 * @brief Use customize tone 
 * @param TstVoipValue.value1 Customer dial tone 
 * @param TstVoipValue.value2 Customer ringing tone 
 * @param TstVoipValue.value3 Customer busy tone 
 * @param TstVoipValue.value4 Customer call waiting tone 
 * @see VOIP_MGR_USE_CUST_TONE TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_USE_CUST_TONE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	extern RESULT DspcodecSetCustomTone(DSPCODEC_TONE nTone, ToneCfgParam_t *pToneCfg);

	TstVoipValue stVoipValue;
	short *pToneTable;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	pToneTable = (short*)&ToneTable[CUSTOM_TONE1+stVoipValue.value1];	//dial
	DspcodecSetCustomTone(DSPCODEC_TONE_DIAL, (ToneCfgParam_t *)pToneTable);
	
	pToneTable = (short*)&ToneTable[CUSTOM_TONE1+stVoipValue.value2];	//ring
	DspcodecSetCustomTone(DSPCODEC_TONE_RINGING, (ToneCfgParam_t *)pToneTable);
	
	pToneTable = (short*)&ToneTable[CUSTOM_TONE1+stVoipValue.value3];	//busy
	DspcodecSetCustomTone(DSPCODEC_TONE_BUSY, (ToneCfgParam_t *)pToneTable);
	
	pToneTable = (short*)&ToneTable[CUSTOM_TONE1+stVoipValue.value4];	//waiting
	DspcodecSetCustomTone(DSPCODEC_TONE_CALL_WAITING, (ToneCfgParam_t *)pToneTable);
	
	return 0;
}
#else
int do_mgr_VOIP_MGR_USE_CUST_TONE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	extern int RtkAc49xApiSetCustomTone(TstVoipValue * custom_tone);
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	RtkAc49xApiSetCustomTone(&stVoipValue);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_TONE
 * @brief Enable disconnection tone detection 
 * @see VOIP_MGR_SET_DIS_TONE_DET TstVoipdistonedet_parm 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_DIS_TONE_DET(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#ifdef FXO_BUSY_TONE_DET
	TstVoipdistonedet_parm stVoipdistonedet_parm_mgr;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipdistonedet_parm_mgr, (TstVoipdistonedet_parm*)para, sizeof(TstVoipdistonedet_parm));
	
	memcpy(&stVoipdistonedet_parm, &stVoipdistonedet_parm_mgr, sizeof(TstVoipdistonedet_parm));
	
	dis_tone_det_cfg_apply();
#endif
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_DIS_TONE_DET(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	// no handler 
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_AGC
 * @brief Enable / disable speaker AGC 
 * @param TstVoipValue.value Enable (1) or disable (0) speaker AGC 
 * @see VOIP_MGR_SET_SPK_AGC TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_SPK_AGC(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	spk_agc_mode[stVoipValue.ch_id]=stVoipValue.value;
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_SPK_AGC(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	spk_agc_mode[stVoipValue.ch_id]=stVoipValue.value;
	agc_enable[stVoipValue.ch_id] =  spk_agc_mode[stVoipValue.ch_id];
	RtkAc49xApiAgcConfig(stVoipValue.ch_id, agc_enable[stVoipValue.ch_id], AGC_LOCATION__AT_DECODER_OUTPUT);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_AGC
 * @brief Set speaker AGC level 
 * @param TstVoipValue.value Speaker AGC level 
 * @see VOIP_MGR_SET_SPK_AGC_LVL TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_SPK_AGC_LVL(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	spk_agc_lvl[stVoipValue.ch_id]=stVoipValue.value;
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_SPK_AGC_LVL(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));

	if (spk_agc_mode[stVoipValue.ch_id]==1)// SPK AGC is enable
	{
	Tac49xAgcTargetEnergy tar_eng;
		/* stVoipValue.value range: 0(small:-25dBm) to 8(large:-1dBm), space: 3dBm */
		tar_eng = AGC_TARGET_ENERGY__minus25_DBM - 3*stVoipValue.value;
	RtkAc49xApiAgcEnergySlope(stVoipValue.ch_id, tar_eng, AGC_GAIN_SLOPE__1_00_DB_SEC);
	}
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_AGC
 * @brief Set speaker AGC gain up 
 * @param TstVoipValue.value Speaker AGC gain up value 
 * @see VOIP_MGR_SET_SPK_AGC_GUP TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_SPK_AGC_GUP(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	spk_agc_gup[stVoipValue.ch_id]=stVoipValue.value;
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_SPK_AGC_GUP(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	spk_agc_gup = stVoipValue.value;
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_AGC
 * @brief Set speaker AGC gain down 
 * @param TstVoipValue.value Speaker AGC gain down value 
 * @see VOIP_MGR_SET_SPK_AGC_GDOWN TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_SPK_AGC_GDOWN(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	spk_agc_gdown[stVoipValue.ch_id]=stVoipValue.value;
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_SPK_AGC_GDOWN(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	spk_agc_gdown = stVoipValue.value;
	//Note: IO ctrl VOIP_MGR_SET_SPK_AGC_GUP should be called first.
	if (spk_agc_mode[stVoipValue.ch_id] == 1)
		RtkAc49xApiAgcDeviceConfig(spk_agc_gdown+1, spk_agc_gup+1);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_AGC
 * @brief Enable / disable MIC AGC 
 * @param TstVoipValue.value Enable (1) or disable (0) MIC AGC 
 * @see VOIP_MGR_SET_MIC_AGC TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_MIC_AGC(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	mic_agc_mode[stVoipValue.ch_id]=stVoipValue.value;
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_MIC_AGC(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	mic_agc_mode[stVoipValue.ch_id]=stVoipValue.value;
	agc_enable[stVoipValue.ch_id] = agc_enable[stVoipValue.ch_id] | mic_agc_mode[stVoipValue.ch_id];
	if (mic_agc_mode[stVoipValue.ch_id] == 1)
		RtkAc49xApiAgcConfig(stVoipValue.ch_id, agc_enable[stVoipValue.ch_id], AGC_LOCATION__AT_ENCODER_INPUT);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_AGC
 * @brief Set MIC AGC level 
 * @param TstVoipValue.value MIC AGC level 
 * @see VOIP_MGR_SET_MIC_AGC_LVL TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_MIC_AGC_LVL(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	mic_agc_lvl[stVoipValue.ch_id]=stVoipValue.value;
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_MIC_AGC_LVL(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
		
	if (mic_agc_mode[stVoipValue.ch_id]==1)// MIC AGC is enable
	{
		Tac49xAgcTargetEnergy tar_eng;
		/* stVoipValue.value range: 0(small:-25dBm) to 8(large:-1dBm), space: 3dBm */
		tar_eng = AGC_TARGET_ENERGY__minus25_DBM - 3*stVoipValue.value;
		RtkAc49xApiAgcEnergySlope(stVoipValue.ch_id, tar_eng, AGC_GAIN_SLOPE__1_00_DB_SEC);
	}
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_AGC
 * @brief Set MIC AGC gain up 
 * @param TstVoipValue.value MIC AGC up value 
 * @see VOIP_MGR_SET_MIC_AGC_GUP TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_MIC_AGC_GUP(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	mic_agc_gup[stVoipValue.ch_id]=stVoipValue.value;
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_MIC_AGC_GUP(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	mic_agc_gup = stVoipValue.value;
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_AGC
 * @brief Set MIC AGC gain down 
 * @param TstVoipValue.value MIC AGC down value 
 * @see VOIP_MGR_SET_MIC_AGC_GDOWN TstVoipValue 
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_SET_MIC_AGC_GDOWN(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	mic_agc_gdown[stVoipValue.ch_id]=stVoipValue.value;
	return 0;
}
#else
int do_mgr_VOIP_MGR_SET_MIC_AGC_GDOWN(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	mic_agc_gdown = stVoipValue.value;
	//Note: IO ctrl VOIP_MGR_SET_MIC_AGC_GUP should be called first.
	if (mic_agc_mode[stVoipValue.ch_id] == 1)
		RtkAc49xApiAgcDeviceConfig(mic_agc_gdown+1, mic_agc_gup+1);
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_PLUSEDIAL
 * @brief Enable / disable pluse digits detection 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable Enable (1) or disable (0) pluse digits detection 
 * @param TstVoipCfg.cfg Pause time (ms) 
 * @see VOIP_MGR_SET_PULSE_DIGIT_DET TstVoipCfg 
 */
int do_mgr_VOIP_MGR_SET_PULSE_DIGIT_DET(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#ifdef PULSE_DIAL_DET
	TstVoipCfg stVoipCfg;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	set_pulse_det(stVoipCfg.ch_id, stVoipCfg.enable, stVoipCfg.cfg); /* 0: disable 1: enable Pulse Digit Detection */
#endif
	return 0;
}

/**
 * @ingroup VOIP_DSP_PLUSEDIAL
 * @brief Turn on / off pluse dial 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param TstVoipCfg.enable Enable (1) or disable (0) pluse digits detection 
 * @see VOIP_MGR_SET_DIAL_MODE TstVoipCfg 
 */
int do_mgr_VOIP_MGR_SET_DIAL_MODE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
#ifdef PULSE_DIAL_GEN
	TstVoipCfg stVoipCfg;
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	DAA_Set_PulseDial_cch(stVoipCfg.ch_id, stVoipCfg.cfg /* 0: disable 1: enable Pulse dial */);
#endif
#endif
	return 0;
}

/**
 * @ingroup VOIP_DSP_PLUSEDIAL
 * @brief Check whether dial mode is pluse dial 
 * @param TstVoipCfg.ch_id Channel ID 
 * @param [out] TstVoipCfg.cfg Dial mode (0: disable 1: enable Pulse dial)
 * @see VOIP_MGR_GET_DIAL_MODE TstVoipCfg 
 */
int do_mgr_VOIP_MGR_GET_DIAL_MODE(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
#ifdef PULSE_DIAL_GEN
	TstVoipCfg stVoipCfg;
	
	memcpy(&stVoipCfg, (TstVoipCfg*)para, sizeof(TstVoipCfg));
	
	stVoipCfg.cfg = DAA_Get_Dial_Mode(stVoipCfg.ch_id); /* 0: disable 1: enable Pulse dial */
	//copy_to_user(user, &stVoipCfg, sizeof(TstVoipCfg));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stVoipCfg, sizeof(TstVoipCfg)))
	{
		return 0;
	}
#endif
#endif
	return 0;
}

/**
 * @ingroup VOIP_DSP_PLUSEDIAL
 * @brief Configure plus dial generation 
 * @param TstVoipValue.value PPS 
 * @param TstVoipValue.value5 Make duration (ms) 
 * @param TstVoipValue.value6 Pause duration (ms) 
 * @see VOIP_MGR_PULSE_DIAL_GEN_CFG TstVoipValue 
 */
int do_mgr_VOIP_MGR_PULSE_DIAL_GEN_CFG(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
#ifdef PULSE_DIAL_GEN
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	DAA_PulseDial_Gen_Cfg(stVoipValue.value/*pps*/, stVoipValue.value5/*make duration*/, stVoipValue.value6/*pause duration*/);
#endif
#endif
	return 0;
}

/**
 * @ingroup VOIP_DSP_PLUSEDIAL
 * @brief Generate pluse dial 
 * @param TstVoipValue.ch_id Channel ID 
 * @param TstVoipValue.value Digit 
 * @see VOIP_MGR_GEN_PULSE_DIAL TstVoipValue 
 */
int do_mgr_VOIP_MGR_GEN_PULSE_DIAL(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#ifdef CONFIG_RTK_VOIP_DRIVERS_FXO
#ifdef PULSE_DIAL_GEN
	TstVoipValue stVoipValue;
	
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipValue, (TstVoipValue*)para, sizeof(TstVoipValue));
	
	pulse_dial_in_cch(stVoipValue.ch_id, stVoipValue.value/*digit*/);
#endif
#endif
	return 0;
}

/**
 * @ingroup VOIP_DSP_IVR
 * @brief Play textual, G.711, G.729 and G.723 IVR 
 * @note This function can play four kinds of IVR, and echo of them 
 *       use its structure. 
 * @see VOIP_MGR_PLAY_IVR TstVoipPlayIVR_Header TstVoipPlayIVR_G711 TstVoipPlayIVR_G729 TstVoipPlayIVR_G72363 TstVoipPlayIVR_Text
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_PLAY_IVR(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#if 0
	TstVoipPlayIVR_Header * const pHeader = stVoipPlayIVR_Header;
	TstVoipPlayIVR_G72363 * const pHeaderG723 = stVoipPlayIVR_Header;
	TstVoipPlayIVR_G729 * const pHeaderG729 = stVoipPlayIVR_Header;
	TstVoipPlayIVR_G711 * const pHeaderG711 = stVoipPlayIVR_Header;
	TstVoipPlayIVR_Text * const pHeaderText = stVoipPlayIVR_Header;
			
	stVoipPlayIVR_Header = (TstVoipPlayIVR_Header*)para;
			
	switch( pHeader->type )
	{
		case IVR_PLAY_TYPE_G723_63:
			//PRINT_Y("G.723 IVR Play\n");
			pHeader ->playing_period_10ms = 
					PlayIvrG72363( pHeaderG723->ch_id,
								   pHeaderG723->nFramesCount,
								   pHeaderG723->data,
								   &pHeaderG723 ->nRetCopiedFrames );
			break;
			
		case IVR_PLAY_TYPE_G729:
			//PRINT_Y("G.729 IVR Play\n");
			pHeader ->playing_period_10ms = 
					PlayIvrG729( pHeaderG729->ch_id,
								 pHeaderG729->nFramesCount,
								 pHeaderG729->data,
								 &pHeaderG729 ->nRetCopiedFrames );	
			break;
					
		case IVR_PLAY_TYPE_G711A:
			//PRINT_Y("G.711a IVR Play\n");
			pHeader ->playing_period_10ms = 
					PlayIvrG711( pHeaderG711->ch_id,
								 pHeaderG711->nFramesCount,
								 pHeaderG711->data,
								 &pHeaderG711 ->nRetCopiedFrames );	
			break;		

#ifdef CONFIG_RTK_VOIP_IVR_TEXT				
		case IVR_PLAY_TYPE_TEXT:
			//PRINT_Y("Text IVR Play\n");
			pHeader ->playing_period_10ms =
				PlayIvrText2Speech( pHeaderText->ch_id, pHeaderText->direction, pHeaderText->szText2speech );
			break;
#endif

		default:
			PRINT_Y( "IVR not support this type:(%d)\n", pHeader ->type );
			break;		
	}

	//copy_to_user(user, &stVoipPlayIVR_Header, sizeof(TstVoipPlayIVR_Header));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, stVoipPlayIVR_Header, sizeof(TstVoipPlayIVR_Header)))
	{
		return 0;
	}
#endif
	return 0;
}
#else
int do_mgr_VOIP_MGR_PLAY_IVR(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
#if 0 // ToDo
	extern void SetTextPlayBuf(unsigned int chid, unsigned char* pText);
	TstVoipPlayIVR_Header stVoipPlayIVR_Header;

#ifndef ACMW_PLAYBACK
	copy_from_user(&stVoipPlayIVR_Header, (TstVoipPlayIVR_Header *)user, sizeof(TstVoipPlayIVR_Header));
	PlayIvrDispatcher( &stVoipPlayIVR_Header, user );
	copy_to_user(user, &stVoipPlayIVR_Header, sizeof(TstVoipPlayIVR_Header));
#else
	{	
		union
		{
			TstVoipPlayIVR_Text	stVoipPlayIVR_Text;
			TstVoipPlayIVR_G72363	stVoipPlayIVR_G72363;
			TstVoipPlayIVR_G729	stVoipPlayIVR_G729;
			TstVoipPlayIVR_G711	stVoipPlayIVR_G711;
		} save;

		TstVoipPlayIVR_Header * const pHeader = &stVoipPlayIVR_Header;
		TstVoipPlayIVR_G72363 * const pHeaderG723 = ( TstVoipPlayIVR_G72363 * )&stVoipPlayIVR_Header;
		TstVoipPlayIVR_G729 * const pHeaderG729 = ( TstVoipPlayIVR_G729 * )&stVoipPlayIVR_Header;
		TstVoipPlayIVR_G711 * const pHeaderG711 = ( TstVoipPlayIVR_G711 * )&stVoipPlayIVR_Header;
		copy_from_user(pHeader, (TstVoipPlayIVR_Header *)user, sizeof(TstVoipPlayIVR_Header));

		switch( pHeader ->type )
		{
			case IVR_PLAY_TYPE_G723_63:
			{
				copy_from_user(&save.stVoipPlayIVR_G72363, (TstVoipPlayIVR_G72363 *)user, sizeof(TstVoipPlayIVR_G72363));
				unsigned int ch = save.stVoipPlayIVR_G72363.ch_id;
				extern int play_g723h_flag[];
				
				pHeaderG723->nRetCopiedFrames = IvrPlayBufWrite(ch, (char*)save.stVoipPlayIVR_G72363.data, save.stVoipPlayIVR_G72363.nFramesCount, G723_FRAME_SIZE);
				//PRINT_R("[%d]\n", pHeaderG723->nRetCopiedFrames);// The length which write to play buf
			
				if (play_g723h_flag[ch] == 0)
				{
					play_g723h_flag[ch] = 1;
					RtkAc49xApiPlayIvrTdmStart(ch, CODER__G723HIGH);
					PRINT_MSG("Play G723-6.3K File Start!\n");
				}

				break;
			}
			
			case IVR_PLAY_TYPE_G729:
			{
				copy_from_user(&save.stVoipPlayIVR_G729, (TstVoipPlayIVR_G729 *)user, sizeof(TstVoipPlayIVR_G729));
				unsigned int ch = save.stVoipPlayIVR_G729.ch_id;
				extern int play_g729_flag[];
				
				pHeaderG729->nRetCopiedFrames = IvrPlayBufWrite(ch, (char*)save.stVoipPlayIVR_G729.data, save.stVoipPlayIVR_G729.nFramesCount, G729_FRAME_SIZE);
				//PRINT_R("[%d]\n", pHeaderG729->nRetCopiedFrames);// The length which write to play buf
			
				if (play_g729_flag[ch] == 0)
				{
					play_g729_flag[ch] = 1;
					RtkAc49xApiPlayIvrTdmStart(ch, CODER__G729);
					PRINT_MSG("Play G729 File Start!\n");
				}

				break;
			}
			
			case IVR_PLAY_TYPE_G711A:
			{
				copy_from_user(&save.stVoipPlayIVR_G711, (TstVoipPlayIVR_G711 *)user, sizeof(TstVoipPlayIVR_G711));
				unsigned int ch = save.stVoipPlayIVR_G711.ch_id;
				extern int play_g711_flag[];
				
				pHeaderG711->nRetCopiedFrames = IvrPlayBufWrite(ch, (char*)save.stVoipPlayIVR_G711.data, save.stVoipPlayIVR_G711.nFramesCount, G711_FRAME_SIZE);
				//PRINT_R("[%d]\n", pHeaderG711->nRetCopiedFrames);// The length which write to play buf
			
				if (play_g711_flag[ch] == 0)
				{
					play_g711_flag[ch] = 1;
					RtkAc49xApiPlayIvrTdmStart(ch, CODER__G711ALAW);
					PRINT_MSG("Play G711 File Start!\n");
				}

				break;
			}
			
			case IVR_PLAY_TYPE_TEXT:
			{
				copy_from_user(&save.stVoipPlayIVR_Text, (TstVoipPlayIVR_Text *)user, sizeof(TstVoipPlayIVR_Text));		
				SetTextPlayBuf(save.stVoipPlayIVR_Text.ch_id, save.stVoipPlayIVR_Text.szText2speech);
				break;
			}
		}
		
		copy_to_user(user, &stVoipPlayIVR_Header, sizeof(TstVoipPlayIVR_Header));
	}
#endif
#endif
	return 0;
}
#endif

/**
 * @ingroup VOIP_DSP_IVR
 * @brief Check if IVR is still playing 
 * @see VOIP_MGR_POLL_IVR TstVoipPollIVR
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_POLL_IVR(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipPollIVR stVoipPollIVR;

	memcpy(&stVoipPollIVR, (TstVoipPollIVR*)para, sizeof(TstVoipPollIVR));
	
	stVoipPollIVR.bPlaying =
			PollIvrPlaying( stVoipPollIVR.ch_id );
	//copy_to_user(user, &stVoipPollIVR, sizeof(TstVoipPollIVR));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stVoipPollIVR, sizeof(TstVoipPollIVR)))
	{
		return 0;
	}
	return 0;
}
#else
int do_mgr_VOIP_MGR_POLL_IVR(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipPollIVR stVoipPollIVR;

	memcpy(&stVoipPollIVR, (TstVoipPollIVR*)para, sizeof(TstVoipPollIVR));
	
#ifndef ACMW_PLAYBACK
	stVoipPollIVR.bPlaying =
		PollIvrPlaying( stVoipPollIVR.ch_id );
#else
	/*
		#define PB_END	0
		#define PB_TDM	1
		#define PB_NET	2
		#define PB_SIL	3
	*/

	if ( RtkAc49xApiPollIvr(stVoipPollIVR.ch_id) == 0 )
		stVoipPollIVR.bPlaying = 0;
	else
		stVoipPollIVR.bPlaying = 1;
#endif
	//copy_to_user(user, &stVoipPollIVR, sizeof(TstVoipPollIVR));
	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, &stVoipPollIVR, sizeof(TstVoipPollIVR)))
	{
		return 0;
	}
	return 0;	
}
#endif

/**
 * @ingroup VOIP_DSP_IVR
 * @brief Stop IVR playing 
 * @see VOIP_MGR_STOP_IVR TstVoipStopIVR
 */
#if ! defined (AUDIOCODES_VOIP)
int do_mgr_VOIP_MGR_STOP_IVR(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipStopIVR stVoipStopIVR;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipStopIVR, (TstVoipStopIVR*)para, sizeof(TstVoipStopIVR));
	
	StopIvrPlaying( stVoipStopIVR.ch_id );
	return 0;
}
#else
int do_mgr_VOIP_MGR_STOP_IVR(unsigned short cmd, unsigned char* para, unsigned short seq_no, unsigned short length)
{
	TstVoipStopIVR stVoipStopIVR;

	if (1 == ethernetDspSentL2ResponsePacket(cmd, seq_no, NULL, 0))
	{
		return 0;
	}
	memcpy(&stVoipStopIVR, (TstVoipStopIVR*)para, sizeof(TstVoipStopIVR));
	
#ifndef ACMW_PLAYBACK
	StopIvrPlaying( stVoipStopIVR.ch_id );
#else
	RtkAc49xApiPlayIvrEnd(stVoipStopIVR.ch_id);
#endif
	return 0;
}
#endif


