/* display.h    Definiciones del m�dulo Display */
#if !defined(DISPLAY_H)

#define  DISPLAY_H

#include "Tiempo.h"
#include "Buffer.h"
#include "Varios.h"
#include "driver/gpio.h"

// Variables para manejar los pines del display
#define GPIO_OUTPUT_IO_0    CONFIG_GPIO_OUTPUT_0
#define GPIO_OUTPUT_IO_1    CONFIG_GPIO_OUTPUT_1
#define GPIO_OUTPUT_IO_2    CONFIG_GPIO_OUTPUT_2
#define GPIO_OUTPUT_IO_3    CONFIG_GPIO_OUTPUT_3
#define GPIO_OUTPUT_IO_4    CONFIG_GPIO_OUTPUT_4
#define GPIO_OUTPUT_IO_5    CONFIG_GPIO_OUTPUT_5
#define GPIO_OUTPUT_IO_6    CONFIG_GPIO_OUTPUT_6

typedef struct Dp_Control Dp_Control;

struct Dp_Control
{                  
   /* Banderas */
   unsigned char  flag_eof, //Fin de archivo
                  flag_finish, //Fin de comunicacion
                  flag_ca1, //Modo complemento a 1
                  flag_25,  //Modo baja intensidad 
                  flag_off;  //Modo display apagado 
               
   
   /* Almacena el número a imprimir */
   Bf_data digit;
   
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
char print_digit(char digit);

char send_digits_display(char digit, char flag_25, char flag_CA1, char flag_off, char flag_TO10);

/* == FIN DE RUTINAS DE INTERFAZ == */

#endif