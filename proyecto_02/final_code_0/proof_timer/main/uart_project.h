#if !defined(UART_PROJECT_H)

#define  UART_PROJECT_H

#define CLKSRC 80000000

#include "soc/uart_struct.h"
#include "soc/uart_reg.h"
#include "driver/uart.h"

/* Configura la UART0 */
void uart0_config(uint32_t baudrate, int tx_pin, int rx_pin);

/* Envía un byte usando UART0 */
void uart0_send_byte(uint8_t data);

/* Recibe un byte usando UART0 */
uint8_t uart0_receive_byte();

/* Envia una cadena de caracteres usando UART0 */
void uart0_send_string(const char* str);

/* Devuelve numero bytes almacenados en RXFIFO */
uint8_t get_uart0_rxfifo_cnt (void);

/* Devuelve numero bytes almacenados en TXFIFO */
uint8_t get_uart0_txfifo_cnt (void);

/* Flush a TX FIFO */
void reset_uart0_txfifo (void);

/* Flush a RX FIFO */
void reset_uart0_rxfifo (void);

#endif