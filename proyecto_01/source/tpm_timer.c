/*
 * The Clear BSD License
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted (subject to the limitations in the disclaimer below) provided
 *  that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of the copyright holder nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE GRANTED BY THIS LICENSE.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_tpm.h"

#include "fsl_common.h"
#include "pin_mux.h"
#include "clock_config.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* Output */

// BOARD_INITPINS_PIN_D2_GPIO
// BOARD_INITPINS_PIN_D2_PIN

// BOARD_INITPINS_PIN_D4_GPIO
// BOARD_INITPINS_PIN_D4_PIN

// BOARD_INITPINS_PIN_D6_GPIO
// BOARD_INITPINS_PIN_D6_PIN

/* Input */
// BOARD_INITPINS_PIN_D3_GPIO
// BOARD_INITPINS_PIN_D3_PIN

// BOARD_INITPINS_PIN_B0_GPIO
// BOARD_INITPINS_PIN_B0_PIN

// BOARD_INITPINS_PIN_B1_GPIO
// BOARD_INITPINS_PIN_B1_PIN

// BOARD_INITPINS_PIN_B2_GPIO
// BOARD_INITPINS_PIN_B2_PIN

/* define instance */
#define BOARD_TPM TPM0
/* Interrupt number and interrupt handler for the TPM instance used */
#define BOARD_TPM_IRQ_NUM TPM0_IRQn
#define BOARD_TPM_HANDLER TPM0_IRQHandler
/* Get source clock for TPM driver */
#define TPM_SOURCE_CLOCK (CLOCK_GetFreq(kCLOCK_PllFllSelClk)/4)

//#define TPM_SOURCE_CLOCK (CLOCK_GetFreq(kCLOCK_Er32kClk))

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile bool tpmIsrFlag = false;
volatile uint32_t milisecondCounts = 0U;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */

// recordar que la lógica está inversa
// el común va a Vcc
// para encender el 7 segmentos, el segmento debe estar en 0V
void on_sec_1() // enciende leds de sección 1
{
	// GPIO_PortToggle(BOARD_INITPINS_PIN_D2_GPIO, 1u << BOARD_INITPINS_PIN_D2_PIN);
	GPIO_PortClear(BOARD_INITPINS_PIN_D2_GPIO, 1u << BOARD_INITPINS_PIN_D2_PIN);
}

void on_sec_2() // enciende leds de sección 2
{
    // GPIO_PortToggle(BOARD_INITPINS_PIN_D4_GPIO, 1u << BOARD_INITPINS_PIN_D4_PIN);
	GPIO_PortClear(BOARD_INITPINS_PIN_D4_GPIO, 1u << BOARD_INITPINS_PIN_D4_PIN);
}

void on_sec_3() // enciende leds de sección 3
{
    // GPIO_PortToggle(BOARD_INITPINS_PIN_D6_GPIO, 1u << BOARD_INITPINS_PIN_D6_PIN);
	GPIO_PortClear(BOARD_INITPINS_PIN_D6_GPIO, 1u << BOARD_INITPINS_PIN_D6_PIN);
}

void off_sec_1() // apaga leds de sección 1
{
	// on_sec_1();
	GPIO_PortSet(BOARD_INITPINS_PIN_D2_GPIO, 1u << BOARD_INITPINS_PIN_D2_PIN);
}

void off_sec_2() // apaga leds de sección 2
{
	// on_sec_2();
	GPIO_PortSet(BOARD_INITPINS_PIN_D4_GPIO, 1u << BOARD_INITPINS_PIN_D4_PIN);
}

void off_sec_3() // apaga leds de sección 3
{
	// on_sec_3();
	GPIO_PortSet(BOARD_INITPINS_PIN_D6_GPIO, 1u << BOARD_INITPINS_PIN_D6_PIN);
}

