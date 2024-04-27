/* presentacion.h    Definiciones del m�dulo Presentacion */
#if !defined(PRESENTACION_H)

#define  PRESENTACION_H

#include "Tiempo.h"
#include "Varios.h"

typedef struct P_Control P_Control;
//Estructura de control de display
struct P_Control
{                  
   /* Banderas */
   unsigned char  flag_Falla, //Falla de comunicacion
                  flag_Finish; //Fin de comunicación

    /* Almacena el número a imprimir en las unidades */
   unsigned char digitU;
   /* Almacena el número a imprimir en las decenas */
   unsigned char digitD;
   /* Almacena el número a imprimir en las centenas */
   unsigned char digitC;
               
   /* Interfaz al módulo de tiempo */
   Tm_Num   n_periodo;

   };
   
/* ======= Rutinas ======== */
/* Rutina para iniciar el módulo (su estructura de datos) */   
char P_Inicie (P_Control *p, Tm_Num n_periodo);
                  
/* Rutina para procesar el módulo (dentro del loop de polling) */				
void P_Procese (P_Control *p);

/* ===== RUTINAS DE INTERFAZ ====== */

//Actualza el digito presenta en las unidades

//Actualza el flag de fin de archivo
char update_flag_finish(P_Control *p, char status_flag);
//Actualza el flag de fallas y titilación
char update_flag_fallas(P_Control *p, char status_flag);

/* == FIN DE RUTINAS DE INTERFAZ == */

#endif