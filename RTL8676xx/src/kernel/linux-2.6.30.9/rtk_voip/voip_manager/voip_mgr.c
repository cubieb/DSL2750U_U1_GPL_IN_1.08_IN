#include <linux/config.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/if_ether.h>
#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <linux/spinlock.h>
#include <asm/unaligned.h>
#include <linux/netfilter.h>

#include "../include/voip_types.h"
#include "../voip_rx/rtk_trap.h"
#include "voip_feature.h"
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
//#include "../voip_tx/direct_send_mgr.h"
#include "../voip_tx/ethernet_dsp_tx.h"
#else
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
#include "voip_mgr_help_dsp.h"
#include "../voip_tx/ethernet_dsp_tx.h"
#endif
//#include "../voip_drivers/Slic_api.h"
#endif
#include "con_mux.h"
#include "snd_define.h"
#include "../include/voip_control.h"
#include "../include/voip_params.h"
#include "../include/voip_version.h"
#include "../voip_dsp/rtp/rtpTypes.h"
#include "voip_init.h"

char bDebugMsg = 1;	// enable or disable debug message. Default is enable.
char benableDbg = 0;	// enable or disable debug message. Default is disable.
char bBootMsg = 0;
#ifndef CONFIG_RTK_VOIP_ETHERNET_DSP
extern struct nf_sockopt_ops voip_mgr_sockopts;
#endif
extern int dsp_init_first;
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP
extern int voip_ch_num;
#endif

#if defined( CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST )
#elif defined( CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP )
//int rtkVoipIsEthernetDsp = 1;
void ( * fn_ethernetDsp_RtpTx )(RtpPacket* pst) = ethernetDsp_RtpTx;
void ( * fn_ethernetDsp_RtcpTx )(RtcpPacket* pst) = ethernetDsp_RtcpTx;
void ( * fn_ethernetDsp_T38Tx )( unsigned int chid, unsigned int sid, void* packet, unsigned int pktLen) = ethernetDsp_T38Tx;
#else
//int rtkVoipIsEthernetDsp = 0;
void ( * fn_ethernetDsp_RtpTx )(RtpPacket* pst) = NULL;
void ( * fn_ethernetDsp_RtcpTx )(RtcpPacket* pst) = NULL;
void ( * fn_ethernetDsp_T38Tx )( unsigned int chid, unsigned int sid, void* packet, unsigned int pktLen) = NULL;
#endif

#if defined (CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP) && defined (CONFIG_RTK_VOIP_LED)
extern volatile unsigned int sip_registed[];     //register ok:1, no register:0
extern volatile unsigned int daa_hook_status[];  //off-hook:1 ,on-hook:0
extern volatile unsigned int slic_hook_status[]; //off-hook:1 ,on-hook:0 ,flash-hook:2
extern volatile unsigned int fxs_ringing[];                      //no ring:0 ,voip incoming ring:1
extern volatile unsigned int daa_ringing;
#ifdef CONFIG_RTK_VOIP_DRIVERS_VIRTUAL_DAA
extern unsigned int pstn_ringing;	//pstn incoming ring:1 ,no ring and voip incoming ring:0
extern char relay_2_PSTN_flag[]; /* 1: relay to PSTN , 0: relay to SLIC */
#elif defined CONFIG_RTK_VOIP_DRIVERS_SI3050
extern unsigned int pstn_ringing[];
#else
extern unsigned int pstn_ringing[];
#endif
#endif

int fax_modem_in(uint32 ch_id, char input);
int fax_modem_out(uint32 ch_id);
void flush_fax_modem_fifo(uint32 ch_id);

//shlee move from pcm_interface.c due to host side doesnt include voip_driver folder.
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
/* Configure Cache at write allocate mode */
void set_write_allocate( void )
{
	unsigned int  temp;
__asm__ __volatile__ \
("	;\n\t"
	"mfc0	$8, $20		;\n\t"
	"nop					;\n\t"
	"or 		$8, $8, 0x80	;\n\t"
	"nop					;\n\t"
	"mtc0	$8, $20		;\n\t"
	"nop					;\n\t"
	"nop					;\n\t"
	"mfc0	%0, $20		;\n\t"
	:"=r" (temp)
	:
);
	//printk("\r\nset_write_allocate : temp:%X", temp);

}

void read_write_allocate( void )
{
	unsigned int  temp;
__asm__ __volatile__ \
("	;\n\t"
	"nop					;\n\t"
	"mfc0	%0, $20		;\n\t"
	:"=r" (temp)
	:
);
	printk("\r\nread_write_allocate : temp:%X", temp);

}
#endif


#define USE_DTMF_FIFO

#ifndef USE_DTMF_FIFO
static char latest_input[MAX_VOIP_CH_NUM] = {[0 ... MAX_VOIP_CH_NUM-1]='Z'};

int dtmf_in(uint32 ch_id, char input)
{
	if(input != 'Z') { //slince
	  latest_input[ch_id] = input;
	  //printk("k:%c ", latest_input[ch_id]);
	}
	return 0;
}

int dtmf_out(uint32 ch_id, char * output)
{
	*output = latest_input[ch_id];
	latest_input[ch_id] = 'Z';
	return 0;
}
#else  // kenny: DTMF FIFO
#define DTMF_PCM_FIFO_SIZE 10
static char latest_input[MAX_VOIP_CH_NUM][DTMF_PCM_FIFO_SIZE]={{0}};
static char dtmf_wp[MAX_VOIP_CH_NUM]={0}, dtmf_rp[MAX_VOIP_CH_NUM]={0};

