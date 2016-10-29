#include <linux/string.h>
#include "rtk_voip.h"
#include "voip_types.h"
#include "snd_mux_slic.h"
#include "snd_help.h"

#include "fsk.h"
#include "../voip_dsp/include/fskcid_gen.h"

// --------------------------------------------------------
// FSK redirect function 
// --------------------------------------------------------

extern TstVoipFskClid cid_info[VOIP_CH_NUM];

void CID_for_FSK_SW(unsigned int chid, char service_type, unsigned char msg_type, TstFskClidData* clid_data)
{
	extern char fsk_cid_state[];
	const uint32 cch = chid;

#if ! defined (AUDIOCODES_VOIP)

	// if (fxs off-hook && type-1 fsk)
	if ((SLIC_Get_Hook_Status(cch, 1) == 1) && (service_type == 0))
	{
		//PRINT_Y("cid send abort\n");
		return;	// don't send cid when phone off-hook
	}
									
	if ( fsk_cid_state[chid] == 0)
	{
		cid_info[chid].cid_mode = service_type;	// Service Type 
		cid_info[chid].cid_msg_type = msg_type;		// Message Type
		memcpy(cid_info[chid].cid_data, clid_data, FSK_MDMF_SIZE*sizeof(TstFskClidData));
#if 0
		int i;
		for (i=0; i<FSK_MDMF_SIZE; i++)
		{
			printk("cid_info[%d].cid_data[%d].type = %d\n", chid, i, cid_info[chid].cid_data[i].type);
			printk("cid_info[%d].cid_data[%d].data = %s\n", chid, i, cid_info[chid].cid_data[i].data);
		}
			
#endif
		
		fsk_cid_state[chid] = 1;
	}
	else
	{
		// If fsk CID is during transmission, ignore new one.
	}
	
#endif
}


void SLIC_gen_FSK_CID(unsigned int chid, char mode, unsigned char msg_type, TstFskClidData* clid_data)
{
#ifndef AUDIOCODES_VOIP
	/* HW FSK CID gen is not supported, so it always use SW FSK CID gen. */
	//CID_for_FSK_SW(chid, mode, msg_type , str, str2, cid_name);
	CID_for_FSK_SW(chid, mode, msg_type, clid_data);
#endif

}

int SLIC_gen_VMWI(unsigned int chid, TstFskClidData* clid_data)
{
#ifndef AUDIOCODES_VOIP

	init_softfskcidGen(chid);
	CID_for_FSK_SW(chid, 0 /*type 1*/, FSK_MSG_MWSETUP, clid_data);

	return 1;
#endif
}




