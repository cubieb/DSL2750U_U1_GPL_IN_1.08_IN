/*========================================================================
**                       BME Debug Utilities
**      bmestat.h - include file for status reporting fns of bme hardware 
**
**      Copyright (c) 2001, Ikanos Communications
**  This Ikanos Communication Inc. proprietary information must be
**  maintained in the strictest of confidence and may not be copied,
**  disclosed,or used without the express prior written consent of an
**  officer of Ikanos Communication Inc.
**=======================================================================*/


typedef struct { 
    u32  start; 
    u32  size; 
    u32  rPtr; 
    u32  wPtr;
    u32  used;
    u16   cfg;   // bits 2=rdAgent, 1=wrtAgent, 0=Valid; 
    u8   wrap;   // bits 1=rdWrap, 0=wrtWrap; 
    u8    err;   // bits 2=SrcDstErr, 1=InvErr, 0=UnderflowOrOverflowErr 
    u32  regA;   // hw regs A,B,C,D of that module 
    u32  regB; 
    u32  regC; 
    u32  regD; 
} BmeFifo_t; 

BmeFifo_t   XFfifo, XIfifo, TOfifo, YIfifo, Zfifo; 
BmeFifo_t   Afifo, DMfifo, BFfifo, BIfifo, CFfifo, CIfifo; 

// void BmeStatus( int chNum, char *moduleName, int dbgLevel); 
void BmeTxStatus( int chNum, int dbgLevel); 
void BmeRxStatus( int chNum, int dbgLevel); 
void btx( int chNum); 
void brx( int chNum); 

void BmeAfifoStatus( int chNum, int dbgLevel); 
void BmeBfifoStatus( int chNum, int dbgLevel); 
void BmeCfifoStatus( int chNum, int dbgLevel); 
void BmeDmcStatus( int chNum, int dbgLevel); 
void BmeDmcCmdLog( ); 
void BmeRdAfifoState( int chNum, BmeFifo_t *fifo); 
void BmeShowFifoStatus( int chNum, char *fifoName, BmeFifo_t *fifo); 
void BmeRdDMfifoState( int chNum, BmeFifo_t *fifo); 
void BmeRdBFfifoState( int chNum, BmeFifo_t *fifo); 
void BmeRdBIfifoState( int chNum, BmeFifo_t *fifo); 

void BmeXfifoStatus( int chNum, int dbgLevel); 
void BmeYfifoStatus( int chNum, int dbgLevel); 
void BmeZfifoStatus( int chNum, int dbgLevel); 
void BmeFmcStatus( int chNum, int dbgLevel); 
void BmeRdXFfifoState( int chNum, BmeFifo_t *fifo); 
void BmeRdYIfifoState( int chNum, BmeFifo_t *fifo); 
void BmeRdZfifoState( int chNum, BmeFifo_t *fifo); 

