/*!
 * @file debug.h
 *
 * @brief Header file for debug and logging functions
 *
 * @date 15 mars 2018
 * @author nicls67
 */

#ifndef WORK_LIB_LOG_H_
#define WORK_LIB_LOG_H_

#define PERIOD_MS_TASK_DISPLAY_SENSORS 5000 /*!< Period for displaying temperature and humidity data */


/*!
 * @brief Defines the debug states
 */
typedef enum
{
	INIT, /*!< Init state : display the main menu */
	WAIT_INIT, /*!< Wait for a received character in init state */
	DISPLAY_DATA, /*!< Display sensor data in continuous */

}debug_state_t;

/*!
 * @brief Class used for debugging on usart link
 * @details This class defines functions used for sending debug data on USART link.
 */
class UsartDebug
{
public:

	/*!
	 * @brief Class UsartDebug constructor
	 * @details Initializes the class UsartDebug
	 * @return Nothing
	 */
	UsartDebug();

	/*!
	 * @brief Send a string on USART link
	 * @details This functions sends the requested string on USART link by calling driver's transmission function
	 * @param [in] str Pointer to the string being sent
	 * @return Nothing
	 */
	void sendData(char* str);

	/*!
	 * @brief Send a integer data on USART link
	 * @details This functions sends the requested integer on USART link by calling driver's transmission function.
	 * 			The integer is first converted into a string and then sent
	 * @param [in] data integer data to be sent
	 * @param [in] base numerical base used to convert integer into string (between 2 and 36)
	 * @return Nothing
	 */
	void sendInteger(uint16_t data, uint8_t base);

	/*!
	* @brief Send a boolean data on USART link
	* @details 	This functions sends the requested boolean on USART link by calling driver's transmission function.
	* 			The boolean data is first converted into a string and then sent
	* @param [in] data boolean data to be sent
	* @return Nothing
	*/
	void sendBool(bool data);

	/*!
	 * @brief Displays sensors data on usart link
	 * @details This task sends sensors data (temperature and humidity) on usart link every 5 seconds
	 * @return Nothing
	 */
	static void DisplaySensors_task();

	/*!
	 * @brief Check is debug mode is active or not
	 * @details This function checks if debug mode is active or not.
	 * @return TRUE is debug mode is active, FALSE otherwise
	 */
	bool isDebugModeActive();

	/*!
	 * @brief Activates debug mode
	 * @details This function activates USART debug mode.
	 * @return Nothing
	 */
	void activateDebugMode();

	/*!
	 *  @brief Management of debug mode
	 *  @details This function manages the debug mode according to the following state machine :
	 *  		 	  - init state : display main menu
	 *  		 	  - WAIT_INIT state : handles user choice in main menu and selects next state
	 *  		 	  - DISPLAY_DATA state : display sensor data periodically
	 *
	 *  It is called each time a data is received on USART and debug mode is active
	 *
	 *  @param [in] rcv_char 8 bits character received on USART
	 *  @return Nothing
	 */
	void DebugModeManagement(uint8_t rcv_char);

private:

	debug_state_t debug_state;
	bool debugModeActive_F; /*!< Debug mode activation flag */

};

#include "../../bsw/bsw.h"
#include "../../scheduler/scheduler.h"

#endif /* WORK_LIB_LOG_H_ */
