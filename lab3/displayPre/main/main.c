#include <stdio.h>
#include "Tiempo.h"
#include "display.h"

#include "../../../../../../esp/v5.2.1/esp-idf/components/soc/esp32c2/include/soc/timer_group_struct.h"
#include "driver/gpio.h"


Tm_Control c_tiempo;
Dp_Control c_display;

char Atender_timer(char atienda)
{ 
    static uint64_t start_time = 0;
    static uint64_t elapsed_time = 0;


    if (elapsed_time >= 6250)
    {

        return SI;
    }
    else
        return NO;
};


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

//Digitos
gpio_set_direction(CONFIG_GPIO_DIGIT_0, GPIO_MODE_OUTPUT);
gpio_set_direction(CONFIG_GPIO_DIGIT_1, GPIO_MODE_OUTPUT);
gpio_set_direction(CONFIG_GPIO_DIGIT_2, GPIO_MODE_OUTPUT);


uint32_t read_lo, read_hi;

    //Habilita edición de  registros
    TIMERG0.hw_timer[0].config.tx_en = 0;
    //Comfigurar divisor de frecuencia
    TIMERG0.hw_timer[0].config.tx_divider = 50000;
    // desabilitar autoreload
    TIMERG0.hw_timer[0].config.tx_autoreload = 0;
    //Decrementa el valor del contador
    TIMERG0.hw_timer[0].config.tx_increase = 0;
    
    //Permite ajustar el valor del contador
    TIMERG0.hw_timer[0].load.val = 1;
    TIMERG0.hw_timer[0].loadlo.val = 20;
    TIMERG0.hw_timer[0].loadhi.val = 0;
    TIMERG0.hw_timer[0].load.val = 0;
    //Deshabilita edición de  registros
    TIMERG0.hw_timer[0].config.tx_en = 1;


    Tm_Periodo periodos[3];
    Tm_Timeout timeouts[3];


    print_digit(1);
    gpio_set_level(CONFIG_GPIO_DIGIT_0,1);
    gpio_set_level(CONFIG_GPIO_DIGIT_1,0);
    gpio_set_level(CONFIG_GPIO_DIGIT_2,1);


    Tm_Inicie(&c_tiempo, periodos, 3, timeouts, 3, Atender_timer);
    //Display
    Dp_Inicie(&c_display, 0, 0, 1, 2);
    
    //Inicio del periodo
    Tm_Inicie_periodo(&c_tiempo, 0, 1);
   

}
