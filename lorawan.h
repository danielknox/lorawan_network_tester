#ifndef PROBE_LORAWAN__H_
#define PROBE_LORAWAN__H_

enum spread_factor {
  SF_7, SF_8, SF_9, SF_10, SF_11, SF_12
};

enum join_type {
  OTAA, ABP
};

typedef struct transmit_result {
  int noise;
  float freq;
};

int getTransmitPower();

int sfToNum();
char* sfToText(spread_factor sf);

void hardwareReset();
void initLorawan();

bool loraJoinIfNeeded();

bool loraIsJoined();

bool loraTransmit(spread_factor sf, transmit_result& result);

#endif //PROBE_LORAWAN__H_
