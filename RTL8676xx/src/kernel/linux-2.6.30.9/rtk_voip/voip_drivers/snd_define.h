#ifndef __SND_DEFINE_H__
#define __SND_DEFINE_H__

//#include <linux/config.h>
//#include "../voip_dsp/include/fskcid_gen.h"

#define PHONE_ON_HOOK		0
#define PHONE_OFF_HOOK		1
#define PHONE_FLASH_HOOK	2
#define PHONE_STILL_ON_HOOK	3
#define PHONE_STILL_OFF_HOOK	4
#define PHONE_UNKNOWN		5
#define FXO_ON_HOOK		6
#define FXO_OFF_HOOK		7
#define	FXO_FLASH_HOOK		8
#define FXO_STILL_ON_HOOK	9
#define FXO_STILL_OFF_HOOK	10
#define FXO_RING_ON		11
#define FXO_RING_OFF		12
#define FXO_BUSY_TONE		13
#define FXO_CALLER_ID		14
#define FXO_RING_TONE_ON	15
#define FXO_RING_TONE_OFF	16
#define FXO_POLARITY_REVERSAL	17
#define FXO_CURRENT_DROP	18
#define FXO_DIS_TONE		19
#define FXO_UNKNOWN		20
#define OUTBAND_FLASH_EVENT	21
#define FAX_CED			30
#define FAX_DIS_TX		31
#define FAX_DIS_RX		32
#define FAX_DCN_TX		33
#define FAX_DCN_RX		34
#define MODEM_LOCAL		35
#define MODEM_LOCAL_DELAY	36      // This event means that DSP need to check if DIS_RX event occur within 4~6 sec after modem tone event is detected.
#define MODEM_REMOTE		37      // MODEM_REMOTE must right after MODEM_LOCAL_DELAY. see ced.c


#define SLIC_PCM_OFF		0
#define SLIC_PCM_ON		1

typedef enum
{
	PCMMODE_LINEAR,
	PCMMODE_ALAW,
	PCMMODE_ULAW,
	PCMMODE_WIDEBAND_LINEAR,
	PCMMODE_WIDEBAND_ALAW,
	PCMMODE_WIDEBAND_ULAW,
	PCMMODE_UNKNOWN,
} PCM_MODE;

typedef enum
{
    COUNTRY_USA,		/* USA			*/
    COUNTRY_UK,			/* UK			*/
    COUNTRY_AUSTRALIA,	/* Australia	*/
    COUNTRY_HK,			/* HK			*/
    COUNTRY_JP,			/* Japan		*/
    COUNTRY_SE,			/* Sweden		*/
    COUNTRY_GR,			/* German		*/
    COUNTRY_FR,			/* France		*/
    COUNTRY_TW,			/* Taiwan		*/
    COUNTRY_BE,			/* Belgium 		*/
    COUNTRY_FL,			/* Finland 		*/
    COUNTRY_IT,			/* Italy 		*/
    COUNTRY_CN,			/* China		*/
    COUNTRY_CUSTOME
}COUNTRY;		// reference to VOIP_MGR_SET_COUNTRY if you want to modify this table 


typedef struct {
	unsigned char CH;		// CH = 0 ~ 3
	unsigned char ring_set;		// Ring_ON: ring_set = 1 ,  Ring_OFF: ring_set = 0

} ring_struct;

#if 0
typedef struct {
	unsigned char CH;		// CH:0 - 3
	unsigned char change;		// 1: Change. 0: No-Change
	unsigned char hook_status;	// 1: Off-Hook, 0: On-Hook
} hook_struck;
#endif

typedef struct {
	unsigned int pause_time;	// unit in ms
	unsigned int break_min_ths;	// unit in ms
	unsigned int break_max_ths;	// unit in ms
}stPulseDetParam;


#if 0
/*Slic_api.c function prototype*/
int SLIC_init(int pcm_mode);
void SLIC_reset(int CH, int codec_law, unsigned char slic_number);
void CID_for_FSK_HW(unsigned int chid, char mode, unsigned char msg_type, char *str, char *str2, char *cid_name);
void CID_for_FSK_SW(unsigned int chid, char service_type, unsigned char msg_type, TstFskClidData* clid_data);
void FXS_Ring(ring_struct *ring);
unsigned char FXS_Check_Ring(ring_struct *ring);
void Hook_state(hook_struck *hook);
void Set_SLIC_Tx_Gain(unsigned char chid, unsigned char tx_gain);
void Set_SLIC_Rx_Gain(unsigned char chid, unsigned char rx_gain);
void SLIC_Set_Ring_Cadence(unsigned char chid, unsigned short OnMsec, unsigned short OffMsec);
void SLIC_Set_Ring_Freq_Amp(unsigned char chid, char preset);
void Init_Event_Polling_Use_Timer(void);
void SLIC_Hook_Polling(hook_struck *hook, unsigned int fhk_min_time, unsigned int fhk_time);
void OnHookLineReversal(int chid, unsigned char bReversal);
void SLIC_Set_LineVoltageZero(int chid);
void SLIC_CPC_Gen(int chid, unsigned int time_in_ms_of_cpc_signal);
void SendNTTCAR(int chid);
void disableOscillators(unsigned int chid);
void SetOnHookTransmissionAndBackupRegister(int chid);
void RestoreBackupRegisterWhenSetOnHookTransmission(unsigned int chid);
void SLIC_Set_Impendance_Country(unsigned char chid, unsigned short country, unsigned short impd);
void SLIC_Set_Impendance(unsigned char chid, unsigned short preset);
void SLIC_Set_PCM_state(int chid, int enable);
void SLIC_gen_FSK_CID(unsigned int chid, char mode, unsigned char msg_type, TstFskClidData* clid_data);
int SLIC_gen_VMWI(unsigned int chid, TstFskClidData* clid_data);
void ring_det_cad_set( unsigned int ring_on_msec, unsigned int first_ringoff_msec, unsigned int ring_off_msec);
void ring_times_set(unsigned int chid, unsigned int ringOn, unsigned int ringOff);
void vir_daa_ring_det_set(unsigned int on_ths, unsigned int off_ths);
unsigned char SLIC_Get_Hook_Status(int chid);
void SLIC_read_ram(unsigned char chid, unsigned short num, unsigned int *val);
void SLIC_dump_reg(unsigned char chid);
void SLIC_dump_ram(unsigned char chid);

#ifdef PULSE_DIAL_DET
void set_pulse_det(unsigned int chid, unsigned int enable, unsigned int pause_time);
unsigned int get_pulse_det(unsigned int chid);
#endif
void HookPollingEnable(int chid);
void HookPollingDisable(int chid);
void FXS_FXO_functionality_init(unsigned char chid);
void FXS_FXO_DTx_DRx_Loopback(unsigned char chid, unsigned int enable);
unsigned int FXS_Line_Check(int chid);
void SLIC_OnHookTrans_PCM_start(unsigned char chid);

/*Slic_api.c variable extern*/
extern char fsk_cid_state[];
extern char ntt_skip_dc_loop[];
extern volatile char fsk_alert_state[];
extern volatile char fsk_alert_time[];

extern char fsk_spec_areas[];
extern int Is_DAA_Channel(int chid);

/* SLIC relay */
extern void SLIC_Relay_init( void );
extern void SLIC_Relay_set(unsigned char chid, unsigned char on);
#endif

#endif	/* __SND_DEFINE_H__ */

