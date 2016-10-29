#ifndef _VDSL_CH_H
#define _VDSL_CH_H
#include "bme.h" 
#include "symb_boundary.h"
#include "vdsl_framer.h"
#include "fixed_bgtable.h"
#include "clock_eff_gen.h"

typedef enum {
	OP_TRAINING = 0, 
	MED_TRAINING,
	TX_SHOWTIME_INIT, 
	TX_GOTO_SHOWTIME, 
	TX_SHOWTIME
} TX_STATES;

typedef enum {
	MEAS_NPW=0,
	SIG_DETECT,  
	AGC_SETTING, 
	CLOCK_REC, 
	SYM_BOUNDARY, 
	AGC_SETTING_II,
	FFT_COARSE_GAIN, 
	FEQ_TRAINING_STAGE, 
	FEQ_TRAINING_DFE,
	RX_SHOWTIME_INIT,
	RX_GOTO_SHOWTIME, 
	RX_SHOWTIME
} RX_STATES;

typedef enum {
	TRAINING_MODE_REVERB = 0,
	TRAINING_MODE_MEDLEY,
	TRAINING_MODE_MEDLEY_DFE,
	TRAINING_MODE_REVERB_SYNCHRO00,
	TRAINING_MODE_REVERB_SYNCHRO11,
	TRAINING_MODE_MEDLEY_SYNCHRO00,
	TRAINING_MODE_MEDLEY_SYNCHRO11,
	SHOWTIME_DEF
} OP_MODE;

#define MAX_BANDS	4

typedef struct {
	LOCATION			loc;
	BAND_PLANS			bandplan;
	char				tx_showtime_param_file[100];
	char				tx_showtime_param_sem_file[100];
	char				tx_showtime_sem_file[100];
	char				cpe_tx_medley_start_file_sem[100];
	char				cpe_tx_medley_start_done_file_sem[100];
	char				cpe_rx_medley_start_file_sem[100];
	char				cpe_rx_medley_start_done_file_sem[100];
	char				co_tx_medley_start_file_sem[100];
	char				co_tx_medley_start_done_file_sem[100];
	char				co_rx_medley_start_file_sem[100];
	char				co_rx_medley_start_done_file_sem[100];
	char				co_rx_start_file_sem[100];
	char				cpe_tx_start_file_sem[100];
	char				rx_showtime_param_file[100];
	char				rx_showtime_sem_file[100];
	char				rx_showtime_param_sem_file[100];
	int					num_tx_bands;
	int					tx_band_start[MAX_BANDS];
	int					tx_band_stop[MAX_BANDS];
	int					tx_pilot_tone; 
	int					zbuf_offset;
	int					tx_ptone_scr_on;
	int					num_rx_bands;
	int					rx_band_start[MAX_BANDS];
	int					rx_band_stop[MAX_BANDS];
	int					rx_pilot_tone; 
	int					noise_band_start;
	char				rx_perf_dir[100];
	u8					rx_vbus_chan_num;
	u8					rx_afe_chan_num;
	u8					rx_afe_chip_index;
	int					rx_ptone_scr_on;
	u8					is_enable_feq_interp;
	int					noise_margin;
	int					coding_gain;
} VDSL_CFG;


typedef struct {
	u16		frame_num;
	u8		ch_num;
	u8		tag;
} BUF_ID;
          
