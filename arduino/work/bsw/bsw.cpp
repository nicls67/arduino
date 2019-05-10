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
	BSW_cnf_struct.p_usart = 0;
	BSW_cnf_struct.p_dio = new dio();
	BSW_cnf_struct.p_timer = new timer();
	BSW_cnf_struct.p_dht22 = new dht22(); /* TODO : DHT22 sensor class shall be created by ASW user class */
	BSW_cnf_struct.p_cpuload = new CpuLoad(); /* TODO : Create a configuration (scheduler ?) to manage if cpu load is needed or not */
	BSW_cnf_struct.p_i2c = 0;
	BSW_cnf_struct.p_lcd = 0;
}

