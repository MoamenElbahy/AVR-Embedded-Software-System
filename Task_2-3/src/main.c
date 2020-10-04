#include <util/delay.h>
#include "ses_lcd.h"
#include "ses_uart.h"
#include <avr/io.h>

#define BAUDRATE 57600
#define Delay_time 500

int main(void){
   uart_init(BAUDRATE); //UART_Init(const unsigned int baud_rate);
   lcd_init();
   stdout = lcdout;
   while (1)
   {
      _delay_ms(Delay_time);
      fprintf(uartout, "START\n");
      printf("START");
   }
   return 0;
}