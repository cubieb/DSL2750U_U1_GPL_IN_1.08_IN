#ifndef __ATHEROS_SERIAL_H__
#define __ATHEROS_SERIAL_H__

#define HAL_WRITE_UINT32( _register_, _value_ ) \
        (*((volatile unsigned int *)(_register_)) = (_value_))
#define HAL_READ_UINT32( _register_, _value_ ) \
        ((_value_) = *((volatile unsigned int  *)(_register_)))
        
#define CYG_MACRO_START do {
#define CYG_MACRO_END   } while (0)

#define CYGARC_HAL_SAVE_GP()                    \
    CYG_MACRO_START                             \
    register unsigned int  __gp_save;            \
    asm volatile ( "move   %0,$28;"             \
                   ".extern _gp;"               \
                   "la     $gp,_gp;"            \
                     : "=r"(__gp_save))

#define CYGARC_HAL_RESTORE_GP()                                 \
    asm volatile ( "move   $gp,%0;" :: "r"(__gp_save) );        \
    CYG_MACRO_END

#define HAL_READ_UINT8( _register_, _value_ )           \
    CYG_MACRO_START                                     \
    ((_value_) = *((volatile unsigned int *)(_register_))); \
    CYG_MACRO_END

#define HAL_WRITE_UINT8( _register_, _value_ )          \
    CYG_MACRO_START                                     \
    (*((volatile unsigned int *)(_register_)) = (_value_)); \
    CYG_MACRO_END


#define CYGNUM_CALL_IF_SET_COMM_ID_QUERY_CURRENT -1
#define CYGNUM_CALL_IF_SET_COMM_ID_EMPTY         -2
#define CYGNUM_CALL_IF_SET_COMM_ID_MANGLER       -3
extern unsigned int hal_ar7100_sys_frequency(void);
int 
cyg_hal_plf_comms_init(void);
static int
cyg_hal_plf_serial_getc_nonblock(void* __ch_data, unsigned char* ch);
void
cyg_hal_plf_serial_putc(void* port, unsigned char __ch);
void
cyg_hal_plf_serial_init_channel(void* __ch_data);
static void
cyg_hal_plf_serial_set_baud(unsigned char * port, unsigned short baud_divisor);

#endif
