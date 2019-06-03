/*!
 * @file dio_reg_atm2560.h
 *
 * @brief Defines DIO register addresses for ATMEGA2560
 *
 * @date 19 mars 2019
 * @author nicls67
 */

#ifndef WORK_BSW_DIO_DIO_REG_ATM2560_H_
#define WORK_BSW_DIO_DIO_REG_ATM2560_H_


#define PORTA_PTR (volatile uint8_t *)(0x02 + 0x20) /*!< Macro defining pointer to PORT A register */
#define PORTB_PTR (volatile uint8_t *)(0x05 + 0x20) /*!< Macro defining pointer to PORT B register */
#define PORTC_PTR (volatile uint8_t *)(0x08 + 0x20) /*!< Macro defining pointer to PORT C register */
#define PORTD_PTR (volatile uint8_t *)(0x0B + 0x20) /*!< Macro defining pointer to PORT D register */

#define PINA_PTR (volatile uint8_t *)(0x00 + 0x20) /*!< Macro defining pointer to PIN A register */
#define PINB_PTR (volatile uint8_t *)(0x03 + 0x20) /*!< Macro defining pointer to PIN B register */
#define PINC_PTR (volatile uint8_t *)(0x06 + 0x20) /*!< Macro defining pointer to PIN C register */
#define PIND_PTR (volatile uint8_t *)(0x09 + 0x20) /*!< Macro defining pointer to PIN D register */

#define DDRA_PTR (volatile uint8_t *)(0x01 + 0x20) /*!< Macro defining pointer to DDR A register */
#define DDRB_PTR (volatile uint8_t *)(0x04 + 0x20) /*!< Macro defining pointer to DDR B register */
#define DDRC_PTR (volatile uint8_t *)(0x07 + 0x20) /*!< Macro defining pointer to DDR C register */
#define DDRD_PTR (volatile uint8_t *)(0x0A + 0x20) /*!< Macro defining pointer to DDR D register */



#endif /* WORK_BSW_DIO_DIO_REG_ATM2560_H_ */
