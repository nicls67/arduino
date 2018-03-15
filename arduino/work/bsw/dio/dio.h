/*!
 * @file dio.h
 * @brief DIO library header file
 *
 * @date 13 mars 2018
 * @author nicls67
 */

#ifndef WORK_BSW_DIO_DIO_H_
#define WORK_BSW_DIO_DIO_H_

#include <avr/io.h>

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

private:

	/*!
	 * @brief DIO hardware initialization function
	 * @details This function initializes DIO ports as input or output
	 * @return Nothing
	 */
	void dio_init();
};


#endif /* WORK_BSW_DIO_DIO_H_ */
