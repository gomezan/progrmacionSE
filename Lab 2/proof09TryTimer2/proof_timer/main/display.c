/* display.c    implementaci�n del módulo Display */

#include "display.h"

extern Tm_Control c_tiempo;
extern Buffer_Control c_buff;

//Variables de funcionamiento
#define PERIODO_BASE    160
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


   dp->flag_eof=dp->flag_ca1 =dp->flag_25=dp->flag_off=0;   
   dp->digit =NUM_MUERTO;
   
   dp->n_periodo = n_periodo;
   if ( !Tm_Inicie_periodo(&c_tiempo, n_periodo, PERIODO_BASE) )
      return NO;
      
   dp->n_to10 = n_to10;
   dp->n_to5 = n_to5;
   dp->n_to2 = n_to2;

   return SI;
   };


//Se encarga de revizar que la lógica de la terminación de comunicación se realize de forma correcta.
  char Dp_logicaFinRecepcion(Dp_Control *dp, Buffer_Control *buf){

   if(Bf_Vacio(buf)){

        printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
      if(dp->flag_eof){

         if(Tm_Hubo_timeout(&c_tiempo, dp->n_to10)){
            dp->flag_finish=1;
         }

      return NO;

      }else{
         dp->flag_eof=1;
         Tm_Inicie_timeout(&c_tiempo, dp->n_to10, TIEMPO_EOF);
          return NO;
      } 

   }
   return SI;

  } 


//Se encarga de revizar que la lógica del apagado del display se realize de forma correcta.
  char Dp_logicaApagado(Dp_Control *dp){
  
  if(dp->flag_off){

      if(Tm_Hubo_timeout(&c_tiempo, dp->n_to2)){
         dp->flag_off=0;
      } else {
         return NO;
      }
  }

  return SI;
  } 


char print_digit(char digit)
{
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
    static unsigned char pins[7] = 
    {
        GPIO_OUTPUT_IO_0,
        GPIO_OUTPUT_IO_1,
        GPIO_OUTPUT_IO_2,
        GPIO_OUTPUT_IO_3,
        GPIO_OUTPUT_IO_4,
        GPIO_OUTPUT_IO_5,
        GPIO_OUTPUT_IO_6
    };

    printf("\n\n");
    for(int i = 6; i>=0; i--)
    {
        if((int)(lookup_table[(int)(digit)]) & (1 << i))
        {
            gpio_set_level(pins[i],0);
            printf("0");
        }
        else
        {
            gpio_set_level(pins[i],1);
            printf("1");
        }

    }
    printf("\n\nprinting = %d\n\n",(int)(lookup_table[(int)(digit)]));

    return TRUE;
/*
    if(lookup_table[digit] & 0x01) // bit 0 - a
    {
        gpio_set_level(GPIO_OUTPUT_IO_0, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_0, 1);
    }


    if(lookup_table[digit] & 0x02) // bit 1 - b
    {
        gpio_set_level(GPIO_OUTPUT_IO_1, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_1, 1);
    }

    if(lookup_table[digit] & 0x04) // bit 2 - c
    {
        gpio_set_level(GPIO_OUTPUT_IO_2, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_2, 1);
    }

    if(lookup_table[digit] & 0x08) // bit 3 - d
    {
        gpio_set_level(GPIO_OUTPUT_IO_3, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_3, 1);
    }

    if(lookup_table[digit] & 0x10) // bit 4 - e
    {
        gpio_set_level(GPIO_OUTPUT_IO_4, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_4, 1);
    }

    if(lookup_table[digit] & 0x20) // bit 5 - f
    {
        gpio_set_level(GPIO_OUTPUT_IO_5, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_5, 1);
    }

    if(lookup_table[digit] & 0x40) // bit 6 - g
    {
        gpio_set_level(GPIO_OUTPUT_IO_6, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_6, 1);
    } */
}

char send_digits_display(char digit, char flag_25, char flag_CA1, char flag_off, char flag_TO10)
{
    static int cont = 0;

    printf("digito: %d",digit);

    if(digit == 0x23 || digit == 0x24 || digit == 0x25 || digit == 0x26)
        return FALSE;

    if(flag_CA1 == 1)
        digit = ~digit;

    digit &= 0x0F;

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

    if(flag_off)
    {
        // Apagado
        digit = 0x10;
    }

    if(flag_TO10 == 1)
    {
        // guión
        digit = 0x11;
    }
    
    return print_digit(digit);

}

/* Rutina para procesar el módulo (dentro del loop de polling) */				
void Dp_Procese (Dp_Control *dp){

   Bf_data raw_digit;

   char on;
   char active;

   on=Dp_logicaApagado(dp);

   if(on){

      //Verificar el estado de fin de archivo
      active=Dp_logicaFinRecepcion(dp, &c_buff);
      if(active){

         //Verificar estado de timeout de 5 s (baja intensidad)
         if(dp->flag_25){
            if( Tm_Hubo_timeout(&c_tiempo, dp->n_to5)){
               dp->flag_25=0;
               Tm_Termine_timeout (&c_tiempo,dp->n_to5);
            }
         }

         //Leer caracter del buffer
         Bf_Bajar_Dato(&c_buff, &raw_digit);
         printf("baje %c",raw_digit);

         //Bajar bandera eof si esta encendida

         if(dp->flag_eof){
            dp->flag_eof=0;
            dp->flag_finish = 0;
            Tm_Termine_timeout(&c_tiempo,dp->n_to10);
         }

         //Identificar el caracter del buffer

         if(raw_digit=='&'){
         dp->flag_ca1=1;
 
         } else if(raw_digit=='$'){
         dp->flag_ca1=0;

         }  else if(raw_digit=='%'){
         Tm_Inicie_timeout(&c_tiempo, dp->n_to2, TIEMPO_OFF);
         dp->flag_off = 1;
         dp->digit=NUM_MUERTO;
   
         }  else if (raw_digit=='#'){

            if(dp->flag_25){

            Tm_Termine_timeout (&c_tiempo,dp->n_to5);
            Tm_Inicie_timeout(&c_tiempo, dp->n_to5, TIEMPO_LOW);

            } else{
            Tm_Inicie_timeout(&c_tiempo, dp->n_to5, TIEMPO_LOW);
            }

         dp->flag_25=1;
   
         // Si no es un caracter especial entonces es un digito crudo
         } else{
         dp->digit=raw_digit;
         }

      }     

   }

    Bf_data var;
    Bf_Libre(&c_buff,&var);

   //Se imprime cualquier digito dentro de la variable Digit
   printf("\n %d %d banderas: %d %d digito: %d",var,Bf_Vacio(&c_buff),dp->flag_eof,dp->flag_finish,dp->digit);
   send_digits_display(dp->digit,dp->flag_25,dp->flag_ca1,dp->flag_off,dp->flag_finish);

   };

/* ===== RUTINAS DE INTERFAZ ====== */
/* Rutina para activar un canal. Indica si se pudo activar. */


/* == FIN DE RUTINAS DE INTERFAZ == */
