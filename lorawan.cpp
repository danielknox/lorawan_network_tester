#include "lorawan.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"

bool joined = false;

int getTransmitPower() {
  return 14;
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

void init_lorawan() {
  // Toggle RN2483 reset
  pinMode(LORA_RESET_PIN, OUTPUT);
  digitalWrite(LORA_RESET_PIN, LOW);
  delay(1000);
  digitalWrite(LORA_RESET_PIN, HIGH);
  delay(1000);
}

bool loraJoinIfNeeded() {
  if(getJoinType() == OTAA) {
    drawFullScreenIcon(joiningNetwork);
    lorawan.join();
  } else {
    if(!lorawan.personalize()) return false;
  }
  return true;
}

bool loraIsJoined() {
  if(lorawan.getLinkCheckGateways()>0) return true;
  return false;
}

bool loraTransmit(spread_factor sf, transmit_result& result) {
  //placeholder
  delay(2500);
  result.noise = 8 + sf;
  result.freq = 868.1;
  return sf >= SF_9;
}
