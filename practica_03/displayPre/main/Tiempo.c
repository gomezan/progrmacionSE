/* Tiempo.c    Implementación del modulo de tiempo */
#if !defined(TIEMPO_C)

#define TIEMPO_C

#include "Tiempo.h"

/* Rutina para iniciar el módulo (su estructura de datos) */   
char Tm_Inicie (Tm_Control *tcp,
                Tm_Periodo *pp, 
                Tm_Num nper, 
                Tm_Timeout *tp, 
                Tm_Num nto,
                Tm_Atender *atender)
   {
   /* Tabla de períodos */
   tcp->pp = pp;
   tcp->nper = nper;
   for (; nper; ++pp, --nper)
      {
      pp->banderas = 0;
      pp->contador = pp->periodo = 0;
      };
   
   /* Tabla de timeouts/retardos */
   tcp->tp = tp;
   tcp->nto = nto;
   for (; nto; ++tp, --nto)
      *tp = 0;
   
   /* Rutina para manejar el timer (HW) */
   tcp->atender = atender;
   
   return SI;
   };


char Tm_Initialize_Timer_0()
{
   TIMERG0.hw_timer[0].config.tx_en=0;
   TIMERG0.hw_timer[0].config.tx_divider=50000;
   TIMERG0.hw_timer[0].config.tx_autoreload=0;
   TIMERG0.hw_timer[0].config.tx_increase=0;
   TIMERG0.hw_timer[0].load.val=1;
   TIMERG0.hw_timer[0].loadlo.val=10;
   TIMERG0.hw_timer[0].loadhi.val=0;
   TIMERG0.hw_timer[0].load.val=0;
   TIMERG0.hw_timer[0].config.tx_en=1;

   return SI;
}

   /* Rutina para atender el timer */
char Atender_timer (char atienda)
   {
    uint32_t read_lo, read_hi;
    TIMERG0.hw_timer[0].update.tx_update=1;
    read_lo = TIMERG0.hw_timer[0].lo.val;
    read_hi = TIMERG0.hw_timer[0].hi.val;
    TIMERG0.hw_timer[0].update.tx_update=0;


    if((read_lo == 0) && (read_hi == 0))
    {
        if(atienda)
        {
            TIMERG0.hw_timer[0].config.tx_en=0;
            TIMERG0.hw_timer[0].load.val=1;
            TIMERG0.hw_timer[0].loadlo.val=10;
            TIMERG0.hw_timer[0].loadhi.val=0;
            TIMERG0.hw_timer[0].load.val=0;
            TIMERG0.hw_timer[0].config.tx_en=1;
        }
        return SI;
    }
    else
        return NO;
};
                
/* Rutina para procesar el módulo (dentro del loop de polling) */				
void Tm_Procese (Tm_Control *tcp)
   {
   
   Tm_Num n;
   Tm_Periodo *pp;
   Tm_Timeout *tp;
   
   //Solo avanza cuando el HW así lo permite
   if ( !(tcp->atender(SI)) )
      return;
      
   //Actualizar periodos
   for (n = tcp->nper, pp = tcp->pp; n; ++pp, --n)
      if (pp->banderas & TM_PER_F_ACTIVO)
         {
         --(pp->contador);
         if ( !(pp->contador) )
            {
            pp->banderas |= TM_PER_F_FC;
            pp->contador = pp->periodo;
            };
         };

   // Actualizar timeouts
   for (n = tcp->nto, tp = tcp->tp; n; ++tp, --n)
      if (*tp)
         --(*tp);
   };

/* ===== RUTINAS DE INTERFAZ ====== */
/* Configurar un período para que empiece a funcionar */
char Tm_Inicie_periodo (Tm_Control *tcp, 
                        Tm_Num num_periodo,
                        Tm_Contador periodo)
   {
   Tm_Periodo *pp = tcp->pp + num_periodo;
   
   if (num_periodo >= tcp->nper)
      return NO;
      
   pp->banderas = TM_PER_F_ACTIVO;
   pp->contador = pp->periodo = periodo;
   
   return SI;
   };

/* Desactivar un período para que deje de funcionar */
void Tm_Termine_periodo (Tm_Control *tcp, Tm_Num num_periodo)
   {
   Tm_Periodo *pp = tcp->pp + num_periodo;
   
   if (num_periodo >= tcp->nper)
      return;

   pp->banderas &= ~TM_PER_F_ACTIVO;
   };

/* Verificar si hubo fin de conteo en un periodo */
char Tm_Hubo_periodo (Tm_Control *tcp, Tm_Num num_periodo)
   {
   Tm_Periodo *pp = tcp->pp + num_periodo;
   
   if (num_periodo >= tcp->nper)
      return NO;

   if (pp->banderas & TM_PER_F_FC)
      return SI;
   
   return NO;
   };

/* Bajar la bandera de conteo en un periodo */
void Tm_Baje_periodo (Tm_Control *tcp, Tm_Num num_periodo)
   {
   Tm_Periodo *pp = tcp->pp + num_periodo;
   
   if (num_periodo >= tcp->nper)
      return;

   pp->banderas &= ~TM_PER_F_FC;
   };

/* Configurar un timeout/retardo para que empiece a funcionar */
char Tm_Inicie_timeout (Tm_Control *tcp, Tm_Num num_timeout, Tm_Contador espera)
   {
   Tm_Timeout *tp = tcp->tp + num_timeout;
   
   if (num_timeout >= tcp->nto)
      return NO;
      
   *tp = espera;
   
   return SI;
   };

/* Desactivar un timeout/retardo para que deje de funcionar */
void Tm_Termine_timeout (Tm_Control *tcp, Tm_Num num_timeout)
   {
   Tm_Timeout *tp = tcp->tp + num_timeout;
   
   if (num_timeout >= tcp->nto)
      return;
      
   *tp = 0;
   };

/* Verificar si hubo timeout (o se completo el retardo) */
char Tm_Hubo_timeout (Tm_Control *tcp, Tm_Num num_timeout)
   {
   Tm_Timeout *tp = tcp->tp + num_timeout;
   
   if (num_timeout >= tcp->nto)
      return NO;
      
   if (*tp)
      return NO;

   return SI;
   };

/* == FIN DE RUTINAS DE INTERFAZ == */

#endif