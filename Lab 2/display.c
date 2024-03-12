/* Adc.c    implementaci�n del m�dulo Display */

#include "display.h"
#include <Tiempo.h>
#include <Rx.h>


extern Tm_Control c_tiempo;

/* Estados */
#define E_ESPERE_PERIODO      0
#define E_ESPERE_TO           1
#define E_ESPERE_EOC          2   

/* Rutina para iniciar el m�dulo (su estructura de datos) */   
char Dp_Inicie (Adc_Control *acp, 
                 (volatile unsigned char) *d_dato,
                 (volatile unsigned char) *d_control,
                 (volatile unsigned char) *d_estado,
                  Tm_Num n_periodo,
                  Tm_Contador periodo,
                  Tm_Num n_to,
                  Tm_Contador espera)
   {
   unsigned char n;
   
   acp->d_dato = d_dato;
   acp->d_control = d_control;
   acp->d_estado = d_estado;
                  
   acp->ch_activo = acp->ch_con_dato = 0;
   for (n = ADC_NUM_CANALES - 1; n; --n)
      /* Se inician los valores en 0 aunque no sea imprescindible hacerlo */
      acp->datos[n] = 0;
   
   acp->estado = E_ESPERE_PERIODO;
   acp->canal = 0;                                                                                                           
   *d_control = 0;   /* Registro de control con S&H abierto, sin EOC y canal en 0 */
   
   acp->n_periodo = n_periodo;
   acp->periodo = periodo;
   if ( !Tm_Inicie_periodo(&c_tiempo, n_periodo, periodo) )
      return NO;
      
   acp->n_to = n_to;
   acp->espera = espera;
   };
                  
/* Rutina para procesar el m�dulo (dentro del loop de polling) */				
void Dp_Procese (Adc_Control *acp)
   {
   switch (acp->estado)
      {
      case E_ESPERE_PERIODO:
         if ( Tm_Hubo_periodo(&c_tiempo, acp->n_periodo) )
            {
            Tm_Baje_periodo(&c_tiempo, acp->n_periodo);
            if (acp->ch_activo & (1 << adc->canal))
               {
               *(acp->d_control) = ADCCNTL_F_SAH | (ADCCNTL_F_CH & acp->canal);
               Tm_Inicie_timeout(&c_tiempo, acp->n_to, acp->espera);
               acp->estado = E_ESPERE_TO;
               }
               else
               {
               ++(adc->canal);
               if (adc->canal >= ADC_NUM_CANALES)
                  adc->canal = 0;
               };
            };
         break;
      case E_ESPERE_TO:
         if  ( Tm_Hubo_timeout(&c_tiempo, acp->n_to) )
            {
            *(acp->d_control) &= ~ADCCNTL_F_SAH;
            *(acp->d_control) |= ADCCNTL_F_SOC;
            acp->estado = E_ESPERE_EOC;
            };
         break;
      case E_ESPERE_EOC:
         if (*(adc->estado) & ADC_F_EOC)
            {
            acp->datos[adc->canal] = *(adc->d_dato);
            acp->ch_con_dato |= (1 << adc->canal);
            ++(adc->canal);
            if (adc->canal >= ADC_NUM_CANALES)
               adc->canal = 0;
            acp->estado = E_ESPERE_PERIODO;
            };
         break;
      };
   };

/* ===== RUTINAS DE INTERFAZ ====== */
/* Rutina para activar un canal. Indica si se pudo activar. */
char Adc_Active_canal (Adc_Control *acp, 
                       unsigned char canal)
   {
   if (canal >= ADC_NUM_CANALES)
      return NO;
   
   acp->ch_activo |= (1 << adc->canal);
   
   return SI;
   };                       

/* Rutina para desactivar un canal. Indica si se pudo desactivar. */
char Adc_Desactive_canal (Adc_Control *acp, 
                       unsigned char canal)
   {
   if (canal >= ADC_NUM_CANALES)
      return NO;
   
   acp->ch_activo &= ~(1 << adc->canal);
   
   return SI;
   };                       

/* Rutina para leer el dato de un canal. Indica si hay un dato v�lido. */
char Adc_Dato_canal (Adc_Control *acp, 
                     unsigned char canal,
                     unsigned char *dp)
   {
   if (canal >= ADC_NUM_CANALES)
      return NO;
   
   if (acp->ch_con_dato & (1 << adc->canal))
      {
      *dp = adc->datos[canal];
      return SI;
      };

   return NO;
   };                       

/* == FIN DE RUTINAS DE INTERFAZ == */
