#include "screen.h"
#include "joystick.h"
#include "menu.h"
#include "state.h"
#include "gps.h"
#include "lorawan.h"
#include "settings.h"

void setup() {
  Serial.begin(115200);
  initGPS();  
  initLorawan();
  initScreen();
  initJoystick();
  loadSettings();
  setState(&menuState);
}

void loop() {
  spinCurrentState();
}
