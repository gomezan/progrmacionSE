/* button.h    Definiciones del m�dulo de botones */
#if !defined(BUTTON_H)

#define  BUTTON_H

#include "Tiempo.h"
#include "presentacion.h"
#include "Varios.h"
#include "driver/gpio.h"
//No olvidar!!!!!!
#include "display.h"

// Variables para manejar los pines del display
#define CONFIG_GPIO_INPUT_0     GPIO_NUM_35
#define CONFIG_GPIO_INPUT_1     GPIO_NUM_32

//
typedef char Bt_procesar ();

typedef struct Bt_Control Bt_Control;
//Estructura de control de display
struct Bt_Control
{                  
   /* Banderas */
   unsigned char  flag_preionado; //Bandera para saber si el botón fue presionado
   
   /* Almacena el estado previo con el fin de identificar el flanco de subida*/
    unsigned char estadoPrevio;

   /* Almacena el GPIO de lectura */
   int puerto;

   /* Rutina para manejar el timer (HW) */
   Bt_procesar *proc;
   
   /* Interfaz al módulo de tiempo */
   Tm_Num   n_periodo;
   Tm_Num   n_to;

   };
   
/* ======= Rutinas ======== */
/* Rutina para iniciar el módulo (su estructura de datos) */   
char Bt_Inicie (Bt_Control *bt, int puerto ,Tm_Num n_periodo, Tm_Num n_to, Bt_procesar *proc);
                  
/* Rutina para procesar el módulo (dentro del loop de polling) */				
void Bt_Procese (Bt_Control *bt);

/* ===== RUTINAS DE INTERFAZ ====== */

//Interfaz que permite incrementar el valor de N
void increment_N();

//Interfaz que permite decrementar el valor de N
void decrement_N();

/* == FIN DE RUTINAS DE INTERFAZ == */

#endif