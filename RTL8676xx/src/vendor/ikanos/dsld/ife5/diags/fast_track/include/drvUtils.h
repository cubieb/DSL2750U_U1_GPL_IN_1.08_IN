extern void setXFIFOAccessBit (int stat, int channel);
extern int  read_afifo_status(int line_num, int *fifo_start_addr, int *size);
extern int  afifo_data_avail(int line_num, int fifo_size);
extern int  write_to_file(char *host, char *user, char *dirname, char *filename, char *buf, int  length);
extern int  read_from_file(char *host, char *user, char *dirname, char *filename, char *buf, int  length);
extern int 	read_zfifo_status(int line_num, int *fifo_start_addr, int *size);
extern int  zfifo_data_avail(int line_num, int fifo_size);

#ifndef VX_WORKS
#define abs(val)	(val>=0? val:-val)
// extern void	memset(u8 *, u8 val, int size);
#endif
