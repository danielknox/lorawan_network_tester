#include "Arduino.h"
#ifndef PROBE_GPS__H_
#define PROBE_GPS__H_

typedef struct {
  int32_t latitude;  // Integer degrees, scaled by 10 Sig Digits
  int32_t longitude; // Integer degrees, scaled by 10 Sig Digits
  int32_t altitude;  // Integer cms
  uint16_t hdop;     // Horizontal Dilution of Precision x 1000
} loc;

void initGPS();
bool hasGPSLock();
bool retieveGPS(loc* location);

#endif //PROBE_GPS__H_
