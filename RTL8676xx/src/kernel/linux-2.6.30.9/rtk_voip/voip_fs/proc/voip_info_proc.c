#include <linux/proc_fs.h>
#include <linux/init.h>
#include <linux/module.h>
#include "rtk_voip.h"
#include "voip_version.h"
#include "voip_feature.h"
//#include "Slic_api.h"
#include "snd_mux_slic.h"
//#include "Daa_api.h"
#include "snd_mux_daa.h"
#include "voip_init.h"
#include "voip_proc.h"
#include "voip_qos.h"

extern int voip_ch_num;
extern int slic_ch_num;

#ifdef ETHERNET_DSP_DEBUG_HOST
extern unsigned int dsp_ctrl_tx_cnt;
#endif
#ifdef ETHERNET_DSP_DEBUG_DSP
extern unsigned int dsp_resp_tx_cnt;
extern unsigned int dsp_rtp_tx_cnt;
extern unsigned int dsp_rtcp_tx_cnt;
extern unsigned int dsp_t38_tx_cnt;
#endif
#ifdef CONFIG_RTK_VOIP_QOS
int voip_qos;
#endif

int get_voip_info( char *buf, char **start, off_t off, int count, int *eof, void *data )
{
	int i;
	int n;
	
	if( off ) {	/* In our case, we write out all data at once. */
		*eof = 1;
		return 0;
	}

	n = sprintf(buf, "* VoIP Middleware	:");
	if (RTK_VOIP_MW_FEATURE == VOIP_MW_REALTEK)
		n += sprintf(buf + n, "Realtek\n");
	else if (RTK_VOIP_MW_FEATURE == VOIP_MW_AUDIOCODES)
		n += sprintf(buf + n, "AudioCodes\n");
		
	n += sprintf(buf + n, "* VoIP Version		:%s\n", VOIP_VERSION);
	n += sprintf(buf + n, "* VoIP Channel Number	:%d\n", voip_ch_num);
	n += sprintf(buf + n, "* SLIC Number		:%d\n", slic_ch_num);
	n += sprintf(buf + n, "* DAA Number		:%d\n", voip_ch_num - slic_ch_num);
	n += sprintf(buf + n, "* IVR Support		:");
	if (RTK_VOIP_IVR_FEATURE == IVR_SUPPORT)
		n += sprintf(buf + n, "Yes\n");
	else
		n += sprintf(buf + n, "No\n");
	
	n += sprintf(buf + n, "* T.38 Support		:");
	if (RTK_VOIP_T38_FEATURE == CODEC_T38_SUPPORT)
		n += sprintf(buf + n, "Yes\n");
	else
		n += sprintf(buf + n, "No\n");
#if !defined(CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST) && defined(CONFIG_RTK_VOIP_T38)
		extern char* t38_version_str;
		n += sprintf(buf + n, "%s", t38_version_str);
#endif
		
	n += sprintf(buf + n, "* Codec Support\n");
	n += sprintf(buf + n, "  - G.711u/a	:Yes\n");
	
	if (RTK_VOIP_G729_FEATURE == CODEC_G729_SUPPORT)
		n += sprintf(buf + n, "  - G.729	:Yes\n");

	if (RTK_VOIP_G723_FEATURE == CODEC_G723_SUPPORT)
		n += sprintf(buf + n, "  - G.723	:Yes\n");
		
	if (RTK_VOIP_G726_FEATURE == CODEC_G726_SUPPORT)
		n += sprintf(buf + n, "  - G.726	:Yes\n");
		
	if (RTK_VOIP_G722_FEATURE == CODEC_G722_SUPPORT)
		n += sprintf(buf + n, "  - G.722	:Yes\n");

	if (RTK_VOIP_GSMFR_FEATURE == CODEC_GSMFR_SUPPORT)
		n += sprintf(buf + n, "  - GSM-FR	:Yes\n");
		
	if (RTK_VOIP_iLBC_FEATURE == CODEC_iLBC_SUPPORT)
		n += sprintf(buf + n, "  - iLBC	:Yes\n");

	if (RTK_VOIP_SPEEX_NB_FEATURE == CODEC_SPEEX_NB_SUPPORT)
		n += sprintf(buf + n, "  - SPEEX-NB	:Yes\n");

#ifndef CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST
	n += sprintf(buf + n, "* FXS Line Status:\n");
	for(i=0; i < slic_ch_num; i++)
	{
		n += sprintf(buf + n, "  - Port%d: ", i);
		switch (FXS_Line_Check(i))
		{
			case 0:
				n += sprintf(buf + n, "Phone Dis-connected\n");
				break;
			case 1:
				n += sprintf(buf + n, "Phone Connected(On-hook)\n");
				break;
			case 2:
				n += sprintf(buf + n, "Phone Connected(Off-hook)\n");
				break;
			case 3:
				n += sprintf(buf + n, "Check Timeout(May connect too many phone set)\n");
				break;
			case 4:
				n += sprintf(buf + n, "Can't Check Status\n");
				break;
			default:
				n += sprintf(buf + n, "Unknow State\n");
				break;
		}
	}
	
#if defined (CONFIG_RTK_VOIP_DRIVERS_FXO) && !defined (CONFIG_RTK_VOIP_DRIVERS_VIRTUAL_DAA)

	n += sprintf(buf + n, "* FXO Line Status:\n");
		
	for(i = slic_ch_num; i < voip_ch_num; i++)
	{
		n += sprintf(buf + n, "  - Port%d: ", i);

		switch (DAA_Check_Line_State(i))
		{
			case 0:
				n += sprintf(buf + n, "Line Connected\n");
				break;
			case 1:
				n += sprintf(buf + n, "Line Dis-connected\n");
				break;
			case 2:
				n += sprintf(buf + n, "Line Busy\n");
				break;
			default:
				n += sprintf(buf + n, "Unknow State\n");
				break;
		}
	}
#endif
#endif	//CONFIG_RTK_VOIP_ETHERNET_DSP_IS_HOST

	*eof = 1;
	return n;
}

