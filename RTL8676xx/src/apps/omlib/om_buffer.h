#ifndef  __OM_BUFFER_H__
#define  __OM_BUFFER_H__

typedef struct tag_ST_OM_BLOCK
{
    char    *pBuffer;
    int        nSize;
    int     nStrLen;
}ST_OM_BLOCK;


#define OM_BUFF_NEW(n)   OM_BufferNew(n, __FILE__, __LINE__)
#define OM_BUFF_FREE(p)  {OM_BufferFree(p); p = NULL;}
/* 创建一个BUFFER，尺寸可以为0 */
ST_OM_BLOCK    *OM_BufferNew(int nSize, char *szFile, int nLine);

/* 复制一个buffer */
ST_OM_BLOCK    *OM_BufferCopy(ST_OM_BLOCK *pBuffer);

/* 释放buffer */
void            OM_BufferFree(ST_OM_BLOCK *pBuffer);

/* 将buf的长度设置为最少有min_size指定的范围 */
int                OM_BufferMinSize(ST_OM_BLOCK *pBuffer, int min_size);

/* 插入一段文本到buf中 */
int                OM_BufferInsTxt(ST_OM_BLOCK *pBuffer, int pos,
                        const char *new_text, int new_text_size, int eat);

#endif /* __OM_BUFFER_H__ */
