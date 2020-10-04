#ifndef SES_BUTTON_H_
#define SES_BUTTON_H_

#include <avr/interrupt.h>

/* INCLUDES ******************************************************************/

#include "ses_common.h"
#include "ses_led.h"

typedef void (*pButtonCallback)(void);

/* FUNCTION PROTOTYPES *******************************************************/
/**
 * Initializes rotary encoder and joystick button
 */
void button_init(bool);

/** 
 * Get the state of the joystick button.
 */
bool button_isJoystickPressed(void);

/** 
 * Get the state of the rotary button.
 */
bool button_isRotaryPressed(void);

void button_setRotaryButtonCallback(pButtonCallback callback);
void button_setJoystickButtonCallback(pButtonCallback callback);

void button_checkState(void);

#endif /* SES_BUTTON_H_ */