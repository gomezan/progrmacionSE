/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define SOPT5_UART0RXSRC_UART_RX 0x00u /*!<@brief UART0 Receive Data Source Select: UART_RX pin */
#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART0 Transmit Data Source Select: UART0_TX pin */

/*! @name PORTA1 (number 35), J1[2]/D0/UART1_RX_TGTMCU
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_DEBUG_UART_RX_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_DEBUG_UART_RX_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                              /* @} */

/*! @name PORTA2 (number 36), J1[4]/D1/UART1_TX_TGTMCU
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_DEBUG_UART_TX_PORT PORTA               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_DEBUG_UART_TX_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                              /* @} */

/*! @name PORTD2 (number 95), J2[4]/D9
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_PIN_D2_FGPIO FGPIOD             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_D2_GPIO GPIOD               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_D2_GPIO_PIN_MASK (1U << 2U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_PIN_D2_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_PIN_D2_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_PIN_D2_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                       /* @} */

/*! @name PORTD4 (number 97), J2[6]/D10
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_PIN_D4_FGPIO FGPIOD             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_D4_GPIO GPIOD               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_D4_GPIO_PIN_MASK (1U << 4U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_PIN_D4_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_PIN_D4_PIN 4U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_PIN_D4_PIN_MASK (1U << 4U)      /*!<@brief PORT pin mask */
                                                       /* @} */

/*! @name PORTD6 (number 99), J2[8]/D11
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_PIN_D6_FGPIO FGPIOD             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_D6_GPIO GPIOD               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_D6_GPIO_PIN_MASK (1U << 6U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_PIN_D6_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_PIN_D6_PIN 6U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_PIN_D6_PIN_MASK (1U << 6U)      /*!<@brief PORT pin mask */
                                                       /* @} */

/*! @name PORTB0 (number 53), J4[2]/A0
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_PIN_B0_FGPIO FGPIOB             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_B0_GPIO GPIOB               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_B0_GPIO_PIN_MASK (1U << 0U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_PIN_B0_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_PIN_B0_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_PIN_B0_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                       /* @} */

/*! @name PORTB1 (number 54), J4[4]/A1
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_PIN_B1_FGPIO FGPIOB             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_B1_GPIO GPIOB               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_B1_GPIO_PIN_MASK (1U << 1U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_PIN_B1_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_PIN_B1_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_PIN_B1_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                       /* @} */

/*! @name PORTB2 (number 55), J4[6]/A2
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_PIN_B2_FGPIO FGPIOB             /*!<@brief FGPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_B2_GPIO GPIOB               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_PIN_B2_GPIO_PIN_MASK (1U << 2U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_PIN_B2_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_PIN_B2_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_PIN_B2_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                       /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
