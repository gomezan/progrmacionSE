#include <stdio.h>
#include "Tiempo.h"
#include "display.h"

#include "driver/gpio.h"


Tm_Control c_tiempo;
Dp_Control c_display;


void app_main(void){

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

   
    Tm_Initialize_Timer_0();

    Tm_Periodo periodos[2];
    Tm_Timeout timeouts[1];

    Tm_Inicie(&c_tiempo, periodos, 2, timeouts, 1, Atender_timer);
    //Display
    Dp_Inicie(&c_display, 0);
    
    //Inicio del periodo
    Tm_Inicie_periodo(&c_tiempo, 0, 1);

    while(1){
        
        if ( Atender_timer(NO) ){
            Tm_Procese(&c_tiempo);
        }

        if (Tm_Hubo_periodo(&c_tiempo,0) )
        {
            Dp_Procese (&c_display);
            Tm_Baje_periodo (&c_tiempo,0);
        } 
    }

    }


