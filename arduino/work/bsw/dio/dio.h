/*!
 * @file dio.h
 * @brief DIO library header file
 *
 * @date 13 mars 2018
 * @author nicls67
 */

#ifndef WORK_BSW_DIO_DIO_H_
#define WORK_BSW_DIO_DIO_H_



#define PORT_CNF_OUT 1 /*!< Pin is configured as output */
#define PORT_CNF_IN  0 /*!< Pin is configured as input */

#define ENCODE_PORT(port, pin)  (uint8_t)((((uint8_t)(port & 0xF)) << 3) | (uint8_t)(pin & 0x7)) /*!< Macro used to encode port and pin indexes into one single byte */
#define DECODE_PORT(portcode)   (uint8_t)((portcode >> 3) & 0xF) /*!< Macro used to extract port index */
#define DECODE_PIN(portcode)    (uint8_t)(portcode & 0x7) /*!< Macro used to extract pin index */

#define PORT_A 0  /*!< PORTA index */
#define PORT_B 1  /*!< PORTB index */
#define PORT_C 2  /*!< PORTC index */
#define PORT_D 3  /*!< PORTD index */


/*!
 * @brief DIO class
 * @details This class defines all useful functions for digital input/output ports
 */
class dio
{
public:

	/*!
	 * @brief dio class constructor
	 * @details Initializes class dio and calls DIO hardware initialization function
	 *
	 * @return Nothing
	 */
	dio();

	/*!
	 * @brief Port setting function
	 * @details This function sets the requested digital output to the requested state.
	 * 			The corresponding port and pin index is extracted from parameter portcode.
	 *
	 * @param [in] portcode Encoded pin and register index
	 * @param [in] state Requested state to set pin
	 * @return Nothing
	 */
	void dio_setPort(uint8_t portcode, bool state);

	/*!
	 * @brief Inverts the state of output port
	 * @details This function inverts the state of the chosen pin.
	 *  		The corresponding port and pin index is extracted from parameter portcode.
	 *
	 * @param [in] portcode Encoded pin and register index
	 * @return Nothing
	 *
	 */
	void dio_invertPort(uint8_t portcode);

	/*!
	 *  @brief Gets the logical state of selected pin
	 *  @details This function gets the logical value of the selected pin.
	 *   		 The corresponding port and pin index is extracted from parameter portcode.
	 *
	 *  @param [in] portcode Encoded pin and register index
	 *  @return Logical state of selected pin
	 */
	bool dio_getPort(uint8_t portcode);

	/*!
	 *  @brief Gets the logical state of the memorized pin
	 *  @details This function gets the logical value of the memorized pin.
	 *   		 The corresponding port and pin index are stored in class members PINx_addr_mem and PINx_idx_mem.
	 *   		 This mechanism is used to speed up reading time as this function no longer needs to extract register address and pin index from portcode.
	 *
	 *  @return Logical state of selected pin
	 */
	bool dio_getPort_fast(void);

	/*!
	 *  @brief Changes the IO configuration of the selected pin
	 *  @details This function configures the selected pin as input or output according to parameter cnf.
	 *   		 The corresponding port and pin index is extracted from parameter portcode.
	 *
	 *  @param [in] portcode Encoded pin and register index
	 *  @param [in] cnf Requested configuration for the selected pin\n
	 *   			PORT_CNF_OUT (1) : pin configured as output\n
	 *   			PORT_CNF_IN  (0) : pin configured as input
	 *
	 *   @return Nothing
	 */
	void dio_changePortPinCnf(uint8_t portcode, uint8_t cnf);

	/*!
	 * @brief Memorizes PINx register address and pin index
	 * @details This function is used to speed up reading of register PINx.
	 * 			Register address and pin index are decoded from portcode parameter and stored for later use by function dio_getPort_fast.
	 *
	 * @param [in] portcode Encoded pin and register index
	 * @return Nothing
	 */
	void dio_memorizePINaddress(uint8_t portcode);


private:

	/*!
	 * @brief Digital ports hardware initialization function
	 * @details This function initializes digital ports as input or output and sets their initial values
	 * @return Nothing
	 */
	void ports_init();

	/*!
	 * @brief Gets the physical address of the requested register PORTx
	 * @details This function retrieves the address of the register PORTx where x is encoded into the parameter portcode.
	 *
	 * @param [in] portcode Encoded port code
	 * @returns Pointer to the PORTx register
	 */
	uint8_t* getPORTxAddress(uint8_t portcode);

	/*!
	 * @brief Gets the physical address of the requested register PINx
	 * @details This function retrieves the address of the register PINx where x is encoded into the parameter portcode.
	 *
	 * @param [in] portcode Encoded port code
	 * @returns Pointer to the PINx register
	 */
	uint8_t* getPINxAddress(uint8_t portcode);

	/*!
	 * @brief Gets the physical address of the requested register DDRx
	 * @details This function retrieves the address of the register DDRx where x is encoded into the parameter portcode.
	 *
	 * @param [in] portcode Encoded port code
	 * @returns Pointer to the DDRx register
	 */
	uint8_t* getDDRxAddress(uint8_t portcode);

	uint8_t* PINx_addr_mem; /*!< Memorizes physical address of register PINx in order to speed up register reading time in function dio_getPort_fast */
	uint8_t  PINx_idx_mem; /*!< Memorizes pin index of register PINx in order to speed up register reading time in function dio_getPort_fast */
};

#include "../bsw.h"
#include "dio_port_cnf.h"
#include "dio_reg_atm2560.h"

#endif /* WORK_BSW_DIO_DIO_H_ */
