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



/*!
 * @brief DHT 22 driver class
 * @details This class defines all useful functions for DHT22 temperature and humidity sensor
 */
class dht22
{

public:

	/*!
	 * @brief dht22 class constructor.
	 * @details Initializes the class dht22.
	 * @param [in] port Encoded configuration of the port used for 1-wire communication.
	 * @return Nothing
	 */
	dht22(uint8_t port);

	/*!
	 * @brief Temperature get function
	 * @details This functions writes the temperature value at the given address and returns the validity of the data.
	 * 			If the values have not been refreshed during the current PIT, a read operation is performed on the DHT22 device.
	 *
	 * @param [in] temperature Address where the temperature shall be written
	 * @return Validity of the data
	 */
	bool getTemperature(uint16_t* temperature);

	/*!
	 * @brief Humidity get function
	 * @details This functions writes the humidity value at the given address and returns the validity of the data.
	 * 			If the values have not been refreshed during the current PIT, a read operation is performed on the DHT22 device.
	 *
	 * @param [in] humidity Address where the humidity shall be written
	 * @return Validity of the data
	 */
	bool getHumidity(uint16_t* humidity);


private:

	uint8_t dht22_port; /*!< Variable containing the port used for 1-wire communication */
	dio* dio_ptr; /*!< Pointer to the DIO object */
	uint16_t mem_temperature; /*!< Memorized value of temperature */
	uint16_t mem_humidity; /*!< Memorized value of humidity */
	bool mem_validity; /*!< Memorized value of validity */
	uint32_t pit_last_read; /*!< Value of the PIT number when the last read operation has been performed */

	/*!
	 * @brief Initializes the communication
	 * @details This function initializes the communication with DHT22 using 1-wire protocol
	 * @return Nothing
	 */
	void initializeCommunication();

	/*!
	 * @brief Reads the data from DHT22
	 * @details This function communicates with DHT22 using 1-wire protocol to read raw values of temperature and humidity.
	 * 			A checksum check is done when communication is finished to validate the received data.
	 * 			Validity of the data, temperature and humidity values and memorized in the associated class members.
	 *
	 * @return Nothing
	 */
	void read();

};

extern dht22* p_global_BSW_dht22; /*!< Pointer to dht22 driver object */

#endif /* WORK_BSW_DHT22_DHT22_H_ */
