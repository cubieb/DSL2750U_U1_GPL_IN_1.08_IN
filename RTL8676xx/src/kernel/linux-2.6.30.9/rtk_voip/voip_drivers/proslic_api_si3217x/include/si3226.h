#ifndef SI3226H_H
#define SI3226H_H

#include "proslic.h"

/*
** SI3226 DataTypes/Function Definitions 
*/

#define NUMIRQ 4


/*
** Defines structure for configuring gpio
*/
typedef struct {
	uInt8 outputEn;
	uInt8 analog;
	uInt8 direction;
	uInt8 manual;
	uInt8 polarity;
	uInt8 openDrain;
	uInt8 batselmap;
} Si3226_GPIO_Cfg;

/*
** Defines structure for configuring dc feed
*/
typedef struct {
	ramData slope_vlim;
	ramData slope_rfeed;
	ramData slope_ilim;
	ramData delta1;
	ramData delta2;
	ramData v_vlim;
	ramData v_rfeed;
	ramData v_ilim;
	ramData const_rfeed;
	ramData const_ilim;
	ramData i_vlim;
	ramData lcronhk;
	ramData lcroffhk;
	ramData lcrdbi;
	ramData longhith;
	ramData longloth;
	ramData longdbi;
	ramData lcrmask;
	ramData lcrmask_polrev;
	ramData lcrmask_state;
	ramData lcrmask_linecap;
	ramData vcm_oh;
	ramData vov_bat;
	ramData vov_gnd;
} Si3226_DCfeed_Cfg;
/*
** Defines structure for configuring dcdc converter
*/
typedef struct {
	ramData vbatr_expect;
	ramData dcdc_vref_min;
	ramData dcdc_fsw_norm;
	ramData dcdc_fsw_norm_lo;
	ramData dcdc_fsw_ring;
	ramData dcdc_fsw_ring_lo;
	ramData dcdc_din_lim;
	ramData dcdc_vout_lim;
	ramData dcdc_uvhyst;
	ramData dcdc_uvthresh;
	ramData dcdc_ovthresh;
	ramData dcdc_oithresh;
	ramData coef_p_hvic;
	ramData p_th_hvic;
	ramData vbath_expect;
	uInt8	cm_clamp;
	uInt8	autoRegister;
	ramData cm_dbi;
	uInt8 irqen1;
	uInt8 irqen2;
	uInt8 irqen3;
	uInt8 irqen4;
} Si3226_General_Cfg;
/*
** Defines structure for configuring pcm
*/
typedef struct {
	uInt8 pcmFormat;
	uInt8 widebandEn;
	uInt8 pcm_tri;
	uInt8 tx_edge;

} Si3226_PCM_Cfg;

/*
** Defines structure for configuring pulse metering
*/
typedef void Si3226_PulseMeter_Cfg;

/*
** Defines structure for configuring FSK generation
*/
typedef struct {
	ramData fsk01;
	ramData fsk10;
	ramData fskamp0;
	ramData fskamp1;
	ramData fskfreq0;
	ramData fskfreq1;
	uInt8 eightBit;
	uInt8 fskdepth;
} Si3226_FSK_Cfg;

/*
** Defines structure for configuring dtmf decode
*/
typedef struct {
	ramData dtmfdtf_b0_1;
	ramData dtmfdtf_b1_1;
	ramData dtmfdtf_b2_1;
	ramData dtmfdtf_a1_1;
	ramData dtmfdtf_a2_1;
	ramData dtmfdtf_b0_2;
	ramData dtmfdtf_b1_2;
	ramData dtmfdtf_b2_2;
	ramData dtmfdtf_a1_2;
	ramData dtmfdtf_a2_2;
	ramData dtmfdtf_b0_3;
	ramData dtmfdtf_b1_3;
	ramData dtmfdtf_b2_3;
	ramData dtmfdtf_a1_3;
	ramData dtmfdtf_a2_3;
} Si3226_DTMFDec_Cfg;

/*
** Defines structure for configuring impedence synthesis
*/
typedef struct {
	ramData zsynth_b0;
	ramData zsynth_b1;
	ramData zsynth_b2;
	ramData zsynth_a1;
	ramData zsynth_a2;
	uInt8 ra;
} Si3226_Zsynth_Cfg;

