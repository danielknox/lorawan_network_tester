#include "settings.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "menu.h"
#include <FlashStorage.h>

typedef struct {
  boolean valid;
  unsigned int transmitInterval;
  join_type joinType;
  spread_factor sf;
} Settings;

FlashStorage(flash_store, Settings);
Settings deviceSettings;

/**************************************************************************/
/*!
    @brief  Gets the current join type
    @return  The current join type
*/
/**************************************************************************/
join_type getJoinType() {
  return deviceSettings.joinType;
}

/**************************************************************************/
/*!
    @brief  Gets the current spread factor
    @return  The current spread factor
*/
/**************************************************************************/
spread_factor getSpreadFactor() {
  return deviceSettings.sf;
}

/**************************************************************************/
/*!
    @brief  Gets the current transmit interval
    @return  The current transmit interval
*/
/**************************************************************************/
unsigned int getTransmitInterval() {
  return deviceSettings.transmitInterval;
}

/**************************************************************************/
/*!
    @brief  Sets the internal settings structure join type
    @param  The chosen join type
*/
/**************************************************************************/
void setJoinType(join_type chosenJoinType) {
  deviceSettings.joinType = chosenJoinType;
}

/**************************************************************************/
/*!
    @brief  Sets the internal settings structure transmit interval
    @param  The chosen transmit interval
*/
/**************************************************************************/
void setSpreadFactor(spread_factor chosenSpreadFactor) {
  deviceSettings.sf = chosenSpreadFactor;
}

/**************************************************************************/
/*!
    @brief  Sets the internal settings structure transmit interval
    @param  The chosen transmit interval
*/
/**************************************************************************/
void setTransmitInterval(unsigned int chosenTransmitInterval) {
  deviceSettings.transmitInterval = chosenTransmitInterval;
}

void initSettingsState() {
  //init
}

/**************************************************************************/
/*!
    @brief  Loads the settings from internal flash. If not set, default populate
*/
/**************************************************************************/
void loadSettings(){
   deviceSettings = flash_store.read();
   // Not populated yet, so load default values
   if(!deviceSettings.valid){
    setJoinType(OTAA);
    setSpreadFactor(SF_7);
    setTransmitInterval(MANUAL_TRANSMIT);
   }
}

/**************************************************************************/
/*!
    @brief  Saves the settings to internal flash
*/
/**************************************************************************/
void saveSettings(){
  deviceSettings.valid = true;
  flash_store.write(deviceSettings);
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
