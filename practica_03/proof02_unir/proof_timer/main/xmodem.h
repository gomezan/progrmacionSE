/* xmodem.h    Definiciones del módulo Display */
#if !defined(XMODEM_H)

#define  XMODEM_H

/*  10 segundos según Wikipedia, como base de tiempo de 360 Hz 
    para 10 segundos sería 10*360=3600 */
#define TICKS_TO_START_NAK 3600
/* 7 segundos según Wikipedia, como base de tiempo de 360 Hz
    para 10 segundos sería 7*360=2520 */
#define TICKS_TO_PAQ 2520
#define MAX_RETRANSMISSIONS 10

#include "Tiempo.h"
#include "Buffer.h"
#include "presentacion.h"
#include "Varios.h"
#include "uart_project.h"
#include "driver/gpio.h"

#define CONFIG_GPIO_OUTPUT_XMODEM     GPIO_NUM_22

typedef enum {INICIO,RX_PAQ,WAIT_DISPLAY,WAIT_PAQ,WAIT_PARTIAL_TO_PAQ} XM_STATE_T;

void XM_fsm_xmodem();

unsigned char paq_ok(uint8_t datos[], uint8_t num_paq);

#endif