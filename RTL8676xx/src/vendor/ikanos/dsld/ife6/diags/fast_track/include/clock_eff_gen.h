
#ifndef _CLOCK_EFF_GEN
#define _CLOCK_EFF_GEN


#define PLL_DAC_BITS			(12)               /* 12 bits DAC */
#define PLL_SHR_BITS    		(22-PLL_DAC_BITS) 
#define MAX_1X					(short)(((1L << (PLL_DAC_BITS - 1)) - 1))     
#define MIN_1X					(short)(~MAX_1X)
#define PLL_DAC_VAL     		(1 << (PLL_DAC_BITS - 1))

#define START_PILOT_CAL      	(800000)
#define CAL_FIRST_SHR        	(1)
#define CAL_SECOND_SHR       	(4)
#define PILOT_FIRST_AVRG_NUM  	(1 << CAL_FIRST_SHR)
#define PILOT_SECOND_AVRG_NUM 	(1 << CAL_SECOND_SHR)

#define SHR_0_BITS				(0)
#define SHR_1_BITS				(1)             
#define SHR_2_BITS				(2) 
#define SHR_3_BITS				(3)              
#define SHR_4_BITS				(4)            
#define SHR_5_BITS				(5)            
#define SHR_6_BITS				(6)             
#define SHR_7_BITS				(7)             
#define SHR_8_BITS				(8)             

#define PILOT_ID        		(60)
/******************************************************************
 * First set of coefficients
 ******************************************************************/
#define H11						(89)
#define H12						(20)
/******************************************************************
 * Second set of coefficients
 ******************************************************************/
#define H21				  		(11)
#define H22				 		( 3)      
#define CHANGE_COEFFS_NUM 		(3000)

#define UNLOCKED				(0)
#define LOCKING					(1)
#define LOCKED					(2)

#define	MAX_AVG_ARRAY			16

typedef struct DPLL_PARAM
{
	long			w_long;
	long			acc_pilot_tones;
	long            acc_pilot_real;
	long            acc_pilot_imag;
	unsigned long   symbol_cnt;
    unsigned long   calibration_count;
	short			g1;
	short			g2;
	short           shr_cnt;
	short			error;
	short			state;
	short           pilot_average_num;
	short           current_average_cnt; // for PLL
	short           current_cal_avg_cnt;
	short           cal_num_avg;         // number of average for pilot cal
	short           cal_acc_shr;
	short           cal_init_param_flag;
	short           pilot_id;
	short           pilot_real;
    short           pilot_imag;
	short           current_pilot_real;
	short           current_pilot_imag;
    short           sat_acc_filter_cnt;      // test 32 range
	short           save_pilot_flag;
	short           symbol_boundary_count;
	short		gain_correction;
	int				ptone_dmem_buf_rd_ptr;
	int				avg_ph_err[MAX_AVG_ARRAY];
	int				curr_err_cnt;
	int				curr_err_index;
	int				is_pilot_acquired;
}DPLL_PARAM;

extern DPLL_PARAM phase_lock_param;
/*************************************************************************/
#endif  /* _CLOCK_EFF_GEN */