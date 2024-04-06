#if !defined(UART_PROJECT_H)

#define  UART_PROJECT_H

#define CLKSRC 80000000

#include "soc/uart_struct.h"
#include "soc/uart_reg.h"
#include "driver/uart.h"

void uart0_config(uint32_t baudrate, int tx_pin, int rx_pin){

    // Desactivar temporalmente la UART
    UART0.conf0.val = 0;

    // Configurar el baud rate
    uint32_t parteEntera = (uint32_t) (CLKSRC / baudrate);
    uint32_t parteDecimal = (uint32_t)(((CLKSRC / baudrate) - parteEntera) * 10);
    parteEntera = parteEntera & UART_CLKDIV_V;
    parteDecimal = parteDecimal & UART_CLKDIV_FRAG_V;
    UART0.clk_div.val = (parteEntera | (parteDecimal << UART_CLKDIV_FRAG_S));

    // Configurar los bits de datos, paridad y bits de parada
    UART0.conf0.val |= (1 << UART_TICK_REF_ALWAYS_ON_S)
                    | (3 << UART_BIT_NUM_S)                // 8 bits de datos
                    | (0 << UART_PARITY_EN_S)              // Paridad deshabilitada
                    | (1 << UART_STOP_BIT_NUM_S);          // 1 bit de parada

    uart_set_pin(UART_NUM_0, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
}

void uart0_send_byte(uint8_t data) {
    UART0.fifo.rw_byte = data; // Escribir el byte en el FIFO
}

// Recibe un byte usando UART0
uint8_t uart0_receive_byte() {
    return (uint8_t)(UART0.fifo.rw_byte);
}

// Envia una cadena de caracteres usando UART0
void uart0_send_string(const char* str) {
    uart0_send_byte((uint8_t)*str++);
}

//Devuelve numero bytes almacenados en RXFIFO
uint8_t get_uart0_rxfifo_cnt (void){
    return UART0.status.rxfifo_cnt;
}

//Devuelve numero bytes almacenados en TXFIFO
uint8_t get_uart0_txfifo_cnt (void){
    return UART0.status.txfifo_cnt;
}

// Flush a TX FIFO
void reset_uart0_txfifo (void){
    UART0.conf0.txfifo_rst = UART_TXFIFO_RST_V;
    UART0.conf0.txfifo_rst = 0;
}


// Flush a RX FIFO
void reset_uart0_rxfifo (void){
    UART0.conf0.rxfifo_rst = UART_RXFIFO_RST_V;
    UART0.conf0.rxfifo_rst = 0;
}

#endif