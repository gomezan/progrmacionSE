/* display.h    Definiciones del m�dulo Display */
#if !defined(DISPLAY_H)

#define  DISPLAY_H

#include <Tiempo.h>

typedef struct Dp_Control Dp_Control;

struct Dp_Control
                  
   /* Banderas */
   unsigned char  flag_eof, //Fin de archivo
                  flag_finish, //Fin de comunicacion
                  flag_ca1, //Modo complemento a 1
                  flag_25,  //Modo baja intensidad 
                  flag_off;  //Modo display apagado 
               
   
   /* Control de la m�quina de estados */
   unsigned char digit;
   
   
   /* Interfaz al m�dulo de tiempo */
   Tm_Num   n_periodo,
            n_to10,
            n_to2,
            n_to5;
   };
   
/* ======= Rutinas ======== */
/* Rutina para iniciar el m�dulo (su estructura de datos) */   
char Dp_Inicie (Dp_Control *dp, Tm_Num n_periodo, Tm_Num n_to10, Tm_Num n_to2, Tm_Num n_to5);
                  
/* Rutina para procesar el m�dulo (dentro del loop de polling) */				
void Dp_Procese (Adc_Control *acp);

/* ===== RUTINAS DE INTERFAZ ====== */
//Pendiente

/* == FIN DE RUTINAS DE INTERFAZ == */

#endif