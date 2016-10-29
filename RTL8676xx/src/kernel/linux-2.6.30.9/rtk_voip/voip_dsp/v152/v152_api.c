#include <linux/string.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include "rtk_voip.h"
#include "voip_types.h"
#include "voip_control.h"
#include "voip_params.h"
#include "voip_debug.h"
#include "voip_init.h"

#include "dsp_main.h"	// V152_SwitchCodecNow()
#include "RtpTransmitter.h"

#include "v152_api.h"
#include "v152_priv.h"

v152_vars_t v152_vars;

// check if bit number is greater then session number 
CT_ASSERT( sizeof( v152_vars.bitsEnable ) * 8 >= SESS_NUM );
CT_ASSERT( sizeof( v152_vars.bitsSwitchCodec ) * 8 >= SESS_NUM );

CT_ASSERT( REASON_RTP_VBD != 0 );	// If assert is met, change REASON_RTP_VBD number to be non-zero. 

int V152_Initialize( void )
{
	memset( &v152_vars, 0, sizeof( v152_vars ) );
	
	return 0;
}

void V152_InitializeSession( int sid, int enable )
{
	if( sid >= SESS_NUM )
		return;
		
	memset( &v152_vars.session[ sid ], 0, sizeof( v152_vars.session[ 0 ] ) );
	
	//CT_ASSERT( REASON_RTP_VBD != 0 ); 	// If assert is met, uncomment below code or change REASON_RTP_VBD number. 
	//v152_vars.session[ sid ].prevRtp = REASON_RTP_AUDIO;
	
	v152_vars.bitsSwitchCodec &= ~( 1 << sid );
	
	if( enable )
		v152_vars.bitsEnable |= ( 1 << sid );
	else
		v152_vars.bitsEnable &= ~( 1 << sid );
}

int V152_StateTransition( int sid, v152reason reason )
{
	v152_session_vars_t *pvar = &v152_vars.session[ sid ];
	int ret = 0;
	
	if( sid >= SESS_NUM )
		return 0;
	
	// check enable
	if( !( v152_vars.bitsEnable & ( 1 << sid ) ) )
		return 0;
	
	// save & check rtp reason  
	switch( reason ) {
	case REASON_RTP_VBD:
		if( pvar ->prevRtp == REASON_RTP_VBD )
			return 0;
		break;
		
	case REASON_RTP_AUDIO:
		if( pvar ->prevRtp != REASON_RTP_VBD )	// don't check REASON_RTP_AUDIO, because inital value is 0 
			return 0;
		break;
		
	default:
		goto label_save_and_check_rtp_reason_done;
		break;
	}

	pvar ->prevRtp = reason;

label_save_and_check_rtp_reason_done:
	
	// check to do state transition 
	switch( pvar ->state ) {
	case ST_AUDIO:
		if( reason <= REASON_SEPARATOR ) {
			pvar ->state = ST_VBD;
			pvar ->reasonVBD = ( pvar ->reasonVBD << 8 ) | ( reason & 0xFF );
			v152_vars.bitsSwitchCodec |= ( 1 << sid );
			ret = 1;
		}
		break;
		
	case ST_VBD:
		if( reason > REASON_SEPARATOR ) {
			pvar ->state = ST_AUDIO;
			pvar ->reasonAudio = ( pvar ->reasonAudio << 8 ) | ( reason & 0xFF );
			v152_vars.bitsSwitchCodec |= ( 1 << sid );
			ret = 1;
		}
		break;
		
	default:
		break;
	}
	
	return ret;
}

static inline void V152_SwitchCodecNow( int s_id, int stAudio )
{
	extern TstVoipPayLoadTypeConfig astVoipPayLoadTypeConfig[];
	const TstVoipPayLoadTypeConfig *pstVoipPayLoadTypeConfig;
	
	uint32 ch_id = chanInfo_GetChannelbySession( s_id );
	unsigned long flags;
	
	pstVoipPayLoadTypeConfig = &astVoipPayLoadTypeConfig[ s_id ];
	
	save_flags(flags); cli();
	RtpTx_addTimestamp( s_id );	// add older codec's packet timestamp 
	DSP_CodecRestart(ch_id, s_id,
					 ( stAudio ? pstVoipPayLoadTypeConfig ->uPktFormat : pstVoipPayLoadTypeConfig ->uPktFormat_vbd ),
					 ( stAudio ? pstVoipPayLoadTypeConfig ->nFramePerPacket : 2 ),	// G.711 so force to set 2
					 pstVoipPayLoadTypeConfig ->nG723Type,
					 ( stAudio ? pstVoipPayLoadTypeConfig ->bVAD : 0 ),	// turn off VAD
					 pstVoipPayLoadTypeConfig ->nJitterDelay,
					 pstVoipPayLoadTypeConfig ->nMaxDelay,
					 ( stAudio ? pstVoipPayLoadTypeConfig ->nJitterFactor : 13 ),	// turn off JBC
					 pstVoipPayLoadTypeConfig ->nG726Packing,
					 pstVoipPayLoadTypeConfig ->nPcmMode);
	RtpTx_subTimestamp( s_id );	// sub newer codec's packet timestamp
	restore_flags(flags);
}

void V152_SwitchCodecIfNecessary( void )
{
	// This function will be called in period of 10ms, so it should be
	// very concise!! 
	
	int i;
	uint32 bit;

	if( v152_vars.bitsSwitchCodec == 0 )
		return;
		
	for( i = 0, bit = 0x01L; i < SESS_NUM; i ++, bit <<= 1 )
		if( v152_vars.bitsSwitchCodec & bit ) {
			const int stAudio = ( v152_vars.session[ i ].state == ST_AUDIO ? 1 : 0 );
			V152_SwitchCodecNow( i, stAudio );
		}
		
	v152_vars.bitsSwitchCodec = 0;
}

int V152_CheckIfSendVBD( int sid )
{
	v152_session_vars_t *pvar = &v152_vars.session[ sid ];
	int ret;
	
	if( sid >= SESS_NUM )
		return 0;
		
	if( !( v152_vars.bitsEnable & ( 1 << sid ) ) )
		return 0;
		
	switch( pvar ->state ) {
	case ST_AUDIO:
		ret = 0;
		break;
		
	case ST_VBD:
		ret = 1;
		break;
		
	default:
		return 0;
		break;
	}
	
	if( v152_vars.bitsSwitchCodec & ( 1 << sid ) )	// still switching 
		ret ^= 1;
	
	return ret;
}

voip_initcall( V152_Initialize );