int __init voip_info_init( void )
{
	create_proc_read_entry( PROC_VOIP_DIR "/info",0, NULL, get_voip_info, NULL );
	return  0;
}

void __exit voip_info_exit( void )
{
	remove_proc_entry( PROC_VOIP_DIR "/info", NULL );
}

voip_initcall_proc(voip_info_init);
voip_exitcall(voip_info_exit);

int __init voip_proc_structure_init( void )
{
	extern int voip_ch_num;
	extern int sess_num;

	int i;
	char buf[ 256 ];
	
	proc_mkdir( PROC_VOIP_DIR, NULL );	// 'voip'

#ifdef CONFIG_RTK_VOIP_DRIVERS_ATA_DECT	
	proc_mkdir( PROC_VOIP_DIR "/" PROC_VOIP_DECT_DIR, NULL );
#endif 
	
#ifdef CONFIG_RTK_VOIP_DRIVERS_PCM
	proc_mkdir( PROC_VOIP_DIR "/" PROC_VOIP_PCM_DIR, NULL );
#endif
	
	proc_mkdir( PROC_VOIP_DIR "/" PROC_VOIP_CH_MAPS_DIR, NULL );
	
	for( i = 0; i < voip_ch_num; i ++ ) {	// 'voip/ch%02u'
		sprintf( buf, PROC_VOIP_DIR "/" PROC_VOIP_CH_FORMAT, i );
		proc_mkdir( buf, NULL );
	}

	for( i = 0; i < sess_num; i ++ ) {		// 'voip/ss%02u'
		sprintf( buf, PROC_VOIP_DIR "/" PROC_VOIP_SS_FORMAT, i );
		proc_mkdir( buf, NULL );
	}
	
	return 0;
}

void __exit voip_proc_structure_exit( void )
{
	extern int voip_ch_num;
	extern int sess_num;

	int i;
	char buf[ 256 ];
	
	proc_mkdir( PROC_VOIP_DIR, NULL );	// 'voip'
	
	for( i = 0; i < voip_ch_num; i ++ ) {	// 'voip/ch%02u'
		sprintf( buf, PROC_VOIP_DIR "/" PROC_VOIP_CH_FORMAT, i );
		remove_proc_entry( buf, NULL );
	}

	for( i = 0; i < sess_num; i ++ ) {		// 'voip/ss%02u'
		sprintf( buf, PROC_VOIP_DIR "/" PROC_VOIP_SS_FORMAT, i );
		remove_proc_entry( buf, NULL );
	}
}

