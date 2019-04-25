/*!
 * @file bsw.cpp
 * @brief BSW main file
 *
 *  @date 13 mars 2018
 *  @author nicls67
 */

#include <avr/io.h>
#include <stdlib.h>



#include "bsw.h"

T_BSW_cnf_struct BSW_cnf_struct;

void bsw_init()
{
	BSW_cnf_struct.p_usart = new usart(USART_BAUDRATE);
	BSW_cnf_struct.p_dio = new dio();
	BSW_cnf_struct.p_timer = new timer();
	BSW_cnf_struct.p_dht22 = new dht22();
	BSW_cnf_struct.p_cpuload = new CpuLoad();
	BSW_cnf_struct.p_i2c = new I2C(I2C_BITRATE);

	BSW_cnf_struct.p_lcd = 0;
}

