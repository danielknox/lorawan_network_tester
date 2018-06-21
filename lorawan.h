#include "Arduino.h"
#ifndef PROBE_LORAWAN__H_
#define PROBE_LORAWAN__H_

enum spread_factor {
  SF_7 = 7, SF_8, SF_9, SF_10, SF_11, SF_12
};

enum join_type {
  OTAA, ABP
};

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
size_t getHweui(char *buffer, size_t size);
bool loraJoinIfNeeded();

bool loraIsJoined();

transmit_responce loraTransmit(bool manual, spread_factor sf, transmit_result& result);

#endif //PROBE_LORAWAN__H_
