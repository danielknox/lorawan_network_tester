#include "settings.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "menu.h"
#include <FlashStorage.h>
#include "joystick.h"
#include "gps.h"

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

//used by both settings menu and exit/save
int settingsCursor = 1;


/**************************************************************************/
/*!
    @brief  Sets the screen up for settings exit.
*/
/**************************************************************************/
void initExitState() {
  setLineInverted(0, false);
  setLineInverted(1, true);
  setLineInverted(2, true);
  setLineInverted(3, true);
  setLineInverted(4, true);
  clearScreen();

  drawText(38, 2, "Save As");
  drawText(32, 3, "Default?");
  drawText(28+80, 4, "No");
  setLineInverted(4, false);
  clearSpace(0,4, 80);
  drawText(22, 4, "Yes");
  settingsCursor = 0;
}


/**************************************************************************/
/*!
    @brief  Checks for joystick commands and updates screen accordingly
*/
/**************************************************************************/
void loopExitState() {
  feedGPS();
  drawGPSIcon();
  drawBatteryIcon();
  switch(readJoystick()) {
    case JOY_PRESSED:
      if(!settingsCursor) //if YES
        saveSettings();
      setState(&menuState);
      break;
    case JOY_LEFT:
      if(settingsCursor) { //if NO
        setLineInverted(4, true);
        clearSpace(80, 4, 160);
        drawText(28+80, 4, "No");
        setLineInverted(4, false);
        clearSpace(0, 4, 80);
        drawText(22, 4, "Yes");
        settingsCursor = 0;
      }
      break;
    case JOY_RIGHT:
      if(!settingsCursor) { //if YES
        setLineInverted(4, false);
        clearSpace(80, 4, 160);
        drawText(28+80, 4, "No");
        setLineInverted(4, true);
        clearSpace(0, 4, 80);
        drawText(22, 4, "Yes");
        settingsCursor = 1;
      }
      break;
  }
}

state settingsExitState = {
  &initExitState,
  &loopExitState,
  NULL
};

#define POINTER_OFFSET    (4)


/**************************************************************************/
/*!
    @brief  Draws the join type value to the screen plus joystick widgets if necassary
*/
/**************************************************************************/
void showJoin() {
  clearSpace(68,1,160);
  switch(getJoinType()) {
    case OTAA:
      drawText(68+5+12+5,1,"OTAA");
      break;
    case ABP:
      drawText(68+5+12+5+6,1,"ABP");
      break;
  }
  if(settingsCursor == 1) {
    drawText(68+5, 1, "<");
    drawText(68+5+12+5+48+5, 1, ">");
  }
}

/**************************************************************************/
/*!
    @brief  Draws the spread factor value to the screen plus joystick widgets if necassary
*/
/**************************************************************************/
void showSF() {
  clearSpace(44,2,160);
  drawText(44+8+12+8, 2, sfToText(getSpreadFactor()));
  if(settingsCursor == 2) {
    if(getSpreadFactor() > SF_7)
      drawText(44+8, 2, "<");
    if(getSpreadFactor() < SF_12)
      drawText(44+8+12+8+60+8, 2, ">");
  }
}

#define SECOND    (1000)
#define MINUTE    (60*SECOND)

const unsigned int intervals[10] = {MANUAL_TRANSMIT, SECOND, 2*SECOND, 5*SECOND, 10*SECOND, 30*SECOND, MINUTE, 2*MINUTE, 5*MINUTE, 10*MINUTE};

/**************************************************************************/
/*!
    @brief  For a given time interval, returns the next interval strictly greater. If the provided interval is equal to or larger
    than the largest permitted, the maximum interval is returned.
    @param The current time interval
    @return The next larger interval or the maximum allowed interval
*/
/**************************************************************************/
unsigned int nextInterval(unsigned int curr) {
  for(int i=0; i<9; i++)
    if(intervals[i]>curr)
      return intervals[i];
  return intervals[9];
}


/**************************************************************************/
/*!
    @brief  For a given time interval, returns the next interval strictly smaller. If the provided interval is equal to or less than
    than the smallest permitted, the minimum interval is returned.
    @param The current time interval
    @return The next smaller interval or the minimum allowed interval
*/
/**************************************************************************/
unsigned int prevInterval(unsigned int curr) {
  for(int i=9; i>0; i--)
    if(intervals[i]<curr)
      return intervals[i];
  return intervals[0];
}

/**************************************************************************/
/*!
    @brief  Finds the nearest valid interval that is not larger than the provided value
    @param The current time interval
    @return The nearest valid interval
*/
/**************************************************************************/
unsigned int nearestInterval(unsigned int curr) {
  for(int i=9; i>0; i--)
    if(intervals[i]<=curr)
      return intervals[i];
  return intervals[0];
}

