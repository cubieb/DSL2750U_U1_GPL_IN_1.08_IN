#ifndef _DEVICE_H_
#define _DEVICE_H_

extern PService init_service(PServiceTemplate svctmpl, PDevice pdev, unsigned int dynsvcidx);
extern void destroy_service(PService);
void device_devicelist(PDevice pdev, UFILE *up);
void device_servicelist(PDevice pdev, UFILE *up);

#endif /* _DEVICE_H_ */
