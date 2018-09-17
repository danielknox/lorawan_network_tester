/* 
 *  Author: Keith Greenhow and Daniel Knox
 *  
 *  Decoder function for TTN:
  function Decoder(bytes, port) {
    // Decode an uplink message from a buffer
    // (array) of bytes to an object of fields.
    var decoded = {};

    if (port === 1){
      decoded = { 
        trigger_button:  !!(bytes[0] &  1) ,
        gps:  !!(bytes[0] & 2)
      }
      if(decoded.gps){
        decoded.latitude = ((bytes[1]<<16)) + ((bytes[2]<<8)) + bytes[3];
        decoded.latitude = ((decoded.latitude * 200) - 900000000) / 10000000;
        decoded.longitude = ((bytes[4]<<16)) + ((bytes[5]<<8)) + bytes[6];
        decoded.longitude =((decoded.longitude * 400 ) - 1800000000) / 10000000;
        decoded.altitude = ((bytes[7]<<8)) + bytes[8];
        decoded.hdop = bytes[9]/10; 
      }
    return decoded;
    }
  }
 */

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
