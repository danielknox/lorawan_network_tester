#include "usb.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "lorawan.h"
#include "SerialCommand.h"

SerialCommand sCmd;

void setOtaaKeys(){
  char *arg1;
  char *arg2;
  arg1 = sCmd.next();
  if (arg1 == NULL || !(strlen(arg1) == 16)) {
    Serial.println("INVALID APPEUI");
    return;
  }
  arg2 = sCmd.next();
  if (arg2 == NULL || !(strlen(arg2) == 32)) {
    Serial.println("INVALID APPKEY");
    return;
  }

  if(provisionOTAA(arg1,arg2)){
      Serial.println("OK");
      return;
  } else {
    Serial.println("RN2483 FAILURE");
  }
  return;
}

void setAbpKeys(){
  char *arg1;
  char *arg2;
  char *arg3;
  arg1 = sCmd.next();
  if (arg1 == NULL || !(strlen(arg1) == 8)) {
    Serial.println("INVALID DEVADDR");
    return;
  }
  arg2 = sCmd.next();
  if (arg2 == NULL || !(strlen(arg2) == 32)) {
    Serial.println("INVALID NWKSKEY");
    return;
  }
  arg3 = sCmd.next();
  if (arg3 == NULL || !(strlen(arg3) == 32)) {
    Serial.println("INVALID APPSKEY");
    return;
  }

  if(provisionABP(arg1,arg2,arg3)){
      Serial.println("OK");
      return;
  } else {
    Serial.println("RN2483 FAILURE");
  }
  
  return;
}

void getHweui(){
  char buffer[16];
  if(getHweui(buffer, 16)>0){
    Serial.println("OK");
    Serial.print("Dev EUI: ");
    Serial.println(buffer);
    return;
  }
  Serial.println("RN2483 FAILURE");
  return;
}

void initUSB(){
  sCmd.addCommand("!AT+CFGOTAA", setOtaaKeys);    
  sCmd.addCommand("!AT+CFGABP", setAbpKeys);
  sCmd.addCommand("!AT+HWEUI?", getHweui);
}

void enterUSBMode() {
  drawFullScreenIcon(usbConnector);
  initUSB();
  Serial.println("USB MODE");
}

void usbSpin() {
  sCmd.readSerial();
}

state usbState = {
  &enterUSBMode,
  &usbSpin,
  NULL
};
