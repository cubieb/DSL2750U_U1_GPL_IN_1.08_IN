#ifndef _SDRAM_MAP_H
#define	_SDRAM_MAP_H

#include "mem.h"

#define	TOT_TONES		2048
#define CODE_SIZE		(1024*1024)
#define	SDRAM_BASE_ADDR	(0x4000000 + CODE_SIZE)


typedef struct {
	unsigned char	share_mem[1024];
	unsigned short	tx_bit_tone_table[NUM_VDSL_CHANNELS][TOT_TONES];
	unsigned short	tx_gain_table[NUM_VDSL_CHANNELS][TOT_TONES];
	u16				tx_gi[2048];
	u16				tx_upb[2048];
	u16				tx_precmp[2048];
   	u8				tx_showtime_bit_table[2048];
	short			teq_coefs[64];
	u8				b_buf_out[1024*32];
	int				fft_abs_real[2048];
	int				fft_abs_imag[2048];
	u8				feq_cg[1024];
	u16				rx_tone_bit_table[2048];
	u16				symb_tone_bit_table[400];
	tComplex_s16	feq_coef_msb16[2048];
	tComplex_s8		feq_coef_msb8[2048];
	u8				snr_table[10*1024];	 //  noise pwr
	s16				snr_array[2048];  //  SNR in log2 format
	s16				rx_gi[2048];
   	u8				rx_showtime_bit_table[NUM_VDSL_CHANNELS][2048];
	u32				npwr_table_showtime[NUM_VDSL_CHANNELS][2048];	 	//  noise pwr
	s16				snr_array_showtime[NUM_VDSL_CHANNELS][2048];  		//  SNR in log2 format
} 

SDRAM_MAP;

extern SDRAM_MAP *sdram_map;

#endif