#include <stdint.h>
#include <avr/io.h>

void wait (void) {

    uint16_t i;
        for (i = 0xC80; i > 0 ; i--)  {
            // prevent code optimization by using inline assembler
            asm volatile ( "nop" ); // one cycle with no operation
        }
}

     int main(void) {

	while (1) {
		wait ();
	}
}