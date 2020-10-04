/*INCLUDES ************************************************************/
#include "ses_lcd.h"
#include "ses_scheduler.h"
#include "ses_alarmClock.h"

/* PRIVATE VARIABLES **************************************************/
static systemTime_t alarmTime;

void alarmClock_setTime(systemTime_t time)
{
    alarmTime = time;
}

systemTime_t alarmClock_getTime()
{
    return alarmTime;
}

/* Event Dispatching Functions **************************************************/

void joystickPressedDispatch(void *param)
{
    const Event e = {.signal = JOYSTICK_PRESSED};
    fsm_dispatch(&theFsm, &e);
}

void rotaryPressedDispatch(void *param)
{
    const Event e = {.signal = ROTARY_PRESSED};
    fsm_dispatch(&theFsm, &e);
}

void timeOutDispatch(void *param)
{
    const Event e = {.signal = TIMEOUT};
    fsm_dispatch(&theFsm, &e);
}

void alarmActivate()
{
    //Check first if alarm time matches current system time, then dispatch alarm-activated event
    if ((scheduler_getTime() == alarmClock_getTime()))
    {
        const Event e = {.signal = ALARM_ACTIVATED};
        fsm_dispatch(&theFsm, &e);
    }
}

void greenLedFlash()
{
    led_greenToggle();
}

/* Functionality **************************************************/

static void flashRedLed(void *param)
{
    led_redToggle();
}

static void initial_Display()
{
    lcd_setCursor(0, 1);
    fprintf(lcdout, "Set the clock time");
    lcd_setCursor(3, 2);
    fprintf(lcdout, "HH:MM:SS");
}

static void LCD_Display()
{

    lcd_setCursor(4, 1);
    if (theFsm.state == &normalOperation)
    {
        struct time_t currentTime = scheduler_time();
        fprintf(lcdout, "%02d:%02d:%02d", currentTime.hour, currentTime.minute, currentTime.second);
    }
    else
    {
        fprintf(lcdout, "%02d:%02d", (theFsm.timeSet).hour, (theFsm.timeSet).minute);
    }
}

/* Task Creation **************************************************/
static taskDescriptor timeoutDispatch_task = {.task = &timeOutDispatch, .param = NULL, .period = 0, .execute = 0, .expire = 5000};
static taskDescriptor flashRedLed_task = {.task = &flashRedLed, .param = NULL, .period = 250, .execute = 1, .expire = 0};
static taskDescriptor timeDisplayDispatch_task = {.task = &LCD_Display, .param = NULL, .period = 500, .execute = 1, .expire = 0};
static taskDescriptor greenLED_task = {.task = &greenLedFlash, .param = NULL, .period = 1000, .execute = 1, .expire = 0};
static taskDescriptor alarmActivate_task = {.task = &alarmActivate, .param = NULL, .period = 1000, .execute = 1, .expire = 0};

/* Functions to add tasks to scheduler (e.g for callbacks) **************************************************/
void timeDisplayDispatchTaskCreator()
{
    scheduler_add(&timeDisplayDispatch_task);
}
void timeOutDispatchTaskCreator()
{
    scheduler_add(&timeoutDispatch_task);
}
void flashRedLedTaskCreator()
{
    scheduler_add(&flashRedLed_task);
}

/* FSM States **************************************************/

fsmReturnStatus setTimeHour(Fsm *fsm, const Event *event)
{
    switch (event->signal)
    {
    case ENTRY:
        initial_Display();
        return RET_HANDLED;
    case JOYSTICK_PRESSED:
        lcd_clear();
        timeDisplayDispatchTaskCreator();
        return TRANSITION(setTimeMin);
    case ROTARY_PRESSED:
        lcd_clear();
        timeDisplayDispatchTaskCreator();
        ((theFsm.timeSet).hour)++;
        ((theFsm.timeSet).hour) %= 24; //24-hour mode, reset at 24 to 0
        return RET_HANDLED;
    case EXIT:
        lcd_clear();
        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
}

fsmReturnStatus setTimeMin(Fsm *fsm, const Event *event)
{
    switch (event->signal)
    {
    case ENTRY:
        scheduler_setTime(scheduler_calculateSystemTime(fsm->timeSet));
        return RET_HANDLED;
    case JOYSTICK_PRESSED: //after time is set
        scheduler_setTime(scheduler_calculateSystemTime(fsm->timeSet));
        (theFsm.timeSet).hour = 0;
        (theFsm.timeSet).minute = 0;
        return TRANSITION(normalOperation);
    case ROTARY_PRESSED:
        ((theFsm.timeSet).minute)++;
        ((theFsm.timeSet).minute) %= 60;
        return RET_HANDLED;
    case EXIT:
        lcd_clear();
        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
}

fsmReturnStatus normalOperation(Fsm *fsm, const Event *event)
{
    switch (event->signal)
    {
    case ENTRY:
        scheduler_add(&greenLED_task);
        timeDisplayDispatchTaskCreator();
        return RET_HANDLED;
    case JOYSTICK_PRESSED:
        return TRANSITION(setHourAlarm);
    case ROTARY_PRESSED:
        fsm->isAlarmEnabled = !(fsm->isAlarmEnabled);
        if (fsm->isAlarmEnabled)
        {
            led_yellowOn();
            scheduler_add(&alarmActivate_task);
        }
        else
        {
            led_yellowOff();
            scheduler_remove(&alarmActivate_task);
        }
        return RET_HANDLED;
    case ALARM_ACTIVATED:
        if (fsm->isAlarmEnabled == true)
        {
            return TRANSITION(alarmOn);
        }
    case EXIT:
        lcd_clear();
        scheduler_remove(&greenLED_task);
        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
}
fsmReturnStatus setHourAlarm(Fsm *fsm, const Event *event)
{
    switch (event->signal)
    {
    case JOYSTICK_PRESSED:
        return TRANSITION(setMinAlarm);
    case ROTARY_PRESSED:
        (fsm->timeSet.hour)++;
        (fsm->timeSet.hour) %= 24;
        return RET_HANDLED;
    case EXIT:
        lcd_clear();
        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
}
fsmReturnStatus setMinAlarm(Fsm *fsm, const Event *event)
{
    switch (event->signal)
    {
    case JOYSTICK_PRESSED:

        alarmClock_setTime(scheduler_calculateSystemTime(fsm->timeSet));
        return TRANSITION(normalOperation);
    case ROTARY_PRESSED:
        (fsm->timeSet).minute++;
        (fsm->timeSet).minute %= 60;
        return RET_HANDLED;
    case EXIT:
        lcd_clear();
        return RET_HANDLED;
    default:
        return RET_IGNORED;
    }
}

fsmReturnStatus alarmOn(Fsm *fsm, const Event *event)
{
    switch (event->signal)
    {
    case ENTRY:
        flashRedLedTaskCreator();
        return RET_HANDLED;
    case EXIT:
        lcd_clear();
        led_redOff();
        return RET_HANDLED;
    case JOYSTICK_PRESSED:
        return TRANSITION(normalOperation);
    case ROTARY_PRESSED:
        return TRANSITION(normalOperation);
    case TIMEOUT:
        return TRANSITION(normalOperation);
    default:
        return RET_IGNORED;
    }
}