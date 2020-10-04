/* INCLUDES ******************************************************************/
#include "ses_common.h"
#include "ses_led.h"
#include "ses_timer.h"

typedef void (*pButtonCallback)(void);

#define ROTARY_ENCODER_PORT PORTB
#define ROTARY_ENCODER_PIN 6

#define JOY_STICK_PORT PORTB
#define JOY_STICK_PIN 7

#define BUTTON_NUM_DEBOUNCE_CHECKS 5
#define BUTTON_DEBOUNCE_PERIOD 30
#define BUTTON_DEBOUNCE_POS_JOYSTICK 0x01
#define BUTTON_DEBOUNCE_POS_ROTARY 0x02

volatile static pButtonCallback pRotaryButtonCallback;
volatile static pButtonCallback pJoyStickCallback;

/** 
 * Get the state of the joystick button.
 * return true if the joystick button is pressed.
 */
bool button_isJoystickPressed(void)
{
   return ((PIN_REGISTER(JOY_STICK_PORT) & (1 << JOY_STICK_PIN)) == 0);
}
/** 
 * Get the state of the rotary button.
 * return true if the rotary button is pressed.
 */
bool button_isRotaryPressed(void)
{
   return ((PIN_REGISTER(ROTARY_ENCODER_PORT) & (1 << ROTARY_ENCODER_PIN)) == 0);
}

void button_setRotaryButtonCallback(pButtonCallback callback)
{
   pRotaryButtonCallback = callback;
}

void button_setJoystickButtonCallback(pButtonCallback callback)
{
   pJoyStickCallback = callback;
}

void button_checkState(void)
{
   static uint8_t state[BUTTON_NUM_DEBOUNCE_CHECKS] = {}; //set the number of required debounces
   static uint8_t index = 0;
   static uint8_t debouncedState = 0;
   uint8_t lastDebouncedState = debouncedState; //saving the last debounced state
   // each bit in every state byte represents one button
   state[index] = 0;
   if (button_isJoystickPressed())
   {
      state[index] |= (1 << BUTTON_DEBOUNCE_POS_JOYSTICK);
   }
   if (button_isRotaryPressed())
   {
      state[index] |= (1 << BUTTON_DEBOUNCE_POS_ROTARY);
   }
   index++;
   if (index == BUTTON_NUM_DEBOUNCE_CHECKS)
   {
      index = 0;
   }
   // init compare value and compare with ALL reads, only if
   // we read BUTTON_NUM_DEBOUNCE_CHECKS consistent "1's" in the state
   // array, the button at this position is considered pressed
   uint8_t j = 0xFF;
   for (uint8_t i = 0; i < BUTTON_NUM_DEBOUNCE_CHECKS; i++)
   {
      j = j & state[i];
   }
   debouncedState = j;

   if ((pRotaryButtonCallback != NULL) && ((lastDebouncedState>>BUTTON_DEBOUNCE_POS_ROTARY) & 1) && (debouncedState ^ lastDebouncedState))
   {
      pRotaryButtonCallback(); // compare with the last debounce state
   }
   if ((pJoyStickCallback != NULL) && ((lastDebouncedState>>BUTTON_DEBOUNCE_POS_JOYSTICK) & 1) && (debouncedState ^ lastDebouncedState))
   {
      pJoyStickCallback();
   }
}

/** 
 * button initializer takes boolean argument , true if using the debouncing mode.
 */
void button_init(bool debouncing)
{
   DDR_REGISTER(ROTARY_ENCODER_PORT) &= (~(1 << ROTARY_ENCODER_PIN));
   DDR_REGISTER(JOY_STICK_PORT) &= (~(1 << JOY_STICK_PIN));
   ROTARY_ENCODER_PORT |= (1 << ROTARY_ENCODER_PIN);
   JOY_STICK_PORT |= (1 << JOY_STICK_PIN);

   uint8_t timer1_debounce_period = BUTTON_DEBOUNCE_PERIOD / BUTTON_NUM_DEBOUNCE_CHECKS;

   if (debouncing)
   {
      timer1_setCallback(button_checkState);
      timer1_start(timer1_debounce_period);
   }
   else
   {

      // TODO initialization for direct interrupts (e.g. setting up the PCICR register)

      PCICR |= (1 << PCIE0); /*activate the pin change interrupt, 
                                  PCICR – Pin Change Interrupt Control Register,
                                  Any change on any enabled PCINT7:0 pin will cause an interrupt. */

      PCMSK0 |= ((1 << ROTARY_ENCODER_PIN) | (1 << JOY_STICK_PIN));
   }
}

ISR(PCINT0_vect)
{
   if ((pRotaryButtonCallback != NULL) && (PCMSK0 |= ((1 << ROTARY_ENCODER_PIN))) && (button_isRotaryPressed()))
   {
      pRotaryButtonCallback();
   }
   else if ((pJoyStickCallback != NULL) && (PCMSK0 |= ((1 << JOY_STICK_PIN))) && (button_isJoystickPressed()))
   {
      pJoyStickCallback();
   }
}