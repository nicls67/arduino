/*!
 * @file scheduler.cpp
 *
 * @brief Defines scheduler class
 *
 * @date 16 mars 2018
 * @author nicls67
 */

#include <stdlib.h>
#include <avr/io.h>

#include "../lib/operators.h"

#include "../bsw/usart/usart.h"
#include "../bsw/dio/dio.h"
#include "../bsw/timer/timer.h"
#include "../bsw/dht22/dht22.h"
#include "../bsw/bsw.h"
#include "../asw/log/log.h"
#include "../asw/keepAliveLed/keepAliveLed.h"
#include "../asw/TempSensor/TempSensor.h"
#include "../asw/asw.h"
#include "../main.h"

#include "scheduler.h"

scheduler* p_scheduler; /*!< Pointer to scheduler object */

scheduler::scheduler()
{
	/* Initialize task configuration structure */
	Task_cnf_struct.task_nb = 0;
	Task_cnf_struct.firstTask = 0;

	/* Initialize counter to 1, then the tasks are not started at first PIT to avoid HW initialization issue */
	pit_number = 1;

	/* Configure timer for periodic interrupt */
	BSW_cnf_struct.p_timer->configureTimer1(PRESCALER_PERIODIC_TIMER, TIMER_CTC_VALUE);
}

void scheduler::launchPeriodicTasks()
{
	Task_t* cur_task = Task_cnf_struct.firstTask;
	uint8_t task_nb = 0; /* Used for debug */

	/* Parse all tasks */
	while(cur_task != 0)
	{
		/* If the task shall be launched at the current cycle */
		if((pit_number % (cur_task->period / SW_PERIOD_MS)) == 0)
		{
			task_nb++;
//#ifdef DEBUG_FLAG
//			ASW_cnf_struct.p_usartDebug->sendData((char*)"Launching task ");
//			ASW_cnf_struct.p_usartDebug->sendInteger(task_nb,10);
//			ASW_cnf_struct.p_usartDebug->sendData((char*)"\n");
//#endif
			/* Launch the task */
			(*cur_task->TaskPtr)();
		}

		/* Go to next task */
		cur_task = cur_task->nextTask;
	}

	/* Increment counter */
	pit_number++;

//#ifdef DEBUG_FLAG
//	ASW_cnf_struct.p_usartDebug->sendData((char*)"pit number ");
//	ASW_cnf_struct.p_usartDebug->sendInteger(pit_number,10);
//	ASW_cnf_struct.p_usartDebug->sendData((char*)"\n");
//#endif
}

void scheduler::startScheduling()
{
	BSW_cnf_struct.p_timer->startTimer1();
}


void scheduler::addPeriodicTask(TaskPtr_t task_ptr, uint16_t a_period)
{
	Task_t* new_task;
	Task_t* cur_task;

	/* Create a new task with the given parameters */
	new_task = new Task_t;
	new_task->TaskPtr = task_ptr;
	new_task->period = a_period;
	new_task->nextTask = 0;

	cur_task = Task_cnf_struct.firstTask;

	if(cur_task ==0)
		Task_cnf_struct.firstTask = new_task;
	else
	{
		/* Find the last task of the chain */
		while(cur_task->nextTask != 0)
			cur_task = cur_task->nextTask;

		/* Link the new task to the chain */
		cur_task->nextTask = new_task;
	}

	/* Increment task number */
	Task_cnf_struct.task_nb++;

}

inline uint32_t scheduler::getPitNumber()
{
	return pit_number;
}
