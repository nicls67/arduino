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

			/* Launch the task */
			(*cur_task->TaskPtr)();
		}

		/* Go to next task */
		cur_task = cur_task->nextTask;
	}

	/* Increment counter */
	pit_number++;
}

void scheduler::startScheduling()
{
	BSW_cnf_struct.p_timer->startTimer1();
}


void scheduler::addPeriodicTask(TaskPtr_t task_ptr, uint16_t a_period, uint8_t a_task_id)
{
	Task_t* new_task;
	Task_t* cur_task;

	/* Create a new task with the given parameters */
	new_task = new Task_t;
	new_task->TaskPtr = task_ptr;
	new_task->period = a_period;
	new_task->task_id = a_task_id;
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

uint32_t scheduler::getPitNumber()
{
	return pit_number;
}


bool scheduler::removePeriodicTask(uint8_t a_task_id)
{
	/* First find the task in the scheduler */
	Task_t* cur_task = Task_cnf_struct.firstTask;
	Task_t* prev_task = Task_cnf_struct.firstTask;

	while ((cur_task != 0) && (cur_task->task_id != a_task_id))
	{
		prev_task = cur_task;
		cur_task = cur_task->nextTask;
	}


	/* If pointer is equal to 0, the requested task does not exist */
	if (cur_task == 0)
		return false;

	/* The task exists, remove it from scheduler */
	Task_cnf_struct.task_nb--;
	if (cur_task == Task_cnf_struct.firstTask)
		Task_cnf_struct.firstTask = cur_task->nextTask;
	else
		prev_task->nextTask = cur_task->nextTask;

	delete cur_task;

	return true;


}
