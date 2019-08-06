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
	status = IDLE;

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

	/* Initialize activation flags */
	isTempConvActivated = false;
	isPressConvActivated = false;

	/* Add monitoring function into scheduler */
	task_period = BMP180_MONITORING_DEFAULT_PERIOD;
	p_global_scheduler->addPeriodicTask((TaskPtr_t)(&Bmp180::Bmp180Monitoring_Task), task_period);
}

void Bmp180::readCalibData()
{
	bool ret_status;

	if(status == IDLE)
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
				if(status == IDLE)
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

//	p_global_ASW_DebugInterface->sendInteger(calibration_data.AC1,10);
//	p_global_ASW_DebugInterface->nextLine();
//	p_global_ASW_DebugInterface->sendInteger(calibration_data.AC2,10);
//	p_global_ASW_DebugInterface->nextLine();
//	p_global_ASW_DebugInterface->sendInteger(calibration_data.AC3,10);
//	p_global_ASW_DebugInterface->nextLine();
//	p_global_ASW_DebugInterface->sendInteger(calibration_data.AC4,10);
//	p_global_ASW_DebugInterface->nextLine();
//	p_global_ASW_DebugInterface->sendInteger(calibration_data.AC5,10);
//	p_global_ASW_DebugInterface->nextLine();
//	p_global_ASW_DebugInterface->sendInteger(calibration_data.AC6,10);
//	p_global_ASW_DebugInterface->nextLine();
//	p_global_ASW_DebugInterface->sendInteger(calibration_data.B1,10);
//	p_global_ASW_DebugInterface->nextLine();
//	p_global_ASW_DebugInterface->sendInteger(calibration_data.B2,10);
//	p_global_ASW_DebugInterface->nextLine();
//	p_global_ASW_DebugInterface->sendInteger(calibration_data.MB,10);
//	p_global_ASW_DebugInterface->nextLine();
//	p_global_ASW_DebugInterface->sendInteger(calibration_data.MC,10);
//	p_global_ASW_DebugInterface->nextLine();
//	p_global_ASW_DebugInterface->sendInteger(calibration_data.MD,10);
//	p_global_ASW_DebugInterface->nextLine();
}

void Bmp180::readChipID()
{
	bool ret_status;

	if(status == IDLE)
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
	*data = temperature_value.value;

	return temperature_value.ready;
}

bool Bmp180::getPressureValue(uint16_t* data)
{
	*data = pressure_value.value;

	return pressure_value.ready;
}

void Bmp180::startNewTemperatureConversion()
{
	bool ret_status;

	if(status == IDLE)
	{
		uint8_t data[2] = {BMP180_CTRL_MEAS_EEP_ADDR, BMP180_CTRL_MEAS_START_TEMP_CONV};
		ret_status = i2c_drv_ptr->write(data, BMP180_I2C_ADDR, 2, true);

		/* If the conversion is started, start a timer, else set the driver status as failed */
		if(ret_status)
		{
			p_global_BSW_timer->configureTimer3(BMP180_TIMER_PRESCALER_VALUE, BMP180_TEMP_MEAS_TIMER_CTC_VALUE);
			p_global_BSW_timer->startTimer3();
			status = TEMP_CONV_IN_PROGRESS;
		}
		else
			status = COMM_FAILED;
	}
}

