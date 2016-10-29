#include "rtk_voip.h"
#include "voip_types.h"
#include "con_register.h"
#include "con_mux.h"

// retrieve con_ptr, only *mux* can use this!! 
extern voip_con_t *get_voip_con_ptr( uint32 cch );

uint32 dch_from_cch( uint32 cch )
{
	voip_con_t * const p_con = get_voip_con_ptr( cch );
	
	return ( p_con ->dsp_ptr ? p_con ->dsp_ptr ->dch : -1 );
}

#if 0
void bus_fifo_set_tx_mute_cch( uint32 cch, int enable )
{
	voip_con_t * const p_con = get_voip_con_ptr( cch );
	
	p_con ->con_ops ->bus_fifo_set_tx_mute( p_con, enable );
}

void bus_fifo_set_rx_mute_cch( uint32 cch, int enable )
{
	voip_con_t * const p_con = get_voip_con_ptr( cch );
	
	p_con ->con_ops ->bus_fifo_set_rx_mute( p_con, enable );
}

void bus_fifo_clean_tx_cch( uint32 cch )
{
	voip_con_t * const p_con = get_voip_con_ptr( cch );
	
	p_con ->con_ops ->bus_fifo_clean_tx( p_con );
}

void bus_fifo_clean_rx_cch( uint32 cch )
{
	voip_con_t * const p_con = get_voip_con_ptr( cch );
	
	p_con ->con_ops ->bus_fifo_clean_rx( p_con );
}
#endif

int con_enable_cch( uint32 cch, int enable )
{
	voip_con_t * const p_con = get_voip_con_ptr( cch );
	
	return p_con ->con_ops ->enable( p_con, enable );
}

void SLIC_CPC_Gen_cch( uint32 cch, unsigned int time_in_ms_of_cpc_signal )
{
	extern void SLIC_CPC_Gen_con( const voip_con_t *p_con, unsigned int time_in_ms_of_cpc_signal );
	voip_con_t * const p_con = get_voip_con_ptr( cch );
	
	return SLIC_CPC_Gen_con( p_con, time_in_ms_of_cpc_signal );
}

snd_type_t get_snd_type_cch( uint32 cch )
{
	voip_con_t * const p_con = get_voip_con_ptr( cch );
	voip_snd_t * p_snd;
	
	if( cch >= CON_CH_NUM )
		return SND_TYPE_NONE;
	
	p_snd = p_con ->snd_ptr;
	
	if( !p_snd )
		return SND_TYPE_NONE;
	
	return p_snd ->snd_type;
}

// pluse dail 
#ifdef PULSE_DIAL_GEN
void DAA_PulseGenProcess_msec(void)
{
	extern void DAA_PulseGenProcess_msec_con( const voip_con_t * const p_con );
	uint32 cch;
	
	for( cch = 0; cch < CON_CH_NUM; cch ++ ) {
		DAA_PulseGenProcess_msec_con( get_voip_con_ptr( cch ) );
	}
}

void DAA_PulseGenKill_cch(unsigned int cch)
{
	extern void DAA_PulseGenKill_con( voip_con_t * p_con );
	
	voip_con_t * const p_con = get_voip_con_ptr( cch );
	
	DAA_PulseGenKill_con( p_con );	
}

int pulse_dial_in_dch(uint32 dch, char input)
{
	extern int pulse_dial_in_cch(uint32 ch_id, char input);
	const uint32 cch = dch_from_cch( dch );
	
	return pulse_dial_in_cch( cch, input );
}
#endif /* PULSE_DIAL_GEN */

