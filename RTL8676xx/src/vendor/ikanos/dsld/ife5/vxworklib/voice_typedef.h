#ifndef _VOICE_DRIVER_TYPEDEF_H
#define _VOICE_DRIVER_TYPEDEF_H

typedef char               INT8;
typedef short              INT16;
typedef int                INT32;
typedef unsigned short     UINT16;
typedef unsigned int       UINT32;
typedef unsigned char      UINT8;
typedef char               CHAR;
typedef void               VOID;
typedef unsigned long long UINT64;
typedef unsigned char      UCHAR;
typedef unsigned char      BOOL;
typedef unsigned int       UINT;
typedef int                INT;
#define SUCCESS  0 
#define FAILURE  -1
#define TRUE 1
#define FALSE 0
#define NULL 0
#define YES 	1
#define NO 	0
#define ERROR 	-1
#define WAIT    0x00000001
#define OK 	1
typedef short    Flag;
typedef short    Word16;
typedef long     Word32;
typedef unsigned short  uWord16;
typedef unsigned long   uWord32;

#define Abs(a)  ( ((a)>=0) ? (a) :(-(a)) )
#define Mul(a,b) (((int) (a) * (int) (b)) << 1)
#define Round(a) ((Word16)(((a)+0x00008000L)>>16))
#define RoundM(a,b) ( (Word16) ( ( ( (Word32)(a) * (Word32)(b) *2) + 0x00008000L) >>16))

#define Fix(a) ((Word16) ((a)>> 16))
#define Divide(a,b) ((((Word32)(a)<< 16) /(b)) >> 1)
#define TOLONG(lsb,msb) (Word32) (uWord16)(lsb) + (((Word32) (uWord16) (msb) ) < < 16)


#define RATE_LENGTH     30
#define BUTTERDLINELEN  4
#define BUTTERCOEFSLEN  5

#define LVL_47dBm       0x0040
#define LVL_43dBm       0x0064
#define LVL_0dBm        0x3800

#endif
