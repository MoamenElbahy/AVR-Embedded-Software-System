/*INCLUDES *******************************************************************/
#include <inttypes.h>
#include <avr/io.h>
#include "ses_common.h"

/* DEFINES & MACROS **********************************************************/

#define ADC_INVALID_CHANNEL 0xFFFF

#define MIC_PORT PORTF
#define MIC_PIN1 0
#define MIC_PIN2 1

#define TEMP_PORT PORTF
#define TEMP_PIN 2

#define LIGHT_PORT PORTF
#define LIGHT_PIN 4

#define JOY_STICK_PORT PORTF
#define JOY_STICK_PIN 5

#define VREF 1.6
#define ADC_VREF_SRC (1 << REFS1) | (1 << REFS0)

#define ADC_PRESCALE (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) //the ADC prescaler in the macro

#define ADC_TEMP_MAX 40
#define ADC_TEMP_MIN 20
#define ADC_TEMP_FACTOR 10
#define ADC_TEMP_RAW_MAX 256.77
#define ADC_TEMP_RAW_MIN 481.46

/* to signal that the given channel was invalid */

enum ADCChannels
{
  ADC_MIC_NEG_CH = 0, /* ADC0 */
  ADC_MIC_POS_CH,     /* ADC1 */
  ADC_TEMP_CH,        /* ADC2 */
  ADC_RESERVED1_CH,   /* ADC3 */
  ADC_LIGHT_CH,       /* ADC4 */
  ADC_JOYSTICK_CH,    /* ADC5 */
  ADC_RESERVED2_CH,   /* ADC6 */
  ADC_RESERVED3_CH,   /* ADC7 */
  ADC_NUM             /* number of ADC channels*/
};

enum JoystickDirections
{
  RIGHT = 0,
  UP,
  LEFT,
  DOWN,
  NO_DIRECTION
};

/* FUNCTION PROTOTYPES *******************************************************/

/**
 * Initializes the ADC
 */
void adc_init(void)
{
  DDR_REGISTER(MIC_PORT) &= ~((1 << MIC_PIN1) | (1 << MIC_PIN2));
  MIC_PORT &= ~((1 << MIC_PIN1) | (1 << MIC_PIN2)); //deactivate their internal pull-up resistors

  DDR_REGISTER(TEMP_PORT) &= (~(1 << TEMP_PIN));
  TEMP_PORT &= (~(1 << TEMP_PIN)); //deactivate the temperture port internal pull-up resistors

  DDR_REGISTER(LIGHT_PORT) &= (~(1 << LIGHT_PIN));
  LIGHT_PORT &= (~(1 << LIGHT_PIN)); //deactivate the light port internal pull-up resistors

  DDR_REGISTER(JOY_STICK_PORT) &= (~(1 << JOY_STICK_PIN));
  JOY_STICK_PORT &= (~(1 << JOY_STICK_PIN)); //deactivate their internal pull-up resistors

  PRR0 &= (~PRADC); //Disable power reduction mode for the ADC module

  ADMUX |= ADC_VREF_SRC; //set reference voltage, the macro ADC_VREF_SRC
  ADMUX &= (~(1 << ADLAR)); //set the ADC result right adjusted

  ADCSRA |= (ADC_PRESCALE | (1 << ADEN)); //ADC Control and Status Register A
  ADCSRA &= (~(1 << ADATE));              //prevent auto triggering
}
/**
 * Read the raw ADC value of the given channel
 * @adc_channel The channel as element of the ADCChannels enum
 * @return The raw ADC value
 */
uint16_t adc_read(uint8_t adc_channel)
{
  if (adc_channel >= ADC_NUM)
  {
    return ADC_INVALID_CHANNEL;
  }
  else
  {
    ADMUX |= adc_channel;
    ADCSRA |= (1 << ADSC);
    return ADC;
  }
}
/**
 * Read the current joystick direction
 * @return The direction as element of the JoystickDirections enum
 
 * Read the current temperature
 * @return Temperature in tenths of degree celsius
 */
int16_t adc_getTemperature()
{
  int16_t adc = adc_read(ADC_TEMP_CH);
  int16_t slope = ADC_TEMP_FACTOR * ((ADC_TEMP_MAX - ADC_TEMP_MIN) / (ADC_TEMP_RAW_MAX - ADC_TEMP_RAW_MIN));
  int16_t offset = ADC_TEMP_FACTOR * (ADC_TEMP_MAX - (ADC_TEMP_RAW_MAX * slope));
  return (adc * slope + offset) / ADC_TEMP_FACTOR;
}