#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/*INCLUDES *******************************************************************/
#include "ses_common.h"
#include "ses_alarmClock.h"

/* DEFINES & MACROS **********************************************************/

#define MAXTIME 86400000 //60*60*24*1000 for calculating from day to ms
#define HOURTOMS 3600000 //60*60*1000 for calc. from hours to ms
#define MINTOMS 60000 //60*1000 for calc. from min to ms
#define SECTOMS 1000 // sec to ms

/* TYPES ********************************************************************/

/**type of function pointer for tasks */
typedef void (*task_t)(void *);

/** Task structure to schedule tasks
 */
typedef struct taskDescriptor_s
{
	task_t task;				   ///< function pointer to call
	void *param;				   ///< pointer, which is passed to task when executed
	uint16_t expire;			   ///< time offset in ms, after which to call the task
	uint16_t period;			   ///< period of the timer after firing; 0 means exec once
	uint8_t execute : 1;		   ///< for internal use
	uint8_t reserved : 7;		   ///< reserved
	struct taskDescriptor_s *next; ///< pointer to next task, internal use
} taskDescriptor;

/**type of time-keeping variable */
typedef uint32_t systemTime_t;

bool alarmIsOn;

/* FUNCTION PROTOTYPES *******************************************************/

/**
 * Initializes the task scheduler. Uses hardware timer2 of the AVR.
 */
void scheduler_init();

/**
 * Runs scheduler in an infinite loop.
 */
void scheduler_run();

/**
 * Adds a new task to the scheduler.
 * May be called from any context (interrupt or main program)
 *
 * @param td   Pointer to taskDescriptor structure. The scheduler takes
 *             possesion of the memory pointed at by td until the task
 *             is removed by scheduler_remove or -- in case of a 
 *             non-periodic task -- the task is executed. td->expire 
 *             and td->execute are changed to by the task scheduler.
 *
 * @return     false, if task is already scheduled or invalid (NULL)
 *             true, if task was successfully added to scheduler and 
 *             will be executed after td->expire ms.
 */
bool scheduler_add(taskDescriptor *td);

/**
 * Removes a task from the scheduler.
 * May be called from any context (interrupt or main program)
 *
 * @param td	pointer to task descriptor to remove
 * */
void scheduler_remove(taskDescriptor *td);

/** Getter function for the global variable for 
 * system time keeping
 * 
 * @return systemTime_t	returns time in ms
 * */
systemTime_t scheduler_getTime();

/** Setter function for the global variable for 
 * system time keeping
 * 
 * @param systemTime_t takes time in ms as parameter
 * */
void scheduler_setTime(systemTime_t time);

/** Transforms system time (ms) to a more readable human format time
 * 
 * @return systemTime_t	returns time struct containing hours, minutes, seconds and milliseconds.
 * */
struct time_t scheduler_time();

/** Calculates the time in ms from time struct containting human-readable time
 * 
 * @param systemTime_t takes time in ms as parameter
 * 
 * @return systemTime_t returns the calculated time value in milliseconds
 * */
systemTime_t scheduler_calculateSystemTime(struct time_t time);

#endif /* SCHEDULER_H_ */
