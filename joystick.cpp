#include "joystick.h"

#include <Arduino.h>
#include "hardware.h"

AxisJoystick joystick(JOYSTICK_FIRE_PIN, JOYSTICK_X_PIN, JOYSTICK_Y_PIN);

/**************************************************************************/
/*!
    @brief  Setting up for joystick controls
*/
/**************************************************************************/
void initJoystick() {
    pinMode(JOYSTICK_FIRE_PIN, INPUT_PULLUP); 
}

/**************************************************************************/
/*!
    @brief  Read the current state from the joystick
*/
/**************************************************************************/
joyState readJoystick() {
  return joystick.singleRead();
}

