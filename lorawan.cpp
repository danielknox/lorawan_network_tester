#include "lorawan.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"
#include "hardware.h"
#include <TheThingsNetwork.h>

TheThingsNetwork lorawan(RN2483_SERIAL, DEBUG_SERIAL, FREQ_PLAN);

int getTransmitPower() {
  return lorawan.getPWR();
}

int sfToNum(spread_factor sf) {
  switch(sf) {
    case SF_7:  return 7;
    case SF_8:  return 8;
    case SF_9:  return 9;
    case SF_10: return 10;
    case SF_11: return 11;
    case SF_12: return 12;
    default: return 7;
  }
}

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
  lorawan.linkCheck(1); // We want a link check with every message sent.
  drawFullScreenIcon(joiningNetwork);
  if(getJoinType() == OTAA) {
    lorawan.setSF(sfToNum(getSpreadFactor()));
    if(!lorawan.join(1)) return false; // Attempt a single join, return false if fails.
  } else {
    if(!lorawan.personalize()) return false; // Tells device to use ABP, if keys are not valid return false
  }
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
bool loraTransmit(spread_factor sf, transmit_result& result) {
  byte payload[1];
  ttn_response_t response = lorawan.sendBytes(payload, sizeof(payload), 1, true, sfToNum(sf));
  if(response == TTN_ERROR_SEND_COMMAND_FAILED || response == TTN_ERROR_UNEXPECTED_RESPONSE){
    return false;
  }
  else if(response == TTN_SUCCESSFUL_RECEIVE){
    result.noise = lorawan.getLinkCheckMargin();
    result.freq = lorawan.getFreq()/1000000;
  }
  return true;
}
