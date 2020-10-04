#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ses_led.h"
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
   /*
    for red select x=0;
        yellow x =1;
        green x =2;
    */
   uint8_t x = 0;
   led_Init(x);
   led_On(x);
   _delay_ms(1000);
   led_Toggle(x);
   _delay_ms(1000);
   led_On(x);
   _delay_ms(1000);
   led_Off(x);
   _delay_ms(1000);
   x = 1;
   led_Init(x);
   led_On(x);
   _delay_ms(1000);
   led_Toggle(x);
   _delay_ms(1000);
   led_On(x);
   _delay_ms(1000);
   led_Off(x);
   _delay_ms(1000);
   x = 2;
   led_Init(x);
   led_On(x);
   _delay_ms(1000);
   led_Toggle(x);
   _delay_ms(1000);
   led_On(x);
   _delay_ms(1000);
   led_Off(x);
   _delay_ms(1000);
}