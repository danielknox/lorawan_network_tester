#include "screen.h"
#include "joystick.h"
#include "menu.h"
#include "state.h"

void setup() {
  initScreen();
  initJoystick();
  setState(&menuState);
}

void loop() {
  spinCurrentState();
}