int pulse_1() // revisa switch 1, si es HIGH devuelve 1, si es FALSE devuelve 0
{
	if(GPIO_PinRead(BOARD_INITPINS_PIN_B0_GPIO,BOARD_INITPINS_PIN_B0_PIN))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int pulse_2() // revisa switch 2, si es HIGH devuelve 1, si es FALSE devuelve 0
{
	if(GPIO_PinRead(BOARD_INITPINS_PIN_B1_GPIO,BOARD_INITPINS_PIN_B1_PIN))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int pulse_3() // revisa switch 3, si es HIGH devuelve 1, si es FALSE devuelve 0
{
    if(GPIO_PinRead(BOARD_INITPINS_PIN_B2_GPIO,BOARD_INITPINS_PIN_B2_PIN))
    {
        return 1;
    }
    else
    {
    	return 0;
    }
}

int main(void)
{
    uint32_t cnt = 0;
    uint32_t loop = 5;
    //uint32_t secondLoop = 1000U;
    //const char *signals = "-|";
    tpm_config_t tpmInfo;

    /* Board pin, clock, debug console init */
    BOARD_InitPins();
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    /* Select the clock source for the TPM counter as kCLOCK_PllFllSelClk */
    CLOCK_SetTpmClock(1U);

    TPM_GetDefaultConfig(&tpmInfo);

#ifndef TPM_PRESCALER
#define TPM_PRESCALER kTPM_Prescale_Divide_128
#endif

    /* TPM clock divide by TPM_PRESCALER */
    tpmInfo.prescale = TPM_PRESCALER;

    /* Initialize TPM module */
    TPM_Init(BOARD_TPM, &tpmInfo);

    /*
     * Set timer period.
     */
    
    TPM_SetTimerPeriod(BOARD_TPM, 10000000);
	//Configura interrupciones con base en el reloj
    TPM_EnableInterrupts(BOARD_TPM, kTPM_TimeOverflowInterruptEnable);
    //Habilitar interrupciones
    EnableIRQ(BOARD_TPM_IRQ_NUM);
	//Configurar reloj
    TPM_StartTimer(BOARD_TPM, kTPM_SystemClock);

    cnt = 0;

	// Estados para la máquina de estados a usar
	// EVALUATING - el estado donde se evalúa los distintos switches
	// ON_# - estados donde se enciende cada sección y se espera al estado apagado
	// OFF_# - estados donde se apaga cada sección y se evalúa la sección siguiente
	//         Si ninguna sección está encendida se vuelve al estado EVALUATING

    typedef enum {EVALUATING,ON_1,OFF_1,ON_2,OFF_2,ON_3,OFF_3} STATE_T;
    STATE_T state = EVALUATING;
    while (true)
    {
    	if(tpmIsrFlag) // Se cumple la interrupción cada 100 ms
    	{
    		cnt++; // Se suma el contador
    		if (cnt >= loop) // Si el contador llega a 5 se ejecuta la FSM, eso es cada 500 ms
			{
				cnt = 0; // Se reinicia el contador
				switch(state)
				{
					case EVALUATING: // Se evalúa si algún switch sección está encendida
						if(pulse_1() == 1)
						{
							// Si el switch 1 está encendido
							on_sec_1();   // Se enciende la sección 1
							state = ON_1; // Se actualiza el estado a ON_1
						}
						else if(pulse_2() == 1)
						{
							// Si el switch 2 está encendido
							on_sec_2();   // Se enciende la sección 2
							state = ON_2; // Se actualiza el estado a ON_2
						}
						else if(pulse_3() == 1)
						{
							// Si el switch 3 está encendido
							on_sec_3();   // Se enciende la sección 3
							state = ON_3; // Se actualiza el estado a ON_3
						}
						else
						{
							// En cualquier otro caso se repite el estado
							state = EVALUATING;
						}
					break;

					case ON_1:
						// Como la ejecución es 500 ms después se apaga la sección 1
						off_sec_1();
						state = OFF_1; // Se actualiza el estado a OFF_1
					break;

					case OFF_1:
						// 500 ms después de apagar la sección se debe cambiar de sección
						if(pulse_2() == 1)
						{
							// Si el switch 2 está encendido
							on_sec_2();   // Se enciende la sección 2
							state = ON_2; // Se actualiza al estado ON_2
						}
						else if(pulse_3() == 1)
						{
							// Si el switch 3 está encendido y el 2 no
							on_sec_3();   // Se enciende la sección 3
							state = ON_3; // Se actualiza al estado ON_3
						}
						else if(pulse_1() == 1)
						{
							// Si el switch 1 está encendido y los demás no
							on_sec_1();   // Se enciende la sección 1
							state = ON_1; // Se actualiza al estado ON_1
						}
						else
						{
							// En cualquier otro caso, con ningún switch encendido
							state = EVALUATING; // Se actualiza al estado EVALUATING
						}
					break;

					case ON_2:
						// Como la ejecución es cada 500 ms
						off_sec_2();   // Se apaga la sección 2
						state = OFF_2; // Se actualiza al estado OFF_2
					break;

					case OFF_2:
						// 500 ms después de apagar la sección se debe cambiar de sección
						if(pulse_3() == 1)
						{
							// Si el switch 3 está encendido
							on_sec_3();   // Se enciende la sección 3
							state = ON_3; // Se actualiza al estado ON_3
						}
						else if(pulse_1() == 1)
						{
							// Si el switch 1 está encendido y el 3 no
							on_sec_1();   // Se enciende la sección 1
							state = ON_1; // Se actualiza al estado ON_1
						}
						else if(pulse_2() == 1)
						{
							// Si el switch 2 está encendido y los demás no
							on_sec_2();   // Se enciende la sección 2
							state = ON_2; // Se actualiza al estado ON_2
						}
						else
						{
							// En cualquier otro caso, con ningún switch encendido
							state = EVALUATING; // Se actualiza al estado EVALUATING
						}
					break;

					case ON_3:
						// Como pasaron 500 ms
						off_sec_3();   // Se apaga la sección 3
						state = OFF_3; // Se actualiza al estado OFF_3
					break;

					case OFF_3:
						// 500 ms después de apagar la sección se debe cambiar de sección
						if(pulse_1() == 1)
						{
							// Si el switch 1 está encendido
							on_sec_1();   // Se enciende la sección 1
							state = ON_1; // Se actualiza al estado ON_1
						}
						else if(pulse_2() == 1)
						{
							// Si el switch 2 está encendido
							on_sec_2();   // Se enciende la sección 2
							state = ON_2; // Se actualiza al estado ON_2
						}
						else if(pulse_3() == 1)
						{
							// Si el switch 3 está encendido
							on_sec_3();   // Se enciende la sección 3
							state = ON_3; // Se actualiza al estado ON_3
						}
						else
						{
							// En cualquier otro caso, con ningún switch encendido
							state = EVALUATING; // Se actualiza al estado EVALUATING
						}
					break;
				}
			}

    	}
    
        __WFI();
    }
}

//Funci�n que permite controlar el reloj para generar la se�al 
void BOARD_TPM_HANDLER(void)
{
    /* Clear interrupt flag.*/
    TPM_ClearStatusFlags(BOARD_TPM, kTPM_TimeOverflowFlag);
	// Se sube la bandera a ser usada en el código principal cada 100 ms
    tpmIsrFlag = true;
}
