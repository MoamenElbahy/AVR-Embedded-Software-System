/*INCLUDES ************************************************************/
#include "util/atomic.h"
#include "ses_pwm.h"
#include "ses_common.h"
#include "avr/interrupt.h"
#include "ses_led.h"
#include "ses_lcd.h"
#include "util/atomic.h"
#include "ses_timer.h"
#include "ses_uart.h"

/* DEFINES & MACROS **********************************************************/
#define ARRAYSIZE 9

/* PRIVATE VARIABLES **************************************************/
static volatile uint16_t peaksPerTimerInterrupt = 0;
static uint16_t frequencyArray[ARRAYSIZE] = {};
static uint8_t arrayCounter = 0;
static uint16_t recentFrequency = 0;

/*FUNCTION DEFINITION ************************************************/
void motorFrequency_init()
{
    // Setting the port direction register to output and enabling it at the start.
    DDR_REGISTER(PORTD) &= ~(1 << PORTD0);
    PORTD |= (1 << PORTD0); // activate pull-up resistor

    // Clearing the external interrupt mask register.
    EIMSK &= ~(1 << INT0);
    // Enabling rising-edge interrupts
    EICRA |= ((1 << ISC00) | (1 << ISC01));
    // Clear pending interrupts
    EIFR |= (1 << INTF0);
    // Clearing the external interrupt mask register.
    EIMSK |= (1 << INT0);

    //initialize the motor frequency status LEDs
    led_yellowInit();
    led_greenInit();

    //start the timer to take frequency readings
    timer5_start();
}

uint16_t motorFrequency_getRecent()
{
    uint16_t currentPeaks = 0;
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        currentPeaks = peaksPerTimerInterrupt;  //make a copy of the number of peaks
        peaksPerTimerInterrupt = 0; // reset the counted peaks for the next calculation
    }

   recentFrequency = (currentPeaks * MODIFIER * 60) / 6; //calculate in RPM

    //if motor stopped
    if (recentFrequency == 0)
    {
        led_greenOn();
        led_yellowOff();
    }
    else //motor running
    {
        led_greenOff();
    }

    if (arrayCounter >= ARRAYSIZE) //if the end of the arrays is reached
    {
        arrayCounter = 0; //reset the index
    }
    frequencyArray[arrayCounter] = recentFrequency; //fill the array with the frequancy reading
    arrayCounter++;

    return recentFrequency; 
}

uint16_t motorFrequency_getMedian()
{
    uint16_t median;
    uint16_t copyArray[ARRAYSIZE] = {};

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        for (uint8_t i = 0; i < ARRAYSIZE; ++i)
        {
            copyArray[i] = frequencyArray[i]; //make a copy of the frequancy array
        }
    }

    for (uint8_t i = 0; i < ARRAYSIZE; ++i)  //sort the elements in a ascending order
    {

        for (uint8_t j = i + 1; j < ARRAYSIZE; ++j)
        {

            if (copyArray[i] > copyArray[j])
            {

                uint16_t a = copyArray[i]; // a holds the larger value
                copyArray[i] = copyArray[j];
                copyArray[j] = a;
            }
        }
    }
    if (ARRAYSIZE % 2 == 0) //check if the number of the elements inside the array in odd or even
    {
        //if even take the average of the two middle elements
        median = (copyArray[ARRAYSIZE / 2] + copyArray[((ARRAYSIZE / 2) - 1)]) / 2; 
    }
    else
    {
        //if odd take the middle elements
        median = copyArray[ARRAYSIZE / 2];
    }
    return median;
}

void frequency_displayer() //display the frequancy ond the median on the LCD
{
    lcd_clear();
    
    lcd_setCursor(0, 0);
    fprintf(lcdout, "Current RPM %d", recentFrequency);

    lcd_setCursor(0, 2);
    fprintf(lcdout, "Current Median %d", motorFrequency_getMedian());
}

ISR(INT0_vect)
{
    led_yellowToggle();
    peaksPerTimerInterrupt++;
}