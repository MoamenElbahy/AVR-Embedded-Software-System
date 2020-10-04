#ifndef SES_ALARMCLOCK_H_
#define SES_ALARMCLOCK_H_

/*INCLUDES *******************************************************************/
#include "ses_led.h"
#include "ses_scheduler.h"

/* DEFINES & MACROS **********************************************************/
#define TRANSITION(newState) (fsm->state = newState, RET_TRANSITION)

/** typedef for state event handler functions */
typedef uint8_t fsmReturnStatus; //< typedef to be used with above enum
typedef struct fsm_s Fsm;        //< typedef for alarm clock state machine
typedef struct event_s Event;    //< event type for alarm clock fsm
typedef fsmReturnStatus (*State)(Fsm *, const Event *);

/** return values */
struct time_t
{
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint16_t milli;
};

/**type of time-keeping variable */
typedef uint32_t systemTime_t;

enum
{
    RET_HANDLED,   //< event was handled
    RET_IGNORED,   //< event was ignored; not used in this implementation
    RET_TRANSITION //< event was handled and a state transition occurred
};

/** possible events */
enum
{
    ENTRY,
    EXIT,
    JOYSTICK_PRESSED,
    ROTARY_PRESSED,
    ALARM_ACTIVATED,
    TIMEOUT //5 secound passed and no action
};

struct fsm_s
{
    State state;           //< current state, pointer to event handler
    bool isAlarmEnabled;   //< flag for the alarm status
    struct time_t timeSet; //< multi-purpose var for system time and alarm time
};

struct event_s
{
    uint8_t signal; //< identifies the type of event
};

Fsm theFsm;

/* sets and calls initial state of state machine */
inline static void fsm_init(Fsm *fsm, State init)
{
    Event entryEvent = {.signal = ENTRY};
    fsm->state = init;
    fsm->state(fsm, &entryEvent);
}

/* dispatches events to state machine, called in application*/
inline static void fsm_dispatch(Fsm *fsm, const Event *event)
{
    static Event entryEvent = {.signal = ENTRY};
    static Event exitEvent = {.signal = EXIT};
    State s = fsm->state;                       //store current state in s variable
    fsmReturnStatus r = fsm->state(fsm, event); //store return status of current state with given event
    if (r == RET_TRANSITION)
    {
        s(fsm, &exitEvent);           //< call exit action of last state
        fsm->state(fsm, &entryEvent); //< call entry action of new state
    }
}

/** Setter function for the global variable for 
 * alarm time keeping
 * @param systemTime_t takes time in ms as parameter
 * */
void alarmClock_setTime(systemTime_t);

/**
 * Getter function for the global alarm time keeping variable
 * @return time in ms
 * */
systemTime_t alarmClock_getTime();

/**
 * Adds a task to the scheduler to send a timeout event to the FSM after 5 seconds
 * */
void timeOutDispatchTaskCreator();

/**
 * Adds a task to the scheduler to flash the LED with 4 Hz frequency
 * */
void flashRedLedTaskCreator();

/**
 * Dispatches a joystick-pressed event to the FSM
 * */
void joystickPressedDispatch();

/**
 * Dispatches a rotary-pressed event to the FSM
 * */
void rotaryPressedDispatch();

/**
 * Dispatches an time out event after 5 seconds to the FSM
 * */
void timeOutDispatch();

void greenLedFlash();

/**
 * Dispatches an alarm-activated event, if alarm time equals system time
 * */
void alarmActivate();

/**
 * Function representing state of setting the hours in the alarmclock.
 * @param Fsm * pointer to current fsm
 * @param const Event * pointer to passed event by the dispatch function
 * 
 * @return fsmReturnStatus returns the type of event handling, whether transition, or not, or ignored.
 * */
fsmReturnStatus setTimeHour(Fsm *, const Event *);

/**
 * Function representing state of setting the minutes in the alarmclock.
 * @param Fsm * pointer to current fsm
 * @param const Event * pointer to passed event by the dispatch function
 * 
 * @return fsmReturnStatus returns the type of event handling, whether transition, or not, or ignored.
 * */
fsmReturnStatus setTimeMin(Fsm *, const Event *);

/**
 * Function representing state of normal time display.
 * @param Fsm * pointer to current fsm
 * @param const Event * pointer to passed event by the dispatch function
 * 
 * @return fsmReturnStatus returns the type of event handling, whether transition, or not, or ignored.
 * */
fsmReturnStatus normalOperation(Fsm *, const Event *);

/**
 * Function representing state of setting hours in the alarm time.
 * @param Fsm * pointer to current fsm
 * @param const Event * pointer to passed event by the dispatch function
 * 
 * @return fsmReturnStatus returns the type of event handling, whether transition, or not, or ignored.
 * */
fsmReturnStatus setHourAlarm(Fsm *, const Event *);

/**
 * Function representing state of setting minutes in the alarm time.
 * @param Fsm * pointer to current fsm
 * @param const Event * pointer to passed event by the dispatch function
 * 
 * @return fsmReturnStatus returns the type of event handling, whether transition, or not, or ignored.
 * */
fsmReturnStatus setMinAlarm(Fsm *, const Event *);

/**
 * Function representing the state of the alarm when activated.
 * @param Fsm * pointer to current fsm
 * @param const Event * pointer to passed event by the dispatch function
 * 
 * @return fsmReturnStatus returns the type of event handling, whether transition, or not, or ignored.
 * */
fsmReturnStatus alarmOn(Fsm *, const Event *);

#endif /* SES_ALARMCLOCK_H_ */