int dtmf_in(uint32 ch_id, char input)
{
	if(input != 'Z') { //slince
		if ((dtmf_wp[ch_id]+1)%DTMF_PCM_FIFO_SIZE != dtmf_rp[ch_id])
		{
	  		latest_input[ch_id][(unsigned char)(dtmf_wp[ch_id])] = input;
	  		//PRINT_G("dtmf_in:%c(%d)\n", latest_input[ch_id][dtmf_wp[ch_id]], ch_id);
			dtmf_wp[ch_id] = (dtmf_wp[ch_id]+1) % DTMF_PCM_FIFO_SIZE;
	  	}
	  	else // FIFO overflow
	  	{ 
	  		PRINT_R("DTMF FIFO overflow,(%d)\n", ch_id);
	  	}
	}
	return 0;
}


#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
int dtmf_in_from_pkt(uint32 ch_id, char input)
{
	return dtmf_in( ch_id, input );
}
#endif

int dtmf_out(uint32 ch_id, char * output)
{
	if ( dtmf_wp[ch_id] == dtmf_rp[ch_id]) {
		// FIFO empty
		*output = 'Z';
	}
	else
	{
#if 0
		if ((latest_input[ch_id][dtmf_rp[ch_id]] > '9') || (latest_input[ch_id][dtmf_rp[ch_id]] < '0'))
		{
			if ((latest_input[ch_id][dtmf_rp[ch_id]] > 'D') || (latest_input[ch_id][dtmf_rp[ch_id]] < 'A'))
			{
				if ((latest_input[ch_id][dtmf_rp[ch_id]] != '*') && (latest_input[ch_id][dtmf_rp[ch_id]] != '#'))
				{
					PRINT_R("Err for dtmf_out: %d(%d)\n", latest_input[ch_id][dtmf_rp[ch_id]], ch_id);
					*output = 'Z';
					return 0;
				}
			}
		}
#endif
		*output = latest_input[ch_id][(unsigned char)(dtmf_rp[ch_id])];
                dtmf_rp[ch_id] = (unsigned char)(dtmf_rp[ch_id]+1) % DTMF_PCM_FIFO_SIZE;
	}
//	latest_input[ch_id] = 'Z';
	return 0;
}

void flush_dtmf_fifo(uint32 ch_id)
{
	int i;
	for (i=0; i< DTMF_PCM_FIFO_SIZE; i++)
	{
		latest_input[ch_id][i] = 0;
	}
	dtmf_wp[ch_id] = 0;
	dtmf_rp[ch_id] = 0;
}
#endif


#define USE_HOOK_FIFO

#ifdef USE_HOOK_FIFO
#define HOOK_PCM_FIFO_SIZE 10
static char hook_fifo[MAX_VOIP_CH_NUM][HOOK_PCM_FIFO_SIZE] = {{0}};
static char hook_wp[MAX_VOIP_CH_NUM]={0}, hook_rp[MAX_VOIP_CH_NUM]={0};
static char prev_out[MAX_VOIP_CH_NUM]={0};

#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
static int hook_det_flag[MAX_VOIP_CH_NUM] = {0};

int hook_det_start(uint32 ch_id)
{
	hook_det_flag[ch_id] = 1;
}

int hook_det_stop(uint32 ch_id)
{
	hook_det_flag[ch_id] = 0;
}
#endif

int hook_in(uint32 ch_id, char input)
{
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	if (hook_det_flag[ch_id] != 1)
		return 0;
#endif

	if ((input != PHONE_UNKNOWN) && (input != FXO_UNKNOWN))/* not equal to PHONE_UNKNOWN, then put in to fifo */
	{
		if ((hook_wp[ch_id]+1)%HOOK_PCM_FIFO_SIZE != hook_rp[ch_id])
		{
			PRINT_G("hook in: %d\n", input);
			hook_fifo[ch_id][(unsigned char)(hook_wp[ch_id])] = input;
			hook_wp[ch_id] = (hook_wp[ch_id]+1) % HOOK_PCM_FIFO_SIZE;
			//printk("hook_wp=%d\n", hook_wp[ch_id]);

#if defined (CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP) && defined (CONFIG_RTK_VOIP_LED)
			extern void led_state_watcher(unsigned int chid);
			switch (input)
			{
				case PHONE_OFF_HOOK:
	                                //PRINT_MSG("PHONE_OFF_HOOK(%d)\n", ch_id);
					fxs_ringing[ch_id] = 0;
					slic_hook_status[ch_id] = input;
					break;
					
				case PHONE_ON_HOOK:
	                                //PRINT_MSG("PHONE_ON_HOOK(%d)\n", ch_id);
					fxs_ringing[ch_id] = 0;
					slic_hook_status[ch_id] = input;
					break;
					
				case PHONE_FLASH_HOOK:
	                                //PRINT_MSG("PHONE_FLASH_HOOK(%d)\n", ch_id);
 	                               	slic_hook_status[ch_id] = input;
					break;	
						
				case FXO_ON_HOOK:
					//PRINT_MSG("FXO_ON_HOOK(%d)\n", ch_id);
					daa_hook_status[ch_id] = 0;
					break;	
						
				case FXO_OFF_HOOK:
					//PRINT_MSG("FXO_OFF_HOOK(%d)\n", ch_id);
					daa_hook_status[ch_id] = 1;
					break;
							
				case FXO_RING_ON:
					//PRINT_MSG("FXO_RING_ON(%d)\n", ch_id);
					//pstn_ringing = 1; //move to Slic_api.c
					break;
	
				case FXO_RING_OFF:
					//PRINT_MSG("FXO_RING_OFF(%d)\n", ch_id);
					//pstn_ringing = 0; //move to Slic_api.c
					break;
	
				case FXO_BUSY_TONE:
					//PRINT_MSG("FXO_BUSY_TONE(%d)\n", ch_id);
					break;
	
				case FXO_RING_TONE_ON:
					//PRINT_MSG("FXO_RING_TONE_ON(%d)\n", ch_id);
					break;
	
				case FXO_RING_TONE_OFF:
					//PRINT_MSG("FXO_RING_TONE_OFF(%d)\n", ch_id);
					break;
	
				case FXO_POLARITY_REVERSAL:
					//PRINT_MSG("FXO_POLARITY_REVERSAL(%d)\n", ch_id);
					break;
				
				case PHONE_STILL_ON_HOOK:
				case PHONE_STILL_OFF_HOOK:
					if (1 == Is_DAA_Channel(ch_id))
					{
						PRINT_R("Error Hook Status, ch=%d, status=%d\n", ch_id, input);
					}
					break;
				case FXO_STILL_ON_HOOK:
				case FXO_STILL_OFF_HOOK:
					if (1 != Is_DAA_Channel(ch_id))
					{
						PRINT_R("Error Hook Status, ch=%d, status=%d\n", ch_id, input);
					}
					break;
				
				default:
					PRINT_R("Unknow Hook Status, ch=%d, status=%d\n", ch_id, input);
					break;
			}
			led_state_watcher(ch_id);
#endif
	  	}
	  	else
	  	{
	  		PRINT_R("HOOK FIFO overflow,(%d)\n", ch_id);
	  	}
		//printk("k:%c ", hook_fifo[ch_id]);
	}
	return 0;
}

