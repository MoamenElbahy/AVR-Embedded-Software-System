#include <stdint.h>
#include "ses_timer.c"
#include "ses_led.h"
#include "ses_uart.h"
#include <avr/io.h>

#define BAUDRATE 57600
static int count = 1;
static int c2 = 0;

void softwareTimer() 
// to modify the timer interval from 1ms to 1s
{
  if (count <= 1000)
  {
    if (count == 1000)
    {
      led_yellowToggle();
      fprintf(uartout, "the time now is %d \n", c2);
      c2++;

      count = 1;
    }
    count++;
  }
}

int main(void)
{
  led_yellowInit();
  uart_init(BAUDRATE);
  pTimerCallback timer_ptr;
  timer_ptr = &softwareTimer;
  timer2_setCallback(timer_ptr);
  sei();
  timer2_start();
  while (1)
  {
  }

  return 0;
}