#include "screen.h"
#include "joystick.h"
#include "menu.h"
#include "state.h"
#include "gps.h"

void setup() {
  initGPS();  
  initScreen();
  initJoystick();
  setState(&menuState);
}

void loop() {
  spinCurrentState();
}
