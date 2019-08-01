/*!
 * @file Bmp180.cpp
 *
 * @brief Bmp180 class source file
 *
 * @date 27 juil. 2019
 * @author nicls67
 */

#include <avr/io.h>

#include "../../lib/LinkedList/LinkedList.h"
#include "../../scheduler/scheduler.h"

#include "../I2C/I2C.h"
#include "../timer/timer.h"
#include "Bmp180.h"



Bmp180* p_global_BSW_bmp180;

Bmp180::Bmp180()
{
	/* Create new instance of I2C driver if needed */
	if(p_global_BSW_i2c == 0)
		p_global_BSW_i2c = new I2C(BMP180_I2C_BITRATE);

	/* Create timer object if it is still not initialized */
	if(p_global_BSW_timer == 0)
		p_global_BSW_timer = new timer();

	i2c_drv_ptr = p_global_BSW_i2c;

	/* Set status to OK */
	status = OK;

	/* Read chip ID */
	chip_id = 0;
	readChipID();

	/* Read calibration data */
	uint8_t* ptr = (uint8_t*)(&calibration_data);
	for(uint8_t i=0; i<sizeof(T_BMP180_calib_data); i++)
		ptr[i] = 0;
	readCalibData();

	/* Initialize measurement data */
	temperature_value.ready = false;
	temperature_value.value = 0;
	temperature_value.ts = 0;
	pressure_value.ready = false;
	pressure_value.value = 0;
	pressure_value.ts = 0;

	/* No conversion is in progress */
	conversionInProgress = NONE;
}

void Bmp180::readCalibData()
{
	bool ret_status;

	if(status == OK)
	{
		uint8_t buf_calib_data[sizeof(T_BMP180_calib_data)];
		ret_status = i2c_drv_ptr->writeByte(BMP180_CHIP_ID_CALIB_EEP_START_ADDR, BMP180_I2C_ADDR, false);

		if(ret_status)
		{
			ret_status = i2c_drv_ptr->read(BMP180_I2C_ADDR, sizeof(T_BMP180_calib_data), buf_calib_data);
			if(ret_status)
			{
				/* Check that the read has been correctly performed,
				 * none of the calibration data shall be equal to 0 or 0xffff */
				uint16_t* cal = (uint16_t*)buf_calib_data;
				for(uint8_t i=0; i<sizeof(T_BMP180_calib_data)/2; i++)
				{
					if((*cal == 0) || (*cal == 0xffff))
						status = COMM_FAILED;

					cal++;
				}

				/* If the status is still OK, copy the buffer into the calibration data structure
				 * An inversion is done between MSB and LSB during the copy because AtMega2560 is little endian */
				if(status == OK)
				{
					uint8_t* ptr = (uint8_t*)&calibration_data;
					for(uint8_t i=0; i<sizeof(T_BMP180_calib_data); i=i+2)
					{
						*ptr = buf_calib_data[i+1];
						*(ptr+1) = buf_calib_data[i];
						ptr=ptr+2;;
					}
				}
			}
			else
				status = COMM_FAILED;
		}
		else
			status = COMM_FAILED;
	}
}

void Bmp180::readChipID()
{
	bool ret_status;

	if(status == OK)
	{
		ret_status = i2c_drv_ptr->writeByte(BMP180_CHIP_ID_EEP_ADDR, BMP180_I2C_ADDR, false);

		if(ret_status)
		{
			ret_status = i2c_drv_ptr->read(BMP180_I2C_ADDR, 1, &chip_id);
			if((chip_id != BMP180_CHIP_ID_EXPECTED) || (!ret_status))
				status = COMM_FAILED;
		}
		else
			status = COMM_FAILED;
	}
}

bool Bmp180::getTemperatureValue(uint16_t* data)
{
	bool retval = false;

	/* If the status is not OK, no value is returned */
	if(status == OK)
	{
		/* Data is returned only if a measurement is ready */
		if(temperature_value.ready)
		{
			retval = true;
			*data = temperature_value.value;
		}

		/* Value is valid for only x PITs */
		if(p_global_scheduler->getPitNumber() - temperature_value.ts > BMP180_DATA_VALIDITY_TMO)
			temperature_value.ready = false;

		/* A new measurement is started */
		startNewTemperatureConversion();
	}

	return retval;
}

void Bmp180::startNewTemperatureConversion()
{
	bool ret_status;

	if((status == OK) && (conversionInProgress == NONE))
	{
		uint8_t data[2] = {BMP180_CTRL_MEAS_EEP_ADDR, BMP180_CTRL_MEAS_START_TEMP_CONV};
		ret_status = i2c_drv_ptr->write(data, BMP180_I2C_ADDR, 2, true);

		/* If the conversion is started, start a timer, else set the driver status as failed */
		if(ret_status)
		{
			p_global_BSW_timer->configureTimer3(BMP180_TIMER_PRESCALER_VALUE, BMP180_TEMP_MEAS_TIMER_CTC_VALUE);
			p_global_BSW_timer->startTimer3();
			conversionInProgress = TEMPERATURE;
		}
		else
			status = COMM_FAILED;
	}
}

void Bmp180::conversionTimerInterrupt()
{
	bool ret_status;

	/* Stop timer */
	p_global_BSW_timer->stopTimer3();

	/* If driver status is OK and a conversion is in progress */
	if((status == OK) && (conversionInProgress != NONE))
	{
		/* Read the result in sensor EEPROM */
		ret_status = i2c_drv_ptr->writeByte(BMP180_OUT_REG_LSB_EEPROM_ADDR, BMP180_I2C_ADDR, false);

		if(ret_status)
		{
			uint8_t lsb, msb;
			ret_status = i2c_drv_ptr->read(BMP180_I2C_ADDR, 1, &lsb);

			if(ret_status)
			{
				ret_status = i2c_drv_ptr->writeByte(BMP180_OUT_REG_MSB_EEPROM_ADDR, BMP180_I2C_ADDR, false);
				if(ret_status)
				{
					ret_status = i2c_drv_ptr->read(BMP180_I2C_ADDR, 1, &msb);
					if(ret_status)
					{
						uint16_t RawValue = (msb << 8) + lsb;
						/* Convert the value in real temperature or pressure */
						if(conversionInProgress == TEMPERATURE)
							CalculateTemperature(RawValue);

					}
					else
						status = COMM_FAILED;
				}
				else
					status = COMM_FAILED;
			}
			else
				status = COMM_FAILED;
		}
		else
			status = COMM_FAILED;

		conversionInProgress = NONE;
		/* TODO : conversion in progress flag could be put in driver status */
	}
}

void Bmp180::CalculateTemperature(uint16_t UT)
{
	int32_t X1 = ((int32_t)UT - (int32_t)calibration_data.AC6)*(int32_t)calibration_data.AC5 / (int32_t)(32768);
	int32_t X2 = (int32_t)calibration_data.MC * (int32_t)(2048) / (X1 + (int32_t)calibration_data.MD);
	int32_t B5 = X1+X2;
	temperature_value.value = (uint16_t)((B5 + 8)/(16));
	temperature_value.ready = true;
	temperature_value.ts = p_global_scheduler->getPitNumber();
}
