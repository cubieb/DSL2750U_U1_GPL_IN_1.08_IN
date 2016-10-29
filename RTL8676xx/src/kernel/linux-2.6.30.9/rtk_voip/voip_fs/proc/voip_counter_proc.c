#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include "rtk_voip.h"
#include "voip_types.h"
#include "voip_init.h"
#include "voip_proc.h"

#define VOIP_COUNTER_DISABLE	0
#define VOIP_COUNTER_ENABLE	1
#define VOIP_COUNTER_RESET	255

#ifdef SUPPORT_VOIP_DBG_COUNTER

int32 gVoipCounterEnable = 0;

static uint32 pcm_rx_count[BUS_PCM_CH_NUM] = {0};
static uint32 pcm_tx_count[BUS_PCM_CH_NUM] = {0};
static uint32 rtp_rx_count[SESS_NUM] = {0};
static uint32 rtp_tx_count[SESS_NUM] = {0};
static uint32 dsp_process_count[SESS_NUM] = {0};
static uint32 tone_gen_count[SESS_NUM] = {0};

void reset_all_count(void)
{
	int i;

	for (i=0; i<BUS_PCM_CH_NUM; i++)
	{
		pcm_rx_count[i] = 0;
		pcm_tx_count[i] = 0;
	}

	for (i=0; i<SESS_NUM; i++)
	{
		rtp_rx_count[i] = 0;
		rtp_tx_count[i] = 0;
		dsp_process_count[i] = 0;
		tone_gen_count[i] = 0;
	}
}

void PCM_rx_count(uint32 chid)
{
	pcm_rx_count[chid]++;
}

void PCM_tx_count(uint32 chid)
{
	pcm_tx_count[chid]++;
}

void RTP_rx_count(uint32 sid)
{
	rtp_rx_count[sid]++;
}

void RTP_tx_count(uint32 sid)
{
	rtp_tx_count[sid]++;
}

void DSP_process_count(uint32 sid)
{
	dsp_process_count[sid]++;
}

void Tone_gen_count(uint32 sid)
{
	tone_gen_count[sid]++;
}

static int voip_counter_read(char *page, char **start, off_t off,
        int count, int *eof, void *data)
{
	int chid, sid;
	int n;

	if( off ) {	/* In our case, we write out all data at once. */
		*eof = 1;
		return 0;
	}

	n = sprintf(page, "VoIP counter information:\n");

	if (gVoipCounterEnable == VOIP_COUNTER_DISABLE)
	{
		n += sprintf(page+n, "  *VoIP counter is disabled.\n");
	}
	else if (gVoipCounterEnable == VOIP_COUNTER_ENABLE)
	{
		n += sprintf(page+n, "  *PCM Counter:\n");
		for (chid = 0; chid < BUS_PCM_CH_NUM; chid++)
		{
			n += sprintf(page+n, "    - ch%d rx: %d \n", chid, pcm_rx_count[chid]);
			n += sprintf(page+n, "    - ch%d tx: %d \n", chid, pcm_tx_count[chid]);
		}
		
		n += sprintf(page+n, "  *RTP Counter:\n");
		for (sid = 0; sid < SESS_NUM; sid++)
		{
			n += sprintf(page+n, "    - sid%d rx: %d \n", sid, rtp_rx_count[sid]);
			n += sprintf(page+n, "    - sid%d tx: %d \n", sid, rtp_tx_count[sid]);
		}
		
		n += sprintf(page+n, "  *DSP Counter:\n");
		for (sid = 0; sid < SESS_NUM; sid++)
		{
			n += sprintf(page+n, "    - sid%d dsp process: %d \n", sid, dsp_process_count[sid]);
		}
		
		for (sid = 0; sid < SESS_NUM; sid++)
		{
			n += sprintf(page+n, "    - sid%d tone gen: %d \n", sid, tone_gen_count[sid]);
		}
	}

	*eof = 1;
	return n;
}

static int voip_counter_write(struct file *file, const char *buffer, 
                               unsigned long count, void *data)
{
	char tmp[128];

	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(tmp, buffer, 128)) {
		sscanf(tmp, "%d", &gVoipCounterEnable);
	}

	if (gVoipCounterEnable == VOIP_COUNTER_RESET)
	{
		reset_all_count();
		gVoipCounterEnable = VOIP_COUNTER_DISABLE;
	}
	//printk("gVoipCounterEnable = %d\n", gVoipCounterEnable);

	return count;
}
#endif	//SUPPORT_VOIP_DBG_COUNTER

static int __init voip_proc_counter_init(void)
{
#ifdef SUPPORT_VOIP_DBG_COUNTER
	struct proc_dir_entry *voip_counter_proc;

	voip_counter_proc = create_proc_entry(PROC_VOIP_DIR "/counter", 0, NULL);
	
	if (voip_counter_proc == NULL)
	{
		printk("voip_counter_proc NULL!! \n");
		return -1;
	}
	voip_counter_proc->read_proc = voip_counter_read;
	voip_counter_proc->write_proc = voip_counter_write;
#else
	printk("SUPPORT_VOIP_DBG_COUNTER is not defeind!\n");
#endif
	
	return 0;
}

static void __exit voip_proc_counter_exit( void )
{
#ifdef SUPPORT_VOIP_DBG_COUNTER
	remove_proc_entry( PROC_VOIP_DIR "/counter", NULL );
#else
	printk("SUPPORT_VOIP_DBG_COUNTER is not defeind!\n");
#endif
}

voip_initcall_proc( voip_proc_counter_init );
voip_exitcall( voip_proc_counter_exit );
