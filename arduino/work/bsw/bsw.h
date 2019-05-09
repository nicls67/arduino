/*!
 * @file bsw.h
 * @brief BSW main header file
 *
 * @date 13 mars 2018
 * @author nicls67
 */

#ifndef WORK_BSW_BSW_H_
#define WORK_BSW_BSW_H_


/*!
 * @brief BSW configuration structure
 * @details This structure contains all pointers to instanced drivers objects
 */
typedef struct
{
	usart* p_usart; /*!< Pointer to usart driver object */
	dio* p_dio; /*!< Pointer to dio driver object */
	timer* p_timer; /*!< Pointer to timer driver object */
	dht22* p_dht22; /*!< Pointer to dht22 driver object */
	CpuLoad* p_cpuload; /*!< Pointer to cpu load library object */
	I2C* p_i2c; /*!< Pointer to I2C driver object */
	LCD* p_lcd; /*!< Pointer to LCD driver object */
}
T_BSW_cnf_struct;

extern T_BSW_cnf_struct BSW_cnf_struct; /*!< BSW configuration structure */


/*! @brief Initialization of BSW
 *  @details This function instantiates all driver objects, leading hardware initialization.
 *           The addresses of driver objects are then stored in BSW_cnf_struct structure.
 *  @return Nothing
 */
void bsw_init();


#endif /* WORK_BSW_BSW_H_ */
