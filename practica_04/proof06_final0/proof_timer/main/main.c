
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Buffer.h"
#include "Tiempo.h"
#include "display.h"
#include "button.h"
#include "hal/timer_ll.h"
#include "hal/mwdt_ll.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "soc/uart_struct.h"
#include "soc/uart_reg.h"
#include "xmodem.h"
#include "uart_project.h"
#include "presentacion.h"
#include "esp_timer.h"
#include "driver/timer.h"
#include "driver/timer_types_legacy.h"

#define CLKSRC 80000000
#define BAUDRATE 2400

#define SIZE_BUFFER 176

extern timg_dev_t TIMERG0;

Buffer_Control c_buff;
Tm_Control c_tiempo;
Dp_Control c_display;
P_Control c_presentacion;
Bt_Control c_botonesInc;
Bt_Control c_botonesDec;
volatile Buffer_Control c_buff_uart;

void app_main(void)
{
    //7 segmentos
    gpio_set_direction(CONFIG_GPIO_OUTPUT_0, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_GPIO_OUTPUT_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_GPIO_OUTPUT_2, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_GPIO_OUTPUT_3, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_GPIO_OUTPUT_4, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_GPIO_OUTPUT_5, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_GPIO_OUTPUT_6, GPIO_MODE_OUTPUT);

    // Pin para prueba
    gpio_set_direction(CONFIG_GPIO_OUTPUT_XMODEM, GPIO_MODE_OUTPUT);

    //Digitos
    gpio_set_direction(CONFIG_GPIO_DIGIT_0, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_GPIO_DIGIT_1, GPIO_MODE_OUTPUT);
    gpio_set_direction(CONFIG_GPIO_DIGIT_2, GPIO_MODE_OUTPUT);

    //botones
    gpio_set_direction(CONFIG_GPIO_INPUT_0, GPIO_MODE_INPUT);
    gpio_set_direction(CONFIG_GPIO_INPUT_1, GPIO_MODE_INPUT);

    Timer_Initialize(0,0);

    Tm_Periodo periodos[4];
    Tm_Timeout timeouts[5];

    Bf_data data[SIZE_BUFFER] = {};

    Bf_Inicie(&c_buff,data,SIZE_BUFFER);
    Tm_Inicie(&c_tiempo,periodos,4,timeouts,5,Atender_timer);

    // Cada 360 Hz
    // Actualizar el valor de multiplexación
    Tm_Inicie_periodo (&c_tiempo,0,1);

    // Cada 120 Hz
    // Actualizar el valor de los digitos en presentación
    Tm_Inicie_periodo (&c_tiempo,1,3);

    // Cada 360 Hz
    // Revisar estado de botón
    Tm_Inicie_periodo (&c_tiempo,2,1);
    Tm_Inicie_periodo (&c_tiempo,3,1);

    
    esp_task_wdt_deinit();


    Dp_Inicie(&c_display, 0);
    P_Inicie(&c_presentacion, 1,2);
    Bt_Inicie(&c_botonesInc,CONFIG_GPIO_INPUT_0,2,3,increment_N);
    Bt_Inicie(&c_botonesDec,CONFIG_GPIO_INPUT_1,3,4,decrement_N);

    uart0_config(2400,17,16); // Configura UART0 a 2400 baudios tx 17 y rx 16

    vaciar_buffer_rx();

    esp_task_wdt_deinit();

    gpio_set_level(CONFIG_GPIO_OUTPUT_XMODEM, 1); 

    /* TO para enviar NAK continuamente */
    Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);    

    uint8_t echo_var = 3;

    uint32_t contador_rx = 0;

    while (1) {
        
        if (Tm_Hubo_periodo(&c_tiempo,0) )
        {
            Dp_Procese (&c_display);
            Tm_Baje_periodo(&c_tiempo,0);
        }
        
        if (Tm_Hubo_periodo(&c_tiempo,1) )
        {
            P_Procese (&c_presentacion);
            Tm_Baje_periodo(&c_tiempo,1);
        }

        if (Tm_Hubo_periodo(&c_tiempo,2) )
        {
            Bt_Procese(&c_botonesInc);
            Tm_Baje_periodo(&c_tiempo,2);
        }

        if (Tm_Hubo_periodo(&c_tiempo,3) )
        {
            Bt_Procese(&c_botonesDec);
            Tm_Baje_periodo(&c_tiempo,3);
        }

        XM_fsm_xmodem();
        
    }
}
