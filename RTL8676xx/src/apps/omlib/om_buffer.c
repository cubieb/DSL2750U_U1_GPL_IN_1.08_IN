#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "om_buffer.h"
#include "../include/om.h"


/* Make a new buffer. nSize can be 0 */
ST_OM_BLOCK    *OM_BufferNew(int nSize, char *szFile, int nLine)
{
    ST_OM_BLOCK *pBuf;

    pBuf=(ST_OM_BLOCK *)malloc(sizeof(ST_OM_BLOCK));
    if(!pBuf)    return NULL;

    memset(pBuf, 0, sizeof(ST_OM_BLOCK));

    if(nSize){
        pBuf->pBuffer=(char*)malloc(nSize);
        if(!pBuf->pBuffer){
            free(pBuf);
            return NULL;
        }
        pBuf->nSize=nSize;
        pBuf->pBuffer[0] = '\0';
    }
    return pBuf;
}

/* Copy an existing buffer */
ST_OM_BLOCK    *OM_BufferCopy(ST_OM_BLOCK *pBuffer)
{
    ST_OM_BLOCK *pBuf;

    if(!pBuffer)    return NULL;

    pBuf=OM_BUFF_NEW(0);
    if(pBuf){
        pBuf->pBuffer=strdup(pBuffer->pBuffer);
        pBuf->nStrLen=strlen(pBuffer->pBuffer);
        pBuf->nSize=pBuf->nStrLen+1;
    }
    return pBuf;
}

/* free a buffer */
void    OM_BufferFree(ST_OM_BLOCK *pBuffer)
{
    if(!pBuffer)    return;
    if(pBuffer->pBuffer)
        free(pBuffer->pBuffer);
    free(pBuffer);
}

/* size the buffer to a minimum size. This allows mem-copies into the buffer */
int    OM_BufferMinSize(ST_OM_BLOCK *pBuffer, int min_size)
{
    char    *newbuf;
    int    newsize;

    if(!pBuffer || ( min_size < pBuffer->nSize))
        return 0;

    /* We pad the buffer size to be large enough to handle an insert
       or two */
    newsize=min_size + 100;
    newbuf = (char *)malloc(newsize);
    if(!newbuf)
        return -1;    /* Out of memory */

    *newbuf=0;
    if(pBuffer->pBuffer){
        /* Copy the used pBuffer + terminating NULL */
        memmove(newbuf, pBuffer->pBuffer, pBuffer->nStrLen+1);
        free(pBuffer->pBuffer);
    }
    pBuffer->pBuffer=newbuf;
    pBuffer->nSize=newsize;
    return 0;
}

/* Inserts text into a dynamic buffer. The buffer will be resized if necessary,
   and the buffer size updated to reflect this. The pos arg gives the initial
   position to insert. 0 for begining of string, count for in the middle of
   the string, and -1 for the end of the string. use eat to delete characters
   when doing the insert */
int    OM_BufferInsTxt(ST_OM_BLOCK *pBuffer, int pos,
            const char *new_text, int new_text_size, int eat)
{
    char    *p;

    if( !pBuffer )
        return TBS_PARAM_ERR;    /* don't have the necessary params */

    if(pBuffer->nStrLen < eat)
        eat = pBuffer->nStrLen;

    if(OM_BufferMinSize(pBuffer, pBuffer->nStrLen + new_text_size - eat) < 0){
        /* Out of memory */
        return TBS_OUT_OF_MEM;
    }

    /* If the pos is larger then the position, just append it */
    if(pos > pBuffer->nStrLen)
        pos = -1;
    if(!new_text)
        new_text_size=0;

    /* We now have a buffer large enough. Lets do it */
    if(pos<0){    /* append to the end */
        p = pBuffer->pBuffer + pBuffer->nStrLen - eat;

        if(new_text && new_text_size)
            memmove(p, new_text, new_text_size);
        pBuffer->nStrLen += new_text_size - eat;
        pBuffer->pBuffer[pBuffer->nStrLen] = 0;        /* NULL terminate the string */
        return TBS_SUCCESS;
    }

    /* Move the tail to create a gap */
    if(pBuffer->nStrLen - pos < eat)
        eat = pBuffer->nStrLen - pos;
    memmove( pBuffer->pBuffer + pos + new_text_size, pBuffer->pBuffer + pos + eat,
            pBuffer->nStrLen - pos - eat + 1);

    /* Move in the new stuff */
    if(new_text)
        memmove( pBuffer->pBuffer + pos, new_text, new_text_size);
    pBuffer->nStrLen += new_text_size - eat;
    pBuffer->pBuffer[pBuffer->nStrLen] = '\0';

    return TBS_SUCCESS;
}

