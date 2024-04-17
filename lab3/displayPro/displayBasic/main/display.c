/* display.c    implementaci�n del módulo Display */

#include "display.h"


extern Tm_Control c_tiempo;

#define PERIODO_BASE    160  //Hz
#define NUM_MUERTO    0x10   /// Apagado
#define PERIODO_T1    3       // Divisor a 1 Hz
#define PERIODO_T2    6     // Divisor a 2 Hz


/* Rutina para iniciar el módulo (su estructura de datos) */   
char Dp_Inicie (Dp_Control *dp, Tm_Num n_periodo)
   {

    //Inicializar variables
   dp->flag_Finish=dp->flag_Falla=0;
   // Inicializar los números a imprimir    
   dp->digitU =NUM_MUERTO;
   dp->digitD =NUM_MUERTO;
   dp->digitC =NUM_MUERTO;

   // Inica la posición nula
   dp->mul =0x00;
   
   //Inicializar periodo
   dp->n_periodo = n_periodo;
   if ( !Tm_Inicie_periodo(&c_tiempo, n_periodo, PERIODO_BASE) )
      return NO;      

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
char send_digits_display(char digit, char flag_falla, char flag_finish)
{
    //Variable auxliar 
    static int cont = 0;

    //WTF
    if(digit == 0x23 || digit == 0x24 || digit == 0x25 || digit == 0x26)
        return FALSE;


    digit &= 0x0F;

    //Baja intensidad
    if(flag_falla == 1)
    {
        cont++;
        if(cont > PERIODO_T1)
        {
            cont = 0;
        }
        else
        {
            digit = NUM_MUERTO;
        }
    }
    else
    {
        cont = 0;
    }

        if(flag_falla == 2)
    {
        cont++;
        if(cont > PERIODO_T2)
        {
            cont = 0;
        }
        else
        {
            digit = NUM_MUERTO;
        }
    }
    else
    {
        cont = 0;
    }


    //Fin de envío
    if(flag_finish == 1)
    {
        // guión
        digit = 0x11;
    }
    
    return print_digit(digit);

}

/* Rutina para procesar el módulo (dentro del loop de polling) */				
void Dp_Procese (Dp_Control *dp){

    char digit;

    //Incrmento la posición del multiplexor
    ++dp->mul;
    //Revisar posible desborde
    if(dp->mul>3){
        dp->mul=1;
    }
    switch(dp->mul){

        case(1):
            digit=dp->digitU;
            gpio_set_level(CONFIG_GPIO_DIGIT_0,0);
            gpio_set_level(CONFIG_GPIO_DIGIT_1,1);
            gpio_set_level(CONFIG_GPIO_DIGIT_2,1);
        break;

        case(2):
            digit=dp->digitD;
            gpio_set_level(CONFIG_GPIO_DIGIT_0,1);
            gpio_set_level(CONFIG_GPIO_DIGIT_1,0);
            gpio_set_level(CONFIG_GPIO_DIGIT_2,1);
        break;

        case(3):
            digit=dp->digitC;
            gpio_set_level(CONFIG_GPIO_DIGIT_0,1);
            gpio_set_level(CONFIG_GPIO_DIGIT_1,1);
            gpio_set_level(CONFIG_GPIO_DIGIT_2,0);
        break;

        default:

            digit=NUM_MUERTO;
            gpio_set_level(CONFIG_GPIO_DIGIT_0,1);
            gpio_set_level(CONFIG_GPIO_DIGIT_1,1);
            gpio_set_level(CONFIG_GPIO_DIGIT_2,0);
        break;


    }

    //
    send_digits_display(digit, dp->flag_Falla, dp->flag_Finish);

}

/* ===== RUTINAS DE INTERFAZ ====== */
/* Rutina para activar un canal. Indica si se pudo activar. */

//Actualza el digito presenta en las unidades
char update_u(Dp_Control *dp,char digit){

    if(digit>9)
        return NO;

    dp->digitU=digit;
    return SI;
}
//Actualza el digito presenta en las decenas
char update_d(Dp_Control *dp,char digit){

    if(digit>9)
        return NO;

    dp->digitD=digit;
    return SI;
}
//Actualza el digito presenta en las centenas
char update_c(Dp_Control *dp, char digit){

    if(digit>9)
        return NO;

    dp->digitC=digit;
    return SI;
}
//Actualza el flag de finalización de envío
char update_flag_finish(Dp_Control *dp, char status_flag){

    dp->flag_Finish=status_flag;
    return SI;
}
//Actualza el flag de fallas y titilación
char update_flag_fallas(Dp_Control *dp, char status_flag){

    dp->flag_Falla=status_flag;
    return SI;
}
/* == FIN DE RUTINAS DE INTERFAZ == */
