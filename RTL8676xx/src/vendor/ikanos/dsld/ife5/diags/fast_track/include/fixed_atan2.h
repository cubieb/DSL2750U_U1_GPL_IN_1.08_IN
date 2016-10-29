#ifndef FIXED_ATAN
#define FIXED_ATAN

#define  FFT_TONES          (2048)
#define  PI_FOURTH          (8192)
#define  HALF               (16384)
#define  ATANC1             (unsigned short)(20857)  /* 0.318253  in q.16 */
#define  ATANC2             (unsigned short)(217)    /* 0.003314  in q.16 */
#define  ATANC3             (unsigned short)(8579)   /* 0.130908  in q.16 */
#define  ATANC4             (unsigned short)(4492)   /* 0.068542  in q.16 */
#define  ATANC5             (unsigned short)(600)    /* 0.009159  in q.16 */
#define  ONEDIVTWOPI        (unsigned short)(10430)  /* 1/(2pi)   in q.16 */

short calculate_delay(long real, long imag, short length);
short fixed_pt_atan2(short imag, short real);

#endif
