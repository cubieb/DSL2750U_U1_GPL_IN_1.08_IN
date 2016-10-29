
#include "xdsl_rmc.h"
#include "bme.h"
#include "bme_app.h"
#include "bme_dma.h"
#include "mem.h"
#include "afe.h" 

extern int	host_mode_flag;
							    
// channel ranges from 0..31						
// baseAddress is one of the PIO E0xxxxxx addresses
// numegs = number of regs to read. Assumes that data[] has enuf room to hold
void 	writePIO_Regs(int *regAddress, int numRegs, int *data);
void 	readPIO_Regs(int *RegAddress, int numRegs, int *data);
void 	writePIO_SingleRegChannel(int *baseRegAddress, int channel, int  data);
void 	writePIO_short(u16 *addr, u16 val);
void	initHostIO(int endianType, int baseAddr);
u16 	readPIO_short(u16 *addr);
int 	readPIO_SingleRegChannel(int *baseRegAddress, int channel);
int 	getBMEAddress (int dst_address, int offset, int *isFifoDst, int port, int *address);
int 	writeBMEMemories (char *src_address, char *dst_address, int offset, int len, int port, int write_mode, int dma_channel, int dma_event_flag, void *sourceSavedMemPtr);
int 	readBMEMemories (char *src_address, char *dst_address, int offset, int len, int port, int write_mode, int dma_channel, int dma_event_flag, void *sourceSavedMemPtr);
int	hostPIO_read(int address);
int	hostPIO_write(int address, int data);
int	hostWriteBMEMemories(char *dst, char *src, int len, int write_mode, int fifo_flag, int chan_num);
int	hostReadBMEMemories(char *dst, char *src, int len, int read_mode, int fifo_flag, int chan_num);
int     writeBMEMemories_diag(char *src_address, char *dst_address, int offset, int len, int port, int write_mode, int dma_channel, int dma_event_flag);
int     readBMEMemories_diag (char *src_address, char *dst_address, int offset, int len, int port, int read_mode, int dma_channel, int dma_event_flag);
void    train_up_gain_correction(unsigned short *train_up, unsigned short *precomp_up, unsigned char *bit_size, 
							  unsigned short *g, unsigned short *g_upbo, unsigned short length, 
							  int status_flag, int pilot_tone);
void set_ZFIFO_wrt_ptr(int wrt_pos, int ch_num);
void init_vbus_all_chan_4k(int ce_enable);
void set_zero_ce();
void downloaddmc ();
void downloadrmc ();
void downloadfmc ();
void tx_chg_pilot_tone(int ptone_index); 
void tx_set_gi(u16 *tx_gi, int length);
void write_XF_Fifo(int ch_num, u8 *xdata, int len);
void send_periodicData();
void tx_dmt(int line_num, char *cfg_file, int ce_enable);
void read_ibfifo_status(int line_num, int *fifo_start_addr, int *size);
int ibfifo_data_avail(int line_num, int fifo_size);
void clear_bfifo(int line_num);
int read_afifo_status(int line_num, int *fifo_start_addr, int *size);
int afifo_data_avail(int line_num, int fifo_size);
void programDMAController (DMA_DATABASE *p, int channel);
void enable_dmc();
void enable_rmc();
void enable_fmc();

extern void iposDelay (void);



