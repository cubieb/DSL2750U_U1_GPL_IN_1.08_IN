#include <linux/kernel.h>
#include "voip_mgr_help_dsp.h"

static unsigned int gDSP_id = 0;

unsigned int Get_Ethernet_DSP_ID(void)
{	
	return gDSP_id;
}

int Set_Ethernet_DSP_ID(unsigned char dsp_id)
{	
	if (( dsp_id < 0 ) || ( dsp_id > 15))
	{
		printk("%s: NOT support dsp_id=%d\n", __FUNCTION__, dsp_id);
		return 0;
	}
		
	gDSP_id = dsp_id;
	printk("Set DSP ID to %d\n", dsp_id);
	
	return 1;
}

