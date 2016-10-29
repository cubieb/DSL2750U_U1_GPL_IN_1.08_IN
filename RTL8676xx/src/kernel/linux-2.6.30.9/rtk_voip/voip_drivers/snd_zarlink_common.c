#include <linux/kernel.h>
#include "voip_types.h"
#include "con_register.h"
#include "voip_debug.h"
#include "snd_define.h"
#include "zarlink_api.h"

// --------------------------------------------------------
// zarlink common ops 
// --------------------------------------------------------

void SOLAC_PCMSetup_priv_ops( voip_snd_t *this, int enable )
{
	RTKLineObj * const pLine = (RTKLineObj * )this ->priv;
	int pcm_mode = pLine->pcm_law_save;
	
#if 0
	PRINT_Y( "%s()line#%d, enable=%d,pLine->pcm_law_save=%d\n",\
			 __FUNCTION__,pLine->ch_id, enable, pLine->pcm_law_save );
#endif

	if( !enable )
		return;
	
	// check if need to switch between narrowband/wideband mode 
	if (enable == 2) { /* Change to wideband mode */
		switch( pLine->pcm_law_save ) {
			case PCMMODE_LINEAR:
				pcm_mode =  PCMMODE_WIDEBAND_LINEAR; 
				break;
			case PCMMODE_ALAW:
				pcm_mode =  PCMMODE_WIDEBAND_ALAW; 
				break;
			case PCMMODE_ULAW:
				pcm_mode =  PCMMODE_WIDEBAND_ULAW; 
				break;
			default:
			/* do nothing */
			break;
		}
	}
		
	if (enable == 1) { /* Change to narrow band mode */
		switch( pLine->pcm_law_save ) {
			case PCMMODE_WIDEBAND_ALAW: 	
			PRINT_R("%s() Doesn't support PCMMODE_WIDEBAND_ALAW\n", __FUNCTION__);
			pcm_mode = 	PCMMODE_ALAW;
			break;

			case PCMMODE_WIDEBAND_ULAW: 
			PRINT_R("%s() Doesn't support PCMMODE_WIDEBAND_ULAW\n", __FUNCTION__);
			pcm_mode = 	PCMMODE_ULAW;
			break;

			case PCMMODE_WIDEBAND_LINEAR:
			pcm_mode = 	PCMMODE_LINEAR;
			break;

			default:
			/* do nothing */
			break;
		}
	}
	
	if( pcm_mode >= 0 ) {
		ZarlinkSetFxsPcmMode(pLine, pcm_mode);
		ZarlinkSetFxsAcProfileByBand(pLine, pcm_mode);
	} else {
		PRINT_R("%s() pcm_mode %d error\n", __FUNCTION__, pcm_mode);
	}		
}


