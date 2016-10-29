/**********************************************************************
 Copyright (c), 1991-2008, T&W ELECTRONICS(SHENTHEN) Co., Ltd.
 文件名称 : usblib_util.h
 文件描述 : 通用函数集成


 函数列表 :


 修订记录 :
          1 创建 : 雷達
            日期 : 2008-5-12
            描述 :

**********************************************************************/
#ifndef __USBLIB_UTIL_H__
#define __USBLIB_UTIL_H__

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>  /* open, create */
#include <unistd.h> /* read */
#include <ctype.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include "tbsutil.h"

/* radar */
/*
#ifndef SAFE_FREE
#define SAFE_FREE(x)    do{ if(x){ free(x); (x) = NULL; } }while(0)
#endif
*/
#ifndef SAFE_CLOSE
#define SAFE_CLOSE(fd)  do{ if(fd){ close(fd); fd = 0;} }while(0)
#endif

#ifndef xmalloc
// Die if we can't allocate and zero size bytes of memory.
static inline void *xmalloc(size_t sz)
{
    void *p = (void *)calloc(1, sz);
    if (!p){
        perror("malloc");
        exit(2);
    }
    return p;
}
#endif

#ifndef xzalloc
#define xzalloc(sz) xmalloc(sz)
#endif

/* for human reading */
#ifndef KSCALE
#define KSCALE(lb, lbs) (( (lb) * ((long long) (lbs)) + 1024/2 ) / 1024)
#endif

/* strip " ", "\n" */
static void inline strstrip(char *strbuf)
{
    int i;
    char *pstr = strbuf;
    if (NULL == pstr)
        return;
    for (i = strlen(pstr) -1; i >= 0; i--) {
        if ( (0x10 == pstr[i]) || (0x13 == pstr[i]) || isspace(pstr[i]) )
            pstr[i] = 0;
		else
	        break;
    }
}

/* append a "line" string with "\n" to filepath */
/* return zero for success, or return EBADF for failed */
/* file will be created if not exist */
static int inline fappendline(const char *filepath, char *line)
{
    int fd = 0;
    int nbytes = 0;
    int wbytes = 0;

    assert(filepath && line);
    fd = open(filepath, O_APPEND | O_WRONLY); // | O_SYNC
    if (fd < 0){
        /* perhaps file not exist */
        fd = open(filepath, O_CREAT | O_TRUNC | O_WRONLY);  // | O_SYNC);
        if (fd < 0){
            /* lumme, file can't be created?! */
            perror("open");
            return (EBADF);
        }
    }
    nbytes = strlen(line);
    if (nbytes){
        wbytes = write(fd, line, nbytes);
        if(wbytes != nbytes)
        {
            perror("write");
            return (EBADF);
        }
        
        wbytes = write(fd, "\n", strlen("\n")); /* "\n" is not always equal one byte */
        if(wbytes != strlen("\n"))
        {
            perror("write");
            return (EBADF);
        }
    }
    close(fd);
    return 0;
}

#endif  /* __USBLIB_UTIL_H__ */
