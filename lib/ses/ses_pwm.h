#ifndef SES_PWM_H_
#define SES_PWM_H_

/*INCLUDES-------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "ses_common.h"
#include <avr/interrupt.h>

/*PROTOTYPES-----------------------------------------------------------------*/
/**
 * Intializes the PWM, used to generate a PWM 
 * signal with a fixed frequency but adjustable duty cycle can be used to
 * control the motor speed
 * uses timer0 as the clock to supply the power to the motor
 */
void pwm_init(void);

/**
 * Used to set the duty cycle of the PWM
 * in order to control the DC motor
 *
 * @param dutyCycle		specifies the ON percentage of pulse duration
 */
void pwm_setDutyCycle(uint8_t dutyCycle);

/**
 * Used to toggle the state of the motor
 * in order to control the DC motor
 * used as jotstick button callBack
 */
void pwm_toggleMotor();

#endif /* SES_PWM_H_ */
