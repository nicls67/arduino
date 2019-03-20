/*!
 * @file dht22.h
 *
 * @brief DHT22 driver header file
 *
 * @date 23 mars 2018
 * @author nicls67
 */

#ifndef WORK_BSW_DHT22_DHT22_H_
#define WORK_BSW_DHT22_DHT22_H_




#define DHT22_PORT ENCODE_PORT(PORT_B, 6) /*!< DHT22 is connected to port PB6 */

/*!
 * @brief DHT 22 driver class
 * @details This class defines all useful functions for DHT22 temperature and humidity sensor
 */
class dht22
{

public:

	/*!
	 * @brief dht22 class constructor
	 * @details Initializes the class dht22
	 * @return Nothing
	 */
	dht22();

	/*!
	 * @brief Reads the data from DHT22
	 * @details This function communicates with DHT22 using 1-wire protocol to read raw values of temperature and humidity.
	 * 			A checksum check is done when communication is finished to validate the received data
	 *
	 * @param [out] raw_humidity Raw humidity value received from sensor
	 * @param [out] raw_temperature Raw temperature value received from sensor
	 * @return Validity of the read value
	 */
	bool read(uint16_t* raw_humidity, uint16_t* raw_temperature);

private:

	/*!
	 * @brief Initializes the communication
	 * @details This function initializes the communication with DHT22 using 1-wire protocol
	 * @return Nothing
	 */
	void initializeCommunication();

};

#include "../bsw.h"
#include "../../asw/asw.h"

#endif /* WORK_BSW_DHT22_DHT22_H_ */
