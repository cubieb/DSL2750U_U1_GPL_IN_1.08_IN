/*--------------------------------
		Includes
---------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <fcntl.h>
#include <getopt.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <stdint.h>


/*--------------------------------
		Definitions
---------------------------------*/

#if 0
#define reg(address)	(*(volatile uint32 *)((uint32)address))
#define	REG32(reg)		(*(volatile uint32 *)(reg))
#define	REG16(reg)		(*(volatile uint16 *)(reg))
#define	REG8(reg)		(*(volatile uint8 *)(reg))
#endif

