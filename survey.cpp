#include "survey.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"
#include "joystick.h"

long nextPing;

void initSurvey() {
  loraJoinIfNeeded();
  
  setLineInverted(0, true);
  setLineInverted(1, false);
  setLineInverted(2, true);
  setLineInverted(3, false);
  setLineInverted(4, true);

  clearScreen();
  
  char buffer[20];
  drawText(5, 1, "TX");
  drawText(5, 2, sfToText(getSpreadFactor()));
  snprintf(buffer, 20, "%ddBM", getTransmitPower());
  drawText(97, 2, buffer);
  drawText(5, 3, "RX Qual.");

  nextPing = millis() + getTransmitInterval();
}

void doPing(boolean manual) {
  drawSmallIcon(25, 0, 24, 24, transmitting);
  transmit_result res;
  clearLine(4);
  switch(loraTransmit(manual, getSpreadFactor(), res)) {
    case TEST_SUCCESS:
      char buffer[20];
      clearSpace(25, 0, 25);
      clearSpace(50, 1, 160);
      snprintf(buffer, 20, "%3d.%dMHz", (int)res.freq, (int)(res.freq*10)%10);
      drawText(64, 1, buffer);
      snprintf(buffer, 20, "%ddBM", res.noise);
      drawText(5, 4, buffer);
      break;

    case TEST_FAIL:
      clearSpace(25, 0, 25);
      drawText(5, 4, "Fail");
      break;

    case TEST_ERROR:
      clearSpace(25, 0, 25);
      drawText(5, 4, "Error");
      break;
  }
}

void spinSurvey() {
  drawGPSIcon();
  drawBatteryIcon();
  if(getTransmitInterval()) {//if auto mode
    if(((long)millis()-nextPing)>=0) {
      doPing(false);
      nextPing = millis() + getTransmitInterval();
    }
  } else { //if manual mode
    if(readJoystick() == JOY_PRESSED)
      doPing(true);
  }
}

state surveyState = {
  &initSurvey,
  &spinSurvey,
  NULL
};
