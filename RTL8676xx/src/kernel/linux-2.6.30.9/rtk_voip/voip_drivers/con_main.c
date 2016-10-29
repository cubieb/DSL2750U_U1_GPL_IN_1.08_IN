#include "rtk_voip.h"
#include "voip_types.h"
#include "voip_init.h"
#include "voip_proc.h"
#include "voip_debug.h"
#include "con_register.h"
#include "con_bind.h"
#include "con_bus_fifo.h"
#include "con_bus_handler.h"
#include "con_event.h"
#include "con_lec_buf.h"

// --------------------------------------------------------
// global variables for reference 
// --------------------------------------------------------

int slic_ch_num = SLIC_CH_NUM;		//nunmber of slic
int voip_ch_num = VOIP_CH_NUM;
int dsp_ch_num = DSP_CH_NUM;
int sess_num = SESS_NUM;
int pcm_period = PCM_PERIOD;

// --------------------------------------------------------
// channel mapping basic ops & functions 
// --------------------------------------------------------

__attribute__ ((section(".con_desc_bss")))
static voip_con_t voip_con[ CON_CH_NUM ];

static void __init init_polling_timer( unsigned long data )
{
	extern void Init_Event_Polling_Use_Timer( unsigned long data );
	
	//return ;
	
//#if ! defined (AUDIOCODES_VOIP)
#ifdef EVENT_POLLING_TIMER
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP
#ifdef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_DSP
	Init_Event_Polling_Use_Timer( data );
#endif
#else
	Init_Event_Polling_Use_Timer( data );
#endif
#endif
//#endif
}

static int con_enable( voip_con_t *this, int enable )
{
	voip_snd_t * const p_snd = this ->snd_ptr;
	voip_bus_t * const p_bus = this ->bus_ptr;
	voip_bus_t * const p_bus2 = this ->bus2_ptr;
	voip_dsp_t * const p_dsp = this ->dsp_ptr;
	
	//printk( "this ->cch=%d, this ->enabled=%d\n", this ->cch, this ->enabled );
	//printk( "enable=%d, this ->share_var=%d\n", enable, this ->share_var );
	//printk( "p_snd=%p, p_bus=%p, p_dsp=%p\n", p_snd, p_bus, p_dsp );
	
	if( ( this ->enabled && enable ) ||
		( !this ->enabled && !enable ) )
	{
		return -1;
	}

	// ----------------------------------------------------
	// wideband support check 
	if( enable == 2 ) {
		if( this ->band_mode_bind & 
			( BAND_MODE_16K | BAND_MODE_8K_PLUS ) )
		{
		} else {
			DBG_ERROR( "No wideband mode support on cch %d\n", this ->cch );
			return -1;
		}
	} else if( enable == 1 ) {
		if( this ->band_mode_bind & BAND_MODE_8K )
		{
		} else {
			DBG_ERROR( "No narrowband mode support on cch %d\n", this ->cch );
			return -1;
		}
	}
	
	// ----------------------------------------------------
	// share bus 
	if( this ->share_bind & CON_SHARE_BUS ) {
		if( p_bus ->enabled && enable ) {
			DBG_ERROR( "Shared bus (%d) still enabled.\n", p_bus ->bch );
			p_bus ->bus_ops ->enable( p_bus, 0 );
		}
		
		if( enable ) {
			p_bus ->bus_ops ->set_timeslot( p_bus, p_snd ->TS1, p_snd ->TS2 );
			p_bus ->con_ptr = this;
		}
	}
	
	// ----------------------------------------------------
	// Normal process 
	
	// assign 'enabled'
	this ->enabled = p_snd ->enabled = p_bus ->enabled = p_dsp ->enabled 
		= enable;
	p_bus ->reseting = 0;
	if( ( enable == 2 || enable == 0 ) && p_bus2 ) {
		p_bus2 ->enabled = enable;
		p_bus2 ->reseting = 0;
	}
	this ->band_factor_var = ( enable == 2 ? 2 : 1 );		// narrowband: 1, wideband: 2 
	this ->band_sample_var = ( enable == 2 ? 160 : 80 );	// narrowband: 80, wideband: 160 
	
	// call corresponding snd/bus/dsp 
	// enable / disable call flows are different to be more safe 
	// (it may not need it)
	if( enable ) {
		this ->con_ops ->bus_fifo_flush( this, 1, 1 );
		this ->con_ops ->bus_fifo_set_tx_mute( this, 0 );
		this ->con_ops ->bus_fifo_set_rx_mute( this, 0 );
		this ->con_ops ->lec_buf_flush( this );
		
		p_snd ->snd_ops ->enable( p_snd, enable );
		p_dsp ->dsp_ops ->enable( p_dsp, enable ); 
		p_bus ->bus_ops ->enable( p_bus, enable ); 
	} else {
		p_bus ->bus_ops ->enable( p_bus, enable ); 
		p_snd ->snd_ops ->enable( p_snd, enable );
		p_dsp ->dsp_ops ->enable( p_dsp, enable ); 
	}
	
	return 0;
}

