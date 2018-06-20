#include "joystick.h"

#include <Arduino.h>
#include "hardware.h"

AxisJoystick joystick(JOYSTICK_FIRE_PIN, JOYSTICK_X_PIN, JOYSTICK_Y_PIN);

void initJoystick() {
    pinMode(JOYSTICK_FIRE_PIN, INPUT_PULLUP); 
}

joyState readJoystick() {
  return joystick.singleRead();
}

