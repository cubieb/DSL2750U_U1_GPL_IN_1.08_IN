#ifndef _MISCDRV_H_
#define _MISCDRV_H_

/* Structure that is the state information for the default credit distribution callback.
 * Drivers should instantiate (zero-init as well) this structure in their driver instance
 * and pass it as a context to the HTC credit distribution functions */
typedef struct _COMMON_CREDIT_STATE_INFO {
    a_uint32_t TotalAvailableCredits;      /* total credits in the system at startup */
    a_uint32_t CurrentFreeCredits;         /* credits available in the pool that have not been
                                            given out to endpoints */
    HTC_ENDPOINT_CREDIT_DIST *pLowestPriEpDist;  /* pointer to the lowest priority endpoint dist struct */
} COMMON_CREDIT_STATE_INFO;

A_STATUS host_setup_credit_dist(HTC_HANDLE HTCHandle, COMMON_CREDIT_STATE_INFO *pCredInfo);

#endif
