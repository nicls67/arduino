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

class TempSensor
{
public:

	/*!
	 * @brief Class constructor
	 * @details This function initializes all data of the class TempSensor
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

private:

	uint16_t read_temperature; /*!< Raw value of temperature read from DHT22 (= real temperature *10) */
	uint16_t read_humidity; /*!< Raw value of humidity read from DHT22 (= real humidity *10) */
	bool validity_last_read; /*!< Validity of last read temperature and humidity data */

	bool validity; /*!< validity of official temperature and humidity data */
	uint32_t valid_pit; /*!< pit number of the last time when data were valid */

	uint16_t valid_temp; /*!< Valid value of temperature */
	uint16_t valid_hum; /*!< Valid value of humidity */
};

#include "../../scheduler/scheduler.h"
#include "../../bsw/bsw.h"
#include "../asw.h"

#endif /* WORK_ASW_TEMPSENSOR_TEMPSENSOR_H_ */
