#if !defined(UART_PROJECT_H)

#define  UART_PROJECT_H

#include "soc/uart_struct.h"
#include "soc/uart_reg.h"
#include "driver/uart.h"
#include "hal/uart_ll.h"
#include "esp_intr_alloc.h"
#include "Buffer.h"

#define SIZE_BUFFER_UART 4

#define PRO_INTR_STATUS_REG_1_REG      (*((volatile uint32_t *)0x3FF000F0))
#define APP_INTR_STATUS_REG_1_REG      (*((volatile uint32_t *)0x3FF000FC))
#define PRO_UART_INTR_MAP_REG      (*((volatile uint32_t *)0x3FF0018C))
#define APP_UART_INTR_MAP_REG      (*((volatile uint32_t *)0x3FF002A0))

#define UART_INTR_CONFIG_FLAG (UART_INTR_RXFIFO_FULL)
/*#define UART_INTR_CONFIG_FLAG ((UART_INTR_RXFIFO_FULL) \
                            | (UART_INTR_RXFIFO_TOUT) \
                            | (UART_INTR_RXFIFO_OVF) \
                            | (UART_INTR_BRK_DET) \
                            | (UART_INTR_PARITY_ERR))*/

#define XOFF 19
#define XON 17
#define CLKSRC 80000000

/* Simbolos de Xmodem */
#define SOH 1
#define EOT 4
#define ACK 6
#define NAK 21
#define CAN 24

static uart_isr_handle_t isr_handle;

/* Configura la UART0 */
void uart0_config(uint32_t, int, int);

/* Envía un byte usando UART0 */
void uart0_send_byte(uint8_t);

// Recibe un byte usando UART0
uint8_t uart0_receive_byte(uint8_t *);

// Envia una cadena de caracteres usando UART0
void uart0_send_string(const char*);

// Devuelve numero bytes almacenados en RXFIFO
uint8_t get_uart0_rxfifo_cnt (void);

// Devuelve numero bytes almacenados en TXFIFO
uint8_t get_uart0_txfifo_cnt (void);

void reset_uart0_rxfifo (void);

// Flush a TX FIFO
void reset_uart0_txfifo (void);

// Flush a RX FIFO
void reset_uart0_rxfifo (void);

/* Return 1 if has something and 0 if not */
uint8_t is_not_empty_uart0(void);

/* vacia buffer de rx */
void vaciar_buffer_rx();

// Handle de interrupcion
static void uart0_isr_handler(void*);

#endif