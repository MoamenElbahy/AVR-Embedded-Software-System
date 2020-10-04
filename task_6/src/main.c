/*INCLUDES ************************************************************/
#include <stdint.h>
#include "ses_led.h"
#include "ses_timer.h"
#include "ses_button.h"
#include "ses_lcd.h"
#include "ses_uart.h"
#include "ses_alarmClock.h"
#include "ses_scheduler.h"
#include <avr/io.h>

int main(void)
{
    //Initializations
    lcd_init();
    button_init(true);
    scheduler_init();
    led_redInit();
    led_yellowInit();
    led_greenInit();
    fsm_init(&theFsm, setTimeHour);

    //Button Callbacks
    pButtonCallback pJoystickPressedDispatch = joystickPressedDispatch;
    button_setJoystickButtonCallback(pJoystickPressedDispatch);
    pButtonCallback pRotatyPressedDispatch = rotaryPressedDispatch;
    button_setRotaryButtonCallback(pRotatyPressedDispatch);

    //Enable Interrupts
    sei();

    scheduler_run();

    return 0;
}