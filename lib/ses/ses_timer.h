#ifndef SES_TIMER_H_
#define SES_TIMER_H_

/*INCLUDES *******************************************************************/

#include "ses_common.h"
#include <avr/interrupt.h>

/* DEFINES & MACROS **********************************************************/

#define PRESCALER1 64			   // prescaler to to change the clock frequancy
#define PRESCALER5 1024
#define FRQ_CPU 16000000
#define MODIFIER 10 // number of interrupts per second
#define TIMER2_CYC_FOR_1MILLISEC (FRQ_CPU/1000) / PRESCALER1

/*PROTOTYPES *****************************************************************/

/**type of function pointer used as timer callback
 */
typedef void (*pTimerCallback)(void);

typedef uint16_t (*pTimer5Callback)(void);

/**
 * Sets a function to be called when the timer fires. If NULL is
 * passed, no callback is executed when the timer fires.
 *
 * @param cb  valid pointer to callback function
 */
void timer2_setCallback(pTimerCallback cb);

/**
 * Starts hardware timer 2 of MCU with a period
 * of 1 ms.
 */
void timer2_start();

/**
 * Stops timer 2.
 */
void timer2_stop();

/**
 * Sets a function to be called when the timer fires.
 *
 * @param cb  pointer to the callback function; if NULL, no callback
 *            will be executed.
 */
void timer1_setCallback(pTimerCallback cb);

/**
 * Start timer 1 of MCU to trigger on compare match every 5ms.
 */
void timer1_start(uint8_t timer1Interval);

/**
 * Stops timer 1 of the MCU if it is no longer needed.
 */
void timer1_stop();



/**
 * Sets a function to be called when the timer fires.
 *
 * @param cb  pointer to the callback function; if NULL, no callback
 *            will be executed.
 */
void timer5_setCallback(pTimer5Callback cb);

/**
 * Starts hardware timer 5 of MCU.
 */
void timer5_start();

/**
 * Stops timer 5.
 */
void timer5_stop();

#endif /* SES_TIMER_H_ */
