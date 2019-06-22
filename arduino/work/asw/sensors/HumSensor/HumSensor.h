/*!
 * @file HumSensor.h
 *
 * @brief Class HumSensor header file
 *
 * @date 20 juin 2019
 * @author nicls67
 */

#ifndef WORK_ASW_SENSORS_HUMSENSOR_HUMSENSOR_H_
#define WORK_ASW_SENSORS_HUMSENSOR_HUMSENSOR_H_

/*!
 * @brief Class for humidity sensor
 * @details This class defines all functions used to read data from humidity sensor and monitor it.
 * 			It is inherited from class Sensor.
 */
class HumSensor : public Sensor
{
public:

	/*!
	 * @brief Class constructor
	 * @details This function initializes all data of the class HumSensor. If needed, it creates a new instance of the DHT22 sensor object. It also adds the periodic task in the scheduler.
	 * @return Nothing
	 */
	HumSensor();

	/*!
	 * @brief Overloaded class constructor
	 * @details This function initializes all data of the class HumSensor. It sets validity timeout and task period to the given value.
	 * 			If needed, it creates a new instance of the DHT22 sensor object. It also adds the periodic task in the scheduler.
	 * @return Nothing
	 */
	HumSensor(uint16_t val_tmo, uint16_t period);

	/*!
	 * @brief Task for reading humidity values
	 * @details This task reads humidity data using DHT22 driver. It is called periodically.
	 * @return Nothing
	 */
	static void readHumSensor_task();

	/*!
	 * @brief Task period update
	 * @details This function updates the period of the temperature task.
	 *
	 * @param [in] period New period of the task
	 * @return True if the period has been updated, false otherwise
	 */
	bool updateTaskPeriod(uint16_t period);


private:

};

extern HumSensor* p_global_ASW_HumSensor; /*!< Pointer to TempSensor object */

#endif /* WORK_ASW_SENSORS_HUMSENSOR_HUMSENSOR_H_ */
