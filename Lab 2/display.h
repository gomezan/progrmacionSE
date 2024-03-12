/* Adc.h    Definiciones del m�dulo Display */
#if !defined(DISPLAY_H)

#define  DISPLAY_H

#include <Tiempo.h>

typedef struct Dp_Control Dp_Control;

struct Dp_Control
                  
   /* Banderas */
   unsigned char  flag_ca1, //Modo complemento a 1
                  flag_li;  //Modo baja intensidad 
               
   
   /* Control de la m�quina de estados */
   char estado;
   
   /* Interfaz al m�dulo de tiempo */
   Tm_Num   n_periodo,
            n_to10;
            n_to2;
            n_to5;
   Tm_Contador periodo;
   };
   
/* ======= Rutinas ======== */
/* Rutina para iniciar el m�dulo (su estructura de datos) */   
char Adc_Inicie (Adc_Control *acp, (volatile unsigned char) *d_dato,
                 (volatile unsigned char) *d_control, (volatile unsigned char) *d_estado,
                  Tm_Num n_periodo, Tm_Contador periodo, Tm_Num n_to, Tm_Contador espera);
                  
/* Rutina para procesar el m�dulo (dentro del loop de polling) */				
void Adc_Procese (Adc_Control *acp);

/* ===== RUTINAS DE INTERFAZ ====== */
/* Rutina para activar un canal. Indica si se pudo activar. */
char Adc_Active_canal (Adc_Control *acp, unsigned char canal);

/* Rutina para desactivar un canal. Indica si se pudo desactivar. */
char Adc_Desactive_canal (Adc_Control *acp, unsigned char canal);

/* Rutina para leer el dato de un canal. Indica si hay un dato v�lido. */
char Adc_Dato_canal (Adc_Control *acp, unsigned char canal, unsigned char *dp);

/* == FIN DE RUTINAS DE INTERFAZ == */

#endif