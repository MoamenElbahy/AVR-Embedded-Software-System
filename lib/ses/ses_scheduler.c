/*INCLUDES ************************************************************/
#include "ses_timer.h"
#include "ses_scheduler.h"
#include "util/atomic.h"
#include "ses_lcd.h"

/* PRIVATE VARIABLES **************************************************/
static taskDescriptor *taskList = NULL; //Head of the linked list

static systemTime_t systemTime; // current time in ms

/*FUNCTION DEFINITION ************************************************
    Callback for timer
	Loop through taskList every timer interval, subtract 1 from expiry, and mark next for execution at expire = 0
	Reset expire to the period after execution for periodic tasks, and remove non-periodic tasks
*/
static void scheduler_update(void)
{
	taskDescriptor *currentTask = taskList; // Linked list head
	while (currentTask != NULL)				// Loop as long as the list is not empty
	{
		if (currentTask->expire == 0)
		{
			currentTask->execute = 1; // mark for execution
			if (currentTask->period != 0)
			{
				currentTask->expire = currentTask->period; //reset expire to the period
			}
		}
		(currentTask->expire)--;
		currentTask = currentTask->next; //move to the next task
	}
	//updating the time
	systemTime++;
	if (systemTime >= MAXTIME)
	{
		systemTime = 0;
	}
}

void scheduler_init()
{
	pTimerCallback timer_ptr;
	timer_ptr = &scheduler_update;
	timer2_setCallback(timer_ptr);
	sei();
	timer2_start();
}

void scheduler_run()
{
	// superloop, called from main
	while (1)
	{
		taskDescriptor task_toBeExecuted; //Struct which saves the task to be executed outside the atomic block
		taskDescriptor *currentTask = taskList;
		while (currentTask != NULL) // loop as long as the list is not empty
		{
			ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
			{
				if (currentTask->execute == 1)
				{
					task_toBeExecuted.task = currentTask->task;
					task_toBeExecuted.param = currentTask->param;
					task_toBeExecuted.execute = 1;
					currentTask->execute = 0;
					if (currentTask->period == 0) // remove non-periodic tasks
					{
						scheduler_remove(currentTask);
					}
				}
				currentTask = currentTask->next; //move to the next task
			}
			if (task_toBeExecuted.execute)
			{
				task_toBeExecuted.task(task_toBeExecuted.param);
				task_toBeExecuted.execute = 0;
			}
		}
	}
}

bool scheduler_add(taskDescriptor *toAdd)
{
	taskDescriptor *currentTask = taskList;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if (toAdd == NULL)
		{ //check for null task added
			return false;
		}

		if (taskList == NULL)
		// in case of an empty task list
		{
			taskList = toAdd; // toAdd becomes the head of the list
			return true;
		}
		else // incase of a populated list
		{
			while (currentTask->next != NULL) //check for end of the list
			{
				if (currentTask == toAdd)
				{ //if task to be added is already scheduled
					return false;
				}
				currentTask = currentTask->next;
			}
			if (currentTask == toAdd)
			{ //if -last- element is already scheduled (extra check)
				return false;
			}
			else //adding the task
			{
				currentTask->next = toAdd;
				return true;
			}
		}
	}
	return true;
}

void scheduler_remove(taskDescriptor *toRemove)
{
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if (toRemove != NULL)
		{
			taskDescriptor *current = taskList;
			if (toRemove == current) // if removing the first element of the list
			{
				taskList = current->next;
			}
			else
			{
				while ((current->next != toRemove) && (current->next != NULL))
				{
					current = current->next; // search for to remove in the list and make sure that it exists inside the list
				}
				if ((current->next != NULL) && (current->next != NULL))
				{
					current->next = current->next->next;
				}
			}
		}
	}
}

/* Calculation wrapper functions **************************************************/
uint8_t scheduler_calcHours(systemTime_t time) { return (time / (HOURTOMS)); }
uint8_t scheduler_calcMinutes(systemTime_t time) { return (time % (HOURTOMS)) / (MINTOMS); }
uint8_t scheduler_calcSeconds(systemTime_t time) { return ((time % (HOURTOMS)) % (MINTOMS)) / SECTOMS; }
uint16_t scheduler_calcMilliseconds(systemTime_t time) { return (((time % (HOURTOMS)) % (MINTOMS)) % SECTOMS); }

/* Getter and Setter functions for global time **************************************************/
systemTime_t scheduler_getTime()
{
	return systemTime;
}

void scheduler_setTime(systemTime_t time)
{
	systemTime = time;
}

/* Time conversions **************************************************/
struct time_t scheduler_time()
{
	struct time_t currentTime;
	while (systemTime > MAXTIME)
	{
		systemTime -= MAXTIME;
	}
	currentTime.hour = scheduler_calcHours(systemTime);
	currentTime.minute = scheduler_calcMinutes(systemTime);
	currentTime.second = scheduler_calcSeconds(systemTime);
	currentTime.milli = scheduler_calcMilliseconds(systemTime);
	return currentTime;
}

systemTime_t scheduler_calculateSystemTime(struct time_t time)
{
	return ((time.hour * HOURTOMS) + (time.minute * MINTOMS) + (time.second * SECTOMS) + time.milli);
}
