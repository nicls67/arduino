/*!
 * @file CpuLoad.cpp
 *
 * @brief Defines functions of class CpuLoad
 *
 * @date 21 mars 2019
 * @author nicls67
 */

#include <avr/io.h>
#include "CpuLoad.h"

CpuLoad::CpuLoad()
{
	uint8_t i;
	current_load = 0;
	avg_load = 0;
	sample_cnt = 0;
	sample_idx = 0;
	last_sum_value = 0;
	max_load = 0;

	for(i=0;i<NB_OF_SAMPLES;i++)
		sample_mem[i] = 0;
}

void CpuLoad::ComputeCPULoad()
{
	uint16_t counter_value = BSW_cnf_struct.p_timer->getTimer1Value();

	/* Compute current load */
	current_load = (uint8_t)((uint32_t)(counter_value * 100) / (uint32_t)TIMER_CTC_VALUE);

	if(sample_cnt < NB_OF_SAMPLES)
		sample_cnt++;

	/* Compute average load */
	avg_load = (last_sum_value - sample_mem[sample_idx] + current_load) / sample_cnt;

	/* Memorize the value */
	sample_mem[sample_idx] = current_load;
	sample_idx = (sample_idx + 1) % NB_OF_SAMPLES;

	/* Compute maximum load */
	if(current_load > max_load)
		max_load = current_load;

}

