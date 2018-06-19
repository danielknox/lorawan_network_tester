#include "lorawan.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"

bool joined = false;

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

void loraJoinIfNeeded() {
  if(joined) return;

//  if(getJoinType() == OTAA) {
//    drawFullScreenIcon(usbConnector);
//    delay(2500);
//  }

  joined = true;
}

bool loraIsJoined() {
  return joined;
}

bool loraTransmit(spread_factor sf, transmit_result& result) {
  //placeholder
  delay(2500);
  result.noise = 8 + sf;
  result.freq = 868.1;
  result.power = 14;
  return sf >= SF_9;
}
