/* display.c    implementaci�n del módulo Display */

#include "display.h"

extern Tm_Control c_tiempo;

//Variables de funcionamiento
#define PERIODO_BASE    160  //Hz
#define NUM_MUERTO    -1
#define TIEMPO_OFF    320       // TO de 2  segundos
#define TIEMPO_EOF    1600     // TO de 10 segundos
#define TIEMPO_LOW    800       // TO de 5  segundos


/* Rutina para iniciar el módulo (su estructura de datos) */   
char Dp_Inicie (Dp_Control *dp, 
                   Tm_Num n_periodo, 
                   Tm_Num n_to10, 
                   Tm_Num n_to2, 
                   Tm_Num n_to5)
   {

    //Inicializar variables
   dp->flag_eof=dp->flag_ca1 =dp->flag_25=dp->flag_off=0;
   // Inicializar el número a imprimir    
   dp->digit =NUM_MUERTO;
   
   //Inicializar periodo
   dp->n_periodo = n_periodo;
   if ( !Tm_Inicie_periodo(&c_tiempo, n_periodo, PERIODO_BASE) )
      return NO;
      
   //número de timeouts   
   dp->n_to10 = n_to10;
   dp->n_to5 = n_to5;
   dp->n_to2 = n_to2;

   return SI;
   };



//Contiene la lógica para imprimir caracteres en el 7 segmentos
char print_digit(char digit)
{

    //Contiene los 18 carácteres que el sistema soporta imprimir en el 7 segmentos
    static char lookup_table[18] = {
        /* xgfe dcba */
        0x3F, // x011 1111 - 0
        0x06, // x000 0110 - 1
        0x5B, // x101 1011 - 2
        0x4F, // x100 1111 - 3
        0x66, // x110 0110 - 4
        0x6D, // x110 1101 - 5
        0x7D, // x111 1101 - 6
        0x07, // x000 0111 - 7
        0x7F, // x111 1111 - 8
        0x6F, // x110 1111 - 9
        0x77, // x111 0111 - A
        0x7C, // x111 1100 - B (b)
        0x39, // x011 1001 - C
        0x5E, // x101 1110 - D (d)
        0x79, // x111 1001 - E
        0x71, // x111 0001 - F
        0x00, // x000 0000 - off
        0x40  // x100 0000 - guión
    };

    

    //Encender pines del caracter especifico
    for(int i = 6; i>=0; i--)
    {

        //Determina el estado de los pines
        
        if((int)(lookup_table[(int)(digit)]) & (1 << i))
        {

            //Apagar pines
            gpio_set_level(pins[i],1);
            printf("0");
        }
        else
        {
            //Encender pines
            gpio_set_level(pins[i],0);
            printf("1");
        }

    }

    return TRUE;
}


//Se encarga de controlar la lógica que regula la impresión del digito de salida con base en las banderas
char send_digits_display(char digit, char flag_25, char flag_CA1, char flag_off, char flag_TO10)
{
    //Variable auxliar 
    static int cont = 0;

    //WTF
    if(digit == 0x23 || digit == 0x24 || digit == 0x25 || digit == 0x26)
        return FALSE;

    //Complemento a 1 
    if(flag_CA1 == 1)
        digit = ~digit;

    //WTF
    digit &= 0x0F;

    //Baja intensidad
    if(flag_25 == 1)
    {
        cont++;
        if(cont > 3)
        {
            cont = 0;
        }
        else
        {
            // Apagado
            digit = 0x10;
        }
    }
    else
    {
        cont = 0;
    }

    //Apagado de 2 segundos
    if(flag_off)
    {
        // Apagado
        digit = 0x10;
    }

    //Fin de envío
    if(flag_TO10 == 1)
    {
        // guión
        digit = 0x11;
    }
    
    return print_digit(digit);

}

/* Rutina para procesar el módulo (dentro del loop de polling) */				
void Dp_Procese (Dp_Control *dp){

printf("hola mundo");

}

/* ===== RUTINAS DE INTERFAZ ====== */
/* Rutina para activar un canal. Indica si se pudo activar. */


/* == FIN DE RUTINAS DE INTERFAZ == */
