/*!
 * @file DisplayManagement.h
 *
 * @brief Display management class header file
 *
 * @date 1 mai 2019
 * @author nicls67
 */

#ifndef WORK_ASW_DISPLAY_MGT_DISPLAYMANAGEMENT_H_
#define WORK_ASW_DISPLAY_MGT_DISPLAYMANAGEMENT_H_

/* TODO : LCD configuration shall be done here, not in display interface */

#define DISPLAY_MGT_PERIOD_TASK_SENSOR 5000 /*!< Display is updated every 5s */

const uint8_t* tempDisplayString[] = "Temperature : xx.x";
/*!
 * @brief Display management class
 * @details This class manages all displays. It is a top-level class.
 * 			It retrieves the data computed by other ASW classes and displays them.
 * 			It is interfaced with DisplayInterface class to display data on screens.
 * 			One interface class is used for each screen.
 */
class DisplayManagement {

public:

	/*!
	 * @brief Class constructor
	 * @details This class initializes display management. \n
	 * 			It created a display interface object and initializes all class variables.
	 *
	 * 	@return Nothing
	 */
	DisplayManagement();

	/*!
	 * @brief Periodic task for displaying sensor data
	 * @details This function displays the sensors data on the screen.
	 * 			Currently temperature and humidity data coming from dht22 sensor are displayed. \n
	 * 			It is called periodically by scheduler.
	 *
	 * 	@return Nothing
	 */
	static void DisplaySensorData_Task();

	/*!
	 * @brief Interface pointer get function
	 * @details This function returns the pointer to the display interface object
	 *
	 * @return Pointer to display interface object
	 */
	inline DisplayInterface* GetIftPointer()
	{
		return p_display_ift;
	}

private:

	DisplayInterface * p_display_ift; /*!< Pointer to the display interface object */

};



#endif /* WORK_ASW_DISPLAY_MGT_DISPLAYMANAGEMENT_H_ */