#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP
int hook_in_from_pkt(uint32 ch_id, char input)
{
	return hook_in( ch_id, input ); 
}
#endif

int hook_out(uint32 ch_id, char * output)
{
	if ( hook_wp[ch_id] == hook_rp[ch_id]) // FIFO empty
	{
		if (Is_DAA_Channel(ch_id) == 1)
		{
			if (prev_out[ch_id] == FXO_ON_HOOK )
				*output = FXO_STILL_ON_HOOK;
			else if (prev_out[ch_id] == FXO_FLASH_HOOK )
				*output = FXO_STILL_OFF_HOOK;
			else if (prev_out[ch_id] == FXO_OFF_HOOK )
				*output = FXO_STILL_OFF_HOOK;
			else	// error handling
			{
				*output = FXO_STILL_ON_HOOK;
				prev_out[ch_id] = FXO_ON_HOOK;
			}

		}
		else
		{
			if (prev_out[ch_id] == PHONE_ON_HOOK )
				*output = PHONE_STILL_ON_HOOK;
			else if (prev_out[ch_id] == PHONE_FLASH_HOOK )
				*output = PHONE_STILL_OFF_HOOK;
			else if (prev_out[ch_id] == PHONE_OFF_HOOK )
				*output = PHONE_STILL_OFF_HOOK;
			else	// error handling
			{
				*output = PHONE_STILL_ON_HOOK;
				prev_out[ch_id] = PHONE_ON_HOOK;
			}
		}
	}
	else
	{
		*output = hook_fifo[ch_id][(unsigned char)(hook_rp[ch_id])];
		if ((hook_fifo[ch_id][(unsigned char)(hook_rp[ch_id])] != FXO_RING_ON) &&
		    (hook_fifo[ch_id][(unsigned char)(hook_rp[ch_id])] != FXO_RING_OFF) &&
		    (hook_fifo[ch_id][(unsigned char)(hook_rp[ch_id])] != FXO_BUSY_TONE) &&
		    (hook_fifo[ch_id][(unsigned char)(hook_rp[ch_id])] != FXO_RING_TONE_ON) &&
		    (hook_fifo[ch_id][(unsigned char)(hook_rp[ch_id])] != FXO_RING_TONE_OFF) &&
		    (hook_fifo[ch_id][(unsigned char)(hook_rp[ch_id])] != FXO_POLARITY_REVERSAL) &&
		    (hook_fifo[ch_id][(unsigned char)(hook_rp[ch_id])] != FXO_CURRENT_DROP) &&
		    (hook_fifo[ch_id][(unsigned char)(hook_rp[ch_id])] != FXO_DIS_TONE) )
		{
			prev_out[ch_id] = hook_fifo[ch_id][(unsigned char)(hook_rp[ch_id])];
		}
                hook_rp[ch_id] = (hook_rp[ch_id]+1) % HOOK_PCM_FIFO_SIZE;
		//printk("hook_rp=%d\n", hook_rp[ch_id]);
	}
//	hook_fifo[ch_id] = 'Z';
	//printk("hook out: %d\n", *output);
	return 0;
}

void flush_hook_fifo(uint32 ch_id)
{
	int i;
	for (i=0; i< HOOK_PCM_FIFO_SIZE; i++)
	{
		hook_fifo[ch_id][i] = 0;
	}
	hook_wp[ch_id] = 0;
	hook_rp[ch_id] = 0;
	prev_out[ch_id] = 0;
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	extern void Init_Hook_Polling(unsigned char CH);
	Init_Hook_Polling(ch_id);
#endif
}
#endif


#define ENERGY_FIFO_SIZE 10
static char eng_fifo[MAX_VOIP_CH_NUM][ENERGY_FIFO_SIZE]={{0}};
static char eng_wp[MAX_VOIP_CH_NUM]={0},eng_rp[MAX_VOIP_CH_NUM]={0};

int energy_in(uint32 ch_id, char input)
{

	if ((eng_wp[ch_id]+1)%ENERGY_FIFO_SIZE != eng_rp[ch_id])
	{
		eng_fifo[ch_id][(unsigned char)(eng_wp[ch_id])] = input;
		eng_wp[ch_id] = (eng_wp[ch_id]+1) % ENERGY_FIFO_SIZE;
	}
	else
	{
		PRINT_R("ENERGY FIFO overflow,(%d)\n", ch_id);
	}
		//printk("k:%c ", latest_input[ch_id]);

	return 0;
}

int energy_out(uint32 ch_id)
{
	int output;

	if ( eng_wp[ch_id] == eng_rp[ch_id])
	{
		// FIFO empty
		return -1;
	}
	else
	{
		output = eng_fifo[ch_id][(unsigned char)(eng_rp[ch_id])];
                eng_rp[ch_id] = (eng_rp[ch_id]+1) % ENERGY_FIFO_SIZE;

                return output;
	}
}

