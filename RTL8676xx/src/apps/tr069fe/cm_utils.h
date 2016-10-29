#ifndef	__CM_UTILS_H__
#define	__CM_UTILS_H__

#include <stdio.h>
#include <stdlib.h>

#ifndef __USE_GNU
#define __USE_GNU
#endif

#include <string.h>

#include "tbsutil.h"


#define	CLEAR_TEXT(var)			\
	{	if(var) free(var);		\
		var=NULL;				\
	}

#define	SET_TEXT(var, string)			\
	{	if(var) free(var);		\
		var=NULL;			\
		if(string) var=strdup(string);	\
	}

#define	SET_N_TEXT(var, string, size)		\
	{	if(var) free(var);		\
		var=NULL;			\
		if(string) var=strndup(string, size);	\
	}

#define	SET_INT(var, num)			\
	{	if(var) free(var);		\
		asprintf(&var, "%d", num);	\
	}

#define	SET_MEM(var, string, size)	\
      {	if(var) free(var);		\
		var = NULL;	\
		if(size > 0)	\
		{	\
			var = (char*)malloc(size + 1);	\
			memset(var, 0, size+1);	\
			memcpy(var, string, size);		\
		}	\
      }

#define TR069_APPEND_LIST(head, tail, node) \
{ \
    if (NULL != tail) \
    { \
        tail->Next = node; \
    } \
    else \
    { \
        head = node; \
    } \
    tail = node; \
}


#endif	//_CM_UTILS_H__
