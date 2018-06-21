#include "survey.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"
#include "joystick.h"
#include "menu.h"

long nextPing;

/**************************************************************************/
/*!
    @brief  Sets up the screen for survey mode
*/
/**************************************************************************/
void initSurvey() {
  if(!loraJoinIfNeeded()) {
    setState(&menuState);
    return;
  }
  
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
  
  if(getTransmitInterval())
    nextPing = millis() + 1000;
  else
    drawText(2, 3, "Press to Scan");
}

/**************************************************************************/
/*!
    @brief  Sends a single lorawan transmission. Updates the screen with the results.
    @param Whether the ping request was made by the user or by the automatic interval
*/
/**************************************************************************/
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
      snprintf(buffer, 20, "GW %d", res.gateways);
      drawText(95, 4, buffer);
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

/**************************************************************************/
/*!
    @brief  Checks for the timer interval in automatic mode of joystick press in manual mode. Also updates the gps + battery icon.
*/
/**************************************************************************/
void spinSurvey() {
  drawGPSIcon();
  drawBatteryIcon();
  if(getTransmitInterval()) {//if auto mode
    if(((long)millis()-nextPing)>=0) {
      doPing(false);
      nextPing = millis() + getTransmitInterval();
    }
    if(readJoystick() == JOY_LEFT)
      setState(&menuState);
  } else { //if manual mode
    switch(readJoystick()) {
      case JOY_PRESSED:
        doPing(true);
        break;
      case JOY_LEFT:
        setState(&menuState);
        break;
    }
      
  }
}

state surveyState = {
  &initSurvey,
  &spinSurvey,
  NULL
};
