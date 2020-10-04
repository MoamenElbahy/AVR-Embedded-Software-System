/*INCLUDES ************************************************************/
#include "ses_timer.h"
#include "ses_scheduler.h"
#include "util/atomic.h"
#include "ses_lcd.h"
#include "ses_uart.h"
#include "ses_button.h"
#include "ses_led.h"



int main(void)
{
  led_redInit();
  led_yellowInit();
  button_init(true);

  pButtonCallback pledRedToggle = &led_redToggle;
  pButtonCallback pledYellowToggle = &led_yellowToggle;
  button_setJoystickButtonCallback(pledRedToggle);
  button_setRotaryButtonCallback(pledYellowToggle);

  sei();                         // enable interrupts globally

  while(1){}

 return 0;
}

