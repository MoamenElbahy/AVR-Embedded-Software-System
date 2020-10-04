#ifndef SES_MOTORFREQUENCY_H_
#define SES_MOTORFREQUENCY_H_

/*INCLUDES-------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ses_common.h"
#include <avr/interrupt.h>

/*PROTOTYPES-----------------------------------------------------------------*/
/**
 * Sets the external interrupt control registers
 * and unmasks the interrupt
 * also initializes the motor status LEDs,
 * and starts the timer for the speed calculation
 */ 
void motorFrequency_init();

/**
 * computes the value of the recently calculated motor frequency in RPM
 * called every 100 ms by timer5
 * also sets the motor status LEDs accordingly
 * @return the recent computed motor frequency in RPM
 */
uint16_t motorFrequency_getRecent();

/**
 * computes the median value of the recent N recordings of frequency measurements
 * specified by arraySize global variable
 * in case of an even array size, it computes the average of the two
 * mid values
 * @return median of the last N number of specified frequency samples, in RPM
 */
uint16_t motorFrequency_getMedian();

/**
 * Used as a task for the scheduler
 * called every 1s to display to the LCD
 * the recent frequency and median calculations
 */
void frequency_displayer();

#endif /* SES_MOTORFREQUENCY_H_ */
