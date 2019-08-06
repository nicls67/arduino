/*!
 * @file SensorManagement.cpp
 *
 * @brief SensorManagement class source code file
 *
 * @date 22 juin 2019
 * @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>

#include "../../lib/LinkedList/LinkedList.h"
#include "../../lib/String/String.h"

#include "../sensors/Sensor.h"
#include "../sensors/TempSensor/TempSensor.h"
#include "../sensors/HumSensor/HumSensor.h"
#include "../sensors/PressSensor/PressSensor.h"

#include "SensorManagement.h"
#include "sensor_configuration.h"

SensorManagement* p_global_ASW_SensorManagement;


SensorManagement::SensorManagement()
{
	/* Initialize class members */
	nb_sensors = sizeof(SensorManagement_Sensor_Config_list)/sizeof(T_SensorManagement_Sensor_Config);
	sensor_ptr_table = (void**)malloc(nb_sensors * sizeof(void*));

	/* Create sensor objects */
	for(uint8_t i=0; i<nb_sensors; i++)
	{
		switch(SensorManagement_Sensor_Config_list[i].sensor_type)
		{
		case TEMPERATURE :
			{
			TempSensor* temp_ptr = new TempSensor(SensorManagement_Sensor_Config_list[i].validity_tmo, SensorManagement_Sensor_Config_list[i].period);
			sensor_ptr_table[i] = temp_ptr;
			}
			break;
		case HUMIDITY :
			{
			HumSensor* hum_ptr = new HumSensor(SensorManagement_Sensor_Config_list[i].validity_tmo, SensorManagement_Sensor_Config_list[i].period);
			sensor_ptr_table[i] = hum_ptr;
			}
			break;
		case PRESSURE :
			{
			PressSensor* press_ptr = new PressSensor(SensorManagement_Sensor_Config_list[i].validity_tmo, SensorManagement_Sensor_Config_list[i].period);
			sensor_ptr_table[i] = press_ptr;
			}
			break;
		}
	}
}

bool SensorManagement::updateTaskPeriod(uint16_t period)
{
	bool retval = true;
	bool res;
	Sensor sensor_ptr;

	for(uint8_t i=0; i<nb_sensors; i++)
	{
		res = ((Sensor*)(sensor_ptr_table[i]))->updateTaskPeriod(period);

		if(!res)
			retval = false;
	}

	return retval;
}

void SensorManagement::getFullStringFormattedValue(uint8_t sensor_idx, String* str)
{
	str->Clear();
	str->appendString(SensorManagement_Sensor_Config_list[sensor_idx].data_name_str);
	str->appendSpace();
	str->appendChar((uint8_t)':');
	str->appendSpace();

	if(((Sensor*)(sensor_ptr_table[sensor_idx]))->getValidity())
	{
		str->appendInteger(((Sensor*)(sensor_ptr_table[sensor_idx]))->getValueInteger(), 10);
		str->appendChar((uint8_t)'.');
		str->appendInteger(((Sensor*)(sensor_ptr_table[sensor_idx]))->getValueDecimal(), 10);
		str->appendSpace();
		str->appendString(SensorManagement_Sensor_Config_list[sensor_idx].unit_str);
	}
	else
		str->appendString((uint8_t*)"invalide");
}

void* SensorManagement::getSensorObjectPtr(T_SensorManagement_Sensor_Type type)
{
	void* retval = 0;
	uint8_t idx = 0;

	/* Find the index of the corresponding sensor */
	while((idx < nb_sensors) && (SensorManagement_Sensor_Config_list[idx].sensor_type != type))
		idx++;

	/* If the sensor has been found */
	if(idx < nb_sensors)
	{
		/* Get the object pointer */
		retval = sensor_ptr_table[idx];
	}

	return retval;
}
