#ifndef __RT2008_ETH_H__
#define __RT2008_ETH_H__

#define NUM_RX_DESC 20
#define NUM_TX_DESC 20

//   For loopback test

typedef struct _BUFFER_ELEM_    BUFFER_ELEM;

struct _BUFFER_ELEM_
{
	int tx_idx;
    unsigned char *pbuf;
    BUFFER_ELEM       *next;
    
    
};

typedef struct _VALID_BUFFER_STRUCT_    VALID_BUFFER_STRUCT;

struct _VALID_BUFFER_STRUCT_
{
    BUFFER_ELEM    *head;
    BUFFER_ELEM    *tail;	
};

u32 mii_mgr_read(u32 phy_addr, u32 phy_register, u32 *read_data);
u32 mii_mgr_write(u32 phy_addr, u32 phy_register, u32 write_data);

BUFFER_ELEM *rt2880_free_buf_entry_dequeue(VALID_BUFFER_STRUCT *hdr);

extern VALID_BUFFER_STRUCT  rt2880_free_buf_list;
extern VALID_BUFFER_STRUCT  rt2880_busing_buf_list;
//extern static BUFFER_ELEM   rt2880_free_buf[PKTBUFSRX];


#endif
