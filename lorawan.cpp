#include "lorawan.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"
#include "hardware.h"
#include <TheThingsNetwork.h>

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
    if(lorawan.personalize()) return true;
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
   @brief  Transmit the data payload
   @return Returns true if successful transmission (not necessaryily confirmed), else return false as their was a failure (perhaps duty cycle restriction?)
*/
/**************************************************************************/
transmit_responce loraTransmit(spread_factor sf, transmit_result& result) {
  byte payload[1];
  ttn_response_t response = lorawan.sendBytes(payload, sizeof(payload), 1, true, sfToNum(sf));
  switch(response) {
    case TTN_ERROR_SEND_COMMAND_FAILED:
      return TEST_ERROR;
    
    case TTN_ERROR_UNEXPECTED_RESPONSE:
      result.freq = lorawan.getFreq()/1000000;
      return TEST_FAIL;
    
    case TTN_SUCCESSFUL_RECEIVE:
    case TTN_SUCCESSFUL_TRANSMISSION:
      result.freq = lorawan.getFreq()/1000000;
      result.noise = lorawan.getLinkCheckMargin();
      result.gateways = lorawan.getLinkCheckGateways();
      return TEST_SUCCESS;
  }
}
