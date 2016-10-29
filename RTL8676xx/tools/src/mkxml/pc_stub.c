
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#ifdef _cplusplus
    #if _cplusplus
        extern "C" {
    #endif
#endif

int pc_app_item_get( void *data , char *item_name , unsigned short *len )
{
    long lFd = 0;
    struct stat stStat;
    unsigned long ulLen = *len;
    char *pcBuf = data;
    char acFileName[64];

    stStat.st_size = 0;

    sprintf(acFileName, "/etc/%s", item_name);
    lFd = open(acFileName, O_RDONLY);
    if (lFd < 0)
    {
        return -1;
    }

    (void)fstat(lFd, &stStat);
    if (ulLen < stStat.st_size)
    {
        *len = (unsigned short)stStat.st_size;
        return -1;
    }

    ulLen = (unsigned long)read(lFd, pcBuf, stStat.st_size);
    if ((unsigned long)(-1) == ulLen)
    {
        return -1;
    }

    return 0;
}


int pc_app_item_save( void *data , char *item_name ,unsigned short len )
{
    long lFd = 0;
    long lOffset = 0;
    char *pcBuf = data;
    unsigned long ulLen = len;
    char acFileName[64];

    sprintf(acFileName, "/etc/%s", item_name);
    lFd = open(acFileName, O_WRONLY | O_CREAT, 0666);
    if ( 0 > lFd )
    {
        return -1;
    }

    while (ulLen > 0)
    {
        lOffset = write(lFd, pcBuf, ulLen);
        if (lOffset == -1 )
        {
            close(lFd);

            return -1;
        }
        else if (lOffset < (long)ulLen )
        {
            pcBuf += lOffset;
        }
        else
        {
            ;
        }
        ulLen -= lOffset;
    }

    close(lFd);
    return 0;
}


void wl_api_reg(void * pstApiFunSet)
{
	;
}



#ifdef _cplusplus
    #if _cplusplus
        }
    #endif
#endif
