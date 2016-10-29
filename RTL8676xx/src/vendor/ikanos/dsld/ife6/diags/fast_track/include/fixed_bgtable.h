#ifndef _BGTABLE__H_
#define _BGTABLE__H_ 

#define MAX_BIT		16
#define ADJ_FACTOR	0

typedef struct {
	int		max_bit_limit;			// largest constellation size: user input
	int		min_bit_limit;
	int		tot_bit_capacity;
	int		tot_bit_to_carry;
	int		numBands;
	int		startIndex[4];
	int		stopIndex[4];
	short		pilot_tone_index;
	short	noise_margin;			// in dB: user input
	short	coding_gain;			// in dB: user input
	short	snr_table[MAX_BIT+2];
	short	*snr_val;
} tBG_TABLE;

#endif