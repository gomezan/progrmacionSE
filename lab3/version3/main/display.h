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

//Estados de funcionamiento
#define posU        0xFE; //1111 1110
#define posD        0xFD; //1111 1101
#define posC        0xFB; //1111 1011


typedef struct Dp_Control Dp_Control;
//Estructura de control de display
struct Dp_Control
{                  
   /* Banderas */
   unsigned char  flag_Falla, //Falla de comunicacion
                  flag_Finish; //Fin de comunicaci
               
   
   /* Almacena el número a imprimir en las unidades */
    unsigned char digitU;
   /* Almacena el número a imprimir en las decenas */
   unsigned char digitD;
   /* Almacena el número a imprimir en las centenas */
   unsigned char digitC;

   /* Almacena la posiciión de multiplexación */
   unsigned char mul;
   
   /* Interfaz al módulo de tiempo */
   Tm_Num   n_periodo;

   };
   
/* ======= Rutinas ======== */
/* Rutina para iniciar el módulo (su estructura de datos) */   
char Dp_Inicie (Dp_Control *dp, Tm_Num n_periodo);
                  
/* Rutina para procesar el módulo (dentro del loop de polling) */				
void Dp_Procese (Dp_Control *dp);

/* ===== RUTINAS DE INTERFAZ ====== */
//Contiene la lógica para imprimir caracteres en el 7 segmentos de tres posiciones
char print_digit(char digit);

//Se encarga de controlar la lógica que regula la impresión del digito de salida con base en las banderas
char send_digits_display(char digit, char flag_falla, char flag_finish);

//Actualza el digito presenta en las unidades
char update_u(Dp_Control *dp, char digit);
//Actualza el digito presenta en las decenas
char update_d(Dp_Control *dp,char digit);
//Actualza el digito presenta en las centenas
char update_c(Dp_Control *dp, char digit);
//Actualza el flag de finalización de envío
char update_flag_finish(Dp_Control *dp, char status_flag);
//Actualza el flag de fallas y titilación
char update_flag_fallas(Dp_Control *dp, char status_flag);

/* == FIN DE RUTINAS DE INTERFAZ == */

#endif