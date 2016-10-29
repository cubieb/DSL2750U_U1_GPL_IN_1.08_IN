#ifndef _PKTLOG_FMT_H_
#define _PKTLOG_FMT_H_

#define PKTLOG_HDR 0
#define PKTLOG_PAYLOAD 1
#define PKTLOG_TAG "ATH_PKTLOG: " 

#define PKTLOG_DEFAULT_BUFSIZE (512 * 1024)
#define PKTLOG_MAX_BYTES_TO_READ 4096

struct ath_pktlog_bufhdr {
    a_int32_t magic_num;  /* Used by post processing scripts */
    a_int32_t version;    /* Set to CUR_PKTLOG_VER */
};


typedef struct ath_pktlog_buf{
    struct ath_pktlog_bufhdr bufhdr;
    a_uint8_t log_data[0];
} ath_pktlog_buf_t;


typedef struct ath_pktlog_info {
    ath_pktlog_buf_t *buf;
    a_int32_t wr_offset;
    a_int32_t rd_offset;
    a_int32_t fold_offset;
    a_int32_t log_state;
    adf_os_size_t buf_size;           /* Size of buffer in bytes */
    a_uint32_t read_bufhdr;
    a_uint32_t payload_size;
    a_uint32_t read_flag;
#ifndef MAGPIE_HOST_SCHEDULING 
    adf_os_bh_t pktlogtq;
#endif
} ath_pktlog_info_t;


struct ath_pktlog_hdr {
    u_int32_t flags;    /* See flags defined below */
    u_int16_t log_type; /* Type of log information foll this header */
    u_int16_t size;       /* Size of variable length log information in bytes */
    u_int32_t timestamp;
};

#define PKTLOG_MOV_RD_IDX(_rd_offset, _log_buf, _log_size)  \
    do { \
	if((_rd_offset + sizeof(struct ath_pktlog_hdr) + \
		    ((struct ath_pktlog_hdr *)((_log_buf)->log_data + \
					        (_rd_offset)))->size) <= _log_size) { \
	    _rd_offset = ((_rd_offset) + sizeof(struct ath_pktlog_hdr) + \
		    ((struct ath_pktlog_hdr *)((_log_buf)->log_data + \
					       (_rd_offset)))->size); \
	    } else { \
		_rd_offset = ((struct ath_pktlog_hdr *)((_log_buf)->log_data +  \
			    (_rd_offset)))->size;  \
		} \
	(_rd_offset) = (((_log_size) - (_rd_offset)) >= \
			sizeof(struct ath_pktlog_hdr)) ? _rd_offset:0;\
	} while(0)
	
	

#endif


