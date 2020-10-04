#include <stdint.h>
#include <avr/io.h>
#define processor_freq 16000  //clock per millisecond
#define inst_num_1 9     //number of instructions

void wait (uint16_t millis) {

    uint32_t delay =  millis*processor_freq /inst_num_1;

        for (uint32_t clks = 0; clks < delay ; clks++) {
            asm volatile ( "nop" ); 
     }
}
int main(void) {

  DDRG |= 0x02;
	while (1) {
		wait (180);
		PORTG ^= 0x02;
	}
	return 0;
}