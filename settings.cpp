#include "settings.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "menu.h"

join_type getJoinType() {
  return OTAA;
}

spread_factor getSpreadFactor() {
  return SF_7;
}

unsigned int getTransmitInterval() {
  return MANUAL_TRANSMIT;
}

void initSettingsState() {
  //init
}

void settingsStateLoop() {
  //check for joystick
  setState(&menuState);
}

void exitSettings() {
  //save
}

state settingsState = { 
  &initSettingsState,
  &settingsStateLoop,
  &exitSettings
};
