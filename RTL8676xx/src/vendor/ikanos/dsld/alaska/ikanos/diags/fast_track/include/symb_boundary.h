#ifndef _SYMBOL_BOUNDARY_H_
#define _SYMBOL_BOUNDARY_H_

typedef enum {SYMBOL_BOUND_MAN_SET=0, SYMBOL_BOUNDARY_FAILED, SYMBOL_BOUNDARY_COARSE_EST, SYMBOL_BOUNDARY_COARSE_EST_DONE, SYMBOL_BOUNDARY_FINE_EST, SYMBOL_BOUNDARY_DONE} BOUNDARY_DETECT_STATUS;
typedef enum {DEFINE_POSITION = 0, DEFINE_ANGLE} DETECT_STAGE;

typedef struct {
	char						flag;
	short					start;				// band start
	short					stop;				// band stop
	BOUNDARY_DETECT_STATUS	status;
	DETECT_STAGE			stage;
	short					step_delay_shifts;
	short					delay_step_size;
	short					fft_size;
	short					tot_step;
	short					curr_delay_count;
	short					fft_avg_shifts;
	short					num_fft_avgs;
	short					curr_fft_avg_num;
	short					max_mag_delay;
	short					tot_delay_adj;
	short					delay_adj;
	short					angle_value;
	short					is_symb_tone_bit_tab_inited;
	short					dmt_size;
	short					loaded_tones;
	int						max_mag;
	int						new_demapper_context;
	int						old_demapper_context;
	int						*main_real;
	int						*main_imag;
	short					*fft_real_part;
	short					*fft_imag_part;
	unsigned short			*tone_bit_tab;
} SYMB_STATUS;

#endif