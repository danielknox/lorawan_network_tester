#ifndef PROBE_SETTINGS__H_
#define PROBE_SETTINGS__H_

#include "state.h"
#include "lorawan.h"

#define MANUAL_TRANSMIT   (0)

join_type getJoinType();
spread_factor getSpreadFactor();
unsigned int getTransmitInterval();
void loadSettings();

extern state settingsState;

#endif //PROBE_SETTINGS__H_
