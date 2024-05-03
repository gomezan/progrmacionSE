/* button.c    implementación del módulo de botones */

#include "button.h"

extern Tm_Control c_tiempo;

#define PERIODO_BASE 10 // 36 Hz
#define DEBOUNCE_TO 5   // Debouncing 

/* Rutina para iniciar el módulo (su estructura de datos) */
char Bt_Inicie(Bt_Control *bt, int puerto ,Tm_Num n_periodo, Tm_Num n_to, Bt_procesar *proc)
{

    // Inicializar variables
    bt->flag_preionado=0;
    // Inicializar los números a imprimir
    bt->estadoPrevio =0;

    // Inicializar el puerto de entrada
    bt->puerto = puerto;

    //Asignar la rutina de procesamiento
     bt->proc = proc;

    // Inicializar periodo
    bt->n_periodo = n_periodo;
    if (!Tm_Inicie_periodo(&c_tiempo, n_periodo, PERIODO_BASE))
        return NO;

    //Inicializar timeout
     bt->n_to = n_to;

    return SI;
};


void increment_N(){
    print_digit(0);
}

void decrement_N(){
    print_digit(1);
}


/* Rutina para procesar el módulo (dentro del loop de polling) */
void Bt_Procese(Bt_Control *bt)
{

    int estado =gpio_get_level(bt->puerto);

    if(bt->flag_preionado){

        if(Tm_Hubo_timeout(&c_tiempo,bt->n_to)){
                bt->flag_preionado=0;
                bt->estadoPrevio=estado;
                //Actualizar N
                bt->proc();
            }

    } else {

        if((estado==1) && (bt->estadoPrevio==0)){

            bt->flag_preionado=1;
            Tm_Inicie_timeout (&c_tiempo,bt->n_to,DEBOUNCE_TO);
        }

        bt->estadoPrevio=estado;

    }

}

/* ===== RUTINAS DE INTERFAZ ====== */
/* Rutina para activar un canal. Indica si se pudo activar. */


/* == FIN DE RUTINAS DE INTERFAZ == */
