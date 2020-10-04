#include "ses_button.h"
#include "ses_led.h"
#include <util/delay.h>
#include "ses_lcd.h"
#include "ses_uart.h"
#include <avr/io.h>

void Light_Red_led(void)
{

   if (!button_isRotaryPressed())
      led_redOff();
   else
   {
      while (button_isRotaryPressed())
         led_redOn();
         _delay_ms(3000);
         led_redOff();
   }
}

void Light_Green_led(void)
{

   if (!button_isJoystickPressed())
      led_greenOff();
   else
   {
      while (button_isJoystickPressed())
         led_greenOn();
         _delay_ms(3000);
         led_greenOff();
   }
}

int main(void)
{  uart_init(57600);

   bool debouncing = false;
  button_init(debouncing);
   led_redInit();
   led_greenInit();
   _delay_ms(3000);
   Light_Red_led();
   Light_Green_led();

      int x = 0;
   while (1) {
      fprintf(uartout, "Time now is: %d seconds \n", x);
      _delay_ms(1000);
      x += 1;
}
}
