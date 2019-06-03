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

#include "../lib/LinkedList/LinkedList.h"
#include "../lib/operators/operators.h"

#include "../bsw/timer/timer.h"
#include "../bsw/cpuLoad/CpuLoad.h"

#include "../asw/asw.h"

#include "scheduler.h"

#include "../main.h"


scheduler* p_global_scheduler;

scheduler::scheduler()
{
	/* Initialize task configuration structure */
	TasksLL_ptr = new LinkedList();

	/* Create Timer object of needed */
	if(p_global_BSW_timer == 0)
		p_global_BSW_timer = new timer();

	/* Initialize CPU load computation */
	if(isDebugModeActivated && (p_global_BSW_cpuload == 0))
	{
		p_global_BSW_cpuload = new CpuLoad();
	}

	/* Initialize counter to 1, then the tasks are not started at first PIT to avoid HW initialization issue */
	pit_number = 1;

	/* No task exists now */
	task_count = 0;

	/* Configure timer for periodic interrupt */
	p_global_BSW_timer->configureTimer1(PRESCALER_PERIODIC_TIMER, TIMER_CTC_VALUE);
}

void scheduler::launchPeriodicTasks()
{
	uint8_t task_nb = 0; /* Used for debug */
	Task_t* cur_task;

	/* First reset element pointer */
	TasksLL_ptr->ResetElementPtr();

	/* Check if there is at least one task to launch */
	if(TasksLL_ptr->IsLLEmpty() == false)
	{
		/* Parse all tasks */
		do
		{
			cur_task = (Task_t*)(TasksLL_ptr->getCurrentElement());
			/* If the task shall be launched at the current cycle */
			if((pit_number % (cur_task->period / SW_PERIOD_MS)) == 0)
			{
				task_nb++;

				/* Launch the task */
				(*cur_task->TaskPtr)();
			}

		}
		while(TasksLL_ptr->MoveToNextElement());
	}

	/* Compute CPU load */
	if(p_global_BSW_cpuload != 0)
		p_global_BSW_cpuload->ComputeCPULoad();

	/* Increment counter */
	pit_number++;
}

void scheduler::startScheduling()
{
	p_global_BSW_timer->startTimer1();
}


void scheduler::addPeriodicTask(TaskPtr_t task_ptr, uint16_t a_period)
{
	Task_t* new_task;

	/* Create a new task with the given parameters */
	new_task = new Task_t;
	new_task->TaskPtr = task_ptr;
	new_task->period = a_period;

	TasksLL_ptr->AttachNewElement((void*)new_task);

	task_count++;
}

uint32_t scheduler::getPitNumber()
{
	return pit_number;
}


bool scheduler::removePeriodicTask(TaskPtr_t task_ptr)
{
	bool result;

	result = TasksLL_ptr->RemoveElement((CompareFctPtr_t)&scheduler::LLElementCompare, (void*)task_ptr);

	if(result)
		task_count--;

	return result;
}

bool scheduler::LLElementCompare(void* LLElement, void* CompareElement)
{
	Task_t* task_ptr = (Task_t*)LLElement;
	TaskPtr_t compare_ptr = (TaskPtr_t)CompareElement;

	if(task_ptr->TaskPtr == compare_ptr)
		return true;
	else
		return false;
}

bool scheduler::updateTaskPeriod(TaskPtr_t task_ptr, uint16_t period)
{
	Task_t* task_data_ptr;
	bool TaskFound = false;

	/* Find the task into the task chain */
	TaskFound = TasksLL_ptr->FindElement((CompareFctPtr_t)&scheduler::LLElementCompare, (void*)task_ptr, (void**)&task_data_ptr);

	if(TaskFound)
		task_data_ptr->period = period;

	return TaskFound;
}
