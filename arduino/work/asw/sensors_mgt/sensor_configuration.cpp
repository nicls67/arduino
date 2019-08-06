/*!
 * @file sensor_configuration.cpp
 *
 * @brief Sensor configuration file
 *
 * @date 22 juin 2019
 * @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>

#include "../../lib/LinkedList/LinkedList.h"
#include "../../lib/String/String.h"

#include "SensorManagement.h"
#include "sensor_configuration.h"

#define SENSOR_MGT_CNF_DEFAULT_PERIOD 1000 /*!< Default period for sensors task */
#define SENSOR_MGT_CNF_DEFAULT_TMO 15000 /*!< Default timeout value for sensors */

/*!
 * @brief Sensor configuration table
 */
T_SensorManagement_Sensor_Config SensorManagement_Sensor_Config_list[3] =
{
		{
				TEMPERATURE,
				SENSOR_MGT_CNF_DEFAULT_PERIOD,
				SENSOR_MGT_CNF_DEFAULT_TMO,
				(uint8_t*)"T",
				(uint8_t*)"degC"
		},
		{
				HUMIDITY,
				SENSOR_MGT_CNF_DEFAULT_PERIOD,
				SENSOR_MGT_CNF_DEFAULT_TMO,
				(uint8_t*)"H",
				(uint8_t*)"%"
		},
		{
				PRESSURE,
				SENSOR_MGT_CNF_DEFAULT_PERIOD,
				SENSOR_MGT_CNF_DEFAULT_TMO,
				(uint8_t*)"P",
				(uint8_t*)"hPa"
		}
};
