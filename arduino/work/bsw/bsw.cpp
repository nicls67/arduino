/*!
 * @file bsw.cpp
 * @brief BSW main file
 *
 *  @date 13 mars 2018
 *  @author nicls67
 */

#include <avr/io.h>
#include <stdlib.h>

#include "../lib/string/String.h"

#include "usart/usart.h"
#include "timer/timer.h"
#include "I2C/I2C.h"
#include "lcd/LCD.h"
#include "dio/dio.h"
#include "dht22/dht22.h"
#include "cpuLoad/CpuLoad.h"

#include "bsw.h"

T_BSW_cnf_struct BSW_cnf_struct;

void bsw_init()
{
	BSW_cnf_struct.p_dio = new dio();

	BSW_cnf_struct.p_timer = 0;
	BSW_cnf_struct.p_cpuload = 0;
	BSW_cnf_struct.p_i2c = 0;
	BSW_cnf_struct.p_lcd = 0;
	BSW_cnf_struct.p_usart = 0;
	BSW_cnf_struct.p_dht22 = 0;
}

