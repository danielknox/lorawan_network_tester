#include "usb.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"

void enterUSBMode() {
  drawFullScreenIcon(usbConnector);
}

void usbSpin() {
  if(Serial.available()) {
    char c = Serial.read();
    //do a thing...
  }
}

state usbState = {
  &enterUSBMode,
  &usbSpin,
  NULL
};
