#ifndef __TR069ERRNO_H__
#define __TR069ERRNO_H__

/* -- INCLUDES -- */
#include "cfg_interface.h"

/* -- GLOBALS & CONSTANTS -- */
#define 	ERRMNS 	9000
#define 	ERRRD 	9001
#define 	ERRIE 	9002
#define 	ERRIA 	9003
#define 	ERRRE 	9004

// Added by Ranganath.S on 11/07/05
#define 	ERRIPN	9005
#define 	ERRIPT	9006
#define 	ERRIPV	9007
#define 	ERRNWP	9008
#define 	ERRNRR	9009
#define 	ERRDE	9010
#define 	ERRUE	9011
#define 	ERRADE	9012
#define 	ERRSRR	8005


/* -- FUNCTION PROTOTYPES -- */
char *getFaultString (int errNum);

#endif
