/*!
 * @file Sensor.h
 *
 * @brief Sensor class header file
 *
 * @date 20 juin 2019
 * @author nicls67
 */

#ifndef WORK_ASW_SENSORS_SENSOR_H_
#define WORK_ASW_SENSORS_SENSOR_H_

/*!
 * @brief Generic class for sensor device
 * @details This class defines a generic sensor, as handled by class SensorManagement. It should not be instantiated.
 * 			Only inherited classes shall be instantiated.
 */
class Sensor {

public:

	/*!
	 * @brief Sensor class constructor
	 * @details This function initializes the class.
	 *
	 * @return Nothing
	 */
	Sensor();

	/*!
	 * @brief Overloaded sensor class constructor.
	 * @details This function initializes the class. It sets validity timeout and task period to the given value.
	 *
	 * @param [in] val_tmo Validity timeout
	 * @param [in] period Task period
	 * @return Nothing
	 */
	Sensor(uint16_t val_tmo, uint16_t period);

	/*!
	 * @brief Task for reading sensor values
	 * @details This task reads sensor data using sensor driver. It is called periodically.
	 * 			This function shall be re-written in each inherited class.
	 * @return Nothing
	 */
	static void readSensor_task(){}

	/*!
	 * @brief Get pointer to raw sensor data
	 * @details This function returns a pointer to the class member raw_data
	 * @return Pointer to raw_data
	 */
	inline uint16_t* getRawDataPtr()
	{
		return &raw_data;
	}

	/*!
	 * @brief Get sensor value function
	 * @details This function returns the value of sensor data. If the official value is not valid, the function return false.
	 * @param [out] value Sensor value
	 * @return Validity
	 */
	 inline bool getValue(uint16_t* value)
	 {
		 *value = valid_value;
		 return validity;
	 }

	/*!
	 * @brief Validity setting function
	 * @details This function sets the class member validity_last_read
	 * @param [in] validity Value of validity
	 * @return Nothing
	 */
	inline void setLastValidity(bool validity)
	{
		validity_last_read = validity;
	}

	/*
	 * @brief Updates last valid values of sensor data
	 * @details This function updates official values of sensor data to the last read values if they are valid.
	 *          If the read values are not valid for more than 1 minute, official values are set invalid.
	 * @return Nothing
	 */
	void updateValidData();

	/*!
	 * @brief Data formatting function - Integer part
	 * @details This function return the integer part of the sensor value
	 *
	 * @return Integer value of the sensor data
	 */
	inline uint16_t getValueInteger()
	{
		return (uint16_t)(valid_value/10);
	}

	/*!
	 * @brief Data formatting function - Decimal part
	 * @details This function return the decimal part of the sensor value
	 *
	 * @return Decimal value of the sensor data
	 */
	inline uint8_t getValueDecimal()
	{
		return (uint8_t)(valid_value%10);
	}

	/*!
	 * @brief Data validity get function
	 * @details This function returns the validity of the sensor data
	 *
	 * @returns True if the sensor values are valid, false otherwise
	 */
	inline bool getValidity()
	{
		return validity;
	}

	/*!
	 * @brief Task period update
	 * @details This function updates the period of the sensor task. It shall be re-written in each inherited class.
	 *
	 * @param [in] period New period of the task
	 * @return True if the period has been updated, false otherwise
	 */
	bool updateTaskPeriod(uint16_t period) {return false;}

	/*!
	 * @brief Task period get function
	 * @details This function returns the period of the sensor task
	 *
	 * @return Period of the task (ms)
	 */
	inline uint16_t getTaskPeriod()
	{
		return task_period;
	}

	/*!
	 * @brief Validity timeout setting function
	 * @details This function sets the validity timeout.
	 *
	 * @param [in] timeout New value of timeout.
	 * @return Nothing
	 */
	inline void setValidityTMO(uint16_t timeout)
	{
		validity_tmo = timeout;
	}

protected:
	bool validity; /*!< Validity of sensor data */
	bool validity_last_read; /*!< Validity of last read sensor data */

	uint32_t valid_pit; /*!< pit number of the last time when data were valid */
	uint16_t validity_tmo; /*!< Number of PITs after which the sensor value is declared invalid */

	uint16_t raw_data; /*!< Raw value of sensor data (directly coming from driver */

	uint16_t valid_value; /*!< Valid value of sensor data */

	uint16_t task_period; /*!< Task period */

};

#endif /* WORK_ASW_SENSORS_SENSOR_H_ */