void flush_energy_fifo(uint32 ch_id)
{
	int i;
	for (i=0; i< ENERGY_FIFO_SIZE; i++)
	{
		eng_fifo[ch_id][i] = 0;
	}
	eng_wp[ch_id] = 0;
	eng_rp[ch_id] = 0;
}


#ifdef CONFIG_RTK_VOIP_DECT_DSPG_SUPPORT

#define DECT_FIFO_SIZE 10
static unsigned char dect_fifo[DECT_FIFO_SIZE]={{0}};
static unsigned char dect_wp=0,dect_rp=0;

int dect_in(unsigned char input)
{

	if ((dect_wp+1)%DECT_FIFO_SIZE != dect_rp)
	{
		dect_fifo[dect_wp] = input;
		dect_wp = (dect_wp+1) % DECT_FIFO_SIZE;
	}
	else
	{
		PRINT_R("DECT FIFO overflow.\n");
	}

	return 0;
}

int dect_out(void)
{
	int output;

	if ( dect_wp == dect_rp)
	{
		// FIFO empty
		return -1;
	}
	else
	{
		output = (int)dect_fifo[dect_rp];
                dect_rp = (dect_rp+1) % DECT_FIFO_SIZE;

                return output;
	}
}

void flush_dect_fifo(void)
{
	int i;
	for (i=0; i< DECT_FIFO_SIZE; i++)
	{
		dect_fifo[i] = 0;
	}
	dect_wp = 0;
	dect_rp = 0;
}
#endif //CONFIG_RTK_VOIP_DECT_DSPG_SUPPORT


#define FAX_CHK_TIMER_RATE	20				//uint: 10ms
#define FAX_CHK_TIMEEOUT	(600/FAX_CHK_TIMER_RATE)	//unit: 10ms
static struct timer_list fax_chk_timer;
static int fax_chk_start[PCM_CH_NUM] = {[0 ... PCM_CH_NUM-1] = 0};
static int fax_chk_cnt[PCM_CH_NUM] = {[0 ... PCM_CH_NUM-1] = 0};
static int fax_chk_get_dis_rx[PCM_CH_NUM] = {[0 ... PCM_CH_NUM-1] = 0};

static void Fax_Check(unsigned long data)
{	
	int ch, i, no_del=0;

	for (ch=0; ch < PCM_CH_NUM; ch++)
	{
		if (fax_chk_start[ch] == 1)
		{
			if (fax_chk_get_dis_rx[ch] == 1)
			{
				// Get DIS_RX, drop modem event which is detected previously.
				//printk("--> del 1\n");
				fax_chk_start[ch] = 0;
				goto DEL_TIMER;
			}
			else
			{
				// Not Timeout, continous check DIS_RX.
				fax_chk_timer.expires = jiffies + FAX_CHK_TIMER_RATE;
				add_timer(&fax_chk_timer);
			}
			
			// Check Timeout, doesn't get DIS_RX, so put MODEM_LOCAL into FIFO.
			if (fax_chk_cnt[ch] >= FAX_CHK_TIMEEOUT)
			{
				fax_modem_in(ch, MODEM_LOCAL);
				fax_chk_start[ch] = 0;
				//printk("-->del 2\n");
				goto DEL_TIMER;
			}
			PRINT_MSG("%d ", fax_chk_cnt[ch]);
			fax_chk_cnt[ch]++;

		}
	}

	return;

DEL_TIMER:
	for (i=0; i < PCM_CH_NUM; i++)
	{
		// someone channel still checking, don't del timer
		if (fax_chk_start[i] != 0)
			no_del = 1;
		else
		{
			fax_chk_cnt[i] = 0;
			fax_chk_get_dis_rx[i] = 0;
			//printk("dis_rx=0, ch=%d\n", i);
		}
	}
	
	if (no_del == 1)
		return;
			
	PRINT_MSG("Delete Fax Check Timer For Polling\n");
	del_timer(&fax_chk_timer);

	return;

}


#define FAX_MODEM_PCM_FIFO_SIZE 5
//static char fax_modem_fifo[4][FAX_MODEM_PCM_FIFO_SIZE] = {0};
//static char fax_modem_wp[4]={0,0,0,0}, fax_modem_rp[4]={0,0,0,0};
static char fax_modem_fifo[PCM_CH_NUM][FAX_MODEM_PCM_FIFO_SIZE];
static char fax_modem_wp[PCM_CH_NUM], fax_modem_rp[PCM_CH_NUM];
extern uint32 fax_modem_det_mode[];  /* fax modem det mode, 0:auto-hi-speed-fax. 1:fax. 2:modem, 3: auto-low-speed-fax */

int fax_modem_in(uint32 ch_id, char input)
{

#ifdef AUDIOCODES_VOIP
	if((input == 1) || (input == 2))// 1: fax, 2: modem
#endif
	{

#ifndef AUDIOCODES_VOIP
#if 1
		if (fax_modem_det_mode[ch_id] == 0) // auto det fax/modem which support high-speed fax (33.6k)
		{
			if (input == MODEM_LOCAL_DELAY)
			{
				fax_chk_start[ch_id] = 1;
				init_timer(&fax_chk_timer);
				fax_chk_timer.expires = jiffies + FAX_CHK_TIMER_RATE;
				fax_chk_timer.function = Fax_Check;
				add_timer(&fax_chk_timer);
				PRINT_MSG("Add Fax Check Timer\n");
				return 0;
			}
			else if (input == FAX_DIS_RX)
			{
				fax_chk_get_dis_rx[ch_id] = 1;
				//printk("dis_rx = 1, ch=%d\n", ch_id);
			}
		}
#endif
#endif
                if ((fax_modem_wp[ch_id]+1)%FAX_MODEM_PCM_FIFO_SIZE != fax_modem_rp[ch_id])
		{
	  		fax_modem_fifo[ch_id][fax_modem_wp[ch_id]] = input;
                        fax_modem_wp[ch_id] = (fax_modem_wp[ch_id]+1) % FAX_MODEM_PCM_FIFO_SIZE;
	  		//printk("fax_modem_wp=%d\n", fax_modem_wp[ch_id]);
	  	}
	  	else
	  	{
	  		PRINT_R("FAX/MODEM FIFO overflow,(%d)\n", ch_id);
	  	}
		//printk("fax in: %d, ch=%d\n", input, ch_id);
	}
	return 0;
}

