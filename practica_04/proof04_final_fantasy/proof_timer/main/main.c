
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

void IRAM_ATTR t0isr(void)
{
    // Retrieve the interrupt status
    uint32_t timer_intr_status = TIMERG0.int_st_timers.t0_int_st;

    // Clear the interrupt status
   TIMERG0.int_clr_timers.t0_int_clr = 1;

    gpio_set_level(CONFIG_GPIO_OUTPUT_XMODEM, 0);
    Tm_Procese(&c_tiempo);
    gpio_set_level(CONFIG_GPIO_OUTPUT_XMODEM, 1);
    // Perform necessary operations based on the interrupt status
    if (timer_intr_status) {
        // Handle the alarm interrupt here
        // For example, toggle an LED or perform specific actions
    }

    // Reset the timer if needed
//    TIMERG0.hw_timer[0].config.tx_alarm_en = 1; // Re-enable the timer alarm
}


void Timer_Initialize (int group_timer, int timer_idx)
{
    
   TIMERG0.hw_timer[timer_idx].config.tx_divider=22222; // Divide the clock by 22222
   TIMERG0.hw_timer[timer_idx].config.tx_autoreload=1; // autoreload activated
   TIMERG0.hw_timer[timer_idx].config.tx_increase=0; // decrease
   TIMERG0.hw_timer[timer_idx].load.val=1; // activate load
   TIMERG0.hw_timer[timer_idx].loadlo.val=10; // reload with low val equal to 10
   TIMERG0.hw_timer[timer_idx].loadhi.val=0; // reload with high val equal to 0
   TIMERG0.hw_timer[timer_idx].load.val=0; // deactivate load
   TIMERG0.hw_timer[timer_idx].alarmhi.val=0;
    TIMERG0.hw_timer[timer_idx].alarmlo.val=0;
   TIMERG0.hw_timer[timer_idx].config.tx_alarm_en=1;
   TIMERG0.hw_timer[timer_idx].config.tx_level_int_en=1;

    timer_enable_intr(group_timer,timer_idx);
    timer_isr_register(0, 0, t0isr, NULL, ESP_INTR_FLAG_IRAM, NULL);

   TIMERG0.hw_timer[timer_idx].config.tx_en=1; // enable timer
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

    //botones
    gpio_set_direction(CONFIG_GPIO_INPUT_0, GPIO_MODE_INPUT);
    gpio_set_direction(CONFIG_GPIO_INPUT_1, GPIO_MODE_INPUT);

    timer_config_t config = {
        .divider = 22222,
        .counter_dir = TIMER_COUNT_DOWN,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = true,
    };
    timer_init(0, 0, &config);

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
        if(is_not_empty_uart0())
        {
            //printf("verga");
//            uart0_send_byte(97);
//            uart0_send_byte(c_buff_uart.cont+48);
            uart0_receive_byte(&echo_var);
            uart0_send_byte(echo_var);
            //printf("%d %d\t",echo_var,UART0.status.rxfifo_cnt);
        }
        /*
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
        */
    }
}
