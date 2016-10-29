#ifndef CHF_UART_H
#define CHF_UART_H





/* TODO - undefined macros - to be defined! */
#define CHM_PP_PERIPH  (0xFF000000) 
#define CHM_PERIPH_DBELL_NP_BELLFROMPP  (0x0) 
#define CHM_PERIPH_TIMER1_VALUE  (0x0)


#define PPIO(_reg) ( *((volatile U32 *) \
                       (((U32) CHM_PP_PERIPH) + ((U32) (CHARM_##_reg)))))


/* +-----------------------------------------------------------------------+ */ 
/* |                                                                       | */
/* | CHARM CONSOLE UART BLOCK HARDWARE DEFINITIONS                         | */
/* |                                                                       | */
/* +-----------------------------------------------------------------------+ */ 

#define CHARM_UART                 (0x300) /* Console UART Block             */

/*** Registers in the Console UART Block (offsets from CHM_MAP_PERIPH) *******/

#define CHARM_UART_TX              (0x300) /* UART Tx (write)                */
#define CHARM_UART_RX              (0x300) /* UART Rx (read)                 */
#define CHARM_UART_CTRL            (0x304) /* UART control (write)           */
#define CHARM_UART_STAT            (0x304) /* UART status (read)             */
#define CHARM_UART_AUX             (0x308) /* UART auxiliary (read/write)    */
#define CHARM_UART_FIFO            (0x30C) /* UART fifo (read/write)         */

/*** UART receive register ***************************************************/

#define CHARM_UART_RX_VALID        (0x00100) /* Set if data is valid          */
#define CHARM_UART_RX_DATA         (0x000ff) /* Data bits                     */

/*** UART control/status register ********************************************/
#define CHARM_UART_CTRL_TIMEOUT_MASK (0xFF000000) /* rw RX timeout value           */
#define CHARM_UART_CTRL_TIMEOUT_SHIFT        (24) /* Position of  RX timeout field */

/* Break bit */


/* Baudrates */
#define CHARM_UART_CTRL_BAUD_MASK  (0x0003800) /*    All the baudrate bits       */
#define CHARM_UART_CTRL_FLEX_BAUD  (0x0002000) /* rw Fully programmable baudrate */
#define CHARM_UART_CTRL_TEST_BAUD  (0x0003800) /* rw program. baud from 8 MHz clk*/
#define CHARM_UART_CTRL_BAUD_9600  (0x0000000) /* rw  9600 baud                  */
#define CHARM_UART_CTRL_BAUD_19200 (0x0000800) /* rw 19200 baud                  */
#define CHARM_UART_CTRL_BAUD_31250 (0x0001000) /* rw 31250 baud                  */
#define CHARM_UART_CTRL_BAUD_38400 (0x0001800) /* rw 38400 baud                  */
                                      
/* Interrupts */                      
#define CHARM_UART_CTRL_TOINTEN    (0x0004000) /* rw Rx timeout irq on           */
#define CHARM_UART_CTRL_INTMASK    (0x0010014) /* rw Interrupt Mask              */
#define CHARM_UART_CTRL_INTENB     (0x0010014) /* rw Interrupts Enabled          */
#define CHARM_UART_CTRL_INTDIS     (0x0000000) /* rw Interrupts disabled         */
#define CHARM_UART_CTRL_BRINTENB   (0x0010000) /* rw Enable break interrupt      */
#define CHARM_UART_CTRL_RXINTENB   (0x0000010) /* rw Enable rx interrupt         */
#define CHARM_UART_CTRL_TXINTENB   (0x0000004) /* rw Enable tx interrupt         */
                                      
#define CHARM_UART_CTRL_GENBREAK   (0x0020000) /* rw Generate break condition    */
#define CHARM_UART_CTRL_BREAK      (0x0008000) /* w  Break seen (clear)          */
#define CHARM_UART_CTRL_LOOPBACK   (0x0000400) /* rw Loopback data               */
#define CHARM_UART_CTRL_UARTEN     (0x0000200) /* rw Enable UART (0 for reset)   */
#define CHARM_UART_CTRL_RXORUN     (0x0000020) /* w  Receiver overrun (clear)    */
                                      
#define CHARM_UART_STAT_BREAK      (0x0008000) /* rc Break seen                  */
#define CHARM_UART_STAT_RXORUN     (0x0000020) /* rc Receiver overrun (status)   */
#define CHARM_UART_STAT_RXNOTEMPTY (0x0000008) /* ro Rx buffer contains data     */
#define CHARM_UART_STAT_TXFULL     (0x0000002) /* ro Tx buffer is full           */
#define CHARM_UART_STAT_TXIDLE     (0x0000001) /* ro Tx empty and transmit done  */

/*** Uart auxiliary register *************************************************/

#define CHARM_UART_AUX_TXLIMIRQ    (0x00800000) /* ro Tx fifo limit IRQ pending */
#define CHARM_UART_AUX_RXLIMIRQ    (0x00400000) /* ro Tx fifo limit IRQ pending */
#define CHARM_UART_AUX_RXTOIRQ     (0x00200000) /* ro Rx timeout IRQ pending    */
#define CHARM_UART_AUX_TXLIM       (0x00100000) /* ro Tx fifo level<=limit      */
#define CHARM_UART_AUX_RXLIM       (0x00080000) /* ro Rx fifo level>limit       */
#define CHARM_UART_AUX_BRIRQ       (0x00040000) /* ro Break interrupt pending   */
#define CHARM_UART_AUX_TXIRQ       (0x00020000) /* ro Tx interrupt pending      */
#define CHARM_UART_AUX_RXIRQ       (0x00010000) /* ro Rx interrupt pending      */
#define CHARM_UART_AUX_RX_DATA     (0x00008000) /* ro Rx (synced) data -        */
#define CHARM_UART_AUX_TX_DATA     (0x00004000) /* ro Tx data - read only       */
#define CHARM_UART_AUX_FBD_MASK    (0x00003FFF) /* rw Flexiboard mask           */


/*** Uart fifo level & status register *************************************************/
#define CHARM_UART_FIFO_RX_LEVEL_MASK (0x000000FF) /* rw Transmit fifo fill level        */
#define CHARM_UART_FIFO_RX_LEVEL_SHIFT         (0) /*    Position of RX fill level field */
#define CHARM_UART_FIFO_TX_LEVEL_MASK (0x0000FF00) /* rw Transmit fifo fill level        */
#define CHARM_UART_FIFO_TX_LEVEL_SHIFT         (8) /*    Position of TX fill level field */
#define CHARM_UART_FIFO_RX_LIM_MASK   (0x00FF0000) /* ro Receive  fifo irq  level        */
#define CHARM_UART_FIFO_RX_LIM_SHIFT          (16) /*    Position of RX limit irq  limit */
/* Note: a status flag/IRQ is generated if RX level > RX limit */
#define CHARM_UART_FIFO_TX_LIM_MASK   (0xFF000000) /* ro Transmit fifo irq  limit        */
#define CHARM_UART_FIFO_TX_LIM_SHIFT          (24) /*    Position of TX limit irq  limit */
/* Note: a status flag/IRQ is generated if TX level <= TX limit */

/*** Compatibility symbols - for code shared with Helium family devices ******/

#define CHARM_UART_CONTROL         CHARM_UART_CTRL
#define CHARM_UART_STATUS          CHARM_UART_STAT

#define HF_PP_UART_TX              CHARM_UART_TX
#define HF_PP_UART_RX              CHARM_UART_RX
#define HF_PP_UART_CONTROL         CHARM_UART_CTRL
#define HF_PP_UART_STATUS          CHARM_UART_STAT

/*** UART receive register */

#define HF_PP_UART_RX_VALID        CHARM_UART_RX_VALID
#define HF_PP_UART_RX_DATA         CHARM_UART_RX_DATA

/*** UART control/status register */

#define HF_PP_UART_CS_BAUD_MASK    CHARM_UART_CTRL_BAUD_MASK
#define HF_PP_UART_CS_BAUD_9600    CHARM_UART_CTRL_BAUD_9600
#define HF_PP_UART_CS_BAUD_19200   CHARM_UART_CTRL_BAUD_19200
#define HF_PP_UART_CS_BAUD_31250   CHARM_UART_CTRL_BAUD_31250
#define HF_PP_UART_CS_BAUD_38400   CHARM_UART_CTRL_BAUD_38400

#define HF_PP_UART_CS_LOOPBACK     CHARM_UART_CTRL_LOOPBACK
#define HF_PP_UART_CS_UARTEN       CHARM_UART_CTRL_UARTEN
#define HF_PP_UART_CS_INTMASK      CHARM_UART_CTRL_INTMASK
#define HF_PP_UART_CS_INTDIS       CHARM_UART_CTRL_INTDIS
#define HF_PP_UART_CS_WAKEUP       CHARM_UART_CTRL_INTENB
#define HF_PP_UART_CS_LPIRQ        CHARM_UART_CTRL_INTENB
#define HF_PP_UART_CS_MPIRQ        CHARM_UART_CTRL_INTENB
#define HF_PP_UART_CS_HPIRQ        CHARM_UART_CTRL_INTENB
#define HF_PP_UART_CS_FIQ          CHARM_UART_CTRL_INTENB
#define HF_PP_UART_CS_RXINTENABLE  CHARM_UART_CTRL_RXINTENB
#define HF_PP_UART_CS_TXINTENABLE  CHARM_UART_CTRL_TXINTENB

#define HF_PP_UART_CS_RXORUN       CHARM_UART_STAT_RXORUN
#define HF_PP_UART_CS_RXNOTEMPTY   CHARM_UART_STAT_RXNOTEMPTY
#define HF_PP_UART_CS_TXFULL       CHARM_UART_STAT_TXFULL
#define HF_PP_UART_CS_TXEMPTY      CHARM_UART_STAT_TXIDLE


/*** TIMER register */

#define CLOCKFREQ                  (8000000) /* (Hz) */
#define CHARM_TIMER1_VALUE         (0x108) /* Timer 1 current value          */
#define TIMER_CURRENT_TIME         (PPIO(TIMER1_VALUE))
#define HELIUM_UART_BAUDRATE       CHARM_UART_CTRL_BAUD_38400


#endif
