#include "usb.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "lorawan.h"
#include <SerialCommand.h>

SerialCommand sCmd;

void setOtaaKeys(){
  char *arg1;
  char *arg2;
  arg1 = sCmd.next();
  if (arg1 == NULL) {
    return;
  }
  arg2 = sCmd.next();
  if (arg2 == NULL) {
    return;
  }

  if(sizeof(arg1) == 16 && sizeof(arg2) == 32){
    if(provisionOTAA(arg1,arg2)){
      Serial.println("OK");
      return;
    }
  }
  return;
}

void setAbpKeys(){
  char *arg1;
  char *arg2;
  char *arg3;
  arg1 = sCmd.next();
  if (arg1 == NULL) {
    return;
  }
  arg2 = sCmd.next();
  if (arg2 == NULL) {
    return;
  }
  arg3 = sCmd.next();
  if (arg3 == NULL) {
    return;
  }

  if(sizeof(arg1) == 8 && sizeof(arg2) == 32 && sizeof(arg3) == 32){
    if(provisionABP(arg1,arg2,arg3)){
      Serial.println("OK");
      return;
    }
  }
  return;
}

void initUSB(){
  sCmd.addCommand("!AT+CFGOTAA", setOtaaKeys);    
  sCmd.addCommand("!AT+CFGABP",setAbpKeys);
}

void enterUSBMode() {
  drawFullScreenIcon(usbConnector);
  initUSB();
}

void usbSpin() {
  sCmd.readSerial();
}

state usbState = {
  &enterUSBMode,
  &usbSpin,
  NULL
};
