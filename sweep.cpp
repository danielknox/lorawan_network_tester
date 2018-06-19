#include "sweep.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"
#include "gps.h"
#include "joystick.h"
#include "menu.h"

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

  drawText(5, 1, "TX");
  drawText(5, 2, "Press To Sweep");
  drawText(5, 3, "RX Qual.");
}

bool testSF(spread_factor sf) {
  bool success;
  transmit_result res;
  char buffer[20];
  if( success = loraTransmit(sf, res)) {
    clearLine(4);
    snprintf(buffer, 20, "%ddBM", res.noise);
    drawText(5, 4, buffer);
    clearSpace(50, 1, 160);
    snprintf(buffer, 20, "%d.%dMHz", (int)res.freq, (int)(res.freq*10)%10);
    drawText(50, 1, buffer);
  }
  clearLine(2);
  drawText(5, 2, sfToText(sf));
  snprintf(buffer, 20, "%ddBM", res.power);
  drawText(80, 2, buffer);
  return success;
}

void spinSweep() {
//  if(hasGPSLock()) {
//    
//  }

  if(readJoystick() == JOY_PRESSED) {
    bool success[6];
    success[0] = testSF(SF_7);
    delay(1000);
    success[0] = testSF(SF_8);
    delay(1000);
    success[0] = testSF(SF_9);
    delay(1000);
    success[0] = testSF(SF_10);
    delay(1000);
    success[0] = testSF(SF_11);
    delay(1000);
    success[0] = testSF(SF_12);
    delay(1000);
    setState(&sweep_results);
  }
}

void initSweepResults() {
  setState(&menuState);
}

void spinSweepResults() {
}

state sweepState = {
  &initSweep,
  &spinSweep,
  NULL
};
