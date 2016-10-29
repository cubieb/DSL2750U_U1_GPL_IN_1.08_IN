#ifndef __V152_API_H__
#define __V152_API_H__

typedef enum {
	ST_AUDIO = 0,
	ST_VBD,
	ST_ERR_SID,
} v152st;	// V152 states 

typedef enum {
	REASON_NONE = 0x00,
	// reason to VBD
	REASON_SIG_VBD_CED	= 0x10,
	REASON_SIG_VBD_PREAMBLE,
	REASON_SIG_VBD_CNG,
	REASON_RTP_VBD		= 0x20,
	
	// separator 
	REASON_SEPARATOR		= 0x7F,
	// reason to AUDIO
	REASON_VOC_BI_SILENCE	= 0x80,	// bidirectional silence 
	REASON_TDM_VOICE		= 0x90,
	REASON_TDM_SIG_END,
	REASON_RTP_AUDIO		= 0xA0,
	REASON_MGC_SIG			= 0xB0,	// MGC signaling 
	REASON_OFB_SIG,					// other out-of-band signaling 
} v152reason;	// V152 reason (reason should be less than 0xFF)

// initial v152 on start 
extern int  V152_Initialize( void );

// initial v152 when setup session 
extern void V152_InitializeSession( int sid, int enable );

// Voice-VBD state transition (not really switch codec)
extern int V152_StateTransition( int sid, v152reason reason );

// switch codec on handler startup 
extern void V152_SwitchCodecIfNecessary( void );

// decide payload type when sending RTP
extern int V152_CheckIfSendVBD( int sid );


#endif // __V152_API_H__