typedef struct {
	LOCATION			location;
	BAND_PLANS			bandplan;
	CHANNEL_STATES			ch_state;
	char				tx_showtime_init_infile[100];
	char				tx_showtime_file_sem[100];
	char				tx_showtime_param_sem_file[100];
	char				tx_medley_start_file_sem[100];
	char				tx_medley_start_done_file_sem[100];
	char				cpe_tx_start_file_sem[100];
	int					ch_num;
	int					is_tx_enabled;
	int					is_rx_enabled;
	// tx
	u8					is_tx_ptone_scr_on;
	int 				curr_tx_symbol_num;
	int					tx_fft_size;
	TX_STATES			curr_tx_state;
	int		   			tx_showtime_start;
	int					num_tx_bands;
	int					tx_band_start[MAX_BANDS];
	int					tx_band_stop[MAX_BANDS];
	int					tx_pilot_tone; 
	int					zbuf_offset;
	u8					is_medley_training_started;
	u16					tx_rs_code_N;
	u16					tx_rs_code_R;
	u16					tx_prefix_len; //excluding beta
	u16					tx_suffix_len;
	u16					tx_beta_len;
	u16					is_op_training_started;
	VDSL_Framer_Param	tx_framer_param;
	u16					*tx_tone_bit_table;
	u16					*tx_gain_table;
	s16					*tx_gi;
	u8					*tx_bit_table;
	u16					*tx_upb;
	u16					*tx_precmp;
	// rx
	char				rx_showtime_init_outfile[100];
	char				rx_showtime_param_sem_file[100];
	char				rx_showtime_file_sem[100];
	char				rx_medley_start_file_sem[100];
	char				rx_medley_start_done_file_sem[100];
	char				co_rx_start_file_sem[100];
	char				rx_perf_dir[100];
	u8					showtime_init_mode;
	OP_MODE				operation_mode_rx; 
	int					curr_medley_symb_num_rx;
	u8					rx_vbus_chan_num;
	u8					rx_afe_chan_num;
	u8					rx_afe_chip_index;
	int					curr_rx_symbol_num;
	int					dfe_training_start_sym_num;
	u8					is_enable_feq_interp;
	u8					is_enable_dfe_feq_train;
	u8					is_rx_medley_dfe_started;
	u16					fft_size;
	u8					isRxShowtime_inited;
	u8					is_enable_clk_recv;
	u8					is_rx_ptone_scr_on;
	u8					feq_train_started;
	RX_STATES			curr_rx_state;
	int					num_rx_bands;
	int					rx_band_start[MAX_BANDS];
	int					rx_band_stop[MAX_BANDS];
	int					rx_pilot_tone; 
	int					tot_symb_delay;
	u16					rx_rs_code_N;
	u16					rx_rs_code_R;
	u16					rx_prefix_len; //excluding beta
	u16					rx_suffix_len;
	u16					rx_beta_len;
	u16					next_sym_2_cap;
	u16					frame_offset;
	u16					rx_pwr_monitor_sig_start;
	u16					num_rx_pwr_monitor_sig_bins;
	u16					rx_pwr_monitor_nos_start;
	u16					num_rx_pwr_monitor_nos_bins;
	int					sig_pwr;
	int					nos_pwr;
	u8					rx_fft_scale_shift;
	u8					teq_scale_shift;
	u8					num_teq_taps;
	u8					num_feq_cg_avgs;
	u8					num_npw_avgs;
	u8					sym_bound_count;
	u8					is_symb_bound_done;
	u8					is_doing_agc_2;
	u8					is_doing_clk_rev;
	u8					curr_feq_avg_index;
	u16					feq_train_duration;
	u8					tag_size;
	u8					desired_clk_recv_state;
	u16					rx_kf;
	u16					loaded_tones;
	u16					b_buf_data_avail;
	int					noise_margin;
	int					coding_gain;
	SYMB_STATUS			symb_status;
	tBG_TABLE			pbg_table;
	VDSL_Framer_Param	fast_path_cfg;
	int				snr_sem;
	int				clk_rev_task_sem;
	s16					*teq_coefs;
	void				*b_buf_start;
	tComplex_s16		*b_buf_out;
	int					*fft_abs_real;
	int					*fft_abs_imag;
	u8					*feq_cg;
	u16					*rx_tone_bit_table;
	tComplex_s16		*feq_coef_msb16;
	tComplex_s8			*feq_coef_msb8;
	u32					*snr_table;
	s16					*snr_array;  //  SNR in log2 format
	s16					*rx_gi;
	u8					*rx_showtime_bit_table;
	u8					*host_rx_dst_buf;
	u8					*host_rx_tensilica_buf;
	s16					*snr_array_showtime;
	u32					*npwr_showtime;
	DPLL_PARAM          *phase_lock_ptr; 
} XDSL_LINE;

extern int	is_init_gain_table_written;

