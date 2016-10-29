
#ifndef __SMI_H__
#define __SMI_H__

#include <types.h>

int smi_reset(u32 port, u32 pinRST);
int smi_init(u32 port, u32 pinSCK, u32 pinSDA);
int smi_read(u32 mAddrs, u32 *rData);
int smi_write(u32 mAddrs, u32 rData);

#endif /* __SMI_H__ */


