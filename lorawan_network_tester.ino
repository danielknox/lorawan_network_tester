#include "screen.h"
#include "joystick.h"
#include "menu.h"
#include "state.h"
#include "gps.h"
#include "lorawan.h"

void setup() {
  Serial.begin(115200);
  initGPS();  
  initLorawan();
  initScreen();
  initJoystick();
  setState(&menuState);
}

void loop() {
  spinCurrentState();
}
