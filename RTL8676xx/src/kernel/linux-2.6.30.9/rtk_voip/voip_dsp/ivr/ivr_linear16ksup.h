#ifndef __IVR_LINEAR_16K_SUP_H__
#define __IVR_LINEAR_16K_SUP_H__

#define LINEAR_16K_IN_BUFFER_SIZE	128		/* 10ms * 128 = 1.28 seconds. (power of 2) */
#define LINEAR_16K_FRAME_SIZE		320

typedef struct {
	unsigned char in_buffer[ LINEAR_16K_FRAME_SIZE * LINEAR_16K_IN_BUFFER_SIZE ];
	unsigned int  in_buffer_ri;
	unsigned int  in_buffer_wi;
} ivr_play_linear_16k_t;

/* Initialize 16K coder when it is going to play */
extern void InitializeIvrLinear16kPlayer( unsigned int chid, ivr_play_linear_16k_t *pIvrPlay );

/* Add data to codec buffer. nCount is in unit of frame (320 bytes) */
/* return number of data is copied in unit of frame (320 bytes). */
extern unsigned int PutDataIntoIvrLinear16kPlayer( ivr_play_linear_16k_t *pIvrPlay, 
								   const unsigned char *pData, 
								   unsigned int nCount );

/* Run it to get 10ms data. */
/* Return 0 to indicate play done */
extern int RunIvrLinear16kPlayer( unsigned int chid, ivr_play_linear_16k_t *pIvrPlay, 
					 const Word16 **ppOut );

/* total play period in buffer */
extern unsigned int GetPredictionPeriodOfLinear16k( const ivr_play_linear_16k_t *pIvrPlay );

#endif /* __IVR_LINEAR_16K_SUP_H__ */

