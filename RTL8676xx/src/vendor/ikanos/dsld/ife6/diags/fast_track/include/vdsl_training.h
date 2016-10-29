#ifndef VDSL_TRAINING
#define VDSL_TRAINING

#include "vdsl_init_const.h"

/* Commented By SACHIN DESAI */
/*typedef  unsigned char  u8; */
/*typedef  unsigned short u16; */


#define  MAX(x,y)  ((x) > (y) ? (x) : (y))

/************************************************************************************************/
static u8 message_ab[16][NUM_ROWS];
static u8 message_cd[16][NUM_ROWS];
static u8 message_bc[16][NUM_ROWS];
static u8 message_wa[4][NUM_ROWS];
static u8 message_dw[4][NUM_ROWS];
u8 scr_bytes_array[SCR_BYTES_ARRAY_SIZE];
u8 scr_bytes_swapped_array[SCR_BYTES_ARRAY_SIZE];
static u8 synchro_00_array[SYNCHRO_ARRAY_SIZE];
static u8 synchro_11_array[SYNCHRO_ARRAY_SIZE];
static u8 xx_scrambled_low_four_bits[16][16];
static u8 xx_scrambled_high_four_bits[16][16];
static u8 two_tones_descr_table[16][16];


/***************************************************************************************************/
void accumulate_single_tone(short *real,short *imag,u8 scr1,u8 scr2,long *acc_real,long *acc_imag);
void init_periodic_bytes_scrambler_table(u8 *scr_array_ptr,u8 *scr_swapped_array_ptr, u8 *scr_four_bits_array_ptr);
u8   two_bits_scrambler_output();
u8   scrambler_output();

void create_descrambled_table(void);
u16  calculate_scr_starting_index(u16 index);
void create_scrambled_tables(void);
void init_synchro_tables(u8 *synchro_00_ptr, u8 *synchro_11_ptr);
void init_o_p_trainig_arrays(void);
void craete_message_array(int row, int col, int offset, u8 mssg_array[][NUM_ROWS], u8 *scr_ptr);
/*****************************************************************************************************/

void create_o_p_training_symbol(char x, u8 *array);
void create_o_p_synchro_00_symbol(u8 *dmt_symbol);
void create_o_p_synchro_11_symbol(u8 *dmt_symbol);
void create_medley_synchro_00_symbol(u8 *dmt_symbol,u16 dmt_idx);
void create_medley_synchro_11_symbol(u8 *dmt_symbol,u16 dmt_idx);
void create_medley_training_symbol(char msg1, char msg2,u8 *array_ptr,u16 dmt_idx);

u8 decode_o_p_training_symbol_before_synchro(short *real_ptr,short *imag_ptr,u16 start,
											 u16 num_avrg, u8 *msg);
u8 decode_o_p_training_symbol_during_synchro(u8 *slicer_output,u16 start, u16 num_avrg, u8 *msg);
u8 decode_medley_training_symbol_before_synchro(u8 *slicer_output,u16 start, u16 num_avrg,
												  u16 dmt_idx, u8 *msg1, u8 *msg2);
u8 decode_medley_training_symbol_during_synchro(u8 *slicer_output,u16 start, u16 num_avrg,
												u16 dmt_idx, u8 *msg1, u8 *msg2);
/***************************************************************************************************/

/***************************************************************************************************/
/* firs argument is an angle, second is a constelation point */
u8 rotate_tone[4][4]=
{
	{0,1,2,3},
	{2,0,3,1},
	{1,3,0,2},
	{3,2,1,0},
};

u8 constellation_mapping_table[2][2] = 
{
	{1,0},
	{3,2}
};




#endif
