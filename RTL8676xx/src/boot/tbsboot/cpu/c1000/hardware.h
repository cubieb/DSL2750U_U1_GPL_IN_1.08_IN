#ifndef __HARDWARE_H__
#define __HARDWARE_H__

#include <config.h>

#if defined(CONFIG_COMCERTO_1000)
#include "comcerto_1000.h"
#else
#error no architecture selected!!!
#endif


#define CSE_BOOT	4
#endif
