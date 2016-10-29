/*
    $Copyright Open Broadcom Corporation$   
*/

#ifndef _uio_h_
#define _uio_h_

struct _ufile;
typedef struct _ufile UFILE;

UFILE *usopen(char *str, int maxlen);
UFILE *ufopen(const char *fname);
UFILE *udopen(int fd);
int utell(UFILE *up);
char *ubuffer(UFILE *up);
void uclose(UFILE *up);
int uprintf(UFILE *up, char *fmt, ...);
int ufileno(UFILE *up);
void uflush(UFILE *up);

#endif /* uio_h_ */