extern void tx_init_table(XDSL_LINE *curr_line);
extern void tx_set_lcs_lcp(u8 channel, u16 tx_prefix_len, u16 tx_suffix_len, u16 tx_beta_len);
extern void do_tx(XDSL_LINE *curr_line);
extern void init_ch_tx(XDSL_LINE *line, int ch_num, LOCATION location, VDSL_CFG *vdsl_cfg);
extern void init_ch_rx(XDSL_LINE *line, int ch_num, LOCATION location, VDSL_CFG *vdsl_cfg);
extern void init_rx_teq_coef(XDSL_LINE *curr_line);
extern void	init_rx_bit_table(XDSL_LINE *curr_line);
extern void	init_rx_feq_gains(XDSL_LINE *curr_line, int feq_cg_val);
extern void	reset_snr_acc(XDSL_LINE *curr_line);
extern void enable_tx(int);
extern void enable_rx(int);
extern void tx_set_upb(u16 *tx_upb, int length);
extern void tx_set_precmp(u16 *tx_precmp, int length, LOCATION loc, BAND_PLANS bandplan);
extern void coordinate_tx_rx(XDSL_LINE *tx_line, XDSL_LINE *rx_line, int mode);
extern void turn_off_ptone_scr();
extern void turn_on_ptone_scr();

extern void do_rx(XDSL_LINE *curr_line);
extern void rmc_start_line(u16 line_num, u16 fft_scale_shift, u16 num_tones, u8 win_enable, u16 beta_len, u16 loaded_tones);
extern void rmc_config_tra(u16 line_num, u16 fft_scale_shift, u16 num_tones, u8 win_enable, u16 beta_len, u8 teq_scale_shift, u8 num_teq_taps,
				u16 prefix_len, u16 suffix_len, u16 loaded_tones, int frame_no);
extern void rmc_capture_fft_out(u16 line_num, int frame_offset, int duration);
extern void rmc_skip_samples(u16 line_num, u16 num_samples, u16 tag);
extern void rmc_stop_line(u16 line_num, u16 frame_num);
extern int  capture_Abuf(u8 *c_buf, u16 line_num, u16 num_tones, u16 beta_len, u16 duration, int is_to_capture);
extern int  is_not_in_band(int bin_index, int num_band, int *band_start, int *band_stop);
extern int  capture_Bbuf(XDSL_LINE *line, u16 expected_frame_num, u8 expected_tag, int duration, int loaded_tones, int tag_size);

// function prototypes for clk recovery 
extern void setup_clk_rev(XDSL_LINE *curr_line);
extern void start_clk_rev_task(int line_num, XDSL_LINE *curr_line);
extern int	curr_clk_recovery_status(XDSL_LINE *curr_line);
extern DPLL_PARAM *create_dpll_param();
// function prototypes for symbol boundary detection
extern void init_vdsl_training();
extern void init_symbol_boundary(XDSL_LINE *ch);
extern void setup_symb_bound_band(XDSL_LINE *ch);
extern void descramble_single_tone(short re,short im,u8 scr,short *x,short *y);
extern int  doVDSL_symb_boundary_detect(XDSL_LINE *ch);
extern int  getSymSkip(XDSL_LINE *curr_line);

// signal detection
extern int  doSigDetect(XDSL_LINE *curr_line);
extern int	doNoisePow(XDSL_LINE *curr_line);

extern int 	do_agc(XDSL_LINE *curr_line);

// function prototypes for FEQ
extern void init_feq(XDSL_LINE *ch);
extern int  doFeqCg(XDSL_LINE *curr_line);
extern int	feq_interp(XDSL_LINE *curr_line, int interval);

extern void reduce_bits(tBG_TABLE *pbg_table, int excess_bits, unsigned char *bit_table_array, 
				 unsigned short *gain_table_array, int length);


extern void SetShowtimeTstOpts( int rs_on, int crc_on, int scr_on, int atm_off);
extern int  WrtFmcShowtimeCmds( int nBytes, int nCWf, int Nf, int Kf, int fifoFlag, int chan_num);
extern int  WrtDmcShowtimeCmds( int nBytes, int nCWf, int Nf, int Kf, int fifoFlag, int chNum);
extern int 	ver0_WrtFmcShowtimeCmds( int cmdSeqNum);
extern int  ver0_WrtDmcShowtimeCmds( int cmdSeqNum);

extern void start_rx_task(u8 line_num, XDSL_LINE *curr_line);
#endif

