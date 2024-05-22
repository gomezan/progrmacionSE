#include "uart_project.h"

volatile Buffer_Control c_buff_uart;

void uart0_config(uint32_t baudrate, int tx_pin, int rx_pin){

    int dato = 0;
    // Inicializacion buffer
    Bf_data data[SIZE_BUFFER_UART] = {};

    Bf_Inicie(&c_buff_uart,data,SIZE_BUFFER_UART);

    // Desactivar temporalmente la UART
    UART0.conf0.val = 0;

    // Configurar el baud rate
    uint32_t parteEntera = (uint32_t) (CLKSRC / baudrate);
    uint32_t parteDecimal = (uint32_t)(((CLKSRC / baudrate) - parteEntera) * 10);
    parteEntera = parteEntera & UART_CLKDIV_V;
    parteDecimal = parteDecimal & UART_CLKDIV_FRAG_V;
    UART0.clk_div.val = (parteEntera | (parteDecimal << UART_CLKDIV_FRAG_S));

    //Configurar tamaño de la RX FIFO
    UART0.mem_conf.rx_size = 15;

    // Configurar los bits de datos, paridad y bits de parada
    UART0.conf0.val |= (1 << UART_TICK_REF_ALWAYS_ON_S)
                    | (3 << UART_BIT_NUM_S)                // 8 bits de datos
                    | (0 << UART_PARITY_EN_S)              // Paridad deshabilitada
                    | (1 << UART_STOP_BIT_NUM_S);          // 1 bit de parada

    // Limpiar banderas de interrupcion
    UART0.int_clr.val = 0xFFFFFFFF;

    // Habilitar interrupcion
    UART0.int_ena.rxfifo_full = 1;

    // Configuracion threshold para interrupcion al estar la FIFO llena en 1 valor
    // 7 menos significativos
    UART0.conf1.rxfifo_full_thrhd = 0x01;
    // 3 mas significativos
    UART0.mem_conf.rx_flow_thrhd_h3 = 0x0;

    // Configurar handle de interrupcion

    dato = esp_intr_alloc(ETS_UART0_INTR_SOURCE, (void *) NULL, uart0_isr_handler, NULL, &isr_handle);

    
    printf("Holaaaaaaaaaaaaaaaaaaaaaaaa %d\n",dato);

    uart_intr_config_t uart_intr = {
        .intr_enable_mask = UART_INTR_CONFIG_FLAG,
        .rxfifo_full_thresh = 0x01,
    };

    dato = uart_intr_config(UART_NUM_0, &uart_intr);

    printf("Holaaaaaaaaaaaaaaaaaaaaaaaa %d\n",dato);

    // Configurar pines de TX y RX
    uart_set_pin(UART_NUM_0, tx_pin, rx_pin, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

}

static void uart0_isr_handler(void* arg) {
    // Verificar la fuente de la interrupcion
    //if(UART0.int_st.rxfifo_full)
    //{
        if (!Bf_Lleno(&c_buff_uart)) {
            volatile uint8_t contador = UART0.status.rxfifo_cnt;
            volatile uint8_t data = (uint8_t)(UART0.fifo.rw_byte);
            Bf_Subir_Dato(&c_buff_uart,data);
            data = contador + contador;
            contador = UART0.status.rxfifo_cnt;
            volatile uint8_t status_reg = UART0.int_st.rxfifo_full;
            UART0.int_clr.rxfifo_full = 1;
        } else{
            UART0.int_ena.rxfifo_full = 0;
        }
    //}
}

void uart0_send_byte(uint8_t data) {
    UART0.fifo.rw_byte = data; // Escribir el byte en el FIFO
}

// Recibe un byte usando UART0
uint8_t uart0_receive_byte(uint8_t *var) {

    Bf_data llega_dato;

    if(Bf_Bajar_Dato(&c_buff_uart,&llega_dato)){
        uart0_send_byte(llega_dato);
        *var = llega_dato;
        UART0.int_ena.rxfifo_full = 1;
        return 1;
    }

    uart0_send_byte(llega_dato);

    return 0;
}

// Envia una cadena de caracteres usando UART0
void uart0_send_string(const char* str) {
    uart0_send_byte((uint8_t)*str++);
}

// Devuelve numero bytes almacenados en RXFIFO
uint8_t get_uart0_rxfifo_cnt (void){
    return UART0.status.rxfifo_cnt;
}


// Devuelve numero bytes almacenados en TXFIFO
uint8_t get_uart0_txfifo_cnt (void){
    return UART0.status.txfifo_cnt;
}

// Flush a TX FIFO
void reset_uart0_txfifo (void){
    UART0.conf0.txfifo_rst = UART_TXFIFO_RST_V;
}

// Flush a RX FIFO
void reset_uart0_rxfifo (void){
    UART0.conf0.rxfifo_rst = UART_RXFIFO_RST_V;
}

/* Return 1 if has something and 0 if not */
uint8_t is_not_empty_uart0(void){

    if(!Bf_Vacio(&c_buff_uart))
    {
        return 1;
    }

    return 0;

    if((UART0.status.rxfifo_cnt) || (UART0.mem_cnt_status.rx_cnt))
    {
        return 1;
    }
    return 0;
}

/* vacia buffer de rx */
void vaciar_buffer_rx()
{
    uint8_t valor = 0;
    for(int i = 0; i < 128; i++)
    {
        valor = UART0.fifo.rw_byte;
    }
}