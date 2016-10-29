#ifndef __SND_PROSLIC_TYPE_H__
#define __SND_PROSLIC_TYPE_H__

#include "proslic_api/inc/proslic.h"
#include "proslic_api/inc/vdaa.h"
#include "proslic_api/inc/sys_driv_type.h"

#define INVALID_HOOK_STATUS	( 0xFF )

typedef struct {
	unsigned char hookStatus;		// save FXS/FXO hook status to speed up 
	unsigned char cid_reg64_prev;
	unsigned char pcm_law_save;		// save last PCM mode (PCM_MODE)
} user_priv_t;

typedef struct {
	unsigned long connect_count;
	unsigned long timeout_count;
	unsigned long TR_thres;
	unsigned long TIP_thres;
} link_check_t;

typedef struct {
	controlInterfaceType *ProHWIntf;	// a daisy chain use one instance  
	ctrl_S *spiGciObj;					// SPI interface 
	ProslicDeviceType *ProSLICDevices;	// one chip one instance 
	proslicChanType *ProObj;			// one FXS one instance 
	vdaaChanType *daas;					// one DAA one instance 	
	
	user_priv_t user;					// user private data 
	link_check_t line;					// link check data 
} ProslicContainer_t;		// snd private data point to this! 

//////////////////////////////////////////////////////////////////
// common function for various proslic 

typedef struct {
	int ring_setup_preset;
} proslic_args_t;

extern void proslic_alloc_objs( ProslicContainer_t container[], 
		int size, int devices, int chan, 
		int type );

extern void proslic_init_user_objs( ProslicContainer_t container[], 
		int size, int type );

extern void proslic_init( const voip_snd_t p_snds[],
		ProslicContainer_t container[],
		int size,
		const proslic_args_t *p_args, 
		int pcm_mode);


#endif // __SND_PROSLIC_TYPE_H__

