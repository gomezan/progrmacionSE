/* Tiempo.c    Implementación del modulo de tiempo */
#if !defined(TIEMPO_C)

#define TIEMPO_C

#include "Tiempo.h"

extern Tm_Control c_tiempo;

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

/* Rutina para atender el timer */
char Atender_timer (char atienda)
   {
      
   if(!(TIMERG0.hw_timer[0].config.tx_alarm_en))
   {
      if(atienda)
      {
         TIMERG0.hw_timer[0].config.tx_alarm_en = 1;
      }
      return SI;
   }
   else
      return NO;
};

/* Rutina de atención de la interrupción */
void IRAM_ATTR t0isr(void)
{
    // Retrieve the interrupt status
    uint32_t timer_intr_status = TIMERG0.int_st_timers.t0_int_st;

    // Clear the interrupt status
   TIMERG0.int_clr_timers.t0_int_clr = 1;

    gpio_set_level(CONFIG_GPIO_OUTPUT_XMODEM, 0);
    Tm_Procese(&c_tiempo);
    gpio_set_level(CONFIG_GPIO_OUTPUT_XMODEM, 1);
    // Perform necessary operations based on the interrupt status
    if (timer_intr_status) {
        // Handle the alarm interrupt here
        // For example, toggle an LED or perform specific actions
    }

    // Reset the timer if needed
//    TIMERG0.hw_timer[0].config.tx_alarm_en = 1; // Re-enable the timer alarm
}

/* Rutina para inicializar el timer del sistema */
void Timer_Initialize (int group_timer, int timer_idx)
{

    timer_config_t config = {
        .divider = 22222,
        .counter_dir = TIMER_COUNT_DOWN,
        .counter_en = TIMER_PAUSE,
        .alarm_en = TIMER_ALARM_EN,
        .auto_reload = true,
    };
    timer_init(group_timer, timer_idx, &config);

   TIMERG0.hw_timer[timer_idx].config.tx_en=0; // disable timer 
   TIMERG0.hw_timer[timer_idx].config.tx_divider=22222; // Divide the clock by 22222
   TIMERG0.hw_timer[timer_idx].config.tx_autoreload=1; // autoreload activated
   TIMERG0.hw_timer[timer_idx].config.tx_increase=0; // decrease
   TIMERG0.hw_timer[timer_idx].load.val=1; // activate load
   TIMERG0.hw_timer[timer_idx].loadlo.val=10; // reload with low val equal to 10
   TIMERG0.hw_timer[timer_idx].loadhi.val=0; // reload with high val equal to 0
   TIMERG0.hw_timer[timer_idx].load.val=0; // deactivate load
   TIMERG0.hw_timer[timer_idx].alarmhi.val=0;
    TIMERG0.hw_timer[timer_idx].alarmlo.val=0;
   TIMERG0.hw_timer[timer_idx].config.tx_alarm_en=1;
   TIMERG0.hw_timer[timer_idx].config.tx_level_int_en=1;

    timer_enable_intr(group_timer,timer_idx);
    timer_isr_register(group_timer, timer_idx, t0isr, NULL, ESP_INTR_FLAG_IRAM, NULL);

   TIMERG0.hw_timer[timer_idx].config.tx_en=1; // enable timer
}    

                
/* Rutina para procesar el módulo (dentro del loop de polling) */				
void Tm_Procese (Tm_Control *tcp)
   {
   Tm_Num n;
   Tm_Periodo *pp;
   Tm_Timeout *tp;

   //printf("Entra Timer Procese\n");
   
   if ( !(tcp->atender(SI)) )
      return;

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

   //printf("Periodo %d\n",pp->banderas & TM_PER_F_FC);
   
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