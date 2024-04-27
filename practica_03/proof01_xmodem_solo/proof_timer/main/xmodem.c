#include "xmodem.h"


extern Tm_Control c_tiempo;
extern Buffer_Control c_buff;

void XM_fsm_xmodem()
{
    static XM_STATE_T state = INICIO;
    static uint8_t num_paq = 0;
    static uint8_t num_bytes = 0;
    static uint8_t num_retransmissions = 0;
    static uint8_t data_raw[131] = {};
    static uint8_t rx_data = 0;
    static uint8_t check_sum = 0;

    switch( state )
    {
        case INICIO:
            if(Tm_Hubo_timeout(&c_tiempo,0))
            {
                uart0_send_byte(NAK);
                Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                state = INICIO;
            }
            else if(uart0_receive_byte(&rx_data))
            {
                if(rx_data == SOH)
                {
                    Tm_Inicie_timeout(&c_tiempo,1,TICKS_TO_PAQ);
                    num_bytes = 1;
                    num_paq = 1;
                    check_sum = 0;
                    state = RX_PAQ;
                }
            }
        break;

        case RX_PAQ:
            if(is_not_empty_uart0())
            {
                if(num_bytes < 131)
                {
                    if((UART0.status.rxfifo_cnt > 0) || (UART0.mem_cnt_status.rx_cnt > 0));
                    uart0_receive_byte(&rx_data);
                    data_raw[num_bytes-1] = rx_data;
                    num_bytes++;                    
                    gpio_set_level(CONFIG_GPIO_OUTPUT_XMODEM, 0);
                    Bf_Subir_Dato(&c_buff,num_bytes);
                    gpio_set_level(CONFIG_GPIO_OUTPUT_XMODEM, 1);
                    Tm_Inicie_timeout(&c_tiempo,1,TICKS_TO_PAQ);
                }
                else if(num_bytes >= 131) // cuando vale 128 es el último dato
                {
                    while(uart0_receive_byte(&rx_data));
                    data_raw[num_bytes-1] = rx_data;

                    if(paq_ok(data_raw,num_paq) == 1)
                    {
                        state = WAIT_DISPLAY;
                    }
                    else if(paq_ok(data_raw,num_paq) == 2)
                    {
                        //  Como llegó el último paquete guardado
                        //    se pide el siguiente paquete sin guardar este 
                        uart0_send_byte(ACK);
                        num_paq++;
                        Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                        state = WAIT_PAQ;
                    }
                    else if(num_retransmissions < MAX_RETRANSMISSIONS)
                    {
                        uart0_send_byte(NAK);
                        num_retransmissions++;
                        Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                        state = WAIT_PAQ;
                    }
                    else
                    {
                        uart0_send_byte(CAN);
                        num_bytes = 0;
                        num_paq = 0;
                        num_retransmissions = 0;
                        Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                        state = INICIO;
                    }
                }
            }
        /*
            if(is_not_empty_uart0() && (num_bytes < 129))
            {
                uart0_receive_byte(&rx_data);
                data_raw[num_bytes-1] = rx_data;
                num_bytes++;
                Tm_Inicie_timeout(&c_tiempo,1,TICKS_TO_PAQ);
            }
            else if(is_not_empty_uart0() && (num_bytes >= 129)) // cuando vale 128 es el último dato
            {
                gpio_set_level(CONFIG_GPIO_OUTPUT_XMODEM, 0);
                uart0_receive_byte(&rx_data);
                data_raw[num_bytes-1] = rx_data;

                if(paq_ok(data_raw,num_paq) == 1)
                {
                    state = WAIT_DISPLAY;
                }
                else if(paq_ok(data_raw,num_paq) == 2)
                {
                    //  Como llegó el último paquete guardado
                    //    se pide el siguiente paquete sin guardar este 
                    uart0_send_byte(ACK);
                    num_paq++;
                    Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                    state = WAIT_PAQ;
                }
                else if(num_retransmissions < MAX_RETRANSMISSIONS)
                {
                    uart0_send_byte(NAK);
                    num_retransmissions++;
                    Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                    state = WAIT_PAQ;
                }
                else
                {
                    uart0_send_byte(CAN);
                    num_bytes = 0;
                    num_paq = 0;
                    num_retransmissions = 0;
                    Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                    state = INICIO;
                }
            } */
            else if(Tm_Hubo_timeout(&c_tiempo,1))
            {
                uart0_send_byte(NAK);
                Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                num_retransmissions++;
                state = WAIT_PAQ;
            }
        break;

        case WAIT_DISPLAY:
            if(Bf_75_Vacio(&c_buff))
            {
                for(int i = 0; i < 128; i++)
                {
                    Bf_Subir_Dato(&c_buff,data_raw[i+2]);
                }
                uart0_send_byte(ACK);
                num_paq++;
                Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                state = WAIT_PAQ;
            }
        break;

        case WAIT_PAQ:
            if(is_not_empty_uart0())
            {
                uart0_receive_byte(&rx_data);
                if(rx_data == SOH)
                {
                    num_bytes = 1;
                    Tm_Inicie_timeout(&c_tiempo,1,TICKS_TO_PAQ);
                    state = RX_PAQ;
                }
                else if(rx_data == EOT)
                {
                    /* Se terminó el archivo, levantar bandera */
                    state = INICIO;
                    /* Como se acabó el archivo no estoy seguro de este timeout, pero supongo que sí */
                    Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                }
                else
                {
                    Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                    state = WAIT_PARTIAL_TO_PAQ;
                }
            }
            else if(Tm_Hubo_timeout(&c_tiempo,0))
            {
                if(num_retransmissions < MAX_RETRANSMISSIONS)
                {
                    uart0_send_byte(NAK);
                    Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                    num_retransmissions++;
                }
                else
                {
                    uart0_send_byte(CAN);
                    num_bytes = 0;
                    num_paq = 0;
                    num_retransmissions = 0;
                    Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                    state = INICIO;
                }
            }
        break;

        case WAIT_PARTIAL_TO_PAQ:
            /* De este no estoy seguro porque no he encontrado qué hacer si llega dato malo
                en vez de SOH o EOT, igual se supone en 10 segundos ya debería llegar paquete completo */
            if(Tm_Hubo_timeout(&c_tiempo,0))
            {
                if(num_retransmissions < MAX_RETRANSMISSIONS)
                {
                    uart0_send_byte(NAK);
                    Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                    num_retransmissions++;
                    state = WAIT_PAQ;
                }
                else
                {
                    uart0_send_byte(CAN);
                    num_bytes = 0;
                    num_paq = 0;
                    num_retransmissions = 0;
                    Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                    state = INICIO;
                }
            }
        break;

    }

    /*
    switch( state )
    {
        case INICIO:
            if(Tm_Hubo_timeout(&c_tiempo,0))
            {
                uart0_send_byte(NAK);
                Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                state = INICIO;
            }
            else if(uart0_receive_byte(&rx_data))
            {
                if(rx_data == SOH)
                {
                    Tm_Inicie_timeout(&c_tiempo,1,TICKS_TO_PAQ);
                    num_paq = 1;
                    num_bytes = 1;
                    check_sum = 0;
                    state = RX_PAQ;
                }
            }
        break;

        case RX_NUM:
            if(Tm_Hubo_timeout(&c_tiempo,1))
            {
                uart0_send_byte(NAK);
                Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);
                state = INICIO;
            }
            else if((get_uart0_rxfifo_cnt() > 0) && (num_bytes == 1))
            {
                uart0_receive_byte(&rx_data);
                Tm_Inicie_timeout(&c_tiempo,1,TICKS_TO_PAQ);
                data_raw[0] = rx_data;
                ++num_bytes;
                state = RX_NUM;
            }
            else if((get_uart0_rxfifo_cnt() > 0) && (num_bytes == 2))
            {
                uart0_receive_byte(&rx_data);
                Tm_Inicie_timeout(&c_tiempo,1,TICKS_TO_PAQ);
                data_raw[1] = rx_data;
                ++num_bytes;
                state = RX_PAQ;
            }
        break;

        case RX_PAQ:
            if(Tm_Hubo_timeout(&c_tiempo,1))
            {
                uart0_send_byte(NAK);
                Tm_Inicie_timeout(&c_tiempo,1,TICKS_TO_PAQ);
                state = RX_PAQ;
            }
            else if (uart0_receive_byte(&rx_data) && (num_bytes < 132))
            {
                data_raw[num_bytes-1] = rx_data;
                check_sum += rx_data;
                ++num_bytes;
                Tm_Inicie_timeout(&c_tiempo,1,TICKS_TO_PAQ);
                state = RX_PAQ;
            }
            else if (uart0_receive_byte(&rx_data) && (num_bytes == 132))
            {
                if((check_sum == rx_data) && (num_paq == data_raw[0]) && ((~num_paq) == data_raw[1]))
                {
                    // PAQ OK 
                    state = WAIT_DISPLAY;
                }
                else
                {
                    
                }
            }            
            
    }*/
    
}

unsigned char paq_ok(uint8_t datos[], uint8_t num_paq)
/* retorna 1 sí está bien, 2 si debe seguir sin guardar y 0 si está mal */
{
    uint8_t suma = 0; 
    uint8_t valor = ~(datos[1]);
    if((datos[0]) != valor)
        return 0;

    if(datos[0] == (num_paq-1))
        return 2;

    if(datos[0] != num_paq)
        return 0;

    for(int i = 2; i < 130; i++)
    {
        suma+=datos[i];
    }

    if(suma != datos[130])
        return 0;

    return 1;
}