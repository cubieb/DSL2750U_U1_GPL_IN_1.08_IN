/*
** Copyright (c) 2007 by Silicon Laboratories
**
** $Id: si_voice_datatypes.h,v 1.1 2011/03/15 08:13:29 yachang Exp $
**
** si_voice_datatypes.h
** ProSLIC datatypes file
**
** Author(s): 
** laj
**
** Distributed by: 
** Silicon Laboratories, Inc
**
** This file contains proprietary information.	 
** No dissemination allowed without prior written permission from
** Silicon Laboratories, Inc.
**
** File Description:
** This is the header file that contains
** type definitions for the data types
** used in the demonstration code.
**
** Dependancies:
** 
**
*/
#ifndef DATATYPES_H
#define DATATYPES_H

#include "../../../include/voip_types.h"

#ifndef TRUE
#define TRUE (1)
#endif
#ifndef FALSE
#define FALSE (0)
#endif
#define BOOLEAN int

#ifndef NULL
#define NULL ((void *) 0)
#endif

#if 0
typedef char			int8;
typedef short int		int16;
typedef long			int32;
#endif

typedef unsigned char		uInt8;
typedef uInt8			uChar;
typedef unsigned short int	uInt16;
typedef unsigned long		uInt32;

/*
** RAM data 
*/

#define ramData uInt32

#endif
