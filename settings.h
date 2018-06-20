#ifndef PROBE_SETTINGS__H_
#define PROBE_SETTINGS__H_

#include "state.h"
#include "lorawan.h"

#define MANUAL_TRANSMIT   (0)

void setJoinType(join_type chosenJoinType);
void setSpreadFactor(spread_factor chosenSpreadFactor);
void setTransmitInterval(unsigned int chosenTransmitInterval);
void loadSettings();
void saveSettings();
join_type getJoinType();
spread_factor getSpreadFactor();
unsigned int getTransmitInterval();

extern state settingsState;

#endif //PROBE_SETTINGS__H_
