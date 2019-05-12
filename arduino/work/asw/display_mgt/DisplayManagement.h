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

#define DISPLAY_MGT_LCD_I2C_ADDR 0x27 /*!< I2C address of the screen */

#define DISPLAY_MGT_PERIOD_TASK_SENSOR 5000 /*!< Display is updated every 5s */
#define DISPLAY_MGT_PERIOD_WELCOME_MSG_REMOVAL 5000 /*!< Time after which one the welcome message is removed */

#define DISPLAY_MGT_LINE_TEMP 0 /*!< Current temperature is displayed on line 0 */
#define DISPLAY_MGT_LINE_HUM 1 /*!< Current humidity is displayed on line 1 */

#define DISPLAY_MGT_I2C_BITRATE (uint32_t)100000 /*!< I2C bus bitrate is 100 kHz */

/*!
 * @brief LCD configuration structure
 * @details This structure defines the initial configuration of the LCD screen.
 */
const T_LCD_conf_struct LCD_init_cnf = {
	DISPLAY_MGT_I2C_BITRATE,
	DISPLAY_MGT_LCD_I2C_ADDR,
	LCD_CNF_BACKLIGHT_ON,
	LCD_CNF_TWO_LINE,
	LCD_CNF_FONT_5_8,
	LCD_CNF_DISPLAY_ON,
	LCD_CNF_CURSOR_OFF,
	LCD_CNF_CURSOR_BLINK_OFF,
	LCD_CNF_ENTRY_MODE_DIRECTION_RIGHT,
	LCD_CNF_ENTRY_MODE_DISPLAY_SHIFT_OFF
};


const uint8_t welcomeMessageString[] = "Bienvenue !";
const uint8_t tempDisplayString[] = "Temperature : "; /*!< String used for temperature display */
const uint8_t humidityDisplayString[] = "Humidite : "; /*!< String used for humidity display */

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

	/*!
	 * @brief Sensor pointer get function
	 * @details This function returns the pointer to the temperature sensor object
	 *
	 * @return Pointer to sensor object
	 */
	inline TempSensor* GetTempSensorPtr()
	{
		return p_tempSensor;
	}

	/*!
	 * @brief End of welcome message task
	 * @details This task clears the welcome message from the screen and start periodic display of sensor data.
	 * 			This task shall be added in scheduler when the welcome message is displayed on screen.
	 * 			As it shall be called only once, the task removes itself from the scheduler after the first call.
	 *
	 * 	@return Nothing
	 */
	static void RemoveWelcomeMessage_Task();

private:

	DisplayInterface * p_display_ift; /*!< Pointer to the display interface object */
	TempSensor* p_tempSensor; /*!< Pointer to the temperature sensor object */

};



#endif /* WORK_ASW_DISPLAY_MGT_DISPLAYMANAGEMENT_H_ */
