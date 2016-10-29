#ifndef _FRAMERCPU_H_
#define _FRAMERCPU_H_

#include "mem.h"

#define	MAX_NUM_CMD							16
#define	Q_WRAP_MASK							(MAX_NUM_CMD-1)
#define MAX_FRM_MSG 						MAX_NUM_CMD
#define SZ_FRAMER_CPU_ENTRY					4 										/*4 longs : (sizeof (BmeTxCtrlMsg_t)) 	*/
#define SZ_FRAMER_CPU_ENTRY_IN_BYTES		(SZ_FRAMER_CPU_ENTRY * (sizeof (int))) 	/* 4 longs : (sizeof (BmeTxCtrlMsg_t)) 	*/
#define MAX_RX_MSG 							MAX_NUM_CMD
#define SZ_RX_CPU_ENTRY						8 										/* 8 longs :  */
#define SZ_RX_CPU_ENTRY_IN_BYTES			(SZ_RX_CPU_ENTRY * (sizeof (int))) 
#define MAX_DEFRAMER_MSG 					MAX_NUM_CMD
#define SZ_DEFRAMER_CPU_ENTRY				4 										/* 4 longs : (sizeof (BmeTxCtrlMsg_t)) */
#define SZ_DEFRAMER_CPU_ENTRY_IN_BYTES		(SZ_DEFRAMER_CPU_ENTRY*(sizeof (int))) 	/* 4 longs : (sizeof (BmeTxCtrlMsg_t)) */
#define SZ_FRAMER_CPU_STATUS				64

typedef enum {DIRECT_WRITE_CMD_Q = 0,  BUFFERED_WRITE} CMD_Q_WRITE_MODE;

extern int	write_q_mode;

extern void	initCircBuffers();
extern void initCpuCmdQ(void);
extern int 	writeFramerCPU_CQ (int channel, int  *data);
extern int 	writeDeFramerCPU_CQ (int channel, int  *data);
extern int 	writeRxCPU_CQ (int channel, int  *data);
extern int 	flush_xframer_cmd_q(int channel);
#endif