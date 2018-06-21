#include "menu.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "joystick.h"
#include "usb.h"
#include "sweep.h"
#include "survey.h"
#include "settings.h"

#define POINTER_OFFSET    (4)

int cursor;

/**************************************************************************/
/*!
   @brief  Ease of use function for moving the curso on screen
*/
/**************************************************************************/
void moveCursor(int newLine) {
  clearSpace(0,cursor,20);
  cursor = newLine;
  if((cursor == 1 || cursor == 2) && !isDeviceProvisioned()) {
    drawText(POINTER_OFFSET,cursor,"X");
  } else {
    drawText(POINTER_OFFSET,cursor,">");
  }
}

/**************************************************************************/
/*!
   @brief  Sets up the screen for the main menu
*/
/**************************************************************************/
void initMenuState() {
  cursor = 1;
  setLineInverted(0, false);
  setLineInverted(1, true);
  setLineInverted(2, true);
  setLineInverted(3, true);
  setLineInverted(4, true);
  clearScreen();
  drawText(55,0,"Menu");
  drawText(20,1,"Survey");
  drawText(20,2,"Sweep");
  drawText(20,3,"Settings");
  drawText(20,4,"USB");
  
  cursor = 0;
  moveCursor(1);
}

/**************************************************************************/
/*!
   @brief  Checks the yostick for inputs and updates the screen or changes state accordingly
*/
/**************************************************************************/
void menuStateLoop() {
  joyState joy = readJoystick();
  switch(joy) {
    case JOY_PRESSED:
      switch(cursor) {
        case 1:
          if(isDeviceProvisioned()) {
            setState(&surveyState);
            return;
          }
          break;
        case 2:
          if(isDeviceProvisioned()) {
            setState(&sweepState);
            return;
          }
          break;
        case 3:
          setState(&settingsState);
          return;
        case 4:
          setState(&usbState);
          return;
      }
      break;
    case JOY_UP:
      if(cursor>1)
        moveCursor(cursor-1);
      break;
    case JOY_DOWN:
      if(cursor<4)
        moveCursor(cursor+1);
      break;
  }
}

state menuState = {
  &initMenuState,
  &menuStateLoop,
  NULL
};
