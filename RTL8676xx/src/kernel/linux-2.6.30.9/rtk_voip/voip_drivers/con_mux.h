#ifndef __CON_MUX_H__
#define __CON_MUX_H__

#include "voip_types.h"
#include "con_register.h"

// bus fifo
//extern void bus_fifo_set_tx_mute_cch( uint32 cch, int enable );
//extern void bus_fifo_set_rx_mute_cch( uint32 cch, int enable );
//extern void bus_fifo_clean_tx_cch( uint32 cch );
//extern void bus_fifo_clean_rx_cch( uint32 cch );

// enable a control channel, so all binding channel will be enabled too. 
extern int con_enable_cch( uint32 cch, int enable );

// enable CPC gen 
extern void SLIC_CPC_Gen_cch( uint32 cch, unsigned int time_in_ms_of_cpc_signal );

// access snd type 
extern snd_type_t get_snd_type_cch( uint32 cch );

// pluse dail 
extern void DAA_PulseGenProcess_msec(void);
extern void DAA_PulseGenKill_cch(unsigned int cch);

#endif /* __CON_MUX_H__ */

