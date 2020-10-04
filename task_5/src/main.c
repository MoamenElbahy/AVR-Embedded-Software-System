/*INCLUDES ************************************************************/
#include <stdint.h>
#include "ses_led.h"
#include "ses_timer.h"
#include "ses_button.h"
#include "ses_lcd.h"
#include "ses_uart.h"
#include <avr/io.h>
#include "ses_pwm.h"
#include "ses_scheduler.h"
#include "ses_motorFrequency.h"

static taskDescriptor task_displayFrequency;

void frequency_taskCreator()
{
    task_displayFrequency.task = &frequency_displayer;
    task_displayFrequency.param = NULL;
    task_displayFrequency.expire = 0;
    task_displayFrequency.period = 1000;
    task_displayFrequency.execute = 0;
    scheduler_add(&task_displayFrequency);
}

int main(void)
{
    pwm_init();
    motorFrequency_init();
    lcd_init();
    button_init(true);
    sei();

    //Button Callbacks
    pButtonCallback pMotorButtonCallback = &pwm_toggleMotor;
    button_setJoystickButtonCallback(pMotorButtonCallback);
    pTimer5Callback pMotorFrequency = &motorFrequency_getRecent;
    timer5_setCallback(pMotorFrequency);

    frequency_taskCreator();
    scheduler_init();
    scheduler_run();

    while (1);
    return 0;
}