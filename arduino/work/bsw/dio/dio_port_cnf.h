/*!
 * @file dio_port_cnf.h
 *
 * @brief Digital ports configuration file
 *
 * @date 19 mars 2019
 * @author nicls67
 */

#ifndef WORK_BSW_DIO_DIO_PORT_CNF_H_
#define WORK_BSW_DIO_DIO_PORT_CNF_H_

/*!
 * @brief Defines the configuration of DDRB register
 * @details This constant defines the direction of IO pins of PORT B. It will configure register DDRB.\n
 *          PB0 : N/A\n
 *          PB1 : N/A\n
 *          PB2 : N/A\n
 *          PB3 : N/A\n
 *          PB4 : IN\n
 *          PB5 : N/A\n
 *          PB6 : OUT\n
 *          PB7 : OUT\n
 */
#define PORTB_CNF_DDRB (uint8_t)0b11000000

/*!
 * @brief Defines the configuration of PORTB register
 * @details This constant defines the initial state of IO pins for PORT B. It will configure register PORTB.
 * 			For outputs pins, it defines the initial level (high or low). For input pins, it defines if the pins is configured as high-Z or pull-up.\n
 *          PB0 : N/A\n
 *          PB1 : N/A\n
 *          PB2 : N/A\n
 *          PB3 : N/A\n
 *          PB4 : Pull-up\n
 *          PB5 : N/A\n
 *          PB6 : HIGH\n
 *          PB7 : LOW\n
 */
#define PORTB_CNF_PORTB (uint8_t)0b01010000

#define PORT_A 0  /*!< PORTA index */
#define PORT_B 1  /*!< PORTB index */
#define PORT_C 2  /*!< PORTC index */
#define PORT_D 3  /*!< PORTD index */

#endif /* WORK_BSW_DIO_DIO_PORT_CNF_H_ */
