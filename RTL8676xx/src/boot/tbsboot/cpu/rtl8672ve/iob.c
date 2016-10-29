/*
 * iob.c
 *
 * This file is for buffer management of SerComm TCP/IP
 * it a test version
 *
 * Last Revised: 11/8/2003
 * History:
 *      11/8/2003 Casey   Created.
 *
 */

#include "iob.h"
//#include "libc.h"

// type definitions
//
#ifndef uint32

#define uint8							unsigned char
#define uint16							unsigned short
#define uint32							unsigned int
#define int8							char
#define int16							short
#define int32							int

#endif	// uint32

extern uint32 intLock(void);
extern void intUnlock(uint32);
#define splimp()		0
#define splx(s)			

#define UNCACHE_MASK	0x20000000
#define IOB_POOL_SZ		sizeof(IOB)*(NUM_OF_IOB)

static IOB		*iob;
static char		cIobBlk[IOB_POOL_SZ];
static char		*iobBlk;		/* point to non-cache area */
static IOB_POOL	iobFreePool;

/*
 * Buffer management
 */

int initIOB(void)
{
int i;

	iobBlk = (char*)((unsigned long)cIobBlk | UNCACHE_MASK);

	memset(iobBlk, 0, sizeof(cIobBlk));

    iobFreePool.count = NUM_OF_IOB;

    /* init free buffer */
    iob = (IOB *)iobBlk;

    for(i=0; i<NUM_OF_IOB-1; i++) {
        IOB_NEXT(&(iob[i])) = (char *)&(iob[i+1]);
    }
    IOB_NEXT(&(iob[NUM_OF_IOB-1])) = 0;
    iobFreePool.head = (char *)&(iob[0]);
        
    return 0;
}

char *IOBGetHdr(char *mp)
{
    unsigned long   p = (unsigned long)mp;
    int             idx;

	/* Check iob validition */
    if ((mp < &iobBlk[0]) || (mp >= &iobBlk[IOB_POOL_SZ])) {
		/* out of IOB pool */
    	printf("IOBGetHdr> ptr %x invalid, should be %x < ptr < %x\n\r", (uint32)mp, (uint32)&iobBlk[0], (uint32)&iobBlk[IOB_POOL_SZ]);
    	return 0;
    }
    
    idx = (p - (unsigned long)iobBlk) / sizeof(IOB);
    p = (unsigned long)&iob[idx];
	return ((char *)p);
}

  
  
int freeIOB(char *mp)
{
int s;

    if( !mp ) {
		printf("freeIOB> null ptr!\n\r");
        return -1;
    }
    if(!(mp = IOBGetHdr(mp))) {
		printf("freeIOB> ptr invalid!\n\r");
		s = splimp();
        while (1) {}
    }

	s = splimp();
    if ( IOB_NEXT(mp) ) {
    	splx(s);
		printf("freeIOB> Try to free a free IOB %x\n\r", (uint32)mp);
        return -1;
    }

	IOB_NEXT(mp) = iobFreePool.head;
    iobFreePool.head = mp;
    iobFreePool.count++;
    
	splx(s);
	return 0;
}
  

char *getIOB(void)
{
char    *mp = 0;
int		s;

	if(!iobFreePool.count)
		return 0;
		
    s = splimp();
    
	mp = iobFreePool.head;
	if ((mp < &iobBlk[0]) || (mp >= &iobBlk[IOB_POOL_SZ])) {
		printf("iob: head ptr %x invalid\n\r", (uint32)mp);
		while(1) {}
	}
	iobFreePool.head = IOB_NEXT(iobFreePool.head);
	iobFreePool.count--;

	IOB_PKT_OFFSET(mp) = IOB_OFFSET;
    IOB_PKT_LEN(mp) = 0;
    IOB_NEXT(mp) = 0;		/* allocated */

    splx(s);

    return mp;
}

int IOBCopy(char *src, char*dst)
{
int *src32, *dst32;
int	i;
	if(!(src32 = (int*)IOBGetHdr(src)))
		return -1;
	if(!(dst32 = (int*)IOBGetHdr(dst)))
		return -1;
	for(i=0; i<sizeof(IOB)/4; i++)
		dst32[i] = src32[i];
	return 0;
}

	

void IOBStatus(void)
{
	printf("num of free iob = %d\n\r", iobFreePool.count);
}
