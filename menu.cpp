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
  drawText(POINTER_OFFSET,cursor,">");
}

void moveCursor(int newLine) {
  clearSpace(0,cursor,20);
  cursor = newLine;
  drawText(POINTER_OFFSET,cursor,">");
}

void menuStateLoop() {
  joyState joy = readJoystick();
  switch(joy) {
    case JOY_PRESSED:
      switch(cursor) {
        case 1:
          //setState(&surveyState);
          Serial.println("survey");
          return;
        case 2:
          setState(&sweepState);
          return;
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