int fax_modem_out(uint32 ch_id)
{
	int output;

	if ( fax_modem_wp[ch_id] == fax_modem_rp[ch_id]) // FIFO empty
	{
		output = 0;
		//printk("output = %d\n", output);
	}
	else
	{
		output = fax_modem_fifo[ch_id][fax_modem_rp[ch_id]];
                fax_modem_rp[ch_id] = (fax_modem_rp[ch_id]+1) % FAX_MODEM_PCM_FIFO_SIZE;
		//printk("fax_modem_rp=%d\n", fax_modem_rp[ch_id]);
		//printk("output = %d\n", output);
	}
	return output;
}

void flush_fax_modem_fifo(uint32 ch_id)
{
	int i;
	for (i=0; i< FAX_MODEM_PCM_FIFO_SIZE; i++)
	{
		fax_modem_fifo[ch_id][i] = 0;
	}
	fax_modem_wp[ch_id] = 0;
	fax_modem_rp[ch_id] = 0;

	fax_chk_start[ch_id] = 0;
	fax_chk_cnt[ch_id] = 0;
	fax_chk_get_dis_rx[ch_id] = 0;
}


#define RFC2833_FIFO_SIZE 10


static TstRfc2833event rfc2833_event_fifo[MAX_SESS_NUM][RFC2833_FIFO_SIZE]={{{0}}};
static char rfc2833_event_wp[MAX_SESS_NUM]={0},rfc2833_event_rp[MAX_SESS_NUM]={0};

int rfc2833_event_fifo_wrtie(uint32 s_id, unsigned int event)
{

	if ( ( event < 0) || ( event > 16) )
	{
		PRINT_R("Not support event %d for RFC2833 event FIFO\n", event);
		return -1;
	}
		
	if ((rfc2833_event_wp[s_id]+1)%RFC2833_FIFO_SIZE != rfc2833_event_rp[s_id])
	{
		rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_wp[s_id])].event = event;
		rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_wp[s_id])].marker = 1;
		rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_wp[s_id])].play = 0;
		rfc2833_event_wp[s_id] = (rfc2833_event_wp[s_id]+1) % RFC2833_FIFO_SIZE;
		//PRINT_Y("W");
	}
	else
	{
		PRINT_R("RFC2833 Event FIFO overflow,(%d)\n", s_id);
	}

	return 0;
}

unsigned int Read_current_event_fifo_state(uint32 s_id, TstRfc2833event* pEvent)
{
	unsigned int ret = 0;
	
	if (rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_rp[s_id])].marker == 1)
	{
		//pEvent = &rfc2833_event_fifo[s_id][rfc2833_event_rp[s_id]];
		memcpy(pEvent, &rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_rp[s_id])], sizeof(TstRfc2833event));
		ret = 1;
	}
	else
	{
		//pEvent = NULL;
		ret = 0;
	}
	
	return ret;
}

void Update_current_event_fifo_state(uint32 s_id, RtpEventDTMFRFC2833* pEvent)
{
	rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_rp[s_id])].edge = pEvent->edge;
	rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_rp[s_id])].volume = pEvent->volume;
	rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_rp[s_id])].duration = pEvent->duration;
	//PRINT_Y("U");
	
}

void Update_current_event_play_state(uint32 s_id, unsigned int play)
{
	rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_rp[s_id])].play = play;
	//PRINT_Y("P");
	
}

int rfc2833_event_fifo_read(uint32 s_id)
{
	//int output;

	if ( rfc2833_event_wp[s_id] == rfc2833_event_rp[s_id])
	{
		// FIFO empty
		return -1;
	}
	else
	{
		rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_rp[s_id])].marker = 0;
		rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_rp[s_id])].edge = 0;
		rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_rp[s_id])].duration = 0;
		rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_rp[s_id])].event = 0;
		rfc2833_event_fifo[s_id][(unsigned char)(rfc2833_event_rp[s_id])].play = 0;
		rfc2833_event_rp[s_id] = (rfc2833_event_rp[s_id]+1) % RFC2833_FIFO_SIZE;
		return 0;
	}
}

void flush_rfc2833_event_fifo(uint32 s_id)
{
	int i;
	for (i=0; i< RFC2833_FIFO_SIZE; i++)
	{
		rfc2833_event_fifo[s_id][i].marker = 0;
		rfc2833_event_fifo[s_id][i].edge = 0;
		rfc2833_event_fifo[s_id][i].duration = 0;
		rfc2833_event_fifo[s_id][i].event = 0;
		rfc2833_event_fifo[s_id][i].play = 0;
	}
	rfc2833_event_wp[s_id] = 0;
	rfc2833_event_rp[s_id] = 0;
}


#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
static unsigned long timeout;
static TstDtmfEvent stDtmfEvent[PCM_CH_NUM];
static TstHookEvent stHookEvent[PCM_CH_NUM];
static TstFaxModemEvent stFaxModemEvent[PCM_CH_NUM];
static TstFaxDisTxEvent stFaxFaxDisTxEvent[PCM_CH_NUM];
static TstFaxDisRxEvent stFaxFaxDisRxEvent[PCM_CH_NUM];
static struct timer_list event_tx_timer;

