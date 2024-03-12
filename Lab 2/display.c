/* display.c    implementaci�n del m�dulo Display */

#include "display.h"
#include <Tiempo.h>
#include <Rx.h>


extern Tm_Control c_tiempo;

//Variables de funcionamiento
#define PERIODO_BASE    160
#define NUM_MUERTO    -1
#define TIEMPO_OFF    320
#define TIEMPO_EOF    16000
#define TIEMPO_LOW    800


/* Rutina para iniciar el m�dulo (su estructura de datos) */   
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
  char Dp_logicaFinRecepcion(Dp_Control *dp, unsigned int tam){

   if(!(tam)){

      if(dp->flag_eof){

         if(Tm_Hubo_timeout(&c_tiempo, dp->n_to10)){
            dp->finish=1;
         }

      return No;

      }else{
         dp->flag_eof=1;
         Tm_Inicie_timeout(&c_tiempo, acp->n_to10, TIEMPO_EOF);
          return No;
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
                  
/* Rutina para procesar el m�dulo (dentro del loop de polling) */				
void Dp_Procese (Dp_Control *dp){

   unsigned char raw_digit;
   
   
   //Verificar si es necesario enviar un XON
   //PENDIENTE
   unsigned int tam=getTamBuffer();

   if(tam<=24){
     sendXON() 
   }
   //Verificar estado de timeout de 2 s (display apagado)
   char on=Dp_logicaApagado(dp);
   
   
   if(on){

      //Verificar el estado de fin de archivo

      char inactive=Dp_logicaFinRecepcion(dp,tam);
      if(inactive){

         //Verificar estado de timeout de 5 s (baja intensidad)

         if(dp->flag_25){
            if( Tm_Hubo_timeout(&c_tiempo, acp->n_to5)){
               dp->flag_eof=0;
               Tm_Termine_timeout (&c_tiempo,acp->n_to5);
               db->flag_finish=0;

            }
         }

         //Leer caracter del buffer
         //PENDIENTE
         raw_digit=requestBuffer();

         //Bajar bandera eof si esta encendida

         if(dp->flag_eof){
            dp->flag_eof=0;

         }

         //Identificar el caracter del buffer

         if(raw_digit=="&"){
         dp->flag_ca1=1;
 
         } else if(raw_digit=="$"){
         dp->flag_ca1=0;

         }  else if(raw_digit=="%"){
         Tm_Inicie_timeout(&c_tiempo, acp->n_to2, TIEMPO_OFF);
         dp->digit=NUM_MUERTO;
   
         }  else if (raw_digit=="#"){

            if(dp->flag_25){

            Tm_Termine_timeout (&c_tiempo,acp->n_to5);
            Tm_Inicie_timeout(&c_tiempo, acp->n_to5, TIEMPO_LOW);

            } else{
            Tm_Inicie_timeout(&c_tiempo, acp->n_to5, TIEMPO_LOW);
            }

         dp->flag_25=1;
   
         // Si no es un caracter especial entonces es un digito crudo
         } else{
         dp->digit=raw_digit;
         }

      }     

   }

   //Se imprime cualquier digito dentro de la variable Digit
   sendDigitDisplay();

   };

/* ===== RUTINAS DE INTERFAZ ====== */
/* Rutina para activar un canal. Indica si se pudo activar. */


/* == FIN DE RUTINAS DE INTERFAZ == */
