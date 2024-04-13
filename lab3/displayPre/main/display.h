/* display.h    Definiciones del m�dulo Display */
#if !defined(DISPLAY_H)

#define  DISPLAY_H

#include "Tiempo.h"
#include "Varios.h"
#include "driver/gpio.h"

// Variables para manejar los pines del display
#define CONFIG_GPIO_OUTPUT_0     GPIO_NUM_2
#define CONFIG_GPIO_OUTPUT_1     GPIO_NUM_4
#define CONFIG_GPIO_OUTPUT_2     GPIO_NUM_16
#define CONFIG_GPIO_OUTPUT_3     GPIO_NUM_17
#define CONFIG_GPIO_OUTPUT_4     GPIO_NUM_5 
#define CONFIG_GPIO_OUTPUT_5     GPIO_NUM_18  
#define CONFIG_GPIO_OUTPUT_6     GPIO_NUM_19

#define CONFIG_GPIO_DIGIT_0     GPIO_NUM_25
#define CONFIG_GPIO_DIGIT_1     GPIO_NUM_33
#define CONFIG_GPIO_DIGIT_2     GPIO_NUM_32

    //Pines que controlan la salida del 7 segmentos
    static unsigned char pins[7] = 
    {
        CONFIG_GPIO_OUTPUT_0,
        CONFIG_GPIO_OUTPUT_1,
        CONFIG_GPIO_OUTPUT_2,
        CONFIG_GPIO_OUTPUT_3,
        CONFIG_GPIO_OUTPUT_4,
        CONFIG_GPIO_OUTPUT_5,
        CONFIG_GPIO_OUTPUT_6
    };

typedef struct Dp_Control Dp_Control;


//Estructura de control de display
struct Dp_Control
{                  
   /* Banderas */
   unsigned char  flag_eof, //Fin de archivo
                  flag_finish, //Fin de comunicacion
                  flag_ca1, //Modo complemento a 1
                  flag_25,  //Modo baja intensidad 
                  flag_off;  //Modo display apagado 
               
   
   /* Almacena el número a imprimir */
   unsigned char digit;
   
   /* Interfaz al módulo de tiempo */
   Tm_Num   n_periodo,
            n_to10,
            n_to2,
            n_to5;
   };
   
/* ======= Rutinas ======== */
/* Rutina para iniciar el módulo (su estructura de datos) */   
char Dp_Inicie (Dp_Control *dp, Tm_Num n_periodo, Tm_Num n_to10, Tm_Num n_to2, Tm_Num n_to5);
                  
/* Rutina para procesar el módulo (dentro del loop de polling) */				
void Dp_Procese (Dp_Control *dp);

/* ===== RUTINAS DE INTERFAZ ====== */
//Contiene la lógica para imprimir caracteres en el 7 segmentos
char print_digit(char digit);

//Se encarga de controlar la lógica que regula la impresión del digito de salida con base en las banderas
char send_digits_display(char digit, char flag_25, char flag_CA1, char flag_off, char flag_TO10);

/* == FIN DE RUTINAS DE INTERFAZ == */

#endif