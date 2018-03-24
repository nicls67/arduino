/*!
 * @file dio.h
 * @brief DIO library header file
 *
 * @date 13 mars 2018
 * @author nicls67
 */

#ifndef WORK_BSW_DIO_DIO_H_
#define WORK_BSW_DIO_DIO_H_



#define PORT_CNF_OUT 1
#define PORT_CNF_IN  0


/*!
 * @brief DIO class
 * @details This class defines all useful functions for digital input/output ports
 */
class dio
{
public:

	/*!
	 * @brief dio class constructor
	 * @details Initializes class dio and calls DIO hardware intialization function
	 *
	 * @return Nothing
	 */
	dio();

	/*!
	 * @brief Port B setting function
	 * @details This function sets the requested digital output on port B to the requested state
	 *
	 * @param [in] pin pin of PORT B to set
	 * @param [in] state requested state to set pin
	 * @return Nothing
	 */
	void dio_setPortB(uint8_t pin, bool state);

	/*!
	 * @brief Inverts the state of output port
	 * @details This function inverts the state of the chosen pin of port B
	 *
	 * @param [in] pin Pin to invert
	 * @return Nothing
	 *
	 */
	void dio_invertPortB(uint8_t pin);

	/*!
	 *  @brief Gets the logical state of selected pin
	 *  @details This function gets the logical value of the selected pin of port B
	 *
	 *  @param [in] pin pin of port B to get value
	 *  @return Logical state of selected pin
	 */
	bool dio_getPortB(uint8_t pin);

	/*!
	 *  @brief Changes the IO configuration of the selected pin of port B
	 *  @details This function configures the selected pin of port B as input or output according to parameter cnf
	 *
	 *  @param [in] pin pin selected for configuration change
	 *  @param [in] cnf Requested configuration for the selected pin\n
	 *   			PORT_CNF_OUT (1) : pin configured as output\n
	 *   			PORT_CNF_IN  (0) : pin configured as input
	 *
	 *   @return Nothing
	 */
	void dio_changePortBPinCnf(uint8_t pin, uint8_t cnf);


private:

	/*!
	 * @brief DIO hardware initialization function
	 * @details This function initializes DIO ports as input or output
	 * @return Nothing
	 */
	void dio_init();
};


#endif /* WORK_BSW_DIO_DIO_H_ */
