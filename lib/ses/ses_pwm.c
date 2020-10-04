/*INCLUDES ************************************************************/
#include "util/atomic.h"
#include "ses_pwm.h"
#include "ses_common.h"
#include "avr/interrupt.h"
#include "ses_lcd.h"
#include "ses_motorFrequency.h"

/* DEFINES & MACROS **********************************************************/

#define PWM_OUTPUT_PORT PORTG
#define PWM_OUTPUT_PIN 5

/* PRIVATE VARIABLES **************************************************/
static uint8_t motorState = 0; // flag to monitor motor state, 0 = off, 1 = on.
static uint8_t dutyCycle = 90;

void pwm_setDutyCycle(uint8_t dutyCycle)
{
    OCR0B = 255 - ((256 / 100) * dutyCycle); // for inverted PWM mode
}

void pwm_init(void)
{
    // setting the data direction of PWM pin as output
    DDR_REGISTER(PWM_OUTPUT_PORT) |= (1 << PWM_OUTPUT_PIN);

    // disabling the prescalar and using the internal cpu clock
    TCCR0B |= (1 << CS00);
    TCCR0B &= ~((1 << CS01) | (1 << CS02));

    // configuring the timer to set OC0B when counter reaches OCR0B, inverting mode
    TCCR0A |= ((1 << COM0B0) | (1 << COM0B1));

    //enabling fast PWM with top at 0xFF, mode '3'
    TCCR0A |= ((1 << WGM00) | (1 << WGM01));
    TCCR0B &= ~(1 << WGM02);

    // start motor in an off state
    pwm_setDutyCycle(0);

    //enabling timer0
    PRR0 &= ~(1 << PRTIM0);
}

void pwm_toggleMotor()
{
    if (motorState == 1) // if motor is already on
    {
        pwm_setDutyCycle(0); // set duty cycle to zero to turn off
        motorState = 0;
        TCCR0B &= ~(1 << CS00); //disconnect clock
    }
    else
    {
        pwm_setDutyCycle(dutyCycle);
        motorState = 1;
        TCCR0B |= (1 << CS00);
    }
}
