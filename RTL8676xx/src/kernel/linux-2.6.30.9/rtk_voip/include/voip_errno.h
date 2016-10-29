#ifndef __VOIP_ERRNO_H__
#define __VOIP_ERRNO_H__

// This error number extend linux standard one, so we give 1024 
// to avoid conflict.  

#define EVOIP_BASE		1024

enum {
	EVOIP_RESEND_ERR = EVOIP_BASE,	// re-send 3 times fail 
	EVOIP_DUP_SEQ,		// receive duplicate seq 
};

#endif /* __VOIP_ERRNO_H__ */