void clean_event_cheak_ack(unsigned short category, unsigned short rev_seq)
{
	int chid=0;
	
	for (chid=0; chid < voip_ch_num; chid++)
	{
		if (category == VOIP_MGR_DTMF_INTO_FIFO)
		{
			if ((stDtmfEvent[chid].check_ack == 1) && (stDtmfEvent[chid].seq == rev_seq))
			{
				stDtmfEvent[chid].check_ack = 0;
				//PRINT_G("dtmf: ack\n");
			}
		}
		else if (category == VOIP_MGR_HOOK_INTO_FIFO)
		{
			if ((stHookEvent[chid].check_ack == 1) && (stHookEvent[chid].seq == rev_seq))
			{
				stHookEvent[chid].check_ack = 0;
				//PRINT_G("hook: ack\n");
			}
		}
		else if (category == VOIP_MGR_FAX_MODEM_PASS_CFG)
		{
			if ((stFaxModemEvent[chid].check_ack == 1) && (stFaxModemEvent[chid].seq == rev_seq))
			{
				stFaxModemEvent[chid].check_ack = 0;
				//PRINT_G("FAX: ack\n");
			}
		}
		else if (category == VOIP_MGR_FAX_DIS_TX_DETECT)
		{
			if ((stFaxFaxDisTxEvent[chid].check_ack == 1) && (stFaxFaxDisTxEvent[chid].seq == rev_seq))
			{
				stFaxFaxDisTxEvent[chid].check_ack = 0;
				//PRINT_G("FAX DIS TX: ack\n");
			}
		}
		else if (category == VOIP_MGR_FAX_DIS_RX_DETECT)
		{
			if ((stFaxFaxDisRxEvent[chid].check_ack == 1) && (stFaxFaxDisRxEvent[chid].seq == rev_seq))
			{
				stFaxFaxDisRxEvent[chid].check_ack = 0;
				//PRINT_G("FAX DIS RX: ack\n");
			}
		}
	}
	
}

