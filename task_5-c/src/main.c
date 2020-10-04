/*
/*INCLUDES ************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include "ses_led.h"
#include "ses_timer.h"
#include "ses_button.h"
#include "ses_lcd.h"
#include "ses_uart.h"
#include <avr/io.h>
#include "ses_pwm.h"
#include "ses_scheduler.h"
#include "ses_motorFrequency.h"

#define BAUDRATE 57600

int main(void)
{
    uint16_t kP, kI, kD, aW, fT, fC, e, eI, eLast, u;

    kP = 10;
    kI = 20;
    kD = 15;
    aW = 25; //limits the integration (anti wind-up)

    fT = 100;             //target Frequancy
    //fC = recentFrequency; //current Frequancy

    e = fT - fC;

    //eI = max(min((eI + e), aW), -aW);

    u = kP * e + kI * eI + kD * (eLast - e);

    pwm_setDutyCycle(u);
} 