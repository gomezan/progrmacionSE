/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "Buffer.h"
#include "Tiempo.h"
#include "display.h"
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

/**
 * Brief:
 * This test code shows how to configure gpio and how to use gpio interrupt.
 *
 * GPIO status:
 * GPIO18: output (ESP32C2/ESP32H2 uses GPIO8 as the second output pin)
 * GPIO19: output (ESP32C2/ESP32H2 uses GPIO9 as the second output pin)
 * GPIO4:  input, pulled up, interrupt from rising edge and falling edge
 * GPIO5:  input, pulled up, interrupt from rising edge.
 *
 * Note. These are the default GPIO pins to be used in the example. You can
 * change IO pins in menuconfig.
 *
 * Test:
 * Connect GPIO18(8) with GPIO4
 * Connect GPIO19(9) with GPIO5
 * Generate pulses on GPIO18(8)/19(9), that triggers interrupt on GPIO4/5
 *
 */

#define CLKSRC 80000000
#define BAUDRATE 2400

//#define GPIO_OUTPUT_IO_7 CONFIG_GPIO_OUTPUT_7
#define SIZE_BUFFER 176

extern timg_dev_t TIMERG0;

Buffer_Control c_buff;
Tm_Control c_tiempo;
Dp_Control c_display;
P_Control c_presentacion;

void IRAM_ATTR t0isr(void)
{
    // Retrieve the interrupt status
    uint32_t timer_intr_status = TIMERG0.int_st_timers.t0_int_st;

    // Clear the interrupt status
   TIMERG0.int_clr_timers.t0_int_clr = 1;

    gpio_set_level(CONFIG_GPIO_OUTPUT_XMODEM, 0);
    gpio_set_level(CONFIG_GPIO_OUTPUT_XMODEM, 1);
    // Perform necessary operations based on the interrupt status
    if (timer_intr_status) {
        // Handle the alarm interrupt here
        // For example, toggle an LED or perform specific actions
    }

    // Reset the timer if needed
    TIMERG0.hw_timer[0].config.tx_alarm_en = 1; // Re-enable the timer alarm
}

void Timer_Initialize (int group_timer, int timer_idx)
{
    TIMERG0.hw_timer[0].config.tx_en = 0;

    TIMERG0.hw_timer[timer_idx].config.tx_divider=22222; // Divide the clock by 22222
    TIMERG0.hw_timer[timer_idx].load.val=1; // activate load
    TIMERG0.hw_timer[timer_idx].loadlo.val=10; // reload with low val equal to 10
    TIMERG0.hw_timer[timer_idx].loadhi.val=0; // reload with high val equal to 0
    TIMERG0.hw_timer[timer_idx].load.val=0; // deactivate load
    TIMERG0.hw_timer[0].config.tx_autoreload = 1; // Enable autoreload
    TIMERG0.hw_timer[0].config.tx_increase = 0; // Set counter to count down
    TIMERG0.hw_timer[0].alarmhi.tx_alarm_hi = 0;
    TIMERG0.hw_timer[0].alarmlo.tx_alarm_lo = 0;
    TIMERG0.hw_timer[0].config.tx_alarm_en = 1;
    TIMERG0.hw_timer[0].config.tx_level_int_en = 1;

    timer_enable_intr(group_timer,timer_idx);
    timer_isr_register(0, 0, t0isr, NULL, ESP_INTR_FLAG_IRAM, NULL);    
    
    TIMERG0.hw_timer[0].config.tx_en = 1;
}   

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

    Timer_Initialize(0,0);

    Tm_Periodo periodos[3];
    Tm_Timeout timeouts[3];

    Bf_data data[SIZE_BUFFER] = {};

    Bf_Inicie(&c_buff,data,SIZE_BUFFER);
    Tm_Inicie(&c_tiempo,periodos,3,timeouts,3,Atender_timer);

    // Cada 360 Hz
    // Actualizar el valor de multiplexación
    Tm_Inicie_periodo (&c_tiempo,0,1);

    // Cada 120 Hz
    // Actualizar el valor de los digitos en presentacion
    Tm_Inicie_periodo (&c_tiempo,1,3);

    Dp_Inicie(&c_display, 0);
    P_Inicie(&c_presentacion, 1,2);

    uart0_config(2400,17,16); // Configura UART0 a 2400 baudios tx 17 y rx 16

    vaciar_buffer_rx();

    esp_task_wdt_deinit();

//    reset_uart0_txfifo();

//    reset_uart0_rxfifo();
    gpio_set_level(CONFIG_GPIO_OUTPUT_XMODEM, 1);

    /* TO para enviar NAK continuamente */
    Tm_Inicie_timeout(&c_tiempo,0,TICKS_TO_START_NAK);    
    while (1) {

        /*
        if ( Atender_timer(NO) )
        {
            Tm_Procese(&c_tiempo);
        }

        

        if (Tm_Hubo_periodo(&c_tiempo,0) )
        {
            //gpio_set_level(GPIO_OUTPUT_IO_0,cnt%2);
            Dp_Procese (&c_display);
            Tm_Baje_periodo(&c_tiempo,0);
        }

        if (Tm_Hubo_periodo(&c_tiempo,1) )
        {
            P_Procese (&c_presentacion);
            Tm_Baje_periodo(&c_tiempo,1);
        }

        XM_fsm_xmodem();*/
        
    }
}
