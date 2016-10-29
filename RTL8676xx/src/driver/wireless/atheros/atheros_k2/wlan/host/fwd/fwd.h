#ifndef __FWD_MAGPIE_
#define __FWD_MAGPIE_

#include <athdefs.h>
#include <hif.h>

/* fwd specific data - begin */

#define FWD_MAX_CHUNK           1024
#define FWD_MAX_TRIES           1000
#define FWD_TIMEOUT_MSECS       5000

/*
 * XXX Pack 'em
 */
typedef struct {
  a_uint16_t  more_data;     /* Is there more data? */
  a_uint16_t  len;           /* Len this segment    */
  a_uint32_t  offset;        /* Offset in the file  */
} fwd_cmd_t;

/*
 * No enums across platforms
 */
#define FWD_RSP_ACK     0x1
#define FWD_RSP_SUCCESS 0x2
#define FWD_RSP_FAILED  0x3

typedef struct {
    a_uint32_t   rsp;       /* ACK/SUCCESS/FAILURE */ 
    a_uint32_t   offset;    /* rsp for this ofset  */
}fwd_rsp_t;

typedef struct {
 HIF_HANDLE      hif_handle;     /* which i/f              */
 a_uint8_t       tx_pipe;        /* default tx pipe        */
 a_uint8_t       rx_pipe;        /* default rx pipe        */
 adf_os_timer_t  tmr;            /* retry timer            */
 a_uint8_t      *image;         /* ptr to the image       */
 a_uint32_t      target_upload_addr;
 a_uint32_t      size;           /* total image size       */
 a_uint32_t      offset;         /* current offset         */
 a_uint16_t       chunk_retries;  /* Retries per chunk      */
 a_uint16_t       ntries;         /* num of download retries*/
} fwd_softc_t;






#endif
