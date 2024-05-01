/* presentacion.c    implementaci�n del módulo Presentacion */

#include "presentacion.h"


extern Tm_Control c_tiempo;
extern Dp_Control c_display;
extern Buffer_Control  c_buff;

#define PERIODO_BASE    4  // 120 Hz


/* Rutina para iniciar el módulo (su estructura de datos) */   
char P_Inicie (P_Control *p, Tm_Num n_periodo, Tm_Num n_to)
   {

    //Inicializar variables
   p->flag_Finish=p->flag_Falla=p->flag_vacio, p->flag_display=0;

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


    p->n_to = n_to;      

   return SI;
   };

//Función encargada de actualizar el paquete de interes
char getPaquete(char *dp){
    Bf_data llega_dato;
    
    if(Bf_Bajar_Dato(&c_buff,&llega_dato))
    {
        *dp = llega_dato;
        return 1;
    }

    return 0;
}

//Función encargada de decodificar los números de los datos
void decodificar(P_Control *p, char digit){

    p->digitU = (char)(digit%10);
    p->digitD = (char)((digit/10)%10);
    p->digitC = (char)(digit/100);

    /*
    p->digitU=5;
    p->digitD=5;
    p->digitC=2;
    */

}

/* Rutina para procesar el módulo (dentro del loop de polling) */				
void P_Procese (P_Control *p){

    char paquete=0;

    //Leo el nuevo paquete
    getPaquete(&paquete);
    //extraer u,d y c
    decodificar(p,paquete);
    //Actualizar valores del modulo de display
    //digitos
     update_u(&c_display,p->digitU);
     update_d(&c_display,p->digitD);
     update_c(&c_display,p->digitC);
     //bandera de fin de envio
     update_flag_finish(&c_display,p->flag_Finish);

    //bandera de falla de envio
    if(p->flag_Falla){

        if(Bf_Vacio(&c_buff)){

            if(!p->flag_vacio){
                p->flag_vacio=1;
                p->flag_display=1;
                Tm_Inicie_timeout (&c_tiempo,p->n_to,10);
            }

            if(p->flag_display==1){

                if(Tm_Hubo_timeout(&c_tiempo,p->n_to)){
                p->flag_display=2;
            }

            }

        }else{
            p->flag_vacio=0;
            Tm_Termine_timeout (&c_tiempo,p->n_to);
            p->flag_display=0;
        } 
    } else{
        p->flag_display=0;
    }

     update_flag_fallas(&c_display,p->flag_display);

}

/* ===== RUTINAS DE INTERFAZ ====== */
/* Rutina para activar un canal. Indica si se pudo activar. */


//Actualza el flag de finalización de envío
char update_finish(P_Control *p, char status_flag){

    p->flag_Finish=status_flag;
    return SI;
}
//Actualza el flag de fallas y titilación
char update_fallas(P_Control *p, char status_flag){

    p->flag_Falla=status_flag;
    return SI;
}
/* == FIN DE RUTINAS DE INTERFAZ == */
