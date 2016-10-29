#ifndef _HIF_INTERNAL_H_
#define _HIF_INTERNAL_H_

#include <adf_os_mem.h>
#include <adf_nbuf.h>

#include "athdefs.h"
#include "a_osapi.h"
#include "hif.h"

struct hif_device {
    HTC_CALLBACKS htcCallbacks;
};

typedef struct hif_device HIF_DEVICE;

#define GET_HIF_DEVICE_FROM_HANDLE(hnd) ((HIF_DEVICE *)(hnd))

#define HIF_INPROC_MAX_TXPIPES 1
#define HIF_INPROC_MAX_RXPIPES 1

#endif	/* !_HIF_INTERNAL_H_ */
