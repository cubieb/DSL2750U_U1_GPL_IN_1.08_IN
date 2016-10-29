#ifndef __IVR_HELP_H__
#define __IVR_HELP_H__

#ifdef CONFIG_AUDIOCODES_VOIP	// help ACMW to compile pass, but IVR WB function is not workable

typedef enum {		/* This can help to up/down sampling before/after encode/decode */
	SAMPLE_NARROW_BAND,		/* 8K sampling rate */
	SAMPLE_WIDE_BAND,		/* 16k sampling rate */
	SAMPLE_IGNORE,			/* Don't care sampling rate in silence codec */
} SampleRate_t;

extern SampleRate_t VoipChannelSampleRate[];

//#ifdef CONFIG_VOIP_SDK

#define MAX_NB_CHANNELS 1

typedef struct ResamplerState_ ResamplerState;

typedef int (*resampler_basic_func)(ResamplerState *, uint32_t , const int16_t *, uint32_t *, int16_t *, uint32_t *);

struct ResamplerState_ {
   uint32_t in_rate;
   uint32_t out_rate;
   uint32_t num_rate;
   uint32_t den_rate;
   
   int    quality;
   uint32_t nb_channels;
   uint32_t filt_len;
   uint32_t mem_alloc_size;
   int     	int_advance;
   int          frac_advance;
   //float  cutoff;
   uint32_t oversample;
   int          initialised;
   int          started;
   
   /* These are per-channel */
   int32_t  last_sample[MAX_NB_CHANNELS];
   uint32_t samp_frac_num[MAX_NB_CHANNELS];
   uint32_t magic_samples[MAX_NB_CHANNELS];
   
#if SUPPORT_QUALITY_0_TO_3
   int16_t mem[MAX_NB_CHANNELS*96];//support for max. rank 3
#endif
#if SUPPORT_QUALITY_0_TO_10
   int16_t mem[MAX_NB_CHANNELS*512]; //support for max. rank 10
#endif
   int16_t *sinc_table;
   uint32_t sinc_table_length;
   resampler_basic_func resampler_ptr;
         
   int    in_stride;
   int    out_stride;
} ;

#if 0
extern int resampler_process_int_ex( ResamplerState *st, 
                                 uint32_t channel_index, 
                                 const int16_t *in, 
                                 uint32_t in_len_n, 
                                 int16_t *out, 
                                 uint32_t out_len_n,
                                 const char *err_format);
#endif

extern int resampler_process_int(ResamplerState *st,
                                 uint32_t channel_index,
				 const int16_t *in,
				 uint32_t *in_len,
				 int16_t *out,
				 uint32_t *out_len);
//#endif

#else

#include "codec_descriptor.h"

#endif

#endif	//__IVR_HELP_H__

