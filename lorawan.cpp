#include "lorawan.h"
#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"
#include "hardware.h"
#include "battery.h"
#include "gps.h"
#include <TheThingsNetwork.h>

typedef union {
  struct {
    byte triggered : 1;   // Triggered by button press
    byte locInfo   : 1;   // Contains location data
    byte spare1    : 1;   // Spare
    byte spare2    : 1;   // Spare
    byte spare3    : 1;   // Spare
    byte spare4    : 1;   // Spare
    byte spare5    : 1;   // Spare
    byte spare6    : 1;   // Spare
  }; 
  uint8_t packed; 
} PayloadContents;

bool joined = false;

TheThingsNetwork lorawan(RN2483_SERIAL, DEBUG_SERIAL, FREQ_PLAN);

/**************************************************************************/
/*!
    @brief  Check if the device has the required keys.
    @return If device is currently provisioned for the currently selected join mode (OTAA or ABP)
*/
/**************************************************************************/
bool isDeviceProvisioned(){
  if(getJoinType() == OTAA) {
    char currentAppEUI[8];
    if(lorawan.getAppEui(currentAppEUI,8)==8) return true; // We can only check if there is an appEUI, the RN2483 does not let us read keys.
  } else {
    if(lorawan.checkPersonalize()) return true;
  }
  return false;
}

/**************************************************************************/
/*!
    @brief  Provisions the RN2483 with ABP keys.
    @param  The registered appEui (do this on your LoRaWAN network server) - likely to become deprecated for joinEUI
    @param  The registered appKey (do this on your LoRaWAN network server) - 
    @return Keys were valid and sucessfully saved. False if not valid or not sucessfully saved.
*/
/**************************************************************************/
bool provisionOTAA(const char *appEui, const char *appKey){
  return lorawan.provision(appEui,appKey);
}

/**************************************************************************/
/*!
    @brief  Provisions the RN2483 with ABP keys.
    @param  The registered devAddr (do this on your LoRaWAN network server) - 4 bytes
    @param  The registered nwkSKey (do this on your LoRaWAN network server) - 16 bytes
    @param  The registered appSKey (do this on your LoRaWAN network server) - 16 bytes
    @return True if keys were valid and sucessfully saved. False if not valid or not sucessfully saved.
*/
/**************************************************************************/
bool provisionABP(const char *devAddr, const char *nwkSKey, const char *appSKey){
  return lorawan.personalize(devAddr, nwkSKey, appSKey);
}

/**************************************************************************/
/*!
    @brief  Returns the RN2483's current pwr level in DBm
*/
/**************************************************************************/
int getTransmitPower() {
  return lorawan.getPWR();
}

/**************************************************************************/
/*!
    @brief  Read back the SNR of the last recieved packet
*/
/**************************************************************************/
int getSNR() {
  return lorawan.getSNR();
}


/**************************************************************************/
/*!
    @brief  Converts a spread factor to a numerical representation
*/
/**************************************************************************/
int sfToNum(spread_factor sf) {
  return (int)sf;
//  switch(sf) {
//    case SF_7:  return 7;
//    case SF_8:  return 8;
//    case SF_9:  return 9;
//    case SF_10: return 10;
//    case SF_11: return 11;
//    case SF_12: return 12;
//    default: return 7;
//  }
}

/**************************************************************************/
/*!
    @brief  Ease of use to convert the spread factors numeric value back to its enum form.
*/
/**************************************************************************/
spread_factor numToSF(int num) {
  return num>=7 && num<=12 ? (spread_factor)num : SF_7;
}

/**************************************************************************/
/*!
    @brief  Converts a spread factor to a text representation
*/
/**************************************************************************/
char* sfToText(spread_factor sf) {
  switch(sf) {
    case SF_7:  return "SF  7";
    case SF_8:  return "SF  8";
    case SF_9:  return "SF  9";
    case SF_10: return "SF 10";
    case SF_11: return "SF 11";
    case SF_12: return "SF 12";
    default: return "Unknown";
  }
}

/**************************************************************************/
/*!
    @brief  Resets RN2483 by toggling reset pin.
*/
/**************************************************************************/
void hardwareReset(){
  // Toggle RN2483 reset
  pinMode(RN2483_RESET_PIN, OUTPUT);
  digitalWrite(RN2483_RESET_PIN, LOW);
  delay(1000);
  digitalWrite(RN2483_RESET_PIN, HIGH);
  delay(1000);
}

