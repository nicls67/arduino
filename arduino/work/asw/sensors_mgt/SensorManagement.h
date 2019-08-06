/*!
 * @file SensorManagement.h
 *
 * @brief SensorManagement class header file
 *
 * @date 22 juin 2019
 * @author nicls67
 */

#ifndef WORK_ASW_SENSORS_MGT_SENSORMANAGEMENT_H_
#define WORK_ASW_SENSORS_MGT_SENSORMANAGEMENT_H_

/*!
 * @brief Sensor type enumeration
 * @details This enumeration defines all types of sensors available.
 */
typedef enum
{
	TEMPERATURE,
	HUMIDITY,
	PRESSURE
}
T_SensorManagement_Sensor_Type;

/*!
 * @brief Sensor management class
 * @details This class manages all sensors present in the SW. It manages sensor activation and deactivation,
 * 			has a periodic task to retrieve sensor data. It also creates the string with sensors values used by display services.
 */
class SensorManagement {

public:
	/*!
	 * @brief Class constructor
	 * @details This function initializes the class. It allocates the sensor pointer table according to the number of sensors present.
	 * 			For each sensor, the related object is created.
	 *
	 * @return Nothing
	 */
	SensorManagement();

	/*!
	 * @brief Sensor count get function
	 * @details This function returns the number of sensors present in the SW
	 *
	 * @return Number of sensors.
	 */
	inline uint8_t getSensorCount()
	{
		return nb_sensors;
	}

	/*!
	 * @brief Sensors tasks period update
	 * @details This function updates the period of of all sensors tasks. The function updateTaskPeriod is called for each sensor object.
	 *
	 * @param [in] period New period.
	 * @return True if the period has been updated, false otherwise.
	 */
	bool updateTaskPeriod(uint16_t period);

	/*!
	 * @brief Sensor value formatting function.
	 * @details This function gets the value of the selected sensor and formats it into a string using the data name string defined in the configuration.
	 *
	 * @param [in] sensor_idx Index of the requested sensor
	 * @param [out] str Pointer to the formatted string
	 * @return Nothing
	 */
	void getFullStringFormattedValue(uint8_t sensor_idx, String* str);

	/*!
	 * @brief Sensor object pointer get function
	 * @details This function finds the pointer to the sensor object of the given type in sensor_ptr_table and returns this pointer.
	 *
	 * @param [in] type Type of sensor to find
	 * @return Pointer to the sensor object (casted to pointer-to-void)
	 */
	void* getSensorObjectPtr(T_SensorManagement_Sensor_Type type);

private:

	uint8_t nb_sensors; /*!< Number of sensors */
	void** sensor_ptr_table; /*!< Table containing pointers to all sensors objects (declared as pointer to void to avoid including Sensor.h in all files) */
};

extern SensorManagement* p_global_ASW_SensorManagement; /*!< Pointer to the SensorManagement object */

#endif /* WORK_ASW_SENSORS_MGT_SENSORMANAGEMENT_H_ */
