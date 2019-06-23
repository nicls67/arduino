/*!
 * @file sensor_configuration.h
 *
 * @brief Sensors configuration header file
 *
 * @date 22 juin 2019
 * @author nicls67
 */

#ifndef WORK_ASW_SENSORS_MGT_SENSOR_CONFIGURATION_H_
#define WORK_ASW_SENSORS_MGT_SENSOR_CONFIGURATION_H_

/*!
 * @brief Sensor informations structure
 * @details This structure contains all configuration informations needed for each used sensor.
 */
typedef struct
{
	T_SensorManagement_Sensor_Type sensor_type; /* Type of the sensor */
	uint16_t period; /* Period of the sensor periodic task */
	uint16_t validity_tmo; /* Validity timeout */
	uint8_t* data_name_str; /* Pointer to the string containing the data name */
	uint8_t* unit_str; /* Pointer to the string containing the unit of the sensor data */
}
T_SensorManagement_Sensor_Config;

extern T_SensorManagement_Sensor_Config SensorManagement_Sensor_Config_list[2];


#endif /* WORK_ASW_SENSORS_MGT_SENSOR_CONFIGURATION_H_ */
