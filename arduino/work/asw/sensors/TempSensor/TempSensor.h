/*!
 * @file TempSensor.h
 *
 * @brief Class TempSensor header file
 *
 * @date 23 mars 2018
 * @author nicls67
 */

#ifndef WORK_ASW_TEMPSENSOR_TEMPSENSOR_H_
#define WORK_ASW_TEMPSENSOR_TEMPSENSOR_H_



/*!
 * @brief Class for temperature sensor
 * @details This class defines all functions used to read data from temperature sensor and monitor it.
 * 			It is inherited from class Sensor.
 */
class TempSensor : public Sensor
{
public:

	/*!
	 * @brief Class constructor
	 * @details This function initializes all data of the class TempSensor. If needed, it creates a new instance of the DHT22 and BMP180 sensors objects.
	 * 			It also adds the periodic task in the scheduler.
	 * @return Nothing
	 */
	TempSensor();

	/*!
	 * @brief Overloaded class constructor
	 * @details This function initializes all data of the class TempSensor. It sets validity timeout and task period to the given value.
	 * 			If needed, it creates a new instance of the DHT22 and BMP180 sensor objects.
	 * 			It also adds the periodic task in the scheduler.
	 * @return Nothing
	 */
	TempSensor(uint16_t val_tmo, uint16_t period);

	/*!
	 * @brief Task for reading temperature values
	 * @details This task reads temperature data using DHT22 and BMP180 drivers. It is called periodically.
	 * 			The returned temperature is the mean between both sensors values, if only one sensor is valid, only this value is used .
	 * @return Nothing
	 */
	static void readTempSensor_task();

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


#endif /* WORK_ASW_TEMPSENSOR_TEMPSENSOR_H_ */
