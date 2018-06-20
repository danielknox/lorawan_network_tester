#include "usb.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "lorawan.h"

// Cmd Sentence buffer and associated pointers
// static const uint8_t _bufferLen = 83; // 82 + NULL
// char _buffer[_bufferLen];
  uint8_t _bufferLen;
  char* _buffer;
  char *_ptr;
  
void enterUSBMode() {
  drawFullScreenIcon(usbConnector);
}

bool setOtaaKeys(){
  char appEuiBuffer[16];
  char appKeyBuffer[32];
  byte appEuibytesRead;
  byte appKeyBytesRead;
  appEuibytesRead = Serial.readBytesUntil(' ', appEuiBuffer, sizeof(appEuiBuffer)); 
  if(appEuibytesRead == 16){
      appKeyBytesRead = Serial.readBytesUntil('\r', appEuiBuffer, sizeof(appEuiBuffer)); 
      if(appKeyBytesRead == 32){
        return provisionOTAA(appEuiBuffer,appKeyBuffer);
      }
  }
  return false;
}

bool setAbpKeys(){
  char devAddrBuffer[8];
  char nwkSKeyBuffer[32];
  char appSKeyBuffer[32];
  byte devAddrBytesRead;
  byte nwkSKeyBytesRead;
  byte appSKeyBytesRead;
  
  devAddrBytesRead = Serial.readBytesUntil(' ', devAddrBuffer, sizeof(devAddrBuffer)); 
  if(devAddrBytesRead == 8){
      nwkSKeyBytesRead = Serial.readBytesUntil('\r', nwkSKeyBuffer, sizeof(nwkSKeyBuffer)); 
      if(nwkSKeyBytesRead == 32){
        appSKeyBytesRead = Serial.readBytesUntil('\r', appSKeyBuffer, sizeof(appSKeyBuffer)); 
        if(appSKeyBytesRead == 32){
           return provisionABP(devAddrBuffer,nwkSKeyBuffer,appSKeyBuffer);
        }
      }
  }
  return false;
}

void usbSpin() {
  if(Serial.available()){
    char cmdBuffer[10];
    byte bytesRead;
    bytesRead = Serial.readBytesUntil(' ', cmdBuffer, sizeof(cmdBuffer));
    if(bytesRead!=0){
      bool validCommand;
      // Possible valid command
      if  (strcmp("AT+CFGOTAA",cmdBuffer) == 0){
        validCommand = setOtaaKeys();
      } else if (strcmp("AT+CFGABP",cmdBuffer) == 0){
        validCommand = setAbpKeys();
      }
      if(validCommand){
        Serial.println("OK");
      }
    }  
  }
}

state usbState = {
  &enterUSBMode,
  &usbSpin,
  NULL
};
