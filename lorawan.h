#include "Arduino.h"
#ifndef PROBE_LORAWAN__H_
#define PROBE_LORAWAN__H_

enum spread_factor {
  SF_7 = 7, SF_8, SF_9, SF_10, SF_11, SF_12
};

enum join_type {
  OTAA, ABP
};

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

enum transmit_responce {
  TEST_SUCCESS, TEST_FAIL, TEST_ERROR
};

typedef struct transmit_result {
  int noise;
  float freq;
  int gateways;
};

bool isDeviceProvisioned();
bool provisionOTAA(const char *appEui, const char *appKey);
bool provisionABP(const char *devAddr, const char *nwkSKey, const char *appSKey);

int getTransmitPower();

int sfToNum();
char* sfToText(spread_factor sf);

void hardwareReset();
void initLorawan();

bool loraJoinIfNeeded();

bool loraIsJoined();

transmit_responce loraTransmit(spread_factor sf, transmit_result& result);

#endif //PROBE_LORAWAN__H_
