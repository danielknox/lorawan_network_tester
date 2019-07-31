#include "survey.h"

#include <Arduino.h>
#include "icons.h"
#include "screen.h"
#include "settings.h"
#include "joystick.h"
#include "menu.h"
#define TX_INFO_LOOP_SECONDS     (4)

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

  drawText(5, 3, "RX Qual.");
  
  if(getTransmitInterval())
    nextPing = millis() + 1000;
  else
    drawText(2, 4, "Press to Scan");
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
  char buffer[20];
  boolean gwINFO = false;
  unsigned long starttime;
  unsigned long endtime;
  switch(loraTransmit(manual, getSpreadFactor(), res)) {
    case TEST_SUCCESS:
      clearSpace(25, 0, 25);
      clearSpace(50, 1, 160);
      snprintf(buffer, 20, "%3.1fMHz", res.freq);
      drawText(64, 1, buffer);
      clearLine(4);
      snprintf(buffer, 20, "%ddBM", getSNR());
      drawText(5, 4, buffer);
      // Alternates drawing of tx linkmargin, no gateways and tx sf, dbm
      for(int i=0; i<TX_INFO_LOOP_SECONDS; i++) {
          clearLine(2);
          if(gwINFO){
            snprintf(buffer, 20, "%ddBM", res.noise);
            drawText(5, 2, buffer);
            snprintf(buffer, 20, "GW %d", res.gateways);
            drawText(95, 2, buffer);
            gwINFO = false;
          }
          else {
            drawText(5, 2, sfToText(getSpreadFactor()));
            snprintf(buffer, 20, "%ddBM", getTransmitPower());
            drawText(97, 2, buffer);
            gwINFO = true;
          }
          starttime = millis();
          endtime = starttime;
          while ((endtime - starttime) <=1000) // do this loop for up to 1000mS
          {
            if(readJoystick()==JOY_PRESSED){
              doPing(true);
              return;
            }
            endtime = millis();
          }
      }
      break;

    case TEST_FAIL:
      clearLine(2);
      drawText(5, 2, sfToText(getSpreadFactor()));
      snprintf(buffer, 20, "%ddBM", getTransmitPower());
      drawText(97, 2, buffer);
      clearSpace(25, 0, 25);
      drawText(5, 4, "Fail");
      break;

    case TEST_ERROR:
      clearLine(2);
      drawText(5, 2, sfToText(getSpreadFactor()));
      snprintf(buffer, 20, "%ddBM", getTransmitPower());
      drawText(97, 2, buffer);
      clearSpace(25, 0, 25);
      drawText(5, 4, "No Free Chan");
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