void event_tx(void)
{
#if 1
	int chid=0;
	char digit, status;
	TstTxPktCtrl pktCtrl;
	extern char CED_routine(int chid);
	extern int voip_dsp_L2_pkt_tx(unsigned short category, unsigned char pkt_type, unsigned char* pdata, unsigned short data_len, TstTxPktCtrl* txCtrl);

	pktCtrl.dsp_id = Get_Ethernet_DSP_ID();
	
	for (chid=0; chid < voip_ch_num; chid++)
	{
		/* DTMF Event Tx */
		
		if (stDtmfEvent[chid].check_ack == 0)
		{
			dtmf_out(chid, &digit);
			if (digit != 'Z')
			{
				stDtmfEvent[chid].chid = chid;
				stDtmfEvent[chid].digit = digit;
				stDtmfEvent[chid].check_ack = 1;
		
				pktCtrl.seq_no = -1;
				pktCtrl.resend_flag = 0;
				stDtmfEvent[chid].seq = voip_dsp_L2_pkt_tx(VOIP_MGR_DTMF_INTO_FIFO, ETH_EVENT_PKT, &stDtmfEvent[chid], sizeof(TstDtmfEvent), &pktCtrl);
				//PRINT_Y("DTMF send(%d, %c)\n", chid, digit);
			}
		}
		else
		{
			/* Resend Event */
			pktCtrl.seq_no = 0;
			pktCtrl.resend_flag = 1;
			voip_dsp_L2_pkt_tx(VOIP_MGR_DTMF_INTO_FIFO, ETH_EVENT_PKT, &stDtmfEvent[chid], sizeof(TstDtmfEvent), &pktCtrl);
			//PRINT_Y("DTMF re-send(%c)\n", digit);
		}
		
		/* Hook Event Tx */
		if (hook_det_flag[chid] == 1)
		{
			if (stHookEvent[chid].check_ack == 0)
			{
				hook_out(chid, &status);
				if (status != PHONE_STILL_OFF_HOOK &&
					status != PHONE_STILL_ON_HOOK &&
					status != FXO_STILL_OFF_HOOK &&
					status != FXO_STILL_ON_HOOK )
				{
					//PRINT_R("HK st=%d send\n", status);
					stHookEvent[chid].chid = chid;
					stHookEvent[chid].hook = status;
					stHookEvent[chid].check_ack = 1;

					pktCtrl.seq_no = -1;
					pktCtrl.resend_flag = 0;
					stHookEvent[chid].seq = voip_dsp_L2_pkt_tx(VOIP_MGR_HOOK_INTO_FIFO, ETH_EVENT_PKT, (unsigned char*)&stHookEvent[chid], sizeof(TstHookEvent), &pktCtrl);
	
				}
			}
			else
			{
				/* Resend Event */

				pktCtrl.seq_no = 0;
				pktCtrl.resend_flag = 1;
				voip_dsp_L2_pkt_tx(VOIP_MGR_HOOK_INTO_FIFO, ETH_EVENT_PKT, (unsigned char*)&stHookEvent[chid], sizeof(TstHookEvent), &pktCtrl);
				//PRINT_R("HK st=%d re-send\n", status);
			}
		}
		
		/* Fax, Modem Event Tx */
		/* 1: fax, 2: local-modem, 3: remote-modem */
		static prev_fax_modem[PCM_CH_NUM] = {0};
		
		if (stFaxModemEvent[chid].check_ack == 0)
		{
			//stFaxModemEvent[chid].fax_modem = CED_routine(chid);
			switch (fax_modem_out(chid))
			{
				case FAX_CED:
				case FAX_DIS_RX:
					stFaxModemEvent[chid].fax_modem = 1;
					//printk("fax_modem_out: 1\n");
					break;
				case MODEM_LOCAL:
					stFaxModemEvent[chid].fax_modem = 2;
					//printk("fax_modem_out: 2\n");
					break;
				case MODEM_REMOTE:
					stFaxModemEvent[chid].fax_modem = 3;
					//printk("fax_modem_out: 3\n");
				default: 
					//stVoipCfg.enable = 0;
					stFaxModemEvent[chid].fax_modem = 0;
					//printk("fax_modem_out: 0\n");
					break;
			}
			
			if (fax_modem_det_mode[chid] == 1)
			{
				if (stFaxModemEvent[chid].fax_modem > 0)
					stFaxModemEvent[chid].fax_modem = 1;
			}
			else if (fax_modem_det_mode[chid] == 2)
			{
				if (stFaxModemEvent[chid].fax_modem == 1)
					stFaxModemEvent[chid].fax_modem = 0;
			}
			
			if ((stFaxModemEvent[chid].fax_modem !=0 ) && 
				(prev_fax_modem[chid] != stFaxModemEvent[chid].fax_modem))
			{
				PRINT_Y("FAX event send, ch=%d\n", chid);
				stFaxModemEvent[chid].chid = chid;
				stFaxModemEvent[chid].check_ack = 1;
	
				pktCtrl.seq_no = -1;
				pktCtrl.resend_flag = 0;				
				stFaxModemEvent[chid].seq = voip_dsp_L2_pkt_tx(VOIP_MGR_FAX_MODEM_PASS_CFG, ETH_EVENT_PKT, (unsigned char*)&stFaxModemEvent[chid], sizeof(TstFaxModemEvent), &pktCtrl);
				
			}
			prev_fax_modem[chid] = stFaxModemEvent[chid].fax_modem;		

		}
		else
		{
			/* Resend Event */
			pktCtrl.seq_no = 0;
			pktCtrl.resend_flag = 1;
			voip_dsp_L2_pkt_tx(VOIP_MGR_FAX_MODEM_PASS_CFG, ETH_EVENT_PKT, (unsigned char*)&stFaxModemEvent[chid], sizeof(TstFaxModemEvent), &pktCtrl);
			PRINT_Y("FAX re-send, ch=%d\n", chid);
		}
		
		
		/* Fax DIS Tx Event */
		extern int fax_v21_dis_tx_get(unsigned int chid);
		
		if (stFaxFaxDisTxEvent[chid].check_ack == 0)
		{
			stFaxFaxDisTxEvent[chid].fax_dis_tx = fax_v21_dis_tx_get(chid);
			
			if (stFaxFaxDisTxEvent[chid].fax_dis_tx == 1)
			{
				PRINT_Y("FAX DIS TX send, ch=%d\n", chid);
				stFaxFaxDisTxEvent[chid].chid = chid;
				stFaxFaxDisTxEvent[chid].check_ack = 1;
	
				pktCtrl.seq_no = -1;
				pktCtrl.resend_flag = 0;				
				stFaxFaxDisTxEvent[chid].seq = voip_dsp_L2_pkt_tx(VOIP_MGR_FAX_DIS_TX_DETECT, ETH_EVENT_PKT, (unsigned char*)&stFaxFaxDisTxEvent[chid], sizeof(TstFaxDisTxEvent), &pktCtrl);
				
			}
		}
		else
		{
			/* Resend Event */
			pktCtrl.seq_no = 0;
			pktCtrl.resend_flag = 1;
			voip_dsp_L2_pkt_tx(VOIP_MGR_FAX_DIS_TX_DETECT, ETH_EVENT_PKT, (unsigned char*)&stFaxFaxDisTxEvent[chid], sizeof(TstFaxDisTxEvent), &pktCtrl);
			PRINT_Y("FAX DIS TX re-send, ch=%d\n", chid);
		}
		
		/* Fax DIS Rx Event */
		extern int fax_v21_dis_rx_get(unsigned int chid);
		
		if (stFaxFaxDisRxEvent[chid].check_ack == 0)
		{
			stFaxFaxDisRxEvent[chid].fax_dis_rx = fax_v21_dis_rx_get(chid);
			
			if (stFaxFaxDisRxEvent[chid].fax_dis_rx == 1)
			{
				PRINT_Y("FAX DIS RX send, ch=%d\n", chid);
				stFaxFaxDisRxEvent[chid].chid = chid;
				stFaxFaxDisRxEvent[chid].check_ack = 1;
	
				pktCtrl.seq_no = -1;
				pktCtrl.resend_flag = 0;				
				stFaxFaxDisRxEvent[chid].seq = voip_dsp_L2_pkt_tx(VOIP_MGR_FAX_DIS_RX_DETECT, ETH_EVENT_PKT, (unsigned char*)&stFaxFaxDisRxEvent[chid], sizeof(TstFaxDisRxEvent), &pktCtrl);
				
			}
		}
		else
		{
			/* Resend Event */
			pktCtrl.seq_no = 0;
			pktCtrl.resend_flag = 1;
			voip_dsp_L2_pkt_tx(VOIP_MGR_FAX_DIS_RX_DETECT, ETH_EVENT_PKT, (unsigned char*)&stFaxFaxDisRxEvent[chid], sizeof(TstFaxDisRxEvent), &pktCtrl);
			PRINT_Y("FAX DIS RX re-send, ch=%d\n", chid);
		}
		
	}// chid for loop
	
	event_tx_timer.expires = jiffies + 10;
      	add_timer(&event_tx_timer);
	
#else
	if( event_need_ack())
	{
		if (stDtmfEvent.check_ack == 1)
		{
			TstTxPktCtrl pktCtrl;
			pktCtrl.seq_no = 0;
	      		pktCtrl.resend_flag = 1;
	      		voip_dsp_L2_pkt_tx(VOIP_MGR_DTMF_INTO_FIFO, ETH_EVENT_PKT, &stDtmfEvent, sizeof(TstDtmfEvent), &pktCtrl);
	      		PRINT_R("%d ", VOIP_MGR_DTMF_INTO_FIFO-2116);
      		}
      		else if (stHookEvent.check_ack == 1)
      		{
      			TstTxPktCtrl pktCtrl;
			pktCtrl.seq_no = 0;
	      		pktCtrl.resend_flag = 1;
	      		voip_dsp_L2_pkt_tx(VOIP_MGR_HOOK_INTO_FIFO, ETH_EVENT_PKT, &stHookEvent, sizeof(TstHookEvent), &pktCtrl);
	      		PRINT_R("%d ", VOIP_MGR_HOOK_INTO_FIFO-2116);
      		}
      		
      		event_tx_timer.expires = jiffies + 10;
      		add_timer(&event_tx_timer);
	}
	else
	{
		if (stDtmfEvent.check_ack == 1)
			stDtmfEvent.check_ack = 0;
		else if (stHookEvent.check_ack == 1)
			stHookEvent.check_ack = 0;
		
		Del_check_ack_timer();
	}
#endif	
}

