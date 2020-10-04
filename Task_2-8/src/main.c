#include "ses_lcd.h"
#include "ses_uart.h"
#include "ses_adc.h"
#include "ses_led.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define BAUDRATE 57600
#define Delay_time 2500

int main(void)
{

   int16_t light_Value = 0;
   int16_t Temp_Value = 0;

   uart_init(BAUDRATE);
   adc_init();
   stdout = lcdout;
   led_redInit();    // initiate red
   led_yellowInit(); // initiate yellow
   led_greenInit();  // initiate green

   while (1)
   {
      _delay_ms(Delay_time);

      led_redToggle();    //toggle red
      led_yellowToggle(); //toggle yellow
      led_greenToggle();  //toggle green

      light_Value = adc_read(ADC_LIGHT_CH);
      Temp_Value = adc_getTemperature();

      fprintf(uartout, "Temperature readings = %d\nlight readings = %u\n", adc_getTemperature(), adc_read(ADC_LIGHT_CH));
      printf("Temperature = %d\nlight value = %u\n", adc_getTemperature(), adc_read(ADC_LIGHT_CH));
   }
}
