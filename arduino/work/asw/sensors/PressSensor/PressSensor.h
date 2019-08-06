/*!
 * @file PressSensor.h
 *
 * @brief Class PressSensor header file
 *
 * @date 6 aout 2019
 * @author nicls67
 */

#ifndef WORK_ASW_SENSORS_PRESSSENSOR_PRESSSENSOR_H_
#define WORK_ASW_SENSORS_PRESSSENSOR_PRESSSENSOR_H_

/*!
 * @brief Class for pressure sensor
 * @details This class defines all functions used to read data from pressure sensor and monitor it.
 * 			It is inherited from class Sensor.
 */
class PressSensor : public Sensor
{
public:
	/*!
	 * @brief Class constructor
	 * @details This function initializes all data of the class PressSensor. If needed, it creates a new instance of the BMP180 sensor object.
	 * 			It also adds the periodic task in the scheduler.
	 * @return Nothing
	 */
	PressSensor();

	/*!
	 * @brief Overloaded class constructor
	 * @details This function initializes all data of the class PressSensor. It sets validity timeout and task period to the given value.
	 * 			If needed, it creates a new instance of the BMP180 sensor object.
	 * 			It also adds the periodic task in the scheduler.
	 * @return Nothing
	 */
	PressSensor(uint16_t val_tmo, uint16_t period);

	/*!
	 * @brief Task for reading pressure values
	 * @details This task reads pressure data using BMP180 driver. It is called periodically.
	 *
	 * @return Nothing
	 */
	static void readPressSensor_task();

	/*!
	 * @brief Task period update
	 * @details This function updates the period of the pressure task.
	 *
	 * @param [in] period New period of the task
	 * @return True if the period has been updated, false otherwise
	 */
	bool updateTaskPeriod(uint16_t period);
};

#endif /* WORK_ASW_SENSORS_PRESSSENSOR_PRESSSENSOR_H_ */