void Add_event_tx_timer(void)
{
	init_timer(&event_tx_timer);
	event_tx_timer.expires = jiffies + 10;
	event_tx_timer.function = event_tx;
	add_timer(&event_tx_timer);
	PRINT_G("Add Timer for Ack Check\n");
}

void Del_check_ack_timer(void)
{
	del_timer(&event_tx_timer);
	//PRINT_G("Del Timer For check ack\n");
}

#endif //ONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP

/****************************************************************/
#ifndef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
//#if VOIP_CH_NUM > SLIC_CH_NUM
//int pcm_ch_for_DAA[VOIP_CH_NUM] = {[0 ... DECT_CH_NUM+SLIC_CH_NUM-1] = 0, [DECT_CH_NUM+SLIC_CH_NUM ... VOIP_CH_NUM-1] = 1};
//#else
//int pcm_ch_for_DAA[VOIP_CH_NUM] = {[0 ... DECT_CH_NUM+SLIC_CH_NUM-1] = 0};
//#endif

int pcm_ch_for_DAA[ CON_CH_NUM ];

static void pcm_ch_attribute_init(void)
{
	int cch;
	
	for( cch = 0; cch < CON_CH_NUM; cch ++ ) {
		pcm_ch_for_DAA[ cch ] = 
				( get_snd_type_cch( cch ) == SND_TYPE_DAA ? 1 : 0 );
	}
	
#if 0
//	for (ch = 0; ch < SLIC_CH_NUM + DECT_CH_NUM; ch++)
//	{
//		pcm_ch_for_DAA[ch] = 0;	// this pcm channel is for SLIC(FXS).
//		//printk("pcm_ch_for_DAA[%d] = %d\n", ch, pcm_ch_for_DAA[ch]);
//	}
//
//	for (/*ch = SLIC_CH_NUM + DECT_CH_NUM*/; ch < VOIP_CH_NUM; ch++)
//	{
//		pcm_ch_for_DAA[ch] = 1;	// this pcm channel is for DAA(FXO).
//		//printk("pcm_ch_for_DAA[%d] = %d\n", ch, pcm_ch_for_DAA[ch]);
//	}
#endif
}

int Is_DAA_Channel(int chid)
{
	if( chid >= CON_CH_NUM )
		return 0;
		
	return pcm_ch_for_DAA[chid];
}
#endif

/****************************************************************/
static int __init rtk_voip_mgr_init_module(void)
{
  printk("=============RTK VOIP SUITE=============\n");
#ifdef CONFIG_VOIP_SDK
  printk("SDK VoIP Version: %s \n", VOIP_VERSION);
#else
  printk("RTK VoIP Version: %s \n", VOIP_VERSION);
#endif
  printk("INITIAL VOIP MANAGER PROGRAM\n");
  //printk("===========================================\n");

#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
	extern struct nf_sockopt_ops voip_mgr_sockopts;
  if(nf_register_sockopt(&voip_mgr_sockopts))
   {
		PRINT_R(" voip_mgr cannot register sockopt \n");
    return -1;
   }
#endif
#else
	extern struct nf_sockopt_ops voip_mgr_sockopts;
	if(nf_register_sockopt(&voip_mgr_sockopts))
	{
		PRINT_R(" voip_mgr cannot register sockopt \n");
		return -1;
	}
#endif
	
   extern uint64 gVoipFeature;
   extern int rtk_voip_dsp_init(void);
   gVoipFeature = RTK_VOIP_FEATURE;
   BOOT_MSG("Get RTK VoIP Feature.\n");
#ifndef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
	//extern void pcm_ch_attribute_init(void);
	pcm_ch_attribute_init();
#endif
#ifdef CONFIG_RTK_VOIP_MODULE
	pcmctrl_init();
	rtk_trap_init_module();
#endif
#if ! defined (AUDIOCODES_VOIP)
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
  rtk_voip_dsp_init();
  dsp_init_first = 1;
#endif
#else
	rtk_voip_dsp_init();
	dsp_init_first = 1;
#endif
#endif

#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	Add_event_tx_timer(); // only for DSP
	PRINT_G("This is DSP-ID %d\n", Get_Ethernet_DSP_ID());
#endif
#endif

  return 0;
}

/****************************************************************/
static void __exit rtk_voip_mgr_cleanup_module(void)
{
  printk("=============RTK VOIP SUITE============\n");
  printk("Remove VOIP MANAGER PROGRAM\n");
  printk("===========================================\n");
	
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
	extern struct nf_sockopt_ops voip_mgr_sockopts;
	nf_unregister_sockopt(&voip_mgr_sockopts);
#endif
#else
	extern struct nf_sockopt_ops voip_mgr_sockopts;
	nf_unregister_sockopt(&voip_mgr_sockopts);
#endif
	
#if ! defined (AUDIOCODES_VOIP)
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	extern void rtk_voip_dsp_exit(void);
	rtk_voip_dsp_exit();
#endif
#endif

#ifdef CONFIG_RTK_VOIP_MODULE
	rtk_trap_cleanup_module();
	pcmctrl_cleanup();
#endif
}

/****************************************************************/
voip_initcall(rtk_voip_mgr_init_module);
voip_exitcall(rtk_voip_mgr_cleanup_module);

