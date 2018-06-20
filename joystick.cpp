#include "joystick.h"

#include <Arduino.h>

#define SW_PIN  5
#define VRX_PIN A0
#define VRY_PIN A1

AxisJoystick joystick(SW_PIN, VRX_PIN, VRY_PIN);

void initJoystick() {
    pinMode(SW_PIN, INPUT_PULLUP); 
}

joyState readJoystick() {
  return joystick.singleRead();
}

