/*!
 * @file CpuLoad.cpp
 *
 * @brief Defines functions of class CpuLoad
 *
 * @date 21 mars 2019
 * @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>

#include "../../lib/LinkedList/LinkedList.h"
#include "../../scheduler/scheduler.h"
#include "../timer/timer.h"
#include "CpuLoad.h"

CpuLoad* p_global_BSW_cpuload;

CpuLoad::CpuLoad()
{
	uint8_t i;

	/* Initialize class members */
	current_load = 0;
	avg_load = 0;
	sample_cnt = 0;
	sample_idx = 0;
	last_sum_value = 0;
	max_load = 0;

	for(i=0;i<NB_OF_SAMPLES;i++)
		sample_mem[i] = 0;

	/* Create timer object if it is still not initialized */
	if(p_global_BSW_timer == 0)
		p_global_BSW_timer = new timer();
}

void CpuLoad::ComputeCPULoad()
{
	uint16_t counter_value = p_global_BSW_timer->getTimer1Value();

	/* Compute current load */
	current_load = (uint8_t)((uint32_t)(counter_value * 100) / (uint32_t)TIMER_CTC_VALUE);

	if(sample_cnt < NB_OF_SAMPLES)
		sample_cnt++;

	/* Compute average load */
	last_sum_value = last_sum_value - sample_mem[sample_idx] + current_load;
	avg_load = last_sum_value / sample_cnt;

	/* Memorize the value */
	sample_mem[sample_idx] = current_load;
	sample_idx = (sample_idx + 1) % NB_OF_SAMPLES;

	/* Compute maximum load */
	if(current_load > max_load)
		max_load = current_load;

}

