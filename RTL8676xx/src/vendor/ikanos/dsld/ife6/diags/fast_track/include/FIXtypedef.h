/*
   ITU-T G.729A Speech Coder    ANSI-C Source Code
   Version 1.1    Last modified: September 1996

   Copyright (c) 1996,
   AT&T, France Telecom, NTT, Universite de Sherbrooke
   All rights reserved.
*/

/* WARNING: Make sure that the proper flags are defined for your system */

/*
   Types definitions
*/

#ifndef TYPEDEF_H
#define TYPEDEF_H

#define MAX_32 (Word32)0x7fffffffL
#define MIN_32 (Word32)0x80000000L

#define MAX_16 (Word16)0x7fff
#define MIN_16 (Word16)0x8000

typedef	long	Fast32;
typedef	int		Fast16;

typedef	long	Word32;
typedef	short	Word16;
typedef	char	Word8;
typedef	unsigned long	uWord32;
typedef	unsigned short	uWord16;
typedef	unsigned char	uWord8;
typedef	int		Flag;

/*Commented By SACHIN DESAI */
/*typedef unsigned short	uint;*/	/* unsigned integer */

#ifndef UNIX
typedef unsigned int	uint;	/* unsigned integer */
typedef unsigned long	ulong;	/* unsigned long integer */
#endif

typedef short		tint;
typedef tint		bool;	/* logical variable */ 

#ifndef UNIX
#ifdef	WIN32
typedef	__int64		intAccu; 
#else
//typedef __int40		intAccu; /* compile on TXSELICA */
#endif
#endif

/*
#define	TRUE			1
*/

#define	FALSE			0

#endif
