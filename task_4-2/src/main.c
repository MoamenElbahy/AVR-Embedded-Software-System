/*INCLUDES ************************************************************/
#include <stdint.h>
#include "ses_led.h"
#include "ses_timer.h"
#include "ses_button.h"
#include "ses_lcd.h"
#include "ses_uart.h"
#include <avr/io.h>
#include "ses_scheduler.h"

/* PRIVATE VARIABLES **************************************************/
static taskDescriptor task_toggleLed;
static taskDescriptor task_debouncing;
static taskDescriptor task_stopwatch;
static taskDescriptor task_yellowToggle;

static uint8_t yellow_flag = 0;
static uint8_t clock_flag = 0;
static uint16_t seconds = 0;
static uint16_t milliseconds = 0;

typedef enum EColorIndicator // selector for the LED toggling
{
  RED,
  GREEN,
  YELLOW
};

/**
 * Initializing all the LEDs in an off state 
 */
void led_init(void)
{
  led_redInit();
  led_yellowInit();
  led_greenInit();
}

/**
 * Setting the color for the LED toggle task, to be called from the scheduler every 2s
 * */
void ledtoggle_scheduler(void *x)
{
  x = *((uint8_t *)x);

  if (x == 0)
  {
    led_redToggle();
  }
  else if (x == 1)
  {
    led_greenToggle();
  }
  else if (x == 2)
  {
    led_yellowToggle();
  }
}

/** Button Callback for the Joystick button
 * with a flag to keep track of the yellow LED state.
 * */
void yellow_toggleCallback()
{
  if (yellow_flag == 0)
  {
    led_yellowOn();
    yellow_flag = 1;
    scheduler_add(&task_yellowToggle);
  }
  else
  {
    led_yellowOff();
    yellow_flag = 0;
    scheduler_remove(&task_yellowToggle);
  }
}

/** Called by the scheduler after 5s
 * switches the LED off only if the user has not pressed the rotary button again
 * */
void yellow_scheduler()
{
  if (yellow_flag == 1)
  {
    led_yellowOff();
    yellow_flag = 0;
  }
}

/** Called by the scheduler every 0.1s
 * displays stopwatch time in seconds:milliseconds format
 * */
void stopwatch_scheduler(void)
{
  if ((milliseconds + 1) == 10)
  {
    seconds++;
    milliseconds = 0;
  }
  else
    milliseconds++;
  lcd_clear();
  lcd_setCursor(1, 1);
  fprintf(lcdout, "Time is %d:%d s\n", seconds, milliseconds);
}

/** Rotary button callback function
 * Keeps track of stopwatch state (not yet started/started)
 * Starts the stopwatch with the first press, and stops with the second press
 * */
void stopwatch_rotaryCallback()
{
  if (clock_flag == 0)
  {
    scheduler_add(&task_stopwatch);
    clock_flag = 1;
  }
  else
  {
    scheduler_remove(&task_stopwatch);
    clock_flag = 0;
  }
}

/** Task creation and parameter setting *********************************************************/
void toggleled_taskCreator(uint8_t selectColor)
{
  task_toggleLed.task = &ledtoggle_scheduler;
  task_toggleLed.param = (void *)&selectColor;
  task_toggleLed.expire = 2000;
  task_toggleLed.period = 2000;
  task_toggleLed.execute = 0;
  scheduler_add(&task_toggleLed);
}

void debouncing_taskCreator()
{
  task_debouncing.task = &button_checkState;
  task_debouncing.param = NULL;
  task_debouncing.expire = 5;
  task_debouncing.period = 5;
  task_debouncing.execute = 0;
  scheduler_add(&task_debouncing);
}

void yellowtoggle_taskCreator()
{
  task_yellowToggle.task = &yellow_scheduler;
  task_yellowToggle.param = NULL;
  task_yellowToggle.expire = 5000;
  task_yellowToggle.period = 0;
  task_yellowToggle.execute = 0;
}

void stopwatch_taskCreator()
{
  task_stopwatch.task = &stopwatch_scheduler;
  task_stopwatch.param = NULL;
  task_stopwatch.expire = 0;
  task_stopwatch.period = 100;
  task_stopwatch.execute = 0;
}

int main(void)
{
  //Initializations
  lcd_init();
  led_init();
  button_init(true);

  //Button Callbacks
  pButtonCallback pYellowToggleCallback = &yellow_toggleCallback;
  pButtonCallback pStopwatchCallback = &stopwatch_rotaryCallback;
  button_setJoystickButtonCallback(pYellowToggleCallback);
  button_setRotaryButtonCallback(pStopwatchCallback);

  //Task Creation
  toggleled_taskCreator(GREEN);
  debouncing_taskCreator();
  yellowtoggle_taskCreator();
  stopwatch_taskCreator();

  //Starting the scheduler
  scheduler_init();
  scheduler_run();

  return 0;
}