static int con_suspend( voip_con_t *this )
{
	// save 'enabled' to 'suspend', and call enable(0)
	this ->suspend = this ->enabled;
	
	return this ->con_ops ->enable( this, 0 );
}

static int con_resume( voip_con_t *this )
{
	// use 'suspend' value to enable, so it can keep narrow/wideband setting 
	const uint32 enable = this ->suspend;
	
	if( enable == 0 ) {
		DBG_ERROR( "No suspend (%d)?\n", this ->cch );
		return -1;	//
	}
	
	this ->suspend = 0;	// clean it 
	
	return this ->con_ops ->enable( this, enable );
}

// --------------------------------------------------------
// channel mapping architecture 
// --------------------------------------------------------

static void con_init( voip_con_t *this )
{
	this ->con_ops ->bus_fifo_flush( this, 1, 1 );
	
	this ->con_ops ->lec_buf_init_sync_point( this );
}

__attribute__ ((section(".con_desc_data")))
static const con_ops_t voip_con_ops = {
	// common operation 
	.enable = con_enable,
	.init = con_init,
	
	.suspend = con_suspend,
	.resume = con_resume,
	
	// bus fifo - handle isr 
	.isr_bus_read_tx = isr_bus_read_tx,
	.isr_bus_write_rx_TH = isr_bus_write_rx_TH,
	.isr_bus_write_rx_BH = isr_bus_write_rx_BH,
	.isr_bus_write_rx_lo = isr_bus_write_rx_lo,
		
	// bus fifo - dsp call these 
	.dsp_write_bus_tx_get_addr = dsp_write_bus_tx_get_addr,
	.dsp_write_bus_tx_done = dsp_write_bus_tx_done,
	.dsp_read_bus_rx_get_addr = dsp_read_bus_rx_get_addr,
	.dsp_read_bus_rx_peek_addr = dsp_read_bus_rx_peek_addr,
	.dsp_read_bus_rx_done = dsp_read_bus_rx_done,
	.dsp_read_bus_rx_get_fifo_size = dsp_read_bus_rx_get_fifo_size,
	
	// bus fifo - common 
	.bus_fifo_set_tx_mute = bus_fifo_set_tx_mute,
	.bus_fifo_set_rx_mute = bus_fifo_set_rx_mute,
	.bus_fifo_clean_tx = bus_fifo_clean_tx,
	.bus_fifo_clean_rx = bus_fifo_clean_rx,
	.bus_fifo_flush = bus_fifo_flush,
	
	// lec buf - common 
	.lec_buf_init_sync_point = lec_buf_init_sync_point,
	.lec_buf_flush = lec_buf_flush,
	
	// lec buf - handle isr 
	.isr_lec_buf_inc_windex = isr_lec_buf_inc_windex,
	.isr_lec_buf_tx_process = isr_lec_buf_tx_process,
	.isr_lec_buf_tx_process_post = isr_lec_buf_tx_process_post,
	.isr_lec_buf_inc_rindex = isr_lec_buf_inc_rindex,
	.isr_lec_buf_get_ref_addr = isr_lec_buf_get_ref_addr,
	.isr_lec_buf_sync_p = isr_lec_buf_sync_p,
};


// retrieve con_ptr, only *mux* can use this!! 
voip_con_t *get_voip_con_ptr( uint32 cch )
{
	return &voip_con[ cch ];
}

// open for global access 
const voip_con_t *get_const_con_ptr( uint32 cch )
{
	return &voip_con[ cch ];
}

// only con_sup.c can use 
voip_con_t *get_voip_cons_ptr( void )
{
	return voip_con;
}