/**************************************************************************/
/*!
    @brief  Ease of use function to convert valid time intervals to text. Returns NULL on invalid intervals
    @param A valid time interval
    @return A text representation or NULL
*/
/**************************************************************************/
char* intervalToText(unsigned int curr) {
  switch(curr) {
    case MANUAL_TRANSMIT: return "Man";
    case SECOND: return "1s";
    case 2*SECOND: return "2s";
    case 5*SECOND: return "5s";
    case 10*SECOND: return "10s";
    case 30*SECOND: return "30s";
    case MINUTE: return "1m";
    case 2*MINUTE: return "2m";
    case 5*MINUTE: return "5m";
    case 10*MINUTE: return "10m";
    default: return NULL;
  }
}

/**************************************************************************/
/*!
    @brief  Draws the transmission interval value to the screen plus joystick widgets if necassary
*/
/**************************************************************************/
void showTxIvl() {
  clearSpace(92,3,160);
  unsigned int interval = nearestInterval(getTransmitInterval());
  drawText(92+2+12+2, 3,intervalToText(interval));
  if(settingsCursor == 3) {
    if(interval>MANUAL_TRANSMIT)
      drawText(92+2, 3, "<");
    if(interval<10*MINUTE)
      drawText(92+2+12+2+36+2, 3, ">");
  }
}

/**************************************************************************/
/*!
    @brief  Sets up the screen for the Settings UI
*/
/**************************************************************************/
void initSettingsState() {
  settingsCursor = 1;
  setLineInverted(0, false);
  setLineInverted(1, true);
  setLineInverted(2, true);
  setLineInverted(3, true);
  setLineInverted(4, true);
  clearScreen();
  drawText(30, 0, "Settings");
  drawText(20,1,"Join");
  drawText(20,2,"SF");
  drawText(20,3,"Tx IVL");
  drawText(20,4,"Exit");
  drawText(POINTER_OFFSET,settingsCursor,">");

  showJoin();
  showSF();
  showTxIvl();
}


/**************************************************************************/
/*!
    @brief  Ease of use function to redraw the value portion of a particular line.
*/
/**************************************************************************/
void redrawSettings(int line) {
  switch(line) {
    case 1:
      showJoin();
      break;
    case 2:
      showSF();
      break;
    case 3:
      showTxIvl();
      break;
  }
}

/**************************************************************************/
/*!
    @brief  Moves the curor to a new location
*/
/**************************************************************************/
void moveCursorSettings(int newLine) {
  clearSpace(0,settingsCursor,20);
  int oldLine = settingsCursor;
  settingsCursor = newLine;
  drawText(POINTER_OFFSET,settingsCursor,">");
  redrawSettings(oldLine);
  redrawSettings(newLine);
}

/**************************************************************************/
/*!
    @brief  Checks the joystick for actions and updates the screen and settings accordingly
*/
/**************************************************************************/
void settingsStateLoop() {
  feedGPS();
  drawGPSIcon();
  drawBatteryIcon();
  joyState joy = readJoystick();
  switch(joy) {
    case JOY_PRESSED:
      if(settingsCursor == 4) {
        setState(&settingsExitState);
        return;
      }
      break;
    case JOY_UP:
      if(settingsCursor>1)
        moveCursorSettings(settingsCursor-1);
      break;
    case JOY_DOWN:
      if(settingsCursor<4)
        moveCursorSettings(settingsCursor+1);
      break;
    case JOY_LEFT:
      switch(settingsCursor) {
        case 1:
          deviceSettings.joinType = 
            (deviceSettings.joinType == OTAA)
            ? ABP
            : OTAA;
          break;
        case 2:
          if(deviceSettings.sf>SF_7)
            (*(int*)(&deviceSettings.sf))--;
          break;
        case 3:
          deviceSettings.transmitInterval = prevInterval(deviceSettings.transmitInterval);
          break;
      }
      redrawSettings(settingsCursor);
      break;
    case JOY_RIGHT:
      switch(settingsCursor) {
        case 1:
          deviceSettings.joinType = 
            (deviceSettings.joinType == OTAA)
            ? ABP
            : OTAA;
          break;
        case 2:
          if(deviceSettings.sf<SF_12)
            (*(int*)(&deviceSettings.sf))++;
          break;
        case 3:
          deviceSettings.transmitInterval = nextInterval(deviceSettings.transmitInterval);
          break;
      }
      redrawSettings(settingsCursor);
      break;
  }
}

state settingsState = { 
  &initSettingsState,
  &settingsStateLoop,
  NULL
};
