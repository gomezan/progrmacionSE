/* presentacion.c    implementaci�n del módulo Presentacion */

#include "presentacion.h"
#include "display.h"


extern Tm_Control c_tiempo;
extern Dp_Control c_display;

#define PERIODO_BASE    160  //Hz


/* Rutina para iniciar el módulo (su estructura de datos) */   
char P_Inicie (P_Control *p, Tm_Num n_periodo)
   {

    //Inicializar variables
   p->flag_Finish=p->flag_Falla=0;

   /* Almacena el número a imprimir en las unidades */
   p->digitU=0;
   /* Almacena el número a imprimir en las decenas */
   p->digitD=0;
   /* Almacena el número a imprimir en las centenas */
   p->digitC=0;

   //Inicializar periodo
   p->n_periodo = n_periodo;
   if ( !Tm_Inicie_periodo(&c_tiempo, n_periodo, PERIODO_BASE) )
      return NO;      

   return SI;
   };

//Función encargada de actualizar el paquete de interes
char getPaquete(){

    return 0x0000;
}

//Función encargada de decodificar los números de los datos
void decodificar(P_Control *p, char digit){

    p->digitU=5;
    p->digitD=5;
    p->digitU=2;

}

/* Rutina para procesar el módulo (dentro del loop de polling) */				
void P_Procese (P_Control *p){

    char paquete=0;
    char data=0;

    //Leo el nuevo paquete
    paquete=getPaquete();
    //Obtener datos del paquete
    data=255;
    decodificar(p,data);
    //Actualizar valores del modulo de display
    //digitos
     update_u(&c_display,p->digitU);
     update_d(&c_display,p->digitD);
     update_c(&c_display,p->digitC);
     //banderas
     update_flag_finish(&c_display,p->flag_Finish);
     update_flag_fallas(&c_display,p->flag_Falla);

}

/* ===== RUTINAS DE INTERFAZ ====== */
/* Rutina para activar un canal. Indica si se pudo activar. */


//Actualza el flag de finalización de envío
char update_flag_finish(P_Control *p, char status_flag){

    p->flag_Finish=status_flag;
    return SI;
}
//Actualza el flag de fallas y titilación
char update_flag_fallas(P_Control *p, char status_flag){

    p->flag_Falla=status_flag;
    return SI;
}
/* == FIN DE RUTINAS DE INTERFAZ == */