/**************************************************************************/
/*!
    @brief Stub externally called. Begins serial device and resets hardware
*/
/**************************************************************************/
void initLorawan() {
  RN2483_SERIAL.begin(57600);
  hardwareReset();
}


/**************************************************************************/
/*!
    @brief Get HWEUI
*/
/**************************************************************************/
size_t getHweui(char *buffer, size_t size) {
  return lorawan.getHardwareEui(buffer,size);
}
/**************************************************************************/
/*!
   @brief If join is required, lookup required type and attempt to join.
   @return Returns true if sucessfully joined (either valid ABP or successful OTAA). Returns false if failed.  
*/
/**************************************************************************/
bool loraJoinIfNeeded() {
  if(joined) return true;
  drawFullScreenIcon(joiningNetwork);
  lorawan.linkCheck(1); // We want a link check with every message sent.
  if(getJoinType() == OTAA) {
    lorawan.setSF(sfToNum(getSpreadFactor()));
    if(!lorawan.join(1)) return false; // Attempt a single join, return false if fails.
  } else {
    if(!lorawan.personalize()) return false; // Tells device to use ABP, if keys are not valid return false
  }
  joined = true;
  return true;
}

/**************************************************************************/
/*!
   @brief  Check if the network connection is available
   @return Returns true if a linkcheck suggests that the network is currently connected  
*/
/**************************************************************************/
bool loraNetworkConnection() {
  if(lorawan.getLinkCheckGateways()>0) return true;
  return false;
}

/**************************************************************************/
/*!
   @brief  Transmit the data payload. results frequency is populated if the payload was successfully transmitted. The noise and gateway count values are populated if the message is confirmed by the gateway.
   @return Returns TEST_SUCCESS if transmission was successfult and confirmed; TEST_FAIL on a successful transmit, but no confirmation or TEST_ERROR ion hardware issue and duty cycle restrictions
*/
/**************************************************************************/
transmit_responce loraTransmit(bool manual, spread_factor sf, transmit_result& result) {
  byte payload[12];
  memset(payload, 0, sizeof(payload)); // Clear buffer
  PayloadContents frameFormat;
  
  frameFormat.triggered = manual ? 1 : 0;

  // Location Info
  loc locationData;
  if(retieveGPS(&locationData)){  
      frameFormat.locInfo = 1;
      int32_t LatitudeBinary = (locationData.latitude + 900000000 )/ 200;
      int32_t LongitudeBinary = (locationData.longitude + 1800000000 )/ 400;
      
      payload[1] = (LatitudeBinary >> 16) & 0xFF;
      payload[2] = (LatitudeBinary >> 8) & 0xFF;
      payload[3] = LatitudeBinary & 0xFF;

      payload[4] = (LongitudeBinary >> 16) & 0xFF;
      payload[5] = (LongitudeBinary >> 8) & 0xFF;
      payload[6] = LongitudeBinary & 0xFF;
   
      int32_t altitudeGps = locationData.altitude / 100;    
      payload[7] = (altitudeGps >> 8) & 0xFF;
      payload[8] = altitudeGps & 0xFF;
    
      payload[9] = (locationData.hdop / 100);
      frameFormat.locInfo = 1;
  } else {
    frameFormat.locInfo = 0;
  }

  // Battery Info - Pack float into int 
  int voltage = batteryVoltage();
  payload[10] = voltage >> 8;
  payload[11] = voltage;

  // fill blanks
  frameFormat.spare1 = 0;
  frameFormat.spare2 = 0;
  frameFormat.spare3 = 0;
  frameFormat.spare4 = 0;
  frameFormat.spare5 = 0;
  frameFormat.spare6 = 0;

  payload[0] = frameFormat.packed;
  
  ttn_response_t response = lorawan.sendBytes(payload, sizeof(payload), 1, true, sfToNum(sf));
  switch(response) {
    case TTN_ERROR_SEND_COMMAND_FAILED:
      return TEST_ERROR;
    
    case TTN_ERROR_UNEXPECTED_RESPONSE:
      result.freq = lorawan.getFreq()/1000000.0f;
      return TEST_FAIL;
    
    case TTN_SUCCESSFUL_RECEIVE:
    case TTN_SUCCESSFUL_TRANSMISSION:
      result.freq = lorawan.getFreq()/1000000.0f;
      result.noise = lorawan.getLinkCheckMargin();
      result.gateways = lorawan.getLinkCheckGateways();
      return TEST_SUCCESS;
  }
}