void Bmp180::startNewPressureConversion()
{
	bool ret_status;

	if(status == IDLE)
	{
		uint8_t data[2] = {BMP180_CTRL_MEAS_EEP_ADDR, BMP180_CTRL_MEAS_START_PRESS_CONV_OSS0};
		ret_status = i2c_drv_ptr->write(data, BMP180_I2C_ADDR, 2, true);

		/* If the conversion is started, start a timer, else set the driver status as failed */
		if(ret_status)
		{
			p_global_BSW_timer->configureTimer3(BMP180_TIMER_PRESCALER_VALUE, BMP180_PRESS_MEAS_OSS0_TIMER_CTC_VALUE);
			p_global_BSW_timer->startTimer3();
			status = PRESSURE_CONV_IN_PROGRESS;
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
	if(status != COMM_FAILED)
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
						if(status == TEMP_CONV_IN_PROGRESS)
						{
							CalculateTemperature(RawValue);
							status = IDLE;

							/* Start pressure conversion */
							if(isPressConvActivated)
								startNewPressureConversion();
						}
						else if(status == PRESSURE_CONV_IN_PROGRESS)
						{
							CalculatePressure(RawValue);
							status = IDLE;
						}
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
	}
}

void Bmp180::CalculateTemperature(uint16_t UT)
{
	int32_t X1 = ((int32_t)UT - (int32_t)calibration_data.AC6)*(int32_t)calibration_data.AC5 / (int32_t)(32768);
	int32_t X2 = (int32_t)calibration_data.MC * (int32_t)(2048) / (X1 + (int32_t)calibration_data.MD);
	B5_mem = X1+X2;

	temperature_value.value = (uint16_t)((B5_mem + 8)/(16));
	temperature_value.ready = true;
	temperature_value.ts = p_global_scheduler->getPitNumber();
}

void Bmp180::CalculatePressure(uint32_t UP)
{
	int32_t B6 = B5_mem - 4000;
	int32_t X1 = (calibration_data.B2 * (B6 * B6 / 4096))/ 2048;
	int32_t X2 = calibration_data.AC2 * B6 / 2048;
	int32_t X3 = X1 + X2;
	int32_t B3 = (((calibration_data.AC1 * 4 + X3) )+2)/4;
	X1 = calibration_data.AC3 * B6 / 8192;
	X2 = (calibration_data.B1 * (B6 * B6 / 4096)) / 65536;
	X3 = ((X1+X2)+2)/4;
	uint32_t B4 = calibration_data.AC4 * (uint32_t)(X3+32768)/32768;
	uint32_t B7 = ((uint32_t)UP - B3) * (50000);

	int32_t p;
	if(B7 < 0x80000000)
		p = (B7*2)/B4;
	else
		p = (B7/B4)*2;

	X1 = (p/256)*(p/256);
	X1 = (X1*3038)/65536;
	X2 = (-7357 * p)/65536;
	p = p + (X1 + X2 + 3791)/ 16;

	pressure_value.value = (uint16_t)(p/10); /* Remove last digit */
	pressure_value.ready = true;
	pressure_value.ts = p_global_scheduler->getPitNumber();

}

void Bmp180::Bmp180Monitoring_Task()
{
	/* If the status is equal to IDLE, start a new temperature conversion */
	if((p_global_BSW_bmp180->getStatus() == IDLE) && p_global_BSW_bmp180->isTempConversionActivated())
		p_global_BSW_bmp180->startNewTemperatureConversion();

	/* Monitoring of temperature and pressure value */
	p_global_BSW_bmp180->TemperatureMonitoring();
	p_global_BSW_bmp180->PressureMonitoring();
}

void Bmp180::TemperatureMonitoring()
{
	if(p_global_scheduler->getPitNumber() - temperature_value.ts > ((task_period/SW_PERIOD_MS)*2) )
		temperature_value.ready = false;
}

void Bmp180::PressureMonitoring()
{
	if(p_global_scheduler->getPitNumber() - pressure_value.ts > ((task_period/SW_PERIOD_MS)*2) )
		pressure_value.ready = false;
}

void Bmp180::ActivateTemperatureConversion(uint16_t req_period)
{
	isTempConvActivated = true;

	/* Currently task period is updated directly,
	 * then pressure and temperature conversion have always the same period */
	task_period = req_period;
	p_global_scheduler->updateTaskPeriod((TaskPtr_t)(&Bmp180::Bmp180Monitoring_Task), task_period);
}

void Bmp180::ActivatePressureConversion(uint16_t req_period)
{
	isPressConvActivated = true;

	/* Currently task period is updated directly,
	 * then pressure and temperature conversion have always the same period */
	task_period = req_period;
	p_global_scheduler->updateTaskPeriod((TaskPtr_t)(&Bmp180::Bmp180Monitoring_Task), task_period);
}

void Bmp180::StopTemperatureConversion()
{
	isTempConvActivated = false;
}

void Bmp180::StopPressureConversion()
{
	isPressConvActivated = false;
}
