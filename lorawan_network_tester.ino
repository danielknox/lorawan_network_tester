#include "screen.h"
#include "joystick.h"
#include "menu.h"
#include "state.h"
#include "gps.h"
#include "lorawan.h"
#include "settings.h"
#include "wiring_private.h" // pinPeripheral() function

void setup() {
  pinPeripheral(22, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);
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