/*
** Defines structure for configuring hybrid
*/
typedef struct {
	ramData ecfir_c2;
	ramData ecfir_c3;
	ramData ecfir_c4;
	ramData ecfir_c5;
	ramData ecfir_c6;
	ramData ecfir_c7;
	ramData ecfir_c8;
	ramData ecfir_c9;
	ramData ecfir_b0;
	ramData ecfir_b1;
	ramData ecfir_a1;
	ramData ecfir_a2;
} Si3226_hybrid_Cfg;

/*
** Defines structure for configuring GCI CI bits
*/
typedef struct {
	uInt8 gci_ci;
} Si3226_CI_Cfg;

/*
** Defines structure for configuring modem tone detect
*/
typedef struct {
	ramData rxmodpwr;
	ramData rxpwr;
	ramData modem_gain;
	ramData txmodpwr;
	ramData txpwr;
} Si3226_modemDet_Cfg;

/*
** Defines structure for configuring audio eq
*/
typedef struct {
	ramData txaceq_c0;
	ramData txaceq_c1;
	ramData txaceq_c2;
	ramData txaceq_c3;

	ramData rxaceq_c0;
	ramData rxaceq_c1;
	ramData rxaceq_c2;
	ramData rxaceq_c3;
} Si3226_audioEQ_Cfg;

/*
** Defines structure for configuring audio gain
*/
typedef struct {
	ramData acgain;
	uInt8 mute;
} Si3226_audioGain_Cfg;



typedef struct {
	Si3226_audioEQ_Cfg audioEQ;
	Si3226_hybrid_Cfg hybrid;
    Si3226_Zsynth_Cfg zsynth;
	Si3226_audioGain_Cfg txgain;
	Si3226_audioGain_Cfg rxgain;
	ramData rxachpf_b0_1;
	ramData  rxachpf_b1_1;
	ramData  rxachpf_a1_1;
} Si3226_Impedance_Cfg;

/*
** Defines structure for configuring tone generator
*/
typedef struct {
	Oscillator_Cfg osc1;
	Oscillator_Cfg osc2;
	uInt8 omode;
} Si3226_Tone_Cfg; 

/*
** Defines structure for configuring ring generator
*/
typedef struct {
	ramData freq;
	ramData amp;
	ramData offset;
	ramData phas;
	ramData rtper;
	uInt8 talo;
	uInt8 tahi;
	uInt8 tilo;
	uInt8 tihi;
	uInt8 ringcon;
	ramData rtcount;
	ramData adap_ring_min_i;
	ramData slope_ring;
	ramData rrd_delay;
	ramData rrd_delay2;
	ramData counter_vtr_val;
	ramData rtacdb;
	ramData rtacth;
	ramData rtdcdb;
	ramData rtdcth;
	ramData vov_ring_bat;
	ramData vov_ring_gnd;
	ramData vcm_ring;
} Si3226_Ring_Cfg;



/*
** This defines names for the interrupts in the ProSLIC
*/
typedef enum {
OSC1_T1_SI3226,
OSC1_T2_SI3226,
OSC2_T1_SI3226,
OSC2_T2_SI3226,
RING_T1_SI3226,
RING_T2_SI3226,
FSKBUF_AVAIL_SI3226,
VBAT_SI3226,
RING_TRIP_SI3226,
LOOP_STAT_SI3226,
LONG_STAT_SI3226,
VOC_TRACK_SI3226,
DTMF_SI3226,
INDIRECT_SI3226,
TXMDM_SI3226,
RXMDM_SI3226,
PQ1_SI3226,
PQ2_SI3226,
PQ3_SI3226,
PQ4_SI3226,
PQ5_SI3226,
PQ6_SI3226,
RING_FAIL_SI3226,
CM_BAL_SI3226,
USER_IRQ0_SI3226,
USER_IRQ1_SI3226,
USER_IRQ2_SI3226,
USER_IRQ3_SI3226,
USER_IRQ4_SI3226,
USER_IRQ5_SI3226,
USER_IRQ6_SI3226,
USER_IRQ7_SI3226
}Si3226ProslicInt;

#endif
