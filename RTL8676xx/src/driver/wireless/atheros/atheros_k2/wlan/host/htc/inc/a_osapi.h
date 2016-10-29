#ifndef _A_OSAPI_H_
#define _A_OSAPI_H_

#define __ATTRIB_PACK
#define __ATTRIB_PRINTF
#define __ATTRIB_NORETURN

#ifndef INLINE
#define INLINE	adf_os_inline
#endif

#define PREPACK
#define POSTPACK                __ATTRIB_PACK

#endif	/* !_A_OSAPI_H_ */
