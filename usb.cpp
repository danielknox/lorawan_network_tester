#include "usb.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "lorawan.h"
#include "SerialCommand.h"
#include "menu.h"

SerialCommand sCmd;

/**************************************************************************/
/*!
    @brief  Called by SerialCommand to set the OTAA keys.
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  Called by SerialCommand to set the ABP keys.
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  Called by SerialCommand upon a request for the hardware EUI.
*/
/**************************************************************************/
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

/**************************************************************************/
/*!
    @brief  called by SerialCommand to exit AT/USB/Serial mode
*/
/**************************************************************************/
void goToMenu() {
  Serial.println("Exiting");
  setState(&menuState);
}

/**************************************************************************/
/*!
    @brief  Sets up for AT/USB/Serial mode 
*/
/**************************************************************************/
void enterUSBMode() {
  drawFullScreenIcon(usbConnector);
  
  sCmd.addCommand("!AT+CFGOTAA", setOtaaKeys);    
  sCmd.addCommand("!AT+CFGABP", setAbpKeys);
  sCmd.addCommand("!AT+HWEUI?", getHweui);
  sCmd.addCommand("!AT+EXIT", goToMenu);
  
  Serial.println("USB MODE");
}

/**************************************************************************/
/*!
    @brief  Checks for commands on the serial line
*/
/**************************************************************************/
void usbSpin() {
  sCmd.readSerial();
}

state usbState = {
  &enterUSBMode,
  &usbSpin,
  NULL
};
