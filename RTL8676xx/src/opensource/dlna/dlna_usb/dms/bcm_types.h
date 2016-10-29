#ifndef _BCM_TYPES_H_
#define _BCM_TYPES_H_

#include "upnp.h"

#ifdef WIN32
#include <sys\types.h>
#endif

#ifndef BOOL
typedef int    BOOL;
#endif

#ifndef TRUE
#define TRUE        1
#endif

#ifndef FALSE
#define FALSE        0
#endif

#ifndef off_t
//typedef unsigned long off_t;

#endif

#ifndef NULL
#define NULL  0
#endif

/* These status values are dependent on The framework 
 * Our core will use BC_UPNP values and the UPNP_E_XXX
 * values will come from the framework. If the UPNP_E_XX
 * are defined with some other status values we will 
 * redefine them using those.
 */
typedef unsigned int BC_STATUS;

#ifdef UPNP_E_SUCCESS
#define BC_UPNP_E_SUCCESS                             UPNP_E_SUCCESS                             
#define BC_UPNP_E_INVALID_ACTION                     UPNP_E_INVALID_ACTION                     
#define BC_UPNP_E_INVALID_ARG                         UPNP_E_INVALID_ARG                         
#define BC_UPNP_E_ACTION_FAILED                     UPNP_E_ACTION_FAILED                     
#define BC_UPNP_E_ARGUMENT_VALUE_INVALID             UPNP_E_ARGUMENT_VALUE_INVALID             
#define BC_UPNP_E_ARGUMENT_VALUE_OUT_OF_RANGE         UPNP_E_ARGUMENT_VALUE_OUT_OF_RANGE         
#define BC_UPNP_E_OPTIONAL_ACTION_NOT_IMPLEMENTED    UPNP_E_OPTIONAL_ACTION_NOT_IMPLEMENTED  
#define BC_UPNP_E_OUT_OF_MEMORY                     UPNP_E_OUT_OF_MEMORY                     
#define BC_UPNP_E_HUMAN_INTERVENTION_REQUIRED         UPNP_E_HUMAN_INTERVENTION_REQUIRED         
#define BC_UPNP_E_STRING_ARGUMENT_TOO_LONG             UPNP_E_STRING_ARGUMENT_TOO_LONG         
#define BC_UPNP_E_ACTION_NOT_AUTHORIZED             UPNP_E_ACTION_NOT_AUTHORIZED             
#define BC_UPNP_E_SIGNATURE_FAILURE                 UPNP_E_SIGNATURE_FAILURE                 
#define BC_UPNP_E_SIGNATURE_MISSING                 UPNP_E_SIGNATURE_MISSING                 
#define BC_UPNP_E_NOT_ENCRYPTED                     UPNP_E_NOT_ENCRYPTED                     
#define BC_UPNP_E_INVALID_SEQUENCE                     UPNP_E_INVALID_SEQUENCE                 
#define BC_UPNP_E_INVALID_CONTROL_URL                 UPNP_E_INVALID_CONTROL_URL                 
#define BC_UPNP_E_NO_SUCH_SESSION                     UPNP_E_NO_SUCH_SESSION                     
#else
#error  "Status Values Are Not Defined"
#endif

typedef int SINT32;

SINT32 cmsUtl_strcmp(const char *s1, const char *s2);
SINT32 cmsUtl_strcasecmp(const char *s1, const char *s2);
SINT32 cmsUtl_strncmp(const char *s1, const char *s2, SINT32 n); 
SINT32 cmsUtl_strncasecmp(const char *s1, const char *s2, SINT32 n); 
char *cmsUtl_strstr(const char *s1, const char *s2);
char *cmsUtl_strncpy(char *dest, const char *src, SINT32 dlen);
SINT32 cmsUtl_strlen(const char *src);
char *cmsUtl_strcat(char *dest, const char *src);
char *cmsUtl_strcpy(char *dest, const char *src);

#endif



