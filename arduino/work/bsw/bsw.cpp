/*!
 * @file bsw.cpp
 * @brief BSW main file
 *
 *  @date 13 mars 2018
 *  @author nicls67
 */

#include <avr/io.h>
#include <stdlib.h>
#include <avr/wdt.h>

#include "../lib/String/String.h"

#include "usart/usart.h"
#include "timer/timer.h"
#include "I2C/I2C.h"
#include "lcd/LCD.h"
#include "dio/dio.h"
#include "dht22/dht22.h"
#include "cpuLoad/CpuLoad.h"
#include "wdt/Watchdog.h"
#include "bmp180/Bmp180.h"
#include "clock/Clock.h"

#include "bsw.h"

void bsw_init()
{
	/* Watchdog shall be initialized first, to provoke a watchdog reset as early as possible */
	p_global_BSW_wdg = new Watchdog(WDG_TMO_1S);

	p_global_BSW_dio = new dio();

	p_global_BSW_timer = 0;
	p_global_BSW_cpuload = 0;
	p_global_BSW_i2c = 0;
	p_global_BSW_lcd = 0;
	p_global_BSW_usart = 0;
	p_global_BSW_dht22 = 0;
	p_global_BSW_bmp180 = 0;
	p_global_BSW_Clock = 0;
}