voip_initcall( voip_proc_structure_init );
voip_exitcall( voip_proc_structure_exit );

#ifdef CONFIG_RTK_VOIP_QOS
static int voip_qos_read(char *page, char **start, off_t off,
        int count, int *eof, void *data)
{
	int len;
	
	char tmp[300]={0};
	int index = 0;
	if(!voip_qos)
	       len = sprintf(page,"qos is disabled\n");
	else
	{
		//index += sprintf(tmp+index,"voip_qos: %d \n",voip_qos);
		if(voip_qos & VOIP_QOS_INTERRUPT_RX_TASKLET)
		{
			index += sprintf(tmp+index,"INTERRUPT_RX_TASKLET is enabled \n");
		}
		if(voip_qos & VOIP_QOS_RX_HW_HIGH_QUEUE)
                {
                        index += sprintf(tmp+index,"RX_HW_HIGH_QUEUE is enabled \n");
			
                }
		if(voip_qos & VOIP_QOS_RX_SW_HIGH_QUEUE)
                {
                        index += sprintf(tmp+index,"RX_SW_HIGH_QUEUE is enabled \n");
                }
		if(voip_qos & VOIP_QOS_TX_HW_HIGH_QUEUE)
                {
                        index += sprintf(tmp+index,"TX_HW_HIGH_QUEUE is enabled \n");
                }
		if(voip_qos & VOIP_QOS_TX_SW_HIGH_QUEUE)
                {
                        index += sprintf(tmp+index,"TX_SW_HIGH_QUEUE is enabled \n");
                }
		if(voip_qos & VOIP_QOS_TX_DISABLE_FC)
                {
                        index += sprintf(tmp+index,"TX_DISABLE_FC is enabled \n");
                }
		if(voip_qos & VOIP_QOS_LOCAL_SESSION_RESERVE)
                {
                        index += sprintf(tmp+index,"LOCAL_SESSION_RESERVE is enabled \n");
                }
		if(voip_qos & VOIP_QOS_DROP_BIG_TRAFFIC)
                {
                        index += sprintf(tmp+index,"DROP_BIG_TRAFFIC is enabled \n");
                }
		len =  sprintf(page,tmp);
	}
    if (len <= off+count)
        *eof = 1;
    *start = page + off;
    len -= off;

    if (len > count)
        len = count;
    if (len < 0)
        len = 0;
    return len;

}
static int voip_qos_write(struct file *file, const char *buffer, unsigned long count, void *data)
{
	char tmp[128];

	if (count < 2)
		return -EFAULT;

	if (buffer && !copy_from_user(tmp, buffer, 128)) {
		sscanf(tmp,"%d",&voip_qos);
	}
	return count;
}
void __init voip_proc_qos_init (void)
{
	struct proc_dir_entry *voip_qos_proc;
	
	voip_qos |= VOIP_QOS_INTERRUPT_RX_TASKLET;
	voip_qos |= VOIP_QOS_RX_HW_HIGH_QUEUE;
	voip_qos |= VOIP_QOS_RX_SW_HIGH_QUEUE;
	voip_qos |= VOIP_QOS_TX_HW_HIGH_QUEUE;
	voip_qos |= VOIP_QOS_TX_SW_HIGH_QUEUE;
	voip_qos |= VOIP_QOS_TX_DISABLE_FC;
	voip_qos |= VOIP_QOS_LOCAL_SESSION_RESERVE;

	//voip_qos |= VOIP_QOS_DROP_BIG_TRAFFIC;
	
	voip_qos_proc = create_proc_entry(PROC_VOIP_DIR "/qos",0,NULL);
	if (voip_qos_proc == NULL){
		printk("voip_qos_proc NULL!! \n");
		return;
	}

	voip_qos_proc->read_proc = voip_qos_read;
	voip_qos_proc->write_proc = voip_qos_write;

}
void __exit voip_proc_qos_exit( void )
{
	remove_proc_entry( PROC_VOIP_DIR "/qos", NULL );
}

voip_initcall_proc( voip_proc_qos_init );
voip_exitcall( voip_proc_qos_exit );

#endif

