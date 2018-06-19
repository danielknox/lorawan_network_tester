#include "sweep.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"
#include "gps.h"
#include "joystick.h"
#include "menu.h"

bool sfSuccess[6];

void initSweepResults();
void spinSweepResults();
state sweep_results = {
  &initSweepResults,
  &spinSweepResults,
  NULL
};

void initSweep() {
  loraJoinIfNeeded();
  
  setLineInverted(0, true);
  setLineInverted(1, false);
  setLineInverted(2, true);
  setLineInverted(3, false);
  setLineInverted(4, true);

  clearScreen();

  drawText(32, 1, "Press To");
  drawText(50, 2, "Sweep");
}

bool testSF(spread_factor sf) {
  bool success;
  transmit_result res;
  char buffer[20];
  clearLine(2);
  drawText(5, 2, sfToText(sf));
  snprintf(buffer, 20, "%ddBM", getTransmitPower());
  drawText(97, 2, buffer);
  drawSmallIcon(25, 0, 24, 24, transmitting);
  clearLine(4);
  if( success = loraTransmit(sf, res)) {
    clearSpace(25, 0, 25);
    clearSpace(50, 1, 160);
    snprintf(buffer, 20, "%3d.%dMHz", (int)res.freq, (int)(res.freq*10)%10);
    drawText(64, 1, buffer);
    snprintf(buffer, 20, "%ddBM", res.noise);
    drawText(5, 4, buffer);
  } else {
    clearSpace(25, 0, 25);
    drawText(5, 4, "Fail");
  }
  return success;
}

void updateGPSIcon() {
  static bool wasLock = false;
  static bool wasNoLock = false;
  if(hasGPSLock()) {
    if(!wasLock) {
      drawSmallIcon(0, 0, 24, 24, gpsConnected);
      wasNoLock = false;
      wasLock = true;
    }
  } else if(!wasNoLock) {
    drawSmallIcon(0, 0, 24, 24, gpsDisconnected);
    wasNoLock = true;
    wasLock = false;
  }
}

void spinSweep() {
  updateGPSIcon();
  if(readJoystick() == JOY_PRESSED) {
    clearLine(1);
    drawText(5, 1, "TX");
    clearLine(2);
    clearLine(3);
    drawText(5, 3, "RX Qual.");
    sfSuccess[0] = testSF(SF_7);
    updateGPSIcon();
    delay(1000);
    sfSuccess[1] = testSF(SF_8);
    updateGPSIcon();
    delay(1000);
    sfSuccess[2] = testSF(SF_9);
    updateGPSIcon();
    delay(1000);
    sfSuccess[3] = testSF(SF_10);
    updateGPSIcon();
    delay(1000);
    sfSuccess[4] = testSF(SF_11);
    updateGPSIcon();
    delay(1000);
    sfSuccess[5] = testSF(SF_12);
    updateGPSIcon();
    delay(1000);
    setState(&sweep_results);
  }
}

char* sfIndexToText(int index) {
  switch(index) {
    case 0:  return "SF 7";
    case 1:  return "SF 8";
    case 2:  return "SF 9";
    case 3: return "SF10";
    case 4: return "SF11";
    case 5: return "SF12";
    default: return "Unknown";
  }
}

void initSweepResults() {
  setLineInverted(0, true);
  setLineInverted(1, false);
  setLineInverted(2, false);
  setLineInverted(3, false);
  setLineInverted(4, false);
  clearScreen();

  for(int i=0; i<3; i++) {
    setLineInverted(2, sfSuccess[i]);
    clearSpace(i*53, 2, 54);
    drawText(i*53+2, 2, sfIndexToText(i));
    
    setLineInverted(3, sfSuccess[i+3]);
    clearSpace(i*53, 3, 54);
    drawText(i*53+2, 3, sfIndexToText(i+3));
  }

  drawText(2, 1, "SF Avail.");
  drawText(2, 4, "<Back");
  drawText(99, 4, "Menu>");
}

void spinSweepResults() {
  updateGPSIcon();
  switch(readJoystick()) {
    case JOY_LEFT:
      setState(&sweepState);
      return;
    case JOY_RIGHT:
      setState(&menuState);
      return;
  }
}

state sweepState = {
  &initSweep,
  &spinSweep,
  NULL
};