static int __init voip_con_main_init( void )
{
	extern void FXS_FXO_functionality_init_all( const voip_con_t *p_con );
	
	// Now, all snd, bus and dsp are registered,  
	// so we can start to bind them. 
	int i;
	
	for( i = 0; i < CON_CH_NUM; i ++ ) {
		voip_con[ i ].cch = i;
		voip_con[ i ].con_ops = &voip_con_ops;
	}

#if 0	// hack to loopback cch 0 and 1
	voip_con[ 0 ].con_lo_ptr = &voip_con[ 1 ];
	voip_con[ 1 ].con_lo_ptr = &voip_con[ 0 ];
#endif

#if CON_CH_NUM == 16
	i = auto_bind_con_with_policy( voip_con, CON_CH_NUM, BIND_POLICY_FXS_DAA_SHARE );
#else
#if defined( CONFIG_RTK_VOIP_DRIVERS_IP_PHONE )
	i = auto_bind_con_with_policy( voip_con, CON_CH_NUM, BIND_POLICY_AC_ONLY );
#elif defined( CONFIG_RTK_VOIP_WIDEBAND_APP )
	i = auto_bind_con_with_policy( voip_con, CON_CH_NUM, BIND_POLICY_DECT_FXS_DAA_FULLY );
#else
	i = auto_bind_con_with_policy( voip_con, CON_CH_NUM, BIND_POLICY_DECT_FXS_DAA );
#endif
#endif
	
	printk( "VoIP control bind %d (%d) instances successfully.\n", i, CON_CH_NUM );
	
	// --------------------------------------------------------
	// binding ok!!! start to init cch variables 
	
	// init bus handler tasklet 
	bus_handler_tasklet_init();
	
	// each sub-system init 
	for( i = 0; i < CON_CH_NUM; i ++ ) {
		voip_con_t * const p_con = &voip_con[ i ];
		p_con ->con_ops ->init( p_con );
	}
	
	// init FXS / FXS variables for polling 
	FXS_FXO_functionality_init_all( &voip_con[ 0 ] );
	
	
	return 0;
}

voip_initcall_con( voip_con_main_init );

static int __init voip_con_init_polling_timer( void )
{
	// init polling timer 
	init_polling_timer( ( unsigned long )( void * )&voip_con[ 0 ] );	
	
	return 0;
}

voip_initcall_sync( voip_con_init_polling_timer );

// --------------------------------------------------------
// proc 
// --------------------------------------------------------
static int voip_con_main_read_proc( char *buf, char **start, off_t off, int count, int *eof, void *data )
{
#define S_VAL( ptr, fld )	( ptr ? ptr ->fld : -1 )
#define S_STR( ptr, fld )	( ptr && ptr ->fld ? ptr ->fld : "(null)" )

	int n = 0;
	const voip_snd_t *p_snd;
	const voip_bus_t *p_bus;
	const voip_bus_t *p_bus2;
	const voip_dsp_t *p_dsp;
	const voip_con_t *p_con;
	int i;
	
	if( off ) {	/* In our case, we write out all data at once. */
		*eof = 1;
		return 0;
	}
	
	n += sprintf( buf + n, "cch E B sch T name    bch name    bch2 name    dch name    \n" );
	n += sprintf( buf + n, "--- - - ------------- ----------- ------------ ------------\n" );
	
	for( i = 0; i < CON_CH_NUM; i ++ ) {
		
		p_con = &voip_con[ i ];
		p_snd = p_con ->snd_ptr;
		p_bus = p_con ->bus_ptr;
		p_bus2 = p_con ->bus2_ptr;
		p_dsp = p_con ->dsp_ptr;
		
		n += sprintf( buf + n, "%3d %d %X %3d %d %-7s %3d %-7s %4d %-7s %3d %-7s\n",
				S_VAL( p_con, cch ),
				S_VAL( p_con, enabled ),
				S_VAL( p_con, band_mode_bind ),
				S_VAL( p_snd, sch ), S_VAL( p_snd, snd_type ), S_STR( p_snd, name ),
				S_VAL( p_bus, bch ), S_STR( p_bus, name ),
				S_VAL( p_bus2, bch ), S_STR( p_bus2, name ),
				S_VAL( p_dsp, dch ), S_STR( p_dsp, name )
		 );
	}
	
	*eof = 1;
	return n;
#undef S_VAL
#undef S_STR
}

static int __init voip_con_main_proc_init( void )
{
	create_proc_read_entry( PROC_VOIP_DIR "/" PROC_VOIP_CH_MAPS_DIR "/bind", 0, NULL, 
			voip_con_main_read_proc, NULL );
	
	return 0;
}

static void __exit voip_con_main_proc_exit( void )
{
	remove_proc_entry( PROC_VOIP_DIR "/" PROC_VOIP_CH_MAPS_DIR "/bind", NULL );
}

voip_initcall_proc( voip_con_main_proc_init );
voip_exitcall( voip_con_main_proc_exit );


