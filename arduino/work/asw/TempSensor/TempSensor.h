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

#define PERIOD_MS_TASK_TEMP_SENSOR 5000 /*!< Period for reading temperature data */

/*!
 * @brief Class for temperature sensor
 * @details This class defines all functions used to read data from temperature sensor and monotor it
 */
class TempSensor
{
public:

	/*!
	 * @brief Class constructor
	 * @details This function initializes all data of the class TempSensor. If needed, it creates a new instance of the DHT22 sensor object. It also adds the periodic task in the scheduler.
	 * @return Nothing
	 */
	TempSensor();

	/*!
	 * @brief Task for reading temperature and humidity values
	 * @details This task reads temperature and humidity data using DHT22 driver. It is called every 5 seconds.
	 * @return Nothing
	 */
	static void readTempSensor_task();

	/*!
	 * @brief Get pointer to data raw_temperature
	 * @details This function returns a pointer to the class member raw_temperature
	 * @return Pointer to raw_temperature
	 */
	uint16_t* getTempPtr();

	/*!
	 * @brief Get pointer to data raw_humidity
	 * @details This function returns a pointer to the class member raw_humidity
	 * @return Pointer to raw_humidity
	 */
	uint16_t* getHumPtr();

	/*!
	 * @brief Get temperature data
	 * @details This function returns the value of the temperature. If the official value is not valid, the function return false.
	 * @param [out] temp Temperature value
	 * @return Validity of temperature
	 */
	 bool getTemp(uint16_t* temp);

	/*!
	 * @brief Get humidity data
	 * @details This function returns the value of the humidity. If the official value is not valid, the function return false.
	 * @param [out] hum Humidity value
	 * @return Validity of humidity
	 */
	bool getHumidity(uint16_t* hum);

	/*!
	 * @brief Set data val_validity
	 * @details This function sets the class member val_validity
	 * @param [in] validity Value of validity
	 * @return Nothing
	 */
	void setValidity(bool validity);

	/*
	 * @brief updates last valid values of temperature and humidity
	 * @details This function updates official values of temperature and humidity to the last read values if they are valid.
	 *          If the read values are not valid for more than 1 minute, official values are set invalid.
	 * @return Nothing
	 */
	void updateLastValidValues();

	/*!
	 * @brief Temperature formatting function - Integer part
	 * @details This function return the integer part of the temperature
	 *
	 * @return Integer value of the temperature
	 */
	inline uint8_t GetTempInteger()
	{
		return (uint8_t)(valid_temp/10);
	}

	/*!
	 * @brief Temperature formatting function - Decimal part
	 * @details This function return the decimal part of the temperature
	 *
	 * @return Decimal value of the temperature
	 */
	inline uint8_t GetTempDecimal()
	{
		return (uint8_t)(valid_temp%10);
	}

	/*!
	 * @brief Humidity formatting function - Integer part
	 * @details This function return the integer part of the humidity
	 *
	 * @return Integer value of the humidity
	 */
	inline uint8_t GetHumInteger()
	{
		return (uint8_t)(valid_hum/10);
	}

	/*!
	 * @brief Humidity formatting function - Decimal part
	 * @details This function return the decimal part of the humidity
	 *
	 * @return Decimal value of the humidity
	 */
	inline uint8_t GetHumDecimal()
	{
		return (uint8_t)(valid_hum%10);
	}

	/*!
	 * @brief Data validity get function
	 * @details This function returns the validity of the sensor data
	 *
	 * @returns True if the sensor values are valid, false otherwise
	 */
	inline bool GetValidity()
	{
		return validity;
	}

	/*!
	 * @brief Task period update
	 * @details This function updates the period of the temperature task.
	 *
	 * @param [in] period New period of the task
	 * @return True if the period has been updated, false otherwise
	 */
	bool updateTaskPeriod(uint16_t period);

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

private:

	uint16_t read_temperature; /*!< Raw value of temperature read from DHT22 (= real temperature *10) */
	uint16_t read_humidity; /*!< Raw value of humidity read from DHT22 (= real humidity *10) */
	bool validity_last_read; /*!< Validity of last read temperature and humidity data */

	bool validity; /*!< validity of official temperature and humidity data */
	uint32_t valid_pit; /*!< pit number of the last time when data were valid */

	uint16_t valid_temp; /*!< Valid value of temperature */
	uint16_t valid_hum; /*!< Valid value of humidity */

	uint16_t task_period; /*!< Task period */
};




#endif /* WORK_ASW_TEMPSENSOR_TEMPSENSOR_H_ */
