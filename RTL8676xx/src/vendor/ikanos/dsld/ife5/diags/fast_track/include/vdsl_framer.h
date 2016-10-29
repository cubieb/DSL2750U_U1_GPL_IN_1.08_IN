#ifndef VDSL_FRAMER_H
#define VDSL_FRAMER_H

#define	MAX_ni			164*10 // @4k symbol rate, it corresponds to 10496000 bit/sec data rate
#define	MAX_N			255 // max rs codeword length
#define Pct_Redundancy	8	// percent of RS redundancy

typedef struct
{
	int				Hi; // # packets, currently fixed at 138
	int				Ui; // # integer bytes in one packet
	int				Zi; // # dummy bytes for payload traffic adaptation
	int				Ei; // # bytes in the octate to be inserted 
	int				Ni; // # bytes output by RS coder
	int				Ki; // # infomation bytes.
	int				Ri; // # bytes in RS dummy
	int				Pi; //
	int				ni;	// bit rate in terms of multiple of 64kbit/sec
	int				m;
	int				num_code_word_f;
} VDSL_Framer_Param;

extern void init_vdsl_tc(VDSL_Framer_Param *fast_path_cfg, int Lce);
extern int 	setup_fast_path_param(VDSL_Framer_Param *fast_path_cfg, int pf);

#endif