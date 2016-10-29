/*
 * iob.h
 *
 * This file is for buffer management of SerComm TCP/IP
 *
 * Last Revised: 11/8/2003
 * History:
 *      11/8/2003 Casey   Created.
 *
 */

#ifndef _IOB_H_
#define _IOB_H_

#ifndef NUM_OF_IOB
#define NUM_OF_IOB		64     /* number of buffer */
#endif /* NUM_OF_BUF */


#define IOB_OFFSET  14      /* space reserved */
#define IOB_HDR_SZ	8
#define IOB_DATASZ	2048-IOB_HDR_SZ    /* buffer size */

/* buffer structure */ 
typedef struct {
	void * psNextIOB;
	unsigned short	dwLength;
	unsigned short	dwOffset;
	unsigned char dbData[IOB_DATASZ];
} IOB;


/* free buffer pool */
typedef struct {
    char    		*head;
    unsigned int	count;
} IOB_POOL;


/* Buffer management */
#define IOB_PKT_PTR(p)		((char *)p+IOB_HDR_SZ+((IOB *)p)->dwOffset)
#define IOB_PKT_OFFSET(p)	(((IOB *)p)->dwOffset)
#define IOB_PKT_LEN(p)		(((IOB *)p)->dwLength)
#define IOB_NEXT(p)			(((IOB *)p)->psNextIOB)
  

/* exported function prototypes */
extern int initIOB(void);
extern char *IOBGetHdr(char *mp);
extern char *getIOB(void);
extern int freeIOB(char *iob);
extern int IOBCopy(char *src, char *dst);

/* for debug */
extern void IOBStatus(void);

#endif /* _IOB_H_ */

